#include "iglo.h"
#include "ShaderCommon.h"
#include "Universe.h"
#include <array>

void Universe::Unload()
{
	isLoaded = false;
	context = nullptr;
	cmd.Unload();

	computeMassParticles.Unload();
	computeVisualParticles.Unload();

	physics = PhysicsParams();
	frame = 0;
	gpu_nextIndex = 0;
	useComputeShaders = true;
	isPaused = false;

	state = ParticleState();

	gpu_mp0.Unload();
	gpu_mp1.Unload();
	gpu_vp0.Unload();
	gpu_vp1.Unload();
	gpu_mpMass.Unload();

}

bool Universe::Load(ig::IGLOContext& context)
{
	Unload();

	cmd.Load(context, ig::CommandListType::Graphics);

	ig::ReadFileResult f_CS_MP = ig::ReadFile(ShaderFolder + "CS_MassParticles.cso");
	ig::ReadFileResult f_CS_VP = ig::ReadFile(ShaderFolder + "CS_VisualParticles.cso");

	if (!f_CS_MP.success || !f_CS_VP.success)
	{
		ig::PopupMessage("Failed to load one or more shaders from '" + ShaderFolder + "'", "Error", &context);
		return false;
	}

	computeMassParticles.LoadAsCompute(context, ig::Shader(f_CS_MP.fileContent));
	computeVisualParticles.LoadAsCompute(context, ig::Shader(f_CS_VP.fileContent));

	this->context = &context;
	this->isLoaded = true;
	return true;
}

void Universe::Update(bool leftMouseButtonDown, bool rightMouseButtonDown)
{
	if (state.mpMass.size() != state.mp.size()) throw std::exception();

	physics.cursorGravityField = 0;
	if (physics.cursorInteractionEnabled)
	{
		if (leftMouseButtonDown)
		{
			physics.cursorGravityField = 1;
			physics.cursorPos = ig::Vector2((float)context->GetMouseX(), (float)context->GetMouseY());
		}
		if (rightMouseButtonDown)
		{
			physics.cursorGravityField = -1;
			physics.cursorPos = ig::Vector2((float)context->GetMouseX(), (float)context->GetMouseY());
		}
	}

	if (!isPaused)
	{
		if (useComputeShaders)
		{
			PhysicsGPU();
		}
		else
		{
			PhysicsCPU();
		}

		IncrementBuffers();

		frame++;
	}
}

void Universe::IncrementBuffers()
{
	gpu_nextIndex = (gpu_nextIndex + 1) % 2;
}

void Universe::SetParticleState(ParticleState&& newState)
{
	state = std::move(newState);

	OnParticleStateChanged();
}

void Universe::ClearAllParticles()
{
	state = ParticleState();

	OnParticleStateChanged();
}

void Universe::OnParticleStateChanged()
{
	if (!context) throw std::exception();

	frame = 0;

	context->WaitForIdleDevice();

	uint32_t safeNumMP = std::max(GetNumMP(), 1U);
	uint32_t safeNumVP = std::max(GetNumVP(), 1U);

	gpu_mp0.LoadAsStructuredBuffer(*context, sizeof(Particle), safeNumMP, ig::BufferUsage::UnorderedAccess);
	gpu_mp1.LoadAsStructuredBuffer(*context, sizeof(Particle), safeNumMP, ig::BufferUsage::UnorderedAccess);
	gpu_mpMass.LoadAsStructuredBuffer(*context, sizeof(float), safeNumMP, ig::BufferUsage::Default);
	gpu_vp0.LoadAsStructuredBuffer(*context, sizeof(Particle), safeNumVP, ig::BufferUsage::UnorderedAccess);
	gpu_vp1.LoadAsStructuredBuffer(*context, sizeof(Particle), safeNumVP, ig::BufferUsage::UnorderedAccess);

	gpu_nextIndex = 0;

	cmd.Begin();
	{
		if (GetNumMP() > 0)
		{
			WriteMP().SetData(cmd, state.mp.data());
			gpu_mpMass.SetData(cmd, state.mpMass.data());
		}
		if (GetNumVP() > 0)
		{
			WriteVP().SetData(cmd, state.vp.data());
		}
	}
	cmd.End();

	context->Submit(cmd);
	context->WaitForIdleDevice();

	IncrementBuffers();
}

void Universe::EnableComputeShaders(bool enable)
{
	if (useComputeShaders == enable) return;
	useComputeShaders = enable;

	// Switch to using CPU.
	if (!enable)
	{
		// Read the latest particle state
		if (GetNumMP() > 0) ReadStructuredBuffer(*context, cmd, ReadMP(), sizeof(Particle), GetNumMP(), state.mp.data());
		if (GetNumVP() > 0) ReadStructuredBuffer(*context, cmd, ReadVP(), sizeof(Particle), GetNumVP(), state.vp.data());
	}
}

void Universe::SetPhysicsParams(const PhysicsParams& newParams)
{
	this->physics = newParams;
}

void Universe::PhysicsGPU()
{
	float maxVelSquared = physics.maxVelocity * physics.maxVelocity;

	struct PushConstants
	{
		// Dispatch params
		uint32_t dispatchX = 0;

		// Resources
		uint32_t mpOldIndex = IGLO_UINT32_MAX;
		uint32_t vpOldIndex = IGLO_UINT32_MAX;
		uint32_t mpNewIndex = IGLO_UINT32_MAX;
		uint32_t vpNewIndex = IGLO_UINT32_MAX;
		uint32_t mpMassIndex = IGLO_UINT32_MAX;

		// Number of particles
		uint32_t numMP = 0;
		uint32_t numVP = 0;

		PhysicsParams physics;
	};

	PushConstants pushConstants;
	pushConstants.mpOldIndex = ReadMP().GetDescriptor()->heapIndex;
	pushConstants.vpOldIndex = ReadVP().GetDescriptor()->heapIndex;
	pushConstants.mpNewIndex = WriteMP().GetUnorderedAccessDescriptor()->heapIndex;
	pushConstants.vpNewIndex = WriteVP().GetUnorderedAccessDescriptor()->heapIndex;
	pushConstants.mpMassIndex = gpu_mpMass.GetDescriptor()->heapIndex;
	pushConstants.numMP = (uint32_t)state.mp.size();
	pushConstants.numVP = (uint32_t)state.vp.size();
	pushConstants.physics = physics;

	cmd.Begin();
	{

		uint32_t threadsPerGroup = ComputeBlockSizeX * ComputeBlockSizeY;
		if (GetNumMP() > 0)
		{
			uint32_t totalThreadGroups = (GetNumMP() + (threadsPerGroup - 1)) / threadsPerGroup;
			uint32_t gridSize = (uint32_t)ceilf(sqrtf((float)totalThreadGroups));

			cmd.SetPipeline(computeMassParticles);
			pushConstants.dispatchX = gridSize;
			cmd.SetComputePushConstants(&pushConstants, sizeof(pushConstants));
			cmd.DispatchCompute(gridSize, gridSize, 1);
		}
		if (GetNumVP() > 0)
		{
			uint32_t totalThreadGroups = (GetNumVP() + (threadsPerGroup - 1)) / threadsPerGroup;
			uint32_t gridSize = (uint32_t)ceilf(sqrtf((float)totalThreadGroups));

			cmd.SetPipeline(computeVisualParticles);
			pushConstants.dispatchX = gridSize;
			cmd.SetComputePushConstants(&pushConstants, sizeof(pushConstants));
			cmd.DispatchCompute(gridSize, gridSize, 1);
		}

	}
	cmd.End();
	context->Submit(cmd);

}

void Universe::PhysicsCPU()
{
	float maxVelSquared = physics.maxVelocity * physics.maxVelocity;

	// Mass particles
	{
		std::vector<ig::Vector2> acc;
		acc.resize(state.mp.size());
		for (uint64_t a = 0; a < state.mp.size(); a++)
		{
			// Acceleration between mass particles
			for (uint64_t b = a + 1; b < state.mp.size(); b++)
			{
				CalcAccForBoth(acc[a], acc[b], state.mp[a], state.mp[b], state.mpMass[a], state.mpMass[b],
					physics.attractionSoftening, physics.G, physics.spaceScale);
			}

			// Acceleration from cursor
			if (physics.cursorGravityField != 0)
			{
				Particle cursorParticle = Particle(ig::Vector2((float)context->GetMouseX(), (float)context->GetMouseY()));
				acc[a] += CalcAcc(state.mp[a], cursorParticle, physics.cursorMass * (float)physics.cursorGravityField,
					physics.attractionSoftening, physics.G, physics.spaceScale);
			}
		}
		for (uint64_t i = 0; i < state.mp.size(); i++)
		{
			// Movement
			state.mp[i].vel += (acc[i] * physics.timeScale);
			CapMagnitude(state.mp[i].vel, maxVelSquared, physics.maxVelocity);
			state.mp[i].pos += (state.mp[i].vel * physics.timeScale);
		}
	}

	// Visual particles
	{
		for (uint64_t v = 0; v < state.vp.size(); v++)
		{
			// Acceleration from mass particles
			ig::Vector2 acc;
			for (uint64_t m = 0; m < state.mp.size(); m++)
			{
				acc += CalcAcc(state.vp[v], state.mp[m], state.mpMass[m], physics.attractionSoftening, physics.G, physics.spaceScale);
			}

			// Acceleration from cursor
			if (physics.cursorGravityField != 0)
			{
				Particle cursorParticle = Particle(ig::Vector2((float)context->GetMouseX(), (float)context->GetMouseY()));
				acc += CalcAcc(state.vp[v], cursorParticle, physics.cursorMass * (float)physics.cursorGravityField,
					physics.attractionSoftening, physics.G, physics.spaceScale);
			}

			// Movement
			state.vp[v].vel += (acc * physics.timeScale);
			CapMagnitude(state.vp[v].vel, maxVelSquared, physics.maxVelocity);
			state.vp[v].pos += (state.vp[v].vel * physics.timeScale);
		}
	}

	cmd.Begin();
	{
		cmd.AddBufferBarrier(WriteMP(),
			ig::BarrierSync::ComputeShading, ig::BarrierSync::Copy,
			ig::BarrierAccess::UnorderedAccess, ig::BarrierAccess::CopyDest);
		cmd.AddBufferBarrier(WriteVP(),
			ig::BarrierSync::ComputeShading, ig::BarrierSync::Copy,
			ig::BarrierAccess::UnorderedAccess, ig::BarrierAccess::CopyDest);
		cmd.FlushBarriers();

		// Upload result to GPU so the particles can be rendered later
		if (GetNumMP() > 0) WriteMP().SetData(cmd, state.mp.data());
		if (GetNumVP() > 0) WriteVP().SetData(cmd, state.vp.data());

		cmd.AddBufferBarrier(WriteMP(),
			ig::BarrierSync::Copy, ig::BarrierSync::ComputeShading,
			ig::BarrierAccess::CopyDest, ig::BarrierAccess::UnorderedAccess);
		cmd.AddBufferBarrier(WriteVP(),
			ig::BarrierSync::Copy, ig::BarrierSync::ComputeShading,
			ig::BarrierAccess::CopyDest, ig::BarrierAccess::UnorderedAccess);
		cmd.FlushBarriers();
	}
	cmd.End();
	context->Submit(cmd);

}

void Universe::ReadStructuredBuffer(ig::IGLOContext& context, ig::CommandList& cmd, const ig::Buffer& srcBuffer,
	uint32_t elementStride, uint32_t numElements, void* out_writeDest)
{
	if (numElements == 0) return;

	ig::Buffer readable;
	readable.LoadAsStructuredBuffer(context, elementStride, numElements, ig::BufferUsage::Readable);

	context.WaitForIdleDevice();

	cmd.Begin();
	{
		cmd.CopyBuffer(srcBuffer, readable);
	}
	cmd.End();

	context.Submit(cmd);
	context.WaitForIdleDevice();

	readable.ReadData(out_writeDest);
}

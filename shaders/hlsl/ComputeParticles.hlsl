#include "Common.hlsl"

cbuffer PushConstants : register(b0)
{
	// Dispatch params
	uint dispatchX;

	// Resources
	uint mpOldIndex;
	uint vpOldIndex;
	uint mpNewIndex;
	uint vpNewIndex;
	uint mpMassIndex;

	// Number of particles
	uint numMP;
	uint numVP;

	PhysicsParams physics;
}

//TODO: Use 'groupshared' and 'GroupMemoryBarrierWithGroupSync()' to increase performance.

[numthreads(ComputeBlockSizeX, ComputeBlockSizeY, 1)]
void CS_MassParticles(uint3 groupID : SV_GroupID, uint3 threadID : SV_GroupThreadID, uint3 dispatchID : SV_DispatchThreadID)
{
	StructuredBuffer<Particle> mpOld = ResourceDescriptorHeap[mpOldIndex];
	RWStructuredBuffer<Particle> mpNew = ResourceDescriptorHeap[mpNewIndex];
	StructuredBuffer<float> mpMass = ResourceDescriptorHeap[mpMassIndex];

	float maxVelocitySquared = physics.maxVelocity * physics.maxVelocity;

	uint a = (groupID.x * ComputeBlockSizeX + threadID.x) + 
		(groupID.y * ComputeBlockSizeY + threadID.y) * dispatchX * ComputeBlockSizeX;
	if (a >= numMP) return;

	float2 oldPos = mpOld[a].pos;
	float2 oldVel = mpOld[a].vel;

	float2 totalAcc = float2(0, 0);
	for (uint b = 0; b < numMP; b++)
	{
		if (b == a) continue;

		totalAcc += CalcAcc(oldPos, mpOld[b].pos, mpMass[b], physics.spaceScale, physics.G, physics.attractionSoftening);
	}
	if (physics.cursorGravityField != 0)
	{
		totalAcc += CalcAcc(oldPos, physics.cursorPos, physics.cursorMass * (float)physics.cursorGravityField,
			physics.spaceScale, physics.G, physics.attractionSoftening);
	}

	// Movement
	float2 newVel = oldVel + (totalAcc * physics.timeScale);
	CapMagnitude(newVel, maxVelocitySquared, physics.maxVelocity);
	float2 newPos = oldPos + (newVel * physics.timeScale);

	// Write results
	mpNew[a].pos = newPos;
	mpNew[a].vel = newVel;
}

[numthreads(ComputeBlockSizeX, ComputeBlockSizeY, 1)]
void CS_VisualParticles(uint3 groupID : SV_GroupID, uint3 threadID : SV_GroupThreadID, uint3 dispatchID : SV_DispatchThreadID)
{
	StructuredBuffer<Particle> mpOld = ResourceDescriptorHeap[mpOldIndex];
	StructuredBuffer<Particle> vpOld = ResourceDescriptorHeap[vpOldIndex];
	RWStructuredBuffer<Particle> vpNew = ResourceDescriptorHeap[vpNewIndex];
	StructuredBuffer<float> mpMass = ResourceDescriptorHeap[mpMassIndex];

	float maxVelocitySquared = physics.maxVelocity * physics.maxVelocity;

	uint v = (groupID.x * ComputeBlockSizeX + threadID.x) + 
		(groupID.y * ComputeBlockSizeY + threadID.y) * dispatchX * ComputeBlockSizeX;
	if (v >= numVP) return;

	float2 oldPos = vpOld[v].pos;
	float2 oldVel = vpOld[v].vel;

	float2 totalAcc = float2(0, 0);
	for (uint m = 0; m < numMP; m++)
	{
		totalAcc += CalcAcc(oldPos, mpOld[m].pos, mpMass[m], physics.spaceScale, physics.G, physics.attractionSoftening);
	}
	if (physics.cursorGravityField != 0)
	{
		totalAcc += CalcAcc(oldPos, physics.cursorPos, physics.cursorMass * (float)physics.cursorGravityField,
			physics.spaceScale, physics.G, physics.attractionSoftening);
	}

	// Movement
	float2 newVel = oldVel + (totalAcc * physics.timeScale);
	CapMagnitude(newVel, maxVelocitySquared, physics.maxVelocity);
	float2 newPos = oldPos + (newVel * physics.timeScale);

	// Write results
	vpNew[v].pos = newPos;
	vpNew[v].vel = newVel;
}

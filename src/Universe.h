#pragma once

struct ParticleState
{
	ParticleState() = default;

	// Delete copy constructor and copy assignment operator
	ParticleState(const ParticleState&) = delete;
	ParticleState& operator=(const ParticleState&) = delete;

	// Move constructor
	ParticleState(ParticleState&& other) noexcept : mp(std::move(other.mp)), vp(std::move(other.vp)), mpMass(std::move(other.mpMass)) {}

	// Move assignment operator
	ParticleState& operator=(ParticleState&& other) noexcept
	{
		if (this != &other)
		{
			mp = std::move(other.mp);
			vp = std::move(other.vp);
			mpMass = std::move(other.mpMass);
		}
		return *this;
	}

	std::vector<Particle> mp; // Mass particles
	std::vector<Particle> vp; // Visual particles
	std::vector<float> mpMass; // Mass for the mass particles
};

class Universe
{
public:
	Universe() = default;
	~Universe() { Unload(); }

	Universe& operator=(Universe&) = delete;
	Universe(Universe&) = delete;

	void Unload();
	bool IsLoaded() const { return isLoaded; }

	bool Load(ig::IGLOContext&);

	// You can call this function max once per frame.
	void Update(bool leftMouseButtonDown, bool rightMouseButtonDown);

	const ParticleState& GetParticleState() const { return state; }
	void SetParticleState(ParticleState&&);
	void ClearAllParticles();

	uint32_t GetNumMP() const { return (uint32_t)state.mp.size(); }
	uint32_t GetNumVP() const { return (uint32_t)state.vp.size(); }
	uint64_t GetFrame() const { return frame; }

	bool GetComputeShadersEnabled() const { return useComputeShaders; }
	void EnableComputeShaders(bool enable);

	bool GetPaused() const { return isPaused; }
	void SetPaused(bool pause) { isPaused = pause; }

	const PhysicsParams& GetPhysicsParams() const { return physics; }
	void SetPhysicsParams(const PhysicsParams&);

	// Gets the particle buffer that should be read from next.
	const ig::Buffer& ReadMP() const { return (gpu_nextIndex == 1) ? gpu_mp0 : gpu_mp1; }
	const ig::Buffer& ReadVP() const { return (gpu_nextIndex == 1) ? gpu_vp0 : gpu_vp1; }

private:
	bool isLoaded = false;
	ig::IGLOContext* context = nullptr;
	ig::CommandList cmd;

	// Pipelines
	ig::Pipeline computeMassParticles;
	ig::Pipeline computeVisualParticles;

	// Simulation
	PhysicsParams physics;
	uint64_t frame = 0;
	int gpu_nextIndex = 0; // The index of next buffer to write to (gpu_mp0 or gpu_mp1, etc...)
	bool useComputeShaders = true;
	bool isPaused = false;

	// CPU Buffers
	ParticleState state;

	// GPU Buffers
	ig::Buffer gpu_mp0;
	ig::Buffer gpu_mp1;
	ig::Buffer gpu_vp0;
	ig::Buffer gpu_vp1;
	ig::Buffer gpu_mpMass;

	// Gets the particle buffer that should be written to next.
	ig::Buffer& WriteMP() { return (gpu_nextIndex == 0) ? gpu_mp0 : gpu_mp1; }
	ig::Buffer& WriteVP() { return (gpu_nextIndex == 0) ? gpu_vp0 : gpu_vp1; }

	void IncrementBuffers();
	void OnParticleStateChanged();
	void PhysicsGPU();
	void PhysicsCPU();

	static void ReadStructuredBuffer(ig::IGLOContext&, ig::CommandList&, const ig::Buffer& srcBuffer,
		uint32_t elementStride, uint32_t numElements, void* out_writeDest);

};



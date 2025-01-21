#pragma once

struct Galaxy
{
	ig::Vector2 pos = ig::Vector2(0, 0);
	ig::Vector2 vel = ig::Vector2(0, 0);
	uint32_t numRings = 400;//100;
	uint32_t numBands = 400;//20;
	float blackHoleMass = 50;
	float ringRadius = 40;
	float centerEmptySpaceRadius = 20;
};

struct GalaxyRingParams
{
	bool enableRingParticleMass = false;
	float ringParticleMass = 0.001f;
	float extraRingVel = 0;
};

struct GalaxyCollisionParams
{
	float velocity = 0.4f;
	float distFromCenterX = 280;
	float distFromCenterY = 100;
};

struct LinedParticlesParams
{
	uint32_t spacing = 500;
	float minMass = 1.0;
	float maxMass = 1.0;
};

struct RandomMassParticlesParams
{
	uint64_t numParticles = 2500;
	float minMass = 1.0;
	float maxMass = 2.0;
};

ParticleState GenerateGalaxies(const Galaxy* galaxy, uint32_t numGalaxies, const GalaxyRingParams&, const PhysicsParams&);
ParticleState GenerateCenteredGalaxy(const Galaxy& galaxy, const GalaxyRingParams&, const PhysicsParams&, ig::Extent2D backBufferSize);
ParticleState GenerateCollidingGalaxies(const Galaxy* galaxy, uint32_t numGalaxies, const GalaxyCollisionParams&,
	const GalaxyRingParams&, const PhysicsParams&, ig::Extent2D backBufferSize);
ParticleState GenerateLinedParticles(LinedParticlesParams, bool visualParticlesOnly, ig::Extent2D backBufferSize);
ParticleState GenerateRandomMassParticles(RandomMassParticlesParams, ig::Extent2D backBufferSize);

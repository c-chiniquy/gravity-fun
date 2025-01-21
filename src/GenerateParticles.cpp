#include "iglo.h"
#include "ShaderCommon.h"
#include "ColorGradient.h"
#include "Universe.h"
#include "GenerateParticles.h"


ParticleState GenerateGalaxies(const Galaxy* galaxy, uint32_t numGalaxies, const GalaxyRingParams& ringParams,
	const PhysicsParams& physicsParams)
{
	uint64_t totalRingParticles = 0;
	uint64_t totalMP = 0;
	uint64_t totalVP = 0;
	for (uint32_t i = 0; i < numGalaxies; i++)
	{
		uint64_t rp = (uint64_t)galaxy[i].numRings * (uint64_t)galaxy[i].numBands;
		totalRingParticles += rp;
		totalMP += 1;
		if (ringParams.enableRingParticleMass)
		{
			totalMP += rp;
		}
		else
		{
			totalVP += rp;
		}
	}

	ParticleState out;
	out.mpMass.resize(totalMP);
	out.mp.resize(totalMP);
	out.vp.resize(totalVP);

	uint64_t nextMP = 0;
	uint64_t nextVP = 0;

	for (uint32_t i = 0; i < numGalaxies; i++)
	{
		// Center black hole
		out.mp[nextMP] = Particle(galaxy[i].pos, galaxy[i].vel);
		out.mpMass[nextMP] = galaxy[i].blackHoleMass;
		nextMP++;

		float sizePerRing = galaxy[i].ringRadius / (float)galaxy[i].numRings;
		float expectedMass = abs(galaxy[i].blackHoleMass + ringParams.extraRingVel);

		for (uint32_t ring = 0; ring < galaxy[i].numRings; ring++)
		{
			float dist = galaxy[i].centerEmptySpaceRadius + (sizePerRing * (float)ring);
			float vel = sqrt((expectedMass * physicsParams.G) / dist);
			for (uint32_t band = 0; band < galaxy[i].numBands; band++)
			{
				float rot = ((float)band / (float)galaxy[i].numBands) * float(IGLO_PI * 2.0);
				ig::Vector2 position = ig::Vector2(dist * cos(rot), dist * sin(rot)) + galaxy[i].pos;
				ig::Vector2 velocity = ig::Vector2(-vel * sin(rot), vel * cos(rot)) + galaxy[i].vel;
				if (ringParams.enableRingParticleMass)
				{
					out.mp[nextMP] = Particle(position, velocity);
					out.mpMass[nextMP] = ringParams.ringParticleMass;
					nextMP++;
				}
				else
				{
					out.vp[nextVP] = Particle(position, velocity);
					nextVP++;
				}
			}
		}
	}
	return out;
}

ParticleState GenerateCenteredGalaxy(const Galaxy& galaxy, const GalaxyRingParams& ringParams,
	const PhysicsParams& physicsParams, ig::Extent2D backBufferSize)
{
	float centerX = backBufferSize.width / 2.0f;
	float centerY = backBufferSize.height / 2.0f;
	Galaxy out = galaxy;
	out.pos = ig::Vector2(centerX, centerY);
	out.vel = ig::Vector2(0, 0);
	return GenerateGalaxies(&out, 1, ringParams, physicsParams);
}

ParticleState GenerateCollidingGalaxies(const Galaxy* galaxy, uint32_t numGalaxies, const GalaxyCollisionParams& col,
	const GalaxyRingParams& ringParams, const PhysicsParams& physicsParams, ig::Extent2D backBufferSize)
{
	if (numGalaxies != 2) throw std::invalid_argument("Needs 2 galaxies");

	float centerX = backBufferSize.width / 2.0f;
	float centerY = backBufferSize.height / 2.0f;
	Galaxy tempGalaxy[2] = { galaxy[0], galaxy[1] };
	tempGalaxy[0].pos = ig::Vector2(centerX - col.distFromCenterX, centerY - col.distFromCenterY);
	tempGalaxy[0].vel = ig::Vector2(col.velocity, 0);
	tempGalaxy[1].pos = ig::Vector2(centerX + col.distFromCenterX, centerY + col.distFromCenterY);
	tempGalaxy[1].vel = ig::Vector2(-col.velocity, 0);
	return GenerateGalaxies(tempGalaxy, 2, ringParams, physicsParams);
}

ParticleState GenerateLinedParticles(LinedParticlesParams params, bool visualParticlesOnly, ig::Extent2D backBufferSize)
{
	ParticleState out;

	uint32_t cappedSpacing = params.spacing;
	if (cappedSpacing < 1) cappedSpacing = 1;

	uint32_t currentSpacing = 0;
	for (uint32_t y = 0; y < backBufferSize.height; y++)
	{
		for (uint32_t x = 0; x < backBufferSize.width; x++)
		{
			currentSpacing++;
			if (currentSpacing >= cappedSpacing)
			{
				currentSpacing = 0;
				if (visualParticlesOnly)
				{
					out.vp.push_back(Particle(ig::Vector2((float)x, (float)y)));
				}
				else
				{
					out.mpMass.push_back(ig::Random::NextFloat(params.minMass, params.maxMass));
					out.mp.push_back(Particle(ig::Vector2((float)x, (float)y)));
				}
			}
		}
	}

	return out;
}

ParticleState GenerateRandomMassParticles(RandomMassParticlesParams params, ig::Extent2D backBufferSize)
{
	ParticleState out;
	out.mpMass.resize(params.numParticles);
	out.mp.resize(params.numParticles);

	for (uint64_t i = 0; i < params.numParticles; i++)
	{
		out.mpMass[i] = ig::Random::NextFloat(params.minMass, params.maxMass);
		out.mp[i] = Particle(ig::Vector2(
			ig::Random::NextFloat(0, (float)backBufferSize.width),
			ig::Random::NextFloat(0, (float)backBufferSize.height)));
	}

	return out;
}
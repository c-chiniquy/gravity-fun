#pragma once
#include <math.h>

// This header file contains data structs and functions that must
// remain consistent with the compute shaders.

const std::string ShaderFolder = "shaders/";

const uint32_t ComputeBlockSizeX = 16;
const uint32_t ComputeBlockSizeY = 16;

struct ColorPoint
{
	ig::Color32 color = ig::Color32(0, 0, 0);
	float location = 0;
};

struct PhysicsParams
{
	float maxVelocity = 1000;
	float attractionSoftening = 0;
	float spaceScale = 1;
	float timeScale = 1;
	float G = 1;//6.6743; // Gravitational constant

	uint32_t cursorInteractionEnabled = 0; // 0 = disabled, 1 = enabled
	int32_t cursorGravityField = 0; // -1 = negative gravity field, 0 = nothing, 1 = positive gravity field
	float cursorMass = 1000;
	ig::Vector2 cursorPos;
};

struct Particle
{
	Particle(ig::Vector2 pos = ig::Vector2(0, 0), ig::Vector2 vel = ig::Vector2(0, 0)) : pos(pos), vel(vel) {};

	ig::Vector2 pos;
	ig::Vector2 vel;
};

inline void CapMagnitude(ig::Vector2& in_out, float maxMagnitudeSquared, float maxMagnitude)
{
	float magnitudeSquared = (in_out.x * in_out.x) + (in_out.y * in_out.y);
	if (magnitudeSquared > maxMagnitudeSquared)
	{
		float magnitude = sqrt(magnitudeSquared);
		float cap = maxMagnitude / magnitude;
		in_out.x *= cap;
		in_out.y *= cap;
	}
}

// Calculates gravitational acceleration towards another particle
inline ig::Vector2 CalcAcc(const Particle& self, const Particle& other, float otherMass,
	float attractionSoftening, float G, float spaceScale)
{
	// Distance
	ig::Vector2 delta = (other.pos - self.pos) * spaceScale;
	float distSquared = (delta.x * delta.x) + (delta.y * delta.y);
	float dist = sqrt(distSquared + attractionSoftening);
	float dist3 = dist * dist * dist;

	// Acceleration
	float temp = (otherMass * G) / dist3;
	ig::Vector2 acc = delta * temp;

	return acc;
}

// Calculates gravitational acceleration for two particles at the same time
inline void CalcAccForBoth(ig::Vector2& in_out_accA, ig::Vector2& in_out_accB, const Particle& a, const Particle& b,
	float massA, float massB, float attractionSoftening, float G, float spaceScale)
{
	// Distance
	ig::Vector2 delta = (b.pos - a.pos) * spaceScale;
	float distSquared = (delta.x * delta.x) + (delta.y * delta.y);
	float dist = sqrt(distSquared + attractionSoftening);
	float dist3 = dist * dist * dist;

	// Acceleration
	float tempA = (massB * G) / dist3;
	float tempB = (massA * G) / dist3;
	ig::Vector2 accA = (delta * tempA);
	ig::Vector2 accB = (-delta * tempB);

	in_out_accA += accA;
	in_out_accB += accB;
}
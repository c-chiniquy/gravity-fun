
#define ComputeBlockSizeX 16
#define ComputeBlockSizeY 16

struct ColorPoint
{
	uint color;
	float location;
};

struct PhysicsParams
{
	float maxVelocity;
	float attractionSoftening;
	float spaceScale;
	float timeScale;
	float G;

	uint cursorInteractionEnabled;
	int cursorGravityField; // -1 = negative gravity field, 0 = nothing, 1 = positive gravity field
	float cursorMass;
	float2 cursorPos;
};

struct Particle
{
	float2 pos;
	float2 vel;
};

void CapMagnitude(inout float2 v, float maxMagnitudeSquared, float maxMagnitude)
{
	float magnitudeSquared = dot(v, v);
	if (magnitudeSquared > maxMagnitudeSquared)
	{
		float magnitude = sqrt(magnitudeSquared);
		v *= (maxMagnitude / magnitude);
	}
}

float2 CalcAcc(float2 pos, float2 otherPos, float otherMass, float2 spaceScale, float G, float attractionSoftening)
{
	// Distance
	float2 delta = (otherPos - pos) * spaceScale;
	float distSquared = (delta.x * delta.x) + (delta.y * delta.y);
	float dist = sqrt(distSquared + attractionSoftening);
	float dist3 = dist * dist * dist;

	// Acceleration
	float temp = (otherMass * G) / dist3;
	float2 acc = delta * temp;

	return acc;
}
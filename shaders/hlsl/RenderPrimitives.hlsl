#include "Common.hlsl"

cbuffer PushConstants : register(b0)
{
	uint particleBufferIndex;
	uint colorGradientBufferIndex;

	uint numColorPoints;
	uint numParticles;
	float2 screenSize;
	uint uniformColor;
}

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

float4 ConvertToFloat4(uint color32)
{
	return float4(
		(float)(color32 & 0xFF) / 255.0f,
		(float)((color32 >> 8) & 0xFF) / 255.0f,
		(float)((color32 >> 16) & 0xFF) / 255.0f,
		(float)((color32 >> 24) & 0xFF) / 255.0f);
}

float4 GetColorFromGradient(uint particleIndex)
{
	double normLocation = double(particleIndex) / double(numParticles);
	StructuredBuffer<ColorPoint> colorPoints = ResourceDescriptorHeap[colorGradientBufferIndex];

	// Binary search
	int lowerIndex = 0;
	int upperIndex = numColorPoints - 1;
	while (upperIndex - lowerIndex > 1)
	{
		int mid = (lowerIndex + upperIndex) / 2;
		if (colorPoints[mid].location > normLocation)
		{
			upperIndex = mid;
		}
		else
		{
			lowerIndex = mid;
		}
	}

	// Interpolate between the two color points
	double gradient = normLocation - (double)colorPoints[lowerIndex].location;
	double gradientLength = (double)colorPoints[upperIndex].location - (double)colorPoints[lowerIndex].location;
	double t = gradient / gradientLength;

	// Perform linear interpolation of the color
	float4 interpolatedColor = lerp(
		ConvertToFloat4(colorPoints[lowerIndex].color),
		ConvertToFloat4(colorPoints[upperIndex].color),
		(float)t);

	return interpolatedColor;
}

float2 ConvertToScreenSpaceCoords(float2 pos)
{
	float2 screenPos;
	screenPos.x = (pos.x / screenSize.x) * 2.0 - 1.0;
	screenPos.y = 1.0 - (pos.y / screenSize.y) * 2.0;
	return screenPos;
}

#if defined(COLORBUFFER) || defined(UNICOLOR)
PixelInput VS_Rects(uint vertexID : SV_VertexID)
{
	StructuredBuffer<Particle> particles = ResourceDescriptorHeap[particleBufferIndex];

	uint elementSize = sizeof(Particle);
	uint elementIndex = (vertexID / 6);
	uint cornerIndex = (vertexID % 6);

	float2 quad[6] = { float2(0, 0), float2(2, 0), float2(0, 2),
					   float2(0, 2), float2(2, 0), float2(2, 2) };

	float2 cornerPos = particles[elementIndex].pos + quad[cornerIndex] - float2(1, 1);
	float2 screenPos = ConvertToScreenSpaceCoords(cornerPos);

	PixelInput output;
	output.position = float4(screenPos, 0.0f, 1.0f);

#ifdef COLORBUFFER
	output.color = GetColorFromGradient(elementIndex);
#endif
#ifdef UNICOLOR
	output.color = ConvertToFloat4(uniformColor);
#endif

	return output;
}

PixelInput VS_Points(uint vertexID : SV_VertexID)
{
	StructuredBuffer<Particle> particles = ResourceDescriptorHeap[particleBufferIndex];

	float2 particlePos = particles[vertexID].pos;
	float2 screenPos = ConvertToScreenSpaceCoords(particlePos);

	PixelInput output;
	output.position = float4(screenPos, 0.0f, 1.0f);

#ifdef COLORBUFFER
	output.color = GetColorFromGradient(vertexID);
#endif
#ifdef UNICOLOR
	output.color = ConvertToFloat4(uniformColor);
#endif

	return output;
}
#endif

float4 PS_Basic(PixelInput input) : SV_Target
{
	return input.color;
}

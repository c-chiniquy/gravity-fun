#include "Common.hlsl"

cbuffer PushConstants : register(b0)
{
	uint textureIndex;

	float2 inverseHeatmapPixelSize; // Possible values: 1/1, 1/2, 1/4, 1/8, etc...
}

float4 HeatmapGrayscale(uint numParticles)
{
	float v = (float)numParticles / 255.0f;
	return float4(v, v, v, 1.0f);
}

float4 HeatmapColor(uint numParticles)
{
	uint level = numParticles / 256;
	uint red = 0;
	uint green = 0;
	uint blue = 0;
	if (level == 0)
	{
		blue = numParticles;
	}
	else if (level == 1)
	{
		green = numParticles - 256;
		blue = 255;
	}
	else if (level == 2)
	{
		green = 255;
		blue = 255 + 512 - numParticles;
	}
	else if (level == 3)
	{
		blue = 0;
		red = numParticles - 768;
		green = 255;
	}
	else if (level == 4)
	{
		red = 255;
		green = 255 + (1024 - numParticles);
		blue = 0;
	}
	else if (level == 5)
	{
		red = 255;
		green = numParticles - 1280;
		blue = numParticles - 1280;
	}
	else
	{
		red = 255;
		green = 255;
		blue = 255;
	}
	return float4(red / 255.0f, green / 255.0f, blue / 255.0f, 1.0f);
}

struct PixelInput
{
	float4 position : SV_POSITION;
};

PixelInput VS_Heatmap(uint vertexID : SV_VertexID)
{
	float2 quad[4] = { float2(-1, -1), float2(1, -1), float2(-1, 1), float2(1, 1) };

	PixelInput output;
	output.position = float4(quad[vertexID], 0.0f, 1.0f);

	return output;
}

#if defined(HEATMAP_GRAYSCALE) || defined(HEATMAP_COLOR)
float4 PS_Heatmap(PixelInput input) : SV_Target
{
	Texture2D<uint> heatmapTexture = ResourceDescriptorHeap[textureIndex];

	// Out of bounds texture coordinates will always return 0.
	int2 texCoords = uint2(input.position.xy * inverseHeatmapPixelSize);
	uint numParticles = heatmapTexture.Load(int3(texCoords, 0));

#ifdef HEATMAP_GRAYSCALE
	float4 color = HeatmapGrayscale(numParticles);
#endif
#ifdef HEATMAP_COLOR
	float4 color = HeatmapColor(numParticles);
#endif

	return color;
}
#endif
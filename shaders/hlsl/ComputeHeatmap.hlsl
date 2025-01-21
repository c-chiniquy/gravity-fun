#include "Common.hlsl"

cbuffer PushConstants : register(b0)
{
	uint particleBufferIndex;
	uint destTextureIndex;

	float2 inverseHeatmapPixelSize; // Possible values: 1/1, 1/2, 1/4, 1/8, etc...
	uint2 destTextureSize;
	uint numParticles;
	uint dispatchX;
}

[numthreads(ComputeBlockSizeX, ComputeBlockSizeY, 1)]
void CS_Heatmap(uint3 groupID : SV_GroupID, uint3 threadID : SV_GroupThreadID, uint3 dispatchID : SV_DispatchThreadID)
{
	StructuredBuffer<Particle> particles = ResourceDescriptorHeap[particleBufferIndex];
	RWTexture2D<uint> destTexture = ResourceDescriptorHeap[destTextureIndex];

	uint index = (groupID.x * ComputeBlockSizeX + threadID.x) +
		(groupID.y * ComputeBlockSizeY + threadID.y) * dispatchX * ComputeBlockSizeX;
	if (index >= numParticles) return;

	uint2 texCoord = uint2(particles[index].pos * inverseHeatmapPixelSize);

	if (texCoord.x >= 0 && texCoord.x < destTextureSize.x && texCoord.y >= 0 && texCoord.y < destTextureSize.y)
	{
		// Increment pixel value
		InterlockedAdd(destTexture[texCoord], 1);
	}
}


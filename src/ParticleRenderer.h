#pragma once

// Pipeline permutations
enum class PermPrimitives
{
	Points = 0,
	Rects
};
enum class PermBlendStates
{
	BlendDisabled = 0,
	AdditiveBlend
};
enum class PermMSAA
{
	MSAA_Disabled = 0,
	MSAA_Enabled
};
enum class PermColor
{
	Uniform = 0, // Use the same color for all particles
	Gradient, // Each particle has a unique color based on a color gradient
};

enum class DrawMode
{
	Points = 0,
	Rects,
	Heatmap_Grayscale,
	Heatmap_Color
};

struct GraphicsParams
{
	DrawMode drawMode = DrawMode::Rects;

	// For when rendering primitives (points, rects)
	PermBlendStates permBlend = PermBlendStates::AdditiveBlend;
	PermColor permColor = PermColor::Uniform;
	PermMSAA permMSAA = PermMSAA::MSAA_Enabled;
	ig::Color32 uniformColor = ig::Color32(128, 4, 1);

	// For when rendering heatmap
	uint32_t heatmapPixelSize = 1; // 1, 2, 4, 8, 16, etc...
	bool clearHeatmap = true;
};

class ParticleRenderer
{
public:
	ParticleRenderer() = default;
	~ParticleRenderer() { Unload(); }

	ParticleRenderer& operator=(ParticleRenderer&) = delete;
	ParticleRenderer(ParticleRenderer&) = delete;

	void Unload();
	bool IsLoaded() const { return isLoaded; }

	bool Load(ig::IGLOContext&);

	ig::Extent2D GetTextureSize() const { return textureSize; }
	void ResizeTextures(ig::Extent2D textureSize);

	const GraphicsParams& GetGraphicsParams() const { return graphics; }
	void SetGraphicsParams(const GraphicsParams&);

	void ClearHeatmap();
	void SetColorGradient(const ColorGradient* colorGradient);

	// You can call this function max once per frame.
	void Render(const ig::Buffer& bufferMP, const ig::Buffer& bufferVP, uint32_t numMP, uint32_t numVP);

private:
	bool isLoaded = false;
	ig::IGLOContext* context = nullptr;
	const ColorGradient* colorGradient = nullptr;
	ig::CommandList cmd;

	// The size of the render target texture and heatmap texture
	ig::Extent2D textureSize;

	GraphicsParams graphics;

	ig::Pipeline computeHeatmap;
	ig::Pipeline renderHeatmapGrayscale;
	ig::Pipeline renderHeatmapColor;
	ig::Pipeline renderPrims[2][2][2][2]; // [PermPrimitives][PermBlendStates][PermMSAA][PermColor]

	std::shared_ptr<ig::Texture> heatmap;
	std::shared_ptr<ig::Texture> renderTarget; // For MSAA rendering
	ig::RenderTargetDesc renderTargetDesc;

	void RenderHeatmap(const ig::Buffer& bufferMP, const ig::Buffer& bufferVP, uint32_t numMP, uint32_t numVP);
	void RenderPrimitives(const ig::Buffer& bufferMP, const ig::Buffer& bufferVP, uint32_t numMP, uint32_t numVP);
	static ig::Extent2D GetHeatmapSize(ig::Extent2D backBufferSize, uint32_t heatmapPixelSize);

};

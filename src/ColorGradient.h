#pragma once

constexpr uint32_t MaxColorPoints = 8;

class ColorGradient
{
public:
	ColorGradient() = default;
	~ColorGradient() { Unload(); }

	ColorGradient& operator=(ColorGradient&) = delete;
	ColorGradient(ColorGradient&) = delete;

	void Unload();
	bool IsLoaded() const { return isLoaded; }

	bool Load(const ig::IGLOContext&, ig::CommandList&);
	void SetColorGradient(ig::CommandList& cmd, const ColorPoint* colors, uint32_t numColors);

	const ig::Buffer& GetBuffer() const { return buffer; }
	const std::vector<ColorPoint>& GetColors() const { return colors; }
	uint32_t GetNumColorPoints() const { return (uint32_t)colors.size(); }

	// Custom gradient editor
	static bool ImGuiGradientEditor(std::vector<ColorPoint>& in_out_colors);

private:
	bool isLoaded = false;

	std::vector<ColorPoint> colors;
	ig::Buffer buffer;
};


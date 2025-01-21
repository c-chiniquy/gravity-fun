#include "iglo.h"
#include "ShaderCommon.h"
#include "ColorGradient.h"
#include "imgui.h"
#include <array>


bool ColorGradient::ImGuiGradientEditor(std::vector<ColorPoint>& in_out_colors)
{
	bool modified = false;

	std::vector<ColorPoint> sorted = in_out_colors;
	std::sort(sorted.begin(), sorted.end(), [](const ColorPoint& a, const ColorPoint& b)
		{
			return a.location < b.location;
		});

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	ImVec2 gradientSize(300, 20);
	ImVec2 gradientStart = ImGui::GetCursorScreenPos();
	ImVec2 gradientEnd = ImVec2(gradientStart.x + gradientSize.x, gradientStart.y + gradientSize.y);

	for (int i = 0; i < sorted.size() - 1; i++)
	{
		const ColorPoint& p1 = sorted[i];
		const ColorPoint& p2 = sorted[i + 1];

		// Extract color components
		ImU32 col1 = p1.color.rgba;
		ImU32 col2 = p2.color.rgba;

		// Calculate segment positions
		float startX = gradientStart.x + (p1.location * gradientSize.x);
		float endX = gradientStart.x + (p2.location * gradientSize.x);

		// Draw gradient segment
		draw_list->AddRectFilledMultiColor(ImVec2(startX, gradientStart.y), ImVec2(endX, gradientEnd.y), col1, col2, col2, col1);
	}
	ImGui::Dummy(gradientSize);

	for (int i = 0; i < in_out_colors.size(); i++)
	{
		ImGui::PushID(i);

		ImVec4 colorVec = ImGui::ColorConvertU32ToFloat4(in_out_colors[i].color.rgba);

		if (ImGui::ColorEdit4("Color", (float*)&colorVec))
		{
			in_out_colors[i].color = ImGui::ColorConvertFloat4ToU32(colorVec);
			modified = true;
		}

		if (ImGui::SliderFloat("Position", &in_out_colors[i].location, 0.0f, 1.0f))
		{
			modified = true;
		}

		if (ImGui::Button("Remove"))
		{
			in_out_colors.erase(in_out_colors.begin() + i);
			modified = true;
			ImGui::PopID();
			break;
		}

		ImGui::Separator();
		ImGui::PopID();
	}

	if (ImGui::Button("Add Color"))
	{
		in_out_colors.push_back({ ig::Color32(255,255,255), 0.5f }); // Default: White color at midpoint
		modified = true;
	}

	return modified;
}

void ColorGradient::Unload()
{
	isLoaded = false;

	colors.clear();
	colors.shrink_to_fit();

	buffer.Unload();
}

bool ColorGradient::Load(const ig::IGLOContext& context, ig::CommandList& cmd)
{
	Unload();

	if (!buffer.LoadAsStructuredBuffer(context, sizeof(ColorPoint), MaxColorPoints, ig::BufferUsage::Default))
	{
		return false;
	}

	isLoaded = true;

	std::array<ColorPoint, 4> initColors =
	{
		ColorPoint{ig::Colors::Yellow, 0},
		ColorPoint{ig::Colors::Red, 0.5f},
		ColorPoint{ig::Colors::Cyan, 0.5f},
		ColorPoint{ig::Colors::Blue, 1.0f},
	};

	SetColorGradient(cmd, initColors.data(), (uint32_t)initColors.size());
	return true;
}

void ColorGradient::SetColorGradient(ig::CommandList& cmd, const ColorPoint* colors, uint32_t numColors)
{
	if (numColors < 1) throw std::invalid_argument("Too few colors");
	if (numColors > MaxColorPoints) throw std::invalid_argument("Too many colors");

	this->colors.clear();
	this->colors.resize(numColors);

	for (uint32_t i = 0; i < numColors; i++)
	{
		this->colors[i] = colors[i];
	}

	std::array<ColorPoint, MaxColorPoints> bufferData = {};
	for (uint32_t i = 0; i < numColors; i++)
	{
		bufferData[i] = this->colors[i];
	}

	std::sort(bufferData.begin(), bufferData.begin() + numColors, [](const ColorPoint& a, const ColorPoint& b)
		{
			return a.location < b.location;
		});

	this->buffer.SetData(cmd, bufferData.data());
}
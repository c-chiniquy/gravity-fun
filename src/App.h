
// iglo
#include "iglo.h"
#include "iglo_main_loop.h"

// imgui
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx12.h"

// Gravity fun
#include "ShaderCommon.h"
#include "ColorGradient.h"
#include "Universe.h"
#include "ParticleRenderer.h"
#include "GenerateParticles.h"
#include "Scenarios.h"


class App
{
public:

	void Run();

private:

	// iglo callbacks
	void Start();
	void OnLoopExited();
	void OnEvent(ig::Event e);
	void Update(double elapsedTime);
	void FixedUpdate();
	void Draw();
	void OnDeviceRemoved();

	ig::IGLOContext context;
	ig::CommandList cmd;
	ig::MainLoop mainloop;

	static constexpr uint32_t numFramesInFlight = 2;
	static constexpr uint32_t numBackBuffers = 3;

	void ApplyScenario(Scenario scenario);
	void LoadSizeDependantResources(ig::Extent2D backBufferSize);
	void UpdateImgui();

	ColorGradient colorGradient;
	Universe universe;
	ParticleRenderer particleRenderer;

	ig::Descriptor imguiDescriptor;
	ImGuiInputParams inputParams;

	// Mouse interaction with universe frame
	bool leftMouseButtonDown = false;
	bool rightMouseButtonDown = false;

	// Recording
	bool isRecording = false;
	ig::Texture readableTexture;
	const std::string recordingsFolder = "recordings/";

};

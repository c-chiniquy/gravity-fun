#include "Main.h"

// Agility SDK path and version
extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 715; }
extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = ".\\D3D12\\"; }

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool WndProcHook(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) return true;
	return false;
}

void Start()
{
	// NOTE: All command lists we use access the same resources, and we want
	// to use barriers to sync resource usage between these command lists.
	// Since barriers don't work accross different command queues,
	// all these command lists must be of the same type so that they use the same command queue.
	cmd.Load(context, ig::CommandListType::Graphics);

	cmd.Begin();
	{
		if (!colorGradient.Load(context, cmd))
		{
			ig::PopupMessage("Failed to load color gradient!", "Error", &context);
			mainloop.Quit();
			return;
		}
	}
	cmd.End();

	context.WaitForCompletion(context.Submit(cmd));

	if (!universe.Load(context))
	{
		ig::PopupMessage("Failed to load universe!", "Error", &context);
		mainloop.Quit();
		return;
	}

	if (!particleRenderer.Load(context))
	{
		ig::PopupMessage("Failed to load particle renderer!", "Error", &context);
		mainloop.Quit();
		return;
	}
	particleRenderer.SetColorGradient(&colorGradient);

	LoadSizeDependantResources(context.GetBackBufferSize());

	// ImGui
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Keyboard controls
		io.LogFilename = nullptr;
		io.IniFilename = nullptr;

		ImGui_ImplWin32_Init(context.GetWindowHWND());

		ig::Descriptor descriptor = context.GetDescriptorHeap().AllocatePersistentResource();
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = context.GetDescriptorHeap().GetD3D12CPUDescriptorHandleForResource(descriptor);
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = context.GetDescriptorHeap().GetD3D12GPUDescriptorHandleForResource(descriptor);
		ImGui_ImplDX12_Init(
			context.GetD3D12Device(),
			context.GetNumFramesInFlight(),
			ig::GetFormatInfoDXGI(ig::Format::BYTE_BYTE_BYTE_BYTE).dxgiFormat,
			context.GetDescriptorHeap().GetD3D12DescriptorHeap_SRV_CBV_UAV(),
			cpuHandle,
			gpuHandle);
		context.SetWndProcHookCallback(WndProcHook);
	}

	// Randomness
	ig::UniformRandom uniformRandom;
	uniformRandom.SetSeedUsingRandomDevice();
	ig::Random::SetSeed(uniformRandom.NextUInt32());

	ApplyScenario(Scenario::NBodyGalaxy20k);
}

void LoadSizeDependantResources(ig::Extent2D backBufferSize)
{
	// Staging texture for recording back buffer
	if (!readableTexture.IsLoaded() ||
		readableTexture.GetWidth() != backBufferSize.width ||
		readableTexture.GetHeight() != backBufferSize.height)
	{
		context.WaitForIdleDevice();
		readableTexture.Load(context, backBufferSize.width, backBufferSize.height,
			context.GetBackBufferRenderTargetDesc().colorFormats.at(0), ig::TextureUsage::Readable);
		ig::Print(ig::ToString("Readable texture size ", readableTexture.GetWidth(), "x", readableTexture.GetHeight(), "\n"));
	}

	if (particleRenderer.GetTextureSize() != backBufferSize)
	{
		particleRenderer.ResizeTextures(backBufferSize);
	}
}

void ApplyScenario(Scenario scenario)
{
	particleRenderer.ClearHeatmap();
	universe.ClearAllParticles();

	ScenarioDesc desc = GenerateScenarioDesc(scenario, context.GetBackBufferSize());
	universe.SetParticleState(std::move(desc.state));
	universe.SetPhysicsParams(desc.physics);
	particleRenderer.SetGraphicsParams(desc.graphics);
	inputParams = desc.input;
}

void OnLoopExited()
{
	context.WaitForIdleDevice();

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Update(double elapsedSeconds)
{
	// When paused and window is minimized, lower CPU usage
	if (universe.GetPaused() && context.GetWindowMinimized())
	{
		ig::SleepMilliseconds(1);
	}

	universe.Update(leftMouseButtonDown, rightMouseButtonDown);

}

void OnEvent(ig::Event e)
{
	if (e.type == ig::EventType::CloseRequest)
	{
		mainloop.Quit();
		return;
	}

	// Imgui
	ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureKeyboard)
	{
		if (e.type == ig::EventType::KeyPress) return;
		if (e.type == ig::EventType::KeyDown) return;
		if (e.type == ig::EventType::KeyUp) return;
		if (e.type == ig::EventType::TextEntered) return;
	}
	if (io.WantCaptureMouse)
	{
		if (e.type == ig::EventType::MouseMove) return;
		if (e.type == ig::EventType::MouseWheel) return;
		if (e.type == ig::EventType::MouseButtonDown) return;
		if (e.type == ig::EventType::MouseButtonUp) return;
	}

	// App
	if (e.type == ig::EventType::KeyPress)
	{
		switch (e.key)
		{
		case ig::Key::F11:
			context.ToggleFullscreen();
			break;

		case ig::Key::Enter:
			if (context.IsKeyDown(ig::Key::LeftAlt)) context.ToggleFullscreen();
			break;

		case ig::Key::Escape:
			mainloop.Quit();
			break;

		default:
			break;
		}
	}
	else if (e.type == ig::EventType::MouseButtonDown)
	{
		if (e.mouse.button == ig::MouseButton::Left) leftMouseButtonDown = true;
		if (e.mouse.button == ig::MouseButton::Right) rightMouseButtonDown = true;
	}
	else if (e.type == ig::EventType::MouseButtonUp)
	{
		if (e.mouse.button == ig::MouseButton::Left) leftMouseButtonDown = false;
		if (e.mouse.button == ig::MouseButton::Right) rightMouseButtonDown = false;
	}
	else if (e.type == ig::EventType::Resize)
	{
		LoadSizeDependantResources(e.resize);
	}
}

bool DragFloat_Duo(const char* label, const char* format0, const char* format1,
	float* v0, float* v1, float v_speed = 1.0f, float v_min = 0.0, float v_max = 0.0, float item_width = 140.0f)
{
	bool result = false;
	std::string id0 = "##" + std::string(label) + "0";
	std::string id1 = "##" + std::string(label) + "1";

	ImGui::PushItemWidth(item_width);
	result |= ImGui::DragFloat(id0.c_str(), v0, v_speed, v_min, v_max, format0);
	ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
	result |= ImGui::DragFloat(id1.c_str(), v1, v_speed, v_min, v_max, format1);
	ImGui::PopItemWidth();

	return result;
}
bool DragInt_Duo(const char* label, const char* format0, const char* format1,
	int* v0, int* v1, float v_speed = 1.0f, int v_min = 0.0, int v_max = 0.0)
{
	bool result = false;
	std::string id0 = "##" + std::string(label) + "0";
	std::string id1 = "##" + std::string(label) + "1";

	ImGui::PushItemWidth(140);
	result |= ImGui::DragInt(id0.c_str(), v0, v_speed, v_min, v_max, format0);
	ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
	result |= ImGui::DragInt(id1.c_str(), v1, v_speed, v_min, v_max, format1);
	ImGui::PopItemWidth();

	return result;
}

void UpdateImgui()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowSize(ImVec2(0, 0));
	ImGui::Begin("Gravity Fun", nullptr, ImGuiWindowFlags_MenuBar);
	{
		ImGui::BeginMenuBar();
		if (ImGui::BeginMenu("Scenarios"))
		{
			for (uint32_t i = 0; i < IM_ARRAYSIZE(ScenarioItems); i++)
			{
				if (ImGui::MenuItem(ScenarioItems[i]))
				{
					ApplyScenario((Scenario)i);
				}
				if (i == (uint32_t)Scenario::CursorInteraction)
				{
					ImGui::SetItemTooltip(InteractWithParticlesHint);
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Clear particles"))
		{
			universe.ClearAllParticles();
		}
		ImGui::EndMenuBar();

		std::string strA = ig::ToString("Mass particles: ", universe.GetNumMP());
		std::string strB = ig::ToString("Visual particles: ", universe.GetNumVP());
		bool isPaused = universe.GetPaused();
		if (ImGui::Checkbox("Pause", &isPaused))
		{
			universe.SetPaused(isPaused);
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(180);
		ImGui::Text(strA.c_str(), "");
		if (isRecording)
		{
			ImGui::Text(ig::ToString("Recording frame ", universe.GetFrame()).c_str());
			ImGui::SameLine();
		}
		ImGui::SetCursorPosX(180);
		ImGui::Text(strB.c_str(), "");

		// Tabs
		if (ImGui::BeginTabBar("Tab0", 0))
		{
			if (ImGui::BeginTabItem("Galaxies"))
			{
				for (int i = 0; i < 2; i++)
				{
					ImGui::SeparatorText(ig::ToString("Galaxy ", i).c_str());
					DragInt_Duo(ig::ToString("RingsAndBands", i).c_str(), "Rings: %d", "Bands: %d",
						(int*)&inputParams.galaxy[i].numRings, (int*)&inputParams.galaxy[i].numBands, 1.0f, 1, 10000);
					ImGui::PushItemWidth(100);
					ImGui::DragFloat(ig::ToString("Black hole mass##", i).c_str(), &inputParams.galaxy[i].blackHoleMass, 1.0f);
					ImGui::DragFloat(ig::ToString("Ring radius##", i).c_str(), &inputParams.galaxy[i].ringRadius, 1.0f);
					ImGui::DragFloat(ig::ToString("Center space radius##", i).c_str(), &inputParams.galaxy[i].centerEmptySpaceRadius, 1.0f);
					ImGui::PopItemWidth();
				}
				ImGui::SeparatorText("Galaxy collision");
				ImGui::PushItemWidth(100);
				ImGui::DragFloat("velocity", &inputParams.colParam.velocity, 0.1f);
				ImGui::PopItemWidth();
				DragFloat_Duo("Distance from center", "DistX: %.3f", "DistY: %.3f",
					&inputParams.colParam.distFromCenterX,
					&inputParams.colParam.distFromCenterY, 10);

				ImGui::Separator();
				if (ImGui::Button("Default values"))
				{
					inputParams.galaxy[0] = Galaxy();
					inputParams.galaxy[1] = Galaxy();
					inputParams.colParam = GalaxyCollisionParams();
					inputParams.ringParam = ImGuiInputParams().ringParam;
				}
				ImGui::SameLine();
				if (ImGui::Button("Copy Galaxy 0 -> Galaxy 1"))
				{
					inputParams.galaxy[1] = inputParams.galaxy[0];
				}
				ImGui::SeparatorText("Generate particles");
				ImGui::PushItemWidth(100);
				ImGui::DragFloat("Extra velocity for ring particles", &inputParams.ringParam.extraRingVel, 0.1f);
				ImGui::PopItemWidth();
				ImGui::Checkbox("Ring particles have mass", &inputParams.ringParam.enableRingParticleMass);
				ImGui::SetItemTooltip("Note: Mass particles require alot more computational power than visual particles!");
				if (inputParams.ringParam.enableRingParticleMass)
				{
					ImGui::PushItemWidth(100);
					ImGui::DragFloat("Ring particle mass", &inputParams.ringParam.ringParticleMass, 0.001f, 0, 1, "%.4f");
					ImGui::PopItemWidth();
				}
				if (ImGui::Button("Generate 1 galaxy"))
				{
					particleRenderer.ClearHeatmap();
					universe.ClearAllParticles();
					ParticleState state = GenerateCenteredGalaxy(inputParams.galaxy[0], inputParams.ringParam,
						universe.GetPhysicsParams(), context.GetBackBufferSize());
					universe.SetParticleState(std::move(state));
				}
				ImGui::SameLine();
				if (ImGui::Button("Generate 2 galaxies"))
				{
					particleRenderer.ClearHeatmap();
					universe.ClearAllParticles();
					ParticleState state = GenerateCollidingGalaxies(inputParams.galaxy, 2, inputParams.colParam,
						inputParams.ringParam, universe.GetPhysicsParams(), context.GetBackBufferSize());
					universe.SetParticleState(std::move(state));
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("NBody"))
			{
				ImGui::SeparatorText("Lined particles");
				ImGui::PushItemWidth(100);
				ImGui::DragInt("Particle spacing", (int*)&inputParams.linedParam.spacing, 1, 0, 1000);
				ImGui::SetItemTooltip("Less spacing = more particles");
				ImGui::PopItemWidth();
				DragFloat_Duo("Random particle mass", "Min: %.2f", "Max: %.2f",
					&inputParams.linedParam.minMass,
					&inputParams.linedParam.maxMass,
					1.0f, 0, 10000, 100);
				ImGui::SameLine();
				ImGui::Text("Particle mass");
				if (ImGui::Button("Generate lined particles", ImVec2(204, 0)))
				{
					particleRenderer.ClearHeatmap();
					universe.ClearAllParticles();
					ParticleState state = GenerateLinedParticles(inputParams.linedParam, false, context.GetBackBufferSize());
					universe.SetParticleState(std::move(state));
				}

				ImGui::SeparatorText("Randomized particles");
				ImGui::PushItemWidth(100);
				ImGui::DragInt("Particles", (int*)&inputParams.randParam.numParticles, 10, 0, 10000000);
				ImGui::PopItemWidth();
				DragFloat_Duo("Lined particle mass", "Min: %.2f", "Max: %.2f",
					&inputParams.randParam.minMass,
					&inputParams.randParam.maxMass,
					1.0f, 0, 10000, 100);
				ImGui::SameLine();
				ImGui::Text("Particle mass");
				if (ImGui::Button("Generate random particles", ImVec2(204, 0)))
				{
					particleRenderer.ClearHeatmap();
					universe.ClearAllParticles();
					ParticleState state = GenerateRandomMassParticles(inputParams.randParam, context.GetBackBufferSize());
					universe.SetParticleState(std::move(state));
				}

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Physics"))
			{
				ImGui::SeparatorText("Computation");
				bool useComputeShaders = universe.GetComputeShadersEnabled();
				if (ImGui::Checkbox("Use compute shaders", &useComputeShaders))
				{
					universe.EnableComputeShaders(useComputeShaders);
				}
				PhysicsParams physics = universe.GetPhysicsParams();
				bool edited = false;
				ImGui::SetItemTooltip("Note: Disabling compute shaders will drop the FPS significantly\n"
					"if simulating tens of thousands of mass particles!");
				ImGui::SeparatorText("Physics constants");
				ImGui::PushItemWidth(100);
				edited |= ImGui::DragFloat("Max velocity", &physics.maxVelocity, 0.1f, -1, 1000, "%.4f");
				edited |= ImGui::DragFloat("Attraction softening", &physics.attractionSoftening, 0.1f, 0, 1000, "%.4f");
				edited |= ImGui::DragFloat("Space scale", &physics.spaceScale, 0.1f);
				edited |= ImGui::DragFloat("Time scale", &physics.timeScale, 0.1f);
				edited |= ImGui::DragFloat("Gravitational constant", &physics.G);
				ImGui::PopItemWidth();
				ImGui::SeparatorText("Cursor interactions");
				bool enableCursor = (physics.cursorInteractionEnabled == 1) ? true : false;
				if (ImGui::Checkbox("Enable cursor particle interactions", &enableCursor))
				{
					edited = true;
					physics.cursorInteractionEnabled = enableCursor ? 1 : 0;
				}
				ImGui::SetItemTooltip(InteractWithParticlesHint);
				if (physics.cursorInteractionEnabled)
				{
					ImGui::PushItemWidth(100);
					edited |= ImGui::DragFloat("Cursor mass", &physics.cursorMass, 100);
					ImGui::PopItemWidth();
				}
				if (edited)
				{
					universe.SetPhysicsParams(physics);
				}
				if (ImGui::Button("Generate visual particles for cursor interaction"))
				{
					particleRenderer.ClearHeatmap();
					universe.ClearAllParticles();
					LinedParticlesParams linedVisualParams;
					linedVisualParams.spacing = 0;
					ParticleState state = GenerateLinedParticles(linedVisualParams, true, context.GetBackBufferSize());
					universe.SetParticleState(std::move(state));
				}

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Graphics"))
			{
				const char* drawModeItems[] =
				{
					"Points",
					"Rectangles",
					"Heatmap (Grayscale)",
					"Heatmap (Color)"
				};
				GraphicsParams graphics = particleRenderer.GetGraphicsParams();
				int currentDrawModeItem = (int)graphics.drawMode;
				bool edited = false;
				if (ImGui::Combo("Draw mode", &currentDrawModeItem, drawModeItems, IM_ARRAYSIZE(drawModeItems)))
				{
					edited = true;
					graphics.drawMode = (DrawMode)currentDrawModeItem;
				}
				if (graphics.drawMode == DrawMode::Points || graphics.drawMode == DrawMode::Rects)
				{
					bool enableMSAA = (graphics.permMSAA == PermMSAA::MSAA_Enabled);
					if (ImGui::Checkbox("Enable MSAA", &enableMSAA))
					{
						edited = true;
						graphics.permMSAA = enableMSAA ? PermMSAA::MSAA_Enabled : PermMSAA::MSAA_Disabled;
					}
					ImGui::SetItemTooltip("MSAA = Multi sampled anti alias");

					bool useAdditiveBlend = (graphics.permBlend == PermBlendStates::AdditiveBlend);
					if (ImGui::Checkbox("Additive blending", &useAdditiveBlend))
					{
						edited = true;
						graphics.permBlend = useAdditiveBlend ? PermBlendStates::AdditiveBlend : PermBlendStates::BlendDisabled;
					}

					bool useGradientColor = (graphics.permColor == PermColor::Gradient);
					if (ImGui::Checkbox("Color gradient", &useGradientColor))
					{
						edited = true;
						graphics.permColor = useGradientColor ? PermColor::Gradient : PermColor::Uniform;
					}

					if (useGradientColor)
					{
						static std::vector<ColorPoint> colorPoints = {};
						colorPoints = colorGradient.GetColors();
						if (ColorGradient::ImGuiGradientEditor(colorPoints))
						{
							if (colorPoints.size() < 1)
							{
								colorPoints = { ColorPoint{ig::Color32(255,255,255), 0.5f} };
							}
							while (colorPoints.size() > MaxColorPoints)
							{
								colorPoints.pop_back();
							}
							context.WaitForIdleDevice();
							cmd.Begin();
							{
								colorGradient.SetColorGradient(cmd, colorPoints.data(), (uint32_t)colorPoints.size());
							}
							cmd.End();
							context.Submit(cmd);
							context.WaitForIdleDevice();
						}
					}
					else
					{
						ig::Color tempColor = ig::Color(graphics.uniformColor);
						ImGui::PushItemWidth(200);
						if (ImGui::ColorEdit3("Particle color", &tempColor.red))
						{
							edited = true;
							graphics.uniformColor = tempColor.ToColor32();
						}
						ImGui::PopItemWidth();
					}
				}
				if (graphics.drawMode == DrawMode::Heatmap_Grayscale || graphics.drawMode == DrawMode::Heatmap_Color)
				{
					ImGui::PushItemWidth(50);
					const int maxPixelSize = 32;
					if (ImGui::DragInt("Heatmap pixel size", (int*)&graphics.heatmapPixelSize, 1, 1, maxPixelSize))
					{
						edited = true;
						if (graphics.heatmapPixelSize == 0) graphics.heatmapPixelSize = 1;
						if (graphics.heatmapPixelSize > maxPixelSize) graphics.heatmapPixelSize = maxPixelSize;
					}
					ImGui::PopItemWidth();
					edited |= ImGui::Checkbox("Clear heatmap every frame", &graphics.clearHeatmap);
				}
				if (edited)
				{
					particleRenderer.SetGraphicsParams(graphics);
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Recording"))
			{
				ImGui::Text(ig::ToString("Status: ", isRecording ? "Recording!" : "Not recording").c_str());
				ImGui::Text(ig::ToString("Current frame: ", universe.GetFrame()).c_str());
				ImGui::Text(ig::ToString("Resolution: ", context.GetWidth(), "x", context.GetHeight()).c_str());
				ImGui::Text(ig::ToString("Recordings folder: ", recordingsFolder).c_str());
				if (ImGui::Button("Open recordings folder"))
				{
					if (!ig::DirectoryExists(recordingsFolder))
					{
						ig::CreateDirectory(recordingsFolder);
					}
					std::string fullpath = ig::GetCurrentPath() + "/" + recordingsFolder;
					ShellExecuteA(context.GetWindowHWND(), "open", fullpath.c_str(), 0, 0, SW_SHOWNORMAL);
				}
				ImGui::Checkbox("Record frames", &isRecording);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("iglo"))
			{
				std::string str = ig::ToString(ig::igloVersion, " ", context.GetGraphicsAPIShortName());
				ImGui::SeparatorText(str.c_str());

				const char* presentModeItems[] = { "Immediate With Tearing", "Immediate", "Vsync" };
				ig::PresentMode presentMode = context.GetPresentMode();
				if (ImGui::Combo("Present mode", (int*)&presentMode, presentModeItems, IM_ARRAYSIZE(presentModeItems)))
				{
					context.SetPresentMode(presentMode);
				}
				int frameRateLimit = (int)mainloop.GetFrameRateLimit();
				if (ImGui::SliderInt("Frame rate limit", (int*)&frameRateLimit, 0, 500))
				{
					mainloop.SetFrameRateLimit((double)frameRateLimit);
				}
				static float fpsHistory[60] = {};
				static unsigned int currentFpsIndex = 0;
				fpsHistory[currentFpsIndex] = (float)mainloop.GetFPS();
				currentFpsIndex++;
				if (currentFpsIndex >= 60) currentFpsIndex = 0;
				ImGui::PlotLines(ig::ToString("FPS: ", mainloop.GetAvarageFPS()).c_str(), fpsHistory, IM_ARRAYSIZE(fpsHistory));

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();
}

void Draw()
{
	UpdateImgui();

	bool recordThisFrame = (isRecording && !universe.GetPaused());
	if (recordThisFrame)
	{
		if (readableTexture.GetWidth() != context.GetWidth() ||
			readableTexture.GetHeight() != context.GetHeight())
		{
			throw std::runtime_error("Readable texture size doesn't match back buffer size.");
		}
	}

	particleRenderer.Render(universe.ReadMP(), universe.ReadVP(), universe.GetNumMP(), universe.GetNumVP());

	ImGui::Render();
	ImDrawData* imDrawData = ImGui::GetDrawData();
	bool nothingToDraw = (imDrawData->CmdListsCount == 0 && !recordThisFrame);

	cmd.Begin();
	{
		cmd.SetRenderTarget(&context.GetBackBuffer());
		cmd.SetViewport((float)context.GetWidth(), (float)context.GetHeight());
		cmd.SetScissorRectangle(context.GetWidth(), context.GetHeight());

		// Record back buffer by copying it to a readable texture
		if (recordThisFrame)
		{
			cmd.AddTextureBarrier(context.GetBackBuffer(), ig::SimpleBarrier::RenderTarget, ig::SimpleBarrier::CopySource);
			cmd.FlushBarriers();

			cmd.CopyTexture(context.GetBackBuffer(), readableTexture);

			cmd.AddTextureBarrier(context.GetBackBuffer(), ig::SimpleBarrier::CopySource, ig::SimpleBarrier::RenderTarget);
			cmd.FlushBarriers();
		}

		// Draw imgui
		ImGui_ImplDX12_RenderDrawData(imDrawData, cmd.GetD3D12GraphicsCommandList());

		if (nothingToDraw)
		{
			cmd.AddTextureBarrier(context.GetBackBuffer(),
				ig::BarrierSync::None, ig::BarrierSync::None,
				ig::BarrierAccess::NoAccess, ig::BarrierAccess::NoAccess,
				ig::BarrierLayout::RenderTarget, ig::BarrierLayout::Common);
		}
		else
		{
			cmd.AddTextureBarrier(context.GetBackBuffer(), ig::SimpleBarrier::RenderTarget, ig::SimpleBarrier::Common, false);
		}
		cmd.FlushBarriers();
	}
	cmd.End();

	context.Submit(cmd);

	if (recordThisFrame)
	{
		// How to record each frame to a .PNG file:
		// Step 1: GPU copies the back buffer to a readable (AKA staging) texture for CPU read access.
		// Step 2: Wait for copy to finish.
		// Step 3: CPU reads the pixel values and writes to file.
		// This can either be done in parallel (fast), or it can be done the simple way.
		// Here, i'm doing it the simple way by just calling WaitForIdleDevice() every frame.
		context.WaitForIdleDevice();
		// We must read the texture contents before calling context.Present(),
		// because the contents of a Readable usage texture becomes stale the next frame.
		ig::Image image = readableTexture.ReadPixels();
		if (!ig::DirectoryExists(recordingsFolder))
		{
			ig::CreateDirectory(recordingsFolder);
		}
		image.SaveToFile(ig::ToString(recordingsFolder, "frame_", universe.GetFrame(), ".PNG"));
	}

	context.Present();
}

void OnDeviceRemoved()
{
	ig::PopupMessage("Device removal detected!\n"
		"This can happen if a GPU workload takes too long to finish.\n"
		"Try using less mass particles maybe?\n"
		"The app will now quit.", "Error", &context);
	mainloop.Quit();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShow)
{
	if (context.Load(
		ig::WindowSettings("Gravity Fun", 1280, 720, true, true),
		ig::RenderSettings(ig::PresentMode::Vsync, ig::Format::BYTE_BYTE_BYTE_BYTE, numFramesInFlight, numBackBuffers)))
	{
		context.SetOnDeviceRemovedCallback(OnDeviceRemoved);
		mainloop.SetMinimizedWindowBehaviour(ig::MainLoop::MinimizedWindowBehaviour::None);
		mainloop.Run(context, Start, OnLoopExited, Draw, Update, OnEvent);
	}
	return 0;
}

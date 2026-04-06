#include "gui_manager.hpp"

#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>
#include <imgui_stdlib.h>

GuiManager::GuiManager(SDL_GPUDevice *device, SDL_Window *window) {
	this->device = device;
	this->window = window;
}

GuiManager::~GuiManager() {}

void GuiManager::start() {
	// Dear ImGui
	IMGUI_CHECKVERSION();
	context = ImGui::CreateContext();

	ImGuiIO &io{ ImGui::GetIO() };

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

	io.Fonts->AddFontDefaultVector();

	io.Fonts->AddFontFromFileTTF("fonts/MPLUS1Code-Thin.ttf");
	io.Fonts->AddFontFromFileTTF("fonts/MPLUS1Code-ExtraLight.ttf");
	io.Fonts->AddFontFromFileTTF("fonts/MPLUS1Code-Light.ttf");
	io.Fonts->AddFontFromFileTTF("fonts/MPLUS1Code-Medium.ttf");
	io.Fonts->AddFontFromFileTTF("fonts/MPLUS1Code-Regular.ttf");
	io.Fonts->AddFontFromFileTTF("fonts/MPLUS1Code-SemiBold.ttf");
	io.Fonts->AddFontFromFileTTF("fonts/MPLUS1Code-Bold.ttf");

	io.Fonts->AddFontFromFileTTF("fonts/MPLUS2-Thin.ttf");
	io.Fonts->AddFontFromFileTTF("fonts/MPLUS2-ExtraLight.ttf");
	io.Fonts->AddFontFromFileTTF("fonts/MPLUS2-Light.ttf");
	io.Fonts->AddFontFromFileTTF("fonts/MPLUS2-Regular.ttf");
	io.Fonts->AddFontFromFileTTF("fonts/MPLUS2-Medium.ttf");
	io.Fonts->AddFontFromFileTTF("fonts/MPLUS2-SemiBold.ttf");
	io.Fonts->AddFontFromFileTTF("fonts/MPLUS2-Bold.ttf");
	io.Fonts->AddFontFromFileTTF("fonts/MPLUS2-ExtraBold.ttf");
	io.Fonts->AddFontFromFileTTF("fonts/MPLUS2-Black.ttf");

	// Styling
	f32 main_scale{ SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay()) };

	ImGuiStyle &style{ ImGui::GetStyle() };
	style.FontSizeBase = 16.0f;
	//style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	style.FontScaleDpi = main_scale; // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

	// Setup Platform/Renderer backends
	ImGui_ImplSDLGPU3_InitInfo init_info{
		.Device = device,
		.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(device, window),
		.MSAASamples = SDL_GPU_SAMPLECOUNT_1,
		.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
		.PresentMode = SDL_GPU_PRESENTMODE_VSYNC
	};

	ImGui_ImplSDL3_InitForSDLGPU(window);
	ImGui_ImplSDLGPU3_Init(&init_info);
}

void GuiManager::event(SDL_Event *event) {
	ImGui_ImplSDL3_ProcessEvent(event);

	if (event->type == SDL_EVENT_KEY_DOWN) {
		if (event->key.key == SDLK_F12) {
			show_editor = !show_editor;
		}
	}
}

void GuiManager::iterate(const f64 dt) {
	// Start the Dear ImGui frame
	ImGui_ImplSDL3_NewFrame();
	ImGui_ImplSDLGPU3_NewFrame();

	ImGui::NewFrame();

	// All Engine GUI goes in here
	if (show_editor) {
		// Menu Bar
		{
			ImGui::BeginMainMenuBar();
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Quit")) {
					SDL_Event quit_event;
					SDL_zero(quit_event);
					quit_event.type = SDL_EVENT_QUIT;
					quit_event.quit.timestamp = SDL_GetTicksNS();
					SDL_PushEvent(&quit_event);
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window")) {
				ImGui::MenuItem("Demo", nullptr, &show_demo_window);
				ImGui::MenuItem("Script", nullptr, &show_script_window);
				ImGui::MenuItem("Settings", nullptr, &show_settings_window);
				ImGui::EndMenu();
			}

			//ImGui::InputTextMultiline("##script", &script, ImVec2(F32_MIN, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput);

			ImGui::EndMainMenuBar();
		}

		// ImGui Demo window
		if (show_demo_window) {
			ImGui::ShowDemoWindow(&show_demo_window);
		}

		// Settings window
		if (show_settings_window) {
			ImGui::Begin("Display Settings", &show_settings_window, 0);

			static f64 accumulated_time{ 0.0 };
			accumulated_time += dt;
			ImGui::LabelText("Frame Time", "%.6fs", dt);

			static i32 present_mode{ 0 };
			const char *present_modes[]{ "VSync", "Immediate", "Mailbox" };
			ImGui::Combo("Present Mode", &present_mode, present_modes, IM_ARRAYSIZE(present_modes));

			static i32 swapchain_composition{ 0 };
			const char *swapchain_compositions[]{ "SDR", "SDR Linear", "HDR Extended Linear", "HDR10 ST2084" };
			ImGui::Combo("Swapchain Composition", &swapchain_composition, swapchain_compositions, IM_ARRAYSIZE(swapchain_compositions));

			static i32 max_frame_limit{ 60 };
			ImGui::SliderInt("Max Frame Limit", &max_frame_limit, 20, 240, "%dhz", ImGuiSliderFlags_None);

			if (ImGui::Button("Apply")) {
				SDL_SetGPUSwapchainParameters(device, window, (SDL_GPUSwapchainComposition)swapchain_composition, (SDL_GPUPresentMode)present_mode);

				if (present_mode == SDL_GPU_PRESENTMODE_IMMEDIATE) {
					// Enable frame limiter
				}
			}

			ImGui::End();
		}
	}

	// Rendering
	ImGui::Render();

	ImDrawData *draw_data = ImGui::GetDrawData();
	const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f or draw_data->DisplaySize.y <= 0.0f);

	SDL_GPUCommandBuffer *command_buffer = SDL_AcquireGPUCommandBuffer(device);
	SDL_GPUTexture *swapchain_texture;

	SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, window, &swapchain_texture, nullptr, nullptr); // Acquire a swapchain texture

	if (swapchain_texture and not is_minimized) {
		// This is mandatory: call ImGui_ImplSDLGPU3_PrepareDrawData() to upload the vertex/index buffer!
		ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);

		// Setup and start a render pass

		SDL_GPUColorTargetInfo target_info{
			.texture = swapchain_texture,
			.mip_level = 0,
			.layer_or_depth_plane = 0,
			.clear_color = SDL_FColor{ clear_color.x, clear_color.y, clear_color.z, clear_color.w },
			.load_op = SDL_GPU_LOADOP_CLEAR,
			.store_op = SDL_GPU_STOREOP_STORE,
			// resolve_texture,
			// resolve_mip_level,
			// resolve_layer,
			.cycle = false,
			// cycle_resolve_texture,
			// padding1,
			// padding2,
		};

		SDL_GPURenderPass *render_pass = SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);

		// Render ImGui
		ImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer, render_pass);

		SDL_EndGPURenderPass(render_pass);
	}

	// Submit the command buffer
	SDL_SubmitGPUCommandBuffer(command_buffer);
}

void GuiManager::quit() {
	ImGui_ImplSDL3_Shutdown();
	ImGui_ImplSDLGPU3_Shutdown();
	ImGui::DestroyContext();
}
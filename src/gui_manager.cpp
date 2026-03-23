#include "gui_manager.hpp"

#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>
#include <imgui_stdlib.h>

GuiManager::GuiManager(SDL_GPUDevice *device, SDL_Window *window) {
	this->device = device;
	this->window = window;
}

GuiManager::~GuiManager() {
	SDL_Log("GuiManager destructor\n");
}

void GuiManager::start() {
	// Dear ImGui
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

		io.Fonts->AddFontDefaultVector();

		// Styling
		f32 main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

		ImGuiStyle &style = ImGui::GetStyle();
		style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
		style.FontScaleDpi = main_scale; // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

		// Setup Platform/Renderer backends
		ImGui_ImplSDL3_InitForSDLGPU(window);

		ImGui_ImplSDLGPU3_InitInfo init_info = {};
		init_info.Device = device;
		init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(device, window);
		init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
		init_info.SwapchainComposition = SDL_GPU_SWAPCHAINCOMPOSITION_SDR;
		init_info.PresentMode = SDL_GPU_PRESENTMODE_VSYNC;

		ImGui_ImplSDLGPU3_Init(&init_info);
	}
	// RmlUi
	{
		// Rml::SetRenderInterface(nullptr);
		// Rml::SetSystemInterface(nullptr);

		// Rml::Initialise();
	}
}

void GuiManager::event(SDL_Event *event) {
	ImGui_ImplSDL3_ProcessEvent(event);
}

void GuiManager::iterate(const f64 dt) {
	// Start the Dear ImGui frame
	ImGui_ImplSDLGPU3_NewFrame();
	ImGui_ImplSDL3_NewFrame();

	ImGui::NewFrame();

	{
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
	}

	{
		// Display Settings
		ImGui::Begin("Display Settings");

		ImGui::LabelText("", "%.6f", dt);

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

	{
		ImGui::Begin("Script Editor", &show_script_window, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				ImGui::MenuItem("Open...", "Ctrl+O");
				ImGui::MenuItem("Save", "Ctrl+S");
				ImGui::MenuItem("Close", "Ctrl+W");

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (ImGui::Button("Run")) {
			// run script
			//ScriptManager::run_script(script);
			SDL_Log("Running script:\n%s\n", script.c_str());
		}

		ImGui::InputTextMultiline("##editor", &script, ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_AllowTabInput);

		ImGui::End();
	}

	// Rendering
	ImGui::Render();

	ImDrawData *draw_data = ImGui::GetDrawData();
	const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);

	SDL_GPUCommandBuffer *command_buffer = SDL_AcquireGPUCommandBuffer(device); // Acquire a GPU command buffer

	SDL_GPUTexture *swapchain_texture;
	SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer, window, &swapchain_texture, nullptr, nullptr); // Acquire a swapchain texture

	if (swapchain_texture != nullptr && !is_minimized) {
		// This is mandatory: call ImGui_ImplSDLGPU3_PrepareDrawData() to upload the vertex/index buffer!
		ImGui_ImplSDLGPU3_PrepareDrawData(draw_data, command_buffer);

		// Setup and start a render pass

		SDL_GPUColorTargetInfo target_info = {};

		target_info.texture = swapchain_texture;
		target_info.clear_color = SDL_FColor{ clear_color.x, clear_color.y, clear_color.z, clear_color.w };
		target_info.load_op = SDL_GPU_LOADOP_CLEAR;
		target_info.store_op = SDL_GPU_STOREOP_STORE;
		target_info.mip_level = 0;
		target_info.layer_or_depth_plane = 0;
		target_info.cycle = false;

		SDL_GPURenderPass *render_pass = SDL_BeginGPURenderPass(command_buffer, &target_info, 1, nullptr);

		// Render ImGui
		ImGui_ImplSDLGPU3_RenderDrawData(draw_data, command_buffer, render_pass);

		SDL_EndGPURenderPass(render_pass);
	}

	// Submit the command buffer
	SDL_SubmitGPUCommandBuffer(command_buffer);
}

void GuiManager::quit() {
	ImGui_ImplSDLGPU3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
}
#pragma once

#include <SDL3/SDL.h>
#include <imgui.h>

#include <string>

#include "types.hpp"

class GuiManager {
private:
	SDL_GPUDevice *device{ nullptr };
	SDL_Window *window{ nullptr };
	
	ImGuiContext *context{ nullptr };

	ImVec4 clear_color{ ImColor(63, 63, 63) };

	bool show_editor{ true };

	
	bool show_demo_window{ false };
	bool show_script_window{ false };
	bool show_settings_window{ false };
	
	std::string script{ "" };

public:
	GuiManager(SDL_GPUDevice *device, SDL_Window *window);
	~GuiManager();

	void start();
	void event(SDL_Event *event);
	void iterate(const f64 dt);
	void quit();
};
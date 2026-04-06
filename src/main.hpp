#pragma once

#define PROJECT_NAME "Softplayer"

#define NS_PER_SEC 1000000000

#include <SDL3/SDL.h>

#include "gui_manager.hpp"
#include "script_manager.hpp"
#include "types.hpp"

struct AppState {
	GuiManager *gui_manager{ nullptr };
	ScriptManager *script_manager{ nullptr };

	SDL_Window *window{ nullptr };
	SDL_GPUDevice *device{ nullptr };

	u64 last_tick{ 0 };
	u64 last_fixed{ 0 };
	u64 accumulator{ 0 };
	u64 fixed_delta{ NS_PER_SEC / 60 };
	u64 min_delta{ NS_PER_SEC / 240 }; // Default hz limit
};


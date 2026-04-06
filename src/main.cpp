#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL_main.h>

#include "main.hpp"
#include "shader.hpp"

f64 get_avg_frame_time() {
	return 1.0;
}

SDL_AppResult SDL_AppInit(void **appstate, i32 argc, char *argv[]) {
	AppState *state = new AppState;

	*appstate = state;

	if (!state) {
		return SDL_APP_FAILURE;
	}

	bool result;

	SDL_InitFlags init_flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD;
	result = SDL_Init(init_flags);
	if (!result) {
		SDL_Log("%s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
	state->window = SDL_CreateWindow(PROJECT_NAME, 1920, 1080, window_flags);
	if (!state->window) {
		SDL_Log("%s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_GPUShaderFormat format_flags = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL;
	state->device = SDL_CreateGPUDevice(format_flags, true, NULL);
	if (!state->device) {
		SDL_Log("%s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	SDL_Log("GPU Driver: %s", SDL_GetGPUDeviceDriver(state->device));

	result = SDL_ClaimWindowForGPUDevice(state->device, state->window);
	if (!result) {
		SDL_Log("%s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	state->gui_manager = new GuiManager(state->device, state->window);
	state->gui_manager->start();

	state->script_manager = new ScriptManager();
	state->script_manager->start();

	state->last_tick = SDL_GetTicksNS();
	state->last_fixed = SDL_GetTicksNS();

	// shader_test(state->device);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	AppState *state = static_cast<AppState *>(appstate);

	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}

	state->gui_manager->event(event);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
	AppState *state = static_cast<AppState *>(appstate);

	u64 tick = SDL_GetTicksNS();
	u64 delta = tick - state->last_tick;
	state->last_tick = tick;

	state->accumulator += delta;

	// fixed_iterate(dt) calls
	while (state->accumulator >= state->fixed_delta) {
		// fixed iterate
		state->accumulator -= state->fixed_delta;
		f64 dt = state->fixed_delta * 1e-9;
		// fixed_iterate(dt)
	}

	// iterate(dt) calls
	f64 dt = delta * 1e-9;

	state->gui_manager->iterate(dt);

	// frame limiting
	u64 frame_end = SDL_GetTicksNS();
    u64 frame_time = frame_end - tick;  // total time this frame actually took

    if (frame_time < state->min_delta) {
        SDL_DelayNS(state->min_delta - frame_time);
    }

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
	AppState *state = static_cast<AppState *>(appstate);

	SDL_WaitForGPUIdle(state->device);

	state->gui_manager->quit();
	state->script_manager->quit();

	SDL_ReleaseWindowFromGPUDevice(state->device, state->window);
	SDL_DestroyGPUDevice(state->device);
	SDL_DestroyWindow(state->window);

	delete state->gui_manager;
	delete state->script_manager;

	delete state;

	SDL_Quit();
}
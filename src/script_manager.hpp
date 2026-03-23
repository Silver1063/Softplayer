#pragma once

#include "types.hpp"

#include <SDL3/SDL.h>

#include <angelscript.h>

#include <string>

class ScriptManager {
private:
	asIScriptEngine *engine;
	// std::vector<asIScriptModule> *modules;
	asIScriptContext *context;

public:
	ScriptManager();
	~ScriptManager();

	void start();
	void event(SDL_Event *event);
	void iterate(const f64 dt);
	void quit();


	void run(const std::string &script);

	static void message_callback(const asSMessageInfo *msg, void *param);
};
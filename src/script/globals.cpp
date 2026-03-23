#include "globals.hpp"
#include "../types.hpp"

#include <SDL3/SDL.h>

#include <cassert>
#include <string>

void print(const std::string &str) {
	SDL_Log("%s", str.c_str());
}

void register_globals(asIScriptEngine *engine) {
	i32 r;
	r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);
	assert(r >= 0);
}
#include "addons.hpp"

#include <angelscript/scriptstdstring/scriptstdstring.h>

void register_addons(asIScriptEngine *engine) {
	RegisterStdString(engine);
}
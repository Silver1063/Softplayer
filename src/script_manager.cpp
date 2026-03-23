#include "script_manager.hpp"
#include "script/addons.hpp"
#include "script/globals.hpp"

#include <angelscript/scriptbuilder/scriptbuilder.h>

#include <cassert>

ScriptManager::ScriptManager() {
	engine = asCreateScriptEngine();
	context = engine->CreateContext();
}

ScriptManager::~ScriptManager() {}

void ScriptManager::start() {
	i32 r = engine->SetMessageCallback(asFUNCTION(message_callback), 0, asCALL_CDECL);
	assert(r >= 0);

	register_addons(engine);
	register_globals(engine);
}

void ScriptManager::iterate(const f64 dt) {}

void ScriptManager::quit() {
	context->Release();
	engine->ShutDownAndRelease();
}

void ScriptManager::run(const std::string &script) {
	// The CScriptBuilder helper is an add-on that loads the file,
	// performs a pre-processing pass if necessary, and then tells
	// the engine to build a script module.
	CScriptBuilder builder;
	i32 r = builder.StartNewModule(engine, "RunModule");
	if (r < 0) {
		// If the code fails here it is usually because there
		// is no more memory to allocate the module
		printf("Unrecoverable error while starting a new module.\n");
		return;
	}
	r = builder.AddSectionFromMemory("RunScript", script.c_str());
	if (r < 0) {
		// The builder wasn't able to load the file. Maybe the file
		// has been removed, or the wrong name was given, or some
		// preprocessing commands are incorrectly written.
		printf("Please correct the errors in the script and try again.\n");
		return;
	}
	r = builder.BuildModule();
	if (r < 0) {
		// An error occurred. Instruct the script writer to fix the
		// compilation errors that were listed in the output stream.
		printf("Please correct the errors in the script and try again.\n");
		return;
	}

	asIScriptModule *module = engine->GetModule("RunModule");
	asIScriptFunction *function = module->GetFunctionByDecl("void main()");
	if (function == 0) {
		// The function couldn't be found. Instruct the script writer
		// to include the expected function in the script.
		printf("The script must have the function 'void main()'. Please add it and try again.\n");
		return;
	}

	context->Prepare(function);

	r = context->Execute();

	if (r != asEXECUTION_FINISHED) {
		// The execution didn't complete as expected. Determine what happened.
		if (r == asEXECUTION_EXCEPTION) {
			// An exception occurred, let the script writer know what happened so it can be corrected.
			printf("An exception '%s' occurred. Please correct the code and try again.\n", context->GetExceptionString());
		}
	}

	module->Discard();
}

// Implement a simple message callback function
void ScriptManager::message_callback(const asSMessageInfo *msg, void *param) {
	const char *type = "ERR ";
	if (msg->type == asMSGTYPE_WARNING)
		type = "WARN";
	else if (msg->type == asMSGTYPE_INFORMATION)
		type = "INFO";
	SDL_Log("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}
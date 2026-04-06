#include "shader.hpp"

#include <SDL3_shadercross/SDL_shadercross.h>

#include "types.hpp"
#include <cstddef>

void shader_test(SDL_GPUDevice *device) {
	SDL_Log("Running shader test...");

	if (!SDL_ShaderCross_Init()) {
		SDL_Log("SDL_ShaderCross_Init failed: %s", SDL_GetError());
		return;
	}
	SDL_Log("SDL_ShaderCross_Init succeeded");

	const char *hlsl_src = R"(
struct VertexInput {
    float3 position : POSITION;
    float2 uv : TEXCOORD0;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float4 vertex(VertexInput input) : SV_Position
{
    return float4(input.position, 1.0);
})";

	SDL_ShaderCross_HLSL_Info hlsl_info{ hlsl_src, "vertex", nullptr, nullptr, SDL_SHADERCROSS_SHADERSTAGE_VERTEX, 0 };

	usize bytecode_size;
	void *bytecode = SDL_ShaderCross_CompileSPIRVFromHLSL(&hlsl_info, &bytecode_size);

	if (!bytecode) {
		SDL_Log("HLSL compilation failed: %s", SDL_GetError());
		SDL_ShaderCross_Quit();
		return;
	}

	SDL_ShaderCross_SPIRV_Info spirv_info{ (u8 *)bytecode, bytecode_size, "vertex", SDL_SHADERCROSS_SHADERSTAGE_VERTEX, 0 };

	SDL_ShaderCross_GraphicsShaderMetadata *metadata = SDL_ShaderCross_ReflectGraphicsSPIRV((u8 *)bytecode, bytecode_size, 0);

	if (!metadata) {
		SDL_free(bytecode);

		SDL_Log("SPIR-V reflection failed: %s", SDL_GetError());
		SDL_ShaderCross_Quit();
		return;
	}

	SDL_GPUShader *shader = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV(device, &spirv_info, &metadata->resource_info, 0);

	if (!shader) {
		SDL_free(bytecode);
		SDL_free(metadata);

		SDL_Log("HLSL compilation failed: %s", SDL_GetError());
		SDL_ShaderCross_Quit();
		return;
	}

	SDL_Log("HLSL compiled successfully!");
	SDL_Log("  Samplers:         %u", metadata->resource_info.num_samplers);
	SDL_Log("  Storage textures: %u", metadata->resource_info.num_storage_textures);
	SDL_Log("  Storage buffers:  %u", metadata->resource_info.num_storage_buffers);
	SDL_Log("  Uniform buffers:  %u", metadata->resource_info.num_uniform_buffers);

	SDL_free(bytecode);
	SDL_free(metadata);

	SDL_ReleaseGPUShader(device, shader);
	SDL_Log("Shader released cleanly.");

	SDL_ShaderCross_Quit();
	SDL_Log("Shader test complete.");
}
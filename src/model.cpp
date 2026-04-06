#include "model.hpp"

#include <fastgltf/core.hpp>
#include <fastgltf/types.hpp>

void model_test(std::filesystem::path path) {
	fastgltf::Parser parser;
	auto data_buffer = fastgltf::GltfDataBuffer::FromPath(path);
	auto asset = parser.loadGltf(data_buffer.get(), ".");
	fastgltf::Asset &gltf = asset.get();
	
    for (auto &mesh : gltf.meshes) {
		for (auto &prim : mesh.primitives) {
			// index buffer
			if (prim.indicesAccessor.has_value()) {
				auto &accessor = gltf.accessors[prim.indicesAccessor.value()];
				// accessor.count, accessor.componentType, accessor.type
			}

			// vertex attributes
			for (auto &[name, index] : prim.attributes) {
				auto &accessor = gltf.accessors[index];
				if (name == "POSITION") {
					//
				}
				if (name == "NORMAL") {
					//
				}
				if (name == "TEXCOORD_0") {
					//
				}
			}
		}
	}
}
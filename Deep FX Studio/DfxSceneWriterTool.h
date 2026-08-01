#pragma once

#include <string>
#include <algorithm>
#include <fstream>
#include <vector>

namespace dfxscenewriter {

	namespace tools {

		namespace syntax {

			static std::string SceneDeclar = "Scene";
			static std::string NodeDeclar = "Node";
			static std::string LayerDeclar = "Layer";
			static std::string MeshDataDeclar = "MeshData";
			static std::string ConnectionDeclar = "Connection";

			static std::string _intDeclar    = "$int";
			static std::string _size_tDeclar = "$size_t";
			static std::string _floatDeclar  = "$float";
			static std::string _stringDeclar = "$string";
			static std::string _meshDeclar   = "$mesh";
		}

		std::string qreplace(std::string str, char from, char to);

		class SceneWriter {
		private:
			std::ofstream *file;
			int indent_level;
		public:
			SceneWriter(std::ofstream *);

			void beginSceneObject(std::string);
			void beginNodeObject(std::string);
			void beginLayerObject(std::string);
			void beginMeshObject(std::string);
			void beginConnectionObject(std::string);
			void beginArray(size_t size);
			void endArray();
			
			void writeInt(std::string name, int value);
			void writeSize_t(std::string name, size_t value);
			void writeFloat(std::string name, float value);
			void writeString(std::string name, std::string value);
			void writeMesh(std::string name, std::string value);
			void writeArrayValue(float);

			void endObject();

			void indent();
		};
	}
}
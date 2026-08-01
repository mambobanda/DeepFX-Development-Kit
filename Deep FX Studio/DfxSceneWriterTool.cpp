#include "DfxSceneWriterTool.h"

namespace dfxscenewriter {

	namespace tools {

		std::string qreplace(std::string str, char from, char to) {
			std::string temp = str;
			std::replace(temp.begin(), temp.end(), from, to);
			return temp;
		}

		SceneWriter::SceneWriter(std::ofstream *file) {
			indent_level = 0;
			this->file = file;
		}

		void SceneWriter::beginSceneObject(std::string name) {
			indent();
			*file << syntax::SceneDeclar << " #" << name << " {\n\n";
			indent_level++;
		}

		void SceneWriter::beginNodeObject(std::string name) {
			indent();
			*file << syntax::NodeDeclar << " #" << name << " {\n\n";
			indent_level++;
		}

		void SceneWriter::beginLayerObject(std::string name) {
			indent();
			*file << syntax::LayerDeclar << " #" << name << " {\n\n";
			indent_level++;
		}

		void SceneWriter::beginMeshObject(std::string name) {
			indent();
			*file << syntax::MeshDataDeclar << " #" << name << " {\n\n";
			indent_level++;
		}

		void SceneWriter::beginConnectionObject(std::string name) {
			indent();
			*file << syntax::ConnectionDeclar << " #" << name << " {\n\n";
			indent_level++;
		}

		void SceneWriter::writeInt(std::string name, int value) {
			indent();
			*file << syntax::_intDeclar << " " << name << ": "  << value <<";\n\n";
		}

		void SceneWriter::writeSize_t(std::string name, size_t value) {
			indent();
			*file << syntax::_size_tDeclar << " " << name << ": " << value <<";\n\n";
		}

		void SceneWriter::writeFloat(std::string name, float value) {
			indent();
			*file << syntax::_floatDeclar << " " << name << ": " << value <<";\n\n";
		}

		void SceneWriter::writeString(std::string name, std::string value) {
			indent();
			*file << syntax::_stringDeclar << " " << name << ": " << value <<";\n\n";
		}

		void SceneWriter::writeMesh(std::string name, std::string value) {
			indent();
			*file << syntax::_meshDeclar << " " << name << ": " << value <<";\n\n";
		}

		void SceneWriter::endObject() {
			indent_level--;
			indent();
			*file << "}\n\n";
		}

		void SceneWriter::indent() {
			for (int i = 0; i < indent_level; i++) *file << "\t";
		}
	}
}
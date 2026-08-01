/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2020 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include "VAttribute.h"
#include "VBuffer.h"
#include "RJDocument.h"
#include "RJNode.h"
#include "VH5Attribute.h"
#include "VH5Buffer.h"

namespace xvault {

	namespace rapid {

		rjDocument::rjDocument() {
			_root = new rapid::rjNode("root");
		}

		rjDocument::~rjDocument() {
			delete _root;
		}

		Node *rjDocument::root() {
			return _root;
		}

		std::string rjDocument::toString(Node *node, int level) {

			std::stringstream ss;

			std::string td = "";
			for (int inx = 0; inx < level; inx++) td += "\t";

			auto attr = node->attributes().begin();
			auto buff = node->arrays().begin();
			auto nod = node->nodes().begin();

			for (; attr != node->attributes().end();) {

				if ((*attr)->type() == type::INT) {
					ss << td << "\"" << (*attr)->name() << "\":" << std::to_string((*attr)->valueInt());
				}
				else if ((*attr)->type() == type::UNSIGNED_INT) {
					ss << td << "\"" << (*attr)->name() << "\":" << std::to_string((*attr)->valueUInt());
				}
				else if ((*attr)->type() == type::LONG) {
					ss << td << "\"" << (*attr)->name() << "\":" << std::to_string((*attr)->valueLong());
				}
				else if ((*attr)->type() == type::UNSIGNED_LONG) {
					ss << td << "\"" << (*attr)->name() << "\":" << std::to_string((*attr)->valueULong());
				}
				else if ((*attr)->type() == type::LONG_LONG) {
					ss << td << "\"" << (*attr)->name() << "\":" << std::to_string((*attr)->valueLongLong());
				}
				else if ((*attr)->type() == type::UNSIGNED_LONG_LONG) {
					ss << td << "\"" << (*attr)->name() << "\":" << std::to_string((*attr)->valueULongLong());
				}
				else if ((*attr)->type() == type::FLOAT) {
					ss << td << "\"" << (*attr)->name() << "\":" << std::to_string((*attr)->valueFloat());
				}
				else if ((*attr)->type() == type::DOUBLE) {
					ss << td << "\"" << (*attr)->name() << "\":" << std::to_string((*attr)->valueDouble());
				}
				else if ((*attr)->type() == type::BOOL) {
					ss << td << "\"" << (*attr)->name() << "\":" << (*attr)->valueString();
				}
				else if ((*attr)->type() == type::STRING) {
					ss << td << "\"" << (*attr)->name() << "\":" << "\"" << (*attr)->valueString() << "\"";
				}
				else if ((*attr)->type() == type::FLOAT_2) {
					auto fl2 = (*attr)->valueFloat2();
					ss << std::fixed << td << "\"" << (*attr)->name() << "\": ["
						<< fl2.x << ", " << fl2.y 
						<< "]";
				}
				else if ((*attr)->type() == type::FLOAT_3) {
					auto fl3 = (*attr)->valueFloat3();
					ss << std::fixed << td << "\"" << (*attr)->name() << "\": ["
						<< fl3.x << ", " << fl3.y << ", " << fl3.z
						<< "]";
				}
				else if ((*attr)->type() == type::FLOAT_4) {
					auto fl4 = (*attr)->valueFloat4();
					ss << std::fixed << td << "\"" << (*attr)->name() << "\": ["
						<< fl4.x << ", " << fl4.y << ", " << fl4.z << ", " << fl4.w
						<< "]";
				}
				else if ((*attr)->type() == type::FLOAT_9) {
					auto fl9 = (*attr)->valueFloat9();
					float* m = glm::value_ptr(fl9);
					ss << std::fixed << td << "\"" << (*attr)->name() << "\": ["
						<< m[0] << ", " << m[1] << ", " << m[2] 
						<< m[3] << ", " << m[4] << ", " << m[5] 
						<< m[6] << ", " << m[7] << ", " << m[8] 
						<< "]";
				}
				else if ((*attr)->type() == type::FLOAT_16) {
					auto fl16 = (*attr)->valueFloat16();
					float* m = glm::value_ptr(fl16);
					ss << std::fixed << td << "\"" << (*attr)->name() << "\": ["
						<< m[0] << ", " << m[1] << ", " << m[2] << ", " << m[3]
						<< m[4] << ", " << m[5] << ", " << m[6] << ", " << m[7]
						<< m[8] << ", " << m[9] << ", " << m[10] << ", " << m[11]
						<< m[12] << ", " << m[13] << ", " << m[14] << ", " << m[15]
						<< "]";
				}

				attr++;
				if (attr != node->attributes().end()) ss << ",";
				if (attr != node->attributes().end())ss << "\n";

			}

			if (!node->attributes().empty()) {
				if (!node->arrays().empty()) ss << ",\n";
			}

			for (; buff != node->arrays().end();) {

				ss << td << "\"" << (*buff)->name() << "\": [" << std::endl;
				if ((*buff)->type() == type::INT) {

					for (int i = 0; i < (*buff)->bufferSize(); i++) {
						if (i == 0) ss << td;
						ss << (*buff)->bufferInt()[i];
						if (i != ((*buff)->bufferSize() - 1)) ss << ", ";
					}

				}
				else if ((*buff)->type() == type::UNSIGNED_INT) {

					for (int i = 0; i < (*buff)->bufferSize(); i++) {
						if (i == 0) ss << td;
						ss << (*buff)->bufferUInt()[i];
						if (i != ((*buff)->bufferSize() - 1)) ss << ", ";
					}

				}
				else if ((*buff)->type() == type::LONG) {

					for (int i = 0; i < (*buff)->bufferSize(); i++) {
						if (i == 0) ss << td;
						ss << (*buff)->bufferLong()[i];
						if (i != ((*buff)->bufferSize() - 1)) ss << ", ";
					}

				}
				else if ((*buff)->type() == type::UNSIGNED_LONG) {

					for (int i = 0; i < (*buff)->bufferSize(); i++) {
						if (i == 0) ss << td;
						ss << (*buff)->bufferULong()[i];
						if (i != ((*buff)->bufferSize() - 1)) ss << ", ";
					}

				}
				else if ((*buff)->type() == type::LONG_LONG) {

					for (int i = 0; i < (*buff)->bufferSize(); i++) {
						if (i == 0) ss << td;
						ss << (*buff)->bufferLongLong()[i];
						if (i != ((*buff)->bufferSize() - 1)) ss << ", ";
					}

				}
				else if ((*buff)->type() == type::UNSIGNED_LONG_LONG) {

					for (int i = 0; i < (*buff)->bufferSize(); i++) {
						if (i == 0) ss << td;
						ss << (*buff)->bufferULongLong()[i];
						if (i != ((*buff)->bufferSize() - 1)) ss << ", ";
					}

				}
				else if ((*buff)->type() == type::FLOAT) {

					for (int i = 0; i < (*buff)->bufferSize(); i++) {
						if (i == 0) ss << td;
						ss << (*buff)->bufferFloat()[i];
						if (i != ((*buff)->bufferSize() - 1)) ss << ", ";
					}

				}
				else if ((*buff)->type() == type::DOUBLE) {

					for (int i = 0; i < (*buff)->bufferSize(); i++) {
						if (i == 0) ss << td;
						ss << (*buff)->bufferDouble()[i];
						if (i != ((*buff)->bufferSize() - 1)) ss << ", ";
					}

				}
				else if ((*buff)->type() == type::BOOL) {

					for (int i = 0; i < (*buff)->bufferSize(); i++) {
						if (i == 0) ss << td;
						ss << (*buff)->bufferInt()[i];
						if (i != ((*buff)->bufferSize() - 1)) ss << ", ";
					}

				}
				ss << std::endl << td << "]";

				buff++;
				if (buff != node->arrays().end()) ss << ",";
				if (buff != node->arrays().end()) ss << "\n";

			}

			if (!node->arrays().empty() || !node->attributes().empty()) {
				if (!node->nodes().empty()) ss << ",\n";
			}

			for (; nod != node->nodes().end();) {

				ss << td << "\"" << (*nod)->name() << "\": {" << std::endl;
				ss << toString((*nod), level + 1);
				ss << std::endl << td << "}";

				nod++;
				if (nod != node->nodes().end()) ss << ",";
				if (nod != node->nodes().end()) ss << "\n";

			}

			return ss.str();
		}

		void rjDocument::addAttribute(Attribute *attr) {
			((rjNode*)_root)->addAttribute(attr);
		}

		void rjDocument::addBuffer(Buffer *buffer) {
			((rjNode*)_root)->addBuffer(buffer);
		}

		void rjDocument::addNode(Node *node) {
			((rjNode*)_root)->addNode(node);
		}

		std::string rjDocument::toString() {
			std::stringstream ss;

			ss << "{\n";
			ss << toString(_root, 1);
			ss << "\n}\n";

			return ss.str();
		}

		bool rjDocument::fromString(std::string doc, std::string &errorMsg) {

			rapidjson::Document jdoc;
			rapidjson::ParseResult ok = jdoc.Parse<rapidjson::kParseNanAndInfFlag>(doc.c_str());
		
			if (ok) {

				for (rapidjson::Value::ConstMemberIterator itr = jdoc.MemberBegin(); itr != jdoc.MemberEnd(); ++itr) {
					fromJson(itr, root());
				}

				return true;
			}
			else {
				std::stringstream ss;
				auto emsg = std::string(GetParseError_En(ok.Code()));
				ss << "JSON parse error: " << emsg << " (" << ok.Offset() << ")\n";
				ss << doc.substr(ok.Offset(), 50) << ".......\n";
				errorMsg = ss.str();
			}

			return false;
		}

		void rjDocument::fromJson(rapidjson::Value::ConstMemberIterator &obj, Node *parent) {

			if (obj->value.IsObject()) {

				xvault::Node *c_node = new xvault::rapid::rjNode(obj->name.GetString());

				for (rapidjson::Value::ConstMemberIterator itr = obj->value.MemberBegin(); itr != obj->value.MemberEnd(); ++itr) {
					fromJson(itr, c_node);
				}

				parent->nodes().push_back(c_node);

			}
			else if (obj->value.IsArray()) {

				if (obj->value.Size() == 2) {

					glm::vec2 fl2(obj->value[0].GetFloat(), obj->value[1].GetFloat());
					auto attr00 = xvault::h5::h5Attribute::newValueFloat2(obj->name.GetString(), xvault::type::FLOAT_2, fl2);
					parent->attributes().push_back(attr00);

				}
				else if (obj->value.Size() == 3) {

					glm::vec3 fl3(obj->value[0].GetFloat(), obj->value[1].GetFloat(), obj->value[2].GetFloat());
					auto attr00 = xvault::h5::h5Attribute::newValueFloat3(obj->name.GetString(), xvault::type::FLOAT_3, fl3);
					parent->attributes().push_back(attr00);

				}
				else if (obj->value.Size() == 4) {

					glm::vec4 fl4(obj->value[0].GetFloat(), obj->value[1].GetFloat(), obj->value[2].GetFloat(), obj->value[3].GetFloat());
					auto attr00 = xvault::h5::h5Attribute::newValueFloat4(obj->name.GetString(), xvault::type::FLOAT_4, fl4);
					parent->attributes().push_back(attr00);

				}
				else if (obj->value.Size() == 9) {
					glm::mat3 fl9(
						obj->value[0].GetFloat(), obj->value[1].GetFloat(), obj->value[2].GetFloat(),
						obj->value[3].GetFloat(), obj->value[4].GetFloat(), obj->value[5].GetFloat(),
						obj->value[6].GetFloat(), obj->value[7].GetFloat(), obj->value[8].GetFloat());

					auto attr00 = xvault::h5::h5Attribute::newValueFloat9(obj->name.GetString(), xvault::type::FLOAT_9, fl9);
					parent->attributes().push_back(attr00);
				}
				else if (obj->value.Size() == 16) {
					glm::mat4 fl16(
						obj->value[0].GetFloat(), obj->value[1].GetFloat(), obj->value[2].GetFloat(), obj->value[3].GetFloat(),
						obj->value[4].GetFloat(), obj->value[5].GetFloat(), obj->value[6].GetFloat(), obj->value[7].GetFloat(),
						obj->value[8].GetFloat(), obj->value[9].GetFloat(), obj->value[10].GetFloat(), obj->value[11].GetFloat(),
						obj->value[12].GetFloat(), obj->value[13].GetFloat(), obj->value[14].GetFloat(), obj->value[15].GetFloat());

					auto attr00 = xvault::h5::h5Attribute::newValueFloat16(obj->name.GetString(), xvault::type::FLOAT_16, fl16);
					parent->attributes().push_back(attr00);
				}
				
			}
			else {

				if (obj->value.IsBool()) {
					
					auto attr00 = xvault::h5::h5Attribute::newValueInt(obj->name.GetString(), xvault::type::BOOL, obj->value.GetBool() ? 1 : 0);
					parent->attributes().push_back(attr00);
					
				}
				else if (obj->value.IsDouble()) {	

					auto attr00 = xvault::h5::h5Attribute::newValueDouble(obj->name.GetString(), xvault::type::DOUBLE, obj->value.GetDouble());
					parent->attributes().push_back(attr00);
					
				}
				else if (obj->value.IsFloat()) {

					auto attr00 = xvault::h5::h5Attribute::newValueFloat(obj->name.GetString(), xvault::type::FLOAT, obj->value.GetFloat());
					parent->attributes().push_back(attr00);

				}
				else if (obj->value.IsUint()) {

					auto attr00 = xvault::h5::h5Attribute::newValueUInt(obj->name.GetString(), xvault::type::UNSIGNED_INT, obj->value.GetUint());
					parent->attributes().push_back(attr00);
		
				}
				else if (obj->value.IsInt()) {
		
					auto attr00 = xvault::h5::h5Attribute::newValueInt(obj->name.GetString(), xvault::type::INT, obj->value.GetInt());
					parent->attributes().push_back(attr00);
				
				}
				else if (obj->value.IsUint64()) {
				
					auto attr00 = xvault::h5::h5Attribute::newValueULongLong(obj->name.GetString(), xvault::type::UNSIGNED_LONG_LONG, (unsigned long long)obj->value.GetUint64());
					parent->attributes().push_back(attr00);

				}
				else if (obj->value.IsUint64()) {

					auto attr00 = xvault::h5::h5Attribute::newValueULongLong(obj->name.GetString(), xvault::type::LONG_LONG, (long long)obj->value.GetInt64());
					parent->attributes().push_back(attr00);
	
				}
				else if (obj->value.IsString()) {

					auto attr00 = xvault::h5::h5Attribute::newValueString(obj->name.GetString(), xvault::type::STRING, obj->value.GetString());
					parent->attributes().push_back(attr00);

				}

			}

		}
	}
}
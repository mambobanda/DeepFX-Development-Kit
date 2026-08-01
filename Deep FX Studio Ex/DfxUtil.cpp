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

#include <DfxGlobal.h>
#include <RJNode.h>
#include <RJDocument.h>
#include <FlDirectedAcyclicGraph.h>
#include <DsTools.h>
#include "DfxBounds.h"
#include "DfxResourceManager.h"
#include "DfxUtil.h"
#include "DfxItemWidgetType.h"
#include "DfxResource.h"
#include "DfxResourcePtr.h"
#include "DfxMaterialLinker.h"
#include "DfxCacheTrackManager.h"

namespace util {

	deepsg::SgGeoNode *createEmptyPrimitive() {
		auto mesh = dstools::newStandardMesh();

		auto mshResource = new DfxResource(resource::mode::RESOURCE, resource::type::GEOMETRY, "");
		mshResource->setResource(mesh);
		mshResource->setFormat(resource::format::DS_MESH);
		mshResource->setReleaseCallback(DfxObjectHandler::releaseDeepSimplexResource);
		mshResource->setName("Empty");

		DfxResourceManager::getInstance()->geo()->addChild(mshResource);

		DfxObjectHandler obj_handler;
		bool rc = obj_handler.create(mshResource->newPointer(), "", "Empty");

		auto gnode = obj_handler.createGeoNode();

		return gnode;
	}

	std::string replaceAll(std::string str, std::string substring, std::string replace) {
		QString modString(str.c_str());
		return modString.replace(QString(substring.c_str()), QString(replace.c_str())).toStdString();
	}

	std::string spaceTypeToString(unsigned int typ) {

		switch (typ)
		{
		case fl::type::ANY:
			return "any";
			break;
		case fl::type::ATTRIBUTE_PATH:
			return "attribute_Path";
			break;
		case fl::type::BOOL:
			return "bool";
			break;
		case fl::type::CHAR:
			return "char";
			break;
		case fl::type::COMPOUND_PORT:
			return "compound_port";
			break;
		case fl::type::DATA_OBJECT:
			return "data_object";
			break;
		case fl::type::DOUBLE:
			return "double";
			break;
		case fl::type::DOUBLE_2:
			return "double2";
			break;
		case fl::type::DOUBLE_3:
			return "double3";
			break;
		case fl::type::DOUBLE_4:
			return "double4";
			break;
		case fl::type::FLOAT:
			return "float";
			break;
		case fl::type::FLOAT_2:
			return "float2";
			break;
		case fl::type::FLOAT_3:
			return "float3";
			break;
		case fl::type::FLOAT_4:
			return "float4";
			break;
		case fl::type::FLOW:
			return "flow";
			break;
		case fl::type::INT:
			return "int";
			break;
		case fl::type::INT_2:
			return "int2";
			break;
		case fl::type::INT_3:
			return "int3";
			break;
		case fl::type::INT_4:
			return "int4";
			break;
		case fl::type::MAT_2:
			return "Mat2";
			break;
		case fl::type::MAT_3:
			return "Mat3";
			break;
		case fl::type::MAT_4:
			return "Mat4";
			break;
		case fl::type::NONE:
			return "None";
			break;
		case fl::type::SHAPE:
			return "shape";
			break;
		case fl::type::SIZE_T:
			return "size_t";
			break;
		case fl::type::STD_STRING:
			return "std::string";
			break;
		case fl::type::UNSIGNED_CHAR:
			return "uchar";
			break;
		case fl::type::UNSIGNED_INT:
			return "uint";
			break;
		case fl::type::USERDATA:
			return "userdata";
			break;
		case DfxFluentTypes::DS_MESH:
			return "deep_dimplex_mesh";
			break;
		case DfxFluentTypes::DS_POINT_COUNT:
			return "simplex_point_count";
			break;
		case DfxFluentTypes::IMAGE:
			return "image";
			break;
		case DfxFluentTypes::SG_GEO_NODE:
			return "geometry_node";
			break;
		case DfxFluentTypes::SG_NODE:
			return "base_node";
			break;
		case DfxFluentTypes::SG_POINT_CLOUD_NODE:
			return "point_cloud_node";
			break;
		case DfxFluentTypes::SG_SHAPE_NODE:
			return "shape_node";
			break;
		case DfxFluentTypes::SG_STAGE_ACTOR_NODE:
			return "stage_actor_node";
			break;
		case DfxFluentTypes::SG_STAGE_NODE:
			return "stage_node";
			break;
		case DfxFluentTypes::SG_STAGE_PROP_NODE:
			return "stage_prop_node";
			break;
		case DfxFluentTypes::SG_TOOL_NODE:
			return "tool_node";
			break;
		case DfxFluentTypes::SG_WIDGET_NODE:
			return "widget_node";
			break;
		case DfxFluentTypes::RESOURCE:
			return "<resource>";
			break;
		case DfxFluentTypes::RESOURCE_POINTER:
			return "<resource_pointer>";
			break;
		default:
			std::to_string(typ);
			break;
		}

		return "Unknown Value";
	}

	unsigned int spaceTypeToEnum(std::string typ) {

		if ( typ == "any") {
			return fl::type::ANY;
		}
		else if ( typ == "attribute_Path") {
			return fl::type::ATTRIBUTE_PATH;
		}
		else if (typ == "bool") {
			return fl::type::BOOL;
		}
		else if (typ == "char") {
			return fl::type::CHAR;
		}
		else if (typ == "compound_port") {
			return fl::type::COMPOUND_PORT;
		}
		else if (typ == "data_object") {
			return fl::type::DATA_OBJECT;
		}
		else if (typ == "double") {
			return fl::type::DOUBLE;
		}
		else if (typ == "double2") {
			return fl::type::DOUBLE_2;
		}
		else if (typ == "double3") {
			return fl::type::DOUBLE_3;
		}
		else if (typ == "double4") {
			return fl::type::DOUBLE_4;
		}
		else if (typ == "float") {
			return fl::type::FLOAT;
		}
		else if (typ == "float2") {
			return fl::type::FLOAT_2;
		}
		else if (typ == "float3") {
			return fl::type::FLOAT_3;
		}
		else if (typ == "float4") {
			return fl::type::FLOAT_4;
		}
		else if (typ == "flow") {
			return fl::type::FLOW;
		}
		else if (typ == "int") {
			return fl::type::INT;
		}
		else if (typ == "int2") {
			return fl::type::INT_2;
		}
		else if (typ == "int3") {
			return fl::type::INT_3;
		}
		else if (typ == "int4") {
			return fl::type::INT_4;
		}
		else if (typ == "Mat2") {
			return fl::type::MAT_2;
		}
		else if (typ == "Mat3") {
			return fl::type::MAT_3;
		}
		else if (typ == "Mat4") {
			return fl::type::MAT_4;
		}
		else if (typ == "shape") {
			return fl::type::SHAPE;
		}
		else if (typ == "size_t") {
			return fl::type::SIZE_T;
		}
		else if (typ == "std::string") {
			return fl::type::STD_STRING;
		}
		else if (typ == "uchar") {
			return fl::type::UNSIGNED_CHAR;
		}
		else if (typ == "uint") {
			return fl::type::UNSIGNED_INT;
		}
		else if (typ == "userdata") {
			return fl::type::USERDATA;
		}
		else if (typ == "deep_dimplex_mesh") {
			return DfxFluentTypes::DS_MESH;
		}
		else if (typ == "simplex_point_count") {
			return DfxFluentTypes::DS_POINT_COUNT;
		}
		else if (typ == "image") {
			return DfxFluentTypes::IMAGE;
		}
		else if (typ == "geometry_node") {
			return DfxFluentTypes::SG_GEO_NODE;
		}
		else if (typ == "base_node") {
			return DfxFluentTypes::SG_NODE;
		}
		else if (typ == "point_cloud_node") {
			return DfxFluentTypes::SG_POINT_CLOUD_NODE;
		}
		else if (typ == "shape_node") {
			return DfxFluentTypes::SG_SHAPE_NODE;
		}
		else if (typ == "stage_actor_node") {
			return DfxFluentTypes::SG_STAGE_NODE;
		}
		else if (typ == "stage_prop_node") {
			return DfxFluentTypes::SG_STAGE_PROP_NODE;
		}
		else if (typ == "tool_node") {
			return DfxFluentTypes::SG_TOOL_NODE;
		}
		else if (typ == "widget_node") {
			return DfxFluentTypes::SG_WIDGET_NODE;
		}
		else {
			return std::numeric_limits<unsigned int>::max();
		}
		
	}

	std::string anyValueToString(fl::Any *any) {
		unsigned int typ = any->type();
		switch (typ)
		{
		case fl::type::ANY:
			return "*RAW*";
			break;
		case fl::type::ATTRIBUTE_PATH:
			return any->data<std::string>();
			break;
		case fl::type::BOOL:
			return any->data<bool>() ? "True" : "False;";
			break;
		case fl::type::CHAR:
			return  std::string(1, any->data<char>());
			break;
		case fl::type::COMPOUND_PORT:
			return "*RAW*";
			break;
		case fl::type::DATA_OBJECT:
			return "*RAW*";
			break;
		case fl::type::DOUBLE:
			return std::to_string(any->data<double>());
			break;
		case fl::type::DOUBLE_2:
		{
			std::stringstream ss;
			auto v = any->data<glm::dvec2>();
			ss << "( " <<v.x << ", " << v.y <<" )";
			return ss.str();
		}
			break;
		case fl::type::DOUBLE_3:
		{
			std::stringstream ss;
			auto v = any->data<glm::dvec3>();
			ss << "( " << v.x << ", " << v.y << ", " << v.z << " )";
			return ss.str();
		}
			break;
		case fl::type::DOUBLE_4:
		{
			std::stringstream ss;
			auto v = any->data<glm::dvec4>();
			ss << "( " << v.x << ", " << v.y << ", " << v.z  << ", " << v.w << " )";
			return ss.str();
		}
			break;
		case fl::type::FLOAT:
			return std::to_string(any->data<float>());
			break;
		case fl::type::FLOAT_2:
		{
			std::stringstream ss;
			auto v = any->data<glm::vec2>();
			ss << "( " << v.x << ", " << v.y << " )";
			return ss.str();
		}
			break;
		case fl::type::FLOAT_3:
		{
			std::stringstream ss;
			auto v = any->data<glm::vec3>();
			ss << "( " << v.x << ", " << v.y << ", " << v.z << " )";
			return ss.str();
		}
			break;
		case fl::type::FLOAT_4:
		{
			std::stringstream ss;
			auto v = any->data<glm::vec4>();
			ss << "( " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " )";
			return ss.str();
		}
			break;
		case fl::type::FLOW:
			return "*RAW*";
			break;
		case fl::type::INT:
			return std::to_string(any->data<int>());
			break;
		case fl::type::INT_2:
		{
			std::stringstream ss;
			auto v = any->data<glm::ivec2>();
			ss << "( " << v.x << ", " << v.y << " )";
			return ss.str();
		}
			break;
		case fl::type::INT_3:
		{
			std::stringstream ss;
			auto v = any->data<glm::ivec3>();
			ss << "( " << v.x << ", " << v.y << ", " << v.z << " )";
			return ss.str();
		}
			break;
		case fl::type::INT_4:
		{
			std::stringstream ss;
			auto v = any->data<glm::ivec4>();
			ss << "( " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " )";
			return ss.str();
		}
			break;
		case fl::type::MAT_2:
		{
			std::stringstream ss;
			auto v = any->data<glm::mat2>();
			ss << "< " 
			   << &v[0] << ", " << &v[1] << ", "
			   << &v[2] << ", " << &v[3] << ", "
			   << " >";
			return ss.str();
		}
			break;
		case fl::type::MAT_3:
		{
			std::stringstream ss;
			auto v = any->data<glm::mat3>();
			ss << "< "
			   << &v[0] << ", " << &v[1] << ", " << &v[2] << ", "
			   << &v[3] << ", " << &v[4] << ", " << &v[5] << ", "
			   << &v[6] << ", " << &v[7] << ", " << &v[8] << ", "
			   << " >";
			return ss.str();
		}
			break;
		case fl::type::MAT_4:
		{
			std::stringstream ss;
			auto v = any->data<glm::mat4>();
			ss << "< "
				<< &v[0] << ", " << &v[1] << ", " << &v[2] << ", " << &v[3] << ", "
				<< &v[4] << ", " << &v[5] << ", " << &v[6] << ", " << &v[7] << ", "
				<< &v[8] << ", " << &v[9] << ", " << &v[10] << ", " << &v[11] << ", "
				<< &v[12] << ", " << &v[13] << ", " << &v[14] << ", " << &v[15] << ", "
				<< " >";
			return ss.str();
		}
			break;
		case fl::type::NONE:
			return "...";
			break;
		case fl::type::SHAPE:
			return "*RAW*";
			break;
		case fl::type::SIZE_T:
			return std::to_string(any->data<size_t>());
			break;
		case fl::type::STD_STRING:
			return any->data<std::string>();
			break;
		case fl::type::UNSIGNED_CHAR:
			return std::string(1, any->data<unsigned char>());
			break;
		case fl::type::UNSIGNED_INT:
			return std::to_string(any->data<int>());
			break;
		case fl::type::USERDATA:
			return "*RAW*";
			break;
		case DfxFluentTypes::DS_MESH:
			return "*RAW*";
			break;
		case DfxFluentTypes::DS_POINT_COUNT:
			return "*RAW*";
			break;
		case DfxFluentTypes::IMAGE:
			return "*RAW*";
			break;
		case DfxFluentTypes::SG_GEO_NODE:
			return "*RAW*";
			break;
		case DfxFluentTypes::SG_NODE:
			return "*RAW*";
			break;
		case DfxFluentTypes::SG_POINT_CLOUD_NODE:
			return "*RAW*";
			break;
		case DfxFluentTypes::SG_SHAPE_NODE:
			return "*RAW*";
			break;
		case DfxFluentTypes::SG_STAGE_ACTOR_NODE:
			return "*RAW*";
			break;
		case DfxFluentTypes::SG_STAGE_NODE:
			return "*RAW*";
			break;
		case DfxFluentTypes::SG_STAGE_PROP_NODE:
			return "*RAW*";
			break;
		case DfxFluentTypes::SG_TOOL_NODE:
			return "*RAW*";
			break;
		case DfxFluentTypes::SG_WIDGET_NODE:
			return "*RAW*";
			break;
		case DfxFluentTypes::RESOURCE:
		{
			unsigned int uid = any->rawData<DfxResource>()->id();
			return std::to_string(uid);
		}
			break;
		case DfxFluentTypes::RESOURCE_POINTER:
		{
			unsigned int uid = any->rawData<DfxResourcePtr>()->resourceId();
			return std::to_string(uid);
		}
			break;
		default:
			std::to_string(typ);
			break;
		}

		return "N/A";
	}

	std::string anyValueToString2(fl::Any *any) {
		unsigned int typ = any->type();
		switch (typ)
		{
		case fl::type::BOOL:
			return any->data<bool>() ? "True" : "False";
			break;
		case fl::type::CHAR:
			return  std::string(1, any->data<char>());
			break;
		case fl::type::DOUBLE:
			return std::to_string(any->data<double>());
			break;
		case fl::type::DOUBLE_2:
		{
			std::stringstream ss;
			auto v = any->data<glm::dvec2>();
			ss << "[ " << v.x << ", " << v.y << " ]";
			return ss.str();
		}
		break;
		case fl::type::DOUBLE_3:
		{
			std::stringstream ss;
			auto v = any->data<glm::dvec3>();
			ss << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
			return ss.str();
		}
		break;
		case fl::type::DOUBLE_4:
		{
			std::stringstream ss;
			auto v = any->data<glm::dvec4>();
			ss << "[ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " ]";
			return ss.str();
		}
		break;
		case fl::type::FLOAT:
			return std::to_string(any->data<float>());
			break;
		case fl::type::FLOAT_2:
		{
			std::stringstream ss;
			auto v = any->data<glm::vec2>();
			ss << "[ " << v.x << ", " << v.y << " ]";
			return ss.str();
		}
		break;
		case fl::type::FLOAT_3:
		{
			std::stringstream ss;
			auto v = any->data<glm::vec3>();
			ss << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
			return ss.str();
		}
		break;
		case fl::type::FLOAT_4:
		{
			std::stringstream ss;
			auto v = any->data<glm::vec4>();
			ss << "[ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " ]";
			return ss.str();
		}
		break;
		case fl::type::INT:
			return std::to_string(any->data<int>());
			break;
		case fl::type::INT_2:
		{
			std::stringstream ss;
			auto v = any->data<glm::ivec2>();
			ss << "[ " << v.x << ", " << v.y << " ]";
			return ss.str();
		}
		break;
		case fl::type::INT_3:
		{
			std::stringstream ss;
			auto v = any->data<glm::ivec3>();
			ss << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
			return ss.str();
		}
		break;
		case fl::type::INT_4:
		{
			std::stringstream ss;
			auto v = any->data<glm::ivec4>();
			ss << "[ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " ]";
			return ss.str();
		}
		break;
		case fl::type::SIZE_T:
			return std::to_string(any->data<size_t>());
			break;
		case fl::type::STD_STRING:
		{
			std::stringstream ss;
			ss << "\"" << any->data<std::string>() << "\"";
			return ss.str();
		}
			break;
		case fl::type::UNSIGNED_CHAR:
			return std::string(1, any->data<unsigned char>());
			break;
		case fl::type::UNSIGNED_INT:
			return std::to_string(any->data<unsigned int>());
			break;
		case DfxFluentTypes::RESOURCE:
		{
			unsigned int uid = any->rawData<DfxResource>()->id();
			return std::to_string(uid);
		}
		break;
		case DfxFluentTypes::RESOURCE_POINTER:
		{
			unsigned int uid = any->rawData<DfxResourcePtr>()->resourceId();
			return std::to_string(uid);
		}
		default:
			std::to_string(typ);
			break;
		}

		return "\"\"";
	}

	std::string echoSpace(fl::Any *any, int level) {

		std::stringstream ss;

		if (any == nullptr) {
			return echoSpace(DfxSpace::getInstance()->root(), 0);
		}
		else {

			int nlevel = level + 1;
			std::string tab("");
			for (int i = 0; i < level; i++) tab += "    ";

			if (any->type() == fl::type::DATA_OBJECT) {
				auto dao = any->rawData<fl::DataObject>();

				ss << "\n" << tab << "--<" << any->name() << ">--\n";
				for (auto p : dao->properties()) ss << echoSpace(p,  nlevel);
			}
			else {
				ss << tab << "-" << any->label()  << "( "<< spaceTypeToString(any->type()) << " -> " << anyValueToString(any) <<  " )" << std::endl;
			}
		}

		return ss.str();
	}

	std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*> mapAnyValue(fl::Any *any, bool &isAttr, bool &isBuff) {

		unsigned int typ = any->type();
		switch (typ)
		{
		case fl::type::BOOL:
		{
			isAttr = true;

			bool val = false;
			if(!any->isArray()) val = any->data<bool>();
			
			auto attr = xvault::h5::h5Attribute::newValueString("value", xvault::type::BOOL, val ? "true" : "false");
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, nullptr);
		}
			break;
		case fl::type::CHAR:
		{
			isAttr = true;

			char val = '0';
			if (!any->isArray()) val = any->data<char>();

			auto attr = xvault::h5::h5Attribute::newValueInt("value", xvault::type::CHAR, val);
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, nullptr);
		}
			break;
		case fl::type::DOUBLE:
		{
			isAttr = true;

			double val = 0.f;
			if (!any->isArray()) val = any->data<double>();

			auto attr = xvault::h5::h5Attribute::newValueDouble("value", xvault::type::DOUBLE, val);
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, nullptr);
		}
			break;
		case fl::type::DOUBLE_2:
		{
			isBuff = true;
			auto buff = new xvault::h5::h5Buffer;
			buff->setType(xvault::type::DOUBLE);
			buff->setName("value");
			buff->setBufferSize(2);

			auto v = any->data<glm::dvec2>();
			auto arr = new double[2];
			arr[0] = 0.f;
			arr[1] = 0.f;

			if (!any->isArray()) {
				arr[0] = v.x;
				arr[1] = v.y;
			}

			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(nullptr, buff);
		}
			break;
		case fl::type::DOUBLE_3:
		{
			isBuff = true;

			auto buff = new xvault::h5::h5Buffer;
			buff->setType(xvault::type::DOUBLE);
			buff->setName("value");
			buff->setBufferSize(3);

			auto v = any->data<glm::dvec3>();
			auto arr = new double[3];
			arr[0] = 0.f;
			arr[1] = 0.f;
			arr[2] = 0.f;

			if (!any->isArray()) {
				arr[0] = v.x;
				arr[1] = v.y;
				arr[2] = v.z;
			}

			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(nullptr, buff);
		}
			break;
		case fl::type::DOUBLE_4:
		{
			isBuff = true;
			auto buff = new xvault::h5::h5Buffer;
			buff->setType(xvault::type::DOUBLE);
			buff->setName("value");
			buff->setBufferSize(4);

			auto v = any->data<glm::dvec4>();
			auto arr = new double[4];
			arr[0] = 0.f;
			arr[1] = 0.f;
			arr[2] = 0.f;
			arr[3] = 0.f;

			if (!any->isArray()) {
				arr[0] = v.x;
				arr[1] = v.y;
				arr[2] = v.z;
				arr[3] = v.w;
			}

			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(nullptr, buff);
		}
			break;
		case fl::type::FLOAT:
		{
			isAttr = true;

			float val = 0.f;
			if (!any->isArray()) val = any->data<float>();

			auto attr = xvault::h5::h5Attribute::newValueFloat("value", xvault::type::FLOAT, val);
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, nullptr);
		}
			break;
		case fl::type::FLOAT_2:
		{
			isAttr = true;

			glm::vec2 val(0.f);
			if (!any->isArray()) val = any->data<glm::vec2>();

			auto attr = xvault::h5::h5Attribute::newValueFloat2("value", xvault::type::FLOAT_2, val);
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, nullptr);
	
		}
			break;
		case fl::type::FLOAT_3:
		{
			isAttr = true;

			glm::vec3 val(0.f);
			if (!any->isArray()) val = any->data<glm::vec3>();

			auto attr = xvault::h5::h5Attribute::newValueFloat3("value", xvault::type::FLOAT_3, val);
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, nullptr);
		}
			break;
		case fl::type::FLOAT_4:
		{
			isAttr = true;
			auto attr = xvault::h5::h5Attribute::newValueFloat4("value", xvault::type::FLOAT_4, any->data<glm::vec4>());
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, nullptr);
		}
		break;
		case fl::type::MAT_3:
		{
			isAttr = true;

			glm::mat3 val(0.f);
			if (!any->isArray()) val = any->data<glm::mat3>();

			auto attr = xvault::h5::h5Attribute::newValueFloat9("value", xvault::type::FLOAT_9, val);
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, nullptr);
		}
		break;
		case fl::type::MAT_4:
		{
			isAttr = true;

			glm::mat4 val(0.f);
			if (!any->isArray()) val = any->data<glm::mat4>();

			auto attr = xvault::h5::h5Attribute::newValueFloat16("value", xvault::type::FLOAT_16, val);
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, nullptr);
		}
		break;
		case fl::type::INT:
		{
			isAttr = true;

			int val(0.f);
			if (!any->isArray()) val = any->data<int>();

			auto attr = xvault::h5::h5Attribute::newValueInt("value", xvault::type::INT, val);
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, nullptr);
		}
			break;
		case fl::type::INT_2:
		{
			isBuff = true;
			auto buff = new xvault::h5::h5Buffer;
			buff->setType(xvault::type::INT);
			buff->setName("value");
			buff->setBufferSize(2);

			auto v = any->data<glm::ivec2>();
			auto arr = new int[2];
			arr[0] = 0.f;
			arr[1] = 0.f;

			int val(0.f);
			if (!any->isArray()) {
				arr[0] = v.x;
				arr[1] = v.y;
			}

			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(nullptr, buff);
		}
			break;
		case fl::type::INT_3:
		{
			isBuff = true;
			auto buff = new xvault::h5::h5Buffer;
			buff->setType(xvault::type::INT);
			buff->setName("value");
			buff->setBufferSize(3);

			auto v = any->data<glm::ivec3>();
			auto arr = new int[3];
			arr[0] = 0;
			arr[1] = 0;
			arr[2] = 0;

			if (!any->isArray()) {
				arr[0] = v.x;
				arr[1] = v.y;
				arr[2] = v.z;
			}

			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(nullptr, buff);
		}
			break;
		case fl::type::INT_4:
		{
			isBuff = true;
			auto buff = new xvault::h5::h5Buffer;
			buff->setType(xvault::type::INT);
			buff->setName("value");
			buff->setBufferSize(4);

			auto v = any->data<glm::ivec4>();
			auto arr = new int[4];
			arr[0] = 0.f;
			arr[1] = 0.f;
			arr[2] = 0.f;
			arr[3] = 0.f;

			if (!any->isArray()) {
				arr[0] = v.x;
				arr[1] = v.y;
				arr[2] = v.z;
				arr[3] = v.w;
			}

			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(nullptr, buff);
		}
			break;
		case fl::type::SIZE_T:
		{
			isAttr = true;
			
			size_t val = 0;
			if (!any->isArray()) val = any->data<unsigned long long>();

			auto attr = xvault::h5::h5Attribute::newValueULongLong("value", xvault::type::UNSIGNED_LONG_LONG, val);
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, (xvault::h5::h5Buffer*)nullptr);
		}
			break;
		case fl::type::STD_STRING:
		{
			isAttr = true;

			std::string val = "";
			if (!any->isArray()) val = any->data<std::string>();

			auto attr = xvault::h5::h5Attribute::newValueString("value", xvault::type::STRING, val);
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, (xvault::h5::h5Buffer*)nullptr);
		}
			break;
		case fl::type::UNSIGNED_CHAR:
		{
			isAttr = true;

			unsigned char val = 0;
			if (!any->isArray()) val = any->data<unsigned char>();

			auto attr = xvault::h5::h5Attribute::newValueUInt("value", xvault::type::UNSIGNED_INT, any->data<unsigned char>());
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, (xvault::h5::h5Buffer*)nullptr);
		}
			break;
		case fl::type::UNSIGNED_INT:
		{
			isAttr = true;
			auto attr = xvault::h5::h5Attribute::newValueUInt("value", xvault::type::UNSIGNED_INT, any->data<unsigned int>());
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, (xvault::h5::h5Buffer*)nullptr);
		}
			break;
		case DfxFluentTypes::RESOURCE:
		{		
			isAttr = true;
			unsigned int uid = any->rawData<DfxResource>()->id();
			auto attr = xvault::h5::h5Attribute::newValueUInt("value", xvault::type::UNSIGNED_INT, uid);
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, (xvault::h5::h5Buffer*)nullptr);
		}
		break;
		case DfxFluentTypes::RESOURCE_POINTER:
		{
			isAttr = true;
			unsigned int uid = any->rawData<DfxResourcePtr>()->resourceId();
			auto attr = xvault::h5::h5Attribute::newValueUInt("value", xvault::type::UNSIGNED_INT, uid);
			return std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(attr, (xvault::h5::h5Buffer*)nullptr);
		}
		default:
			std::to_string(typ);
			break;
		}

		return  std::pair<xvault::h5::h5Attribute*, xvault::h5::h5Buffer*>(nullptr, nullptr);
	}

	xvault::Document *spaceToVDocument(xvault::Node *node ,fl::Any *any, unsigned int uid) {

		if (any == nullptr && node == nullptr) {
			xvault::Document *_vdoc = new xvault::rapid::rjDocument;
			spaceToVDocument(_vdoc->root(), DfxSpace::getInstance()->root(), 0xffee);
			return _vdoc;
		}
		else {

			if (any->type() == fl::type::DATA_OBJECT) {
				auto dao = any->rawData<fl::DataObject>();

				for (auto p : dao->propertiesOriginalOrder()) {

					if (p->type() == fl::type::DATA_OBJECT) {

						auto dao = p->rawData<fl::DataObject>();
						xvault::Node *c_node = new xvault::rapid::rjNode(std::to_string(uid++));

						auto attr00 = xvault::h5::h5Attribute::newValueString("name", xvault::type::STRING, p->name());
						c_node->attributes().push_back(attr00);

						auto attr01 = xvault::h5::h5Attribute::newValueString("label", xvault::type::STRING, p->label());
						c_node->attributes().push_back(attr01);

						auto attr02 = xvault::h5::h5Attribute::newValueUInt("type", xvault::type::UNSIGNED_INT, p->type());
						c_node->attributes().push_back(attr02);

						if (dao->hasModel()) {
							auto attr03 = xvault::h5::h5Attribute::newValueString("model", xvault::type::STRING, dao->modelReference());
							c_node->attributes().push_back(attr03);
						}

						if (dao->isFunctional()) {
							auto attr04 = xvault::h5::h5Attribute::newValueString("isFunctional", xvault::type::STRING, "TRUE");
							c_node->attributes().push_back(attr04);
						}

						if (!dao->inputReferences().empty()) {
							xvault::Node *ref = new xvault::rapid::rjNode("inputReferences");

							for (auto &refl: dao->inputReferences()) {
								xvault::Node *_ref = new xvault::rapid::rjNode(std::to_string(uid++));

								auto attr00 = xvault::h5::h5Attribute::newValueString("first", xvault::type::STRING, refl.first);
								auto attr01 = xvault::h5::h5Attribute::newValueString("second", xvault::type::STRING, refl.second);

								_ref->attributes().push_back(attr00);
								_ref->attributes().push_back(attr01);

								ref->nodes().push_back(_ref);
							}

							c_node->nodes().push_back(ref);
						}

						if (!dao->outputReferences().empty()) {
							xvault::Node *ref = new xvault::rapid::rjNode("outputReferences");

							for (auto &refl : dao->outputReferences()) {
								xvault::Node *_ref = new xvault::rapid::rjNode(std::to_string(uid++));

								auto attr00 = xvault::h5::h5Attribute::newValueString("first", xvault::type::STRING, refl.first);
								auto attr01 = xvault::h5::h5Attribute::newValueString("second", xvault::type::STRING, refl.second);

								_ref->attributes().push_back(attr00);
								_ref->attributes().push_back(attr01);

								ref->nodes().push_back(_ref);
							}

							c_node->nodes().push_back(ref);
						}

						node->nodes().push_back(c_node);
						spaceToVDocument(c_node, p, uid);

					}
					else {

						xvault::Node *c_node = new xvault::rapid::rjNode(std::to_string(uid++));
						bool isAttr = false;
						bool isBuff = false;

						auto attr00 = xvault::h5::h5Attribute::newValueString("name", xvault::type::STRING, p->name());
				
						auto attr01 = xvault::h5::h5Attribute::newValueString("label", xvault::type::STRING, p->label());

						auto attr02 = xvault::h5::h5Attribute::newValueUInt("type", xvault::type::UNSIGNED_INT, p->type());

						auto attr03 = xvault::h5::h5Attribute::newValueUInt("role", xvault::type::UNSIGNED_INT, p->role());

						c_node->attributes().push_back(attr00);
						c_node->attributes().push_back(attr01);
						c_node->attributes().push_back(attr02);
						c_node->attributes().push_back(attr03);

						auto mappedVal = mapAnyValue(p, isAttr, isBuff);

						if (isAttr) {
							c_node->attributes().push_back(mappedVal.first);
						}
						else if (isBuff) {
							c_node->arrays().push_back(mappedVal.second);
						}

						auto keyMap = DfxKeyFrameManager::getInstance()->getFloatMap(p->absolutePath());

						if (keyMap != nullptr) {
							xvault::Node *c_nodeKeys = new xvault::rapid::rjNode("keys");

							auto keys = keyMap->keys();
							std::sort(keys.begin(), keys.end());

							for (auto k: keys) {
								xvault::Node *c_nodeKeyBlock  = new xvault::rapid::rjNode("key");
								
								auto value = keyMap->value(k);
								auto interpoType = keyMap->interpolationType(k);
								auto controlPoints = keyMap->controlPoints(k);

								auto attr00 = xvault::h5::h5Attribute::newValueUInt("frame", xvault::type::UNSIGNED_INT, k);
								auto attr01 = xvault::h5::h5Attribute::newValueFloat("value", xvault::type::FLOAT, value);
								auto attr02 = xvault::h5::h5Attribute::newValueUInt("interpolation", xvault::type::UNSIGNED_INT, interpoType);
								auto attr03 = xvault::h5::h5Attribute::newValueFloat("bezier_cp0x", xvault::type::FLOAT, controlPoints.first.x);
								auto attr04 = xvault::h5::h5Attribute::newValueFloat("bezier_cp0y", xvault::type::FLOAT, controlPoints.first.y);
								auto attr05 = xvault::h5::h5Attribute::newValueFloat("bezier_cp1x", xvault::type::FLOAT, controlPoints.second.x);
								auto attr06 = xvault::h5::h5Attribute::newValueFloat("bezier_cp1y", xvault::type::FLOAT, controlPoints.second.y);

								c_nodeKeyBlock->attributes().push_back(attr00);
								c_nodeKeyBlock->attributes().push_back(attr01);
								c_nodeKeyBlock->attributes().push_back(attr02);
								c_nodeKeyBlock->attributes().push_back(attr03);
								c_nodeKeyBlock->attributes().push_back(attr04);
								c_nodeKeyBlock->attributes().push_back(attr05);
								c_nodeKeyBlock->attributes().push_back(attr06);

								c_nodeKeys->nodes().push_back(c_nodeKeyBlock);
							}

							c_node->nodes().push_back(c_nodeKeys);
						}

						node->nodes().push_back(c_node);
					}
				}
			}
	
		}

		return nullptr;
	}

	xvault::Document *graphToVDocument(xvault::Node *node, deepsg::SgNode *sgnode, unsigned int uid) {

		auto space = DfxSpace::getInstance();

		if (sgnode == nullptr && node == nullptr) {
			xvault::Document *_vdoc = new xvault::rapid::rjDocument;
			graphToVDocument(_vdoc->root(), deepsg::SgRootNode::getInstance(), 0xffee);
			return _vdoc;
		}
		else {

			xvault::Node *c_node = new xvault::rapid::rjNode(std::to_string(uid++));
			
			auto attr00 = xvault::h5::h5Attribute::newValueString("uid", xvault::type::STRING, sgnode->uniqueName());
			auto attr01 = xvault::h5::h5Attribute::newValueString("reference", xvault::type::STRING, sgnode->referencePath().c_str());
			auto attr02 = xvault::h5::h5Attribute::newValueUInt("visible", xvault::type::UNSIGNED_INT, sgnode->isVisible() ? 1 : 0);
			auto attr03 = xvault::h5::h5Attribute::newValueUInt("renderable", xvault::type::UNSIGNED_INT, sgnode->isRenderable() ? 1 : 0);

			c_node->attributes().push_back(attr00);
			c_node->attributes().push_back(attr01);
			c_node->attributes().push_back(attr02);
			c_node->attributes().push_back(attr03);

			node->nodes().push_back(c_node);
		
			for (auto schild : sgnode->children()) {
				graphToVDocument(c_node, schild, uid++);
			}
		}

		return nullptr;
	}

	xvault::Document *spaceBindingsToVDocument() {
		auto space = DfxSpace::getInstance();

		size_t uid = 0xffee;

		xvault::Document *_vdoc = new xvault::rapid::rjDocument;

		for (auto binding : space->bindingMap()) {
			xvault::Node *c_binding = new xvault::rapid::rjNode(std::to_string(uid++));

			auto bAttr00 = xvault::h5::h5Attribute::newValueString("input", xvault::type::STRING, binding.first);
			c_binding->attributes().push_back(bAttr00);

			for (auto link : binding.second.links()) {
				xvault::Node *c_link = new xvault::rapid::rjNode(std::to_string(uid++));

				auto lAttr00 = xvault::h5::h5Attribute::newValueString("output", xvault::type::STRING, link.output_path);
				auto lAttr01 = xvault::h5::h5Attribute::newValueString("output_node", xvault::type::STRING, link.output_node);
				auto lAttr02 = xvault::h5::h5Attribute::newValueString("input", xvault::type::STRING, link.input_path);
				auto lAttr03 = xvault::h5::h5Attribute::newValueString("input_node", xvault::type::STRING, link.input_node);
				auto lAttr04 = xvault::h5::h5Attribute::newValueInt("stamp", xvault::type::INT, link.stamp);

				c_link->attributes().push_back(lAttr00);
				c_link->attributes().push_back(lAttr01);
				c_link->attributes().push_back(lAttr02);
				c_link->attributes().push_back(lAttr03);
				c_link->attributes().push_back(lAttr04);

				c_binding->nodes().push_back(c_link);
			}

			_vdoc->root()->nodes().push_back(c_binding);
		}

		return _vdoc;
	}

	xvault::Document *materialLinksToVDocument() {
		auto space = DfxSpace::getInstance();

		size_t uid = 0xffee;

		xvault::Document *_vdoc = new xvault::rapid::rjDocument;

		for (auto objMap : DfxMaterialLinker::getInstance()->materialLinkMap()) {
			xvault::Node *c_binding = new xvault::rapid::rjNode(std::to_string(uid++));

			auto bAttr00 = xvault::h5::h5Attribute::newValueString("obj_path", xvault::type::STRING, objMap.first);
			c_binding->attributes().push_back(bAttr00);

			for (auto link : objMap.second->all()) {
				xvault::Node *c_link = new xvault::rapid::rjNode(std::to_string(uid++));

				auto lAttr00 = xvault::h5::h5Attribute::newValueUInt("surfID", xvault::type::UNSIGNED_INT, link.first);
				auto lAttr01 = xvault::h5::h5Attribute::newValueString("material", xvault::type::STRING, link.second);

				c_link->attributes().push_back(lAttr00);
				c_link->attributes().push_back(lAttr01);

				c_binding->nodes().push_back(c_link);
			}

			_vdoc->root()->nodes().push_back(c_binding);
		}

		return _vdoc;
	}

	xvault::Document *surfaceDictionaryToVDocument() {
		auto space = DfxSpace::getInstance();

		size_t uid = 0xffee;

		xvault::Document *_vdoc = new xvault::rapid::rjDocument;

		for (auto objMap : DfxSurfaceDictionary::getInstance()->all()) {
			xvault::Node *c_binding = new xvault::rapid::rjNode(std::to_string(uid++));

			auto bAttr00 = xvault::h5::h5Attribute::newValueString("obj_path", xvault::type::STRING, objMap.first);
			c_binding->attributes().push_back(bAttr00);

			for (auto entry : objMap.second) {
				xvault::Node *c_link = new xvault::rapid::rjNode(std::to_string(uid++));

				auto lAttr00 = xvault::h5::h5Attribute::newValueUInt("uid", xvault::type::UNSIGNED_INT, entry.first);
				auto lAttr01 = xvault::h5::h5Attribute::newValueString("name", xvault::type::STRING, entry.second);

				c_link->attributes().push_back(lAttr00);
				c_link->attributes().push_back(lAttr01);

				c_binding->nodes().push_back(c_link);
			}

			_vdoc->root()->nodes().push_back(c_binding);
		}

		return _vdoc;
	}

	xvault::Document *fluentFlowsToVDocument(bool rootFlow, size_t uid, fl::Flow *flow, xvault::Node *parent, size_t doc_id) {
		auto space = DfxSpace::getInstance();

		size_t __uid = 0xffee;

		if (flow == nullptr) {
			xvault::Document *_vdoc = new xvault::rapid::rjDocument;
			for (auto p : DfxFlowDB::getInstance()->flowList()) {
				fluentFlowsToVDocument(true, p.first, p.second, _vdoc->root(), __uid++);
			}
			return _vdoc;
		}
		else {
			xvault::Node *flow_node = new xvault::rapid::rjNode(std::to_string(__uid++));

			auto lAttr00 = xvault::h5::h5Attribute::newValueUInt("root_flow", xvault::type::UNSIGNED_INT, rootFlow ? 1 : 0);
			auto lAttr01 = xvault::h5::h5Attribute::newValueUInt("uid", xvault::type::UNSIGNED_INT, uid);

			auto lAttr02 = xvault::h5::h5Attribute::newValueInt("id", xvault::type::INT, flow->id());
			auto lAttr03 = xvault::h5::h5Attribute::newValueInt("type", xvault::type::INT, flow->type());
			auto lAttr04 = xvault::h5::h5Attribute::newValueInt("next", xvault::type::INT, flow->next());
			auto lAttr05 = xvault::h5::h5Attribute::newValueInt("yes", xvault::type::INT, flow->yes());
			auto lAttr06 = xvault::h5::h5Attribute::newValueInt("no", xvault::type::INT, flow->no());
			auto lAttr07 = xvault::h5::h5Attribute::newValueString("conditionPath", xvault::type::STRING, flow->conditionPath());
			auto lAttr08 = xvault::h5::h5Attribute::newValueString("conditionOutputNodePath", xvault::type::STRING, flow->conditionOuputNodePath());
			auto lAttr09 = xvault::h5::h5Attribute::newValueString("text", xvault::type::STRING, flow->text());
			auto lAttr10 = xvault::h5::h5Attribute::newValueFloat("posx", xvault::type::FLOAT, flow->posX());
			auto lAttr11 = xvault::h5::h5Attribute::newValueFloat("posy", xvault::type::FLOAT, flow->posY());

			xvault::Node *paths = new xvault::rapid::rjNode("paths");
			xvault::Node *nodes = new xvault::rapid::rjNode("nodes");

			for (auto path : flow->paths()) {
				auto tempAttr = xvault::h5::h5Attribute::newValueString(std::to_string(__uid++), xvault::type::STRING, path);
				paths->attributes().push_back(tempAttr);
			}
			
			for (auto fnode: flow->nodes()) {
				fluentFlowsToVDocument(false, 0, fnode, nodes, __uid++);
			}

			flow_node->attributes().push_back(lAttr00);
			flow_node->attributes().push_back(lAttr01);
			flow_node->attributes().push_back(lAttr02);
			flow_node->attributes().push_back(lAttr03);
			flow_node->attributes().push_back(lAttr04);
			flow_node->attributes().push_back(lAttr05);
			flow_node->attributes().push_back(lAttr06);
			flow_node->attributes().push_back(lAttr07);
			flow_node->attributes().push_back(lAttr08);
			flow_node->attributes().push_back(lAttr09);
			flow_node->attributes().push_back(lAttr10);
			flow_node->attributes().push_back(lAttr11);

			flow_node->nodes().push_back(paths);
			flow_node->nodes().push_back(nodes);

			parent->nodes().push_back(flow_node);
		}

		return nullptr;
	}


	xvault::Document *tracksToVDocument() {
		xvault::Document *_vdoc = new xvault::rapid::rjDocument;
		
		xvault::Node *tracks = new xvault::rapid::rjNode("tracks");

		int counter = 0;
		for (auto track : DfxCacheTrackManager::getInstance()->tracks()) {
			xvault::Node *_track = new xvault::rapid::rjNode("track" + std::to_string(counter++));

			auto attr = xvault::h5::h5Attribute::newValueString("path", xvault::type::STRING, track->path());
			_track->attributes().push_back(attr);

			tracks->nodes().push_back(_track);
		}

		_vdoc->root()->nodes().push_back(tracks);

		return _vdoc;
	}

	void deleteAllSgNodes() {

		auto dag = new fl::DirectedAcyclicGraph<size_t, deepsg::SgNode>();
		std::stack<fl::DirectedAcyclicGraph<size_t, deepsg::SgNode>::Vertex*> nodeStack;
		std::unordered_map<size_t, bool> deletedNodeMap;

		deepsg::SgTraversalFunction travFunc = [dag](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
			auto vertex = new fl::DirectedAcyclicGraph<size_t, deepsg::SgNode>::Vertex();
			vertex->id = node->getID();
			vertex->description = node->referencePath();
			vertex->userData = node;

			dag->addVertex(vertex);

			if (info->parent() != nullptr) {
				dag->addEdge(info->parent()->getID(), vertex);
			}
		};

		deepsg::SgRootNode::getInstance()->traverseScene(travFunc);

		auto dagSorted = dag->topologicalSort();

		for (auto v : dagSorted) nodeStack.push(v);

		std::vector<size_t> deletedNodes;
		while (!nodeStack.empty()) {

			auto node = nodeStack.top();
			nodeStack.pop();

			if (!node->userData->isProtected()) {
				auto deleteditr = deletedNodeMap.find(node->userData->getID());
			
				if (deleteditr == deletedNodeMap.end()) {
					bool rc = util::deleteSgNode(node->userData, deletedNodes);
					for (auto d : deletedNodes) deletedNodeMap[d] = true;

					if (!rc) return;

				}
			}
		}

		for (auto v : dag->allVertices()) delete v;
		delete dag;
	}

	void deleteSgNodes(std::vector<deepsg::SgNode*> nodes) {

		auto dag = new fl::DirectedAcyclicGraph<size_t, deepsg::SgNode>();
		std::stack<fl::DirectedAcyclicGraph<size_t, deepsg::SgNode>::Vertex*> nodeStack;
		std::unordered_map<size_t, bool> selectedNodeMap;
		std::unordered_map<size_t, bool> deletedNodeMap;

		for (auto node : nodes) {
			selectedNodeMap[node->getID()] = true;
		}

		deepsg::SgTraversalFunction travFunc = [dag](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
			auto vertex = new fl::DirectedAcyclicGraph<size_t, deepsg::SgNode>::Vertex();
			vertex->id = node->getID();
			vertex->description = node->referencePath();
			vertex->userData = node;

			dag->addVertex(vertex);

			if (info->parent() != nullptr) {
				dag->addEdge(info->parent()->getID(), vertex);
			}
		};

		deepsg::SgRootNode::getInstance()->traverseScene(travFunc);

		auto dagSorted = dag->topologicalSort();

		for (auto v : dagSorted) nodeStack.push(v);

		std::vector<size_t> deletedNodes;
		while (!nodeStack.empty()) {

			auto node = nodeStack.top();
			nodeStack.pop();

			if (!node->userData->isProtected()) {
				auto selecteditr = selectedNodeMap.find(node->userData->getID());
				auto deleteditr = deletedNodeMap.find(node->userData->getID());
				if (selecteditr != selectedNodeMap.end()) {

					if (deleteditr == deletedNodeMap.end()) {
						bool rc = util::deleteSgNode(node->userData, deletedNodes);
						for (auto d : deletedNodes) deletedNodeMap[d] = true;

						if (!rc) return;

					}
				}
			}
			else {
				DfxGlobal::console::println("Skipping Protected Node.");
				//return;
			}
		}

		for (auto v : dag->allVertices()) delete v;
		delete dag;
	}

	bool deleteSgNode(deepsg::SgNode *node, std::vector<size_t> &deletedNodes) {

		bool deleteNode = true;

		for (auto child : node->children()) {
			deleteNode = deleteSgNode(child, deletedNodes);
		}

		if (deleteNode) {
			auto delCallback = DfxSqNodeDB::getInstance()->deleteCallback(node->uniqueName());

			if (delCallback != nullptr) {
				size_t uid = node->getID();
				delCallback(node);
				deletedNodes.push_back(uid);
				return true;
			}
			else {
				DfxGlobal::console::println("Failed to delete all selected nodes.");
				return false;
			}
		}

		return false;
	}

	std::pair<deepsg::SgNode*, size_t> fetchNode(std::string referencePath) {
		std::pair<deepsg::SgNode*, size_t> output;

		auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

		if (any != nullptr) {
			if (any->type() == fl::type::DATA_OBJECT) {
				auto dao = any->rawData<fl::DataObject>();

				if (dao->hasProperty("sg_geo_node")) {
					auto snode = any->data<fl::DataObject>().property("sg_geo_node")->rawData<deepsg::SgGeoNode>();
					output = std::pair<deepsg::SgNode*, size_t>(snode, DfxFluentTypes::SG_GEO_NODE);
				}
				else if (dao->hasProperty("sg_point_cloud_node")) {
					auto snode = any->data<fl::DataObject>().property("sg_point_cloud_node")->rawData<deepsg::SgPointCloudNode>();
					output = std::pair<deepsg::SgNode*, size_t>(snode, DfxFluentTypes::SG_POINT_CLOUD_NODE);
				}
				else if (dao->hasProperty("sg_shape_node")) {
					auto snode = any->data<fl::DataObject>().property("sg_shape_node")->rawData<deepsg::SgShapeNode>();
					output = std::pair<deepsg::SgNode*, size_t>(snode, DfxFluentTypes::SG_SHAPE_NODE);
				}
				else if (dao->hasProperty("sg_group_node")) {
					auto snode = any->data<fl::DataObject>().property("sg_group_node")->rawData<DfxGroupNode>();
					output = std::pair<deepsg::SgNode*, size_t>(snode, DfxFluentTypes::SG_GROUP_NODE);
				}
				else if (dao->hasProperty("sg_null_node")) {
					auto snode = any->data<fl::DataObject>().property("sg_null_node")->rawData<DfxNullNode>();
					output = std::pair<deepsg::SgNode*, size_t>(snode, DfxFluentTypes::SG_NULL_NODE);
				}
				else {
					output = std::pair<deepsg::SgNode*, size_t>(nullptr, 0);
				}
			}
			else {
				output = std::pair<deepsg::SgNode*, size_t>(nullptr, 0);
			}
		}
		else {
			output = std::pair<deepsg::SgNode*, size_t>(nullptr, 0);
		}

		return output;
	}

	std::string Mat4ToString(glm::mat4 mat) {
		std::stringstream ss;

		ss << "'" << (&mat[0][0])[0] << "\t" << (&mat[0][0])[1] << "\t" << (&mat[0][0])[2] << "\t" << (&mat[0][0])[3] << "'\n";
		ss << "'" << (&mat[0][0])[4] << "\t" << (&mat[0][0])[5] << "\t" << (&mat[0][0])[6] << "\t" << (&mat[0][0])[7] << "'\n";
		ss << "'" << (&mat[0][0])[8] << "\t" << (&mat[0][0])[9] << "\t" << (&mat[0][0])[10] << "\t" << (&mat[0][0])[11] << "'\n";
		ss << "'" << (&mat[0][0])[12] << "\t" << (&mat[0][0])[13] << "\t" << (&mat[0][0])[14] << "\t" << (&mat[0][0])[15] << "'\n";

		return ss.str();
	}

	DfxBoundsMinMax boundingBox(deepsg::SgGeoNode *snode) {

		DfxBounds output;
		output.empty();

		auto adapter = snode->getGeometryAdapter();
		for (size_t i = 0; i < adapter->getVertexCount(); i++) {
			auto pnt = adapter->getVertex(i);
			output.add(pnt.x, pnt.y, pnt.z);
		}

		DfxBoundsMinMax bounds;
		bounds.xmin = output.xmin;
		bounds.ymin = output.ymin;
		bounds.zmin = output.zmin;

		bounds.xmax = output.xmax;
		bounds.ymax = output.ymax;
		bounds.zmax = output.zmax;

		return bounds;
	}
}
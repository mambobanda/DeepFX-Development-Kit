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

#include "DfxMeshAttributes.h"

DfxMeshAttributes::DfxMeshAttributes(unsigned int attrib_id) {
	_attrib_id = attrib_id;
}

DfxMeshAttributes::~DfxMeshAttributes() {

}

void DfxMeshAttributes::setName(std::string nm) {
	_name = nm;
}

std::string DfxMeshAttributes::name() {
	switch (_attrib_id) {
	case EDGE_COUNT:
		return "EdgeCount";
		break;
	case FACE_COUNT:
		return "FaceCount";
		break;
	case POINT_COUNT:
		return "PointCount";;
		break;
	case BASE_POINT_POSITION:
		return "BasePointPosition";
		break;
	case FINAL_POINT_POSITION:
		return "FinalPointPosition";
		break;
	case BASE_POINT_NORMAL:
		return "BasePointNormal";
		break;
	case FINAL_POINT_NORMAL:
		return "FinalPointNormal";
		break;
	case SHAPE:
		return "Shape";
		break;
	case POSITION:
		return "WorldPosition";
		break;
	case ROTATION:
		return "WorldRotation";
		break;
	case SCALE:
		return "WorldScale";
		break;
	default:
		break;
	}

	return "N/A";
}

unsigned int DfxMeshAttributes::type() {
	switch (_attrib_id) {
	case EDGE_COUNT:
		return fl::type::SIZE_T;
		break;
	case FACE_COUNT:
		return fl::type::SIZE_T;
		break;
	case POINT_COUNT:
		return fl::type::SIZE_T;
		break;
	case BASE_POINT_POSITION:
		return fl::type::FLOAT_3;
		break;
	case FINAL_POINT_POSITION:
		return fl::type::FLOAT_3;
		break;
	case BASE_POINT_NORMAL:
		return fl::type::FLOAT_3;
		break;
	case FINAL_POINT_NORMAL:
		return fl::type::FLOAT_3;
		break;
	case SHAPE:
		return fl::type::SHAPE;
		break;
	case POSITION:
		return fl::type::FLOAT_3;
		break;
	case ROTATION:
		return fl::type::FLOAT_3;
		break;
	case SCALE:
		return fl::type::FLOAT_3;
		break;
	default:
		break;
	}

	return fl::type::NONE;
}

bool DfxMeshAttributes::isReadable() {
	
	switch (_attrib_id) {
	case EDGE_COUNT:
		return true;
		break;
	case FACE_COUNT:
		return true;
		break;
	case POINT_COUNT:
		return true;
		break;
	case BASE_POINT_POSITION:
		return true;
		break;
	case FINAL_POINT_POSITION:
		return true;
		break;
	case BASE_POINT_NORMAL:
		return true;
		break;
	case FINAL_POINT_NORMAL:
		return true;
		break;
	case SHAPE:
		return true;
		break;
	case POSITION:
		return true;
		break;
	case ROTATION:
		return true;
		break;
	case SCALE:
		return true;
		break;
	default:
		break;
	}

	return false;
}

bool DfxMeshAttributes::isWritable() {

	switch (_attrib_id) {
	case EDGE_COUNT:
		return false;
		break;
	case FACE_COUNT:
		return false;
		break;
	case POINT_COUNT:
		return false;
		break;
	case BASE_POINT_POSITION:
		return false;
		break;
	case FINAL_POINT_POSITION:
		return true;
		break;
	case BASE_POINT_NORMAL:
		return false;
		break;
	case FINAL_POINT_NORMAL:
		return true;
		break;
	case SHAPE:
		return false;
		break;
	case POSITION:
		return true;
		break;
	case ROTATION:
		return true;
		break;
	case SCALE:
		return true;
		break;
	default:
		break;
	}

	return false;
}

bool DfxMeshAttributes::isArray() {

	switch (_attrib_id) {
	case EDGE_COUNT:
		return false;
		break;
	case FACE_COUNT:
		return false;
		break;
	case POINT_COUNT:
		return false;
		break;
	case BASE_POINT_POSITION:
		return true;
		break;
	case FINAL_POINT_POSITION:
		return true;
		break;
	case BASE_POINT_NORMAL:
		return true;
		break;
	case FINAL_POINT_NORMAL:
		return true;
		break;
	case SHAPE:
		return false;
		break;
	case POSITION:
		return false;
		break;
	case ROTATION:
		return false;
		break;
	case SCALE:
		return false;
		break;
	default:
		break;
	}

	return false;
}

fl::Any *DfxMeshAttributes::getValue() {
	DfxObjectHandler handler;

	if (handler.fetch(path())) {
		
		auto geo = handler.geoNode();
		auto geoAdaptor = geo->getGeometryAdapter();

		fl::Any *any = new fl::Any;

		switch (_attrib_id) {
		case EDGE_COUNT:
		{
			any->newData<size_t>(fl::type::SIZE_T);
			any->setData<size_t>(geoAdaptor->getEdgeCount());
			return any;
		}
			break;
		case FACE_COUNT:
		{
			any->newData<size_t>(fl::type::SIZE_T);
			any->setData<size_t>(geoAdaptor->getPolygonCount());
			return any;
		}
			break;
		case POINT_COUNT:
		{
			any->newData<size_t>(fl::type::SIZE_T);
			any->setData<size_t>(geoAdaptor->getVertexCount());
			return any;
		}
			break;
		case BASE_POINT_POSITION:
		{
			auto arr = new std::vector<glm::vec3>;
			arr->resize(geoAdaptor->getVertexCount());

			for (size_t inx = 0; inx < geoAdaptor->getVertexCount(); inx++) {
				(*arr)[inx] = geoAdaptor->getVertex(inx);
			}

			any->newData<std::vector<glm::vec3>>(fl::type::FLOAT_3, arr);
			any->setIsArray(true);

			return any;
		}
			break;
		case FINAL_POINT_POSITION:
		{
			auto arr = new std::vector<glm::vec3>;
			arr->resize(geoAdaptor->getVertexCount());

			for (size_t inx = 0; inx < geo->vertexBufferSize(); inx++) {
				(*arr)[inx] = geo->getVertex(inx);
			}

			any->newData<std::vector<glm::vec3>>(fl::type::FLOAT_3, arr);
			any->setIsArray(true);

			return any;
		}
			break;
		case BASE_POINT_NORMAL:
		{
			auto arr = new std::vector<glm::vec3>;
			arr->resize(geoAdaptor->getVertexCount());

			for (size_t inx = 0; inx < geoAdaptor->getVertexCount(); inx++) {
				(*arr)[inx] = geoAdaptor->getSmoothNormal(inx);
			}

			any->newData<std::vector<glm::vec3>>(fl::type::FLOAT_3, arr);
			any->setIsArray(true);

			return any;
		}
			break;
		case FINAL_POINT_NORMAL:
		{
			auto arr = new std::vector<glm::vec3>;
			arr->resize(geoAdaptor->getVertexCount());

			for (size_t inx = 0; inx < geoAdaptor->getVertexCount(); inx++) {
				(*arr)[inx] = geoAdaptor->getSmoothNormal(inx);
			}

			any->newData<std::vector<glm::vec3>>(fl::type::FLOAT_3, arr);
			any->setIsArray(true);

			return any;
		}
			break;
		case SHAPE:
			return nullptr;
			break;
		case POSITION:
		{
			any->newData<glm::vec3>(fl::type::FLOAT_3);
			any->setData<glm::vec3>(geo->position());
			return any;
		}
			break;
		case ROTATION:
		{
			any->newData<glm::vec3>(fl::type::FLOAT_3);
			any->setData<glm::vec3>(geo->rotation());
			return any;
		}
			break;
		case SCALE:
		{
			any->newData<glm::vec3>(fl::type::FLOAT_3);
			any->setData<glm::vec3>(geo->scale());
			return any;
		}
			break;
		default:
			break;
		}
	}

	return nullptr;
}

//std::unique_ptr<fl::Any> DfxMeshAttributes::getValueAt(size_t) {
//	std::unique_ptr<fl::Any>(new fl::Any);
//}

size_t DfxMeshAttributes::getValueCount() {

	if (_attrib_id == BASE_POINT_POSITION  || 
		_attrib_id == FINAL_POINT_POSITION ||
		_attrib_id == BASE_POINT_NORMAL    ||
		_attrib_id == FINAL_POINT_NORMAL
		) {

		DfxObjectHandler handler;

		if (handler.fetch(path())) {

			auto geo = handler.geoNode();
			auto geoAdaptor = geo->getGeometryAdapter();

			return geoAdaptor->getVertexCount();
		}
		
	}

	return 0;
}

void DfxMeshAttributes::setValue(fl::Any *val) {
	DfxObjectHandler handler;

	if (handler.fetch(path())) {

		auto geo = handler.geoNode();
		auto geoAdaptor = geo->getGeometryAdapter();

		std::unique_ptr<fl::Any> any(new fl::Any);

		switch (_attrib_id) {
		case FINAL_POINT_POSITION:
		{
			if (val->isArray()) {
				auto arr = val->rawData<std::vector<glm::vec3>>();

				if (geo->vertexBufferSize() <= (*arr).size()) {
					for (size_t inx = 0; inx < geo->vertexBufferSize(); inx++) {
						geo->setVertex(inx, (*arr)[inx]);
					}
				}
			}
		}
		break;
		case POSITION:
		{
			geo->setPosition(val->data<glm::vec3>());
		}
		break;
		case ROTATION:
		{
			geo->setRotation(val->data<glm::vec3>());
		}
		break;
		case SCALE:
		{
			geo->setScale(val->data<glm::vec3>());
		}
		break;
		default:
			break;
		}
	}
}
//
//void DfxMeshAttributes::setValueAt(size_t, std::unique_ptr<fl::Any> val) {
//	// pass
//}
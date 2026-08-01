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

#include "DfxRotateWidget.h"

DfxRotateWidget::DfxRotateWidget() {
	uniformSize = 0.5f;

	createXHandle();
	createYHandle();
	createZHandle();
}


DfxRotateWidget::~DfxRotateWidget() {
	
}

void DfxRotateWidget::createXHandle() {
	deepsg::SgShapeBuffer shBuffer;
	std::vector<glm::vec3> points;

	float sc = uniformSize;
	float hdHt = sc * 4.f;
	float tlHt = hdHt * 5.42f;
	float tlWd = sc * 0.5f;
	float thFactor = 0.96f;

	shBuffer.setColor(255, 0, 0);

	//glm::mat4 rotMat = getRotationMat4(0);

	glm::vec3 tmp;

	for (int i = 0; i <= 30; i++) {
		double angle = 2 *dfxPI*i / 30;
		double x = cos(angle)*tlHt;
		double y = sin(angle)*tlHt;

		tmp = math::rotate90Z(x, y, tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
		tmp = math::rotate90Z(x, y, -tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
	}

	for (int i = 0; i <= 30; i++) {
		double angle = 2 * dfxPI*i / 30;
		double x = cos(angle)*(tlHt * thFactor);
		double y = sin(angle)*(tlHt * thFactor);

		tmp = math::rotate90Z(x, y, -tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
		tmp = math::rotate90Z(x, y, tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
	}

	for (int i = 0; i <= 30; i++) {
		double angle = 2 * dfxPI*i / 30;
		double x = cos(angle)*(tlHt);
		double y = sin(angle)*(tlHt);

		tmp = math::rotate90Z(x, y, -tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
		tmp = math::rotate90Z(x * thFactor, y * thFactor, -tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
	}

	for (int i = 0; i <= 30; i++) {
		double angle = 2 * dfxPI*i / 30;
		double x = cos(angle)*(tlHt);
		double y = sin(angle)*(tlHt);

		tmp = math::rotate90Z(x * thFactor, y * thFactor, tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
		tmp = math::rotate90Z(x, y, tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
	}

	shBuffer.drawQuadStrip(points);
	
	_handles.push_back(shBuffer);
}

void DfxRotateWidget::createYHandle() {
	deepsg::SgShapeBuffer shBuffer;
	std::vector<glm::vec3> points;

	float sc = uniformSize;
	float hdHt = sc * 4.f;
	float tlHt = hdHt * 6.0f;
	float tlWd = sc * 0.5f;
	float thFactor = 0.96f;

	shBuffer.setColor(0, 255, 0);

	//glm::mat4 rotMat = getRotationMat4(1);

	for (int i = 0; i <= 30; i++) {
		double angle = 2 * dfxPI*i / 30;
		double x = cos(angle)*tlHt;
		double y = sin(angle)*tlHt;

		points.push_back(glm::vec3(x, y, tlWd));
		points.push_back(glm::vec3(x, y, -tlWd));
	}

	for (int i = 0; i <= 30; i++) {
		double angle = 2 * dfxPI*i / 30;
		double x = cos(angle)*(tlHt * thFactor);
		double y = sin(angle)*(tlHt * thFactor);

		points.push_back(glm::vec3(x, y, -tlWd));
		points.push_back(glm::vec3(x, y, tlWd));
	}

	for (int i = 0; i <= 30; i++) {
		double angle = 2 * dfxPI*i / 30;
		double x = cos(angle)*(tlHt);
		double y = sin(angle)*(tlHt);

		points.push_back(glm::vec3(x, y, -tlWd));
		points.push_back(glm::vec3(x * thFactor, y * thFactor, -tlWd));

	}

	for (int i = 0; i <= 30; i++) {
		double angle = 2 * dfxPI*i / 30;
		double x = cos(angle)*(tlHt);
		double y = sin(angle)*(tlHt);

		points.push_back(glm::vec3(x * thFactor, y * thFactor, tlWd));
		points.push_back(glm::vec3(x, y, tlWd));

	}

	shBuffer.drawQuadStrip(points);

	_handles.push_back(shBuffer);

}

void DfxRotateWidget::createZHandle() {
	deepsg::SgShapeBuffer shBuffer;
	std::vector<glm::vec3> points;

	float sc = uniformSize;
	float hdHt = sc * 4.f;
	float tlHt = hdHt * 4.8f;
	float tlWd = sc * 0.5f;
	float thFactor = 0.96f;

	shBuffer.setColor(0, 0, 255);

	//glm::mat4 rotMat = getRotationMat4(0);

	glm::vec3 tmp;

	for (int i = 0; i <= 30; i++) {
		double angle = 2 * dfxPI*i / 30;
		double x = cos(angle)*tlHt;
		double y = sin(angle)*tlHt;

		tmp = math::rotate90Y(x, y, tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
		tmp = math::rotate90Y(x, y, -tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
	}

	for (int i = 0; i <= 30; i++) {
		double angle = 2 * dfxPI*i / 30;
		double x = cos(angle)*(tlHt * thFactor);
		double y = sin(angle)*(tlHt * thFactor);

		tmp = math::rotate90Y(x, y, -tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
		tmp = math::rotate90Y(x, y, tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
	}

	for (int i = 0; i <= 30; i++) {
		double angle = 2 * dfxPI*i / 30;
		double x = cos(angle)*(tlHt);
		double y = sin(angle)*(tlHt);

		tmp = math::rotate90Y(x, y, -tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
		tmp = math::rotate90Y(x * thFactor, y * thFactor, -tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
	}

	for (int i = 0; i <= 30; i++) {
		double angle = 2 * dfxPI*i / 30;
		double x = cos(angle)*(tlHt);
		double y = sin(angle)*(tlHt);

		tmp = math::rotate90Y(x * thFactor, y * thFactor, tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
		tmp = math::rotate90Y(x, y, tlWd);
		points.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
	}

	shBuffer.drawQuadStrip(points);

	_handles.push_back(shBuffer);
}

glm::mat4 DfxRotateWidget::getRotationMat4(int handle, deepsg::SgXForm transform) {
	switch (transform.rotationType())
	{
	case deepsg::XYZ:
	{
		// x = 2, y = 0, z = 1
		if (handle == 0) {
			return glm::rotate(glm::mat4(), glm::radians(transform.rotation().x), glm::vec3(1.f, .0f, .0f));
		}
		else if (handle == 1) {
			auto trans = glm::rotate(glm::mat4(), glm::radians(transform.rotation().x), glm::vec3(1.f, .0f, .0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(transform.rotation().y), glm::vec3(0.f, 1.0f, .0f));
			return trans;
		}
		else if (handle == 2) {
			return glm::mat4(1);
		}
		else {
			return glm::mat4(1);
		}
	}
	break;
	case deepsg::XZY:
	{
		// x = 2, y = 0, z = 1
		if (handle == 0) {
			auto trans = glm::rotate(glm::mat4(), glm::radians(transform.rotation().x), glm::vec3(1.f, .0f, .0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(transform.rotation().z), glm::vec3(0.f, 0.0f, 1.0f));
			return trans;
		}
		else if (handle == 1) {
			return glm::rotate(glm::mat4(), glm::radians(transform.rotation().x), glm::vec3(1.f, .0f, .0f));
		}
		else if (handle == 2) {
			return glm::mat4(1);
		}
		else {
			return glm::mat4(1);
		}
	}
	break;
	case  deepsg::YXZ:
	{
		// x = 2, y = 0, z = 1
		if (handle == 0) {
			return glm::mat4(1);
		}
		else if (handle == 1) {
			auto trans = glm::rotate(glm::mat4(), glm::radians(transform.rotation().y), glm::vec3(0.f, 1.0f, .0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(transform.rotation().x), glm::vec3(1.f, 0.0f, .0f));
			return trans;
		}
		else if (handle == 2) {
			return glm::rotate(glm::mat4(), glm::radians(transform.rotation().y), glm::vec3(0.f, 1.0f, .0f));
		}
		else {
			return glm::mat4(1);
		}
	}
	break;
	case  deepsg::YZX:
	{
		// x = 2, y = 0, z = 1
		if (handle == 0) {
			return glm::mat4(1);
		}
		else if (handle == 1) {
			return glm::rotate(glm::mat4(), glm::radians(transform.rotation().y), glm::vec3(0.f, 1.0f, .0f));
		}
		else if (handle == 2) {
			auto trans = glm::rotate(glm::mat4(), glm::radians(transform.rotation().y), glm::vec3(0.f, 1.0f, .0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(transform.rotation().z), glm::vec3(0.f, 0.0f, 1.0f));
			return trans;
		}
		else {
			return glm::mat4(1);
		}
	}
	break;
	case  deepsg::ZXY:
	{
		// x = 2, y = 0, z = 1
		if (handle == 0) {
			auto trans = glm::rotate(glm::mat4(), glm::radians(transform.rotation().z), glm::vec3(0.f, 0.0f, 1.0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(transform.rotation().x), glm::vec3(1.f, 0.0f, 0.0f));
			return trans;
		}
		else if (handle == 1) {

			return glm::mat4(1);
		}
		else if (handle == 2) {

			return glm::rotate(glm::mat4(), glm::radians(transform.rotation().z), glm::vec3(0.f, 0.0f, 1.0f));
		}
		else {
			return glm::mat4(1);
		}
	}
	break;
	case  deepsg::ZYX:
	{
		// x = 2, y = 0, z = 1
		if (handle == 0) {
			return glm::rotate(glm::mat4(), glm::radians(transform.rotation().z), glm::vec3(0.f, 0.0f, 1.0f));
		}
		else if (handle == 1) {

			return glm::mat4(1);
		}
		else if (handle == 2) {
			auto trans = glm::rotate(glm::mat4(), glm::radians(transform.rotation().z), glm::vec3(0.f, 0.0f, 1.0f));
			trans *= glm::rotate(glm::mat4(), glm::radians(transform.rotation().y), glm::vec3(0.f, 1.0f, 0.0f));
			return trans;

		}
		else {
			return glm::mat4(1);
		}
	}
	break;
	default:
		return glm::mat4(1.f);
		break;
	}

	return glm::mat4(1);
}

glm::mat4 DfxRotateWidget::handleTransform(int handle, deepsg::SgXForm parentTransform) {

	auto trans = glm::translate(glm::mat4(1.f), parentTransform.position());
	trans *= getRotationMat4(handle, parentTransform);

	return trans;
}

glm::mat4 DfxRotateWidget::calacRotationMat(int handle, DfxWidgetInterface::OnMoveInfo info) {
	float offsetX = 0.f;
	float offsetY = 0.f;
	float offsetZ = 0.f;

	if (handle == X) {
		float offset00 = info.screenPosX_current - info.screenPosX_previous;
		float offset01 = info.screenPosY_previous - info.screenPosY_current;

		offsetX = (offset00 + offset01) / 2.f;
	}
	else if (handle == Y) {
		float offset00 = info.screenPosX_current - info.screenPosX_previous;
		float offset01 = info.screenPosY_current - info.screenPosY_previous;

		offsetY = (offset00 + offset01) / 2.f;
	}
	else if (handle == Z) {
		float offset00 = info.screenPosX_previous - info.screenPosX_current;
		float offset01 = info.screenPosY_current - info.screenPosY_previous;

		offsetZ = (offset00 + offset01) / 2.f;
	}

	glm::vec3 angle(offsetX, offsetY, offsetZ);

	auto mat = glm::translate(pivot);
	mat *= glm::rotate(glm::radians(offsetZ), glm::vec3(1.f, 0.f, 0.f));
	mat *= glm::rotate(glm::radians(offsetX), glm::vec3(0.f, 1.f, 0.f));
	mat *= glm::rotate(glm::radians(-offsetY), glm::vec3(0.f, 0.f, 1.f));
	mat *= glm::translate((-1.f * pivot));

	return mat;
}

void DfxRotateWidget::onMoveObjectMode(int handle, DfxWidgetInterface::OnMoveInfo info) {
	int X = 0, Y = 1, Z = 2;
	
	if (DfxWidgetManager::getInstance()->isWidgetActive() &&
		!DfxWidgetManager::getInstance()->isNull()) {

		auto transform = DfxWidgetManager::getInstance()->getTransform();

		if (handle == X) {

			float offset00 = info.screenPosX_current - info.screenPosX_previous;
			float offset01 = info.screenPosY_previous - info.screenPosY_current;

			float offset = (offset00 + offset01) / 2.f;

			// Apply Rotation			
			for (auto n : DfxSceneSelectionModel::getInstance()->selected()) {
				if (n->xFormProperties() != nullptr) {
					glm::vec3 iRot = n->rotation();
					n->setRotation(glm::vec3(iRot.x, (iRot.y + (offset * .5f)), iRot.z));
					n->notifyObservers();
				}
			}		
						
		}
		else if (handle == Y) {

			float offset00 = info.screenPosX_current - info.screenPosX_previous;
			float offset01 = info.screenPosY_current - info.screenPosY_previous;

			float offset = (offset00 + offset01) / 2.f;

			// Apply Rotation
			for (auto n : DfxSceneSelectionModel::getInstance()->selected()) {
				if (n->xFormProperties() != nullptr) {
					glm::vec3 iRot = n->rotation();
					n->setRotation(glm::vec3(iRot.x, iRot.y, (iRot.z - (offset * .5f))));
					n->notifyObservers();
				}
			}
		
		}
		else if (handle == Z) {

			float offset00 = info.screenPosX_previous - info.screenPosX_current;
			float offset01 = info.screenPosY_current - info.screenPosY_previous;

			float offset = (offset00 + offset01) / 2.f;

			// Apply Rotation
			for (auto n : DfxSceneSelectionModel::getInstance()->selected()) {
				if (n->xFormProperties() != nullptr) {
					glm::vec3 iRot = n->rotation();
					n->setRotation(glm::vec3((iRot.x + (offset * .5f)), iRot.y, iRot.z));
					n->notifyObservers();
				}
			}
					
		}
		
	}
}

void DfxRotateWidget::onMoveVertexMode(int handle, DfxWidgetInterface::OnMoveInfo info) {
	int X = 0, Y = 1, Z = 2;

	if (DfxWidgetManager::getInstance()->isWidgetActive() &&
		!DfxWidgetManager::getInstance()->isNull()) {

		auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

		if (geonode->getGeometryAdapter()->type() == DfxDeepSimplexGeoAdapter::AdapterType) {
			auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
			auto mesh = adapter->getMesh()->resource<ds::Mesh>();

			for (auto i : DfxMeshSelectionModel::getInstance()->selectedVertexIndices()) {
				
				glm::vec3 pos = mesh->vertices()[i]->position();				
				
				mesh->vertices()[i]->position() = glm::vec3(calacRotationMat(handle, info) * glm::vec4(pos.x, pos.y, pos.z, 1.0));		
			}

			DfxMeshSelectionModel::getInstance()->calcFaceCenter(mesh);
		}
	}
}

void DfxRotateWidget::onMoveEdgeMode(int handle, DfxWidgetInterface::OnMoveInfo info) {
	if (DfxWidgetManager::getInstance()->isWidgetActive() &&
		!DfxWidgetManager::getInstance()->isNull()) {

		auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

		if (geonode->getGeometryAdapter()->type() == DfxDeepSimplexGeoAdapter::AdapterType) {
			auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
			auto mesh = adapter->getMesh()->resource<ds::Mesh>();
			std::unordered_map<size_t, ds::Vert*> vertMap;

			for (auto i : DfxMeshSelectionModel::getInstance()->selectedEdgeIndices()) {
				std::pair<size_t, ds::Vert*> p00(mesh->edges()[i]->v00()->getUID(), mesh->edges()[i]->v00());
				std::pair<size_t, ds::Vert*> p01(mesh->edges()[i]->v01()->getUID(), mesh->edges()[i]->v01());

				vertMap.insert(p00);
				vertMap.insert(p01);
			}

			for (auto v : vertMap) {
				auto pos00 = v.second->position();
				auto mat = calacRotationMat(handle, info);
				v.second->position() = glm::vec3(mat * glm::vec4(pos00.x, pos00.y, pos00.z, 1.0));;
			}

			DfxMeshSelectionModel::getInstance()->calcEdgeCenter(mesh);
		}

	}
}

void DfxRotateWidget::onMoveFaceMode(int handle, DfxWidgetInterface::OnMoveInfo info) {
	if (DfxWidgetManager::getInstance()->isWidgetActive() &&
		!DfxWidgetManager::getInstance()->isNull()) {

		auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

		if (geonode->getGeometryAdapter()->type() == DfxDeepSimplexGeoAdapter::AdapterType) {
			auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
			auto mesh = adapter->getMesh()->resource<ds::Mesh>();
			std::unordered_map<size_t, ds::Vert*> vertMap;

			for (auto v : DfxMeshSelectionModel::getInstance()->selectedFaceIndices()) {
				glm::vec3 center(0);

				for (int i = 0; i < mesh->faces()[v]->verticesCount(); i++) {
					std::pair<size_t, ds::Vert*> p00(mesh->faces()[v]->vertices()[i]->getUID(), mesh->faces()[v]->vertices()[i]);
					vertMap.insert(p00);
				}
			}

			for (auto v : vertMap) {
				glm::vec3 pos = v.second->position();
				v.second->position() = glm::vec3(calacRotationMat(handle, info) * glm::vec4(pos.x, pos.y, pos.z, 1.0));
			}

			DfxMeshSelectionModel::getInstance()->calcFaceCenter(mesh);
		}
	}
}

void DfxRotateWidget::onClickVertexMode(int handle, DfxWidgetInterface::OnClickInfo info) {
	if (DfxWidgetManager::getInstance()->isWidgetActive() &&
		!DfxWidgetManager::getInstance()->isNull()) {

		auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

		if (geonode->getGeometryAdapter()->type() == DfxDeepSimplexGeoAdapter::AdapterType) {
			auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
			auto mesh = adapter->getMesh()->resource<ds::Mesh>();
			glm::vec3 pos(0);

			// Update Vertex
			for (auto i : DfxMeshSelectionModel::getInstance()->selectedVertexIndices()) {
				pos += mesh->vertices()[i]->position();
			}

			pivot = pos / (float)DfxMeshSelectionModel::getInstance()->selectedVertexIndices().size();
		}
	}
}

void DfxRotateWidget::onClickEdgeMode(int handle, DfxWidgetInterface::OnClickInfo info) {

	if (DfxWidgetManager::getInstance()->isWidgetActive() &&
		!DfxWidgetManager::getInstance()->isNull()) {

		auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

		if (geonode->getGeometryAdapter()->type() == DfxDeepSimplexGeoAdapter::AdapterType) {
			auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
			auto mesh = adapter->getMesh()->resource<ds::Mesh>();

			glm::vec3 _edgeCenter = glm::vec3(0);

			for (auto v : DfxMeshSelectionModel::getInstance()->selectedEdgeIndices()) {
				auto center = (mesh->edges()[v]->v00()->position() + mesh->edges()[v]->v01()->position()) / 2.f;
				_edgeCenter += center;
			}

			pivot = _edgeCenter / (float)DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().size();

		}
	}
}

void DfxRotateWidget::onClickFaceMode(int handle, DfxWidgetInterface::OnClickInfo info) {

	if (DfxWidgetManager::getInstance()->isWidgetActive() &&
		!DfxWidgetManager::getInstance()->isNull()) {

		auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();

		if (geonode->getGeometryAdapter()->type() == DfxDeepSimplexGeoAdapter::AdapterType) {
			auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
			auto mesh = adapter->getMesh()->resource<ds::Mesh>();
			glm::vec3 _faceCenter = glm::vec3(0);

			for (auto v : DfxMeshSelectionModel::getInstance()->selectedFaceIndices()) {
				glm::vec3 center(0);

				for (int i = 0; i < mesh->faces()[v]->verticesCount(); i++) {
					center += mesh->faces()[v]->vertices()[i]->position();
				}

				center = center / (float)mesh->faces()[v]->verticesCount();
				_faceCenter += center;
			}

			pivot = _faceCenter / (float)DfxMeshSelectionModel::getInstance()->selectedFaceIndices().size();
		}
	}
}

void DfxRotateWidget::onMove(int handle, DfxWidgetInterface::OnMoveInfo info) {
	
	if (DfxMode::getInstance()->mode() == DfxMode::Mode::OBJECT_EDIT) {
		onMoveObjectMode(handle, info);
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::VERTEX_EDIT) {
		onMoveVertexMode(handle, info);
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::EDGE_EDIT) {
		onMoveEdgeMode(handle, info);
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::FACE_EDIT) {
		onMoveFaceMode(handle, info);
	}
}

void DfxRotateWidget::onClick(int handle, DfxWidgetInterface::OnClickInfo info) {
	if (DfxMode::getInstance()->mode() == DfxMode::Mode::OBJECT_EDIT) {
		return;
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::VERTEX_EDIT) {
		onClickVertexMode(handle, info);
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::EDGE_EDIT) {
		onClickEdgeMode(handle, info);
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::FACE_EDIT) {
		onClickFaceMode(handle, info);
	}
}


void DfxRotateWidget::onRelease(int handle, DfxWidgetInterface::OnReleaseInfo info) {

}
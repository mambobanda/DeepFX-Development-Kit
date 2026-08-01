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

#include "DfxTranslateWidget.h"

DfxTranslateWidget::DfxTranslateWidget() {
	uniformSize = 0.5f;

	createXHandle();
	createYHandle();
	createZHandle();
}

DfxTranslateWidget::~DfxTranslateWidget() {
	
}

void DfxTranslateWidget::createXHandle() {

	deepsg::SgShapeBuffer shBuffer;

	float sc = uniformSize;
	float hdHt = sc * 4.f;
	float tlHt = hdHt * 7.f;
	float tlWd = sc * 0.4f;

	glm::vec3 p1, p2, p3, p4;

	shBuffer.setColor(255, 0, 0);

	// Head 
	p1 = math::rotate90X(-sc, tlHt, -sc);
	p2 = math::rotate90X(0.0, hdHt + tlHt, 0.0);
	p3 = math::rotate90X(sc, tlHt, -sc);
	shBuffer.drawTriangle(p1, p2, p3);

	p1 = math::rotate90X(-sc, tlHt, sc);
	p2 = math::rotate90X(0.0, hdHt + tlHt, 0.0);
	p3 = math::rotate90X(-sc, tlHt, -sc);
	shBuffer.drawTriangle(p1, p2, p3);

	p1 = math::rotate90X(sc, tlHt, sc);
	p2 = math::rotate90X(0.0, hdHt + tlHt, 0.0);
	p3 = math::rotate90X(-sc, tlHt, sc);
	shBuffer.drawTriangle(p1, p2, p3);

	p1 = math::rotate90X(sc, tlHt, -sc);
	p2 = math::rotate90X(0.0, hdHt + tlHt, 0.0);
	p3 = math::rotate90X(sc, tlHt, sc);
	shBuffer.drawTriangle(p1, p2, p3);

	p1 = math::rotate90X(sc, tlHt, sc);
	p2 = math::rotate90X(-sc, tlHt, sc);
	p3 = math::rotate90X(-sc, tlHt, -sc);
	shBuffer.drawTriangle(p1, p2, p3);

	p1 = math::rotate90X(-sc, tlHt, -sc);
	p2 = math::rotate90X(sc, tlHt, -sc);
	p3 = math::rotate90X(sc, tlHt, sc);
	shBuffer.drawTriangle(p1, p2, p3);

	// Tail
	p1 = math::rotate90X(-tlWd, hdHt, tlWd);
	p2 = math::rotate90X(tlWd, hdHt, tlWd);
	p3 = math::rotate90X(tlWd, tlHt, tlWd);
	p4 = math::rotate90X(-tlWd, tlHt, tlWd);
	shBuffer.drawQuad(p1, p2, p3, p4);

	p1 = math::rotate90X(tlWd, hdHt, -tlWd);
	p2 = math::rotate90X(-tlWd, hdHt, -tlWd);
	p3 = math::rotate90X(-tlWd, tlHt, -tlWd);
	p4 = math::rotate90X(tlWd, tlHt, -tlWd);
	shBuffer.drawQuad(p1, p2, p3, p4);

	p1 = math::rotate90X(-tlWd, hdHt, -tlWd);
	p2 = math::rotate90X(-tlWd, hdHt, tlWd);
	p3 = math::rotate90X(-tlWd, tlHt, tlWd);
	p4 = math::rotate90X(-tlWd, tlHt, -tlWd);
	shBuffer.drawQuad(p1, p2, p3, p4);

	p1 = math::rotate90X(tlWd, hdHt, tlWd);
	p2 = math::rotate90X(tlWd, hdHt, -tlWd);
	p3 = math::rotate90X(tlWd, tlHt, -tlWd);
	p4 = math::rotate90X(tlWd, tlHt, tlWd);
	shBuffer.drawQuad(p1, p2, p3, p4);

	_handles.push_back(shBuffer);
}

void DfxTranslateWidget::createYHandle() {

	deepsg::SgShapeBuffer shBuffer;

	float sc = uniformSize;
	float hdHt = sc * 4.f;
	float tlHt = hdHt * 7.f;
	float tlWd = sc * 0.4f;

	shBuffer.setColor(0, 255, 0);

	// Head
	shBuffer.drawTriangle(
		glm::vec3(-sc, tlHt, -sc),
		glm::vec3(0.0, hdHt + tlHt, 0.0),
		glm::vec3(sc, tlHt, -sc));

	shBuffer.drawTriangle(
		glm::vec3(-sc, tlHt, sc),
		glm::vec3(0.0, hdHt + tlHt, 0.0),
		glm::vec3(-sc, tlHt, -sc));

	shBuffer.drawTriangle(
		glm::vec3(sc, tlHt, sc),
		glm::vec3(0.0, hdHt + tlHt, 0.0),
		glm::vec3(-sc, tlHt, sc));

	shBuffer.drawTriangle(
		glm::vec3(sc, tlHt, -sc),
		glm::vec3(0.0, hdHt + tlHt, 0.0),
		glm::vec3(sc, tlHt, sc));

	shBuffer.drawTriangle(
		glm::vec3(sc, tlHt, sc),
		glm::vec3(-sc, tlHt, sc),
		glm::vec3(-sc, tlHt, -sc));

	shBuffer.drawTriangle(
		glm::vec3(-sc, tlHt, -sc),
		glm::vec3(sc, tlHt, -sc),
		glm::vec3(sc, tlHt, sc));

	// Tail
	shBuffer.drawQuad(
		glm::vec3(-tlWd, hdHt, tlWd),
		glm::vec3(tlWd, hdHt, tlWd),
		glm::vec3(tlWd, tlHt, tlWd),
		glm::vec3(-tlWd, tlHt, tlWd));

	shBuffer.drawQuad(
		glm::vec3(tlWd, hdHt, -tlWd),
		glm::vec3(-tlWd, hdHt, -tlWd),
		glm::vec3(-tlWd, tlHt, -tlWd),
		glm::vec3(tlWd, tlHt, -tlWd));

	shBuffer.drawQuad(
		glm::vec3(-tlWd, hdHt, -tlWd),
		glm::vec3(-tlWd, hdHt, tlWd),
		glm::vec3(-tlWd, tlHt, tlWd),
		glm::vec3(-tlWd, tlHt, -tlWd));
	
	shBuffer.drawQuad(
		glm::vec3(tlWd, hdHt, tlWd),
		glm::vec3(tlWd, hdHt, -tlWd),
		glm::vec3(tlWd, tlHt, -tlWd),
		glm::vec3(tlWd, tlHt, tlWd));

	_handles.push_back(shBuffer);
}

void DfxTranslateWidget::createZHandle() {
	deepsg::SgShapeBuffer shBuffer;

	float sc = uniformSize;
	float hdHt = sc * 4.f;
	float tlHt = hdHt * 7.f;
	float tlWd = sc * 0.4f;

	glm::vec3 p1, p2, p3, p4;

	shBuffer.setColor(0, 0, 255);

	// Head 
	p1 = math::rotate90Z(-sc, tlHt, -sc);
	p2 = math::rotate90Z(0.0, hdHt + tlHt, 0.0);
	p3 = math::rotate90Z(sc, tlHt, -sc);
	shBuffer.drawTriangle(p1, p2, p3);

	p1 = math::rotate90Z(-sc, tlHt, sc);
	p2 = math::rotate90Z(0.0, hdHt + tlHt, 0.0);
	p3 = math::rotate90Z(-sc, tlHt, -sc);
	shBuffer.drawTriangle(p1, p2, p3);

	p1 = math::rotate90Z(sc, tlHt, sc);
	p2 = math::rotate90Z(0.0, hdHt + tlHt, 0.0);
	p3 = math::rotate90Z(-sc, tlHt, sc);
	shBuffer.drawTriangle(p1, p2, p3);

	p1 = math::rotate90Z(sc, tlHt, -sc);
	p2 = math::rotate90Z(0.0, hdHt + tlHt, 0.0);
	p3 = math::rotate90Z(sc, tlHt, sc);
	shBuffer.drawTriangle(p1, p2, p3);

	p1 = math::rotate90Z(sc, tlHt, sc);
	p2 = math::rotate90Z(-sc, tlHt, sc);
	p3 = math::rotate90Z(-sc, tlHt, -sc);
	shBuffer.drawTriangle(p1, p2, p3);

	p1 = math::rotate90Z(-sc, tlHt, -sc);
	p2 = math::rotate90Z(sc, tlHt, -sc);
	p3 = math::rotate90Z(sc, tlHt, sc);
	shBuffer.drawTriangle(p1, p2, p3);

	// Tail
	p1 = math::rotate90Z(-tlWd, hdHt, tlWd);
	p2 = math::rotate90Z(tlWd, hdHt, tlWd);
	p3 = math::rotate90Z(tlWd, tlHt, tlWd);
	p4 = math::rotate90Z(-tlWd, tlHt, tlWd);
	shBuffer.drawQuad(p1, p2, p3, p4);

	p1 = math::rotate90Z(tlWd, hdHt, -tlWd);
	p2 = math::rotate90Z(-tlWd, hdHt, -tlWd);
	p3 = math::rotate90Z(-tlWd, tlHt, -tlWd);
	p4 = math::rotate90Z(tlWd, tlHt, -tlWd);
	shBuffer.drawQuad(p1, p2, p3, p4);

	p1 = math::rotate90Z(-tlWd, hdHt, -tlWd);
	p2 = math::rotate90Z(-tlWd, hdHt, tlWd);
	p3 = math::rotate90Z(-tlWd, tlHt, tlWd);
	p4 = math::rotate90Z(-tlWd, tlHt, -tlWd);
	shBuffer.drawQuad(p1, p2, p3, p4);

	p1 = math::rotate90Z(tlWd, hdHt, tlWd);
	p2 = math::rotate90Z(tlWd, hdHt, -tlWd);
	p3 = math::rotate90Z(tlWd, tlHt, -tlWd);
	p4 = math::rotate90Z(tlWd, tlHt, tlWd);
	shBuffer.drawQuad(p1, p2, p3, p4);

	_handles.push_back(shBuffer);
}

glm::mat4 DfxTranslateWidget::handleTransform(int handle, deepsg::SgXForm parentTransform) {
	return glm::translate(glm::mat4(1.f), parentTransform.position());
}

glm::vec3 DfxTranslateWidget::calcDelta(int handle, DfxWidgetInterface::OnMoveInfo info) {
	// namespace OnMoveInfo = DfxWidgetInterface::OnMoveInfo;

	int X = 0, Y = 1, Z = 2;
	glm::vec3 delta(0);

	if (info.type == DfxWidgetInterface::OnMoveInfo::RAY) {

		gte::Ray3<float> r2_prev;
		gte::Ray3<float> r2_current;

		auto transform = DfxWidgetManager::getInstance()->getTransform();
		glm::vec3 npos = transform.position();
		//deepsg::SgShapeNode* snode = nullptr;

		// Previous Camera Ray
		r2_prev.direction = gte::Vector<3, float>{ info.previous.direction.x , info.previous.direction.y, info.previous.direction.z };
		r2_prev.origin = gte::Vector<3, float>{ info.previous.origin.x , info.previous.origin.y, info.previous.origin.z };

		// Current Camera Ray
		r2_current.direction = gte::Vector<3, float>{ info.current.direction.x , info.current.direction.y, info.current.direction.z };
		r2_current.origin = gte::Vector<3, float>{ info.current.origin.x , info.current.origin.y, info.current.origin.z };

		if (handle == X) {
			gte::Ray3<float> r1;

			// Y Normal
			r1.direction = gte::Vector<3, float>{ 1.0f, 0.f, 0.0f };
			r1.origin = gte::Vector<3, float>{ npos.x - 10.0f, npos.y, npos.z };

			// Find closet points on rays
			gte::DCPRay3Ray3<float> qry;
			auto result_prev = qry(r1, r2_prev);
			auto result_current = qry(r1, r2_current);

			glm::vec3 prev_intersection(result_prev.closestPoint[0][0], result_prev.closestPoint[0][1], result_prev.closestPoint[0][2]);
			glm::vec3 current_intersection(result_current.closestPoint[0][0], result_current.closestPoint[0][1], result_current.closestPoint[0][2]);

			// find the displacement
			delta = current_intersection - prev_intersection;
		}
		else if (handle == Y) {
			gte::Ray3<float> r1;

			// Y Normal
			r1.direction = gte::Vector<3, float>{ 0.0f, 1.f, 0.0f };
			r1.origin = gte::Vector<3, float>{ npos.x, npos.y - 10.f, npos.z };

			// Find closet points on rays
			gte::DCPRay3Ray3<float> qry;
			auto result_prev = qry(r1, r2_prev);
			auto result_current = qry(r1, r2_current);

			glm::vec3 prev_intersection(result_prev.closestPoint[0][0], result_prev.closestPoint[0][1], result_prev.closestPoint[0][2]);
			glm::vec3 current_intersection(result_current.closestPoint[0][0], result_current.closestPoint[0][1], result_current.closestPoint[0][2]);

			// find the displacement
			delta = current_intersection - prev_intersection;


		}
		else if (handle == Z) {
			gte::Ray3<float> r1;

			// Z Normal
			r1.direction = gte::Vector<3, float>{ 0.0f, 0.f, 1.0f };
			r1.origin = gte::Vector<3, float>{ npos.x, npos.y, npos.z - 10.0f };

			// Find closet points on rays
			gte::DCPRay3Ray3<float> qry;
			auto result_prev = qry(r1, r2_prev);
			auto result_current = qry(r1, r2_current);

			glm::vec3 prev_intersection(result_prev.closestPoint[0][0], result_prev.closestPoint[0][1], result_prev.closestPoint[0][2]);
			glm::vec3 current_intersection(result_current.closestPoint[0][0], result_current.closestPoint[0][1], result_current.closestPoint[0][2]);

			// find the displacement
			delta = current_intersection - prev_intersection;
		}
	}
	else if (info.type == DfxWidgetInterface::OnMoveInfo::SCREEN) {

		auto transform = DfxWidgetManager::getInstance()->getTransform();
		glm::vec3 npos = transform.position();

		float weigtingDistance = glm::distance(npos, info.current.origin) / 8.48528099f;

		float weight = 1.f/125.f * weigtingDistance;
		auto x_disp = (info.screenPosX_current - info.screenPosX_previous) * weight;
		auto y_disp = (info.screenPosY_current - info.screenPosY_previous) * weight;

		if (handle == X) {
			if (info.view == DfxWidgetInterface::OnMoveInfo::TOP || info.view == DfxWidgetInterface::OnMoveInfo::BOTTOM ||
				info.view == DfxWidgetInterface::OnMoveInfo::FRONT) {

				delta = glm::vec3(x_disp, 0.f, 0.f);
			}
			else if (info.view == DfxWidgetInterface::OnMoveInfo::BACK) {

				delta = glm::vec3(-1.f * x_disp, 0.f, 0.f);
			}
		}
		else if (handle == Y) {
			if (info.view == DfxWidgetInterface::OnMoveInfo::LEFT || info.view == DfxWidgetInterface::OnMoveInfo::RIGHT ||
				info.view == DfxWidgetInterface::OnMoveInfo::FRONT || info.view == DfxWidgetInterface::OnMoveInfo::BACK) {

				delta = glm::vec3(0.f, -1.f * y_disp, 0.f);
			}
		}
		else if (handle == Z) {
			if (info.view == DfxWidgetInterface::OnMoveInfo::LEFT) {
				delta = glm::vec3(0.f, 0.f, -1.f * x_disp);
			}
			else if (info.view == DfxWidgetInterface::OnMoveInfo::RIGHT) {
				delta = glm::vec3(0.f, 0.f, x_disp);
			}
			else if (info.view == DfxWidgetInterface::OnMoveInfo::TOP) {
				delta = glm::vec3(0.f, 0.f, y_disp);
			}
			else if (info.view == DfxWidgetInterface::OnMoveInfo::BOTTOM) {
				delta = glm::vec3(0.f, 0.f, -1.f * y_disp);
			}
		}

	}

	return delta;
}

void DfxTranslateWidget::onMoveObjectMode(int handle, DfxWidgetInterface::OnMoveInfo info) {
	
	if (DfxWidgetManager::getInstance()->isWidgetActive() &&
		!DfxWidgetManager::getInstance()->isNull()) {

		// Apply displacement
		auto delta = calcDelta(handle, info);
		for(auto n: DfxSceneSelectionModel::getInstance()->selected()){
			if (n->xFormProperties() != nullptr) {
				auto newPos = n->position() + delta;
				n->setPosition(newPos);
				n->notifyObservers();
			}
		}

	}
}

void DfxTranslateWidget::onMoveVertexMode(int handle, DfxWidgetInterface::OnMoveInfo info) {
	
	if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedVertexIndices().empty()) {

		if (DfxWidgetManager::getInstance()->isWidgetActive() &&
			!DfxWidgetManager::getInstance()->isNull()) {

			auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();
			auto delta = calcDelta(handle, info);

			if (geonode->getGeometryAdapter()->type() == DfxDeepSimplexGeoAdapter::AdapterType) {
				auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
				auto mesh = adapter->getMesh()->resource<ds::Mesh>();

				// Update Vertex
				for (auto i : DfxMeshSelectionModel::getInstance()->selectedVertexIndices()) {
					mesh->vertices()[i]->position() += delta;
				}

				DfxMeshSelectionModel::getInstance()->calcVertexCenter(mesh);
			}

		}

	}
}

void DfxTranslateWidget::onMoveEdgeMode(int handle, DfxWidgetInterface::OnMoveInfo info) {
	
	if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().empty()) {

		if (DfxWidgetManager::getInstance()->isWidgetActive() &&
			!DfxWidgetManager::getInstance()->isNull()) {

			auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();
			auto delta = calcDelta(handle, info);
			std::unordered_map<size_t, ds::Vert*> vertMap;

			if (geonode->getGeometryAdapter()->type() == DfxDeepSimplexGeoAdapter::AdapterType) {
				auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
				auto mesh = adapter->getMesh()->resource<ds::Mesh>();

				// Update Vertex
				for (auto i : DfxMeshSelectionModel::getInstance()->selectedEdgeIndices()) {
					std::pair<size_t, ds::Vert*> p00(mesh->edges()[i]->v00()->getUID(), mesh->edges()[i]->v00());
					std::pair<size_t, ds::Vert*> p01(mesh->edges()[i]->v01()->getUID(), mesh->edges()[i]->v01());

					vertMap.insert(p00);
					vertMap.insert(p01);		
				}

				for (auto v : vertMap) {
					v.second->position() += delta;
				}

				DfxMeshSelectionModel::getInstance()->calcEdgeCenter(mesh);
			}

		}

	}
}

void DfxTranslateWidget::onMoveFaceMode(int handle, DfxWidgetInterface::OnMoveInfo info) {
	if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedFaceIndices().empty()) {

		if (DfxWidgetManager::getInstance()->isWidgetActive() &&
			!DfxWidgetManager::getInstance()->isNull()) {

			auto geonode = DfxMeshSelectionModel::getInstance()->nodeReference();
			auto delta = calcDelta(handle, info);
			std::unordered_map<size_t, ds::Vert*> vertMap;

			if (geonode->getGeometryAdapter()->type() == DfxDeepSimplexGeoAdapter::AdapterType) {
				auto adapter = (DfxDeepSimplexGeoAdapter*)geonode->getGeometryAdapter();
				auto mesh = adapter->getMesh()->resource<ds::Mesh>();

				// Update Vertex
				for (auto i : DfxMeshSelectionModel::getInstance()->selectedFaceIndices()) {

					for (int j = 0; j < mesh->faces()[i]->verticesCount(); j++) {
						std::pair<size_t, ds::Vert*> p00(mesh->faces()[i]->vertices()[j]->getUID(), mesh->faces()[i]->vertices()[j]);
						vertMap.insert(p00);
					}
				
				}

				for (auto v : vertMap) {
					v.second->position() += delta;
				}

				DfxMeshSelectionModel::getInstance()->calcFaceCenter(mesh);
			}

		}

	}
}

void DfxTranslateWidget::onMove(int handle, DfxWidgetInterface::OnMoveInfo info) {
	
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

void DfxTranslateWidget::onClick(int handle, DfxWidgetInterface::OnClickInfo info) {
	
}

void DfxTranslateWidget::onRelease(int handle, DfxWidgetInterface::OnReleaseInfo info) {
	
}
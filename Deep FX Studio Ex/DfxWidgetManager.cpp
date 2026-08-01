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

#include "DfxWidgetManager.h"

DfxWidgetManager *DfxWidgetManager::_instance = nullptr;

DfxWidgetManager::DfxWidgetManager() {
	widget = nullptr;
}

DfxWidgetManager::~DfxWidgetManager() {

}

DfxWidgetManager *DfxWidgetManager::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxWidgetManager;
	}

	return _instance;
}

void DfxWidgetManager::setWidget(deepsg::SgWidgetNode *wg) {
	widget = wg;
}

deepsg::SgWidgetNode *DfxWidgetManager::getWidget() {
	return widget;
}

deepsg::SgXForm DfxWidgetManager::getTransform() {

	if (DfxMode::getInstance()->mode() == DfxMode::Mode::OBJECT_EDIT) {
		
		if (!DfxSceneSelectionModel::getInstance()->selected().empty()) {

			glm::vec3 pos(0);

			int nodeCounter = 0;
			for (auto n : DfxSceneSelectionModel::getInstance()->selected()) {
			
				if (n->xFormProperties() != nullptr) {
					glm::mat4 transformation = deepsg::SgRootNode::getInstance()->getTransformation4x4(n); // your transformation matrix.
					glm::vec3 scale;
					glm::quat rotation;
					glm::vec3 translation;
					glm::vec3 skew;
					glm::vec4 perspective;
					glm::decompose(transformation, scale, rotation, translation, skew, perspective);

					pos += translation;

					nodeCounter++;
				}
			}

			if (nodeCounter != 0) {
				pos = pos / (float)nodeCounter;
			}

			deepsg::SgXForm transform;
			transform.position() = pos;
			transform.rotation() = DfxSceneSelectionModel::getInstance()->selected()[0]->rotation();
			transform.scale() = DfxSceneSelectionModel::getInstance()->selected()[0]->scale();
			transform.rotationType() = DfxSceneSelectionModel::getInstance()->selected()[0]->rotationType();

			return transform;
		}
		else {
			return deepsg::SgXForm();
		}

	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::VERTEX_EDIT) {
		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedVertexIndices().empty()) {
			glm::mat4 mat = deepsg::SgRootNode::getInstance()->getTransformation4x4(DfxMeshSelectionModel::getInstance()->nodeReference());
			deepsg::SgXForm transform;
			transform.position() = glm::vec3(mat * glm::vec4(DfxMeshSelectionModel::getInstance()->vertexCenter(), 1.0f));

			return transform;
		}
		else {
			return deepsg::SgXForm();
		}
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::EDGE_EDIT) {
		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().empty()) {
			glm::mat4 mat = deepsg::SgRootNode::getInstance()->getTransformation4x4(DfxMeshSelectionModel::getInstance()->nodeReference());
			deepsg::SgXForm transform;
			transform.position() = glm::vec3(mat * glm::vec4(DfxMeshSelectionModel::getInstance()->edgeCenter(), 1.0f));

			return transform;
		}
		else {
			return deepsg::SgXForm();
		}
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::FACE_EDIT) {
		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedFaceIndices().empty()) {
			glm::mat4 mat = deepsg::SgRootNode::getInstance()->getTransformation4x4(DfxMeshSelectionModel::getInstance()->nodeReference());
			deepsg::SgXForm transform;
			transform.position() = glm::vec3(mat * glm::vec4(DfxMeshSelectionModel::getInstance()->faceCenter(), 1.0f));

			return transform;
		}
		else {
			return deepsg::SgXForm();
		}
	}
	else {
		return deepsg::SgXForm();
	}
}

void DfxWidgetManager::clear() {
	if (widget != nullptr) {

		DfxWidgetHandler handle;
		bool rc = handle.fetch(widget->referencePath());

		if (rc) {
			handle.destroy();
		}

		widget = nullptr;
	}
}

void DfxWidgetManager::updateObjectModeWidgets() {

}

bool DfxWidgetManager::isWidgetActive() {

	if (DfxMode::getInstance()->mode() == DfxMode::Mode::OBJECT_EDIT) {
		if (!DfxSceneSelectionModel::getInstance()->selected().empty()) {

			for (auto selected : DfxSceneSelectionModel::getInstance()->selected()) {
				if (selected->xFormProperties() != nullptr) {
					return true;
				}
			}

			return false;
		}
		else return false;
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::VERTEX_EDIT) {
		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedVertexIndices().empty()) return true;
		else return false;
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::EDGE_EDIT) {
		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedEdgeIndices().empty()) return true;
		else return false;
	}
	else if (DfxMode::getInstance()->mode() == DfxMode::Mode::FACE_EDIT) {
		if (DfxMeshSelectionModel::getInstance()->nodeReference() != nullptr && !DfxMeshSelectionModel::getInstance()->selectedFaceIndices().empty()) return true;
		else return false;
	}
	else {
		return false;
	}
}

bool DfxWidgetManager::isNull() {
	return (widget == nullptr);
}

void DfxWidgetManager::pushWidget() {
	widgetStack.push(widget);
	widget = nullptr;
}

void DfxWidgetManager::popWidget() {
	clear();

	if (!widgetStack.empty()) {
		widget = widgetStack.top();
		widgetStack.pop();
	}
}
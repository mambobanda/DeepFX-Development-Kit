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

#include <glm\gtx\normal.hpp>
#include "DfxObjectViewer.h"


DfxObjectViewer::DfxObjectViewer(DfxGlobalEntityName uniquename) :DfxViewItem(DFX_CUSTOM_ITEM, uniquename) {
	setLogicalName("Object Viewer");


	setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inRenderMode, DfxEntityPropertyState::INPUT_STATE);
	//setPropertyState(inDisplacementIndex, DfxEntityPropertyState::INPUT_STATE);
	
	((DfxEnum*)getProperty(inRenderMode))->add("Solid");
	((DfxEnum*)getProperty(inRenderMode))->add("Solid Wire");
	((DfxEnum*)getProperty(inRenderMode))->add("Wire");

}

DfxObjectViewer::~DfxObjectViewer() {

}

void DfxObjectViewer::compute() {
	
	getDataInstance()->makeClean();
}

void DfxObjectViewer::drawObject(Dfx3dMeshData *mdata) {
	int source = ((DfxEnum*)getProperty(inRenderMode))->getCurrentIndex();

	glBegin(GL_TRIANGLES);
	for (size_t inx = 0; inx < mdata->getIndexCount(); inx++) {

		auto vindex = mdata->getIndex(inx);
		glm::vec3 pos;

		pos = mdata->getVertex(vindex);

		glm::vec3 nom = mdata->getNormal(vindex);
		glm::vec3 col = mdata->getColor(vindex);

		glNormal3f(nom.x, nom.y, nom.z);

		if (source == 0) {
			glColor3f(col.x, col.y, col.z);
		}
		else {
			glColor3f(.0251f, .0251f, .0251f);
		}
		glVertex3f(pos.x, pos.y, pos.z);
	}
	glEnd();
}

void DfxObjectViewer::drawProxyObjects(DfxEntityPropertyTypes::ProxyObjectArray *pxs) {
	using namespace DfxEntityPropertyTypes;

	int source = ((DfxEnum*)getProperty(inRenderMode))->getCurrentIndex();

	if (!pxs->isNull()) {
		for (int pinx = 0; pinx < pxs->size(); pinx++) {

			ProxyObject *proxPtr = getProxyPointer(pinx, pxs);

			if (proxPtr != nullptr) {

				if (proxPtr->meshData.getVertexCount() > 0) {

					glm::vec3 selColor(1.f, 0.498f, 0.164);

					glPushMatrix();

					if (!propertyHasBinding(inDisplacementIndex)) {

						glm::vec3 pos = glm::vec3(proxPtr->position.x, proxPtr->position.y, proxPtr->position.z);
						glm::vec3 rot = glm::vec3(proxPtr->rotation.x, proxPtr->rotation.y, proxPtr->rotation.z);
						glm::vec3 scl = glm::vec3(proxPtr->scale.x, proxPtr->scale.y, proxPtr->scale.z);

						glm::mat4 transMat = createTransformationMatrix(pos, rot, scl);
						glMultMatrixf(&transMat[0][0]);
					}

					glPointSize(4);
					glBegin(GL_TRIANGLES);
					size_t counter = 0;
					for (size_t inx = 0; inx < proxPtr->meshData.getIndexCount() / 3; inx++) {

						auto vindex1 = proxPtr->meshData.getIndex(counter);
						auto vindex2 = proxPtr->meshData.getIndex(counter + 1);
						auto vindex3 = proxPtr->meshData.getIndex(counter + 2);

						glm::vec3 pos1;
						glm::vec3 pos2;
						glm::vec3 pos3;

						glm::vec3 col1;
						glm::vec3 col2;
						glm::vec3 col3;

						pos1 = proxPtr->meshData.getVertex(vindex1);
						pos2 = proxPtr->meshData.getVertex(vindex2);
						pos3 = proxPtr->meshData.getVertex(vindex3);

						if (source == 0) {
							
							col1 = proxPtr->meshData.getColor(vindex1);
							col2 = proxPtr->meshData.getColor(vindex2);
							col3 = proxPtr->meshData.getColor(vindex3);
						}
						else {

							col1 = glm::vec3(.0251f, .0251f, .0251f);
							col2 = glm::vec3(.0251f, .0251f, .0251f);
							col3 = glm::vec3(.0251f, .0251f, .0251f);
						}

						glm::vec3 nom = glm::triangleNormal(pos1, pos2, pos3);

						//glm::vec3 nom = proxPtr->meshData.getNormal(vindex);
						//glm::vec3 col = proxPtr->meshData.getColor(vindex);

						/*glNormal3f(nom.x, nom.y, nom.z);

						if (source == 0) {
							glColor3f(col.x, col.y, col.z);
						}
						else {
							glColor3f(.0251f, .0251f, .0251f);
						}*/

						glNormal3f(nom.x, nom.y, nom.z);

						glColor3f(col1.x, col1.y, col1.z);
						glVertex3f(pos1.x, pos1.y, pos1.z);

						glColor3f(col2.x, col2.y, col2.z);
						glVertex3f(pos2.x, pos2.y, pos2.z);

						glColor3f(col3.x, col3.y, col3.z);
						glVertex3f(pos3.x, pos3.y, pos3.z);

						counter += 3;
					}
					glEnd();

					glPopMatrix();
				}
			}
		}
	}
}

void DfxObjectViewer::drawProxies() {
	using namespace DfxEntityPropertyTypes;

	if (propertyHasBinding(inRef)) {

		UID *uid = (UID*)getProperty(inRef);
		ProxyObjectArray *pxs = dfxutil::findProxyMeshData(uid);
		Dfx3dMeshData *mesh = dfxutil::findMeshData(uid);

		if (pxs != nullptr) {
			drawProxyObjects(pxs);
		}
		else if (mesh != nullptr) {
			drawObject(mesh);
		}

	}
}

void DfxObjectViewer::draw() {
	initializeOpenGLFunctions();

	pingParents();
	if (!getDataInstance()->isClean()) {
		compute();
	}

	int   source = ((DfxEnum*)getProperty(inRenderMode))->getCurrentIndex();

	glShadeModel(GL_FLAT);
	switch (source)
	{
	case 0:
		drawProxies();
		break;
	case 1:
	{
		drawProxies();
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.5);
		drawProxies();
		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
	}
	break;
	case 2:
	{
		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.5);
		drawProxies();
		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_LIGHTING);
	}
	break;
	default:
		break;
	}
}

void DfxObjectViewer::calculateBounds() {
}

DfxEntityPropertyTypes::ProxyObject *DfxObjectViewer::getProxyPointer(size_t inx, DfxEntityPropertyTypes::ProxyObjectArray *pxs) {

	using namespace DfxEntityPropertyTypes;

	if (!pxs->isNull()) {

		if (inx >= 0 && inx < pxs->size()) {
			return &(*pxs)[inx];
		}
	}

	return nullptr;
}

glm::vec3 DfxObjectViewer::getDisplacementVertex(size_t proxid, size_t index) {

	using namespace DfxEntityPropertyTypes;

	ProxyObjectArray *pxs = (ProxyObjectArray*)getProperty(inRef);

	if (!pxs->isNull()) {

		if (proxid >= 0 && proxid < pxs->size()) {

			Dfx3dMeshData *mdata = &(*pxs)[proxid].meshData;
			size_t dispIndex = *(float*)getProperty(inDisplacementIndex);

			auto disp = (*pxs)[proxid].displacementMap->getDisplacement(dispIndex);

			if (disp == nullptr) disp = (*pxs)[proxid].displacementMap->getDisplacement(dispIndex - 1);

			if (disp != nullptr) {
				if (index >= 0 && index < disp->getCount()) {
					return disp->getPositions()[index];
				}
				else {
					return glm::vec3(0.f);
				}
			}
			else {
				if (index >= 0 && index < mdata->getVertexCount()) {
					return mdata->getVertex(index);
				}
				else {
					return glm::vec3(0.f);
				}
			}
		}
	}

	return glm::vec3(0.f);
}

glm::mat4 DfxObjectViewer::createTransformationMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) {

	glm::mat4 transMat = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, pos.z));

	transMat *= glm::rotate(glm::mat4(), glm::radians(rot.x), glm::vec3(1.f, .0f, .0f));
	transMat *= glm::rotate(glm::mat4(), glm::radians(rot.y), glm::vec3(0.f, 1.f, .0f));
	transMat *= glm::rotate(glm::mat4(), glm::radians(rot.z), glm::vec3(0.f, .0f, 1.f));

	transMat *= glm::scale(glm::mat4(), glm::vec3(scl.x, scl.y, scl.z));

	return transMat;
}

DfxBounds DfxObjectViewer::bounds() {
	return DfxBounds();
}

bool DfxObjectViewer::selectable() {
	return false;
}

void DfxObjectViewer::importData(std::vector<std::pair<std::string, std::string>> data) {

}

std::vector<std::pair<std::string, std::string>> DfxObjectViewer::exportData() {
	return mapEntityProperties(this);
}

bool DfxObjectViewer::isExportable() {
	return true;
}

void *DfxObjectViewer::creator(DfxGlobalEntityName uniquename) {
	return new DfxObjectViewer(uniquename);
}

void DfxObjectViewer::initialize() {
	DfxViewItem::initialize();

	DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
	DfxEntityDB::createProperty(inRenderMode, "render_mode", new DfxEntityPropertyTypes::DfxEnumPropertyType());
	DfxEntityDB::createProperty(inDisplacementIndex, "displacement_index", new DfxEntityPropertyTypes::FloatPropertyType());
}
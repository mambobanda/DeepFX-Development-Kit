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
#include "DfxCExplodeView.h"

namespace dfxcore {

	DfxCExplodeView::DfxCExplodeView(DfxGlobalEntityName uniquename) :DfxViewItem(DFX_CUSTOM_ITEM, uniquename) {
		setLogicalName("Explode View");

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inRenderMode, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inSpacing, DfxEntityPropertyState::INPUT_STATE);

		((DfxEnum*)getProperty(inRenderMode))->add("Solid");
		((DfxEnum*)getProperty(inRenderMode))->add("Solid Wire");
		((DfxEnum*)getProperty(inRenderMode))->add("Wire");

		*(float*)getProperty(inSpacing) = 1.f;

	}

	DfxCExplodeView::~DfxCExplodeView() {

	}

	void DfxCExplodeView::compute() {
		using namespace DfxEntityPropertyTypes;

		//centers.clear();

		//if (propertyHasBinding(inRef)) {

		//	UID *uid = (UID*)getProperty(inRef);
		//	ProxyObjectArray *pxs = dfxutil::findProxyMeshData(uid);

		//	if (pxs != nullptr) {

		//		for (int pinx = 0; pinx < pxs->size(); pinx++) {

		//			ProxyObject *proxPtr = getProxyPointer(pinx, pxs);

		//			if (proxPtr != nullptr) {

		//				if (proxPtr->meshData.getVertexCount() > 0) {
		//					glm::vec3 p = float3ToVec3(proxPtr->position);
		//					glm::vec3 r = float3ToVec3(proxPtr->rotation);
		//					glm::vec3 s = float3ToVec3(proxPtr->scale);
		//					glm::vec3 center = dfxutil::geo::meshCenter(proxPtr->meshData, p, r, s);
		//					centers.push_back(center);
		//				}
		//			}
		//		}

		//		DfxGlobal::console::println(std::to_string(centers.size()) + " Explode Parts.");
		//	}

		//}

		getDataInstance()->makeClean();
	}

	void DfxCExplodeView::drawProxies() {

		using namespace DfxEntityPropertyTypes;

		if (propertyHasBinding(inRef)) {

			UID *uid = (UID*)getProperty(inRef);
			float spacing = *(float*)getProperty(inSpacing);
			ProxyObjectArray *pxs = dfxutil::findProxyMeshData(uid);

			if (pxs == nullptr)
				return;

			int source = ((DfxEnum*)getProperty(inRenderMode))->getCurrentIndex();
			glm::vec3 proxCenter = proxyCenter(pxs);

			if (!pxs->isNull()) {

				for (int pinx = 0; pinx < pxs->size(); pinx++) {

					ProxyObject *proxPtr = getProxyPointer(pinx, pxs);
					
					if (proxPtr != nullptr) {

						if (proxPtr->meshData.getVertexCount() > 0) {

							glm::vec3 selColor(1.f, 0.498f, 0.164);
							glPushMatrix();
			
							glm::vec3 opos = glm::vec3(proxPtr->position.x, proxPtr->position.y, proxPtr->position.z);
							glm::vec3 pos = opos + (glm::normalize(opos - proxCenter) * spacing);
							glm::vec3 rot = glm::vec3(proxPtr->rotation.x, proxPtr->rotation.y, proxPtr->rotation.z);
							glm::vec3 scl = glm::vec3(proxPtr->scale.x, proxPtr->scale.y, proxPtr->scale.z);

							glm::mat4 transMat = createTransformationMatrix(pos, rot, scl);
							glMultMatrixf(&transMat[0][0]);
							
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
	}

	void DfxCExplodeView::draw() {
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

	void DfxCExplodeView::calculateBounds() {

	}

	DfxEntityPropertyTypes::ProxyObject * DfxCExplodeView::getProxyPointer(size_t inx, DfxEntityPropertyTypes::ProxyObjectArray *pxs) {
		using namespace DfxEntityPropertyTypes;

		if (!pxs->isNull()) {

			if (inx >= 0 && inx < pxs->size()) {
				return &(*pxs)[inx];
			}
		}

		return nullptr;
	}

	glm::vec3 DfxCExplodeView::getDisplacementVertex(size_t proxid, size_t index) {

		using namespace DfxEntityPropertyTypes;

		ProxyObjectArray *pxs = (ProxyObjectArray*)getProperty(inRef);

		if (!pxs->isNull()) {

			if (proxid >= 0 && proxid < pxs->size()) {

				Dfx3dMeshData *mdata = &(*pxs)[proxid].meshData;
				//size_t dispIndex = *(float*)getProperty(inDisplacementIndex);
				size_t dispIndex = 0;

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

	glm::mat4 DfxCExplodeView::createTransformationMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) {

		glm::mat4 transMat = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, pos.z));

		transMat *= glm::rotate(glm::mat4(), glm::radians(rot.x), glm::vec3(1.f, .0f, .0f));
		transMat *= glm::rotate(glm::mat4(), glm::radians(rot.y), glm::vec3(0.f, 1.f, .0f));
		transMat *= glm::rotate(glm::mat4(), glm::radians(rot.z), glm::vec3(0.f, .0f, 1.f));

		transMat *= glm::scale(glm::mat4(), glm::vec3(scl.x, scl.y, scl.z));

		return transMat;
	}

	DfxBounds DfxCExplodeView::bounds() {
		return DfxBounds();
	}

	bool DfxCExplodeView::selectable() {
		return false;
	}

	void DfxCExplodeView::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxCExplodeView::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxCExplodeView::isExportable() {
		return true;
	}

	void *DfxCExplodeView::creator(DfxGlobalEntityName uniquename) {
		return new DfxCExplodeView(uniquename);
	}

	void DfxCExplodeView::initialize() {
		DfxViewItem::initialize();

		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inSpacing, "in_spacing", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inRenderMode,"render_mode", new DfxEntityPropertyTypes::DfxEnumPropertyType());
		
	}

	glm::vec3 DfxCExplodeView::proxyCenter(DfxEntityPropertyTypes::ProxyObjectArray *pxs) {
		using namespace DfxEntityPropertyTypes;

		if (!pxs->isNull()) {

			glm::vec3 sum(0.f);
			size_t counter = 0;
			for (int pinx = 0; pinx < pxs->size(); pinx++) {

				ProxyObject *proxPtr = getProxyPointer(pinx, pxs);
				glm::vec3 proxCenter = glm::vec3(proxPtr->position.x, proxPtr->position.y, proxPtr->position.z);
				if (proxPtr != nullptr) {

					if (proxPtr->meshData.getVertexCount() > 0) {

						glm::vec3 pos = glm::vec3(proxPtr->position.x, proxPtr->position.y, proxPtr->position.z);
						glm::vec3 rot = glm::vec3(proxPtr->rotation.x, proxPtr->rotation.y, proxPtr->rotation.z);
						glm::vec3 scl = glm::vec3(proxPtr->scale.x, proxPtr->scale.y, proxPtr->scale.z);

						glm::mat4 transMat = createTransformationMatrix(pos, rot, scl);

						for (size_t inx = 0; inx < proxPtr->meshData.getVertexCount(); inx++) {

							sum += glm::vec3(transMat * glm::vec4(proxPtr->meshData.getVertex(inx), 1.f));
							counter++;
						}
					}

				
				}
			}

			if (counter > 0) {
				return (sum / (float)counter);
			}
		}

		return glm::vec3(0);
	}
}
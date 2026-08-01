/*
* The Mailo Today Solutions Software License
*
* Copyright 2016 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#include "DfxMeshXForm.h"

DfxMeshXForm::DfxMeshXForm(DfxGlobalEntityName uniquename) :DfxEntity(uniquename) {
	setLogicalName("Mesh XForm");

	DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outRef);
	uid->setRootID(getInstanceID());

	setPropertyState(mesh, DfxEntityPropertyState::DATA_STORE_STATE);

	setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);
	setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inPosition, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inRotation, DfxEntityPropertyState::INPUT_STATE);
	setPropertyState(inScale, DfxEntityPropertyState::INPUT_STATE);

	((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->x = 1.f;
	((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->y = 1.f;
	((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->z = 1.f;
}

DfxMeshXForm::~DfxMeshXForm() {

}

void DfxMeshXForm::compute() {
	using namespace DfxEntityPropertyTypes;

	Dfx3dMeshData *mdata = (Dfx3dMeshData*)getProperty(mesh);
	mdata->clearIndexBuffer();
	mdata->clearVertexBuffer();

	if (propertyHasBinding(inRef)) {

		UID *uid = (UID*)getProperty(inRef);

		Dfx3dMeshData *mesh = dfxutil::findMeshData(uid);

		if (mesh != nullptr) {
			mdata->copy(*mesh);

			glm::vec3 pos(((DfxEntityPropertyTypes::Float3*)getProperty(inPosition))->x,
					((DfxEntityPropertyTypes::Float3*)getProperty(inPosition))->y,
					((DfxEntityPropertyTypes::Float3*)getProperty(inPosition))->z);

			glm::vec3 rot(((DfxEntityPropertyTypes::Float3*)getProperty(inRotation))->x,
					 ((DfxEntityPropertyTypes::Float3*)getProperty(inRotation))->y,
					 ((DfxEntityPropertyTypes::Float3*)getProperty(inRotation))->z);

			glm::vec3 scale(((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->x,
					 ((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->y,
					 ((DfxEntityPropertyTypes::Float3*)getProperty(inScale))->z);
			
			glm::mat4 transMat = glm::translate(glm::mat4(1.f), pos);

			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rot.x), glm::vec3(1.f, .0f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rot.y), glm::vec3(0.f, 1.f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rot.z), glm::vec3(0.f, .0f, 1.f));

			transMat *= glm::scale(glm::mat4(1.f), scale);

			for (size_t inx = 0; inx < mdata->getVertexCount(); inx++) {
				glm::vec3 pos = mdata->getVertex(inx);
				mdata->setVertex(inx, glm::vec3(transMat * glm::vec4(pos, 1.f)));
			}
		}
	}

	makeChildrenDirty();
	getDataInstance()->makeClean();
}

void DfxMeshXForm::onDataChanged(DfxEntityDataInstance *instance) {

}

void DfxMeshXForm::importData(std::vector<std::pair<std::string, std::string>> data) {

}

std::vector<std::pair<std::string, std::string>> DfxMeshXForm::exportData() {
	return mapEntityProperties(this);
}

bool DfxMeshXForm::isExportable() {
	return true;
}

void *DfxMeshXForm::creator(DfxGlobalEntityName uniquename) {
	return new DfxMeshXForm(uniquename);
}

void DfxMeshXForm::initialize() {
	DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
	DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
	DfxEntityDB::createProperty(inPosition, "in_position", new DfxEntityPropertyTypes::Float3PropertyType());
	DfxEntityDB::createProperty(inRotation, "in_rotation", new DfxEntityPropertyTypes::Float3PropertyType());
	DfxEntityDB::createProperty(inScale, "in_scale", new DfxEntityPropertyTypes::Float3PropertyType());

	DfxEntityDB::createProperty(mesh, "@mesh_data", new DfxEntityPropertyTypes::Dfx3DMeshPropertyType());
}
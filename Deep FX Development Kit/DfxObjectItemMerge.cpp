
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

#include "DfxObjectItemMerge.h"

DfxMergeObjectItem::DfxMergeObjectItem(DfxGlobalEntityName uniquename) :DfxEntity(uniquename) {
	setLogicalName("Object Merger");

	DfxEntityPropertyTypes::UID *uid = (DfxEntityPropertyTypes::UID*)getProperty(outRef);
	uid->setRootID(getInstanceID());
	
	setPropertyState(mesh, DfxEntityPropertyState::DATA_STORE_STATE);

	setPropertyState(outRef, DfxEntityPropertyState::OUTPUT_STATE);
	setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
}

DfxMergeObjectItem::~DfxMergeObjectItem() {

}

void DfxMergeObjectItem::compute() {
	using namespace DfxEntityPropertyTypes;

	Dfx3dMeshData *mdata = (Dfx3dMeshData*)getProperty(mesh);
	mdata->clearIndexBuffer();
	mdata->clearVertexBuffer();

	if (propertyHasBinding(inRef)) {
		
		std::vector<TempMeshReference> references;

		std::vector<Dfx3dMeshData *> allMdata = findAllMeshData();
		std::vector<ProxyObjectArray *> allProxies = findAllProxyObjects();

		size_t index_counter = 0;
		for (auto dat : allMdata) {
			TempMeshReference t;
			t.index = index_counter;
			t.hasTransform = false;
			t.transform = glm::mat4();
			t.meshPtr = dat;

			references.push_back(t);

			index_counter++;
		}

		for (auto dat : allProxies) {

			if (!dat->isNull()) {

				for (size_t inx = 0; inx < dat->size(); inx++) {
					ProxyObject *proxPtr = &(*dat)[inx];

					glm::vec3 pos = glm::vec3(proxPtr->position.x, proxPtr->position.y, proxPtr->position.z);
					glm::vec3 rot = glm::vec3(proxPtr->rotation.x, proxPtr->rotation.y, proxPtr->rotation.z);
					glm::vec3 scl = glm::vec3(proxPtr->scale.x, proxPtr->scale.y, proxPtr->scale.z);

					glm::mat4 transMat = createTransformationMatrix(pos, rot, scl);

					TempMeshReference t;
					t.index = index_counter;
					t.hasTransform = true;
					t.transform = transMat;
					t.meshPtr = &proxPtr->meshData;

					references.push_back(t);

					index_counter++;
				}
			}
		}

		size_t totalVertices = 0;
		size_t totalIndicies = 0;

		if (references.size() > 0) {

			for (auto ref : references) {
				totalVertices += ref.meshPtr->getVertexCount();
				totalIndicies += ref.meshPtr->getIndexCount();
			}

			if (totalIndicies > 0 && totalVertices > 0) {

				mdata->resizeIndexBuffer(totalIndicies);
				mdata->resizeVertexBuffer(totalVertices);

				size_t OFFSET = 0;
				size_t INDEX_OFFSET = 0;

				for (auto ref : references) {
					
					for (size_t inj = 0; inj < ref.meshPtr->getVertexCount(); inj++) {
						glm::vec3 point = ref.meshPtr->getVertex(inj);

						if (ref.hasTransform) point = glm::vec3(ref.transform * glm::vec4(point, 1.f));

						mdata->setVertex(OFFSET + inj, point);
						mdata->setNormal(OFFSET + inj, ref.meshPtr->getNormal(inj));
						mdata->setColor(OFFSET + inj, ref.meshPtr->getColor(inj));
					}

					for (size_t inj = 0; inj < ref.meshPtr->getIndexCount(); inj++) {
						mdata->setIndex(INDEX_OFFSET + inj, OFFSET + ref.meshPtr->getIndex(inj));
					}

					OFFSET += ref.meshPtr->getVertexCount();
					INDEX_OFFSET += ref.meshPtr->getIndexCount();
				}
			}
		}
	}

	makeChildrenDirty();
	getDataInstance()->makeClean();
}

void DfxMergeObjectItem::onDataChanged(DfxEntityDataInstance *instance) {

}

void DfxMergeObjectItem::importData(std::vector<std::pair<std::string, std::string>> data) {

}

std::vector<std::pair<std::string, std::string>> DfxMergeObjectItem::exportData() {
	return mapEntityProperties(this);
}

bool DfxMergeObjectItem::isExportable() {
	return true;
}

void *DfxMergeObjectItem::creator(DfxGlobalEntityName uniquename) {
	return new DfxMergeObjectItem(uniquename);
}

void DfxMergeObjectItem::initialize() {
	DfxEntityDB::createProperty(outRef, "@ref", new DfxEntityPropertyTypes::UIDPropertyType());
	DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());

	DfxEntityDB::createProperty(mesh, "@mesh_data", new DfxEntityPropertyTypes::Dfx3DMeshPropertyType());
}

std::vector<DfxEntityPropertyTypes::ProxyObjectArray *> DfxMergeObjectItem::findAllProxyObjects() {
	using namespace DfxEntityPropertyTypes;

	std::vector<DfxEntityPropertyTypes::ProxyObjectArray *> objs;

	if (propertyHasBinding(inRef)) {
		std::vector<size_t> ids;

		UID *uid = (UID*)getProperty(inRef);

		ids.push_back(uid->getRootID());

		for (size_t inx = 0; inx < uid->childCount(); inx++) ids.push_back(uid->getChild(inx)->getRootID());

		for (auto _id : ids) {
			DfxEntityDataInstance *inst = DfxEntityDB::getDataInstance(_id);

			if (inst != nullptr) {

				DfxEntity  *ent = inst->getDfxEntity();

				if (ent->hasDataStoreProperty("@proxy_mesh_data")) {

					size_t iid = ent->dataStorePropertyID("@proxy_mesh_data");
					DfxEntityPropertyTypes::ProxyObjectArray *pdata = (DfxEntityPropertyTypes::ProxyObjectArray*)ent->getProperty(iid);

					objs.push_back(pdata);
				}
			}
		}

	}


	return objs;
}

std::vector<Dfx3dMeshData *> DfxMergeObjectItem::findAllMeshData() {
	using namespace DfxEntityPropertyTypes;

	std::vector<Dfx3dMeshData*> objs;

	if (propertyHasBinding(inRef)) {
		std::vector<size_t> ids;

		UID *uid = (UID*)getProperty(inRef);

		ids.push_back(uid->getRootID());

		for (size_t inx = 0; inx < uid->childCount(); inx++) ids.push_back(uid->getChild(inx)->getRootID());

		for (auto _id : ids) {
			DfxEntityDataInstance *inst = DfxEntityDB::getDataInstance(_id);

			if (inst != nullptr) {

				DfxEntity  *ent = inst->getDfxEntity();

				if (ent->hasDataStoreProperty("@mesh_data")) {

					size_t iid = ent->dataStorePropertyID("@mesh_data");
					Dfx3dMeshData *mdata = (Dfx3dMeshData*)ent->getProperty(iid);

					objs.push_back(mdata);
				}
			}
		}
	}

	return objs;
}

glm::mat4 DfxMergeObjectItem::createTransformationMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) {

	glm::mat4 transMat = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, pos.z));

	transMat *= glm::rotate(glm::mat4(), glm::radians(rot.x), glm::vec3(1.f, .0f, .0f));
	transMat *= glm::rotate(glm::mat4(), glm::radians(rot.y), glm::vec3(0.f, 1.f, .0f));
	transMat *= glm::rotate(glm::mat4(), glm::radians(rot.z), glm::vec3(0.f, .0f, 1.f));

	transMat *= glm::scale(glm::mat4(), glm::vec3(scl.x, scl.y, scl.z));

	return transMat;
}
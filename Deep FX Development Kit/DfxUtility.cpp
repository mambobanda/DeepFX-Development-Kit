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

#include "DfxUtility.h"


namespace dfxutil {

	Dfx3dMeshData *findMeshData(DfxEntityPropertyTypes::UID *uid) {

		DfxEntityDataInstance *inst = DfxEntityDB::getDataInstance(uid->getRootID());

		if (inst != nullptr) {

			DfxEntity  *ent = inst->getDfxEntity();

			if (ent->hasDataStoreProperty("@mesh_data")) {

				size_t iid = ent->dataStorePropertyID("@mesh_data");
				Dfx3dMeshData *mdata = (Dfx3dMeshData*)ent->getProperty(iid);

				return mdata;
			}
		}

		return nullptr;
	}

	DfxEntityPropertyTypes::ProxyObjectArray *findProxyMeshData(DfxEntityPropertyTypes::UID *uid) {

		DfxEntityDataInstance *inst = DfxEntityDB::getDataInstance(uid->getRootID());

		if (inst != nullptr) {

			DfxEntity  *ent = inst->getDfxEntity();

			if (ent->hasDataStoreProperty("@proxy_mesh_data")) {

				size_t iid = ent->dataStorePropertyID("@proxy_mesh_data");
				DfxEntityPropertyTypes::ProxyObjectArray *pdata = (DfxEntityPropertyTypes::ProxyObjectArray*)ent->getProperty(iid);

				return pdata;
			}
		}

		return nullptr;
	}

	std::vector<DfxEntity*> traitUIDFindAll(DfxEntityPropertyTypes::UID &uid) {
		using namespace DfxEntityPropertyTypes;

		std::vector<DfxEntity*> objs;

		DfxTraitUIDTraversalActions t_actions;

		t_actions.next_id = [&objs](size_t id) {

			auto dinstance = DfxEntityDB::getDataInstance(id);

			if (dinstance != nullptr) {
				DfxEntity *ent = dinstance->getDfxEntity();
				objs.push_back(ent);
			}

			return 0;
		};

		t_actions.traverse_children = [](size_t id) {
			return true;
		};	

		uid.traverseUIDTree(t_actions);	

		return objs;
	}

	std::vector<DfxEntity*> traitUIDSearch(DfxEntityPropertyTypes::UID &uid, DfxGlobalEntityName uname, DfxGlobalEntityName uname_limit, bool limit_search) {
		using namespace DfxEntityPropertyTypes;

		std::vector<DfxEntity*> objs;

		if (uname.size() > 0) {

			DfxTraitUIDTraversalActions t_actions;

			t_actions.next_id = [&objs, uname](size_t id) {

				auto dinstance = DfxEntityDB::getDataInstance(id);

				if (dinstance != nullptr) {
					DfxEntity *ent = dinstance->getDfxEntity();

					if (ent->UniqueName() == uname) {
						objs.push_back(ent);
					}
				}
				return 0;
			};

			if (limit_search) {
				t_actions.traverse_children = [uname_limit](size_t id) {

					auto dinstance = DfxEntityDB::getDataInstance(id);

					if (dinstance != nullptr) {
						DfxEntity *ent = dinstance->getDfxEntity();

						if (ent->UniqueName() == uname_limit) {
							return false;
						}
					}

					return true;
				};
			}
			else {
				t_actions.traverse_children = [uname_limit](size_t id) {
					return true;
				};
			}

			uid.traverseUIDTree(t_actions);
		}

		return objs;
	}

	void traitUIDLinkRelated(DfxEntity *ent, DfxPropertyID id) {
		using namespace DfxEntityPropertyTypes;

		if (ent->getPropertyType(id) == TRAIT_UID_TYPE) {

			UID *uid = (UID*)ent->getProperty(id);
			uid->clearChildren();

			auto properties = ent->getDataInstance()->getPropertyList();

			for (auto pid : properties) {

				if (ent->getDataInstance()->getPropertyInstance(pid)->getState() == DfxEntityPropertyState::INPUT_STATE) {
					
					if (ent->getPropertyType(pid) == TRAIT_UID_TYPE) {
						UID *_uid_ = (UID*)ent->getProperty(pid);
						uid->addChild(_uid_);
					}
				}
			}
		}
	}

	namespace geo {

		glm::mat4 createTransformationMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) {

			glm::mat4 transMat = glm::translate(glm::mat4(), glm::vec3(pos.x, pos.y, pos.z));

			transMat *= glm::rotate(glm::mat4(), glm::radians(rot.x), glm::vec3(1.f, .0f, .0f));
			transMat *= glm::rotate(glm::mat4(), glm::radians(rot.y), glm::vec3(0.f, 1.f, .0f));
			transMat *= glm::rotate(glm::mat4(), glm::radians(rot.z), glm::vec3(0.f, .0f, 1.f));

			transMat *= glm::scale(glm::mat4(), glm::vec3(scl.x, scl.y, scl.z));

			return transMat;
		}

		glm::vec3 meshCenter(Dfx3dMeshData &mesh, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) {
			DfxBounds bounds;

			glm::mat4 transMat = createTransformationMatrix(pos, rot, scl);
		

			for (size_t inx = 0; inx < mesh.getVertexCount(); inx++) {

				glm::vec3 p = glm::vec3((glm::vec4(mesh.getVertex(inx), 1) * transMat));
				bounds.add(p.x, p.y, p.z);
			}

			return bounds.center();
		}

		glm::vec3 moveMeshToOrigin(Dfx3dMeshData &mesh) {
			DfxBounds bounds;

			for (size_t inx = 0; inx < mesh.getVertexCount(); inx++) {

				glm::vec3 p = mesh.getVertex(inx);
				bounds.add(p.x, p.y, p.z);
			}

			glm::vec3 pos = bounds.center();

			for (size_t inx = 0; inx < mesh.getVertexCount(); inx++) {

				glm::vec3 p = mesh.getVertex(inx);
				glm::vec3 newp = p - pos;

				//bounds.add(newp.x, newp.y, newp.z);
				mesh.setVertex(inx, newp);
			}

			return pos;
		}
	}
}
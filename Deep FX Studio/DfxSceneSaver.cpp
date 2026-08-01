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

#include "DfxSceneSaver.h"

DfxSceneSaver::DfxSceneSaver() {

}

void DfxSceneSaver::makeUnreferencedObjectsPersistent(std::string path) {
	filesystem::path p(path);
	filesystem::create_directory(p.parent_path().string() + "/" + p.stem().string() + "_objects");

	for (size_t inx = 0; inx < DfxLayerManager::getInstance()->layerCount(); inx++) {

		DfxLayerItem *item = DfxLayerManager::getInstance()->getLayer(inx);

		for (size_t jnx = 0; jnx < item->childCount(); jnx++) {

			DfxLayerItem *node = item->getChild(jnx);
			bool importNode = node->getNode()->getNodeType() == DfxQNodeType::IMPORT_NODE ? true : false;

			if (!importNode) {
				DfxEntity *ent = node->getNode()->getEntity();

				if (ent->UniqueName() == DfxObjectItem_Identifier) {
					DfxObjectItem *obji = (DfxObjectItem*)ent;
					
					if (!obji->isFileReferenced()) {
						Dfx3dMeshData *mesh = (Dfx3dMeshData*)ent->getProperty(DfxObjectItem::mesh);

						std::string path = std::string(p.parent_path().string() + "/" + p.stem().string() + "_objects/") + std::string(ent->getLogicalName()) + ".obj";
						*(std::string*)obji->getProperty(DfxObjectItem::inFilePath) = path;
						
						IOUtilities::meshDataToObj(path, mesh);
					}
				}
			}
		}
	}
}

void DfxSceneSaver::save(std::string path) {
	using namespace dfxscenewriter;

	std::ofstream file(path);

	dfxscenewriter::tools::SceneWriter wtr(&file);

	wtr.beginSceneObject("root"); {

		wtr.writeString("format_name", "scenefx");
		wtr.writeString("format_version", "1");

		for (size_t inx = 0; inx < DfxLayerManager::getInstance()->layerCount(); inx++) {

			DfxLayerItem *item = DfxLayerManager::getInstance()->getLayer(inx);

			wtr.beginLayerObject( tools::qreplace(item->getName(), ' ', '/') ); {

				wtr.writeInt("linked",  item->isLinked()  ? 1 : 0);
				wtr.writeInt("visible", item->isVisible() ? 1 : 0);
				wtr.writeInt("locked",  item->isLocked()  ? 1 : 0);

				for (size_t jnx = 0; jnx < item->childCount(); jnx++) {

					DfxLayerItem *node = item->getChild(jnx);
					bool importNode = node->getNode()->getNodeType() == DfxQNodeType::IMPORT_NODE ? true : false;

					wtr.beginNodeObject(tools::qreplace(node->getName(), ' ', '/')); {

						wtr.writeInt("F2X25_isImportNode", importNode ? 1 : 0);
						wtr.writeInt("F2X25_isItemView", (node->getNodeType() == DfxLayerNodeType::VIEW_NODE) ? 1 : 0);
						wtr.writeString("F2X25_unique_name", node->getNode()->getEntity()->UniqueName());
						wtr.writeString("F2X25_logical_name", tools::qreplace(node->getNode()->getEntity()->getLogicalName(), ' ', '_'));
						wtr.writeSize_t("F2X25_reference_id", node->getNode()->getEntity()->getInstanceID());
						wtr.writeInt("F2X25_graphPosX", node->getNode()->pos().x());
						wtr.writeInt("F2X25_graphPosY", node->getNode()->pos().y());

						if (!importNode) {

							if (node->getNodeType() == DfxLayerNodeType::VIEW_NODE) {
								wtr.writeInt("F2X25_hidden", ((DfxViewItem*)node->getNode()->getEntity())->isHidden() ? 1 : 0);
							}
							else {
								wtr.writeInt("F2X25_hidden", 0);
							}

							wtr.writeInt("F2X25_locked", 0);

							auto properties = mapEntityProperties(node->getNode()->getEntity());

							for (auto prop : properties) {
								wtr.writeString(prop.first, prop.second);
							}
						}

						auto _slots = node->getNode()->getAllSlots();

						for (auto slt : _slots) {
							if (slt->getSlotType() == DfxQNodeSlotType::OUTPUT_SLOT ||
								slt->getSlotType() == DfxQNodeSlotType::GROUP_OUTPUT_SLOT) {

								for (size_t i = 0; i < slt->getAllConnections().size(); i++) {
									auto conn = slt->getAllConnections()[i];

									wtr.beginConnectionObject(std::string("connection_") + std::to_string(i)); {
					
										wtr.writeSize_t("output", conn->slot1()->getPropertyID());

										if(conn->slot2()->getSlotType() == DfxQNodeSlotType::GROUP_INPUT_SLOT)
											wtr.writeSize_t("input", 1000);
										else
											wtr.writeSize_t("input", conn->slot2()->getPropertyID());

										wtr.writeSize_t("source", conn->slot1()->getParentEntity()->getInstanceID());
										wtr.writeSize_t("target", conn->slot2()->getParentEntity()->getInstanceID());
											
										wtr.endObject();
									}
								}
							}
						}

						wtr.endObject();
					}
				}

				wtr.endObject();
			}
		}

		wtr.endObject();
	}
}
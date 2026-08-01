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

#include "DfxXMLSceneSaver.h"
#include "DfxEnvelopeMap.h"
#include "DfxTime.h"


DfxXMLSceneSaver::DfxXMLSceneSaver() {

}

void DfxXMLSceneSaver::makeUnreferencedObjectsPersistent(std::string path) {
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

void DfxXMLSceneSaver::save(std::string path) {
	
	QFile file(path.c_str());

	QDomDocument document;

	QDomElement root = document.createElement("scene");
	root.setAttribute("format_name", "scenefx");
	root.setAttribute("format_version", 1);
	root.setAttribute("frame_count", DfxTime::getInstance()->getTotalFrames());

	for (size_t inx = 0; inx < DfxLayerManager::getInstance()->layerCount(); inx++) {

		DfxLayerItem *item = DfxLayerManager::getInstance()->getLayer(inx);

		QDomElement layer = document.createElement("layer");
		layer.setAttribute("name", item->getName().c_str());
		layer.setAttribute("linked", item->isLinked() ? 1 : 0);
		layer.setAttribute("visible", item->isVisible() ? 1 : 0);
		layer.setAttribute("locked", item->isLocked() ? 1 : 0);

		for (size_t jnx = 0; jnx < item->childCount(); jnx++) {

			DfxLayerItem *node = item->getChild(jnx);
			bool importNode = node->getNode()->getNodeType() == DfxQNodeType::IMPORT_NODE ? true : false;

			QDomElement xnode = document.createElement("node");
			xnode.setAttribute("name", node->getName().c_str());
			xnode.setAttribute("F2X25_isImportNode", importNode ? 1 : 0);
			xnode.setAttribute("F2X25_isItemView", (node->getNodeType() == DfxLayerNodeType::VIEW_NODE) ? 1 : 0);
			xnode.setAttribute("F2X25_unique_name", node->getNode()->getEntity()->UniqueName().c_str());
			xnode.setAttribute("F2X25_logical_name", node->getNode()->getEntity()->getLogicalName().c_str());
			xnode.setAttribute("F2X25_reference_id", node->getNode()->getEntity()->getInstanceID());
			xnode.setAttribute("F2X25_graphPosX", node->getNode()->pos().x());
			xnode.setAttribute("F2X25_graphPosY", node->getNode()->pos().y());

			if (!importNode) {

				if (node->getNodeType() == DfxLayerNodeType::VIEW_NODE) {
					xnode.setAttribute("F2X25_hidden", ((DfxViewItem*)node->getNode()->getEntity())->isHidden() ? 1 : 0);
				}
				else {
					xnode.setAttribute("F2X25_hidden", 0);
				}

				xnode.setAttribute("F2X25_locked", 0);

				auto properties = mapEntityProperties(node->getNode()->getEntity());

				for (auto prop : properties) {
					QDomElement xprop = document.createElement("property");
					xprop.setAttribute("name", prop.first.c_str());
					xprop.setAttribute("value", prop.second.c_str());

					xnode.appendChild(xprop);
				}

				QDomElement xchannels = document.createElement("channels");

				DfxEntityDataInstance *dinstance = node->getNode()->getEntity()->getDataInstance();
				size_t instanceid = node->getNode()->getEntity()->getInstanceID();

				std::vector<size_t> lst = dinstance->getPropertyList();

				for (auto id : lst) {
					if (DfxEnvelopeMap::getInstance()->hasEnvelope(instanceid, id)) {
												
						auto env = DfxEnvelopeMap::getInstance()->findEnvelope(instanceid, id);

						QDomElement xenvelope = document.createElement("envelope");
						xenvelope.setAttribute("property", id);
						xenvelope.setAttribute("type", env->getType());

						for (size_t i = 0; i < env->keyCount(); i++) {
							if (env->hasKey(i)) {
								if (env->getType() == DfxEntityPropertyTypes::iFLOAT_TYPE) {

									float value;
									env->getValue(i, value);

									QDomElement xkey = document.createElement("key");
									xkey.setAttribute("value", value);
									xkey.setAttribute("frame", i);

									xenvelope.appendChild(xkey);
								}
								else if (env->getType() == DfxEntityPropertyTypes::iFLOAT3_TYPE) {
									 
									DfxEntityPropertyTypes::Float3 value;
									env->getValue(i, value);

									QDomElement xkey = document.createElement("key");
									xkey.setAttribute("x", value.x);
									xkey.setAttribute("y", value.y);
									xkey.setAttribute("z", value.z);
									xkey.setAttribute("frame", i);

									xenvelope.appendChild(xkey);
								}
							}
						}
						xchannels.appendChild(xenvelope);
					}
				}
				xnode.appendChild(xchannels);
			}

			auto _slots = node->getNode()->getAllSlots();

			for (auto slt : _slots) {
				if (slt->getSlotType() == DfxQNodeSlotType::OUTPUT_SLOT ||
					slt->getSlotType() == DfxQNodeSlotType::GROUP_OUTPUT_SLOT) {

					for (size_t i = 0; i < slt->getAllConnections().size(); i++) {
						auto conn = slt->getAllConnections()[i];

						QDomElement xconnection = document.createElement("connection");

						xconnection.setAttribute("output", conn->slot1()->getPropertyID());

						if (conn->slot2()->getSlotType() == DfxQNodeSlotType::GROUP_INPUT_SLOT)
							xconnection.setAttribute("input", 1000);
						else
							xconnection.setAttribute("input", conn->slot2()->getPropertyID());

						xconnection.setAttribute("source", conn->slot1()->getParentEntity()->getInstanceID());
						xconnection.setAttribute("target", conn->slot2()->getParentEntity()->getInstanceID());

						xnode.appendChild(xconnection);
					}
				}
			}

			layer.appendChild(xnode);
		}
			
		root.appendChild(layer);
	}

	document.appendChild(root);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		DfxGlobal::console::println("Save scene error: Failed to open/create " + path);
	}
	else {
		QTextStream stream(&file);
		stream << document.toString();
		file.close();

		DfxGlobal::console::println("Save saved to " + path);
	}
}


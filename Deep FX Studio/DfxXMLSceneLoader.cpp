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

#include "DfxXMLSceneLoader.h"
#include "DfxEnvelopeMap.h"
#include "DfxTime.h"
#include "DfxFrameManipulator.h"
#include "DeepStudioGlobal.h"


DfxXMLSceneLoader::DfxXMLSceneLoader() {
}

void DfxXMLSceneLoader::loadScene(QDomElement &e) {

	if (e.hasAttribute("format_name") && e.hasAttribute("format_version")) {
		auto fname = e.attribute("format_name", "NULL");
		auto fversion = e.attribute("format_version", "NULL");
		auto frame_count = e.attribute("frame_count", "NULL").toInt();

		if (fname.compare("scenefx") != 0) {
			DfxGlobal::console::println("Load scene error: wrong format.");
			return;
		}

		if (fversion.toInt() != 1) {
			DfxGlobal::console::println("Load scene error: wrong format version.");
			return;
		}

		if (frame_count > 0) {
			auto frameManip = (DfxFrameManipulator*)DeepStudioGlobal::getInstance()->getFrameManipulator();
			frameManip->setFrameCount(frame_count);
		}

		loadLayers(e);
	}
	else {
		DfxGlobal::console::println("Load scene error: can't find scene format data.");
	}
}

void DfxXMLSceneLoader::loadLayers(QDomElement &e) {

	QDomNode dom_node = e.firstChild();
	while (!dom_node.isNull()) {
		QDomElement layer_e = dom_node.toElement();

		if (!layer_e.isNull()) {
			if (layer_e.tagName().compare("layer", Qt::CaseInsensitive) == 0) {
				
				bool linked = true;
				bool visible = true;
				bool locked = false;
				std::string name = "default";

				if (layer_e.hasAttribute("linked")) {
					linked = layer_e.attribute("linked").toInt() == 1 ? true : false;
				}

				if (layer_e.hasAttribute("visible")) {
					visible = layer_e.attribute("visible").toInt() == 1 ? true : false;
				}

				if (layer_e.hasAttribute("locked")) {
					locked = layer_e.attribute("locked").toInt() == 1 ? true : false;
				}

				if (layer_e.hasAttribute("name")) {
					name = layer_e.attribute("name").toStdString();
				}

				DfxLayerItem *item = DfxLayerManager::getInstance()->addNewLayer();
				item->setName(name);
				item->setLinked(linked);
				item->setHidden(visible);
				item->setLocked(locked);

				loadNodes(item, layer_e);
			}
		}
		
		dom_node = dom_node.nextSibling();
	}
}

void DfxXMLSceneLoader::loadNodes(DfxLayerItem *layer, QDomElement &e) {

	QDomNode dom_node = e.firstChild();
	while (!dom_node.isNull()) {
		QDomElement node_e = dom_node.toElement();

		if (!node_e.isNull()) {
			if (node_e.tagName().compare("node", Qt::CaseInsensitive) == 0) {

				bool isImportNode = false;
				std::string unique_name = "";
				std::string logical_name = "";
				double graphPosX = 0;
				double graphPosY = 0;
				size_t reference_id = 0;
				std::string name = "";
				bool isItemView = false;
				bool hidden = false;
				bool locked = false;

				if (node_e.hasAttribute("F2X25_isImportNode")) {
					isImportNode = node_e.attribute("F2X25_isImportNode").toInt() == 1 ? true : false;
				}
				else {
					DfxGlobal::console::println("Load scene error: Node is missing tag attribute");
					return;
				}

				if (node_e.hasAttribute("F2X25_isItemView")) {
					isItemView = node_e.attribute("F2X25_isItemView").toInt() == 1 ? true : false;
				}
				else {
					DfxGlobal::console::println("Load scene error: Node is missing tag attribute");
					return;
				}

				if (node_e.hasAttribute("F2X25_unique_name")) {
					unique_name = node_e.attribute("F2X25_unique_name").toStdString();
				}
				else {
					DfxGlobal::console::println("Load scene error: Node is missing tag attribute");
					return;
				}

				if (node_e.hasAttribute("F2X25_logical_name")) {
					logical_name = node_e.attribute("F2X25_logical_name").toStdString();
				}
				else {
					DfxGlobal::console::println("Load scene error: Node is missing tag attribute");
					return;
				}

				if (node_e.hasAttribute("F2X25_reference_id")) {
					reference_id = node_e.attribute("F2X25_reference_id").toULongLong();
				}
				else {
					DfxGlobal::console::println("Load scene error: Node is missing tag attribute");
					return;
				}

				if (node_e.hasAttribute("F2X25_graphPosX")) {
					graphPosX = node_e.attribute("F2X25_graphPosX").toDouble();
				}
				else {
					DfxGlobal::console::println("Load scene error: Node is missing tag attribute");
					return;
				}

				if (node_e.hasAttribute("F2X25_graphPosY")) {
					graphPosY = node_e.attribute("F2X25_graphPosY").toDouble();
				}
				else {
					DfxGlobal::console::println("Load scene error: Node is missing tag attribute");
					return;
				}
			
				DfxLayerItem *nodeitem = new DfxLayerItem;
				nodeitem->setItemType(DfxLayerItemType::NODE_ITEM);
				nodeitem->setNodeType(DfxLayerNodeType::GENERIC_NODE);

				if (!isImportNode) {

					if (unique_name.compare("2F2X252X_DFX_GROUP_NODE") == 0) {
						DfxEntity *ent = new DfxNodes::DfxGroupNode(DfxNodes::DfxGroupNode_Identifier);
						DfxQBaseNode *qnode = new DfxQGroupNode(ent);

						qnode->setLayerReference(nodeitem);
						nodeitem->setNode(qnode);			
					}
					else {
						DfxEntity *entity = DfxEntityDB::newEntityInstance(unique_name);

						DfxQBaseNode *qnode = new DfxQStandardNode(entity);

						qnode->setLayerReference(nodeitem);
						nodeitem->setNode(qnode);
					}

					if (node_e.hasAttribute("F2X25_hidden")) {
						hidden = node_e.attribute("F2X25_hidden").toInt() == 1 ? true : false;
					}

					if (node_e.hasAttribute("F2X25_locked")) {
						locked = node_e.attribute("F2X25_locked").toInt() == 1 ? true : false;
					}

					nodeitem->getNode()->getEntity()->setLogicalName(logical_name);
					nodeitem->setOldInstanceID(reference_id);
					nodeitem->getNode()->setPos(graphPosX, graphPosY);
					//nodeitem->setHidden(!hidden);
					//nodeitem->setLocked(!locked);

					if (nodeitem->getNode()->getNodeType() == DfxQNodeType::STANDARD_NODE) {
						((DfxQStandardNode*)nodeitem->getNode())->setTitle(logical_name.c_str());
					}

					if (isItemView) {
						nodeitem->setNodeType(DfxLayerNodeType::VIEW_NODE);
						DfxScene::getInstance()->addItemNoAnnounce((DfxViewItem*)nodeitem->getNode()->getEntity());
					}
						
					std::vector < std::pair<std::string, std::string>> properties;
					findNodeProperties(node_e, properties);
					mapEntityProperties(nodeitem->getNode()->getEntity(), properties);

					findConnections(node_e, connectionInfo);
					
					loadChannels(node_e, 
						nodeitem->getNode()->getEntity(), 
						nodeitem->getNode()->getEntity()->getInstanceID());

					std::ifstream ifs;
					nodeitem->getNode()->getEntity()->load(ifs);

					DfxLayerManager::getInstance()->insertNode(nodeitem, layer);
					
				}
				else {
					nodeitem->setImportNode(true);
				}

				
			}
		}
		dom_node = dom_node.nextSibling();
	}
}

void DfxXMLSceneLoader::findNodeProperties(QDomElement &e, std::vector < std::pair<std::string, std::string>> &properties) {

	QDomNode dom_node = e.firstChild();
	while (!dom_node.isNull()) {
		QDomElement property_e = dom_node.toElement();

		if (!property_e.isNull()) {
			if (property_e.tagName().compare("property", Qt::CaseInsensitive) == 0) {
				
				auto a_str = property_e.attribute("name").toStdString();
				auto b_str = property_e.attribute("value").toStdString();

				std::pair<std::string, std::string> p(a_str, b_str);
				properties.push_back(p);
			}
		}

		dom_node = dom_node.nextSibling();
	}
}

void DfxXMLSceneLoader::loadChannels(QDomElement &e, DfxEntity *entity, size_t instanceID) {

	QDomNode dom_node = e.firstChild();
	while (!dom_node.isNull()) {
		QDomElement property_e = dom_node.toElement();

		if (!property_e.isNull()) {
			if (property_e.tagName().compare("channels", Qt::CaseInsensitive) == 0) {

				QDomNode dom_node02 = property_e.firstChild();
				while (!dom_node02.isNull()) {
					QDomElement property_e02 = dom_node02.toElement();
				
					if (!property_e02.isNull()) {
						if (property_e02.tagName().compare("envelope", Qt::CaseInsensitive) == 0) {

							size_t propid = property_e02.attribute("property").toULongLong();
							size_t typid = property_e02.attribute("type").toULongLong();

							DfxEnvelope *envelope = nullptr;
							if (!DfxEnvelopeMap::getInstance()->hasEnvelope(instanceID, propid)) {
								envelope = new DfxEnvelope(DfxEnvelope_Identifier, entity, propid);
								DfxEnvelopeMap::getInstance()->addEnvelope(instanceID, envelope);

								envelope->resizeToFit(DfxTime::getInstance()->getTotalFrames());
							}
							else {
								envelope = DfxEnvelopeMap::getInstance()->findEnvelope(instanceID, propid);
								envelope->resizeToFit(DfxTime::getInstance()->getTotalFrames());
							}

							QDomNode dom_node03 = property_e02.firstChild();
							while (!dom_node03.isNull()) {
								QDomElement property_e03 = dom_node03.toElement();

								if (!property_e03.isNull()) {
									if (property_e03.tagName().compare("key", Qt::CaseInsensitive) == 0) {

										size_t frame = property_e03.attribute("frame").toULongLong();

										if (typid == DfxEntityPropertyTypes::iFLOAT_TYPE) {
											float value = property_e03.attribute("value").toFloat();
											envelope->setValue(frame, value);
										}
										else if (typid == DfxEntityPropertyTypes::iFLOAT3_TYPE) {
											DfxEntityPropertyTypes::Float3 vec;
											
											float x = property_e03.attribute("x").toFloat();
											float y = property_e03.attribute("y").toFloat();
											float z = property_e03.attribute("z").toFloat();

											vec.x = x; vec.y = y; vec.z = z;

											envelope->setValue(frame, vec);
										}									
									}
								}

								dom_node03 = dom_node03.nextSibling();
							}

						}
					}

					dom_node02 = dom_node02.nextSibling();
				}
			}
		}

		dom_node = dom_node.nextSibling();
	}
}

void DfxXMLSceneLoader::findConnections(QDomElement &e, std::vector<DfxXMLSceneLoader::ConnectionInfo>  &connections) {
	
	QDomNode dom_node = e.firstChild();
	while (!dom_node.isNull()) {
		QDomElement connection_e = dom_node.toElement();

		if (!connection_e.isNull()) {
			if (connection_e.tagName().compare("connection", Qt::CaseInsensitive) == 0) {

				ConnectionInfo info;
				info.output = connection_e.attribute("output").toULongLong();
				info.input = connection_e.attribute("input").toULongLong();
				info.source = connection_e.attribute("source").toULongLong();
				info.target = connection_e.attribute("target").toULongLong();

				connections.push_back(info);
			}
		}

		dom_node = dom_node.nextSibling();
	}
}

void DfxXMLSceneLoader::linkConnections(std::vector<DfxXMLSceneLoader::ConnectionInfo> connectionList) {

	for (auto cinfo : connectionList) {

		DfxLayerItem *source = DfxLayerManager::getInstance()->findNodeByOldID(cinfo.source);
		DfxLayerItem *target = DfxLayerManager::getInstance()->findNodeByOldID(cinfo.target);

		if (source != nullptr && target != nullptr) {

			DfxLayerItem *parent = source->getParent();

			if (parent->getID() == target->getParent()->getID()) {

				DfxQNodeConnection *conn = new DfxQNodeConnection(0);
				parent->getScene()->addItem(conn);

				DfxQNodeSlot *slt01 = source->getNode()->findSlot(cinfo.output);
				DfxQNodeSlot *slt02 = target->getNode()->findSlot(cinfo.input);

				conn->setPort1(slt01);
				conn->setPos1(slt01->scenePos());
				conn->setPort2(slt02);
				conn->setPos2(slt02->scenePos());
				conn->updatePath();

				if (slt02->getSlotType() == DfxQNodeSlotType::GROUP_INPUT_SLOT) {
					conn->setConnectionType(DfxQNodeConnectionType::GROUP);
				}

				conn->bind();
			}
		}
	}
}

void DfxXMLSceneLoader::load(std::string path){

	QFile file(path.c_str());

	QDomDocument document;

	if (!file.open(QIODevice::ReadOnly)) {
		DfxGlobal::console::println("Load scene error: Failed to open " + path);
		return;
	}
	
	if (!document.setContent(&file)) {
		DfxGlobal::console::println("Load scene error: Failed to parse " + path + ", unknown format.");
		return;
	}

	file.close();

	QDomElement docElem = document.documentElement();

	QString name = docElem.tagName();

	if (name.compare("scene", Qt::CaseInsensitive) == 0) {
		loadScene(docElem);
		linkConnections(connectionInfo);
	}
	else {
		DfxGlobal::console::println("Load scene warning: No Scene data found.");
	}
}


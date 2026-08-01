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

#include "DfxSceneLoader.h"
#include "DfxSceneWriterTool.h"


DfxSceneLoader::DfxSceneLoader() {
	state_stack.push(NULL_STATE);
	fileValid = true;
}

DfxSceneLoader::ValueInfo::ValueInfo(std::string ot, std::string on, size_t tp, size_t sz) {

	objectTypename = ot;
	objectName = on;
	type = tp;
	size = sz;
}

void DfxSceneLoader::setFStream(std::ifstream *fs) {
	file = fs;
}

void DfxSceneLoader::newObject(std::string objectTypename, std::string objectName) {
	using namespace dfxscenewriter::tools;

	if (!fileValid) return;

	switch (state_stack.top())
	{
	case NULL_STATE: {
		if (objectTypename.compare("Scene") == 0) {
			state_stack.push(SCENE_STATE);		
		}
	}
		break;
	case SCENE_STATE: {
		if (objectTypename.compare("Layer") == 0) {

			DfxLayerItem *item = DfxLayerManager::getInstance()->addNewLayer();
			item->setName(qreplace(objectName.substr(1, std::string::npos), '/', ' '));

			item_stack.push(item);
			state_stack.push(LAYER_STATE);
		}
	}
		break;
	case NODE_STATE: {

		if (objectTypename.compare("Connection") == 0) {
			connection_info_stack.push(ConnectionInfo());
			state_stack.push(CONNECTION_STATE);
		}
	}
		break;
	case LAYER_STATE: {
		if (objectTypename.compare("Node") == 0) {

			DfxLayerItem *nodeItem = newNodeItem();

			item_stack.push(nodeItem);
			node_property_stack.push(std::vector<std::pair<std::string, std::string>>());
			state_stack.push(NODE_STATE);
		}
	}
		break;
	case CONNECTION_STATE:
		break;
	}
}

void DfxSceneLoader::newValue(std::string objectTypename, std::string objectName, size_t type, size_t size) {
	if (!fileValid) return;

	value_info_stack.push(ValueInfo(objectTypename, objectName, type, size));
}

void DfxSceneLoader::setValue(std::string val) {
	if (!fileValid) return;

	switch (state_stack.top())
	{
	case SCENE_STATE: {
		sceneProperties(value_info_stack.top(), val);
	}
		break;
	case NODE_STATE: {
		nodeProperties(value_info_stack.top(), val);
	}
		break;
	case LAYER_STATE: {
		layerProperties(value_info_stack.top(), val);
	}
		break;
	case CONNECTION_STATE:
		connectionProperties(value_info_stack.top(), val);
		break;
	}
}

void DfxSceneLoader::endValue() {
	if (!fileValid) return;

	value_info_stack.pop();
}

void DfxSceneLoader::endArray() {
	if (!fileValid) return;
}

void DfxSceneLoader::endObject() {
	if (!fileValid) return;

	switch (state_stack.top())
	{
	case SCENE_STATE: {
		setupImportNodes();
		linkConnections();
		state_stack.pop();
	}
		break;
	case NODE_STATE: {
		
		DfxLayerItem *ptr = item_stack.top();

		if (!item_stack.top()->isImportNode()) {
			
			mapEntityProperties(ptr->getNode()->getEntity(), node_property_stack.top());
			ptr->getNode()->getEntity()->load(*file);

			node_property_stack.pop();

			item_stack.pop();

			DfxLayerManager::getInstance()->insertNode(ptr, item_stack.top());
		}
		else {
			delete item_stack.top();
			item_stack.pop();
			import_info_stack.top().layer = item_stack.top();

			importNodeList.push_back(import_info_stack.top());
			import_info_stack.pop();
		}
		
		state_stack.pop();
	}
		break;
	case LAYER_STATE: {

		item_stack.pop();
		state_stack.pop(); 
	}
		break;
	case CONNECTION_STATE: {

		if (item_stack.top()->isImportNode()) {
			import_info_stack.top().connectionList.push_back(connection_info_stack.top());
		}
		else {
			connectionList.push_back(connection_info_stack.top());
		}
		
		connection_info_stack.pop();
		state_stack.pop();
	}
		break;
	}
}

void DfxSceneLoader::error(dfxsceneformat::STATE st, std::string tok) {
	if (!fileValid) return;

	DfxGlobal::console::println(std::string("Error: ") + tok);

	throw tok;
}

DfxLayerItem *DfxSceneLoader::newNodeItem() {

	DfxLayerItem *nodeitem = new DfxLayerItem;
	nodeitem->setItemType(DfxLayerItemType::NODE_ITEM);
	nodeitem->setNodeType(DfxLayerNodeType::GENERIC_NODE);

	return nodeitem;
}

void DfxSceneLoader::sceneProperties(ValueInfo info, std::string value) {
	if (!fileValid) return;

	if ( info.objectName.compare("format_name") == 0) {

		if ( value.compare("scenefx") != 0 ) {
			fileValid = false;
			DfxGlobal::console::println("Open Scene Error: format_name incorrect format, aborting load.");
		}
	}
	else if ( info.objectName.compare("format_version") == 0) {

		try {
			int ver = std::stoi(value);
			if (ver > 1) {
				DfxGlobal::console::println("Open Scene Error:  format version.");
				fileValid = false;
			}
		}
		catch (const std::invalid_argument &ia) {
			DfxGlobal::console::println("Open Scene Error: format_version invalid argument, aborting load.");
			fileValid = false;
		}
	}
}

void DfxSceneLoader::layerProperties(ValueInfo info, std::string value) {

	if (!fileValid) return;

	if (info.objectName.compare("linked") == 0) {

		try {
			int val = std::stoi(value);
			item_stack.top()->setLinked((val == 1) ? true : false);
		}
		catch (const std::invalid_argument &ia) {
			DfxGlobal::console::println("Open Scene Error: Layer \'linked\' invalid argument.");
			fileValid = false;
		}
	}
	else if (info.objectName.compare("visible") == 0) {

		try {
			int val = std::stoi(value);
			item_stack.top()->setHidden((val == 1) ? true : false);
		}
		catch (const std::invalid_argument &ia) {
			DfxGlobal::console::println("Open Scene Error: Layer \'visible\' invalid argument.");
			fileValid = false;
		}
	}
	else if (info.objectName.compare("locked") == 0) {

		try {
			int val = std::stoi(value);
			item_stack.top()->setLocked((val == 1) ? true : false);
		}
		catch (const std::invalid_argument &ia) {
			DfxGlobal::console::println("Open Scene Error: Layer \'locked\' invalid argument.");
			fileValid = false;
		}
	}
	
}

void DfxSceneLoader::nodeProperties(ValueInfo info, std::string value) {
	if (!fileValid) return;

	if (info.objectName.compare("F2X25_isImportNode") == 0) {

		int val = std::stoi(value);
		item_stack.top()->setImportNode(val == 1 ? true : false);

		if (val == 1) {
			ImportNodeInfo iinfo;
			import_info_stack.push(iinfo);
		}
	}
	else if (info.objectName.compare("F2X25_isItemView") == 0) {

		if (!item_stack.top()->isImportNode()) {
			int val = std::stoi(value);

			if (val == 1) {
				item_stack.top()->setNodeType(DfxLayerNodeType::VIEW_NODE);
			}
		}
	}
	else if (info.objectName.compare("F2X25_unique_name") == 0) {

		if (!item_stack.top()->isImportNode()) {

			if (value.compare("2F2X252X_DFX_GROUP_NODE") == 0){
				DfxEntity *ent = new DfxNodes::DfxGroupNode(DfxNodes::DfxGroupNode_Identifier);
				DfxQBaseNode *qnode = new DfxQGroupNode(ent);

				qnode->setLayerReference(item_stack.top());
				item_stack.top()->setNode(qnode);
			}
			else {
				DfxEntity *entity = DfxEntityDB::newEntityInstance(value);

				DfxQBaseNode *qnode = new DfxQStandardNode(entity);

				qnode->setLayerReference(item_stack.top());
				item_stack.top()->setNode(qnode);

				if (item_stack.top()->getNodeType() == DfxLayerNodeType::VIEW_NODE) {
					DfxScene::getInstance()->addItemNoAnnounce((DfxViewItem*)entity);
				}
			}
		}
	}
	else if (info.objectName.compare("F2X25_logical_name") == 0) {

		if (!item_stack.top()->isImportNode()) {

			item_stack.top()->getNode()->getEntity()->setLogicalName(value);

			if (item_stack.top()->getNode()->getNodeType() == DfxQNodeType::STANDARD_NODE) {
				((DfxQStandardNode*)item_stack.top()->getNode())->setTitle(value.c_str());
			}
		}
	}
	else if (info.objectName.compare("F2X25_reference_id") == 0) {
		
		if (!item_stack.top()->isImportNode()) {
			item_stack.top()->setOldInstanceID(std::stoull(value));
		}
		else {
			import_info_stack.top().referenceID = std::stoull(value);
		}
	}
	else if (info.objectName.compare("F2X25_graphPosX") == 0) {

		if (!item_stack.top()->isImportNode()) {

			item_stack.top()->getNode()->setPos(std::stoi(value), item_stack.top()->getNode()->pos().y());
		}
		else {

			import_info_stack.top().x = std::stoi(value);
		}
	}
	else if (info.objectName.compare("F2X25_graphPosY") == 0) {

		if (!item_stack.top()->isImportNode()) {

			item_stack.top()->getNode()->setPos(item_stack.top()->getNode()->pos().x(), std::stoi(value));
		}
		else {

			import_info_stack.top().y = std::stoi(value);
		}
	}
	else if (info.objectName.compare("F2X25_hidden") == 0) {

		int val = std::stoi(value);
	}
	else if (info.objectName.compare("F2X25_locked") == 0) {

		int val = std::stoi(value);
	}
	else { 

		if (!item_stack.top()->isImportNode()) {
			node_property_stack.top().push_back(std::pair<std::string, std::string>(info.objectName, value));
		}
	}
}

void DfxSceneLoader::connectionProperties(ValueInfo info, std::string value) {
	if (!fileValid) return;

	if (info.objectName.compare("output") == 0) {

		try {
			size_t val = std::stoull(value);
			connection_info_stack.top().output = val;
		}
		catch (const std::invalid_argument &ia) {
			DfxGlobal::console::println("Open Scene Error: Connection \'output\' invalid argument.");
			fileValid = false;
		}
	}
	else if (info.objectName.compare("input") == 0) {

		try {
			size_t val = std::stoull(value);
			connection_info_stack.top().input = val;
		}
		catch (const std::invalid_argument &ia) {
			DfxGlobal::console::println("Open Scene Error: Connection \'input\' invalid argument.");
			fileValid = false;
		}
	}
	else if (info.objectName.compare("source") == 0) {

		try {
			size_t val = std::stoull(value);
			connection_info_stack.top().source = val;
		}
		catch (const std::invalid_argument &ia) {
			DfxGlobal::console::println("Open Scene Error: Connection \'source\' invalid argument.");
			fileValid = false;
		}
	}
	else if (info.objectName.compare("target") == 0) {

		try {
			size_t val = std::stoull(value);
			connection_info_stack.top().target = val;
		}
		catch (const std::invalid_argument &ia) {
			DfxGlobal::console::println("Open Scene Error: Connection \'target\' invalid argument.");
			fileValid = false;
		}
	}
}

void DfxSceneLoader::linkConnections() {

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

void DfxSceneLoader::setupImportNodes() {

	for (auto imp : importNodeList) {

		DfxLayerItem *parent = DfxLayerManager::getInstance()->findNodeByOldID(imp.referenceID);
		DfxLayerItem *source  = DfxLayerManager::getInstance()->addImportNode(parent, imp.layer);
		source->getNode()->setPos(imp.x, imp.y);

		for (auto cinfo : imp.connectionList) {

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
					
					if (slt02->getSlotType() == DfxQNodeSlotType::GROUP_INPUT_SLOT)conn->setConnectionType(DfxQNodeConnectionType::GROUP);

					conn->bind();
				}
			}
		}
	}
}
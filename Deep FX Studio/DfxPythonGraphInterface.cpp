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

#include <DfxScene.h>
#include <DfxSceneGraph.h>
#include <QString>
#include "DfxPythonGraphInterface.h"

// Creation / Destruction Functions
size_t createNode(const char* uniqueid) {

	if (QString(uniqueid).compare("2F2X252X_DFX_GROUP_NODE", Qt::CaseInsensitive) == 0) {
		auto grp = DfxLayerManager::getInstance()->addGroupNode(nullptr);
		return grp->getNode()->getEntity()->getInstanceID();
	}
	else {

		DfxGlobalEntityName nm(uniqueid);
		DfxEntity *entity = DfxEntityDB::newEntityInstance(nm);

		DfxLayerManager::getInstance()->addNode(entity);

		return entity->getInstanceID();
	}
}

size_t createViewNode(const char* uniqueid) {

	if (QString(uniqueid).compare("2F2X252X_DFX_GROUP_NODE", Qt::CaseInsensitive) == 0) {
		auto grp = DfxLayerManager::getInstance()->addGroupNode(nullptr);
		return grp->getNode()->getEntity()->getInstanceID();
	}
	else {
		DfxGlobalEntityName nm(uniqueid);
		DfxEntity *entity = DfxEntityDB::newEntityInstance(nm);

		DfxLayerManager::getInstance()->addViewNode((DfxViewItem*)entity);

		return entity->getInstanceID();
	}
}

// Connect To Group
void connectToGroup(size_t source_id, size_t source_property_id, size_t group_id) {

	DfxLayerItem *source = DfxLayerManager::getInstance()->findNodeByID(source_id);
	DfxLayerItem *target = DfxLayerManager::getInstance()->findNodeByID(group_id);

	if (source != nullptr && target != nullptr) {
		
		DfxLayerItem *parent = source->getParent();

		if (target->getNode()->getNodeType() == DfxQNodeType::GROUP_NODE) {

			DfxQNodeSlot *port1 = source->getNode()->findSlot(source_property_id);
			DfxQNodeSlot *port2 = nullptr;

			for (auto slt : target->getNode()->getAllSlots()) {
				if (slt->getType() == DfxQNodeSlotType::GROUP_INPUT_SLOT) port2 = slt;
			}

			if (port2 != nullptr && port1 != nullptr) {

				if (port2->getSlotType() == DfxQNodeSlotType::GROUP_INPUT_SLOT &&
					port1->getPropertyType() == DfxEntityPropertyTypes::TRAIT_UID_TYPE) {

					if (port1->getParentNode() != port2->getParentNode() &&
						port1->getParentNode()->ID() != port2->getParentNode()->ID() &&
						port1->isOutput() != port2->isOutput()) {

						DfxQNodeConnection *conn = new DfxQNodeConnection(0);
						parent->getScene()->addItem(conn);

						conn->setPort1(port1);
						conn->setPos1(port1->scenePos());
						conn->setPos2(port2->scenePos());
						conn->setPort2(port2);
						conn->updatePath();
						conn->setConnectionType(DfxQNodeConnectionType::GROUP);
						conn->bind();
						conn = nullptr;

						DfxGlobal::command::exec("updateViewport");

					}
					else {
						DfxGlobal::console::println("Failed To Connect to Group. Slots Conflict.");
					}
				}
				else {
					DfxGlobal::console::println("Failed To Connect to Group. No Trait ID");
				}
			}
		}
		else {
			DfxGlobal::console::println("Failed To Connect Properties. Node not group node");
		}
	}
}

// Manipulation Functions
void connectProperties(size_t source_id, size_t source_property_id, size_t destination_id, size_t destination_property_id) {

	DfxLayerItem *source = DfxLayerManager::getInstance()->findNodeByID(source_id);
	DfxLayerItem *target = DfxLayerManager::getInstance()->findNodeByID(destination_id);

	if (source != nullptr && target != nullptr) {
		
		DfxLayerItem *parent = source->getParent();

		if (parent->getID() == target->getParent()->getID()) {
			
			DfxQNodeSlot *slt01 = source->getNode()->findSlot(source_property_id);
			DfxQNodeSlot *slt02 = target->getNode()->findSlot(destination_property_id);

			if (slt01->getParentNode() != slt02->getParentNode() &&
				slt01->getParentNode()->ID() != slt02->getParentNode()->ID() &&
				slt01->getPropertyType() == slt02->getPropertyType() &&
				slt01->isOutput() != slt02->isOutput() &&
				!slt01->isConnected(slt02)) {

				DfxQNodeConnection *conn = new DfxQNodeConnection(0);
				parent->getScene()->addItem(conn);

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
			else {
				DfxGlobal::console::println("Failed To Connect properties. Slots Conflict.");
			}
		}
	}
	else {
		DfxGlobal::console::println("Failed To Connect Properties.");
	}
}

void disconnectProperties(size_t source_id, size_t source_property_id, size_t destination_id, size_t destination_property_id) {
}

// Node Property values Get/Set
void  setString(size_t uid, const char* name, const char *val) {
	using namespace DfxEntityPropertyTypes;

	if (hasProperty(uid, name)) {

		auto propid = getPropertyID(uid, name);
		auto ent = DfxEntityDB::getDataInstance(uid);

		if (ent == nullptr) {
			DfxGlobal::console::println("Failed To Set String. Invalid Node ID.");
			return;
		}

		if (ent->getPropertyInstance(propid)->getType() == DfxEntityPropertyTypes::iSTRING_TYPE ||
			ent->getPropertyInstance(propid)->getType() == DfxEntityPropertyTypes::iFILE_PATH_TYPE ||
			ent->getPropertyInstance(propid)->getType() == DfxEntityPropertyTypes::iDIR_PATH_TYPE) {

			*(std::string*)ent->getProperty(propid) = val;
		}
		else {
			DfxGlobal::console::println("Failed To Set String. Property is not a string value.");
		}
	}
	else {
		DfxGlobal::console::println("Failed To Set String. Property not found.");
	}
	
}

const char *getString(size_t uid, const char* name) {
	using namespace DfxEntityPropertyTypes;

	if (hasProperty(uid, name)) {

		auto propid = getPropertyID(uid, name);
		auto ent = DfxEntityDB::getDataInstance(uid);

		if (ent == nullptr) {
			DfxGlobal::console::println("Failed To Get String. Invalid Node ID.");
			return QString("").toUtf8().constData();
		}

		if (ent->getPropertyInstance(propid)->getType() == DfxEntityPropertyTypes::iSTRING_TYPE ||
			ent->getPropertyInstance(propid)->getType() == DfxEntityPropertyTypes::iFILE_PATH_TYPE ||
			ent->getPropertyInstance(propid)->getType() == DfxEntityPropertyTypes::iDIR_PATH_TYPE) {

			QString rtValue(((std::string*)ent->getProperty(propid))->c_str());
			return rtValue.toUtf8().constData();
		}
		else {
			DfxGlobal::console::println("Failed To Get String. Property is not a string value.");
			return QString("").toUtf8().constData();
		}
	}
	else {
		DfxGlobal::console::println("Failed To Get String. Property not found.");
		return QString("").toUtf8().constData();
	}
}

void  setFloat(size_t uid, const char* name, float val) {
	using namespace DfxEntityPropertyTypes;

	if (hasProperty(uid, name)) {

		auto propid = getPropertyID(uid, name);
		auto ent = DfxEntityDB::getDataInstance(uid);

		if (ent == nullptr) {
			DfxGlobal::console::println("Failed To Set Float. Invalid Node ID.");
			return;
		}

		if (ent->getPropertyInstance(propid)->getType() == DfxEntityPropertyTypes::iFLOAT_TYPE ) {

			*(float*)ent->getProperty(propid) = val;
		}
		else {
			DfxGlobal::console::println("Failed To Set Float. Property is not a float value.");
		}
	}
	else {
		DfxGlobal::console::println("Failed To Set Float. Property not found.");
	}
}

float getFloat(size_t uid, const char* name) {
	using namespace DfxEntityPropertyTypes;

	if (hasProperty(uid, name)) {

		auto propid = getPropertyID(uid, name);
		auto ent = DfxEntityDB::getDataInstance(uid);

		if (ent == nullptr) {
			DfxGlobal::console::println("Failed To Get Float. Invalid Node ID.");
			return 0.f;
		}
		if (ent->getPropertyInstance(propid)->getType() == DfxEntityPropertyTypes::iFLOAT_TYPE ) {
			return *(float*)ent->getProperty(propid);
		}
		else {
			DfxGlobal::console::println("Failed To Get Float. Property is not a float value.");
			return 0.f;
		}
	}
	else {
		DfxGlobal::console::println("Failed To Get Float. Property not found.");
		return 0.f;
	}
}

void  setFloat3(size_t uid, const char* name, float x, float y, float z) {
	using namespace DfxEntityPropertyTypes;

	if (hasProperty(uid, name)) {

		auto propid = getPropertyID(uid, name);
		auto ent = DfxEntityDB::getDataInstance(uid);

		if (ent == nullptr) {
			DfxGlobal::console::println("Failed To Set Float3. Invalid Node ID.");
			return;
		}

		if (ent->getPropertyInstance(propid)->getType() == DfxEntityPropertyTypes::iFLOAT3_TYPE) {

			((Float3*)ent->getProperty(propid))->x = x;
			((Float3*)ent->getProperty(propid))->y = y;
			((Float3*)ent->getProperty(propid))->z = z;
		}
		else {
			DfxGlobal::console::println("Failed To Set Float3. Property is not a float3 value.");
		}
	}
	else {
		DfxGlobal::console::println("Failed To Set Float3. Property not found.");
	}
}

std::vector<float> getFloat3(size_t uid, const char* name) {
	using namespace DfxEntityPropertyTypes;

	std::vector<float> vec;
	vec.push_back(.0f);
	vec.push_back(.0f);
	vec.push_back(.0f);

	if (hasProperty(uid, name)) {

		auto propid = getPropertyID(uid, name);
		auto ent = DfxEntityDB::getDataInstance(uid);

		if (ent == nullptr) {
			DfxGlobal::console::println("Failed To Get Float3. Invalid Node ID.");
			return vec;
		}

		if (ent->getPropertyInstance(propid)->getType() == DfxEntityPropertyTypes::iFLOAT3_TYPE) {
			vec[0] = ((Float3*)ent->getProperty(propid))->x;
			vec[1] = ((Float3*)ent->getProperty(propid))->y;
			vec[2] = ((Float3*)ent->getProperty(propid))->z;

			return vec;
		}
		else {
			DfxGlobal::console::println("Failed To Get Float3. Property is not a float3 value.");
			return vec;
		}
	}
	else {
		DfxGlobal::console::println("Failed To Get Float3. Property not found.");
		return vec;
	}
}

void  setEnumIndex(size_t uid, const char* name, int index) {
	using namespace DfxEntityPropertyTypes;

	if (hasProperty(uid, name)) {

		auto propid = getPropertyID(uid, name);
		auto ent = DfxEntityDB::getDataInstance(uid);

		if (ent == nullptr) {
			DfxGlobal::console::println("Failed To Set Enum. Invalid Node ID.");
			return;
		}

		if (ent->getPropertyInstance(propid)->getType() == DfxEntityPropertyTypes::iENUM_TYPE ) {

			int size = ((DfxEnum*)ent->getProperty(propid))->size();

			if (size > 0) {
				if (index >= 0 && index < size) {
					((DfxEnum*)ent->getProperty(propid))->setCurrentIndex(index);
				}
			}
		}
		else {
			DfxGlobal::console::println("Failed To Set Enum. Property is not a enum value.");
		}
	}
	else {
		DfxGlobal::console::println("Failed To Set Enum. Property not found.");
	}
}

int getEnumIndex(size_t uid, const char* name) {
	using namespace DfxEntityPropertyTypes;

	if (hasProperty(uid, name)) {

		auto propid = getPropertyID(uid, name);
		auto ent = DfxEntityDB::getDataInstance(uid);

		if (ent == nullptr) {
			DfxGlobal::console::println("Failed To Get Enum. Invalid Node ID.");
			return 0;
		}

		if (ent->getPropertyInstance(propid)->getType() == DfxEntityPropertyTypes::iENUM_TYPE) {

			return ((DfxEnum*)ent->getProperty(propid))->getCurrentIndex();							
		}
		else {
			DfxGlobal::console::println("Failed To Get Enum. Property is not a enum value.");
			return 0;
		}
	}
	else {
		DfxGlobal::console::println("Failed To Get Enum. Property not found.");
		return 0;
	}
}

// Graph UI Functions
std::vector<size_t> getNodesSelectedInGraph() {
	std::vector<size_t> nodes;

	for (auto ent : DfxSceneGraph::getInstance()->getGraphUI()->getView()->getNodesSelectedInGraph()) {
		nodes.push_back(ent->getInstanceID());
	}

	return nodes;
}

void uniformLayoutSelected() {
	DfxSceneGraph::getInstance()->getGraphUI()->getView()->uniformLayout(false);
}

void uniformLayout() {
	DfxSceneGraph::getInstance()->getGraphUI()->getView()->uniformLayout(true);
}

void viewAllNodes() {
	DfxSceneGraph::getInstance()->getGraphUI()->getView()->fitAllNodes();
}

// Traversal Functions

// Query Functions
const char *getNodeUniqueName(size_t uid) {

	auto dinstance = DfxEntityDB::getDataInstance(uid);

	if (dinstance != nullptr) {
		QString rtvalue(dinstance->getDfxEntity()->UniqueName().c_str());
		return rtvalue.toUtf8().constData();
	}

	return QString("N/A").toUtf8().constData();
}

size_t getPropertyID(size_t uid, const char* name) {

	QString str00(name);
	auto dinstance = DfxEntityDB::getDataInstance(uid);

	if (dinstance != nullptr) {
		auto plist = dinstance->getPropertyList();
		
		for (auto d : plist) {
			QString str01(dinstance->getPropertyName(d).c_str());

			if (str00.compare(str01, Qt::CaseInsensitive) == 0) {
				return d;
			}
		}
	}

	return 0;
}

void printNodeProperties(size_t intanceid) {
	auto ent = DfxEntityDB::getDataInstance(intanceid)->getPropertyList();

	std::stringstream ss;
	for (auto d: ent) {
		ss << "Name: " << DfxEntityDB::getDataInstance(intanceid)->getPropertyName(d) << " ID: " << d << std::endl;
	}

	DfxGlobal::console::println(ss.str());
}

bool hasProperty(size_t uid, const char* name) {

	QString str00(name);
	bool hasProp = false;

	auto ent = DfxEntityDB::getDataInstance(uid)->getPropertyList();

	for (auto d : ent) {

		QString str01(DfxEntityDB::getDataInstance(uid)->getPropertyName(d).c_str());
	
		if (str00.compare(str01, Qt::CaseInsensitive) == 0) {
			hasProp = true;
			break;
		}
	}

	return hasProp;
}

bool hasPropertyID(size_t uid, const char* name, size_t propid) {

	QString str00(name);
	bool hasProp = false;

	auto ent = DfxEntityDB::getDataInstance(uid)->getPropertyList();

	for (auto d : ent) {

		QString str01(DfxEntityDB::getDataInstance(uid)->getPropertyName(d).c_str());

		if (str00.compare(str01, Qt::CaseInsensitive) == 0 && propid == d) {
			hasProp = true;
			break;
		}
	}

	return hasProp;
}

std::vector<size_t> findAllNodes() {

	std::vector<size_t> nodes;

	DfxLayerItem *rootLayer = DfxLayerManager::getInstance()->getRoot();

	std::stack<DfxLayerItem*> stack;

	stack.push(rootLayer);

	while (stack.size() > 0) {
		auto top = stack.top();
		stack.pop();

		if (top->getItemType() == DfxLayerItemType::NODE_ITEM) {
			DfxEntity *ent = top->getNode()->getEntity();
			nodes.push_back(ent->getInstanceID());
		}

		for (size_t inx = 0; inx < top->childCount(); inx++) {
			stack.push(top->getChild(inx));
		}
	}

	return nodes;
}

std::vector<size_t> findAllNodes(const char* uniqueid) {

	std::vector<size_t> nodes;
	QString str00(uniqueid);

	DfxLayerItem *rootLayer = DfxLayerManager::getInstance()->getRoot();

	std::stack<DfxLayerItem*> stack;

	stack.push(rootLayer);

	while (stack.size() > 0) {
		auto top = stack.top();
		stack.pop();

		if (top->getItemType() == DfxLayerItemType::NODE_ITEM) {
			DfxEntity *ent = top->getNode()->getEntity();
			QString str01(ent->UniqueName().c_str());

			if (str00.compare(str01, Qt::CaseInsensitive) == 0) {
				nodes.push_back(ent->getInstanceID());
			}
		}

		for (size_t inx = 0; inx < top->childCount(); inx++) {
			stack.push(top->getChild(inx));
		}
	}

	return nodes;
}

const char *propertyStateType(size_t uid, size_t d) {

	if (DfxEntityDB::getDataInstance(uid)->getPropertyInstance(d)->getState() ==
		DfxEntityPropertyState::DATA_STORE_STATE) {
		return QString("Property State Type: Data Store").toUtf8().constData();
	}
	else if (DfxEntityDB::getDataInstance(uid)->getPropertyInstance(d)->getState() ==
		DfxEntityPropertyState::INPUT_STATE) {
		return QString("Property State Type: Input").toUtf8().constData();
	}
	else if (DfxEntityDB::getDataInstance(uid)->getPropertyInstance(d)->getState() ==
		DfxEntityPropertyState::NORMAL_STATE) {
		return QString("Property State Type: No State").toUtf8().constData();
	}
	else if (DfxEntityDB::getDataInstance(uid)->getPropertyInstance(d)->getState() ==
		DfxEntityPropertyState::OUTPUT_STATE) {
		return QString("Property State Type: Output").toUtf8().constData();
	}
	
	return QString("N/A").toUtf8().constData();
}

bool propertyIsInput(size_t uid, size_t propid) {

	if (DfxEntityDB::getDataInstance(uid)->getPropertyInstance(propid)->getState() ==
		DfxEntityPropertyState::INPUT_STATE) {

		return true;
	}

	return false;
}

bool propertyIsOutput(size_t uid, size_t propid) {

	if (DfxEntityDB::getDataInstance(uid)->getPropertyInstance(propid)->getState() ==
		DfxEntityPropertyState::OUTPUT_STATE) {

		return true;
	}

	return false;
}

bool propertyIsDatastore(size_t uid, size_t propid) {

	if (DfxEntityDB::getDataInstance(uid)->getPropertyInstance(propid)->getState() ==
		DfxEntityPropertyState::DATA_STORE_STATE) {

		return true;
	}

	return false;
}

bool propertyHasParent(size_t uid , size_t propid) {

	auto dinstance = DfxEntityDB::getDataInstance(uid);

	if (dinstance != nullptr) {

		auto pinstance = dinstance->getPropertyInstance(propid);

		if (pinstance != nullptr) {
			if (DfxEntityPropertyState::INPUT_STATE == pinstance->getState()) {

				if (dinstance->getPropertyInstance(propid)->hasBinding()) {

					return true;
				}
			}
		}
	}

	return 0;
}

bool propertyHasChildren(size_t uid, size_t propid) {

	for (auto child : DfxEntityDB::getDataInstance(uid)->getDfxEntity()->getAllChildren()) {

		auto ent = child->getDataInstance()->getPropertyList();

		for (auto d : ent) {

			if (DfxEntityPropertyState::INPUT_STATE ==
				child->getDataInstance()->getPropertyInstance(d)->getState()) {

				if (child->getDataInstance()->getPropertyInstance(d)->hasBinding()) {
					if (child->getDataInstance()->getPropertyInstance(d)->getParent()->getDfxEntity()->getInstanceID() ==
						DfxEntityDB::getDataInstance(uid)->getDfxEntity()->getInstanceID() &&
						child->getDataInstance()->getPropertyInstance(d)->getParent()->getID() == propid) {

						return true;
					}
				}
			}
		}
	}

	return false;
}

size_t getPropertyParent(size_t uid, size_t propid) {

	auto dinstance = DfxEntityDB::getDataInstance(uid);

	if (dinstance != nullptr) {

		auto pinstance = dinstance->getPropertyInstance(propid);

		if (pinstance != nullptr) {
			if (DfxEntityPropertyState::INPUT_STATE == pinstance->getState()) {

				if (dinstance->getPropertyInstance(propid)->hasBinding()) {

					return dinstance->getPropertyInstance(propid)->getParent()->getDfxEntity()->getInstanceID();
				}
			}
		}
	}

	return 0;
}

std::vector<size_t> getPropertyChildren(size_t uid, size_t propid) {

	std::vector<size_t> children;

	for (auto child : DfxEntityDB::getDataInstance(uid)->getDfxEntity()->getAllChildren()) {

		auto ent = child->getDataInstance()->getPropertyList();

		for (auto d : ent) {

			if (DfxEntityPropertyState::INPUT_STATE == 
				child->getDataInstance()->getPropertyInstance(d)->getState()) {

				if (child->getDataInstance()->getPropertyInstance(d)->hasBinding()) {
					if (child->getDataInstance()->getPropertyInstance(d)->getParent()->getDfxEntity()->getInstanceID() ==
						DfxEntityDB::getDataInstance(uid)->getDfxEntity()->getInstanceID() &&
						child->getDataInstance()->getPropertyInstance(d)->getParent()->getID() == propid) {

						children.push_back(child->getInstanceID());
					}
				}
			}
		}	
	}

	return children;
}


bool viewHasSelection() {

	auto items = DfxScene::getInstance()->getSelectedItems();

	if (items.size() > 0) return true;
	else return false;
}

size_t getViewSelection() {

	auto items = DfxScene::getInstance()->getSelectedItems();

	if (items.size() > 0) {
		return items[0]->getInstanceID();
	}
	else {
		DfxGlobal::console::println("Nothing Selected.");
		return 0;
	}
}

bool graphHasSelection() {
	auto nodes = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getNodesSelectedInGraph();

	if (nodes.size() > 0) return true;
	else return false;
}

std::vector<size_t> getGraphSelection() {
	std::vector<size_t> ids;
	auto nodes = DfxSceneGraph::getInstance()->getGraphUI()->getView()->getNodesSelectedInGraph();

	for (auto node: nodes) {
		ids.push_back(node->getInstanceID());
	}

	return ids;
}

bool hasMeshData(size_t uid) {

	auto inst = DfxEntityDB::getDataInstance(uid);

	if (inst != nullptr) {

		DfxEntity  *ent = inst->getDfxEntity();

		if (ent->hasDataStoreProperty("@mesh_data")) {
			return true;
		}
	}

	return false;
}

bool hasProxyMeshData(size_t uid) {

	auto inst = DfxEntityDB::getDataInstance(uid);

	if (inst != nullptr) {

		DfxEntity  *ent = inst->getDfxEntity();

		if (ent->hasDataStoreProperty("@proxy_mesh_data")) {
			return true;
		}
	}

	return false;

}

bool hasChildren(size_t uid) {

	auto inst = DfxEntityDB::getDataInstance(uid);

	if (inst != nullptr) {

		DfxEntity  *ent = inst->getDfxEntity();

		if (ent->getAllChildren().size() > 0) {
			return true;
		}
	}

	return false;
}

std::vector<size_t> getAllChildren(size_t uid) {
	std::vector<size_t> ids;

	auto inst = DfxEntityDB::getDataInstance(uid);

	if (inst != nullptr) {

		DfxEntity  *ent = inst->getDfxEntity();

		for (auto chd: ent->getAllChildren()) {
			ids.push_back(chd->getInstanceID());
		}
	}

	return ids;
}
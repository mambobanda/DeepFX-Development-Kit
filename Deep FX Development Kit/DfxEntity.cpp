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

#include "DfxEntityDB.h"
#include "DfxEntity.h"
#include "DfxEntityUtil.h"

size_t DfxEntity::ENTITY_INSTANCE_ID_GEN = 0x1300;


DfxEntity::DfxEntity() {
	logicalName = "Entity";
	hidden = false;
	delegator = nullptr;
}

DfxEntity::DfxEntity(DfxGlobalEntityName uniquename) {
	hidden = false;
	delegator = nullptr;

	ORIGINAL_NAME = uniquename;
	this->globalID = hash(uniquename);
	logicalName = "Entity";

	instanceID = ++ENTITY_INSTANCE_ID_GEN;

	_createDataInstance();
	_init();

	getDataInstance()->makeDirty();
}

DfxEntity::~DfxEntity() {
	
}

const DfxGlobalEntityName DfxEntity::UniqueName() {
	return ORIGINAL_NAME;
}

void DfxEntity::onDataChanged(DfxEntityDataInstance *instance) {

}

std::string DfxEntity::getLogicalName() {
	return logicalName;
}

void DfxEntity::setLogicalName(std::string str) {
	logicalName = str;
}

size_t DfxEntity::getInstanceID() {
	return instanceID;
}

size_t DfxEntity::getGlobalID() {
	return globalID;
}

void DfxEntity::addChild(DfxEntity *ent) {
	children.push_back(ent);
}

void DfxEntity::removeChild(DfxEntity *ent) {
	removeChild(ent->getInstanceID());
}

void DfxEntity::removeChild(size_t instance_id) {
	std::list<DfxEntity*>::iterator it = children.begin();

	while (it != children.end()) {

		if ((*it)->getInstanceID() == instance_id) {
			children.erase(it);
			break;
		}
		else {
			it++;
		}
	}

}

DfxEntity* DfxEntity::queryChild(size_t instance_id) {
	std::list<DfxEntity*>::iterator it = children.begin();

	while (it != children.end()) {

		if ((*it)->getInstanceID() == instance_id) {
			return (*it);
		}
		else {
			it++;
		}
	}

	return nullptr;
}

DfxEntityChildList DfxEntity::getAllChildren() {
	return children;
}

bool  DfxEntity::parentsAreClean() {

	bool areClean = true;

	DfxEntityDataInstance *dinstance = getDataInstance();

	if (dinstance == nullptr){
		return false;
	}

	std::vector<size_t> lst = dinstance->getPropertyList();

	for (auto pid : lst) {
		if (!dinstance->getPropertyInstance(pid)->isBindingClean()) {
			areClean = false;
			break;
		}
	}

	return areClean;
}

bool DfxEntity::branchIsClean() {
	bool areClean = true;

	DfxEntityDataInstance *dinstance = getDataInstance();

	if (dinstance == nullptr) {
		return false;
	}

	std::vector<size_t> lst = dinstance->getPropertyList();

	for (auto pid : lst) {
		if (!dinstance->getPropertyInstance(pid)->isBindingClean() || 
			!getDataInstance()->isClean()) {
			areClean = false;
			break;
		}
	}

	return areClean;
}

void DfxEntity::bfsQuery(DfxEntity *root) {

	std::queue<DfxEntity*> queue;
	queue.push(root);

	while (!queue.empty())
	{
		DfxEntity *tmp_ent = queue.front();
		queue.pop();

		if (tmp_ent->parentsAreClean()) {

			if (!tmp_ent->getDataInstance()->isClean()) {
				tmp_ent->compute();
			}

			if (tmp_ent->hasChildren()) {
				for (auto ent : tmp_ent->getAllChildren()) {
					if (!ent->isHidden()) {

						if (ent->isDelegate()) {
							if (ent->getDelegator()->delegateesAreClean()) {
								queue.push(ent->getDelegator());
							}
						}
						else {
							queue.push(ent);
						}
					}
				}
			}

			if (tmp_ent->getDelegateTips().size() > 0) {
				for (auto delg : tmp_ent->getDelegateTips()) queue.push(delg);
			}
		}
		/*else {
			std::cout << tmp_ent->getLogicalName() << " " << "Not Ready" << std::endl;
		}*/
	}
}

void DfxEntity::initialize() {
	
}

void DfxEntity::save(std::ofstream &file) {

}

void DfxEntity::load(std::ifstream &file) {

}

void DfxEntity::_createDataInstance() {
	DfxEntityDB::newDfxEntityDataInstance(globalID, instanceID, this);
}

void DfxEntity::_init() {

}

bool DfxEntity::hasChildren() {
	return (children.size() <= 0) ? false : true;
}

bool DfxEntity::hasChild(DfxEntity *ent) {

	for (auto child : children) {
		if (child->getInstanceID() == ent->getInstanceID()) {
			return true;
		}
	}

	return false;
}

void DfxEntity::reset() {

}

bool DfxEntity::hasParent() {
	auto idList = getDataInstance()->getPropertyList();

	for (auto id : idList) {

		if (getDataInstance()->getPropertyInstance(id)->hasBinding()) {
			return true;
		}
	}

	return false;
}

void DfxEntity::setFreeCallback(FreeDataCallback func) {
	freeCallback = func;
}

DfxEntityDataInstance *DfxEntity::getDataInstance() {
	return DfxEntityDB::getDataInstance(instanceID);
}

void *DfxEntity::getProperty(size_t pid) {
	return DfxEntityDB::getDataInstance(instanceID)->getProperty(pid);
}

std::string DfxEntity::getPropertyName(size_t pid) {
	return DfxEntityDB::getDataInstance(instanceID)->getPropertyName(pid);
}

size_t DfxEntity::getPropertyType(size_t pid) {
	return DfxEntityDB::getDataInstance(instanceID)->getPropertyType(pid);
}

void DfxEntity::setPropertyState(size_t pid, DfxEntityPropertyState st) {
	DfxEntityDB::getDataInstance(instanceID)->getPropertyInstance(pid)->setState(st);
}

void DfxEntity::importData(std::vector<std::pair<std::string, std::string>> data) {
	// Do nothing
}

bool DfxEntity::isExportable() {
	return false;
}

std::vector<std::pair<std::string, std::string>> DfxEntity::exportData() {
	return std::vector<std::pair<std::string, std::string>>();
}

bool DfxEntity::propertyHasBinding(size_t pid) {
	return DfxEntityDB::getDataInstance(instanceID)->getPropertyInstance(pid)->hasBinding();
}

void DfxEntity::bind(DfxEntity *outputDfxEntity, size_t outputPropertyID, DfxEntity *intputDfxEntity, size_t inputPropertyID ) {
	DfxEntityDataInstance *output_dinst = DfxEntityDB::getDataInstance(outputDfxEntity->getInstanceID());
	DfxEntityDataInstance *input_dinst  = DfxEntityDB::getDataInstance(intputDfxEntity->getInstanceID());
	

	DfxEntityProperty *output_prop = output_dinst->getPropertyInstance(outputPropertyID);
	DfxEntityProperty *input_prop = input_dinst->getPropertyInstance(inputPropertyID);
	
	input_prop->bind(output_prop);
	input_prop->dirtyParent();
}

void DfxEntity::unbind(DfxEntity *entity, size_t propertyID) {
	DfxEntityDataInstance *dinst = DfxEntityDB::getDataInstance(entity->getInstanceID());
	DfxEntityProperty *prop = dinst->getPropertyInstance(propertyID);
	prop->unbind();
	prop->dirtyParent();
}

void DfxEntity::makeDirty() {
	getDataInstance()->makeDirty();
}

void DfxEntity::makeChildrenDirty() {
	for(auto child: children) {
		child->makeDirty();
	}
}

void DfxEntity::pingParents(bool pingSelf) {

	for (auto id : getDataInstance()->getPropertyList()) {

		if (DfxEntityPropertyState::INPUT_STATE == getDataInstance()->getPropertyInstance(id)->getState()) {

			if (getDataInstance()->getPropertyInstance(id)->hasBinding()) {

				getDataInstance()->getPropertyInstance(id)->getDfxEntity()->pingParents();
			}
		}
	}

	if (pingSelf) {
		if (!getDataInstance()->isClean()) {
			compute();
		}
	}
}

void DfxEntity::makePropertyBindingDirty(size_t inid) {

	for (auto ent : children) {

		auto plist = ent->getDataInstance()->getPropertyList();

		for (auto id : plist) {

			if (ent->getDataInstance()->getPropertyInstance(id)->hasBinding()) {

				if (ent->getDataInstance()->getPropertyInstance(id)->getParent()->getDfxEntity()->getInstanceID() == instanceID) {

					if (ent->getDataInstance()->getPropertyInstance(id)->getParent()->getID() == inid) {
						ent->getDataInstance()->getPropertyInstance(id)->dirtyParent();
					}
				}
			}
		}
	}
}

void DfxEntity::unbindOutput(size_t inid) {

	for (auto ent : children) {

		auto plist = ent->getDataInstance()->getPropertyList();

		for (auto id : plist) {

			if (ent->getDataInstance()->getPropertyInstance(id)->hasBinding()) {

				if (ent->getDataInstance()->getPropertyInstance(id)->getParent()->getDfxEntity()->getInstanceID() == instanceID) {

					if (ent->getDataInstance()->getPropertyInstance(id)->getParent()->getID() == inid) {
						ent->getDataInstance()->getPropertyInstance(id)->unbind();
					}
				}
			}
		}
	}
}

void DfxEntity::unbindEverything() {

	auto plist = getDataInstance()->getPropertyList();

	for (auto id : plist) {

		if (DfxEntityPropertyState::INPUT_STATE == getDataInstance()->getPropertyInstance(id)->getState()) {

			if(getDataInstance()->getPropertyInstance(id)->hasBinding())
				getDataInstance()->getPropertyInstance(id)->unbind();
		}
		else if (DfxEntityPropertyState::OUTPUT_STATE == getDataInstance()->getPropertyInstance(id)->getState()) {

			unbindOutput(id);
		}
	}
}

void DfxEntity::cleanPropertyBinding(size_t inid) {
	getDataInstance()->getPropertyInstance(inid)->cleanParent();
}

bool DfxEntity::isPropertyBindingDirty(size_t inid) {
	return !getDataInstance()->getPropertyInstance(inid)->isParentClean();
}

void DfxEntity::cleanAllProperties() {

	auto idList = getDataInstance()->getPropertyList();

	for (auto id : idList) {
		getDataInstance()->getPropertyInstance(id)->cleanParent();
	}
}

void DfxEntity::dirtyAllProperties() {

	auto idList = getDataInstance()->getPropertyList();

	for (auto id : idList) {
		getDataInstance()->getPropertyInstance(id)->dirtyParent();
	}
}

size_t DfxEntity::hash(DfxGlobalEntityName uniquename) {
	return std::hash<std::string>{}(uniquename);
}

bool DfxEntity::hasDataStoreProperty(std::string name) {
	auto plist = getDataInstance()->getPropertyList();

	for (auto id : plist) {

		if (DfxEntityPropertyState::DATA_STORE_STATE == getDataInstance()->getPropertyInstance(id)->getState()) {

			if (getDataInstance()->getPropertyInstance(id)->getName().compare(name) == 0) {
				return true;
			}			
		}
	}

	return false;
}

size_t DfxEntity::dataStorePropertyID(std::string name) {

	auto plist = getDataInstance()->getPropertyList();

	for (auto id : plist) {

		if (DfxEntityPropertyState::DATA_STORE_STATE == getDataInstance()->getPropertyInstance(id)->getState()) {

			if (getDataInstance()->getPropertyInstance(id)->getName().compare(name) == 0) {
				return id;
			}
		}
	}

	return 0;
}

void DfxEntity::copy(DfxEntity &other) {
	auto properties = mapEntityProperties(&other);
	mapEntityProperties(this, properties);
}

void DfxEntity::hide() {
	hidden = true;
}

void DfxEntity::unhide() {
	hidden = false;
}

bool DfxEntity::isHidden() {
	return hidden;
}

bool DfxEntity::isDelegate() {
	if (delegator != nullptr) return true;
	else return false;
}

void DfxEntity::clearDelegator() {
	delegator = nullptr;
}

DfxEntity *DfxEntity::getDelegator() {
	return delegator;
}

void DfxEntity::setDelegator(DfxEntity *ent) {
	delegator = ent;
}

std::vector<DfxEntity*> DfxEntity::getAllDelegatees() {
	return delegatees;
}

DfxEntity *DfxEntity::getDelegatee(size_t uid) {

	for (auto ent: delegatees) {

		if (ent->getInstanceID() == uid) {
			return ent;
		}
	}
	
	return nullptr;
}

void DfxEntity::addDelegatee(DfxEntity *ent) {
	ent->setDelegator(this);
	delegatees.push_back(ent);
}

void DfxEntity::removeDelegatee(size_t uid) {

	for (auto it = delegatees.begin(); it != delegatees.end(); ++it) {

		if ((*it)->getInstanceID() == uid) {
			(*it)->clearDelegator();
			it = delegatees.erase(it);
			return;
		}
	}
}

void DfxEntity::clearDelegatees() {

	for (auto it = delegatees.begin(); it != delegatees.end(); ++it) {
			(*it)->clearDelegator();
	}

	delegatees.clear();
}

bool DfxEntity::delegateesAreClean() {

	for (auto delegatee: delegatees) {

		if (!delegatee->parentsAreClean()) {
			return false;
		}
	}

	return true;
}

std::vector<DfxEntity*> DfxEntity::getDelegateTips() {
	return delegateTips;
}
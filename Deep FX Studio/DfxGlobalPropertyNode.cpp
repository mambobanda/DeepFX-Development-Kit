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

#include "DfxGlobalPropertyNodes.h"

namespace DfxNodes {

	void cleanLooseGlobal(std::string path, size_t instanceID) {


	}

	ScalarValueGlobal::ScalarValueGlobal(DfxGlobalEntityName uniquename): DfxEntity(uniquename) {
		setLogicalName("Scalar Global");

		setPropertyState(inPath, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inValue, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outValue, DfxEntityPropertyState::OUTPUT_STATE);

		_value = new float;
		*_value = 0;

		getDataInstance()->getPropertyInstance(outValue)->deleteData();
		getDataInstance()->getPropertyInstance(outValue)->setData(_value);
		getDataInstance()->getPropertyInstance(outValue)->setFreeOnDeath(false);

	}

	ScalarValueGlobal::~ScalarValueGlobal() {
		delete _value;
	}

	void ScalarValueGlobal::compute() {

		auto path = *(std::string*)getProperty(inPath);

		auto pinstance = getDataInstance()->getPropertyInstance(outValue);
		pinstance->setData(_value);

		if (path.size() > 0) {

			if (DfxGlobalProperties::getInstance()->hasProperty(path, outValue)) {

				
				auto prop = DfxGlobalProperties::getInstance()->getProperty(path, outValue);

				if (prop->instanceID != getInstanceID()) {
					auto dinstance = DfxEntityDB::getDataInstance(prop->instanceID);
					auto pinstance = dinstance->getPropertyInstance(prop->propertyID);

					if (pinstance->getType() == getDataInstance()->getPropertyInstance(outValue)->getType()) {
						getDataInstance()->getPropertyInstance(outValue)->setData(pinstance->getData());
					}
				}
			}
			else {

				auto prop = DfxGlobalProperties::getInstance()->addProperty(path, getInstanceID(), outValue);
			}

			float newValue = *(float*)getProperty(inValue);
			*(float*)getProperty(outValue) = newValue;
		}


		getDataInstance()->makeClean();
		makeChildrenDirty();
	}

	void ScalarValueGlobal::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void ScalarValueGlobal::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> ScalarValueGlobal::exportData() {
		return mapEntityProperties(this);
	}

	bool ScalarValueGlobal::isExportable() {
		return true;
	}

	void *ScalarValueGlobal::creator(DfxGlobalEntityName uniquename) {
		return new ScalarValueGlobal(uniquename);
	}

	void ScalarValueGlobal::initialize() {
		DfxEntityDB::createProperty(inPath, "global_path", new DfxEntityPropertyTypes::StringPropertyType());
		DfxEntityDB::createProperty(inValue, "in_value", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(outValue, "out_value", new DfxEntityPropertyTypes::FloatPropertyType());
	}
}
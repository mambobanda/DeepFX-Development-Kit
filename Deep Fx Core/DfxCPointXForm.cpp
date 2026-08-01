/*
* The Mailo Today Solutions Software License
*
* Copyright 2017 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#include "DfxCPointXForm.h"

namespace dfxcore {
	
	PointXForm::PointXForm(DfxGlobalEntityName uniquename): DfxEntity(uniquename) {

		setLogicalName("Point XForm");

		using namespace DfxEntityPropertyTypes;

		((Float3*)getProperty(inPosition))->x = .0f;
		((Float3*)getProperty(inPosition))->y = .0f;
		((Float3*)getProperty(inPosition))->z = .0f;

		((Float3*)getProperty(inRotation))->x = .0f;
		((Float3*)getProperty(inRotation))->y = .0f;
		((Float3*)getProperty(inRotation))->z = .0f;

		((Float3*)getProperty(inScale))->x = 1.0f;
		((Float3*)getProperty(inScale))->y = 1.0f;
		((Float3*)getProperty(inScale))->z = 1.0f;

		setPropertyState(inPosition, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inRotation, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inScale, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPointArray, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outPointArray, DfxEntityPropertyState::OUTPUT_STATE);
	}
	
	PointXForm::~PointXForm() {

	}

	void PointXForm::compute() {

		using namespace DfxEntityPropertyTypes;

		Float3 *_pos = (Float3*)getProperty(inPosition);
		glm::vec3 pos(_pos->x, _pos->y, _pos->z);

		Float3 *_rot = (Float3*)getProperty(inRotation);
		glm::vec3 rot(_rot->x, _rot->y, _rot->z);

		Float3 *_scl = (Float3*)getProperty(inScale);
		glm::vec3 scl(_scl->x, _scl->y, _scl->z);

		Float3Array *input = (Float3Array*)getProperty(inPointArray);

		Float3Array *output = (Float3Array *)getProperty(outPointArray);

		if (isPropertyBindingDirty(inPosition) ||
			isPropertyBindingDirty(inRotation) ||
			isPropertyBindingDirty(inScale)    ||
			isPropertyBindingDirty(inPointArray)) {

			if (!input->isNull()) {

				if (input->size() > 0) {

					glm::mat4 transMatrix = glm::mat4(1.0f);

					transMatrix = glm::translate(transMatrix, pos);

					transMatrix *= glm::rotate(glm::mat4(), glm::radians(rot.x), glm::vec3(1.f, 0.0, 0.0));
					transMatrix *= glm::rotate(glm::mat4(), glm::radians(rot.y), glm::vec3(0.f, 1.0, 0.0));
					transMatrix *= glm::rotate(glm::mat4(), glm::radians(rot.z), glm::vec3(0.f, 0.0, 1.0));

					output->resize(input->size());
					for (int i = 0; i < input->size(); i++) {

						glm::vec3 inpos((*input)[i].x * scl.x, (*input)[i].y * scl.y, (*input)[i].z * scl.z);
						glm::vec3 npos = glm::vec3(transMatrix * glm::vec4(inpos , 1.0f));

						(*output)[i].x = npos.x;
						(*output)[i].y = npos.y;
						(*output)[i].z = npos.z;
					}

					makePropertyBindingDirty(outPointArray);
				}
				else {
					output->deleteArray();
				}

				cleanAllProperties();
				getDataInstance()->makeClean();
				makeChildrenDirty();
			}
		}

	}

	void PointXForm::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void PointXForm::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> PointXForm::exportData() {
		return mapEntityProperties(this);
	}

	bool PointXForm::isExportable() {
		return true;
	}

	void *PointXForm::creator(DfxGlobalEntityName uniquename) {
		return new PointXForm(uniquename);
	}


	void PointXForm::initialize() {

		DfxEntityDB::createProperty(inPosition, "position", new DfxEntityPropertyTypes::Float3PropertyType);
		DfxEntityDB::createProperty(inRotation, "rotation", new DfxEntityPropertyTypes::Float3PropertyType);
		DfxEntityDB::createProperty(inScale,"scale", new DfxEntityPropertyTypes::Float3PropertyType);
		DfxEntityDB::createProperty(inPointArray, "in_points", new DfxEntityPropertyTypes::Float3ArrayPropertyType);
		DfxEntityDB::createProperty(outPointArray, "points", new DfxEntityPropertyTypes::Float3ArrayPropertyType);
	}
}
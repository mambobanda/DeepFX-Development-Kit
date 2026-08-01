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

#include "DfxPointGrid.h"

namespace dfxcore {

	DfxPointGrid::DfxPointGrid(DfxGlobalEntityName uniquename) : DfxEntity(uniquename) {

		setLogicalName("Point Grid");

		using namespace DfxEntityPropertyTypes;

		((Float3*)getProperty(inNumberOfPoints))->x = 5.f;
		((Float3*)getProperty(inNumberOfPoints))->y = 5.f;
		((Float3*)getProperty(inNumberOfPoints))->z = 5.f;

		((Float3*)getProperty(inPointSpacing))->x = 1.f;
		((Float3*)getProperty(inPointSpacing))->y = 1.f;
		((Float3*)getProperty(inPointSpacing))->z = 1.f;

		((Float3*)getProperty(inGlobalPosition))->x = 0.f;
		((Float3*)getProperty(inGlobalPosition))->y = 0.f;
		((Float3*)getProperty(inGlobalPosition))->z = 0.f;

		*(float*)getProperty(inRandomisePosition) = .0f;

		setPropertyState(inNumberOfPoints, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inPointSpacing, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inGlobalPosition, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inRandomisePosition, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(outPointArray, DfxEntityPropertyState::OUTPUT_STATE);
	}

	DfxPointGrid::~DfxPointGrid() {

	}

	void DfxPointGrid::compute() {
		using namespace DfxEntityPropertyTypes;

		Float3Array *output = (Float3Array*)getProperty(outPointArray);
		Float3 *globalPos = (Float3*)getProperty(inGlobalPosition);

		glm::dvec3 numPts, spacing;
		double randPosOffset, randSclOffset, randRotOffset;

		numPts.x = ((Float3*)getProperty(inNumberOfPoints))->x;
		numPts.y = ((Float3*)getProperty(inNumberOfPoints))->y;
		numPts.z = ((Float3*)getProperty(inNumberOfPoints))->z;
		
		spacing.x = ((Float3*)getProperty(inPointSpacing))->x;
		spacing.y = ((Float3*)getProperty(inPointSpacing))->y;
		spacing.z = ((Float3*)getProperty(inPointSpacing))->z;

		randPosOffset = *(float*)getProperty(inRandomisePosition);

		std::time_t time = std::time(nullptr);
		std::uniform_real_distribution<> distPos(0.0, randPosOffset);
		unsigned int k = 10;

		DfxGlobal::info::message("Generating Point Grid...");

		if (numPts.x > 0.0f ||
			numPts.y > 0.0f ||
			numPts.z > 0.0f) {

			int xcount = (int)numPts.x;
			int ycount = (int)numPts.y;
			int zcount = (int)numPts.z;

			std::vector<glm::dvec3> _new_points;

			double xspacing = 0.0f;
			double yspacing = 0.0f;
			double zspacing = 0.0f;


			for (int x = 0; x < xcount; x++) {
				for (int y = 0; y < ycount; y++) {
					for (int z = 0; z < zcount; z++) {
						std::default_random_engine genx((++k)*(unsigned int)time);
						std::default_random_engine geny((++k)*(unsigned int)time);
						std::default_random_engine genz((++k)*(unsigned int)time);

						float posx = (float)distPos(genx);
						float posy = (float)distPos(geny);
						float posz = (float)distPos(genz);

						glm::dvec3 newPoint;

						newPoint.x = xspacing + posx;
						newPoint.y = yspacing + posy;
						newPoint.z = zspacing + posz;

						_new_points.push_back(newPoint);

						zspacing += spacing.z;
					}

					zspacing = 0.0f;
					yspacing += spacing.y;
				}

				zspacing = 0.0f;
				yspacing = 0.0f;
				xspacing += spacing.x;
			}

			glm::vec3 gp(globalPos->x, globalPos->y, globalPos->z);
			glm::mat4 trans = glm::translate(glm::mat4(1.f), gp);

			output->deleteArray();
			if (_new_points.size() > 0) {

				if (_new_points.size() > 0) {
					output->resize(_new_points.size());

					for (int i = 0; i < _new_points.size(); i++) {

						glm::vec3 temp = glm::vec3(trans * glm::vec4(_new_points[i], 1.f));

						(*output)[i].x = temp.x;
						(*output)[i].y = temp.y;
						(*output)[i].z = temp.z;
					}
				}
			}		
		}

		makePropertyBindingDirty(outPointArray);
		getDataInstance()->makeClean();
		makeChildrenDirty();

		DfxGlobal::info::message("");
	}


	void DfxPointGrid::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void DfxPointGrid::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxPointGrid::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxPointGrid::isExportable() {
		return true;
	}

	void *DfxPointGrid::creator(DfxGlobalEntityName uniquename) {
		return new DfxPointGrid(uniquename);
	}

	void DfxPointGrid::initialize() {

		DfxEntityDB::createProperty(inNumberOfPoints, "point_count", new DfxEntityPropertyTypes::Float3PropertyType);
		DfxEntityDB::createProperty(inPointSpacing, "point_spacing", new DfxEntityPropertyTypes::Float3PropertyType);
		DfxEntityDB::createProperty(inGlobalPosition, "global_position", new DfxEntityPropertyTypes::Float3PropertyType);
		DfxEntityDB::createProperty(inRandomisePosition, "random_factor", new DfxEntityPropertyTypes::FloatPropertyType);
		DfxEntityDB::createProperty(outPointArray, "points", new DfxEntityPropertyTypes::Float3ArrayPropertyType);
	}
}
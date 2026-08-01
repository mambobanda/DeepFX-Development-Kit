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

#include "DfxCUniformMeshSampler.h"


namespace dfxcore {

	DfxUniformMeshSamplerNode::DfxUniformMeshSamplerNode(DfxGlobalEntityName uniquename): DfxEntity(uniquename) {
		setLogicalName("Uniform Mesh Sampler");

		using namespace DfxEntityPropertyTypes;

		setPropertyState(inRef, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inScale, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inSpacing, DfxEntityPropertyState::INPUT_STATE);
		setPropertyState(inRandomDisplacement, DfxEntityPropertyState::INPUT_STATE);

		setPropertyState(outPoints, DfxEntityPropertyState::OUTPUT_STATE);

		*(float*)getProperty(inSpacing) = .04f;
		*(float*)getProperty(inRandomDisplacement) = .015f;

		((Float3*)getProperty(inScale))->x = 1.f;
		((Float3*)getProperty(inScale))->y = 1.f;
		((Float3*)getProperty(inScale))->z = 1.f;
	}

	DfxUniformMeshSamplerNode::~DfxUniformMeshSamplerNode() {

	}

	void DfxUniformMeshSamplerNode::randomisePoints(vector<glm::vec3> &points){
		float rand = *(float*)getProperty(inRandomDisplacement);

		std::uniform_real_distribution<> distPos(0.0, rand);

		std::default_random_engine genx(9847);
		std::default_random_engine geny(4544);
		std::default_random_engine genz(2958);

		for (size_t i = 0; i < points.size(); i++) {

			float posx = (float)distPos(genx);
			float posy = (float)distPos(geny);
			float posz = (float)distPos(genz);

			points[i] += glm::vec3(posx, posy, posz);
		}
	}
	void DfxUniformMeshSamplerNode::compute() {
		using namespace DfxEntityPropertyTypes;

		UID *uid = (UID*)getProperty(inRef);
		Float3 scl = *(Float3*)getProperty(inScale);
		float spacing = *(float*)getProperty(inSpacing);
		Float3Array *output = (Float3Array *)getProperty(outPoints);

		output->deleteArray();

		if (propertyHasBinding(inRef)) {

			Dfx3dMeshData *mesh = dfxutil::findMeshData(uid);

			if (mesh != nullptr) {

				glm::vec3 scale(scl.x, scl.y, scl.z);
				vector<glm::vec3> points;
				VectorMeshSampler::sampleMesh(*mesh, scale, spacing, points);
				
				if (points.size() > 0) {
					randomisePoints(points);
					output->resize(points.size());

					for (int i = 0; i < points.size(); i++) {
						(*output)[i].x = points[i].x;
						(*output)[i].y = points[i].y;
						(*output)[i].z = points[i].z;
					}

					DfxGlobal::console::println("Uniform Samples Count: " + std::to_string(points.size()) + "\n");
					makePropertyBindingDirty(outPoints);
				}
			}
		}

		cleanAllProperties();
		makeChildrenDirty();
		getDataInstance()->makeClean();
	}

	void DfxUniformMeshSamplerNode::onDataChanged(DfxEntityDataInstance *instance) {

	}

	void DfxUniformMeshSamplerNode::importData(std::vector<std::pair<std::string, std::string>> data) {

	}

	std::vector<std::pair<std::string, std::string>> DfxUniformMeshSamplerNode::exportData() {
		return mapEntityProperties(this);
	}

	bool DfxUniformMeshSamplerNode::isExportable() {
		return true;
	}

	void *DfxUniformMeshSamplerNode::creator(DfxGlobalEntityName uniquename) {
		return new DfxUniformMeshSamplerNode(uniquename);
	}

	void DfxUniformMeshSamplerNode::initialize() {

		DfxEntityDB::createProperty(outPoints, "points", new DfxEntityPropertyTypes::Float3ArrayPropertyType());
		DfxEntityDB::createProperty(inRef, "in_ref", new DfxEntityPropertyTypes::UIDPropertyType());
		DfxEntityDB::createProperty(inScale, "in_scale", new DfxEntityPropertyTypes::Float3PropertyType());
		DfxEntityDB::createProperty(inSpacing, "in_spacing", new DfxEntityPropertyTypes::FloatPropertyType());
		DfxEntityDB::createProperty(inRandomDisplacement, "in_randomise", new DfxEntityPropertyTypes::FloatPropertyType());
	}


}
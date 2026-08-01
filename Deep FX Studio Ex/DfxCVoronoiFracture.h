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
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <FlSpace.h>
#include <FlInterpreterScope.h>
#include <FlModel.h>
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"

namespace compute_node {

	static const std::string voronoiFracture_model_id = "/COMPUTE/VORONOI_FRACTURE_NODE";

	class VoronoiFractureNodeHandler : public fl::Model {
	public:

		/// Voronoi Node
		static void voronoiFractureMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues);

		static inline fl::Any *voronoiFractureNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("voronoi_fracture")
				.setLabel("Voronoi Fracture")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(voronoiFracture_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "set", "Set", std::string())
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "inner_material_id", "Inner Material ID", 0)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "seed", "Seed", 5444.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "cells", "Cells", 20.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "sites_per_cluster", "Sites Per Cluster", 20.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "cluster_radius", "Cluster Radius", 0.09f)
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "in_set", "Origin Set", std::string(""))
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::INPUT, "output", "Output", 1)
				.addProperty(space, fl::type::FLOAT_3, fl::role::INPUT, "samples", "Samples", glm::vec3())
				.addProperty(space, fl::type::BOOL, fl::role::INPUT, "generate_clusters", "Generate Clusters", false)
				.addProperty(space, fl::type::BOOL, fl::role::INPUT, "inherit_attributes", "Inherit Attributes", true)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, voronoiFractureMethod)
				.build(space);

			std::vector<std::pair<std::string, unsigned int>> typesEnum;
			typesEnum.push_back(std::pair<std::string, unsigned int>("Original Mesh", 0));
			typesEnum.push_back(std::pair<std::string, unsigned int>("Fractured Mesh", 1));

			mnode->rawData<fl::DataObject>()->property("output")->setEnumerations(true);
			mnode->rawData<fl::DataObject>()->property("output")->setEnumArray(typesEnum);

			mnode->rawData<fl::DataObject>()->property("samples")->newData<std::vector<glm::vec3>>(fl::type::FLOAT_3);
			mnode->rawData<fl::DataObject>()->property("samples")->setIsArray(true);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return voronoiFractureNode(space);
		}
	};

	inline void registerVoronoiFractureNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = voronoiFracture_model_id;
		desc->name = "Voronoi Fracture";
		desc->description = "Voronoi Fracture Tool";
		desc->category  = "Fracture";
		desc->placement = "Fracture/Voronoi Fracture";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = VoronoiFractureNodeHandler::voronoiFractureNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}
}
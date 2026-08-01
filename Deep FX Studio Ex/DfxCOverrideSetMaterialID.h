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
#include "DfxCacheTrackManager.h"


namespace compute_node {

	static const std::string overrideSetMatID_model_id = "/COMPUTE/OVERRIDE_SET_MAT_ID_NODE";

	class OverrideSetMatIDHandler : public fl::Model {
	public:

		/// Track Set Size Node
		static inline void overrideSetMatIDMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::FLOAT ||
					args[2]->type() != fl::type::FLOAT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0, 2], expected float";
					return;
				}

				if (args[1]->type() != fl::type::STD_STRING) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type[1], expected string";
					return;
				}

				if (args[0]->isArray() || args[1]->isArray()) {
					error->hasError = true;
					error->errorMsg = "Inputs [0, 1] dont't accept arrays.";
					return;
				}

				if (!args[2]->isArray()) {
					error->hasError = true;
					error->errorMsg = "Input [2] only accepts arrays.";
					return;
				}

				auto track = (int)std::abs(args[0]->data<float>());
				auto set = args[1]->data<std::string>();
				auto matids = args[2]->rawData<std::vector<float>>();

				float count = 0.f;
				if (DfxCacheTrackManager::getInstance()->tracks().size() > 0 && track > 0) {
					if (track <= DfxCacheTrackManager::getInstance()->tracks().size() && track > 0) {
						auto ctrack = DfxCacheTrackManager::getInstance()->tracks()[track - 1];

						if (ctrack->snapshot() != nullptr) {
							for (auto iset : ctrack->snapshot()->sets()) {
								if (iset->name() == set) {
									if (iset->indices().size() == matids->size()) {
										if (iset->type() == DfxFluentSet::SIMPLEX) {
											for (size_t i = 0; i < ctrack->snapshot()->simplexObjects().size(); i++) {
												for (size_t j = 0; j < ctrack->snapshot()->simplexObjects()[i]->triangleCount(); j++) {
													ctrack->snapshot()->simplexObjects()[i]->faceIDs()[j] = (unsigned int)(*matids)[i];
												}
											}
										}
									}
									else {
										error->hasError = true;
										error->errorMsg = "Set size & material ID array size mismatch. They must have the same size.)";
										return;
									}
								}					
							}
						}
					}
				}

				auto result00 = new fl::Any();
				result00->newData<float>(fl::type::FLOAT);
				result00->setData<float>(track);

				auto result01 = new fl::Any();
				result01->newData<std::string>(fl::type::STD_STRING);
				result01->setData<std::string>(set);

				returnValues.push_back(result00);
				returnValues.push_back(result01);
			}
		}

		static inline fl::Any *overrideSetMatIDNode(fl::SpaceInterface *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("override_set_mat_id")
				.setLabel("Override Set MatID")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(overrideSetMatID_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "out_track", "Track", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "out_set", "Set", std::string(""))
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "in_track", "Track", 1.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "set_name", "Set Name", std::string(""))
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "mat_id", "Material ID", 0.f)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, overrideSetMatIDMethod)
				.build(space);

			mnode->rawData<fl::DataObject>()->property("mat_id")->newData<std::vector<float>>(fl::type::FLOAT);
			mnode->rawData<fl::DataObject>()->property("mat_id")->setIsArray(true);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return overrideSetMatIDNode(space);
		}
	};

	inline void registerOverrideSetMatIDNode() {
		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = overrideSetMatID_model_id;
		desc->name = "Track Override Set MatID";
		desc->description = "Override material ID in track set";
		desc->category = "Track";
		desc->placement = "Track/Override Set MatID";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = OverrideSetMatIDHandler::overrideSetMatIDNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}
}
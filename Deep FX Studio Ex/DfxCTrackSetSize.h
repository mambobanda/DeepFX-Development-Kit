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

	static const std::string trackSetSize_model_id = "/COMPUTE/TRACK_SET_SIZE_NODE";

	class TrackSetSizeHandler : public fl::Model {
	public:

		/// Track Set Size Node
		static inline void trackSetSizeMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::FLOAT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0], expected float";
					return;
				}

				if (args[1]->type() != fl::type::STD_STRING) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type[1], expected string";
					return;
				}

				if (args[0]->isArray() || args[1]->isArray()) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}

				auto track = (int)std::abs(args[0]->data<float>());
				auto set   = args[1]->data<std::string>();

				float count = 0.f;
				if (DfxCacheTrackManager::getInstance()->tracks().size() > 0 && track > 0) {
					if (track <= DfxCacheTrackManager::getInstance()->tracks().size() && track > 0) {
						auto ctrack = DfxCacheTrackManager::getInstance()->tracks()[track - 1];
						
						if (ctrack->snapshot() != nullptr) {
							for (auto iset: ctrack->snapshot()->sets()) {
								if (iset->name() == set) {
									count = iset->indices().size();
								}
							}
						}
					}
				}

				auto result = new fl::Any();
				result->newData<float>(fl::type::FLOAT);
				result->setData<float>(count);

				returnValues.push_back(result);
			}
		}

		static inline fl::Any *trackSetSizeNode(fl::SpaceInterface *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("track_set_size")
				.setLabel("Track Set Size")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(trackSetSize_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "size", "Size", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "track", "Track", 1.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "set_name", "Set Name", std::string(""))
				.addStaticFunction(fl::DataObject::STATIC_MAIN, trackSetSizeMethod)
				.build(space);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return trackSetSizeNode(space);
		}
	};

	inline void registerTrackSetSizeNode() {
		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = trackSetSize_model_id;
		desc->name = "Track Set Size";
		desc->description = "Retrieve set size from track";
		desc->category = "Track";
		desc->placement = "Track/Set Size";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = TrackSetSizeHandler::trackSetSizeNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}
}
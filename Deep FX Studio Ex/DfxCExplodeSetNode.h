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

	static const std::string explodeSet_model_id = "/COMPUTE/EXPLODE_SET_NODE";

	class ExplodeSetNodeHandler : public fl::Model {
	public:

		/// Explode Set Node
		static inline void explodeSetMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues)
		{
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::FLOAT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type";

					return;
				}


				//glm::vec3 opos = glm::vec3(proxPtr->position.x, proxPtr->position.y, proxPtr->position.z);
				//glm::vec3 pos = opos + (glm::normalize(opos - proxCenter) * spacing);
				//glm::vec3 rot = glm::vec3(proxPtr->rotation.x, proxPtr->rotation.y, proxPtr->rotation.z);
				//glm::vec3 scl = glm::vec3(proxPtr->scale.x, proxPtr->scale.y, proxPtr->scale.z);

				//glm::mat4 transMat = createTransformationMatrix(pos, rot, scl);

				/*auto result = new fl::Any();
				result->newData<float>(fl::type::FLOAT);
				result->setData<float>(a);*/

				//returnValues.push_back(result);
			}
		}

		static inline fl::Any *explodeSetNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("explode_set")
				.setLabel("Explode Set")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(explodeSet_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "out_set", "Set", std::string(""))
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "in_set", "Set", std::string(""))
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "spacing", "Spacing", 1.f)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, explodeSetMethod)
				.build(space);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return explodeSetNode(space);
		}
	};

	inline void registerExplodeSetNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = explodeSet_model_id;
		desc->name = "Explode Set";
		desc->description = "Move items away relative to each other ";
		desc->category = "Transform";
		desc->placement = "Transform/Explode Set";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = ExplodeSetNodeHandler::explodeSetNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}
}
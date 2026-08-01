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
#include "DfxFluentSet.h"
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"
#include "DfxFluentSetManager.h"
#include "DfxFluentBigID.h"

namespace compute_node {

	static const std::string toggleSetVisibilty_model_id = "/COMPUTE/TOGGLE_SET_NODE";

	class ToggleSetVisibiltyNodeHandler : public fl::Model {
	public:

		/// Float Node
		static inline void toggleSetVisibilityMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues)
		{
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::STD_STRING) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0]";
					return;
				}

				if (args[1]->type() != fl::type::UNSIGNED_INT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [1]";
					return;
				}	

				if (args[0]->isArray() || args[1]->isArray()) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}

				auto setName = args[0]->data<std::string>();
				auto action  = args[1]->data<unsigned int>();

				std::string output = "";

				if (!setName.empty()) {
					auto set = DfxFluentSetManager::getInstance()->getSet(setName);

					if (set != nullptr) {
						output = setName;

						if (action == 0) {
							set->setIsVisible(false);
						}
						else if (action == 1) {
							set->setIsVisible(true);
						}
						else {
							error->hasError = true;
							error->errorMsg = "Unknown Action";
							return;
						}
					}
					else {
						error->hasError = true;
						error->errorMsg = "Set Doesn't Exist.";
						return;
					}
				}

				auto rany = new fl::Any();
				rany->newData<std::string>(fl::type::STD_STRING);
				rany->setData(output);

				returnValues.push_back(rany);

			}
		}

		static inline fl::Any *toggleSetVisibilityNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("toggle_set_visibilty")
				.setLabel("Toggle Set Visibilty")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(toggleSetVisibilty_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "out_set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "in_set", "Set", std::string())
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::INPUT, "action", "Action", 0)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, toggleSetVisibilityMethod)
				.build(space);

			std::vector<std::pair<std::string, unsigned int>> actionEnum;
			actionEnum.push_back(std::pair<std::string, unsigned int>("Hide", 0));
			actionEnum.push_back(std::pair<std::string, unsigned int>("Show", 1));

			mnode->rawData<fl::DataObject>()->property("action")->setEnumerations(true);
			mnode->rawData<fl::DataObject>()->property("action")->setEnumArray(actionEnum);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return toggleSetVisibilityNode(space);
		}
	};

	inline void registerToggleSetVisibilityNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = toggleSetVisibilty_model_id;
		desc->name = "Toggle Set Visibility";
		desc->description = "Hide or show a set in the viewport or render.";
		desc->category = "Sets";
		desc->placement = "Sets/Toggle Visibility";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = ToggleSetVisibiltyNodeHandler::toggleSetVisibilityNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}

}
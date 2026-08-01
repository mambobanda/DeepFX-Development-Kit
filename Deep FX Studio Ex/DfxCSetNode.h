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

	static const std::string set_model_id = "/COMPUTE/SET_NODE";

	class SetNodeHandler : public fl::Model {
	public:

		/// Float Node
		static inline void setMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues)
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

				if (args[3]->type() != fl::type::BOOL) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [2]";

					return;
				}

				if (args[0]->isArray() || args[1]->isArray() || args[3]->isArray()) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}

				auto gname = args[0]->data<std::string>();
				auto type  = args[1]->data<unsigned int>();
				auto oexisting = args[3]->data<bool>();
					
				auto cr = dao->property("color_r")->data<float>();
				auto cg = dao->property("color_g")->data<float>();
				auto cb = dao->property("color_b")->data<float>();

				glm::vec3 color(cr, cg, cb);

				std::string output = "";

				if (!gname.empty()) {
					bool g_exists = DfxFluentSetManager::getInstance()->hasSet(gname);

					if (!g_exists && !oexisting) {

						if (type == 0) {
							auto set = new DfxFluentSet(DfxFluentSet::SIMPLEX);
							set->setColor(color);
							set->setName(gname);
							DfxFluentSetManager::getInstance()->addSet(gname, set);
							output = gname;
						}else if (type == 1){
							auto set = new DfxFluentSet(DfxFluentSet::STREAM);
							set->setColor(color);
							set->setName(gname);
							DfxFluentSetManager::getInstance()->addSet(gname, set);
							output = gname;
						}
						else {
							error->hasError = true;
							error->errorMsg = "Unknown Set Type";

							return;
						}
					}
					else if (g_exists) {
						output = gname;
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

		static inline fl::Any *setNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("set")
				.setLabel("Set")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(set_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "out_name", "Name", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "in_name", "Name", std::string("global"))
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::INPUT, "type", "Set Type", 0)
				.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "color", "Color", std::string(""))
				.addProperty(space, fl::type::BOOL, fl::role::INPUT, "only_existing", "Only Existing", false)

				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "color_r", "Diffuse Red", 1.0f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "color_g", "Diffuse Green", 0.0f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "color_b", "Diffuse Blue", 0.0f)

				.addStaticFunction(fl::DataObject::STATIC_MAIN, setMethod)
				.build(space);

			std::vector<std::pair<std::string, unsigned int>> typesEnum;
			typesEnum.push_back(std::pair<std::string, unsigned int>("Simplex", 0));
			typesEnum.push_back(std::pair<std::string, unsigned int>("Stream", 1));

			mnode->rawData<fl::DataObject>()->property("type")->setEnumerations(true);
			mnode->rawData<fl::DataObject>()->property("type")->setEnumArray(typesEnum);

			mnode->rawData<fl::DataObject>()->property("color")->setMiscFlag(-200);
			mnode->rawData<fl::DataObject>()->property("color")->addColorReferences("color_r", "color_g", "color_b");

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return setNode(space);
		}
	};

	inline void registerSetNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = set_model_id;
		desc->name = "Set";
		desc->description = "Create/Retrieve Existing set.";
		desc->category = "Sets";
		desc->placement = "Sets/Set";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = SetNodeHandler::setNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}

}
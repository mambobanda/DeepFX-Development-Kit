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

	static const std::string moveSet_model_id = "/COMPUTE/MOVE_SET_NODE";

	class MoveSetNodeHandler : public fl::Model {
	public:

		/// Float Node
		static inline void moveSetMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues)
		{
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::STD_STRING ||
					args[1]->type() != fl::type::STD_STRING) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0] [1], expected string";

					return;
				}

				if (args[2]->type() != fl::type::UNSIGNED_INT ) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [2], expected uint";

					return;
				}

				if (args[0]->isArray() || args[1]->isArray() || args[2]->isArray()) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}

				auto from = args[0]->data<std::string>();
				auto to = args[1]->data<std::string>();
				auto soutput = args[1]->data<unsigned int>();

				std::string output = "";

				if (!from.empty() && !to.empty()) {
					auto fromSet = DfxFluentSetManager::getInstance()->getSet(from);
					auto toSet = DfxFluentSetManager::getInstance()->getSet(to);

					if (fromSet != nullptr && toSet != nullptr) {

						if (fromSet->type() != DfxFluentSet::EMPTY &&
							toSet->type() != DfxFluentSet::EMPTY) {

							if (toSet->isSoftSet()) {
								error->hasError = true;
								error->errorMsg = "Cannot move to a temporary set (soft set).";
								return;
							}

							if (fromSet->type() == toSet->type()) {
								std::string *setNames = nullptr;

								if (fromSet->type() == DfxFluentSet::SIMPLEX)
									setNames = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<std::string>("@set_name");
								else if (fromSet->type() == DfxFluentSet::STREAM)
									setNames = DfxFluentStreamCollection::getInstance()->atrributeMap().at<std::string>("@set_name");

								std::vector<size_t> indices(fromSet->indices());
								std::vector<size_t> moveIndices;
								for (size_t i = 0; i < indices.size(); i++) {
									auto inx = indices[i];
									auto sname = setNames[inx];

									auto tset = DfxFluentSetManager::getInstance()->getSet(sname);
									if (tset != nullptr) {
										tset->removeIndex(inx);
										moveIndices.push_back(inx);
									}
								}

								toSet->addIndices(moveIndices);

								if (soutput == 0) output = from;
								else output = to;
							}
							else {
								error->hasError = true;
								error->errorMsg = "Set A and Set B reference different types.";
								return;
							}
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

		static inline fl::Any *moveSetNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("move_set")
				.setLabel("Move Set")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(moveSet_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "from", "From", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "to", "To", std::string())
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::INPUT, "output", "Output", 1)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, moveSetMethod)
				.build(space);

			std::vector<std::pair<std::string, unsigned int>> outputTypesEnum;
			outputTypesEnum.push_back(std::pair<std::string, unsigned int>("from", 0));
			outputTypesEnum.push_back(std::pair<std::string, unsigned int>("to", 1));

			mnode->rawData<fl::DataObject>()->property("output")->setEnumerations(true);
			mnode->rawData<fl::DataObject>()->property("output")->setEnumArray(outputTypesEnum);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return moveSetNode(space);
		}
	};

	inline void registerMoveSetNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = moveSet_model_id;
		desc->name = "Move Set";
		desc->description = "Move Contents of Set a to Set b.";
		desc->category = "Sets";
		desc->placement = "Sets/Move Set";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = MoveSetNodeHandler::moveSetNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}

}
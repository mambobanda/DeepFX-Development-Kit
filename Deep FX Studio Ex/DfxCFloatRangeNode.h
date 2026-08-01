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
#include <FlModel.h>
#include <FlInterpreterScope.h>
#include <DfxGlobal.h>
#include <random>
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"

namespace compute_node {

	static const std::string floatRange_model_id = "/COMPUTE/FLOAT_RANGE_NODE";

	class FloatRangeNodeHandler : public fl::Model {
	public:

		/// Scalar Node
		static inline void floatRangeMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {

			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);
			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::FLOAT ||
					args[1]->type() != fl::type::FLOAT ||
					args[2]->type() != fl::type::FLOAT ||
					args[3]->type() != fl::type::FLOAT ||
					args[4]->type() != fl::type::FLOAT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0, 1, 2, 3, 4], expected float.";

					return;
				}

				if (args[5]->type() != fl::type::BOOL ||
					args[6]->type() != fl::type::BOOL ||
					args[7]->type() != fl::type::BOOL ||
					args[8]->type() != fl::type::BOOL) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [5, ,6, 7, 8], expected bool";

					return;
				}

				else if (args[0]->isArray() || args[1]->isArray() || args[2]->isArray() || args[3]->isArray() || args[4]->isArray() ||
					args[5]->isArray() || args[6]->isArray() || args[7]->isArray() || args[8]->isArray()) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}

				auto arrPtr = new std::vector<float>();

				float min = args[0]->data<float>();
				float max = args[1]->data<float>();
				float increment = args[2]->data<float>();
				float random_min = args[3]->data<float>();
				float random_max = args[4]->data<float>();
				bool includeMin = args[5]->data<bool>();
				bool includeMax = args[6]->data<bool>();
				bool random = args[7]->data<bool>();
				bool floor = args[8]->data<bool>();

				float counter = 0.f;
				float counterMax = 0.f;

				if (includeMin) counter = min;
				else counter = min + 1.f;

				if (includeMax) counterMax = max;
				else counterMax = max - 1.f;

				std::minstd_rand gen00(5555);
				std::uniform_int_distribution<int> dist00((int)random_min, (int)random_max);

				std::minstd_rand gen01(5555);
				std::uniform_real<float> dist01(random_min, random_max);

				if (!random) {
					if (counterMax > counter && increment != 0) {
						while (counter <= counterMax) {
							if (floor) {
								arrPtr->push_back(std::floorf(counter));
							}
							else {
								arrPtr->push_back(counter);
							}
							counter += increment;
						}
					}

				}
				else {

					if (random_max > random_min) {

						if (counterMax > counter && increment != 0) {
							while (counter <= counterMax) {

								if (floor) {
									if (std::floorf(random_max) > std::floorf(random_min)) {
										arrPtr->push_back((float)dist00(gen00));
									}
								}
								else {
									

									arrPtr->push_back(dist01(gen01));
								}
								counter += increment;
							}
						}
					}

				}

				auto rany00 = new fl::Any();
				rany00->newData<std::vector<float>>(fl::type::FLOAT, arrPtr);
				rany00->setIsArray(true);

				auto rany01 = new fl::Any();
				rany01->newData<float>(fl::type::FLOAT);
				rany01->setData<float>(arrPtr->size());

				returnValues.push_back(rany00);
				returnValues.push_back(rany01);

			}
		}

		static inline fl::Any *floatRangeNode(fl::SpaceInterface *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("float_range")
				.setLabel("Float Range")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(floatRange_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "array", "Array", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "size", "Size", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "min", "Min", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "max", "Max", 10.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "increment", "Increment", 1.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "min_random", "Random Min", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "max_random", "Random Max", 5.f)
				.addProperty(space, fl::type::BOOL, fl::role::INPUT, "include_min", "Include Min", true)
				.addProperty(space, fl::type::BOOL, fl::role::INPUT, "include_max", "Include Max", true)
				.addProperty(space, fl::type::BOOL, fl::role::INPUT, "random", "Random", false)
				.addProperty(space, fl::type::BOOL, fl::role::INPUT, "floor", "Floor", false)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, floatRangeMethod)
				.build(space);

			mnode->rawData<fl::DataObject>()->property("array")->setIsArray(true);
			mnode->rawData<fl::DataObject>()->property("array")->newData<std::vector<float>>(fl::type::FLOAT);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return floatRangeNode(space);
		}
	};

	inline void registerFloatRangeNode() {
		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = "DFX_FLOAT_RANGE";
		desc->name = "Float Range";	
		desc->description = "Generate an array with a range of numbers from the min to max.";
		desc->category = "Array";
		desc->placement = "Array/Range";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = FloatRangeNodeHandler::floatRangeNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}
}
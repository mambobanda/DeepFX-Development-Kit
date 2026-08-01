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
#include <limits>
#include "DfxFluentSet.h"
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"
#include "DfxFluentSetManager.h"
#include "DfxFluentBigID.h"

namespace compute_node {

	static const std::string filterFloatAttribute_model_id = "/COMPUTE/FILTER_FLOAT_ATTRIBUTE_NODE";	

	inline bool comparisonFunc(unsigned int op, float a, float b) {

		if (op == 0) {
			return a < b;
		}
		else if (op == 1) {
			return a > b;
		}
		else if (op == 2) {
			return a <= b;
		}
		else if (op == 3) {
			return a >= b;
		}
		else if (op == 4) {
			return a == b;
		}
		else if (op == 5) {
			return a != b;
		}
		else if (op == 16) {
			return a > 0;
		}
		else if (op == 17) {
			return a <= 0;
		}

		return false;
	}

	inline bool modOp(unsigned int op, float a, float b) {

		if (op == 6) {
			return std::fmodf(a, std::fmaxf(b, 0.0001f)) == 0;
		}
		else if (op == 7) {
			return std::fmodf(a, std::fmaxf(b, 0.0001f)) != 0;
		}
		else if (op == 8) {
			return std::fmodf(b, std::fmaxf(b, 0.0001f)) == 0;
		}
		else if (op == 9) {
			return std::fmodf(b, std::fmaxf(b, 0.0001f)) != 0;
		}
		
		return 0.f;
	}

	inline bool comparisonFunc(unsigned int op, float attr, float a, float b) {

		if (op == 12) {
			return (a < attr && attr < b);
		}
		else if (op == 13) {
			return (a <= attr && attr <= b);
		}
		else if (op == 14) {
			return (a < attr && attr <= b);
		}
		else if (op == 15) {
			return (a <= attr && attr < b);
		}

		return false;
	}
	
	class FilterFloatAttributeNodeHandler : public fl::Model {
	public:

		static inline std::vector<size_t> filter(unsigned int comparison, DfxFluentSet *set, float *attribute, float a, float b) {
			std::vector<size_t> output;

			if ((0 <= comparison && comparison <= 5) || comparison == 16 || comparison == 17) {
				for (size_t inx = 0; inx < set->indices().size(); inx++) {
					if (comparisonFunc(comparison, attribute[inx], a)) output.push_back(set->indices()[inx]);
				}
			}
			else if (6 <= comparison && comparison <= 9) {
				for (size_t inx = 0; inx < set->indices().size(); inx++) {
					if (modOp(comparison, attribute[inx], a)) output.push_back(inx);
				}
			}
			else if (comparison == 10) {
				if (!set->indices().empty()) {
					size_t _inx = 0;
					float maxfl = -std::numeric_limits<float>::max();
					for (size_t inx = 0; inx < set->indices().size(); inx++) {
						if (attribute[inx] > maxfl) {
							maxfl = attribute[inx];
							_inx = set->indices()[inx];
						}
					}

					output.push_back(_inx);
				}
			}
			else if (comparison == 11) {
				if (!set->indices().empty()) {
					size_t _inx = 0;
					float minfl = std::numeric_limits<float>::max();
					for (size_t inx = 0; inx < set->indices().size(); inx++) {
						if (attribute[inx] < minfl) {
							minfl = attribute[inx];
							_inx = set->indices()[inx];
						}
					}

					output.push_back(_inx);
				}
			}
			else if (12 <= comparison && comparison <= 15) {
				for (size_t inx = 0; inx < set->indices().size(); inx++) {
					if (comparisonFunc(comparison, attribute[inx], a, b)) output.push_back(set->indices()[inx]);
				}
			}	

			return output;
		}

		/// Float Node
		static inline void filterFloatAttributeMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues)
		{
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::STD_STRING) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0, 1], expected string";

					return;
				}

				if (args[1]->type() != fl::type::FLOAT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [1], expected float";

					return;
				}

				if (args[2]->type() != fl::type::UNSIGNED_INT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [1], unsigned int";

					return;
				}

				if (args[3]->type() != fl::type::FLOAT ||
					args[4]->type() != fl::type::FLOAT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0, 1], expected float";

					return;
				}

				if (args[0]->isArray() || args[2]->isArray() || args[3]->isArray() ||
					args[4]->isArray()) {
					error->hasError = true;
					error->errorMsg = "Inputs do not accept arrays.";

					return;
				}

				if (!args[1]->isArray()) {
					error->hasError = true;
					error->errorMsg = "Expected array, found scalar.";
					return;
				}

				auto set_in = args[0]->data<std::string>();
				auto values = args[1]->rawData<std::vector<float>>();
				auto comparison = args[2]->data<unsigned int>();
				auto a = args[3]->data<float>();
				auto b = args[4]->data<float>();

				auto softResultset = DfxFluentSetManager::getInstance()->createSoftSet(DfxFluentSet::EMPTY);
				std::string output = softResultset.first;

				if (!set_in.empty() && !values->empty()) {

					auto set = DfxFluentSetManager::getInstance()->getSet(set_in);

					if (set != nullptr) {

						if (set->indices().size() != values->size()) {
							error->hasError = true;
							error->errorMsg = "Set size, does not match value array size.";
							return;
						}

						if (set->type() == DfxFluentSet::SIMPLEX) {
							if (values->size() > 0) {						
					
								auto results = filter(comparison, set, &(*values)[0], a, b);
								if (!results.empty()) {
									softResultset.second->setType(DfxFluentSet::SIMPLEX);
									for (auto inx : results) softResultset.second->indices().push_back(inx);
								}													
							}
							else {
								error->hasError = true;
								error->errorMsg = "No simplicies found.";
								return;
							}
						}
						else if (set->type() == DfxFluentSet::STREAM) {
							if (values->size() > 0) {

								auto results = filter(comparison, set, &(*values)[0], a, b);
								if (!results.empty()) {
									softResultset.second->setType(DfxFluentSet::STREAM);
									for (auto inx : results) softResultset.second->indices().push_back(inx);
								}
							}
						}
					}
					else {
						error->hasError = true;
						error->errorMsg = "Set not found.";
						return;
					}
				}

				auto rany = new fl::Any();
				rany->newData<std::string>(fl::type::STD_STRING);
				rany->setData(output);

				returnValues.push_back(rany);

			}
		}

		static inline fl::Any *filterFloatAttributeNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("filter_float_attribute")
				.setLabel("Filter Float Attribute")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(filterFloatAttribute_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "set_out", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "set_in", "Set", std::string())
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "value_array", "Value Array", std::string(""))
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::HIDDEN_INPUT, "comparison", "Comparison", 0)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "value_a", "Value A", 0)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "value_b", "Value B", 0)

				.addStaticFunction(fl::DataObject::STATIC_MAIN, filterFloatAttributeMethod)
				.build(space);

			std::vector<std::pair<std::string, unsigned int>> comparisonTypeEnum;
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("Attribute < Value A",  0));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("Attribute > Value A",  1));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("Attribute <= Value A", 2));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("Attribute >= Value A", 3));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("Attribute = Value A",  4));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("Attribute != Value A", 5));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("(attribute % Value A)  = 0", 6));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("(attribute % Value A) != 0", 7));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("(Value A % Attribute)  = 0", 8));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("(Value A % Attribute) != 0", 9));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("Max(Attribute)", 10));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("Min(Attribute)", 11));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("Value A < Attribute < Value B", 12));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("Value A <= Attribute <= Value B", 13));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("Value A < Attribute <= Value B", 14));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("Value A <= Attribute < Value B", 15));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("Attribute = True", 16));
			comparisonTypeEnum.push_back(std::pair<std::string, unsigned int>("Attribute = False", 17));

			mnode->rawData<fl::DataObject>()->property("value_array")->newData<std::vector<float>>(fl::type::FLOAT);
			mnode->rawData<fl::DataObject>()->property("value_array")->setIsArray(true);

			mnode->rawData<fl::DataObject>()->property("comparison")->setEnumerations(true);
			mnode->rawData<fl::DataObject>()->property("comparison")->setEnumArray(comparisonTypeEnum);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return filterFloatAttributeNode(space);
		}
	};

	inline void registerFilterFloatAttributeNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = filterFloatAttribute_model_id;
		desc->name = "Filter Float Attribute";
		desc->description = "Filter set contents by float attribute and condition.";
		desc->category = "Math";
		desc->placement = "Math/Float/Filter Attribute";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = FilterFloatAttributeNodeHandler::filterFloatAttributeNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}

}
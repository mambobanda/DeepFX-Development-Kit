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
#include "DfxFluentSimplexCollection.h"
#include "DfxFluentStreamCollection.h"

namespace compute_node {

	static const std::string updateSetAttribute_model_id = "/COMPUTE/UPDATE_SET_ATTRIBUTE_NODE";

	class UpdateSetAttributeNodeHandler : public fl::Model {
	public:

		/// Float Node
		static inline void updateSetAttributeMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues)
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

				if (args[1]->type() != fl::type::STD_STRING) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [1]";

					return;
				}

				if (!args[3]->isArray()) {
					error->hasError = true;
					error->errorMsg = "Values only accepts arrays.";
					return;
				}

				if (args[0]->isArray() || args[1]->isArray() || args[2]->isArray()) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}

				auto setName = args[0]->data<std::string>();
				auto attrName = args[1]->data<std::string>();
				auto valAny   = args[2];
				auto valArrayAny = args[3];

				std::string output = "";

				if (!setName.empty()) {
					auto set = DfxFluentSetManager::getInstance()->getSet(setName);

					if (set != nullptr) {

						if (set->type() == DfxFluentSet::SIMPLEX) {

							if (DfxFluentSimplexCollection::getInstance()->immutableAttribute(attrName)) {
								error->hasError = true;
								error->errorMsg = "Cannot update immutable attribute (reserved system attribute).";
								return;
							}

							bool hasAttribute = DfxFluentSimplexCollection::getInstance()->atrributeMap().hasAttribute(attrName);

							if (valAny->type() == fl::type::FLOAT || valArrayAny->type() == fl::type::FLOAT) {

								if (!hasAttribute) {
									DfxFluentSimplexCollection::getInstance()->atrributeMap().addAttribute<float>(attrName);
								}
								else {
									if (DfxFluentSimplexCollection::getInstance()->atrributeMap().attributeType(attrName) != DfxFluenType::FLOAT) {
										error->hasError = true;
										error->errorMsg = "Input value type does not match 'existing' attribute type.";
										return;
									}
								}						
								
								if (valAny->type() == fl::type::FLOAT) {
									DfxFluentSimplexCollection::getInstance()->atrributeMap().fill<float>(attrName, valAny->data<float>());
								}
								else {
									auto varray = valArrayAny->rawData<std::vector<float>>();

									if (varray->size() == DfxFluentSimplexCollection::getInstance()->size()) {
										auto attrArr = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<float>(attrName);
										for (size_t inx = 0; inx < varray->size(); inx++) attrArr[inx] = (*varray)[inx];
									}
									else {
										error->hasError = true;
										error->errorMsg = "Number of values do no match Set size.";
										return;
									}
								}
							}
							else if (valAny->type() == fl::type::FLOAT_3 || valArrayAny->type() == fl::type::FLOAT_3) {

								if (!hasAttribute) {
									DfxFluentSimplexCollection::getInstance()->atrributeMap().addAttribute<glm::vec3>(attrName);
								}
								else {
									if (DfxFluentSimplexCollection::getInstance()->atrributeMap().attributeType(attrName) != DfxFluenType::VEC3) {
										error->hasError = true;
										error->errorMsg = "Input value type does not match 'existing' attribute type.";
										return;
									}
								}

								if (valAny->type() == fl::type::FLOAT_3) {
									DfxFluentSimplexCollection::getInstance()->atrributeMap().fill<glm::vec3>(attrName, valAny->data<glm::vec3>());
								}
								else {
									auto varray = valArrayAny->rawData<std::vector<glm::vec3>>();

									if (varray->size() == DfxFluentSimplexCollection::getInstance()->size()) {
										auto attrArr = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<glm::vec3>(attrName);
										for (size_t inx = 0; inx < varray->size(); inx++) attrArr[inx] = (*varray)[inx];
									}
									else {
										error->hasError = true;
										error->errorMsg = "Number of values do no match Set size.";
										return;
									}
								}
							}
							else if (valAny->type() == fl::type::SIZE_T || valArrayAny->type() == fl::type::SIZE_T) {

								if (!hasAttribute) {
									DfxFluentSimplexCollection::getInstance()->atrributeMap().addAttribute<size_t>(attrName);
								}
								else {
									if (DfxFluentSimplexCollection::getInstance()->atrributeMap().attributeType(attrName) != DfxFluenType::SIZE_T) {
										error->hasError = true;
										error->errorMsg = "Input value type does not match 'existing' attribute type.";
										return;
									}
								}

								if (valAny->type() == fl::type::SIZE_T) {
									DfxFluentSimplexCollection::getInstance()->atrributeMap().fill<size_t>(attrName, valAny->data<size_t>());
								}
								else {
									auto varray = valArrayAny->rawData<std::vector<size_t>>();

									if (varray->size() == DfxFluentSimplexCollection::getInstance()->size()) {
										auto attrArr = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<size_t>(attrName);
										for (size_t inx = 0; inx < varray->size(); inx++) attrArr[inx] = (*varray)[inx];
									}
									else {
										error->hasError = true;
										error->errorMsg = "Number of values do no match Set size.";
										return;
									}
								}
							}
							else if (valAny->type() == fl::type::STD_STRING || valArrayAny->type() == fl::type::STD_STRING) {

								if (!hasAttribute) {
									DfxFluentSimplexCollection::getInstance()->atrributeMap().addAttribute<std::string>(attrName);
								}
								else {
									if (DfxFluentSimplexCollection::getInstance()->atrributeMap().attributeType(attrName) != DfxFluenType::STRING) {
										error->hasError = true;
										error->errorMsg = "Input value type does not match 'existing' attribute type.";
										return;
									}
								}

								if (valAny->type() == fl::type::STD_STRING) {
									DfxFluentSimplexCollection::getInstance()->atrributeMap().fill<std::string>(attrName, valAny->data<std::string>());
								}
								else {
									auto varray = valArrayAny->rawData<std::vector<std::string>>();

									if (varray->size() == DfxFluentSimplexCollection::getInstance()->size()) {
										auto attrArr = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<std::string>(attrName);
										for (size_t inx = 0; inx < varray->size(); inx++) attrArr[inx] = (*varray)[inx];
									}
									else {
										error->hasError = true;
										error->errorMsg = "Number of values do no match Set size.";
										return;
									}
								}
							}
							else if (valAny->type() == fl::type::BOOL || valArrayAny->type() == fl::type::BOOL) {

								if (!hasAttribute) {
									DfxFluentSimplexCollection::getInstance()->atrributeMap().addAttribute<int>(attrName);
								}
								else {
									if (DfxFluentSimplexCollection::getInstance()->atrributeMap().attributeType(attrName) != DfxFluenType::BOOL) {
										error->hasError = true;
										error->errorMsg = "Input value type does not match 'existing' attribute type.";
										return;
									}
								}

								if (valAny->type() == fl::type::BOOL) {
									DfxFluentSimplexCollection::getInstance()->atrributeMap().fill<int>(attrName, valAny->data<bool>() ? 1 : 0);
								}
								else {
									auto varray = valArrayAny->rawData<std::vector<bool>>();

									if (varray->size() == DfxFluentSimplexCollection::getInstance()->size()) {
										auto attrArr = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<int>(attrName);
										for (size_t inx = 0; inx < varray->size(); inx++) attrArr[inx] = (*varray)[inx] ? 1 : 0;
									}
									else {
										error->hasError = true;
										error->errorMsg = "Number of values does not match Set size.";
										return;
									}
								}
							}
																			
						}
						else if (set->type() == DfxFluentSet::STREAM) {
							if (valAny->type() == fl::type::FLOAT) {
							}
							else if (valAny->type() == fl::type::FLOAT_3) {
							}
							else if (valAny->type() == fl::type::SIZE_T) {
							}
							else if (valAny->type() == fl::type::STD_STRING) {
							}
							else if (valAny->type() == fl::type::BOOL) {
							}
						}

						output = setName;
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

		static inline fl::Any *updateSetAttributeNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("update_set_attribute")
				.setLabel("Update Set Attribute")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(updateSetAttribute_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "out_set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "in_set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "attribute_name", "Attribute Name", std::string())
				.addProperty(space, fl::type::ANY, fl::role::INPUT, "fill", "Fill", 0)
				.addProperty(space, fl::type::ANY, fl::role::INPUT, "values", "Values", 0)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, updateSetAttributeMethod)
				.build(space);

			mnode->rawData<fl::DataObject>()->property("values")->setIsArray(true);

			mnode->rawData<fl::DataObject>()->property("fill")->setAllowAnyType(true);
			mnode->rawData<fl::DataObject>()->property("values")->setAllowAnyType(true);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return updateSetAttributeNode(space);
		}
	};

	inline void registerUpdateSetAttributeNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = updateSetAttribute_model_id;
		desc->name = "Create/Update Set Attribute";
		desc->description = "Create/Update Existing Set Attribute.";
		desc->category = "Sets";
		desc->placement = "Sets/Update Set Attribute";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = UpdateSetAttributeNodeHandler::updateSetAttributeNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}

}
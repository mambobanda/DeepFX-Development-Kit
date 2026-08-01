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

	static const std::string readAttribute_model_id = "/COMPUTE/READ_ATTRIBUTE_NODE";

	class ReadAttributeNodeHandler : public fl::Model {
	public:

		static inline void readAttributeMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::STD_STRING ||
					args[1]->type() != fl::type::STD_STRING) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0], expected string";
					return;
				}

				if (args[0]->isArray() || args[1]->isArray()) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}

				auto setName  = args[0]->data<std::string>();
				auto attrName = args[1]->data<std::string>();

				if (!setName.empty() && !attrName.empty()) {
					auto set = DfxFluentSetManager::getInstance()->getSet(setName);

					if (set != nullptr) {				
						if(set->indices().size() > 0) { 
							
							DfxFluentChannelMap *_attributeMap = nullptr;
							bool hasLength = false;
							if (set->type() == DfxFluentSet::SIMPLEX) {
								_attributeMap = &DfxFluentSimplexCollection::getInstance()->atrributeMap();
								if (DfxFluentSimplexCollection::getInstance()->size() > 0) hasLength = true;
								else {
									error->hasError = true;
									error->errorMsg = "No Simplicies found.";
									return;
								}
							}
							else if (set->type() == DfxFluentSet::STREAM) {
								_attributeMap = &DfxFluentStreamCollection::getInstance()->atrributeMap();
								if (DfxFluentStreamCollection::getInstance()->size() > 0) hasLength = true;
								else {
									error->hasError = true;
									error->errorMsg = "No Streams found.";
									return;
								}
							}

							if (_attributeMap->hasAttribute(attrName)) {
								auto typ = _attributeMap->attributeType(attrName);

								auto rany01 = new fl::Any();

								if (typ == DfxFluenType::FLOAT) {

									std::vector<float> arr;
									auto attrArray = _attributeMap->at<float>(attrName);
									
									for (auto inx: set->indices()) {
										arr.push_back(attrArray[inx]);
									}

									rany01->newData<std::vector<float>>(fl::type::FLOAT, new std::vector<float>(arr));
									rany01->setIsArray(true);
								}
								else if (typ == DfxFluenType::VEC3) {
									std::vector<glm::vec3> arr;
									auto attrArray = _attributeMap->at<glm::vec3>(attrName);

									for (auto inx : set->indices()) {
										arr.push_back(attrArray[inx]);
									}

									rany01->newData<std::vector<glm::vec3>>(fl::type::FLOAT_3, new std::vector<glm::vec3>(arr));
									rany01->setIsArray(true);
								}
								else if (typ == DfxFluenType::SIZE_T) {
									std::vector<size_t> arr;
									auto attrArray = _attributeMap->at<size_t>(attrName);

									for (auto inx : set->indices()) {
										arr.push_back(attrArray[inx]);
									}

									rany01->newData<std::vector<size_t>>(fl::type::SIZE_T, new std::vector<size_t>(arr));
									rany01->setIsArray(true);
								}
								else if (typ == DfxFluenType::STRING) {
									std::vector<std::string> arr;
									auto attrArray = _attributeMap->at<std::string>(attrName);

									for (auto inx : set->indices()) {
										arr.push_back(attrArray[inx]);
									}

									rany01->newData<std::vector<std::string>>(fl::type::STD_STRING, new std::vector<std::string>(arr));
									rany01->setIsArray(true);
								}
								else if (typ == DfxFluenType::BOOL) {
									std::vector<bool> arr;
									auto attrArray = _attributeMap->at<int>(attrName);

									for (auto inx : set->indices()) {
										arr.push_back(attrArray[inx]);
									}

									rany01->newData<std::vector<bool>>(fl::type::BOOL, new std::vector<bool>(arr));
									rany01->setIsArray(true);
								}							
								
								returnValues.push_back(rany01);
								return;
							}
							else {
								error->hasError = true;
								error->errorMsg = "Attribute Doesn't Exist.";
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

			}

		}

		static inline fl::Any *readAttributeNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("read_attribute")
				.setLabel("Read Attribute")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(readAttribute_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::ANY, fl::role::OUTPUT, "data", "Data", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "attribute", "Attribute", std::string())
				.addStaticFunction(fl::DataObject::STATIC_MAIN, readAttributeMethod)
				.build(space);

			mnode->rawData<fl::DataObject>()->property("attribute")->setMiscFlag(-300);
			mnode->rawData<fl::DataObject>()->property("data")->setIsArray(true);
			mnode->rawData<fl::DataObject>()->property("data")->setAllowAnyType(true);


			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return readAttributeNode(space);
		}
	};

	inline void registerReadAttributeNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = readAttribute_model_id;
		desc->name = "Read Attribute";
		desc->description = "Read Attribute.";
		desc->category = "Sets";
		desc->placement = "Sets/Read Attribute";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = ReadAttributeNodeHandler::readAttributeNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}

}
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
#include <string>
#include "DfxBulletStageNode.h"
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"
#include "DfxFluentSetManager.h"
#include "DfxFluentBigID.h"
#include "DfxFluentSetManager.h"
#include "DfxFluentSimplexCollection.h"

namespace compute_node {

	static const std::string bulletAddForce_model_id = "/COMPUTE/BULLET_ADD_FORCE";

	class BulletAddForceNodeHandler : public fl::Model {
	public:

		/// Float Node
		static inline void bulletAddForceMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues)
		{
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::STD_STRING ||
					args[1]->type() != fl::type::STD_STRING ||
					args[2]->type() != fl::type::STD_STRING ) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0, 1, 2], expected string.";

					return;
				}

				if (args[3]->type() != fl::type::FLOAT_3 ||
					args[4]->type() != fl::type::FLOAT_3 ) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [3, 4], expected float";

					return;
				}

				if (args[5]->type() != fl::type::BOOL ||
					args[6]->type() != fl::type::BOOL ) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [5, 6], expected bool";

					return;
				}

				if (args[0]->isArray() || args[1]->isArray() || args[2]->isArray() || args[3]->isArray() || args[4]->isArray() ||
					args[5]->isArray() || args[6]->isArray()) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}

				std::string outputSet = "";

				auto inset = args[0]->data<std::string>();
				auto stage = args[1]->data<std::string>();
				auto alias = args[2]->data<std::string>();
				auto force = args[3]->data<glm::vec3>();
				auto angularForce = args[4]->data<glm::vec3>();
				auto inc_force_attr = args[5]->data<bool>();
				auto inc_angular_force_attr = args[6]->data<bool>();

				if (!inset.empty() && !stage.empty()) {
					auto set = DfxFluentSetManager::getInstance()->getSet(inset);
					auto stageAny = space->findAbsolute(stage);

					if (set != nullptr && stageAny != nullptr) {
						outputSet = inset;

						fl::DataObject *doa = nullptr;
						DfxBulletStageNode *stageNode = nullptr;

						if (stageAny->type() != fl::type::DATA_OBJECT) {
							error->hasError = true;
							error->errorMsg = "Unknown Stage Type.";
							return;
						}

						dao = stageAny->rawData<fl::DataObject>();
						fl::Any *stageTypeAny = dao->property("stage_type");;
						fl::Any *stageNodeAny = dao->property("sg_stage_node");

						if (stageTypeAny == nullptr) {
							error->hasError = true;
							error->errorMsg = "Unknown Stage Type.";
							return;
						}
						else if (stageNodeAny == nullptr) {
							error->hasError = true;
							error->errorMsg = "Undefined Stage Node.";
						}
						else if (stageTypeAny->type() != fl::type::UNSIGNED_INT) {
							error->hasError = true;
							error->errorMsg = "Unknown Stage Type.";
							return;
						}
						else if (stageTypeAny->data<unsigned int>() != DfxStageType::BULLET_STAGE) {
							error->hasError = true;
							error->errorMsg = "Wrong Stage Type.";
							return;
						}

						stageNode = stageNodeAny->rawData<DfxBulletStageNode>();

						glm::vec3 *force_arr = nullptr;
						glm::vec3 *angular_force_arr = nullptr;

						if (set->type() == DfxFluentSet::SIMPLEX) {
							if (DfxFluentSimplexCollection::getInstance()->size() > 0) {

								std::string *alias_arr = nullptr;
								size_t *uid_arr = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<size_t>("@uid");

								if (!alias.empty()) {
									auto ref = compute::getAttribute<std::string>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@alias", DfxFluenType::STRING);

									if (ref.sameAsRequestedType && ref.exists && ref._data != nullptr) {
										alias_arr = compute::extractAttrRef<std::string>(ref);
									}
								}

								for (auto &index : set->indices()) {

									if (alias_arr != nullptr) {
										auto _alias = alias_arr[index];
										if (_alias != alias) continue;
									}

									auto flAgent = stageNode->findFluentSimplexAgent(index);

									if (flAgent != nullptr) {

										// Force
										if (inc_force_attr) {
											if (force_arr == nullptr) {
												auto ref = compute::getAttribute<glm::vec3>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@force", DfxFluenType::VEC3, true);
												if (!ref.sameAsRequestedType) {
													error->hasError = true;
													error->errorMsg = "Existing attribute '@force' found with different type. Expected vector.";
													return;
												}
												else {
													force_arr = compute::extractAttrRef<glm::vec3>(ref);
												}
											}

											force_arr[index] = force;
										}

										// Angular Force
										if (inc_angular_force_attr) {
											if (angular_force_arr == nullptr) {
												auto ref = compute::getAttribute<glm::vec3>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@angular_force", DfxFluenType::VEC3, true);
												if (!ref.sameAsRequestedType) {
													error->hasError = true;
													error->errorMsg = "Existing attribute '@angular_force' found with different type. Expected vector.";
													return;
												}
												else {
													angular_force_arr = compute::extractAttrRef<glm::vec3>(ref);
												}
											}

											angular_force_arr[index] = angularForce;
										}
									}
								}
							}
						}
					}
					else {
						error->hasError = true;
						error->errorMsg = "Set or Stage doesn't exist.";
						return;
					}
				}


				auto rany = new fl::Any();
				rany->newData<std::string>(fl::type::STD_STRING);
				rany->setData(outputSet);

				returnValues.push_back(rany);

			}
		}

		static inline fl::Any *bulletAddForceNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("bullet_add_force")
				.setLabel("BT Add Force")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(bulletAddForce_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "out_set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "in_set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "stage", "Stage", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "alias", "Alias", std::string())
				.addProperty(space, fl::type::FLOAT_3, fl::role::INPUT, "force", "Force", glm::vec3())
				.addProperty(space, fl::type::FLOAT_3, fl::role::INPUT, "angular_force", "Angular Force", glm::vec3())
				.addProperty(space, fl::type::BOOL, fl::role::HIDDEN_INPUT, "in_force_attr", "<div style='color:green;'>@force</div> Attribute", true)
				.addProperty(space, fl::type::BOOL, fl::role::HIDDEN_INPUT, "in_angular_force_attr", "<div style='color:green;'>@angular_force</div> Attribute", false)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, bulletAddForceMethod)
				.build(space);

			mnode->rawData<fl::DataObject>()->property("stage")->setMiscFlag(-100);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return bulletAddForceNode(space);
		}
	};

	inline void registerBulletAddForceNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = bulletAddForce_model_id;
		desc->name = "Bullet Add Force";
		desc->description = "Add a force for a single frame to a bullet object";
		desc->category = "Bullet";
		desc->placement = "Bullet/Add Force";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = BulletAddForceNodeHandler::bulletAddForceNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}

}
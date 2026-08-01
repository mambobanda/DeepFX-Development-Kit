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
#include "DfxComputeUtility.h"
#include "DfxBulletStageNode.h"
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"
#include "DfxFluentSetManager.h"
#include "DfxFluentBigID.h"
#include "DfxFluentSetManager.h"
#include "DfxFluentSimplexCollection.h"

namespace compute_node {

	static const std::string bulletRigidBodyAttributes_model_id = "/COMPUTE/BULLET_RIGID_BODY_ATTRIBUTES";

	class BulletRigidBodyAttributesNodeHandler : public fl::Model {
	public:

		/// Float Node
		static inline void bulletRigidBodyAttributesMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues)
		{
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::STD_STRING ||
					args[1]->type() != fl::type::STD_STRING ||
					args[2]->type() != fl::type::STD_STRING ){
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0, 1], expected string.";
					return;
				}

				if (args[3]->type() != fl::type::FLOAT ||
					args[4]->type() != fl::type::FLOAT ||
					args[5]->type() != fl::type::FLOAT ||
					args[6]->type() != fl::type::FLOAT ||
					args[7]->type() != fl::type::FLOAT ) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [1, 2, 3, 4, 5]";
					return;
				}


				if (args[8]->type()  != fl::type::BOOL ||
					args[9]->type() != fl::type::BOOL ||
					args[10]->type() != fl::type::BOOL ||
					args[11]->type() != fl::type::BOOL ||
					args[12]->type() != fl::type::BOOL ) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [9, 10, 11, 12, 13, 14, 15]";
					return;
				}

				if (args[0]->isArray() || args[1]->isArray() || args[2]->isArray() || args[3]->isArray() || args[4]->isArray() ||
					args[5]->isArray() || args[6]->isArray() || args[7]->isArray() || args[8]->isArray() || args[9]->isArray() ||
					args[10]->isArray() || args[11]->isArray() || args[12]->isArray()) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}

				std::string outputSet = "";

				auto inset = args[0]->data<std::string>();
				auto stage = args[1]->data<std::string>();
				auto alias = args[2]->data<std::string>();
				auto mass  = args[3]->data<float>();
				auto friction = args[4]->data<float>();
				auto restitution = args[5]->data<float>();
				auto l_dampening = args[6]->data<float>();
				auto a_dampening = args[7]->data<float>();
				auto inc_mass_attr = args[8]->data<bool>();
				auto inc_friction_attr = args[9]->data<bool>();
				auto inc_restitution_attr  = args[10]->data<bool>();
				auto inc_linear_damp_attr  = args[11]->data<bool>();
				auto inc_angular_damp_attr = args[12]->data<bool>();

				if (!inset.empty() && !stage.empty()) {
					auto set = DfxFluentSetManager::getInstance()->getSet(inset);
					auto stageAny = space->findAbsolute(stage);

					outputSet = inset;
					if (set != nullptr && stageAny != nullptr) {
					
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

						float *mass_arr = nullptr;
						float *friction_arr = nullptr;
						float *restitution_arr = nullptr;
						float *l_dampening_arr = nullptr;
						float *a_dampening_arr = nullptr;
						glm::vec3 *velocity_arr = nullptr;
						glm::vec3 *angular_velocity_arr = nullptr;

						if (set->type() == DfxFluentSet::SIMPLEX) {
							if (DfxFluentSimplexCollection::getInstance()->size() > 0) {

								std::string *alias_arr = nullptr;

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

										// Mass
										if (inc_mass_attr) {
											if (mass_arr == nullptr) {
												auto ref = compute::getAttribute<float>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@mass", DfxFluenType::FLOAT, true);
												if (!ref.sameAsRequestedType) {
													error->hasError = true;
													error->errorMsg = "Existing attribute '@mass' found with different type. Expected Float.";
													return;
												}
												else {
													mass_arr = compute::extractAttrRef<float>(ref);
												}
											}

											mass_arr[index] = mass;
										}

										// Friction
										if (inc_friction_attr) {
											if (friction_arr == nullptr) {
												auto ref = compute::getAttribute<float>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@friction", DfxFluenType::FLOAT, true);
												if (!ref.sameAsRequestedType) {
													error->hasError = true;
													error->errorMsg = "Existing attribute '@friction' found with different type. Expected Float.";
													return;
												}
												else {
													friction_arr = compute::extractAttrRef<float>(ref);
												}
											}

											friction_arr[index] = friction;
										}

										// Restitution
										if (inc_restitution_attr) {
											if (restitution_arr == nullptr) {
												auto ref = compute::getAttribute<float>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@restitution", DfxFluenType::FLOAT, true);
												if (!ref.sameAsRequestedType) {
													error->hasError = true;
													error->errorMsg = "Existing attribute '@restitution' found with different type. Expected Float.";
													return;
												}
												else {
													restitution_arr = compute::extractAttrRef<float>(ref);
												}
											}

											restitution_arr[index] = restitution;
										}

										// Linear Dampening
										if (inc_linear_damp_attr) {
											if (l_dampening_arr == nullptr) {
												auto ref = compute::getAttribute<float>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@l_dampening", DfxFluenType::FLOAT, true);
												if (!ref.sameAsRequestedType) {
													error->hasError = true;
													error->errorMsg = "Existing attribute '@l_dampening' found with different type. Expected Float.";
													return;
												}
												else {
													l_dampening_arr = compute::extractAttrRef<float>(ref);
												}
											}

											l_dampening_arr[index] = l_dampening;
										}

										// Angular Dampening
										if (inc_angular_damp_attr) {
											if (a_dampening_arr == nullptr) {
												auto ref = compute::getAttribute<float>(&DfxFluentSimplexCollection::getInstance()->atrributeMap(), "@a_dampening", DfxFluenType::FLOAT, true);
												if (!ref.sameAsRequestedType) {
													error->hasError = true;
													error->errorMsg = "Existing attribute '@a_dampening' found with different type. Expected Float.";
													return;
												}
												else {
													a_dampening_arr = compute::extractAttrRef<float>(ref);
												}
											}

											a_dampening_arr[index] = a_dampening;
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

		static inline fl::Any *bulletRigidBodyAttributesNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("bt_rigid_body_attributes")
				.setLabel("BT Rigid Body Attributes")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(bulletRigidBodyAttributes_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "out_set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "in_set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "stage", "Stage", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "alias", "Alias", std::string())
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "mass", "Mass", 10.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "friction", "Friction", 0.5f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "restitution", "Restitution", 0.5f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "linear_dampening", "Linear Dampening", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "angular_dampening", "Angular Dampening", 0.f)
				.addProperty(space, fl::type::BOOL, fl::role::HIDDEN_INPUT, "in_mass_attr", "<div style='color:green;'>@mass</div> Attribute", true)
				.addProperty(space, fl::type::BOOL, fl::role::HIDDEN_INPUT, "in_friction_attr", "<div style='color:green;'>@friction</div> Attribute", true)
				.addProperty(space, fl::type::BOOL, fl::role::HIDDEN_INPUT, "in_restitution_attr", "<div style='color:green;'>@restitution</div> Attribute", true)
				.addProperty(space, fl::type::BOOL, fl::role::HIDDEN_INPUT, "in_linear_dampening_attr", "<div style='color:green;'>@l_dampening</div> Attribute", false)
				.addProperty(space, fl::type::BOOL, fl::role::HIDDEN_INPUT, "in_angular_dampening_attr", "<div style='color:green;'>@a_dampening</div> Attribute", false)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, bulletRigidBodyAttributesMethod)
				.build(space);

			mnode->rawData<fl::DataObject>()->property("stage")->setMiscFlag(-100);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return bulletRigidBodyAttributesNode(space);
		}
	};

	inline void registerBulletRigidBodyAttributesNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = bulletRigidBodyAttributes_model_id;
		desc->name = "Bullet Rigid Body Attributes";
		desc->description = "Update bullet rigid body attributes";
		desc->category = "Bullet";
		desc->placement = "Bullet/Rigid Body Attributes";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = BulletRigidBodyAttributesNodeHandler::bulletRigidBodyAttributesNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}

}
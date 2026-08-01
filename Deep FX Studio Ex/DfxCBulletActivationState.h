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
#include "DfxBulletStageNode.h"
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"
#include "DfxFluentSetManager.h"
#include "DfxFluentBigID.h"
#include "DfxFluentSetManager.h"
#include "DfxFluentSimplexCollection.h"

namespace compute_node {

	static const std::string bulletActivationState_model_id = "/COMPUTE/BULLET_ACTIVATION_STATE";

	class BulletActivationStateNodeHandler : public fl::Model {
	public:

		/// Float Node
		static inline void bulletActivationStateMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues)
		{
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::STD_STRING ||
					args[1]->type() != fl::type::STD_STRING ||
					args[2]->type() != fl::type::STD_STRING) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0, 1, 2], expected string.";
					return;
				}

				if (args[3]->type() != fl::type::UNSIGNED_INT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [2], expected unsigned int.";
					return;
				}

				if (args[0]->isArray() || args[1]->isArray() || args[2]->isArray() || args[3]->isArray()) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}

				std::string outputSet = "";

				auto inset = args[0]->data<std::string>();
				auto stage = args[1]->data<std::string>();
				auto alias = args[2]->data<std::string>();
				auto state = args[3]->data<unsigned int>();

				if (!inset.empty() && !stage.empty()) {
					auto set = DfxFluentSetManager::getInstance()->getSet(inset);
					auto stageAny = space->findAbsolute(stage);

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

						auto iState = 1;

						if (state == 0) iState = 1;
						else if (state == 1) iState = 2;
						else if (state == 2) iState = 3;
						else if (state == 3) iState = 4;
						else if (state == 4) iState = 5;

						if (set->type() == DfxFluentSet::SIMPLEX) {
							stageNode->setFluentSimplexAgentActivationState(set->indices(), iState, alias);
						}
						else if (set->type() == DfxFluentSet::STREAM) {
							stageNode->setFluentStreamAgentActivationState(set->indices(), iState, alias);
						}

						outputSet = inset;
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

		static inline fl::Any *bulletActivationStateNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("bullet_activation_state")
				.setLabel("BT Activation State")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(bulletActivationState_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "out_set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "in_set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "stage", "Stage", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "alias", "Alias", std::string())
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::INPUT, "state", "State", 1)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, bulletActivationStateMethod)
				.build(space);

			mnode->rawData<fl::DataObject>()->property("stage")->setMiscFlag(-100);

			std::vector<std::pair<std::string, unsigned int>> stateEnum;
			stateEnum.push_back(std::pair<std::string, unsigned int>("Activation State", 0));
			stateEnum.push_back(std::pair<std::string, unsigned int>("Island Sleeping",  1));
			stateEnum.push_back(std::pair<std::string, unsigned int>("Wants Deactivation", 2));
			stateEnum.push_back(std::pair<std::string, unsigned int>("Disable Deactivation", 3));
			stateEnum.push_back(std::pair<std::string, unsigned int>("Disable Simulation", 4));

			mnode->rawData<fl::DataObject>()->property("state")->setEnumerations(true);
			mnode->rawData<fl::DataObject>()->property("state")->setEnumArray(stateEnum);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return bulletActivationStateNode(space);
		}
	};

	inline void registerBulletActivationStateNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = bulletActivationState_model_id;
		desc->name = "Bullet Activation State";
		desc->description = "Update Bullet Activation State";
		desc->category = "Bullet";
		desc->placement = "Bullet/Activation State";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = BulletActivationStateNodeHandler::bulletActivationStateNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}

}
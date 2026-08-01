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

	static const std::string bulletKill_model_id = "/COMPUTE/BULLET_KILL";

	class BulletKillNodeHandler : public fl::Model {
	public:

		/// Float Node
		static inline void bulletKillMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues)
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
					error->errorMsg = "Unknown Input Type [0, 1, 2], expected string.";
					return;
				}

				if (args[0]->isArray() || args[1]->isArray()) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}

				std::string outputSet = "";

				auto inset = args[0]->data<std::string>();
				auto stage = args[1]->data<std::string>();
			
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

						for (size_t inx = 0; inx < set->indices().size(); inx++) {
							if (set->type() == DfxFluentSet::SIMPLEX) {
								stageNode->removeFluentSimplex(set->indices()[inx]);
							}
							else if (set->type() == DfxFluentSet::STREAM) {
								stageNode->removeFluentStream(set->indices()[inx]);
							}
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

		static inline fl::Any *bulletKillNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("bullet_kill")
				.setLabel("BT Kill")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(bulletKill_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "out_set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "in_set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "stage", "Stage", std::string())
				.addStaticFunction(fl::DataObject::STATIC_MAIN, bulletKillMethod)
				.build(space);

			mnode->rawData<fl::DataObject>()->property("stage")->setMiscFlag(-100);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return bulletKillNode(space);
		}
	};

	inline void registerBulletKillNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = bulletKill_model_id;
		desc->name = "Bullet Kill";
		desc->description = "Kill / Remove object from bullet simulation";
		desc->category = "Bullet";
		desc->placement = "Bullet/Kill";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = BulletKillNodeHandler::bulletKillNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}

}
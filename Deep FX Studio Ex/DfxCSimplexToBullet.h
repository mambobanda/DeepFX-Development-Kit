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
#include "DfxUtil.h"
#include "DfxFluentSetManager.h"
#include "DfxFluentSimplexCollection.h"

namespace compute_node {

	static const std::string simplexToBullet_model_id = "/COMPUTE/SIMPLEX_TO_BULLET";

	class SimplexToBulletNodeHandler : public fl::Model {
	public:

		/// Float Node
		static inline void simplexToBulletMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			if (!args.empty()) {

				if (args[0]->type() != fl::type::STD_STRING ||
					args[1]->type() != fl::type::STD_STRING || 
					args[2]->type() != fl::type::STD_STRING ||
					args[5]->type() != fl::type::STD_STRING  ) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [0, 1, 2, 5], expected string.";

					return;
				}

				if (args[3]->type() != fl::type::UNSIGNED_INT ||
					args[4]->type() != fl::type::UNSIGNED_INT) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [3, 4], expected uint.";

					return;
				}

				if (args[0]->isArray() || args[1]->isArray() || args[2]->isArray() || args[3]->isArray() || args[4]->isArray() ||
					args[5]->isArray() ) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}

				std::string outputSet = "";

				auto inset = args[0]->data<std::string>();
				auto stage = args[1]->data<std::string>();
				auto alias = args[2]->data<std::string>();
				auto type  = args[3]->data<unsigned int>();
				auto collisionShape = args[4]->data<unsigned int>();
				auto compositeShapeSet = args[5]->data<std::string>();
	
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
						else if (stageTypeAny->type()!= fl::type::UNSIGNED_INT) {
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

						if (set->type() == DfxFluentSet::SIMPLEX) {
							for (auto &index : set->indices()) {
								stageNode->addFluentSimplex(index, type, collisionShape);
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

		static inline fl::Any *simplexToBulletNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("simplex_to_bullet")
				.setLabel("Simplex To Bullet")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(simplexToBullet_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "out_set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "in_set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "stage", "Stage", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "alias", "Alias", std::string())
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::INPUT, "type", "Type", 0)
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::INPUT, "collision_shape", "Collision Shape", 0)
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "composite_shape_set", "Composite Shape Set", std::string())
				.addStaticFunction(fl::DataObject::STATIC_MAIN, simplexToBulletMethod)
				.build(space);

			mnode->rawData<fl::DataObject>()->property("stage")->setMiscFlag(-100);

			std::vector<std::pair<std::string, unsigned int>> typesEnum;
			typesEnum.push_back(std::pair<std::string, unsigned int>("Rigid Body", 0));
			typesEnum.push_back(std::pair<std::string, unsigned int>("Static Body", 1));
			typesEnum.push_back(std::pair<std::string, unsigned int>("Soft Body", 2));
			typesEnum.push_back(std::pair<std::string, unsigned int>("Kinematic Body", 3));

			std::vector<std::pair<std::string, unsigned int>> collisionShapeEnum;
			collisionShapeEnum.push_back(std::pair<std::string, unsigned int>("Convex", 0));
			collisionShapeEnum.push_back(std::pair<std::string, unsigned int>("Triangle", 1));
			
			mnode->rawData<fl::DataObject>()->property("type")->setEnumerations(true);
			mnode->rawData<fl::DataObject>()->property("type")->setEnumArray(typesEnum);

			mnode->rawData<fl::DataObject>()->property("collision_shape")->setEnumerations(true);
			mnode->rawData<fl::DataObject>()->property("collision_shape")->setEnumArray(collisionShapeEnum);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return simplexToBulletNode(space);
		}
	};

	inline void registerSimplexToBulletNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = simplexToBullet_model_id;
		desc->name = "Simplex To Bullet";
		desc->description = "Add Simplex Set data to Bullet simulation stage";
		desc->category = "Bullet";
		desc->placement = "Bullet/Simplex To Bullet";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = SimplexToBulletNodeHandler::simplexToBulletNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}

}
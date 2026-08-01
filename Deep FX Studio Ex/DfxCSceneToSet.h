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
#include <SgPointCloudNode.h>
#include "DfxFluentTracker.h"
#include "DfxComputeNodeDB.h"
#include "DfxNullHandler.h"
#include "DfxComputeNodeDescription.h"
#include "DfxFluentSimplexCollection.h"
#include "DfxFluentSimplexObject.h"
#include "DfxFluentSetManager.h"
#include "DfxGroupNode.h"
#include "DfxFluentUtil.h"


namespace compute_node {

	static const std::string sceneToSet_model_id = "/COMPUTE/SCENE_TO_SET_NODE";

	class SceneToSetNodeHandler : public fl::Model {
	public:

		/// Float Node
		static inline void sceneToSetMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues)
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

				if (args[3]->type() != fl::type::BOOL) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [3]";

					return;
				}

				if (args[4]->type() != fl::type::BOOL) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type [4]";

					return;
				}

				if (args[0]->isArray() || args[1]->isArray() || args[3]->isArray() || args[4]->isArray()) {
					error->hasError = true;
					error->errorMsg = "None of the inputs accept arrays.";
					return;
				}

				auto spath = args[0]->data<std::string>();
				auto tset  = args[1]->data<std::string>();
				auto type  = args[2]->data<unsigned int>();
				auto nameToAlias = args[3]->data<bool>();
				auto track = args[4]->data<bool>();

				std::string anyName = "##";

				DfxFluentSet *set = nullptr;

				auto rany = new fl::Any();
				rany->newData<std::string>(fl::type::STD_STRING);
				rany->setData(tset);
				returnValues.push_back(rany);

				if (tset.empty()) {
					return;
				}

				if (!DfxFluentSetManager::getInstance()->hasSet(tset)) {
					error->hasError = true;
					error->errorMsg = "Set doesn't exist.";
					return;
				}
				else {
					set = DfxFluentSetManager::getInstance()->getSet(tset);

					if (type == 0) {
						if (set->type() != DfxFluentSet::SIMPLEX) {
							error->hasError = true;
							error->errorMsg = "Set wrong type. Expected simplex/geometry set.";
							return;
						}
					}
					else if (type == 1) {
						if (set->type() != DfxFluentSet::STREAM) {
							error->hasError = true;
							error->errorMsg = "Set wrong type. Expected point set.";
							return;
						}
					}

					if (set->isSoftSet()) {
						error->hasError = true;
						error->errorMsg = "Cannot move to a temporary set (soft set).";
						return;
					}
				}

				deepsg::SgNode *snode = nullptr;

				if (!spath.empty()) {
					auto any = DfxSpace::getInstance()->findAbsolute(spath);

					if (any != nullptr) {

						if (any->type() == fl::type::DATA_OBJECT) {
							auto dao = any->rawData<fl::DataObject>();

							if (dao->hasProperty("sg_geo_node")) {
								snode = any->data<fl::DataObject>().property("sg_geo_node")->rawData<deepsg::SgGeoNode>();
							}
							else if (dao->hasProperty("sg_point_cloud_node")) {
								snode = any->data<fl::DataObject>().property("sg_point_cloud_node")->rawData<deepsg::SgPointCloudNode>();
							}
							else if (dao->hasProperty("sg_shape_node")) {
								snode = any->data<fl::DataObject>().property("sg_shape_node")->rawData<deepsg::SgShapeNode>();
							}
							else if (dao->hasProperty("sg_group_node")) {
								snode = any->data<fl::DataObject>().property("sg_group_node")->rawData<DfxGroupNode>();
							}
							else if (dao->hasProperty("sg_null_node")) {
								snode = any->data<fl::DataObject>().property("sg_null_node")->rawData<DfxNullNode>();
							}
							else {
								error->hasError = true;
								error->errorMsg = "No Suitable Node found";
								return;
							}
						}

					}
					else {
						error->hasError = true;
						error->errorMsg = "Incorrect Path";
						return;
					}

					if (snode != nullptr) {
						std::vector<deepsg::SgGeoAdapterInterface*> adapters;
						deepsg::SgTraversalFunction transverseFunc = [&nameToAlias, space, &type, set, &tset, any, &track](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
							if (type == 0) {

								if (node->getType() == deepsg::SgNodeType::GEOMETRY) {

									if (!set->hasObjectFromScene(node->referencePath())) {

										auto adapter = ((deepsg::SgGeoNode*)node)->getGeometryAdapter();

										if (adapter->getPolygonCount() > 0) {
											
											bool success = false;
											auto index = util::geoNodeToFluentSet((deepsg::SgGeoNode*)node, set, tset, node->referencePath(), info->transformMatrix(), success);
											//auto index = util::objectToFluentSet(adapter, set, tset, node->referencePath(), info->transformMatrix(), success);

											if (track) {
												DfxFluentTracker::getInstance()
													->trackNode(
														node->referencePath(), 
														std::vector<std::pair<size_t, size_t>> {std::pair<size_t, size_t>(index, DfxFluentSet::SIMPLEX)});
											}

											if (nameToAlias) {
												auto tempAny = space->findAbsolute(node->referencePath());
												if (tempAny != nullptr) {
													if (!tempAny->label().empty()) {
														DfxFluentSimplexCollection::getInstance()->atrributeMap().at<std::string>("@alias")[index] = tempAny->label();
													}
													else {
														DfxFluentSimplexCollection::getInstance()->atrributeMap().at<std::string>("@alias")[index] = "##";
													}
												}
												else {
													DfxFluentSimplexCollection::getInstance()->atrributeMap().at<std::string>("@alias")[index] = "##";
												}
											}
										}
									}
								}
							}
							else if (type == 1) {
								if (node->getType() == deepsg::SgNodeType::POINT_CLOUD) {

									auto pcloudNode = (deepsg::SgPointCloudNode*)node;

									if (pcloudNode->geo()->size() > 0) {

										bool success = false;
										auto indexList = util::pointCloudToFluentSet(pcloudNode, set, tset, node->referencePath(), info->transformMatrix(), success);
									
										if (track) {
											std::vector<std::pair<size_t, size_t >> trackList;

											for (auto i : indexList) trackList.push_back(std::pair<size_t, size_t>(i, DfxFluentSet::STREAM));

											DfxFluentTracker::getInstance()->trackNode(node->referencePath(), trackList);
										}
									}
									
								}
								else {
	
									if (node->xFormProperties() != nullptr) {

										size_t streamOffset = DfxFluentStreamCollection::getInstance()->growCollection(1);
										auto positionStream = DfxFluentStreamCollection::getInstance()->atrributeMap().at<glm::vec3>("@position");

										positionStream[streamOffset] = node->position();
										set->addIndices(std::vector<size_t> { streamOffset });

										if (track) {
											DfxFluentTracker::getInstance()
												->trackNode(
													node->referencePath(),
													std::vector<std::pair<size_t, size_t>> {std::pair<size_t, size_t>(streamOffset, DfxFluentSet::STREAM)});
										}
									}
								}
							}
						};

						deepsg::SgRootNode::getInstance()->traverseSubScene(snode, transverseFunc);
					}
					else {
						error->hasError = true;
						error->errorMsg = "Scene node is [nullptr]";
						return;
					}
				}
				
			}
		}

		static inline fl::Any *sceneToSetNode(fl::Space *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("sceneToSet")
				.setLabel("Scene To Set")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(sceneToSet_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "set", "Set", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "scene_node", "Path", std::string())
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "set", "Set Name", std::string())
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::INPUT, "type", "Set Type", 0)
				.addProperty(space, fl::type::BOOL, fl::role::INPUT, "use_name_as_alias", "Use Name As Alias", false)
				.addProperty(space, fl::type::BOOL, fl::role::INPUT, "track_xform", "Track Xform", false)
				.addStaticFunction(fl::DataObject::STATIC_MAIN, sceneToSetMethod)
				.build(space);

			mnode->rawData<fl::DataObject>()->property("scene_node")->setMiscFlag(-100);

			std::vector<std::pair<std::string, unsigned int>> typesEnum;
			typesEnum.push_back(std::pair<std::string, unsigned int>("Simplex", 0));
			typesEnum.push_back(std::pair<std::string, unsigned int>("Stream", 1));

			mnode->rawData<fl::DataObject>()->property("type")->setEnumerations(true);
			mnode->rawData<fl::DataObject>()->property("type")->setEnumArray(typesEnum);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return sceneToSetNode(space);
		}
	};

	inline void registerSceneToSetNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = sceneToSet_model_id;
		desc->name = "Scene To Set";
		desc->description = "Import objects from scenegraph into set.";
		desc->category = "Sets";
		desc->placement = "Sets/Scene To Set";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = SceneToSetNodeHandler::sceneToSetNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}
	
}
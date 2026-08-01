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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxFluentUtil.h"
#include "DfxOpenGLCacheQueue.h"

namespace util {

	size_t objectToFluentSet(deepsg::SgGeoAdapterInterface *adapter, DfxFluentSet *set, std::string setName, std::string scene_path, glm::mat4 xform, bool &success,
		bool cache, glm::vec3 scale) {

		size_t index = 0;

		if (adapter->getPolygonCount() > 0) {	
			auto obj = new DfxFluentSimplexObject;
			obj->copyMesh(adapter, scale);
			
			if (cache) {
				DfxOpenGLCacheQueue::getInstance()->queueCache(obj->cache());
			}

			index = DfxFluentSimplexCollection::getInstance()->addObject(obj, setName, scene_path, xform);
			set->addIndices(std::vector<size_t>{index});

			success = true;
			return index;
		}

		success = false;
		return index;
	}

	size_t geoNodeToFluentSet(deepsg::SgGeoNode *gnode, DfxFluentSet *set, std::string setName, std::string scene_path, glm::mat4 xform, bool &success, bool cache) {
		size_t index = 0;

		auto adapter = gnode->getGeometryAdapter();
		if (adapter->getPolygonCount() > 0) {
			auto obj = new DfxFluentSimplexObject;
			obj->copyGeometryNode(gnode);

			if (cache) {
				DfxOpenGLCacheQueue::getInstance()->queueCache(obj->cache());
			}

			index = DfxFluentSimplexCollection::getInstance()->addObject(obj, setName, scene_path, xform);
			set->addIndices(std::vector<size_t>{index});

			success = true;
			return index;
		}

		success = false;
		return index;
	}

	std::vector<size_t> pointCloudToFluentSet(deepsg::SgPointCloudNode *pcloud, DfxFluentSet *set, std::string setName, std::string scene_path, glm::mat4 xform, bool &success) {
		std::vector<size_t> output;

		if (pcloud->geo()->size() > 0) {
			auto attributes = pcloud->geo();

			size_t streamOffset = DfxFluentStreamCollection::getInstance()->growCollection(attributes->size());

			for (auto &attr : attributes->attributeMap()) {

				std::vector<size_t> indices;
				for (size_t i = 0; i < attributes->size(); i++) {
					output.push_back(streamOffset + i);
					indices.push_back(streamOffset + i);
				}
				set->addIndices(indices);
			
				if (attr.second->cType() == ds::type::FLOAT) {
					auto ref = compute::getAttribute<float>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), attr.first, DfxFluenType::FLOAT, true);
					
					if (ref.sameAsRequestedType && ref.exists && ref._data != nullptr) {
						auto stream = compute::extractAttrRef<float>(ref);
						auto channel = attr.second->at<float>();

						for (size_t i = 0; i < attributes->size(); i++) {
							stream[streamOffset + i] = channel[i];
						}
					}
					else {
						DfxGlobal::console::println("Point Cloud To Set: Did not inherit Attribute '" + attr.first +"', wrong type, expected float.");
					}
				}
				else if (attr.second->cType() == ds::type::VEC3){
					auto ref = compute::getAttribute<glm::vec3>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), attr.first, DfxFluenType::VEC3, true);
					bool isPositionAttribute = (attr.first == "@position");

					if (ref.sameAsRequestedType && ref.exists && ref._data != nullptr) {
						auto stream = compute::extractAttrRef<glm::vec3>(ref);
						auto channel = attr.second->at<glm::vec3>();
					
						for (size_t i = 0; i < attributes->size(); i++) {
							if (isPositionAttribute) {
								stream[streamOffset + i] = glm::vec3(xform * glm::vec4(channel[i], 1.f));
							}
							else {
								stream[streamOffset + i] = channel[i];
							}
						}
						
					}
					else {
						DfxGlobal::console::println("Point Cloud To Set: Did not inherit Attribute '" + attr.first + "', wrong type, expected Float_3.");
					}
				} 
				else if (attr.second->cType() == ds::type::SIZE_T) {
					auto ref = compute::getAttribute<size_t>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), attr.first, DfxFluenType::SIZE_T, true);

					if (ref.sameAsRequestedType && ref.exists && ref._data != nullptr) {
						auto stream = compute::extractAttrRef<size_t>(ref);
						auto channel = attr.second->at<size_t>();
						for (size_t i = 0; i < attributes->size(); i++) {
							stream[streamOffset + i] = channel[i];
						}
					}
					else {
						DfxGlobal::console::println("Point Cloud To Set: Did not inherit Attribute '" + attr.first + "', wrong type, expected Size_t.");
					}
				}
				else if (attr.second->cType() == ds::type::STRING){
					auto ref = compute::getAttribute<std::string>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), attr.first, DfxFluenType::STRING, true);

					if (ref.sameAsRequestedType && ref.exists && ref._data != nullptr) {
						auto stream = compute::extractAttrRef<std::string>(ref);
						auto channel = attr.second->at<std::string>();
						for (size_t i = 0; i < attributes->size(); i++) {
							stream[streamOffset + i] = channel[i];
						}
					}
					else {
						DfxGlobal::console::println("Point Cloud To Set: Did not inherit Attribute '" + attr.first + "', wrong type, expected String.");
					}
				}
				else if (attr.second->cType() == ds::type::BOOL || attr.second->cType() == ds::type::INT) {
					auto ref = compute::getAttribute<int>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), attr.first, DfxFluenType::INT, true);

					if (ref.sameAsRequestedType && ref.exists && ref._data != nullptr) {
						auto stream = compute::extractAttrRef<int>(ref);
						auto channel = attr.second->at<int>();
						for (size_t i = 0; i < attributes->size(); i++) {
							stream[streamOffset + i] = channel[i] ? 1 : 0;
						}
					}
					else {
						DfxGlobal::console::println("Point Cloud To Set: Did not inherit Attribute '" + attr.first + "', wrong type, expected bool.");
					}
				}
				else{
					DfxGlobal::console::println("Point Cloud To Set: Did not inherit Attribute '" + attr.first + "' unsupported fluent type.");
				}

			}
		}

		return output;
	}

	// Copy Attributes
	void copyAttributes(DfxFluentChannelMap &map, size_t from, std::vector<size_t> to) {
		for (auto channel : map.allChannels()) {

			if (channel->cType() == DfxFluenType::FLOAT) {
				auto attr = channel->at<float>();

				for (auto index : to) {
					attr[index] = attr[from];
				}
			}
			else if (channel->cType() == DfxFluenType::VEC3) {
				auto attr = channel->at<glm::vec3>();

				for (auto index : to) {
					attr[index] = attr[from];
				}
			}
			else if (channel->cType() == DfxFluenType::SIZE_T) {
				auto attr = channel->at<size_t>();

				for (auto index : to) {
					attr[index] = attr[from];
				}
			}
			else if (channel->cType() == DfxFluenType::BOOL) {
				auto attr = channel->at<int>();

				for (auto index : to) {
					attr[index] = attr[from];
				}
			}
			else if (channel->cType() == DfxFluenType::STRING) {
				auto attr = channel->at<std::string>();

				for (auto index : to) {
					attr[index] = attr[from];
				}
			}
		}
	}

};
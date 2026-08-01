/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include <SgGridNode.h>
#include "DfxGridHandler.h"
#include "DfxOpenVDBGridAdapter.h"

std::string DfxGridHandler::model_id = "/GRID/GRID_INFO";
size_t DfxGridHandler::ID_GEN = 1;

DfxGridHandler::DfxGridHandler() {

}

DfxGridHandler::~DfxGridHandler() {

}

fl::Any *DfxGridHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	return  fl::DataObject::Builder()
		.setName("volume")
		.setLabel("Volume" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "cache_id", "Cache Identifier", (size_t)0)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "voxel_pixel_size", "Voxel Pixel Size", 8.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "voxel_scale", "Voxel Size", 0.1f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "transparency", "Transparency", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "normalize", "Normalize", true)

		// Render Hint
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "use_render_hint", "Use Render Hint", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "cull_out_of_bounds", "Cull Out of Bounds", true)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "material_id", "Material ID", 0)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "voxel_size", "Voxel Size", 0.2f)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "width",  "Width",  50)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "height", "Height", 50)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "depth",  "Depth",  50)

		.build(space);
}

bool DfxGridHandler::create() {
	// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Objects Branch
	auto object_branch = space->findAbsolute("~/GRIDS");

	/// Proceed only if it exists
	if (object_branch != nullptr) {

		auto object = createModel(space);

		/// Object XForm 
		auto xform = newXForm();

		/// Arrange properties into the right order
		space->move(object, object_branch);
		space->move(xform, object);

		/// Store reference for further use
		_dataObject = object;

		/// Return success
		return true;
	}
	else {
		DfxGlobal::console::println("Grid Handler: Create failed to find branch.");
	}

	return false;
}

deepsg::SgGridNode *DfxGridHandler::gridNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_grid_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgGridNode>();
		}
	}

	return nullptr;
}

deepsg::SgGridNode *DfxGridHandler::createGridNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_grid_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgGridNode>();
		}
		else {

			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Store Scene graph node
			auto scenegraph_node = space->any("sg_grid_node", "Scene Grid Node");
			space->move(scenegraph_node, _dataObject);

			return (deepsg::SgGridNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

DfxVolumeCache *DfxGridHandler::volumeCache() {
	auto cache_uid = _dataObject->data<fl::DataObject>().property("cache_id")->data<size_t>();
	return DfxOpenGLCacheManager::getInstance()->findVolumeCache(cache_uid);
}

bool DfxGridHandler::destroy() {

	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {


		auto ndata00 = gridNode();
		auto gridAny = _dataObject->data<fl::DataObject>().property("sg_grid_node");
		auto cache00 = volumeCache();

		if (ndata00 != nullptr) {
			DfxQSceneManager::getInstance()->removeNode(ndata00);
			delete ndata00;
			gridAny->nullify();
		}

		if (cache00 != nullptr) {
			cache00->clear();
			DfxOpenGLCacheManager::getInstance()->removeVolumeCache(cache00->id());
			delete cache00;
		}
		else {
			DfxGlobal::console::println("Grid Handler: Destroy failed to delete cache.");
		}

		space->remove(_dataObject);
		_dataObject = nullptr;

		return true;
	}

	return false;
}

deepsg::SgNode* DfxGridHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {
	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		/// Master Space
		auto space = DfxSpace::getInstance();

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_grid_node");
		auto xform = any->data<fl::DataObject>().property("xform");

		auto vAdapter = new DfxOpenVDBGridAdapter;
		deepsg::SgGridNode *gnode = new DfxGridNode;
		gnode->setReferencePath(referencePath);
		gnode->setXFormProperties(xform);
		gnode->setGridAdapter(vAdapter);

		auto volumeCache = new DfxVolumeCache;
		volumeCache->setVolumeData(vAdapter);
		volumeCache->setAny(any);

		DfxOpenGLCacheQueue::getInstance()->queueCache(volumeCache);

		any->data<fl::DataObject>().property("cache_id")->setData<size_t>(volumeCache->id());

		DfxOpenGLCacheManager::getInstance()->addVolumeCache(volumeCache->id(), volumeCache);

		/// Store Scene graph node
		scenegraph_node->newData<deepsg::SgGridNode>(DfxFluentTypes::SG_GRID_NODE, gnode);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxGridHandler::deleteNode(deepsg::SgNode *node) {
	DfxGridHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}
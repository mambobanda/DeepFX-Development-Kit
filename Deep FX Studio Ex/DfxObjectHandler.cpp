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

#include "DfxObjectHandler.h"

std::string DfxObjectHandler::model_id = "/GEOMETRY/GEO_NODE_INFO";
size_t DfxObjectHandler::ID_GEN = 1;

DfxObjectHandler::DfxObjectHandler() {

}

DfxObjectHandler::~DfxObjectHandler() {

}

fl::Any *DfxObjectHandler::createModel(fl::Space *space) {

	auto any = fl::DataObject::Builder()
		.setName("object")
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "cache_id", "Cache Identifier", (size_t)0)
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "attribute_id", "Attribute Identifier", (size_t)0)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "resource_path", "External Resource", std::string(""))
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "smooth_shading", "Smooth Shading", true)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "update_cache", "Update Cache Flag", false)
		.addProperty(space, fl::type::INT, fl::role::NONE, "mesh_data", "Mesh Data", nullptr)
		.build(space);

	return any;
}

bool DfxObjectHandler::useSmoothShading() {
	return _dataObject->data<fl::DataObject>().property("smooth_shading")->data<bool>();
}

bool DfxObjectHandler::create(DfxResourcePtr *msh, std::string original_path, std::string name) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Objects Branch
	auto object_branch = space->findAbsolute("~/OBJS");

	/// Proceed only if it exists
	if (object_branch != nullptr) {
		
		std::string _label = "";

		if (name.empty()) {
			size_t id_g = ID_GEN++;
			std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);
			_label = _label;
		}
		else {
			_label = name;
		}

		/// Object Reference
		auto object = createModel(space);
		auto dao = object->rawData<fl::DataObject>();
		object->setLabel(_label);
		dao->property("mesh_data")->newData<DfxResourcePtr>(DfxFluentTypes::RESOURCE_POINTER, msh);
		dao->property("resource_path")->setData<std::string>(original_path);
		
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
		DfxGlobal::console::println("Object Handler: Create failed to find branch.");
	}
	/// Return failure
	return false;
}

deepsg::SgGeoNode *DfxObjectHandler::createGeoNode() {
	
	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_geo_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgGeoNode>();
		}
		else {
			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Create Data Slot
			auto scenegraph_node = space->any("sg_geo_node", "Scene Graph Geometry Node");
			space->move(scenegraph_node, _dataObject);
			
			return (deepsg::SgGeoNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

DfxAttribute *DfxObjectHandler::createAttributes() {
	if (_dataObject != nullptr) {
		auto *root = new DfxRootAttribute;
		auto obj_path = _dataObject->absolutePath();

		root->setPath(obj_path);
		root->setName(_dataObject->label());

		auto e_count = new DfxMeshAttributes(DfxMeshAttributes::EDGE_COUNT);
		e_count->setPath(obj_path);

		auto f_count = new DfxMeshAttributes(DfxMeshAttributes::FACE_COUNT);
		f_count->setPath(obj_path);

		auto p_count = new DfxMeshAttributes(DfxMeshAttributes::POINT_COUNT);
		p_count->setPath(obj_path);

		auto b_point_pos = new DfxMeshAttributes(DfxMeshAttributes::BASE_POINT_POSITION);
		b_point_pos->setPath(obj_path);

		auto f_point_pos = new DfxMeshAttributes(DfxMeshAttributes::FINAL_POINT_POSITION);
		f_point_pos->setPath(obj_path);

		auto b_point_norm = new DfxMeshAttributes(DfxMeshAttributes::BASE_POINT_NORMAL);
		b_point_norm->setPath(obj_path);

		auto f_point_norm = new DfxMeshAttributes(DfxMeshAttributes::FINAL_POINT_NORMAL);
		f_point_norm->setPath(obj_path);

		auto shape = new DfxMeshAttributes(DfxMeshAttributes::EDGE_COUNT);
		shape->setPath(obj_path);

		auto position = new DfxMeshAttributes(DfxMeshAttributes::POSITION);
		position->setPath(obj_path);

		auto rotation = new DfxMeshAttributes(DfxMeshAttributes::ROTATION);
		rotation->setPath(obj_path);

		auto scale    = new DfxMeshAttributes(DfxMeshAttributes::SCALE);
		scale->setPath(obj_path);

		root->addAttribute(e_count);
		root->addAttribute(f_count);
		root->addAttribute(p_count);

		root->addAttribute(b_point_pos);
		root->addAttribute(f_point_pos);

		root->addAttribute(b_point_norm);
		root->addAttribute(f_point_norm);

		root->addAttribute(shape);
		
		root->addAttribute(position);
		root->addAttribute(rotation);
		root->addAttribute(scale);

		_dataObject->data<fl::DataObject>().property("attribute_id")->setData<size_t>(root->id());

		DfxAttributeTree::getInstance()->addAttribute(root);

		return root;
	}
	
	return nullptr;
}

bool DfxObjectHandler::destroy() {

	if (_dataObject != nullptr) {

		auto space = DfxSpace::getInstance();

		auto mdata00 = dsMeshData();
		auto ndata00 = geoNode();
		auto cache00 = objectCache();
		auto attr00 = attributes();

		if (ndata00 != nullptr) {

			DfxQSceneManager::getInstance()->removeNode(ndata00);
			delete ndata00->getGeometryAdapter();
			ndata00->setGeometryAdapter(nullptr);
			delete ndata00;

			auto any = _dataObject->data<fl::DataObject>().property("sg_geo_node");
			any->nullify();

		}
		else {
			DfxGlobal::console::println("Object Handler: Destroy failed to clear node.");
		}

		if (mdata00 != nullptr) {
			mdata00->releasePointer(true);

			auto any = _dataObject->data<fl::DataObject>().property("mesh_data");
			any->nullify();
		}
		else {
			DfxGlobal::console::println("Object Handler: Destroy failed to clear mesh.");
		}

		if (cache00 != nullptr) {
			cache00->clear();
			DfxOpenGLCacheManager::getInstance()->removeObjectCache(cache00->id());
			delete cache00;
		}
		else {
			DfxGlobal::console::println("Object Handler: Destroy failed to delete cache.");
		}

		/*if (attr00 != nullptr) {
			DfxAttributeTree::getInstance()->removeAttribute(attr00);
			delete attr00;
		}
		else {
			DfxGlobal::console::println("Object Handler: Destroy failed to delete attributes.");
		}*/

		DfxMaterialLinker::getInstance()->removeAllLinks(_dataObject->absolutePath());
		DfxSurfaceDictionary::getInstance()->eraseAll(_dataObject->absolutePath());

		space->remove(_dataObject);

		return true;
	}

	return false;
}

bool DfxObjectHandler::updateCache() {
	auto any = _dataObject->data<fl::DataObject>().property("update_cache");
	
	if (any != nullptr) {
		return any->data<bool>();
	}
	else {
		return nullptr;
	}

	return false;
}

void DfxObjectHandler::setUpdateCache(bool val) {
	auto any = _dataObject->data<fl::DataObject>().property("update_cache");

	if (any != nullptr) {
		any->setData<bool>(val);
	}
	
}

deepsg::SgGeoNode *DfxObjectHandler::geoNode() {
	auto any = _dataObject->data<fl::DataObject>().property("sg_geo_node");

	if (any != nullptr) {
		return any->rawData<deepsg::SgGeoNode>();
	}
	else {
		return nullptr;
	}
}

DfxResourcePtr *DfxObjectHandler::dsMeshData() {
	auto any = _dataObject->data<fl::DataObject>().property("mesh_data");

	if (any != nullptr) {
		return any->rawData<DfxResourcePtr>();
	}
	else {
		return nullptr;
	}
}

DfxAttribute *DfxObjectHandler::attributes() {
	auto attribute_uid = _dataObject->data<fl::DataObject>().property("attribute_id")->data<size_t>();
	return DfxAttributeTree::getInstance()->findAttribute(attribute_uid);
}

bool DfxObjectHandler::clearAllObjects() {
	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Objects Branch
	auto object_branch = space->findAbsolute("~/OBJS");

	/// Proceed only if it exists
	if (object_branch != nullptr) {
		for (auto any : object_branch->data<fl::DataObject>().properties()) {
			DfxObjectHandler handler;
			bool rc = handler.fetch(space->absolutePath(any));

			if (rc) {
				if (!handler.destroy()) {
					DfxGlobal::console::println("Object Handler: Clear all failed to destroy object.");
				}
			}
			else {
				DfxGlobal::console::println("Object Handler: Clear all failed to fetch object.");
			}
		}
	}
	else {
		DfxGlobal::console::println("Object Handler: Clear all failed to find branch.");
		return false;
	}

	return true;
}

DfxObjectCache *DfxObjectHandler::objectCache() {
	auto cache_uid =_dataObject->data<fl::DataObject>().property("cache_id")->data<size_t>();
	return DfxOpenGLCacheManager::getInstance()->findObjectCache(cache_uid);
}

DfxObjectCache *DfxObjectHandler::fetchObjectCache() {
	auto geo = geoNode();
	auto adapter = geo->getGeometryAdapter();

	if (adapter->type() == DfxGeoAdapterType::DYNA_MESH) {
		auto dynaMesh = (DfxDynaMesh*)geo->getDynaMesh();
		return dynaMesh->cache();
	}
	else{
		return objectCache();
	}

	return nullptr;
}

void DfxObjectHandler::releaseDeepSimplexResource(void *ptr) {
	auto mesh = (ds::Mesh*)ptr;
	mesh->fastClearMesh();
	delete mesh;
}

deepsg::SgNode* DfxObjectHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {
	
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);
	
	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_geo_node");
		auto msh = any->data<fl::DataObject>().property("mesh_data")->rawData<DfxResourcePtr>();
		auto xform = any->data<fl::DataObject>().property("xform");

		deepsg::SgGeoNode *gnode = new DfxGeometryNode;

		scenegraph_node->newData<deepsg::SgGeoNode>(DfxFluentTypes::SG_GEO_NODE, gnode);

		/// Finish setting up scene graph node and move it to the master scene graph
		DfxDeepSimplexGeoAdapter *dsAdapter = new DfxDeepSimplexGeoAdapter;
		dsAdapter->setMesh(msh);
		gnode->setGeometryAdapter(dsAdapter);
		gnode->setXFormProperties(xform);
		gnode->setReferencePath(referencePath);

		DfxObjectCache *objCache = new DfxObjectCache;
		objCache->setMeshData(dsAdapter);
		objCache->setAny(any);

		DfxOpenGLCacheQueue::getInstance()->queueCache(objCache);

		any->data<fl::DataObject>().property("cache_id")->setData<size_t>(objCache->id());

		DfxOpenGLCacheManager::getInstance()->addObjectCache(objCache->id(), objCache);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxObjectHandler::deleteNode(deepsg::SgNode *node) {
	DfxObjectHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}

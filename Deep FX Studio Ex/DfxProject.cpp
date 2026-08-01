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
#include <VH5File.h>
#include <VH5DocumentResource.h>
#include <VH5DsMeshResource.h>
#include <VH5SurfDictionaryEntry.h>
#include <VH5IResource.h>
#include <DfxGlobal.h>
#include <FlDataObject.h>
#include <DfxPropertyConduit.h>
#include "DfxProject.h"
#include "DfxSurfaceDictionary.h"
#include "DfxResourceManager.h"
#include "DfxMaterialLinker.h"
#include "DfxSurfaceDictionary.h"
#include "DfxSgNodeDB.h"
#include "DfxObjectHandler.h"
#include "DfxCacheTrackManager.h"

std::vector<deepsg::SgStageNode*> DfxProject::simulationStageList;
DfxProject *DfxProject::_instance = nullptr;

DfxProject::DfxProject() {
	_state = nullptr;
	_projectPath = "";
}

DfxProject::~DfxProject() {

}

DfxProject *DfxProject::getInstance() {

	if ( _instance == nullptr ) {
		 _instance = new DfxProject;
	}

	return _instance;
}

void DfxProject::loadEverythingElse00(xvault::File *file) {
	file->walkFile([](std::string name, xvault::iResource *resource, void *userdata) {
		auto prj = (DfxProject*)userdata;

		//prj->loadSurfaceDictionary(name, resource, userdata);
		prj->loadGeometryResources(name, resource, userdata);

	}, this);

	DfxResourceManager::getInstance()->maxId();
}

void DfxProject::saveGeometryResources(xvault::File *file) {

	for (size_t i = 0; i < DfxResourceManager::getInstance()->geo()->childCount(); i++) {
		auto resource = DfxResourceManager::getInstance()->geo()->getChild(i);

		if (resource->mode() == resource::mode::RESOURCE &&
			resource->type() == resource::type::GEOMETRY) {
			if (resource->isEmbedded()) {

				if (resource->format() == resource::format::DS_MESH) {
					auto msh = static_cast<ds::Mesh*>(resource->resource());

					xvault::h5::h5DsMeshResource m_resource;
					m_resource.setResourceID(resource->id());
					m_resource.setName(resource->name());
					m_resource.setMesh(msh);

					file->writeResource("/objects/obj_" + std::to_string(resource->id()), m_resource);
				}

			}
		}
	}

}

void DfxProject::loadGeometryResources(std::string, xvault::iResource *resource, void *userdata) {

	auto content = resource->findAttribute("__content__");
	auto resourceId = resource->findAttribute("__resource_id__");
	auto name = resource->findAttribute("__name__");
	auto engine = resource->findAttribute("__engine__");

	if (content != nullptr) {
		if (content->valueString() == "geometry") {

			if (resourceId != nullptr && engine != nullptr) {
					
				if (engine->valueString() == "deep_simplex") {
					auto msh = xvault::h5::h5DsMeshResource::toDsMesh(resource);

					if (msh != nullptr) {

						std::string rname = "<Mesh Resource>";

						if (name != nullptr) rname = name->valueString();

						auto mshResource = new DfxResource(resource::mode::RESOURCE, resource::type::GEOMETRY, "");
						mshResource->setResource(msh);
						mshResource->setId(resourceId->valueULongLong());
						mshResource->setFormat(resource::format::DS_MESH);
						mshResource->setName(rname);
						mshResource->setReleaseCallback(DfxObjectHandler::releaseDeepSimplexResource);
						DfxResourceManager::getInstance()->geo()->addChild(mshResource);

					}
				}
			}
		}
	}
}

void DfxProject::loadSpaceBindings(xvault::File *file) {

	auto space = DfxSpace::getInstance();

	xvault::h5::h5IResource iBindings;
	xvault::rapid::rjDocument rBindings;
	char *iBindingsDoc = nullptr;

	file->readResource("/fluent_space_bindings", iBindings);
	
	for (auto buff : iBindings.buffers()) {
		if (buff->name() == ".document") {
			if (buff->type() == xvault::type::CHAR) {
				iBindingsDoc = buff->bufferChar();
			}
		}
	}

	if (iBindingsDoc != nullptr) {

		std::string  errorMsg;
		if (!rBindings.fromString(iBindingsDoc, errorMsg)) {
			DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("failed to parse space bindings model (") + errorMsg + ")");
			return;
		}

		for (auto node : rBindings.root()->nodes()) {

			auto inputAttr = node->findAttributeByName("input");

			for (auto link : node->nodes()) {

				auto attr00 = link->findAttributeByName("output");
				auto attr01 = link->findAttributeByName("output_node");
				auto attr02 = link->findAttributeByName("input");
				auto attr03 = link->findAttributeByName("input_node");
				auto attr04 = link->findAttributeByName("stamp");

				auto outputNodeStr = std::string("");
				auto intputNodeStr = std::string("");

				if (attr01 != nullptr) outputNodeStr = attr01->valueString();
				if (attr03 != nullptr) intputNodeStr = attr03->valueString();

				space->addBinding(attr00->valueString(), outputNodeStr, inputAttr->valueString(), intputNodeStr, attr04->valueUInt());
			}

		}

	}
}

void DfxProject::loadSurfaceDictionary(xvault::File *file) {

	xvault::h5::h5IResource iMaterialMap;
	xvault::rapid::rjDocument rSurfaceDictionaryMap;
	char *iSurfaceDictionaryMapDoc = nullptr;

	file->readResource("/surface_dictionary", iMaterialMap);

	for (auto buff : iMaterialMap.buffers()) {
		if (buff->name() == ".document") {
			if (buff->type() == xvault::type::CHAR) {
				iSurfaceDictionaryMapDoc = buff->bufferChar();
			}
		}
	}

	if (iSurfaceDictionaryMapDoc != nullptr) {

		std::string  errorMsg;
		if (!rSurfaceDictionaryMap.fromString(iSurfaceDictionaryMapDoc, errorMsg)) {
			DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("failed to parse surface dictionary model (") + errorMsg + ")");
			return;
		}

		for (auto node : rSurfaceDictionaryMap.root()->nodes()) {

			auto inputAttr = node->findAttributeByName("obj_path");

			for (auto link : node->nodes()) {

				auto attr00 = link->findAttributeByName("uid");
				auto attr01 = link->findAttributeByName("name");

				DfxSurfaceDictionary::getInstance()->insert(inputAttr->valueString(), attr00->valueUInt(), attr01->valueString());
			}

		}

	}
}

void DfxProject::loadMaterialLinks(xvault::File *file) {

	xvault::h5::h5IResource iMaterialMap;
	xvault::rapid::rjDocument rMaterialMap;
	char *iMaterialMapDoc = nullptr;

	file->readResource("/material_links", iMaterialMap);

	for (auto buff : iMaterialMap.buffers()) {
		if (buff->name() == ".document") {
			if (buff->type() == xvault::type::CHAR) {
				iMaterialMapDoc = buff->bufferChar();
			}
		}
	}

	if (iMaterialMapDoc != nullptr) {

		std::string  errorMsg;
		if (!rMaterialMap.fromString(iMaterialMapDoc, errorMsg)) {
			DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("failed to parse material links model (") + errorMsg + ")");
			return;
		}

		for (auto node : rMaterialMap.root()->nodes()) {

			auto inputAttr = node->findAttributeByName("obj_path");

			for (auto link : node->nodes()) {

				auto attr00 = link->findAttributeByName("surfID");
				auto attr01 = link->findAttributeByName("material");

				DfxMaterialLinker::getInstance()->updateObjectLink(inputAttr->valueString(), attr00->valueUInt(), attr01->valueString());
			}

		}

	}
}

void DfxProject::loadFluentsFlows(xvault::File *file) {

	auto space = DfxSpace::getInstance();

	xvault::h5::h5IResource iflows;
	xvault::rapid::rjDocument rflows;
	char *iFlowsDoc = nullptr;

	file->readResource("/fluent_flows", iflows);

	for (auto buff : iflows.buffers()) {
		if (buff->name() == ".document") {
			if (buff->type() == xvault::type::CHAR) {
				iFlowsDoc = buff->bufferChar();
			}
		}
	}

	if (iFlowsDoc != nullptr) {

		std::string errorMsg;
		if (!rflows.fromString(iFlowsDoc, errorMsg)) {
			DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("failed to parse fluent flow model (") + errorMsg + ")");
			return;
		}

		loadFluentsFlows(rflows.root(), true);
		DfxFlowDB::getInstance()->resetMaxID();
	}
}

fl::Flow *DfxProject::loadFluentsFlows(xvault::Node *node, bool isDocument) {

	if (isDocument) {
		for (auto no : node->nodes()) {
			loadFluentsFlows(no, false);
		}
	}
	else {

		auto _flow = new fl::Flow();
		_flow->setSpace(DfxSpace::getInstance());

		auto lAttr00 = node->findAttributeByName("root_flow");
		auto lAttr01 = node->findAttributeByName("uid");

		auto lAttr02 = node->findAttributeByName("id");
		auto lAttr03 = node->findAttributeByName("type");
		auto lAttr04 = node->findAttributeByName("next");
		auto lAttr05 = node->findAttributeByName("yes");
		auto lAttr06 = node->findAttributeByName("no");
		auto lAttr07 = node->findAttributeByName("conditionPath");
		auto lAttr08 = node->findAttributeByName("conditionOutputNodePath");
		auto lAttr09 = node->findAttributeByName("text");
		auto lAttr10 = node->findAttributeByName("posx");
		auto lAttr11 = node->findAttributeByName("posy");

		auto paths = node->findNodeByName("paths");
		auto nodes = node->findNodeByName("nodes");

		if (lAttr02->type() == xvault::type::INT) _flow->setID(lAttr02->valueInt());
		else  _flow->setID(lAttr02->valueUInt());

		if (lAttr03->type() == xvault::type::INT) _flow->setType(lAttr03->valueInt());
		else  _flow->setType(lAttr03->valueUInt());

		if (lAttr04->type() == xvault::type::INT) _flow->setNext(lAttr04->valueInt());
		else  _flow->setNext(lAttr04->valueUInt());

		if (lAttr05->type() == xvault::type::INT) _flow->setYes(lAttr05->valueInt());
		else  _flow->setYes(lAttr05->valueUInt());

		if (lAttr06->type() == xvault::type::INT) _flow->setNo(lAttr06->valueInt());
		else  _flow->setNo(lAttr06->valueUInt());

		_flow->setDecisionConditionPath(lAttr07->valueString());

		_flow->setDecisionOuputNodePath(lAttr08->valueString());

		_flow->setText(lAttr09->valueString());

		if (lAttr10->type() == xvault::type::DOUBLE) _flow->setPosX((float)lAttr10->valueDouble());
		else _flow->setPosX(lAttr10->valueFloat());

		if (lAttr11->type() == xvault::type::DOUBLE) _flow->setPosY((float)lAttr11->valueDouble());
		else _flow->setPosY(lAttr11->valueFloat());

		for (auto no: paths->attributes()) {
			_flow->paths().push_back(no->valueString());
		}

		for (auto no : nodes->nodes()) {
			auto rt_flow = loadFluentsFlows(no, false);

			if (rt_flow != nullptr) 
				_flow->nodes().push_back(rt_flow);
		}

		if (lAttr00->valueUInt()) {
			_flow->resetID();
			DfxFlowDB::getInstance()->addFlow(lAttr01->valueUInt(), _flow);
		}

		return _flow;
	}

	return nullptr;
}

void DfxProject::loadCacheTracks(xvault::File *file) {
	auto space = DfxSpace::getInstance();

	xvault::h5::h5IResource iTracks;
	xvault::rapid::rjDocument rTracks;
	char *itrackDoc = nullptr;

	file->readResource("/cache_tracks", iTracks);

	for (auto buff : iTracks.buffers()) {
		if (buff->name() == ".document") {
			if (buff->type() == xvault::type::CHAR) {
				itrackDoc = buff->bufferChar();
			}
		}
	}

	if (itrackDoc != nullptr) {

		std::string errorMsg;
		if (!rTracks.fromString(itrackDoc, errorMsg)) {
			DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("failed to parse cache track model (") + errorMsg + ")");
			return;
		}

		auto tracks = rTracks.root()->findNodeByName("tracks");

		if (tracks != nullptr) {
			for (int i = 0; i < tracks->nodes().size(); i++) {
				DfxCacheTrack *_track = nullptr;

				auto node = tracks->nodes()[i];
				auto path = node->findAttributeByName("path");

				if (i == 0) {
					_track = DfxCacheTrackManager::getInstance()->track(0);
				}
				else {
					_track = new DfxCacheTrack;
				}

				if (path != nullptr) _track->setPath(path->valueString());

				if (i != 0) DfxCacheTrackManager::getInstance()->addTrack(_track);
			
			}	
		}
		else {
			DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("failed to parse cache track model (Paths node missing)"));
			return;
		}
	}
}

void DfxProject::mapAttributeToAny(xvault::Buffer *buffer, xvault::Attribute *attribute, fl::Any *any, unsigned int type) {

	switch (type) {
	case fl::type::BOOL:
	{
		any->newData<bool>(fl::type::BOOL);
		any->setData<bool>((attribute->valueInt() == 1) ? true : false);
	}
	break;
	case fl::type::CHAR:
	{
		any->newData<char>(fl::type::CHAR);
		any->setData<char>(attribute->valueChar());
	}
	break;
	case fl::type::DOUBLE:
	{
		any->newData<double>(fl::type::DOUBLE);

		if (attribute->type() == xvault::type::FLOAT) {
			any->setData<double>(attribute->valueFloat());
		}
		else {
			any->setData<double>(attribute->valueDouble());
		}
	}
	break;
	case fl::type::DOUBLE_2:
	{
		if (buffer->bufferSize() == 2) {
			glm::dvec2 v(buffer->bufferDouble()[0], buffer->bufferDouble()[1]);
			any->newData<glm::dvec2>(fl::type::DOUBLE_2);
			any->setData<glm::dvec2>(v);
		}
	}
	break;
	case fl::type::DOUBLE_3:
	{
		if (buffer->bufferSize() == 3) {
			glm::dvec3 v(buffer->bufferDouble()[0], buffer->bufferDouble()[1], buffer->bufferDouble()[2]);
			any->newData<glm::dvec3>(fl::type::DOUBLE_3);
			any->setData<glm::dvec3>(v);
		}
	}
	break;
	case fl::type::DOUBLE_4:
	{
		if (buffer->bufferSize() == 4) {
			glm::dvec4 v(buffer->bufferDouble()[0], buffer->bufferDouble()[1], buffer->bufferDouble()[2], buffer->bufferDouble()[3]);
			any->newData<glm::dvec4>(fl::type::DOUBLE_4);
			any->setData<glm::dvec4>(v);
		}
	}
	break;
	case fl::type::FLOAT:
	{
		any->newData<float>(fl::type::FLOAT);

		if (attribute->type() == xvault::type::DOUBLE) {
			any->setData<float>(attribute->valueDouble());
		}
		else {
			any->setData<float>(attribute->valueFloat());
		}
	}
	break;
	case fl::type::FLOAT_2:
	{
		any->newData<glm::vec2>(fl::type::FLOAT_2);
		any->setData<glm::vec2>(attribute->valueFloat2());	
	}
	break;
	case fl::type::FLOAT_3:
	{
		any->newData<glm::vec3>(fl::type::FLOAT_3);
		any->setData<glm::vec3>(attribute->valueFloat3());
	}
	break;
	case fl::type::FLOAT_4:
	{
		any->newData<glm::vec4>(fl::type::FLOAT_4);
		any->setData<glm::vec4>(attribute->valueFloat4());
	}
	break;
	case fl::type::MAT_3:
	{
		any->newData<glm::mat3>(fl::type::MAT_3);
		any->setData<glm::mat3>(attribute->valueFloat9());
	}
	break;
	case fl::type::MAT_4:
	{
		any->newData<glm::mat4>(fl::type::MAT_4);
		any->setData<glm::mat4>(attribute->valueFloat16());
	}
	break;
	case fl::type::INT:
	{
		any->newData<int>(fl::type::INT);

		if (attribute->type() == xvault::type::UNSIGNED_INT) {
			any->setData<int>(attribute->valueUInt());
		}
		else {
			any->setData<int>(attribute->valueInt());
		}
		
	}
	break;
	case fl::type::INT_2:
	{
		if (buffer->bufferSize() == 2) {
			glm::ivec2 v(buffer->bufferInt()[0], buffer->bufferInt()[1]);
			any->newData<glm::ivec2>(fl::type::INT_2);
			any->setData<glm::ivec2>(v);
		}
	}
	break;
	case fl::type::INT_3:
	{
		if (buffer->bufferSize() == 3) {
			glm::ivec3 v(buffer->bufferInt()[0], buffer->bufferInt()[1], buffer->bufferInt()[2]);
			any->newData<glm::ivec3>(fl::type::INT_3);
			any->setData<glm::ivec3>(v);
		}
	}
	break;
	case fl::type::INT_4:
	{
		if (buffer->bufferSize() == 4) {
			glm::ivec4 v(buffer->bufferInt()[0], buffer->bufferInt()[1], buffer->bufferInt()[2], buffer->bufferInt()[3]);
			any->newData<glm::ivec4>(fl::type::INT_4);
			any->setData<glm::ivec4>(v);
		}
	}
	break;
	case fl::type::SIZE_T:
	{
		any->newData<size_t>(fl::type::SIZE_T);

		if (attribute->type() == xvault::type::UNSIGNED_INT) {
			any->setData<size_t>(attribute->valueUInt());
		}
		else {
			any->setData<size_t>(attribute->valueULongLong());
		}
	}
	break;
	case fl::type::STD_STRING:
	{
		any->newData<std::string>(fl::type::STD_STRING);
		any->setData<std::string>(attribute->valueString());
	}
	break;
	case fl::type::UNSIGNED_CHAR:
	{
		any->newData<unsigned char>(fl::type::UNSIGNED_CHAR);
		any->setData<unsigned char>(attribute->valueUInt());
	}
	break;
	case fl::type::UNSIGNED_INT:
	{
		any->newData<unsigned int>(fl::type::UNSIGNED_INT);

		if (attribute->type() == xvault::type::INT) {
			any->setData<unsigned int>(attribute->valueInt());
		}
		else {
			any->setData<unsigned int>(attribute->valueUInt());
		}
	}
	break;
	case DfxFluentTypes::RESOURCE:
	{
		auto resource = DfxResourceManager::getInstance()->geo()->getChildByID(attribute->valueUInt());
		any->newData<DfxResource>(DfxFluentTypes::RESOURCE, resource);
	}
	break;
	case DfxFluentTypes::RESOURCE_POINTER:
	{
		auto resource = DfxResourceManager::getInstance()->geo()->getChildByID(attribute->valueUInt());
		any->newData<DfxResourcePtr>(DfxFluentTypes::RESOURCE_POINTER, resource->newPointer());
	}
	default:
		break;
	}
}

void DfxProject::loadSpace(xvault::Node *node, std::string parentPath, fl::Any *parentAny) {

	auto space = DfxSpace::getInstance();
	auto extendedPath = parentPath;

	auto nameAttr  = node->findAttributeByName("name");
	auto labelAttr = node->findAttributeByName("label");
	auto typeAttr  = node->findAttributeByName("type");
	auto roleAttr  = node->findAttributeByName("role");
	auto modelAttr = node->findAttributeByName("model");
	auto isFunctionalAttr = node->findAttributeByName("isFunctional");
	auto keysNode  = node->findNodeByName("keys");
	auto valueAttr = node->findAttributeByName("value");
	auto valueBuff = node->findBufferByName("value");

	if (nameAttr == nullptr && labelAttr == nullptr && typeAttr == nullptr) {

		if (node->nodes().size() > 0) {
			extendedPath = "~";
			for (auto n : node->nodes()) {
				loadSpace(n, extendedPath, space->root());
			}
		}
	}
	else {

		if (modelAttr != nullptr && parentAny != nullptr) {

			if (parentAny->type() == fl::type::DATA_OBJECT) {

				auto m_any = fl::ModelDB::getInstance()->fetchModel(modelAttr->valueString(), space);

				if (m_any != nullptr) space->move(m_any, parentAny);		

				m_any->setName(nameAttr->valueString());
				m_any->setLabel(labelAttr->valueString());

				if (isFunctionalAttr != nullptr) {
					auto dao = m_any->rawData<fl::DataObject>();
					dao->setIsFunctional(true);
				}
			}
		}

		extendedPath += "/" + nameAttr->valueString();
		auto any = space->findAbsolute(extendedPath);

		if (typeAttr->valueUInt() == fl::type::DATA_OBJECT) {

			if (any == nullptr) {
				any = fl::DataObject::Builder()
					.setName(nameAttr->valueString())
					.setLabel(labelAttr->valueString())
					.build(space);

				space->move(any, parentAny);
			}
			else {
				any->setName(nameAttr->valueString());
				any->setLabel(labelAttr->valueString());
			}

			auto inputReferencesNode = node->findNodeByName("inputReferences");
			auto outputReferencesNode = node->findNodeByName("outputReferences");

			if (inputReferencesNode != nullptr) {
				for (auto &cnode : inputReferencesNode->nodes()) {
					auto firstAttr = cnode->findAttributeByName("first");
					auto secondAttr = cnode->findAttributeByName("second");

					if (firstAttr != nullptr && secondAttr != nullptr) {
						any->rawData<fl::DataObject>()->addInputReference(firstAttr->valueString(), secondAttr->valueString());
					}
				}
			}

			if (outputReferencesNode != nullptr) {
				for (auto &cnode : outputReferencesNode->nodes()) {
					auto firstAttr = cnode->findAttributeByName("first");
					auto secondAttr = cnode->findAttributeByName("second");

					if (firstAttr != nullptr && secondAttr != nullptr) {
						any->rawData<fl::DataObject>()->addOutputReference(firstAttr->valueString(), secondAttr->valueString());
					}
				}
			}

			for (auto n : node->nodes()) {
				loadSpace(n, extendedPath, any);
			}
		}
		else {
			
			if (any == nullptr) {
				any = space->any(nameAttr->valueString(), labelAttr->valueString());
				space->move(any, parentAny);
			}
			else {
				any->setName(nameAttr->valueString());
				any->setLabel(labelAttr->valueString());
			}

			if (!any->isArray()) {
				mapAttributeToAny(valueBuff, valueAttr, any, typeAttr->valueUInt());
			}

			if (roleAttr != nullptr) any->setRole(roleAttr->valueUInt());

			if (keysNode != nullptr) {
				
				auto keyPath = any->absolutePath();
				DfxPropertyConduit::getInstance()->createKeyframeFloatOverride(keyPath, any->data<float>());
				for (auto key: keysNode->nodes()) {

					auto frame  = key->findAttributeByName("frame");
					auto value  = key->findAttributeByName("value");
					auto interpolation = key->findAttributeByName("interpolation");

					// Optional Values
					auto bezier_cp0x = key->findAttributeByName("bezier_cp0x");
					auto bezier_cp0y = key->findAttributeByName("bezier_cp0y");
					auto bezier_cp1x = key->findAttributeByName("bezier_cp1x");
					auto bezier_cp1y = key->findAttributeByName("bezier_cp1y");

					unsigned int frm = frame->valueUInt();
					float val = (float)value->valueDouble();
					unsigned int intr = interpolation->valueUInt();

					DfxPropertyConduit::getInstance()->updateKeyframeFloatOverride(keyPath, frm, val);
					DfxPropertyConduit::getInstance()->updateKeyframeFloatInterpolationType(keyPath, frm, intr);

					if (bezier_cp0x != nullptr && bezier_cp0y != nullptr &&
						bezier_cp1x != nullptr && bezier_cp1y != nullptr) {

						glm::vec2 cp0(0.0f);
						glm::vec2 cp1(0.0f);

						cp0.x = bezier_cp0x->valueDouble();
						cp0.y = bezier_cp0y->valueDouble();
						cp1.x = bezier_cp1x->valueDouble();
						cp1.y = bezier_cp1y->valueDouble();

						DfxPropertyConduit::getInstance()->updateKeyframeFloatControlPoints(keyPath, frm, std::pair<glm::vec2, glm::vec2>(cp0, cp1));
					}
				}

				DfxPropertyConduit::getInstance()->announceNewKeyframeCreated();

			}
		}
	}
}

void DfxProject::loadGraph(xvault::Node *node, deepsg::SgNode *sgnode) {

	auto uidAttr = node->findAttributeByName("uid");
	auto refAttr = node->findAttributeByName("reference");
	auto visAttr = node->findAttributeByName("visible");
	auto renAttr = node->findAttributeByName("renderable");

	if (uidAttr == nullptr &&  refAttr == nullptr) {

		if (node->nodes().size() > 0) {
			for (auto n : node->nodes()) {
				loadGraph(n, deepsg::SgRootNode::getInstance());
			}
		}
	}
	else {
		if (uidAttr->valueString() == "UID_ROOT_NODE") {

			auto _sgnode = sgnode;
			if (_sgnode != nullptr) {
				for (auto n : node->nodes()) {
					loadGraph(n, _sgnode);
				}
			}
		}
		else {
			auto loadCallback = DfxSqNodeDB::getInstance()->loadCallback(uidAttr->valueString());

			if (loadCallback != nullptr) {

				auto _sgnode = loadCallback(refAttr->valueString(), sgnode);

				if (_sgnode != nullptr) {

					if (_sgnode->getType() == deepsg::SgNodeType::STAGE) simulationStageList.push_back((deepsg::SgStageNode*)_sgnode);

					if (visAttr != nullptr) {
						bool visible = visAttr->valueUInt() == 0 ? false : true;

						if (!visible) DfxQSceneManager::getInstance()->toggleVisibilty(_sgnode);
					}

					if (renAttr != nullptr) {
						bool renderable = renAttr->valueUInt() == 0 ? false : true;

						if (!renderable) DfxQSceneManager::getInstance()->toggleRenderable(_sgnode);
					}

					for (auto n : node->nodes()) {
						loadGraph(n, _sgnode);
					}
				}
			}

			
		}
	}
}

bool DfxProject::save(std::string path) {

	if (!path.empty()) {

		std::unique_ptr<xvault::File> ofile(new xvault::h5::h5File);

		if (ofile->open(path, xvault::io::OPEN)) {

			filesystem::path fpath(path);

			xvault::h5::h5ODocumentResource _header(".document");
			xvault::h5::h5ODocumentResource _space(".document");
			xvault::h5::h5ODocumentResource _spaceBindings(".document");
			xvault::h5::h5ODocumentResource _graph(".document");
			xvault::h5::h5ODocumentResource _surfaceDictionary(".document");
			xvault::h5::h5ODocumentResource _materialLinks(".document");
			xvault::h5::h5ODocumentResource _flows(".document");
			xvault::h5::h5ODocumentResource _layout(".document");
			xvault::h5::h5ODocumentResource _tracks(".document");

			auto vSpaceDoc = util::spaceToVDocument();
			auto vGraphDoc = util::graphToVDocument();
			auto vSpaceBindingsDoc = util::spaceBindingsToVDocument();
			auto vSurfaceDictionaryDoc = util::surfaceDictionaryToVDocument();
			auto vMaterialLinks = util::materialLinksToVDocument();
			auto vFlows = util::fluentFlowsToVDocument();
			auto vTracks = util::tracksToVDocument();

			if (vSpaceDoc == nullptr) {
				DfxGlobal::console::println("Failed to Save Project: Unable to serialize space.");
				ofile->close();
				return false;
			}

			if (vGraphDoc == nullptr) {
				DfxGlobal::console::println("Failed to Save Project: Unable to serialize scene graph.");
				ofile->close();
				return false;
			}

			if (_state != nullptr) {
				delete _state;
				_state = nullptr;
			}

			QDateTime dt = QDateTime::currentDateTime();

			_state = new DfxProjectState;
			_state->path = path;
			_state->project_name = fpath.stem().generic_string();
			_state->created = dt.toString(DfxProjectHeader::_dateTimeFormat).toStdString();

			auto createdAttr = xvault::h5::h5Attribute::newValueString("created", xvault::type::STRING, _state->created);
			auto vHeaderDoc = DfxProjectHeader::headerXVaultDocument();
			vHeaderDoc->root()->attributes().push_back(createdAttr);

			_header.setText(vHeaderDoc->toString());
			_space.setText(vSpaceDoc->toString());
			_graph.setText(vGraphDoc->toString());
			_spaceBindings.setText(vSpaceBindingsDoc->toString());
			_surfaceDictionary.setText(vSurfaceDictionaryDoc->toString());
			_materialLinks.setText(vMaterialLinks->toString());
			_flows.setText(vFlows->toString());
			_layout.setText(DfxViewLayoutManager::currentLayoutToString());
			_tracks.setText(vTracks->toString());

			// DfxGlobal::console::println(vSpaceDoc->toString());
			// DfxGlobal::console::println(vGraphDoc->toString());
			// DfxGlobal::console::println(vSpaceBindingsDoc->toString());
			// DfxGlobal::console::println(vSpaceBindingsDoc->toString());
			// DfxGlobal::console::println(vSurfaceDictionaryDoc->toString());
			// DfxGlobal::console::println(vMaterialLinks->toString());
			// DfxGlobal::console::println(vFlows->toString());
			// DfxGlobal::console::println(vTracks->toString());

			ofile->writeResource("/header", _header);
			ofile->writeResource("/fluent_space", _space);
			ofile->writeResource("/fluent_space_bindings", _spaceBindings);
			ofile->writeResource("/scene_graph", _graph);
			ofile->writeResource("/surface_dictionary", _surfaceDictionary);
			ofile->writeResource("/material_links", _materialLinks);
			ofile->writeResource("/fluent_flows", _flows);
			ofile->writeResource("/layout", _layout);
			ofile->writeResource("/cache_tracks", _tracks);

			saveGeometryResources(ofile.get());

			ofile->close();

			delete vSpaceDoc;
			delete vGraphDoc;
			delete vHeaderDoc;
			delete createdAttr;

			return true;
		}
		else {
			DfxGlobal::console::println("Failed to Save Project: Failed to create file.");
			return false;
		}
	}

	return false;
}

bool DfxProject::load(std::string path) {

	if (!path.empty()) {

		std::unique_ptr<xvault::File> ifile(new xvault::h5::h5File);

		if (ifile->open(path, xvault::io::OPEN_READ_ONLY)) {

			int projectFileFormatVer = -1;

			auto iHeader = new xvault::h5::h5IResource();
			auto iFluent = new xvault::h5::h5IResource();
			auto iGraph  = new xvault::h5::h5IResource();
			auto iLayout = new xvault::h5::h5IResource();
			
			xvault::rapid::rjDocument rHeader;
			xvault::rapid::rjDocument rFluent;
			xvault::rapid::rjDocument rGraph;

			char *iHeaderDoc = nullptr;
			char *iFluentDoc = nullptr;
			char *iGraphDoc = nullptr;

			if (ifile->readResource("/header", *iHeader)) {
				bool isDocument = false;
				bool hasBuffer  = false;

				for (auto attr : iHeader->attributes()) {
					if (attr->name() == "__content__") {
						if (attr->type() == xvault::type::STRING) {
							if (attr->valueString() == "document") {
								isDocument = true;
							}
						}
					}
				}

				for (auto buff: iHeader->buffers()) {
					if (buff->name() == ".document") {
						if (buff->type() == xvault::type::CHAR) {
							iHeaderDoc = buff->bufferChar();
							hasBuffer = true;
						}
					}
				}

				if (!isDocument) {
					DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("unknown header format."));
					return false;
				}

				if (!hasBuffer) {
					DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("unknown no header content."));
					return false;
				}
			}
			else {
				DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("Couldn't find header."));
				return false;
			}

			if (ifile->readResource("/fluent_space", *iFluent)) {
				bool isDocument = false;
				bool hasBuffer = false;

				for (auto attr : iFluent->attributes()) {
					if (attr->name() == "__content__") {
						if (attr->type() == xvault::type::STRING) {
							if (attr->valueString() == "document") {
								isDocument = true;
							}
						}
					}
				}

				for (auto buff : iFluent->buffers()) {
					if (buff->name() == ".document") {
						if (buff->type() == xvault::type::CHAR) {
							iFluentDoc = buff->bufferChar();
							hasBuffer = true;
						}
					}
				}

				if (!isDocument) {
					DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("unknown fluent format."));
					return false;
				}

				if (!hasBuffer) {
					DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("unknown no fluent content."));
					return false;
				}
			}
			else {
				DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("Couldn't find header."));
				return false;
			}

			if (ifile->readResource("/scene_graph", *iGraph)) {
				bool isDocument = false;
				bool hasBuffer = false;

				for (auto attr : iGraph->attributes()) {
					if (attr->name() == "__content__") {
						if (attr->type() == xvault::type::STRING) {
							if (attr->valueString() == "document") {
								isDocument = true;
							}
						}
					}
				}

				for (auto buff : iGraph->buffers()) {
					if (buff->name() == ".document") {
						if (buff->type() == xvault::type::CHAR) {
							iGraphDoc = buff->bufferChar();
							hasBuffer = true;
						}
					}
				}

				if (!isDocument) {
					DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("unknown graph format."));
					return false;
				}

				if (!hasBuffer) {
					DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("unknown no graph content."));
					return false;
				}
			}
			else {
				DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("Couldn't find header."));
				return false;
			}

			std::string errorMsg("");
			if (rHeader.fromString(iHeaderDoc, errorMsg)) {
				auto format = ((xvault::rapid::rjNode*)rHeader.root())->findAttributeByName("format");
				auto version = ((xvault::rapid::rjNode*)rHeader.root())->findAttributeByName("version");
				
				int commercialTrialFileFlag = 0;

				auto trialDate = ((xvault::rapid::rjNode*)rHeader.root())->findAttributeByName("trial_date");
				auto trialFile = ((xvault::rapid::rjNode*)rHeader.root())->findAttributeByName("trial_file");
				auto commercialTrialFile = ((xvault::rapid::rjNode*)rHeader.root())->findAttributeByName("commercial_trial_flag");

				if (commercialTrialFile != nullptr) commercialTrialFileFlag = commercialTrialFile->valueUInt();

				if (trialFile != nullptr) {

#ifndef _DEBUG
					if (DeepFxStudioLicenseManager::getState() == DeepFxStudioLicenseManager::NODE_LOCKED_STATE) {
						DfxLicenseInfoDialog::trialCommercialNotCompatible();
						return false;
					}
#endif // !1
					auto now  = QDateTime::currentDateTime();
					auto past = QDateTime::fromString(trialDate->valueString().c_str(), DfxProjectHeader::_dateTimeFormat);

					int daysPast = past.daysTo(now);

#ifndef _DEBUG
					if (daysPast < 0 || daysPast >= 4) {
						DfxLicenseInfoDialog::trialFileExpired();
						return false;
					}
					else {
						DeepFxStudioLicenseManager::setTrialDate(trialDate->valueString());
					}
#endif					
				}
				else {
#ifndef _DEBUG
					
					if (DeepFxStudioLicenseManager::getState() != DeepFxStudioLicenseManager::NODE_LOCKED_STATE && commercialTrialFileFlag != 1010101) {
						DfxLicenseInfoDialog::trialCommercialNotCompatible();
						return false;
					}
#endif			
				}

				if (format != nullptr) {
					if (format->valueString() != "deepfx.studio.proj") {
						DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("Incompatible file format"));
						return false;
					}		
				}
				else {
					DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("Unknown file format"));
					return false;
				}

				if (version != nullptr) {
					projectFileFormatVer = version->valueUInt();
				}
				else {
					DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("Unknown file format"));
					return false;
				}
			}
			else {
				DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("failed to parse header (") + errorMsg + ")");
				return false;
			}

			if (!rFluent.fromString(util::replaceAll(iFluentDoc, "inf", "0.0"), errorMsg)) {
				DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("failed to parse fluent model (") + errorMsg + ")");
				return false;
			}
			
			if (!rGraph.fromString(iGraphDoc, errorMsg)) {
				DfxGlobal::console::println(std::string("Failed to load project: ") + std::string("failed to parse fluent model (") + errorMsg + ")");
				return false;
			}

			if (ifile->readResource("/layout", *iLayout)) {
				if (!iLayout->buffers().empty()) {
					auto buffer = iLayout->buffers()[0];
					DfxViewLayoutManager::switchLayout(std::string(buffer->bufferChar()));
				}
			}

			loadEverythingElse00(ifile.get());

			loadSpace(rFluent.root(), "", nullptr);

			loadFluentsFlows(ifile.get());

			loadSpaceBindings(ifile.get());

			simulationStageList.clear();
			loadGraph(rGraph.root(), nullptr);

			loadSurfaceDictionary(ifile.get());

			loadMaterialLinks(ifile.get());

			loadCacheTracks(ifile.get());

			for (auto stage : simulationStageList) ((DfxSimulationStageBase*)stage)->restoreAll();

			ifile->close();

			delete iHeader;
			delete iFluent;
			delete iGraph;

			emit finishedLoadingNewProject();

			return true;
		}
	}

	emit finishedLoadingNewProject();
	return false;
}

bool DfxProject::updateProject() {
	return false;
}

bool DfxProject::newProject(std::string path) {
	return false;
}

bool DfxProject::isProjectOpen() {
	return !_projectPath.empty();
}

void DfxProject::setProjectPath(std::string path) {
	_projectPath = path;
	emit projectPathChanged();
}

std::string DfxProject::projectPath() {
	return _projectPath;
}
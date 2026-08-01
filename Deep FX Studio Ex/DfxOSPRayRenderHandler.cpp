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

#include "DfxOSPRayRenderHandler.h"
#include "DfxOSPRayRenderNode.h"

std::string DfxOSPRayRenderHandler::model_id = "/RENDER/OSPRAY_RENDER_NODE";
size_t DfxOSPRayRenderHandler::ID_GEN = 1;

DfxOSPRayRenderHandler::DfxOSPRayRenderHandler() {

}

DfxOSPRayRenderHandler::~DfxOSPRayRenderHandler() {

}

fl::Any *DfxOSPRayRenderHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	/// Luxcore Object Reference
	auto ospray = fl::DataObject::Builder()
		.setName("ospray")
		.setLabel("OSPRay Render" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", DfxRenderTarget::LUXCORE)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "engine", "Engine", 2)
		.build(space);


	auto ospray_general = fl::DataObject::Builder()
		.setName("ospray_general")
		.setLabel("OSPRay General Settings")
		.addProperty(space, fl::type::INT, fl::role::NONE, "start_frame", "Start Frame", 0)
		.addProperty(space, fl::type::INT, fl::role::NONE, "end_frame", "End Frame", 120)
		.addProperty(space, fl::type::INT, fl::role::NONE, "frame_step", "Frame Step", 1)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "camera", "Camera Path", std::string(""))
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "print_render_info", "Print Render Info", true)
		
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "denoising", "Denoising", true)

		.addProperty(space, fl::type::INT, fl::role::NONE, "pixel_filter", "Pixel Filter", 2)
		.addProperty(space, fl::type::INT, fl::role::NONE, "pixel_samples", "Pixel Samples", 1)
		.addProperty(space, fl::type::INT, fl::role::NONE, "max_path_length", "Max Path Length", 20)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "min_contribution", "Min Contribution", 0.001f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "variance_threshold", "Variance Threshold", 0.f)

		.build(space);

	auto ospray_path = fl::DataObject::Builder()
		.setName("ospray_path")
		.setLabel("OSPRay Path Tracer Settings")
		.addProperty(space, fl::type::INT, fl::role::NONE, "roulette_path_length", "Roulette Path Length", 5)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "background_refraction", "Background Refraction", false)
		.build(space);

	auto ospray_scivis = fl::DataObject::Builder()
		.setName("ospray_scivis")
		.setLabel("OSPRay SciVis Settings")
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "shadows", "Shadows", false)
		.addProperty(space, fl::type::INT, fl::role::NONE, "aoSamples", "AO Samples", 0)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ao_max_distance", "AO Max Distance", 1020.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "volume_sampling_rate", "Volume Sampling Rate", 1.f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "visible_lights", "Visible Lights", false)
		.build(space);

	auto ospray_ao = fl::DataObject::Builder()
		.setName("ospray_ao")
		.setLabel("OSPRay Ambient Occlusion Settings")
		.addProperty(space, fl::type::INT, fl::role::NONE, "aoSamples", "AO Samples", 1)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ao_max_distance", "AO Max Distance", 1020.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "aoIntensity", "AO Intensity", 1.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "volume_sampling_rate", "Volume Sampling Rate", 1.f)
		.build(space);

	auto ospray_halt_conditions = fl::DataObject::Builder()
		.setName("ospray_halt_conditions")
		.setLabel("OSPRay Halt Conditions")
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "halt_condition", "Halt Condition", 0)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "ipr_halt_condition", "IPR Halt Condition", 0)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "variance", "Variance", 0.1f)
		.addProperty(space, fl::type::INT, fl::role::NONE, "timed_samples", "Timed Samples", 2000)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ipr_variance", "IPR Variance", 0.1f)
		.addProperty(space, fl::type::INT, fl::role::NONE, "ipr_timed_samples", "IPR Timed Samples", 10)
		.build(space);

	auto ospray_tone_mapping = fl::DataObject::Builder()
		.setName("ospray_tone_mapping")
		.setLabel("OSPRay Tone Mapping")
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "enable_tone_mapping", "Enable Tone Mapping", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "aces_color", "ACES Color", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "exposure", "exposure", 1.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "contrast", "contrast", 1.1759f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "shoulder", "shoulder", 0.9746f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "mid_in", "Mid In", 0.18f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "mid_out", "Mid Out", 0.18f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "hdr_max", "HDRi Max", 6.3704f)
		.build(space);

	auto ospray_image_output = fl::DataObject::Builder()
		.setName("ospray_image_output")
		.setLabel("OSPRay Image Output")
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "base_stills_dir", "Stills Directory", std::string(""))
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "base_sequence_dir", "Sequence Directory", std::string(""))
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "prefix", "Image Name Prefix", std::string("render"))
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "naming_convention", "Naming Convention", 0)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "file_format", "File Format", 0)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "save_still_to_disk", "Save Stills To Disk", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "save_sequence_to_disk", "Save Sequence To Disk", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "save_to_memory", "Save To Memory", true)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "create_intermediate_directories", "Create Intermediate Directories", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "save_alpha", "Save Alpha Channel", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "overwrite_existing_file", "Overwrite Existing File", false)
		.build(space);

	space->move(ospray_general, ospray);
	space->move(ospray_path, ospray);
	space->move(ospray_scivis, ospray);
	space->move(ospray_ao, ospray);
	space->move(ospray_image_output, ospray);
	space->move(ospray_halt_conditions, ospray);
	space->move(ospray_tone_mapping, ospray);

	return ospray;
}

bool DfxOSPRayRenderHandler::create() {

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Camera Branch
	auto temp_branch = space->findAbsolute("~/RENDER");

	/// Proceed only if it exists
	if (temp_branch != nullptr) {

		/// Object Reference
		auto object = createModel(space);

		/// Arrange properties into the right order
		space->move(object, temp_branch);

		/// Store reference for further use
		_dataObject = object;

		/// Return success
		return true;
	}

	/// Return failier
	return false;
}

bool DfxOSPRayRenderHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {

		auto nodeAny = _dataObject->data<fl::DataObject>().property("sg_render_node");
		auto node = (DfxOSPRayRenderNode*)_dataObject->data<fl::DataObject>().property("sg_render_node")->rawData<deepsg::SgRenderNode>();

		if (node != nullptr) {
			DfxQSceneManager::getInstance()->removeNode(node);
			delete node;
			nodeAny->nullify();
		}

		space->remove(_dataObject);
		_dataObject = nullptr;

		return true;
	}

	return false;
}

deepsg::SgRenderNode *DfxOSPRayRenderHandler::createNode() {

	if (_dataObject != nullptr) {

		auto any = _dataObject->data<fl::DataObject>().property("sg_render_node");

		if (any != nullptr) {
			return any->rawData<deepsg::SgRenderNode>();
		}
		else {

			/// Master Space
			auto space = DfxSpace::getInstance();

			/// Store Scene graph node
			auto scenegraph_node = space->any("sg_render_node", "Scene Graph Render Node");
			space->move(scenegraph_node, _dataObject);

			return (deepsg::SgRenderNode*)loadNode(_dataObject->absolutePath(), nullptr);
		}
	}

	return nullptr;
}

deepsg::SgNode* DfxOSPRayRenderHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_render_node");

		deepsg::SgRenderNode *gnode = new DfxOSPRayRenderNode();
		gnode->setReferencePath(referencePath);
		((DfxOSPRayRenderNode*)gnode)->initRenderNode();

		scenegraph_node->newData<deepsg::SgRenderNode>(DfxFluentTypes::SG_RENDER_NODE, gnode);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxOSPRayRenderHandler::deleteNode(deepsg::SgNode *node) {
	DfxOSPRayRenderHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}
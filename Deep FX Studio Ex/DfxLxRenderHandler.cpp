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

#include "DfxLxRenderHandler.h"
#include "DfxLuxCoreRenderNode.h"

std::string DfxLxRenderHandler::model_id = "/RENDER/LUXCORE_RENDER_NODE";
size_t DfxLxRenderHandler::ID_GEN = 1;

DfxLxRenderHandler::DfxLxRenderHandler() {

}

DfxLxRenderHandler::~DfxLxRenderHandler() {

}

fl::Any *DfxLxRenderHandler::createModel(fl::Space *space) {

	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	/// Luxcore Object Reference
	auto luxcore = fl::DataObject::Builder()
		.setName("luxcore")
		.setLabel("Luxcore Render" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", DfxRenderTarget::LUXCORE)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "device", "Device", 0)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "engine", "Engine", 0)
		.build(space);

	auto luxcore_general = fl::DataObject::Builder()
		.setName("luxcore_general")
		.setLabel("LuxCoreRender General Settings")
		.addProperty(space, fl::type::INT, fl::role::NONE, "start_frame", "Start Frame", 0)
		.addProperty(space, fl::type::INT, fl::role::NONE, "end_frame", "End Frame", 120)
		.addProperty(space, fl::type::INT, fl::role::NONE, "frame_step", "Frame Step", 1)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "camera", "Camera Path", std::string(""))
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "print_render_info", "Print Render Info", true)
		.build(space);

	auto luxcore_sampling = fl::DataObject::Builder()
		.setName("luxcore_sampling")
		.setLabel("LuxCoreRender Sampling")
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "sampler", "Sampler", 0)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "sobol_adaptive_strength", "Sobol Adaptive Strength", 0.7f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "metropolis_mutation_rate", "Metropolis Large Mutation Probabilty", 0.4f)
		.addProperty(space, fl::type::INT, fl::role::NONE, "metropolis_mutation_rejection_rate", "Metropolis Mutation Rejection Rate", 512)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "metropolis_image_mutation_rate", "Metropolis Image Mutation Rate", 0.1f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "random_adaptive_strength", "Random Adaptive Strength", 0.7f)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "russian_roulette_depth", "Russian Roulette Depth", 3)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "russian_roulette_cap", "Russian Roulette Cap", 0.5f)
		.build(space);

	auto luxcore_path_tracer = fl::DataObject::Builder()
		.setName("luxcore_path")
		.setLabel("LuxCoreRender Path Engine")	
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "max_recursion_depth", "Max Recursion Depth", 6)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "diffuse", "Diffuse", 4)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "glossy", "Glossy", 4)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "specular", "Specular", 6)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "variance_clamp", "Clamping Variance", 0.0f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "force_black_background", "Force Black Background", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "use_light_tracing", "Use Light Tracing", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "light_tracing_glossiness", "Light Tracing Glossiness", 0.05f)
		.build(space);

	auto luxcore_bidir_path_tracer = fl::DataObject::Builder()
		.setName("luxcore_bidir")
		.setLabel("LuxCoreRender Bidirectional Path Engine")
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "max_eye_depth", "Max Eye Depth", 5)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "max_light_depth", "Max Light Depth", 5)
		.build(space);

	auto luxcore_light_strategy = fl::DataObject::Builder()
		.setName("luxcore_light_Strategy")
		.setLabel("LuxCoreRender Light Strategy")
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "type", "Type", 0)
		.build(space);

	auto luxcore_pixel_filter = fl::DataObject::Builder()
		.setName("luxcore_pixel_filter")
		.setLabel("LuxCoreRender Pixel Filter")
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "type", "Type", 0)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "width", "width", 2.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "gaussian_alpha", "Gaussian Filter", 2.0f)
		.build(space);

	auto luxcore_halt_conditions = fl::DataObject::Builder()
		.setName("luxcore_halt_conditions")
		.setLabel("LuxCoreRender Halt Conditions")
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "halt_condition", "Halt Condition", 0)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "ipr_halt_condition", "IPR Halt Condition", 0)
		.addProperty(space, fl::type::INT, fl::role::NONE, "samples", "Samples", 500)
		.addProperty(space, fl::type::INT, fl::role::NONE, "timed_samples", "Timed Samples", 2000)
		.addProperty(space, fl::type::INT, fl::role::NONE, "ipr_samples", "IPR Samples", 100)
		.addProperty(space, fl::type::INT, fl::role::NONE, "ipr_timed_samples", "IPR Timed Samples", 10)
		.build(space);

	auto luxcore_refresh_interval = fl::DataObject::Builder()
		.setName("luxcore_refresh_interval")
		.setLabel("LuxCoreRender Refresh Interval")
		.addProperty(space, fl::type::INT, fl::role::NONE, "refresh_interval", "Refresh Interval", 10)
		.addProperty(space, fl::type::INT, fl::role::NONE, "ipr_refresh_interval", "IPR Refresh Interval", 20)
		.build(space);

	auto luxcore_denoising = fl::DataObject::Builder()
		.setName("luxcore_denoising")
		.setLabel("LuxCoreRender Denoising")
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "enable", "Enable", false)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "type", "Denoiser", 0)
		.addProperty(space, fl::type::INT, fl::role::NONE, "oidn_max_memory", "OIDN Max Memory", 6000)
		.build(space);

	auto luxcore_photon_gi_cache = fl::DataObject::Builder()
		.setName("luxcore_photon_gi_cache")
		.setLabel("LuxCoreRender Photon GI Cache")
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "enable", "Enable", false)
		.addProperty(space, fl::type::INT, fl::role::NONE, "photon_count", "Photon Count", 20)
		.addProperty(space, fl::type::INT, fl::role::NONE, "photon_depth", "Photon Depth", 8)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "enable_indirect_light_cache", "Enable Indirect Light Cache", false)
		.addProperty(space, fl::type::INT, fl::role::NONE, "indirect_halt_threshold", "Indirect Halt Threshold", 5)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "indirect_brute_force_radius_scale", "Indirect Brute Force Radius Scale", 8.0f)
		.addProperty(space, fl::type::INT, fl::role::NONE, "indirect_normal_angle", "Indirect Normal Angle", 10)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "indirect_glossiness_threshold", "Indirect Glossiness Threshold", 0.05f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "indirect_auto_lookup_radius", "Indirect Automatic Look Up Radius", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "indirect_lookup_radius", "Indirect Look Up Radius", 0.15f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "enable_caustic_light_cache", "Enable Caustic Light Cache", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "caustic_merge_photons", "Caustic Merge Photons", true)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "caustic_periodic_update", "Caustic Periodic Update", true)
		.addProperty(space, fl::type::INT, fl::role::NONE, "caustic_max_size", "Caustic Max Size", 1)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "caustic_lookup_radius", "Caustic Lookup Radius", 0.075f)
		.addProperty(space, fl::type::INT, fl::role::NONE, "caustic_normal_angle", "Caustic Normal Angle", 10)
		.addProperty(space, fl::type::INT, fl::role::NONE, "caustic_step_samples", "Caustic Step Samples", 8)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "caustic_step_radius_reduction", "Caustic Step Radius Reduction", 0.93f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "caustic_step_minimum_radius", "Caustic Step Minimum Radius", 0.003f)

		// Deprecated properties, reserved for backwards compatibilty
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "caustic_radius_Scale", "Caustic Radius Scale", 0.25f)
		.addProperty(space, fl::type::INT, fl::role::NONE, "caustic_lookup_max_count", "Caustic Lookup Max Count", 123)


		.build(space);

	auto luxcore_enviroment_light_cache = fl::DataObject::Builder()
		.setName("luxcore_enviroment_light_cache")
		.setLabel("LuxCoreRender Environment Light Cache")
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "enable", "Enable", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "quality", "Quality", 0.5f)
		.addProperty(space, fl::type::INT, fl::role::NONE, "map_width", "Map Width", 256)
		.addProperty(space, fl::type::INT, fl::role::NONE, "samples", "samples", 1)
		.build(space);

	auto luxcore_direct_light_cache = fl::DataObject::Builder()
		.setName("luxcore_direct_light_cache")
		.setLabel("LuxCoreRender Direct Light Cache")
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "enable", "Enable", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "automatic_entry_radius", "Automatic Entry Radius", true)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "entry_radius", "Entry Radius", 0.15f)
		.addProperty(space, fl::type::INT, fl::role::NONE, "warmup_samples", "Warmup Samples", 12)
		.addProperty(space, fl::type::INT, fl::role::NONE, "normal_angle", "Normal Angle", 10)
		.addProperty(space, fl::type::INT, fl::role::NONE, "max_passes", "Max Passes", 1024)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "convergence_threshold", "Convergence Threshold", 1.0f)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "place_entries_in_volumes", "Entries In Volume", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "light_threshold", "Light Threshold", 1.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "target_cache_hit_ratio", "Cache Hit Ratio", 99.5f)
		.addProperty(space, fl::type::INT, fl::role::NONE, "max_depth", "Max depth", 4)
		.addProperty(space, fl::type::INT, fl::role::NONE, "max_samples", "Max Samples", 10000000)
		.build(space);

	auto luxcore_aovs = fl::DataObject::Builder()
		.setName("luxcore_aovs")
		.setLabel("LuxCoreRender AOVS")
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "rgb_imagepipeline", "RGB Image Pipeline", true)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "rgba_imagepipeline", "RGBA Image Pipeline", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "rgb", "RGB", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "rgba", "RGBA", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "alpha", "Alpha", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "depth", "Depth", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "position", "Position", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "geometry_normal", "Geometry Normal", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "shading_normal", "Shading Normal", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "material_id", "Materials", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "direct_diffuse", "Direct Diffuse", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "direct_glossy", "Direct Glossy", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "indirect_diffuse", "Indirect Diffuse", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "indirect_glossy", "Indirect Glossy", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "indirect_specular", "Indirect Specular", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "material_id_mask", "Material ID Mask", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "direct_shadow_mask", "Direct Shadow Mask", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "indirect_shadow_mask", "Indirect Shadow Mask", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "radiance_group", "Radiance Group", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "uv", "UV", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "ray_count", "Ray Count", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "by_material_id", "By Material Id", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "irradiance", "Irrandiance", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "object_id", "Object Id", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "object_id_mask", "Object id Mask", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "by_object_id", "By Object Id", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "samplecount", "Sample Count", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "convergence", "Convergence", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "noise", "Noise", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "serialized_film", "Serialized Film", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "emission", "Emission", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "material_id_color", "Material Id Color", false)
		.build(space);

	auto luxcore_image_pipeline = fl::DataObject::Builder()
		.setName("luxcore_image_pipeline")
		.setLabel("LuxCoreRender Image Pipeline")
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "tonemap_enable", "Enable Tone Mapper", true)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "tonemap_type", "Tone Mapper Type", 0)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "tonemap_lin_auto", "Tone Mapper Auto Brightness", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "tonemap_lin_gain", "Tone Mapper Linear Gain", 0.5f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "tonemap_camera_fstop", "Tone Mapper Camera Settings F-Stop", 2.8f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "tonemap_camera_shutter", "Tone Mapper Camera Settings Shutter", 0.01f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "tonemap_camera_iso", "Tone Mapper Camara Settings ISO ", 100.f)

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "tonemap_rein_pre", "Tone Mapper Reinhard Pre", 1.f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "tonemap_rein_post", "Tone Mapper Reinhard Post", 1.2f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "tonemap_rein_burn", "Tone Mapper Reinhard Burn", 6.0f)

		.addProperty(space, fl::type::BOOL, fl::role::NONE, "bloom_enable", "Enable Bloom", false)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "bloom_radius", "Bloom Radius", 0.07f)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "bloom_strength", "Bloom Strength", .25f)
		.build(space);

	auto luxcore_image_output = fl::DataObject::Builder()
		.setName("luxcore_image_output")
		.setLabel("LuxCoreRender Image Output")
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "base_stills_dir", "Stills Directory", std::string(""))
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "base_sequence_dir", "Sequence Directory", std::string(""))
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "prefix", "Image Name Prefix", std::string("render"))
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "naming_convention", "Naming Convention", 0)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "file_format", "File Format", 0)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "save_still_to_disk", "Save Stills To Disk", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "save_sequence_to_disk", "Save Sequence To Disk", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "save_to_memory", "Save To Memory", true)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "create_intermediate_directories", "Create Intermediate Directories", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "save_aov_in_seperate_folder", "Save AOV in Seperate Directory", false)
		.addProperty(space, fl::type::BOOL, fl::role::NONE, "overwrite_existing_file", "Overwrite Existing File", false)
		.build(space);

	space->move(luxcore_general, luxcore);
	space->move(luxcore_path_tracer, luxcore);
	space->move(luxcore_bidir_path_tracer, luxcore);
	space->move(luxcore_light_strategy, luxcore);
	space->move(luxcore_sampling, luxcore);
	space->move(luxcore_halt_conditions, luxcore);
	space->move(luxcore_pixel_filter, luxcore);
	space->move(luxcore_refresh_interval, luxcore);
	space->move(luxcore_aovs, luxcore);
	space->move(luxcore_denoising, luxcore);
	space->move(luxcore_photon_gi_cache, luxcore);
	space->move(luxcore_enviroment_light_cache, luxcore);
	space->move(luxcore_direct_light_cache, luxcore);
	space->move(luxcore_image_pipeline, luxcore);
	space->move(luxcore_image_output, luxcore);

	return luxcore;
}

bool DfxLxRenderHandler::create() {

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

bool DfxLxRenderHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {

		auto nodeAny = _dataObject->data<fl::DataObject>().property("sg_render_node");
		DfxLuxCoreRenderNode *node = (DfxLuxCoreRenderNode*)_dataObject->data<fl::DataObject>().property("sg_render_node")->rawData<deepsg::SgRenderNode>();

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

deepsg::SgRenderNode *DfxLxRenderHandler::createNode() {

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

deepsg::SgNode* DfxLxRenderHandler::loadNode(std::string referencePath, deepsg::SgNode *parent) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath);

	if (any != nullptr) {

		auto scenegraph_node = any->data<fl::DataObject>().property("sg_render_node");

		deepsg::SgRenderNode *gnode = new DfxLuxCoreRenderNode();
		gnode->setReferencePath(referencePath);
		((DfxLuxCoreRenderNode*)gnode)->initRenderNode();

		scenegraph_node->newData<deepsg::SgRenderNode>(DfxFluentTypes::SG_RENDER_NODE, gnode);

		if (parent != nullptr) {
			DfxQSceneManager::getInstance()->moveNode(gnode, parent);
		}

		return gnode;
	}

	return nullptr;
}

void DfxLxRenderHandler::deleteNode(deepsg::SgNode *node) {
	DfxLxRenderHandler handler;

	if (handler.fetch(node->referencePath())) {
		handler.destroy();
	}
}
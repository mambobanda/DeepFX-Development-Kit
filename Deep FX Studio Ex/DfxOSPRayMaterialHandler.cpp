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

#include <random>
#include "DfxOSPRayMaterialHandler.h"

std::string DfxOSPRayMaterialHandler::model_id = "/MATERIAL/OSPRAY_MATERIAL_INFO";
size_t DfxOSPRayMaterialHandler::ID_GEN = 1;

DfxOSPRayMaterialHandler::DfxOSPRayMaterialHandler() {

}

DfxOSPRayMaterialHandler::~DfxOSPRayMaterialHandler() {

}

bool DfxOSPRayMaterialHandler::fetch(std::string path) {
	return true;
}

fl::Any *DfxOSPRayMaterialHandler::dataObject() {
	return _dataObject;
}

fl::Any *DfxOSPRayMaterialHandler::createModel(fl::Space *space) {
	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	std::minstd_rand gen(std::random_device{}());
	std::uniform_int_distribution<int> dist(50000, 100000);

	return fl::DataObject::Builder()
		.setName("material")
		.setHasModel(true)
		.setModelReference(model_id)
		.setLabel("Material" + str_id)
		.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", DfxRenderTarget::OSPRAY)
		.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("OSPRAY_OUTPUT"))
		.addProperty(space, fl::type::INT, fl::role::NONE, "material_id", "Material ID", dist(gen))

		//.addProperty(space, fl::type::BOOL, fl::role::HIDDEN_INPUT, "smooth_shading_override", "Override Smooth Shading", false)
		//.addProperty(space, fl::type::BOOL, fl::role::HIDDEN_INPUT, "use_smooth_shading", "Use Smooth Shading", false)

		.addProperty(space, fl::type::INT, fl::role::INPUT, "material_output", "Material", std::string(""))

		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Positon", 0)
		.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Positon", 0)

		.setProtected(true)
		.build(space);
}

bool DfxOSPRayMaterialHandler::create() {

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Camera Branch
	auto mat_branch = space->findAbsolute("~/MATS");

	/// Proceed only if it exists
	if (mat_branch != nullptr) {

		size_t id_g = ID_GEN++;
		std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

		auto object = createModel(space);

		space->move(object, mat_branch);

		/// Store reference for further use
		_dataObject = object;

		/// Return success
		return true;
	}

	return false;
}

bool DfxOSPRayMaterialHandler::destroy() {
	return true;
}

bool DfxOSPRayMaterialHandler::clearAllMaterials() {
	return true;
}
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

#include "DfxOSPRayMaterials.h"

namespace ospmat {

	// Materials
	fl::Any *PrincipledModel::createModel(fl::Space *space) {
		return principled(space);
	}

	fl::Any *PrincipledModel::principled(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("principled")
			.setLabel("Principled")
			.setHasModel(true)
			.setModelReference(principledID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::OSPRAY)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("OSPRAY_PRINCIPLED_MAT"))

			.addProperty(space, fl::type::INT, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "base_color", "Base Color", std::string("255:255:255"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "edge_color", "Edge Color", std::string("255:255:255"))
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "metallic", "Metallic", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "diffuse", "Diffuse", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "specular", "Specular", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "ior", "IOR", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "transmission", "Transmission",0.f)
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "transmission_color", "Transmission Color", std::string("255:255:255"))
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "transmission_depth", "transmission Depth", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "roughness", "Roughness", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "anisotropy", "Anisotropy", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "rotation", "Rotation", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "normal", "Normal", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "base_normal", "Base Normal", 1.f)
			.addProperty(space, fl::type::BOOL, fl::role::INPUT, "thin", "Thin", false)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "thickness", "Thickness", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "backlight", "Backlight", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "coat", "coat", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "coat_ior", "Coat IOR", 1.5f)
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "coat_color", "Coat Color", std::string("255:255:255"))
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "coat_thickness", "Coat Thickness", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "coat_roughness", "Coat Roughness", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "coat_normal", "Coat Normal", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "sheen", "Sheen", 0.f)
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "sheen_color", "Sheen Color", std::string("255:255:255"))
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "shin_tint", "Shin Tint", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "sheen_roughness", "Sheen Roughness", 0.2f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "opacity", "Opacity", 1.f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "base_r", "Base Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "base_g", "Base Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "base_b", "Base Blue", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "edge_r", "Edge Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "edge_g", "Edge Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "edge_b", "Edge Blue", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_r", "Transmission Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_g", "Transmission Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_b", "Transmission Blue", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "coat_r", "Coat Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "coat_g", "Coat Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "coat_b", "Coat Blue", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "sheen_r", "Diffuse Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "sheen_g", "Diffuse Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "sheen_b", "Diffuse Blue", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Textures
	fl::Any *ImageTextureModel::createModel(fl::Space *space) {
		return imageTexture(space);
	}

	fl::Any *ImageTextureModel::imageTexture(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("image_texture")
			.setLabel("Image Texture")
			.setHasModel(true)
			.setModelReference(imageTextureID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::OSPRAY)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("OSPRAY_IMAGE_TEXTURE"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "file", "File", std::string(""))
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "uv_rotation", "UV Rotation", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "uv_translation_x", "UV Translation X", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "uv_translation_y", "UV Translation Y", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "uv_scale_x", "UV Scale X", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "uv_scale_y", "UV Scale Y", 1.f)

			.build(space);

		return object;
	}

}
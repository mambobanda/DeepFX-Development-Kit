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

#include <Qfile>
#include <QImage>
#include <DfxGlobal.h>
#include "DfxOSPRayMaterialBuilder.h"


DfxOSPRayMaterialBuilder::DfxOSPRayMaterialBuilder() {

}

DfxOSPRayMaterialBuilder::~DfxOSPRayMaterialBuilder() {
	for (auto mat: _materialMap) {
		delete mat.second;
	}

	for (auto &tex : _textureMap) {
		delete tex.second.tex;
	}

	_textureMap.clear();
	_materialMap.clear();
}

void setMaterialTexture() {

}

void DfxOSPRayMaterialBuilder::buildPrincipledMat(std::string path, fl::DataObject *dao) {
	auto principled = new ospray::cpp::Material("", "principled");

	setMaterialColorParameter<float>("baseColor", "base_color", "base_r", "base_g", "base_b", dao, principled);
	setMaterialColorParameter<float>("edgeColor", "edge_color", "edge_r", "edge_g", "edge_b", dao, principled);
	setMaterialParameter<float>("metallic", "metallic", dao, principled);
	setMaterialParameter<float>("diffuse", "diffuse", dao, principled);
	setMaterialParameter<float>("specular", "specular", dao, principled);
	setMaterialParameter<float>("ior", "ior", dao, principled);
	setMaterialParameter<float>("transmission", "transmission", dao, principled);
	setMaterialColorParameter<float>("transmissionColor", "transmission_color", "transmission_r", "transmission_g", "transmission_b", dao, principled);
	setMaterialParameter<float>("transmissionDepth", "transmission_depth", dao, principled);
	setMaterialParameter<float>("roughness", "roughness", dao, principled);
	setMaterialParameter<float>("anisotropy", "anisotropy", dao, principled);
	setMaterialParameter<float>("rotation", "rotation", dao, principled);
	setMaterialParameter<float>("normal", "normal", dao, principled);
	setMaterialParameter<float>("baseNormal", "base_normal", dao, principled);
	setMaterialParameter<bool>("thin", "thin", dao, principled);
	setMaterialParameter<float>("thickness", "thickness", dao, principled);
	setMaterialParameter<float>("backlight", "backlight", dao, principled);
	setMaterialParameter<float>("coat", "coat", dao, principled);
	setMaterialParameter<float>("coatIor", "coat_ior", dao, principled);
	setMaterialColorParameter<float>("coatColor", "coat_color", "coat_r", "coat_g", "coat_b", dao, principled);
	setMaterialParameter<float>("coatThickness", "coat_thickness", dao, principled);
	setMaterialParameter<float>("coatRoughness", "coat_roughness", dao, principled);
	setMaterialParameter<float>("coatNormal", "coat_normal", dao, principled);
	setMaterialParameter<float>("sheen", "sheen", dao, principled);
	setMaterialColorParameter<float>("sheenColor", "sheen_color", "sheen_r", "sheen_g", "sheen_b", dao, principled);
	setMaterialParameter<float>("sheenTint", "shin_tint", dao, principled);
	setMaterialParameter<float>("sheenRoughness", "sheen_roughness", dao, principled);
	setMaterialParameter<float>("opacity", "opacity", dao, principled);

	/*.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "base_color", "Base Color", std::string("255:255:255"))
		.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "edge_color", "Edge Color", std::string("255:255:255"))
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "metallic", "Metallic", 0.f)
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "diffuse", "Diffuse", 1.f)
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "specular", "Specular", 1.f)
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "ior", "IOR", 1.f)
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "transmission", "Transmission", 0.f)
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
		.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "opacity", "Opacity", 1.f)*/

	principled->commit();

	_materialMap[path] = principled;
}

void DfxOSPRayMaterialBuilder::build() {
	auto space = DfxSpace::getInstance();

	auto any = space->findAbsolute("~/MATS");

	if (any != nullptr) {

		// Load Textures
		for (auto material : any->data<fl::DataObject>().properties()) {

			if (material->type() == fl::type::DATA_OBJECT) {
				auto matObj = material->rawData<fl::DataObject>();


				if (matObj->hasProperty("render_target")) {
					if (matObj->property("render_target")->data<unsigned int>() == DfxRenderTarget::OSPRAY) {

						for (auto node : matObj->properties()) {
							if (node->type() == fl::type::DATA_OBJECT) {
								auto nodeDao = node->rawData<fl::DataObject>();

								if (nodeDao->hasProperty("uid")) {
									if (nodeDao->property("uid")->data<std::string>() == "OSPRAY_IMAGE_TEXTURE") {
										loadTexture(node->absolutePath(), nodeDao);
									}
								}
							}
						}
					}
				}
			}
		}

		for (auto material : any->data<fl::DataObject>().properties()) {

			if (material->type() == fl::type::DATA_OBJECT) {
				auto matObj = material->rawData<fl::DataObject>();

				if (matObj->hasProperty("render_target")) {
					if (matObj->property("render_target")->data<unsigned int>() == DfxRenderTarget::OSPRAY) {

						if (matObj->hasProperty("material_output")) {
							if (space->hasBinding(matObj->property("material_output")->absolutePath())) {

								auto lnks = space->findLinks(matObj->property("material_output")->absolutePath());
								if (!lnks.empty()) {
									auto slt = space->findAbsolute(lnks[0].output_path);
									if (slt != nullptr) {
										auto pNode = slt->parent();

										if (pNode->type() == fl::type::DATA_OBJECT) {
											auto pdao = pNode->rawData<fl::DataObject>();

											if (pdao->hasProperty("uid")) {
												if (pdao->property("uid")->data<std::string>() == "OSPRAY_PRINCIPLED_MAT") {
													buildPrincipledMat(material->absolutePath(), pdao);
												}
											}
											
										}
										
									}
								}
						
							}
						}
					}
				}
			}
		}
	}
}

void DfxOSPRayMaterialBuilder::loadTexture(std::string node_path, fl::DataObject *dao) {

	if (dao->hasProperty("uid")) {
		if (dao->property("uid")->data<std::string>() == "OSPRAY_IMAGE_TEXTURE") {

			auto path = dao->property("file")->data<std::string>();
			auto uv_rotation = dao->property("uv_rotation")->data<float>();
			auto uv_translation_x = dao->property("uv_translation_x")->data<float>();
			auto uv_translation_y = dao->property("uv_translation_y")->data<float>();
			auto uv_scale_x = dao->property("uv_scale_x")->data<float>();
			auto uv_scale_y = dao->property("uv_scale_y")->data<float>();

			if (!path.empty()) {
				if (filesystem::exists(path)) {
					QImage image;

					if (image.load(path.c_str())) {
						
						image = image.convertToFormat(QImage::Format_RGBA8888);
				
						TextureDefintion tdef;
						
						auto tex = new ospray::cpp::Texture("texture2d");
						tex->setParam("format", OSP_TEXTURE_RGBA8);
						tex->setParam("filter", OSP_TEXTURE_FILTER_BILINEAR);
						tex->setParam("data", ospray::cpp::CopiedData((uint32_t*)image.bits(), rkcommon::math::vec2ul(image.width(), image.height())));
						tex->commit();

						tdef.tex = tex;
						tdef.uv_rotation = uv_rotation;
						tdef.uv_translation_x = uv_translation_x;
						tdef.uv_translation_y = uv_translation_y;
						tdef.uv_scale_x = uv_scale_x;
						tdef.uv_scale_y = uv_scale_y;

						_textureMap[node_path] = tdef;
					}
					else {
						DfxGlobal::console::println("Failed to load texture: " + path);
					}

				}
				else {
					DfxGlobal::console::println("Failed to load texture: " + path);
				}
			}
		}
	}
}

ospray::cpp::Material *DfxOSPRayMaterialBuilder::material(std::string path) {
	auto mat = _materialMap.find(path);

	if (mat != _materialMap.end()) {
		return mat->second;
	}

	return nullptr;
}

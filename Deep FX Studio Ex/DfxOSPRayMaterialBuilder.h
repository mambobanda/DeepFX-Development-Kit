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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <FlCommon.h>
#include <DfxSpace.h>
#include <ospray/ospray_util.h>
#include <ospray/ospray_cpp.h>
#include <ospray/ospray_cpp/ext/rkcommon.h>
#include <unordered_map>
#include <filesystem>
#include <DfxRenderTarget.h>

namespace filesystem = std::experimental::filesystem;

class DfxOSPRayMaterialBuilder {
private:

	struct TextureDefintion {
		ospray::cpp::Texture *tex;
		float uv_rotation;
		float uv_translation_x;
		float uv_translation_y;
		float uv_scale_x;
		float uv_scale_y;
	};
private:

	std::unordered_map<std::string, ospray::cpp::Material*> _materialMap;
	std::unordered_map<std::string, TextureDefintion> _textureMap;

private:
	void loadTexture(std::string path, fl::DataObject *dao);
	void buildPrincipledMat(std::string path, fl::DataObject *dao);
public:

	DfxOSPRayMaterialBuilder();
	virtual ~DfxOSPRayMaterialBuilder();

	void build();
	ospray::cpp::Material *material(std::string path);

private:

	template<typename T>
	void setMaterialParameter(std::string osp_param_name, std::string fl_prop_name, fl::DataObject *dao, ospray::cpp::Material *material) {

		auto lnks = DfxSpace::getInstance()->findLinks(dao->property(fl_prop_name)->absolutePath());
		if (!lnks.empty()) {
			auto slt = DfxSpace::getInstance()->findAbsolute(lnks[0].output_path);
			auto sltParent = slt->parent();

			auto texItr = _textureMap.find(sltParent->absolutePath());

			if (texItr != _textureMap.end()) {
				material->setParam("map_" + osp_param_name, *texItr->second.tex);
				material->setParam("map_" + osp_param_name + ".rotation", texItr->second.uv_rotation);
				material->setParam("map_" + osp_param_name + ".scale", rkcommon::math::vec2f(texItr->second.uv_scale_x, texItr->second.uv_scale_y));
				material->setParam("map_" + osp_param_name + ".translation", rkcommon::math::vec2f(texItr->second.uv_translation_x, texItr->second.uv_translation_y));

				return;
			}
		}
		
		material->setParam(osp_param_name, dao->property(fl_prop_name)->data<T>());
	}

	template<typename T>
	void setMaterialColorParameter(std::string osp_param_name, std::string fl_prop_name, 
		std::string red, std::string green, std::string blue, fl::DataObject *dao, ospray::cpp::Material *material) {
		
		auto lnks = DfxSpace::getInstance()->findLinks(dao->property(fl_prop_name)->absolutePath());
		if (!lnks.empty()) {
			auto slt = DfxSpace::getInstance()->findAbsolute(lnks[0].output_path);
			auto sltParent = slt->parent();

			auto texItr = _textureMap.find(sltParent->absolutePath());

			if (texItr != _textureMap.end()) {
				material->setParam("map_" + osp_param_name, *texItr->second.tex);
				material->setParam("map_" + osp_param_name + ".rotation", texItr->second.uv_rotation);
				material->setParam("map_" + osp_param_name + ".scale", rkcommon::math::vec2f(texItr->second.uv_scale_x, texItr->second.uv_scale_y));
				material->setParam("map_" + osp_param_name + ".translation", rkcommon::math::vec2f(texItr->second.uv_translation_x, texItr->second.uv_translation_y));
				
				return;
			}
		}

		auto color = rkcommon::math::vec3f
		(
			dao->property(red)->data<T>(),
			dao->property(green)->data<T>(),
			dao->property(blue)->data<T>()
		);

		material->setParam(osp_param_name, color);
	}
};
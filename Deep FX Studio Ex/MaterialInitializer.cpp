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

#include "MaterialsInitializer.h"

namespace materials {
	void initializeMaterialMap() {

		// Luxcore Materials
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::nullID, luxmat::NullModel::null);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::matteID, luxmat::MatteModel::matte);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::matteTranslucentID, luxmat::MatteTranslucentModel::matteTranslucent);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::emissionID, luxmat::EmissionModel::emission);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::disneyID, luxmat::DisneyModel::disney);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::mixID, luxmat::MixModel::mix_material);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::glassID, luxmat::GlassModel::glass);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::archGlassID, luxmat::ArchGlassModel::archGlass);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::roughGlassID, luxmat::RoughGlassModel::roughGlass);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::carPaintID, luxmat::CarPaintModel::carPaint);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::clothID, luxmat::ClothModel::cloth);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::glossyID, luxmat::GlossyModel::glossy);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::glossyTranslucentID, luxmat::GlossyTranslucentModel::glossyTranslucent);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::metalID, luxmat::MetalModel::metal);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::mirrorID, luxmat::MirrorModel::mirror);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::velvetID, luxmat::VelvetModel::velvet);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::roughMatteID, luxmat::RoughMatteModel::roughMatte);

		// Luxcore Texture Materials
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::imagemapID, luxmat::ImagemapModel::imagemap);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::normalmapID, luxmat::NormalmapModel::normalmap);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::checkerboard2dID, luxmat::CheckerboardModel::checkerboard_2d);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::fbmID, luxmat::FbmModel::fbm);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::fresnelColorID, luxmat::FresnelColorModel::fresnelColor);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::fresnelSopraID, luxmat::FresnelSopraModel::fresnelSopra);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::blendID, luxmat::BlendModel::blend);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::cloudID, luxmat::CloudModel::cloud);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::distortedNoiseID, luxmat::DistortedNoiseModel::distortedNoise);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::magicID, luxmat::MagicModel::magic);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::marbleID, luxmat::MarbleModel::marble);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::musgraveID, luxmat::MusgraveModel::musgrave);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::noiseID, luxmat::NoiseModel::noise);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::stucciID, luxmat::StucciModel::stucci);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::woodID, luxmat::WoodModel::wood);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::voronoiID, luxmat::VoronoiModel::voronoi);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::fresnel01ID, luxmat::Fresnel01Model::fresnel01);

		// Luxcore Utility 
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::mixTexID, luxmat::MixTexModel::mixTex);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::clampID, luxmat::ClampModel::clamp);
		DfxMaterialManager::getInstance()->addMaterialInstance(luxmat::hsvID, luxmat::HsvModel::hsv);

		// OSPRay Materials
		DfxMaterialManager::getInstance()->addMaterialInstance(ospmat::principledID, ospmat::PrincipledModel::principled);

		DfxMaterialManager::getInstance()->addMaterialInstance(ospmat::imageTextureID, ospmat::ImageTextureModel::imageTexture);

	}
}
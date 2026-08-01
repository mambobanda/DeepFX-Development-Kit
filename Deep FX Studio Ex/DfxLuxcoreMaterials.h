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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <FlModel.h>
#include "DfxAny.h"
#include "DfxSpace.h"
#include "DfxRenderTarget.h"


namespace luxmat {

	// Material
	static const std::string nullID = "MATERIAL/LUXCORE_NULL_MAT";
	class NullModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *null(fl::Space *space);
	};

	static const std::string matteID = "MATERIAL/LUXCORE_MATTE_MAT";
	class MatteModel: public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *matte(fl::Space *space);
	};

	static const std::string matteTranslucentID = "MATERIAL/LUXCORE_MATTE_TRANSLUCENT_MAT";
	class MatteTranslucentModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *matteTranslucent(fl::Space *space);
	};

	static const std::string emissionID = "MATERIAL/LUXCORE_EMISSION_MAT";
	class EmissionModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *emission(fl::Space *space);
	};

	static const std::string disneyID = "MATERIAL/LUXCORE_DISNEY_MAT";
	class DisneyModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *disney(fl::Space *space);
	};

	static const std::string mixID = "MATERIAL/LUXCORE_MIX_MAT";
	class MixModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *mix_material(fl::Space *space);
	};

	static const std::string glassID = "MATERIAL/LUXCORE_GLASS_MAT";
	class GlassModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *glass(fl::Space *space);
	};

	static const std::string archGlassID = "MATERIAL/LUXCORE_ARCH_GLASS_MAT";
	class ArchGlassModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *archGlass(fl::Space *space);
	};

	static const std::string roughGlassID = "MATERIAL/LUXCORE_ROUGH_GLASS_MAT";
	class RoughGlassModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *roughGlass(fl::Space *space);
	};

	static const std::string carPaintID = "MATERIAL/LUXCORE_CAR_PAINT_MAT";
	class CarPaintModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *carPaint(fl::Space *space);
	};

	static const std::string clothID = "MATERIAL/LUXCORE_CLOTH_MAT";
	class ClothModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *cloth(fl::Space *space);
	};

	static const std::string glossyID = "MATERIAL/LUXCORE_GLOSSY_MAT";
	class GlossyModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *glossy(fl::Space *space);
	};

	static const std::string glossyTranslucentID = "MATERIAL/LUXCORE_GLOSSY_TRANSLUCENT_MAT";
	class GlossyTranslucentModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *glossyTranslucent(fl::Space *space);
	};

	static const std::string metalID = "MATERIAL/LUXCORE_METAL_MAT";
	class MetalModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *metal(fl::Space *space);
	};

	static const std::string mirrorID = "MATERIAL/LUXCORE_MIRROR_MAT";
	class MirrorModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *mirror(fl::Space *space);
	};

	static const std::string velvetID = "MATERIAL/LUXCORE_VELVET_MAT";
	class VelvetModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *velvet(fl::Space *space);
	};

	static const std::string roughMatteID = "MATERIAL/LUXCORE_ROUGH_MATTE_MAT";
	class RoughMatteModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *roughMatte(fl::Space *space);
	};


	// Texture Material
	static const std::string checkerboard2dID = "MATERIAL/LUXCORE_CHECKERBOARD_TEX";
	class CheckerboardModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *checkerboard_2d(fl::Space *space);
	};

	static const std::string fbmID = "MATERIAL/LUXCORE_FBM_TEX";
	class FbmModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *fbm(fl::Space *space);
	};

	static const std::string imagemapID = "MATERIAL/LUXCORE_IMAGEMAP_MAT";
	class ImagemapModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *imagemap(fl::Space *space);
	};

	static const std::string normalmapID = "MATERIAL/LUXCORE_NORMALMAP_MAT";
	class NormalmapModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *normalmap(fl::Space *space);
	};

	static const std::string fresnelColorID = "MATERIAL/LUXCORE_FRESNEL_TEX";
	class FresnelColorModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *fresnelColor(fl::Space *space);
	};

	static const std::string fresnelSopraID = "MATERIAL/LUXCORE_SOPRA_TEX";
	class FresnelSopraModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *fresnelSopra(fl::Space *space);
	};

	static const std::string blendID = "MATERIAL/LUXCORE_BLEND_TEX";
	class BlendModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *blend(fl::Space *space);
	};

	static const std::string cloudID = "MATERIAL/LUXCORE_CLOUD_TEX";
	class CloudModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *cloud(fl::Space *space);
	};

	static const std::string distortedNoiseID = "MATERIAL/LUXCORE_DISTORTED_NOISE_TEX";
	class DistortedNoiseModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *distortedNoise(fl::Space *space);
	};

	static const std::string magicID = "MATERIAL/LUXCORE_MAGIC_TEX";
	class MagicModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *magic(fl::Space *space);
	};

	static const std::string marbleID = "MATERIAL/LUXCORE_MARBLE_TEX";
	class MarbleModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *marble(fl::Space *space);
	};

	static const std::string musgraveID = "MATERIAL/LUXCORE_MUSGRAVE_TEX";
	class MusgraveModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *musgrave(fl::Space *space);
	};

	static const std::string noiseID = "MATERIAL/LUXCORE_NOISE_TEX";
	class NoiseModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *noise(fl::Space *space);
	};

	static const std::string stucciID = "MATERIAL/LUXCORE_STUCCI_TEX";
	class StucciModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *stucci(fl::Space *space);
	};

	static const std::string woodID = "MATERIAL/LUXCORE_WOOD_TEX";
	class WoodModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *wood(fl::Space *space);
	};

	static const std::string voronoiID = "MATERIAL/LUXCORE_VORONOI_TEX";
	class VoronoiModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *voronoi(fl::Space *space);
	};

	static const std::string fresnel01ID = "MATERIAL/LUXCORE_FRESNEL01_TEX";
	class Fresnel01Model : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *fresnel01(fl::Space *space);
	};

	/*static const std::string fresnel02ID = "MATERIAL/LUXCORE_FRESNEL02_TEX";
	class Fresnel02Model : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *fresnel02(fl::Space *space);
	};*/

	// Texture Util
	static const std::string mixTexID = "MATERIAL/LUXCORE_MIX_TEX";
	class MixTexModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *mixTex(fl::Space *space);
	};

	static const std::string clampID = "MATERIAL/LUXCORE_CLAMP_TEX";
	class ClampModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *clamp(fl::Space *space);
	};

	static const std::string hsvID = "MATERIAL/LUXCORE_HSV_TEX";
	class HsvModel : public fl::Model {
	public:
		fl::Any *createModel(fl::Space *) override;
		static fl::Any *hsv(fl::Space *space);
	};
}
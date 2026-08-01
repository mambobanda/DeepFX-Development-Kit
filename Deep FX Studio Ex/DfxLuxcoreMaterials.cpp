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

#include "DfxLuxcoreMaterials.h"

namespace luxmat {

	// Null
	fl::Any *NullModel::createModel(fl::Space *space) {
		return null(space);
	}

	fl::Any *NullModel::null(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("null")
			.setLabel("Null")
			.setHasModel(true)
			.setModelReference(nullID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUX_NULL_MAT"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Matte
	fl::Any *MatteModel::createModel(fl::Space *space) {
		return matte(space);
	}

	fl::Any *MatteModel::matte(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("matte")
			.setLabel("Matte")
			.setHasModel(true)
			.setModelReference(matteID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUX_MATTE"))
			
			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "diffuse_color", "Diffuse Color", std::string("255:255:255"))
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "opacity", "Opacity", 1.0f)
			.addProperty(space, fl::type::BOOL, fl::role::INPUT, "shadow_catcher", "Shadow Catcher", false)

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "bumpmap", "Bump", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_r", "Diffuse Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_g", "Diffuse Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_b", "Diffuse Blue", 1.0f)
			
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Matte Translucent
	fl::Any *MatteTranslucentModel::createModel(fl::Space *space) {
		return matteTranslucent(space);
	}

	fl::Any *MatteTranslucentModel::matteTranslucent(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("matte_translucent")
			.setLabel("Matte Translucent")
			.setHasModel(true)
			.setModelReference(matteTranslucentID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_MATTE_TRANSLUCENT_MAT"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "reflected_color", "Reflected Color", std::string("127:127:127"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "transmission_color", "Transmission Color", std::string("255:255:255"))
	
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "opacity", "Opacity", 1.0f)

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "bumpmap", "Bump", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflected_r", "Reflected Red", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflected_g", "Reflected Green", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflected_b", "Reflected Blue", .5f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_r", "Transmission Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_g", "Transmission Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_b", "Transmission Blue", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Emission
	fl::Any *EmissionModel::createModel(fl::Space *space) {
		return emission(space);
	}

	fl::Any *EmissionModel::emission(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("emission")
			.setLabel("Emission")
			.setHasModel(true)
			.setModelReference(matteID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUX_EMISSION"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "diffuse_color", "Diffuse Color", std::string("255:255:255"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "gain", "Gain", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "power", "Power", 100.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "efficency", "Efficency", 17.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "importance", "Importance", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "spread", "Spread", 90.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_r", "Diffuse Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_g", "Diffuse Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_b", "Diffuse Blue", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Disney
	fl::Any *DisneyModel::createModel(fl::Space *space) {
		return disney(space);
	}

	fl::Any *DisneyModel::disney(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("disney")
			.setLabel("Disney")
			.setHasModel(true)
			.setModelReference(disneyID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUX_DISNEY"))
			
			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "base_color", "Base Color", std::string("255:255:255"))
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "opacity", "Opacity", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "subsurface", "Sub-Surface", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "metallic", "Metallic", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "specular", "Specular", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "specular_tint", "Specular Tint", 0.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "roughness", "Roughness", .20f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "anisotropic", "Anisotropic", 0.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "sheen", "Sheen", 0.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "sheen_tint", "Sheen Tint", 0.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "clear_coat", "Clear Coat", 0.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "clear_coat_gloss", "Clear Coat Gloss", 1.0f)

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "bumpmap", "Bump", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "base_r", "Base Color Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "base_g", "Base Color Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "base_b", "Base Color Blue", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Mix
	fl::Any *MixModel::createModel(fl::Space *space) {
		return mix_material(space);
	}

	fl::Any *MixModel::mix_material(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("mix")
			.setLabel("Mix")
			.setHasModel(true)
			.setModelReference(mixID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUX_MIX_MATERIAL"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "mat_00", "Material 1", std::string(""))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "mat_01", "Material 2", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "amount", "Mix Amount", .5f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Glass
	fl::Any *GlassModel::createModel(fl::Space *space) {
		return glass(space);
	}

	fl::Any *GlassModel::glass(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("glass")
			.setLabel("Glass")
			.setHasModel(true)
			.setModelReference(glassID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUX_GLASS_MATERIAL"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "transmission_color", "Transmission Color", std::string("255:255:255"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "reflection_color", "Reflection Color", std::string("255:255:255"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "shadow_color", "Shadow Color", std::string("0:0:0"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "ior", "IOR", 1.5f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "dispersion", "Dispersion", 0.f)

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "bumpmap", "Bump", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_r", "Transmission Color Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_g", "Transmission Color Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_b", "Transmission Color Blue", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflection_r", "Reflection Color Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflection_g", "Reflection Color Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflection_b", "Reflection Color Blue", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "shadow_r", "Shadow Color Red", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "shadow_g", "Shadow Color Green", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "shadow_b", "Shadow Color Blue", .0f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Arch Glass
	fl::Any *ArchGlassModel::createModel(fl::Space *space) {
		return archGlass(space);
	}

	fl::Any *ArchGlassModel::archGlass(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("arch_glass")
			.setLabel("Arch Glass")
			.setHasModel(true)
			.setModelReference(archGlassID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_ARCH_GLASS_MAT"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "transmission_color", "Transmission Color", std::string("255:255:255"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "reflection_color", "Reflection Color", std::string("255:255:255"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "shadow_color", "Shadow Color", std::string("0:0:0"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "ior", "IOR", 1.5f)

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "bumpmap", "Bump", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_r", "Transmission Color Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_g", "Transmission Color Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_b", "Transmission Color Blue", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflection_r", "Reflection Color Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflection_g", "Reflection Color Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflection_b", "Reflection Color Blue", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "shadow_r", "Shadow Color Red", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "shadow_g", "Shadow Color Green", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "shadow_b", "Shadow Color Blue", .0f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Rough Glass
	fl::Any *RoughGlassModel::createModel(fl::Space *space) {
		return roughGlass(space);
	}

	fl::Any *RoughGlassModel::roughGlass(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("rough_glass")
			.setLabel("Rough Glass")
			.setHasModel(true)
			.setModelReference(roughGlassID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_ROUGH_GLASS_MAT"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "transmission_color", "Transmission Color", std::string("255:255:255"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "reflection_color", "Reflection Color", std::string("255:255:255"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "shadow_color", "Shadow Color", std::string("0:0:0"))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "bumpmap", "Bump", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "roughness_u", "Roughness U", 0.1f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "roughness_v", "Roughness V", 0.1f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "ior", "IOR", 1.5f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_r", "Transmission Color Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_g", "Transmission Color Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_b", "Transmission Color Blue", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflection_r", "Reflection Color Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflection_g", "Reflection Color Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflection_b", "Reflection Color Blue", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "shadow_r", "Shadow Color Red", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "shadow_g", "Shadow Color Green", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "shadow_b", "Shadow Color Blue", .0f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Car Paint
	fl::Any *CarPaintModel::createModel(fl::Space *space) {
		return carPaint(space);
	}

	fl::Any *CarPaintModel::carPaint(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("car_paint")
			.setLabel("Car Paint")
			.setHasModel(true)
			.setModelReference(carPaintID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_CAR_PAINT_MAT"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "diffuse_color", "Diffuse Color", std::string("2:6:25"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "absorption_color", "Absorption Color", std::string("0:0:0"))
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "absorption_depth", "Absorption Depth", 0.f)

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "bumpmap", "Bump", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "specular_color00", "Specular Color Layer 1", std::string("2:0:0"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "specular_color01", "Specular Color Layer 2", std::string("6:76:10"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "specular_color02", "Specular Color Layer 3", std::string("15:18:20"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "fresnel_effect00", "Fresnel Effect 1", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "fresnel_effect01", "Fresnel Effect 2", 0.9f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "fresnel_effect02", "Fresnel Effect 3", 0.8f)

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "roughness00", "Roughness 1", .88f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "roughness01", "Roughness 2", .8f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "roughness02", "Roughness 3", .015f)
 
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_r", "Diffuse Color Red", .0079f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_g", "Diffuse Color Green", .023f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_b", "Diffuse Color Blue", .1f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "absorption_r", "Absorption Color Red", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "absorption_g", "Absorption Color Green", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "absorption_b", "Absorption Color Blue", .0f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "specular00_r", "Specular Layer 1 Color Red", .0011f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "specular00_g", "Specular Layer 1 Green", .0015f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "specular00_b", "Specular Layer 1 Blue", .0019f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "specular01_r", "Specular Layer 2 Red", .025f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "specular01_g", "Specular Layer 2 Green", .03f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "specular01_b", "Specular Layer 2 Blue", .043f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "specular02_r", "Specular Layer 3 Red", .059f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "specular02_g", "Specular Layer 3 Green", .074f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "specular02_b", "Specular Layer 3 Blue", .082f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Cloth
	fl::Any *ClothModel::createModel(fl::Space *space) {
		return cloth(space);
	}

	fl::Any *ClothModel::cloth(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("cloth")
			.setLabel("Cloth")
			.setHasModel(true)
			.setModelReference(clothID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_CLOTH_MAT"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "warp_diffuse_reflection", "Warp Diffuse Reflection", std::string("255:0:0"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "warp_specular_reflection", "Warp Specular Reflection", std::string("127:127:127"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "weft_diffuse_reflection", "Weft Diffuse Reflection", std::string("255:255:255"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "weft_specular_reflection", "Weft Specular Reflection", std::string("127:127:127"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "thread_count_u", "Thread Count U", 50.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "thread_count_v", "Thread Count V", 50.0f)

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "bumpmap", "Bump", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "warp_diffuse_r", "Warp Diffuse Color Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "warp_diffuse_g", "Warp Diffuse Color Green", 0.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "warp_diffuse_b", "Warp Diffuse Color Blue", 0.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "warp_specular_r", "Warp Reflection Color Red", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "warp_specular_g", "Warp Reflection Color Green", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "warp_specular_b", "Warp Reflection Color Blue", .5f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "weft_diffuse_r", "Weft Diffuse Color Red", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "weft_diffuse_g", "Weft Diffuse Color Green", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "weft_diffuse_b", "Weft Diffuse Color Blue", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "weft_specular_r", "Weft Reflection Color Red", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "weft_specular_g", "Weft Reflection Color Green", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "weft_specular_b", "Weft Reflection Color Blue", .5f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Glossy
	fl::Any *GlossyModel::createModel(fl::Space *space) {
		return glossy(space);
	}

	fl::Any *GlossyModel::glossy(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("glossy")
			.setLabel("Glossy")
			.setHasModel(true)
			.setModelReference(glossyID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_GLOSSY_MAT"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "diffuse_color", "Diffuse Color", std::string("127:127:127"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "specular_color", "Specular Color", std::string("127:127:127"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "absorption_color", "Absorption Color", std::string("0:0:0"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "roughness_u", "Roughness U", 0.1f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "roughness_v", "Roughness V", 0.1f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "ior", "IOR", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "absorption_depth", "Absorption Depth", .0f)

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "bumpmap", "Bump", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_r", "Diffuse Color Red", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_g", "Diffuse Color Green", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_b", "Diffuse Color Blue", .5f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "specular_r", "Specular Color Red", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "specular_g", "Specular Color Green", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "specular_b", "Specular Color Blue", .5f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "absorption_r", "Absorption Color Red", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "absorption_g", "Absorption Color Green", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "absorption_b", "Absorption Color Blue", .0f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Glossy Translucent
	fl::Any *GlossyTranslucentModel::createModel(fl::Space *space) {
		return glossyTranslucent(space);
	}

	fl::Any *GlossyTranslucentModel::glossyTranslucent(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("glossy_translucent")
			.setLabel("Glossy Translucent")
			.setHasModel(true)
			.setModelReference(glossyTranslucentID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_GLOSSY_TRANSLUCENT_MAT"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "diffuse_color", "Diffuse Color", std::string("127:127:127"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "transmission_color", "Transmisson Color", std::string("127:127:127"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "specular_color", "Specular Color", std::string("127:127:127"))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "absorption_color", "Absorption Color", std::string("0:0:0"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "roughness_u", "Roughness U", 0.1f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "roughness_v", "Roughness V", 0.1f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "ior", "IOR", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "absorption_depth", "Absorption Depth", .0f)

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "bumpmap", "Bump", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_r", "Diffuse Color Red", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_g", "Diffuse Color Green", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_b", "Diffuse Color Blue", .5f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_r", "Transmission Color Red", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_g", "Transmission Color Green", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "transmission_b", "Transmission Color Blue", .5f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "specular_r", "Specular Color Red", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "specular_g", "Specular Color Green", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "specular_b", "Specular Color Blue", .5f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "absorption_r", "Absorption Color Red", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "absorption_g", "Absorption Color Green", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "absorption_b", "Absorption Color Blue", .0f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Metal
	fl::Any *MetalModel::createModel(fl::Space *space) {
		return metal(space);
	}

	fl::Any *MetalModel::metal(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("metal")
			.setLabel("Metal")
			.setHasModel(true)
			.setModelReference(metalID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_METAL_MAT"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "fresnel_color", "Fresnel Color", std::string("127:127:127"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "roughness_u", "Roughness U", 0.1f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "roughness_v", "Roughness V", 0.1f)
	
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "bumpmap", "Bump", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "fresnel_r", "Fresnel Color Red", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "fresnel_g", "Fresnel Color Green", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "fresnel_b", "Fresnel Color Blue", .5f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Mirror
	fl::Any *MirrorModel::createModel(fl::Space *space) {
		return mirror(space);
	}

	fl::Any *MirrorModel::mirror(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("mirror")
			.setLabel("Mirror")
			.setHasModel(true)
			.setModelReference(mirrorID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_MIRROR_MAT"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "reflection_color", "Reflection Color", std::string("127:127:127"))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "bumpmap", "Bump", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflection_r", "Reflection Color Red", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflection_g", "Reflection Color Green", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflection_b", "Reflection Color Blue", .5f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Velvet
	fl::Any *VelvetModel::createModel(fl::Space *space) {
		return velvet(space);
	}

	fl::Any *VelvetModel::velvet(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("velvet")
			.setLabel("Velvet")
			.setHasModel(true)
			.setModelReference(velvetID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_VELVET_MAT"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "diffuse_color", "Diffusr Color", std::string("127:127:127"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "p00", "P 1", -2.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "p01", "P 2", 20.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "p02", "P 3", 2.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "thickness", "Thickness", 0.1f)

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "bumpmap", "Bump", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_r", "Diffuse Color Red", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_g", "Diffuse Color Green", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_b", "Diffuse Color Blue", .5f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Rough Matte
	fl::Any *RoughMatteModel::createModel(fl::Space *space) {
		return roughMatte(space);
	}

	fl::Any *RoughMatteModel::roughMatte(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("rough_matte")
			.setLabel("Rough Matte")
			.setHasModel(true)
			.setModelReference(roughMatteID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_ROUGH_MATTE_MAT"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "material_out", "Material", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "diffuse_color", "Diffusr Color", std::string("127:127:127"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "sigma", "Sigma", 0.f)

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "bumpmap", "Bump", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_r", "Diffuse Color Red", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_g", "Diffuse Color Green", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "diffuse_b", "Diffuse Color Blue", .5f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Texture Materials
	fl::Any *ImagemapModel::createModel(fl::Space *space) {
		return imagemap(space);
	}

	fl::Any *ImagemapModel::imagemap(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("imagemap")
			.setLabel("Imagemap")
			.setHasModel(true)
			.setModelReference(imagemapID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUX_IMAGEMAP"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "file", "File", std::string(""))
		
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "gamma", "Gamma", 2.2f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "gain", "Gain", 1.f)

			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::INPUT, "wrap", "Wrap", 0)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::INPUT, "channel", "Channel", 0)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "uv_rotation", "UV Rotation", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "uv_translation_x", "UV Translation X", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "uv_translation_y", "UV Translation Y", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "uv_scale_x", "UV Scale X",  1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "uv_scale_y", "UV Scale Y",  1.f)
			

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	fl::Any *NormalmapModel::createModel(fl::Space *space) {
		return normalmap(space);
	}

	fl::Any *NormalmapModel::normalmap(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("normalmap")
			.setLabel("Normalmap")
			.setHasModel(true)
			.setModelReference(normalmapID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUX_NORMALMAP_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "texture", "Texture Tangent Space", std::string("127:127:127"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "scale", "Scale", 1.f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "texture_x", "Texture X", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "texture_y", "Texture Y", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "texture_z", "Texture Z", 1.f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	fl::Any *CheckerboardModel::createModel(fl::Space *space) {
		return checkerboard_2d(space);
	}

	fl::Any *CheckerboardModel::checkerboard_2d(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("2d_checkerboard")
			.setLabel("2D Checkerboard")
			.setHasModel(true)
			.setModelReference(checkerboard2dID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUX_2D_CHECKERBOARD"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "Color", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "mat_00", "Color 1", std::string(""))
			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "mat_01", "Color 2", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "color00_r", "Color00 R", 0.1f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "color00_g", "Color00 G", 0.1f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "color00_b", "Color00 B", 0.1f)

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "color01_r", "Color01 R", 0.5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "color01_g", "Color01 G", 0.5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "color01_b", "Color01 B", 0.5f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	fl::Any *FbmModel::createModel(fl::Space *space) {
		return fbm(space);
	}

	fl::Any *FbmModel::fbm(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("fbm")
			.setLabel("FBM")
			.setHasModel(true)
			.setModelReference(fbmID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUX_FBM"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "Color", std::string(""))

			.addProperty(space, fl::type::INT, fl::role::INPUT, "octaves", "Octaves", 8)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "roughness", "Roughness", 0.5f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	fl::Any *FresnelColorModel::createModel(fl::Space *space) {
		return fresnelColor(space);
	}

	fl::Any *FresnelColorModel::fresnelColor(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("fresnel_color")
			.setLabel("Fresnel Color")
			.setHasModel(true)
			.setModelReference(fresnelColorID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_FRESNEL_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "reflection_color", "Reflection Color", std::string("127:127:127"))

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflection_r", "Reflection Color Red", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflection_g", "Reflection Color Green", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "reflection_b", "Reflection Color Blue", .5f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	fl::Any *FresnelSopraModel::createModel(fl::Space *space) {
		return fresnelSopra(space);
	}

	fl::Any *FresnelSopraModel::fresnelSopra(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("fresnel_sopra")
			.setLabel("Fresnel Sopra")
			.setHasModel(true)
			.setModelReference(fresnelSopraID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_SOPRA_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "file", "File", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	fl::Any *BlendModel::createModel(fl::Space *space) {
		return blend(space);
	}

	fl::Any *BlendModel::blend(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("blend")
			.setLabel("Blend")
			.setHasModel(true)
			.setModelReference(blendID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_BLEND_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "progression", "Progression Type", std::string("linear"))
			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "direction", "Progression Type", std::string("horizontal"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "bright", "Bright", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "contrast", "Contrast", 1.f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);


		std::vector<std::string> progressionEnum;
		progressionEnum.push_back("linear");
		progressionEnum.push_back("quadratic");
		progressionEnum.push_back("easing");
		progressionEnum.push_back("diagonal");
		progressionEnum.push_back("spherical");
		progressionEnum.push_back("quadratic_spherical");
		progressionEnum.push_back("halo");
		progressionEnum.push_back("radial");

		std::vector<std::string> directionEnum;
		directionEnum.push_back("horizontal");
		directionEnum.push_back("vertical");

		object->rawData<fl::DataObject>()->property("progression")->setStringEnumArray(progressionEnum);
		object->rawData<fl::DataObject>()->property("direction")->setStringEnumArray(directionEnum);

		return object;
	}

	fl::Any *CloudModel::createModel(fl::Space *space) {
		return cloud(space);
	}

	fl::Any *CloudModel::cloud(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("cloud")
			.setLabel("Cloud")
			.setHasModel(true)
			.setModelReference(cloudID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_CLOUD_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "noise_type", "Noise Type", std::string("soft_noise"))
			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "noise_basis", "Noise Basis", std::string("original_perlin"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "noise_size", "Noise Size", .25f)
			.addProperty(space, fl::type::INT, fl::role::INPUT, "noise_depth", "Noise Depth", 2)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "bright", "Bright", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "contrast", "Contrast", 1.f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);


		std::vector<std::string> noiseTypeEnum;
		noiseTypeEnum.push_back("soft_noise");
		noiseTypeEnum.push_back("hard_noise");

		std::vector<std::string> noiseBasisEnum;
		noiseBasisEnum.push_back("original_perlin");
		noiseBasisEnum.push_back("improved_perlin");
		noiseBasisEnum.push_back("voronoi_f1");
		noiseBasisEnum.push_back("voronoi_f2");
		noiseBasisEnum.push_back("voronoi_f3");
		noiseBasisEnum.push_back("voronoi_f4");
		noiseBasisEnum.push_back("voronoi_f2_f1");
		noiseBasisEnum.push_back("voronoi_crackle");
		noiseBasisEnum.push_back("cell_noise");

		object->rawData<fl::DataObject>()->property("noise_type")->setStringEnumArray(noiseTypeEnum);
		object->rawData<fl::DataObject>()->property("noise_basis")->setStringEnumArray(noiseBasisEnum);

		return object;
	}

	fl::Any *DistortedNoiseModel::createModel(fl::Space *space) {
		return distortedNoise(space);
	}

	fl::Any *DistortedNoiseModel::distortedNoise(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("distorted_noise")
			.setLabel("distorted_noise")
			.setHasModel(true)
			.setModelReference(distortedNoiseID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_DISTORTED_NOISE_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "noise_distortion", "Noise Distortion", std::string("original_perlin"))
			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "noise_basis", "Noise Basis", std::string("original_perlin"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "noise_size", "Noise Size", .25f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "distortion", "Distortion", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "bright", "Bright", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "contrast", "Contrast", 1.f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);


		std::vector<std::string> noiseDistortionEnum;
		noiseDistortionEnum.push_back("original_perlin");
		noiseDistortionEnum.push_back("improved_perlin");
		noiseDistortionEnum.push_back("voronoi_f1");
		noiseDistortionEnum.push_back("voronoi_f2");
		noiseDistortionEnum.push_back("voronoi_f3");
		noiseDistortionEnum.push_back("voronoi_f4");
		noiseDistortionEnum.push_back("voronoi_f2_f1");
		noiseDistortionEnum.push_back("cell_noise");

		std::vector<std::string> noiseBasisEnum;
		noiseBasisEnum.push_back("original_perlin");
		noiseBasisEnum.push_back("improved_perlin");
		noiseBasisEnum.push_back("voronoi_f1");
		noiseBasisEnum.push_back("voronoi_f2");
		noiseBasisEnum.push_back("voronoi_f3");
		noiseBasisEnum.push_back("voronoi_f4");
		noiseBasisEnum.push_back("voronoi_f2_f1");
		noiseBasisEnum.push_back("voronoi_crackle");
		noiseBasisEnum.push_back("cell_noise");

		object->rawData<fl::DataObject>()->property("noise_distortion")->setStringEnumArray(noiseDistortionEnum);
		object->rawData<fl::DataObject>()->property("noise_basis")->setStringEnumArray(noiseBasisEnum);

		return object;
	}

	fl::Any *MagicModel::createModel(fl::Space *space) {
		return magic(space);
	}

	fl::Any *MagicModel::magic(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("magic")
			.setLabel("Magic")
			.setHasModel(true)
			.setModelReference(magicID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_MAGIC_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::INT, fl::role::INPUT, "noise_depth", "Noise Depth", 2)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "turbulence", "Turbulence", 5.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "bright", "Bright", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "contrast", "Contrast", 1.f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	fl::Any *MarbleModel::createModel(fl::Space *space) {
		return marble(space);
	}

	fl::Any *MarbleModel::marble(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("marble")
			.setLabel("Marble")
			.setHasModel(true)
			.setModelReference(marbleID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_MARBLE_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "marble_type", "Marble Type", std::string("soft"))
			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "noise_type", "Noise Type", std::string("soft_noise"))
			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "noise_basis", "Noise Basis", std::string("original_perlin"))
			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "noise_basis2", "Noise Basis 2", std::string("sin"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "noise_size", "Noise Size", .25f)
			.addProperty(space, fl::type::INT, fl::role::INPUT, "noise_depth", "Noise Depth", 2)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "turbulence", "Turbulence", 5.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "bright", "Bright", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "contrast", "Contrast", 1.f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);


		std::vector<std::string> marbleTypeEnum;
		marbleTypeEnum.push_back("soft");
		marbleTypeEnum.push_back("sharp");
		marbleTypeEnum.push_back("sharper");

		std::vector<std::string> noiseTypeEnum;
		noiseTypeEnum.push_back("soft_noise");
		noiseTypeEnum.push_back("hard_noise");

		std::vector<std::string> noiseBasisEnum;
		noiseBasisEnum.push_back("original_perlin");
		noiseBasisEnum.push_back("improved_perlin");
		noiseBasisEnum.push_back("voronoi_f1");
		noiseBasisEnum.push_back("voronoi_f2");
		noiseBasisEnum.push_back("voronoi_f3");
		noiseBasisEnum.push_back("voronoi_f4");
		noiseBasisEnum.push_back("voronoi_f2_f1");
		noiseBasisEnum.push_back("voronoi_crackle");
		noiseBasisEnum.push_back("cell_noise");

		std::vector<std::string> noiseBasis2Enum;
		noiseBasis2Enum.push_back("sin");
		noiseBasis2Enum.push_back("saw");
		noiseBasis2Enum.push_back("tri");

		object->rawData<fl::DataObject>()->property("marble_type")->setStringEnumArray(marbleTypeEnum);
		object->rawData<fl::DataObject>()->property("noise_type")->setStringEnumArray(noiseTypeEnum);
		object->rawData<fl::DataObject>()->property("noise_basis")->setStringEnumArray(noiseBasisEnum);
		object->rawData<fl::DataObject>()->property("noise_basis2")->setStringEnumArray(noiseBasis2Enum);

		return object;
	}

	fl::Any *MusgraveModel::createModel(fl::Space *space) {
		return musgrave(space);
	}

	fl::Any *MusgraveModel::musgrave(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("musgrave")
			.setLabel("Musgrave")
			.setHasModel(true)
			.setModelReference(musgraveID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_MUSGRAVE_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "noise_type", "Noise Type", std::string("soft_noise"))
			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "musgrave_type", "Musgrave Type", std::string("multifractal"))
			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "noise_basis", "Noise Basis", std::string("original_perlin"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "dimension", "Dimension", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "intensity", "Intensity", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "lacunarity", "Lacunarity", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "offset", "Offset", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "gain", "Gain", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "octaves", "Octaves", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "noise_size", "Noise Size", .25f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "bright", "Bright", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "contrast", "Contrast", 1.f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)

			.build(space);


		std::vector<std::string> noiseTypeEnum;
		noiseTypeEnum.push_back("soft_noise");
		noiseTypeEnum.push_back("hard_noise");

		std::vector<std::string> musgraveTypeEnum;
		musgraveTypeEnum.push_back("multifractal");
		musgraveTypeEnum.push_back("ridged_multifractal");
		musgraveTypeEnum.push_back("hybrid_multifractal");
		musgraveTypeEnum.push_back("fBM");
		musgraveTypeEnum.push_back("hetero_terrain");

		std::vector<std::string> noiseBasisEnum;
		noiseBasisEnum.push_back("original_perlin");
		noiseBasisEnum.push_back("improved_perlin");
		noiseBasisEnum.push_back("voronoi_f1");
		noiseBasisEnum.push_back("voronoi_f2");
		noiseBasisEnum.push_back("voronoi_f3");
		noiseBasisEnum.push_back("voronoi_f4");
		noiseBasisEnum.push_back("voronoi_f2_f1");
		noiseBasisEnum.push_back("voronoi_crackle");
		noiseBasisEnum.push_back("cell_noise");

		object->rawData<fl::DataObject>()->property("noise_type")->setStringEnumArray(noiseTypeEnum);
		object->rawData<fl::DataObject>()->property("musgrave_type")->setStringEnumArray(musgraveTypeEnum);
		object->rawData<fl::DataObject>()->property("noise_basis")->setStringEnumArray(noiseBasisEnum);

		return object;
	}

	fl::Any *NoiseModel::createModel(fl::Space *space) {
		return noise(space);
	}

	fl::Any *NoiseModel::noise(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("noise")
			.setLabel("Noise")
			.setHasModel(true)
			.setModelReference(noiseID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_NOISE_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::INT, fl::role::INPUT, "noise_depth", "Noise Depth", 2)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "bright", "Bright", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "contrast", "Contrast", 1.f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	fl::Any *StucciModel::createModel(fl::Space *space) {
		return stucci(space);
	}

	fl::Any *StucciModel::stucci(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("stucci")
			.setLabel("Stucci")
			.setHasModel(true)
			.setModelReference(stucciID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_STUCCI_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "noise_type", "Noise Type", std::string("soft_noise"))
			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "stucci_type", "Stucci Type", std::string("plastic"))
			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "noise_basis", "Noise Basis", std::string("original_perlin"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "noise_size", "Noise Size", .25f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "turbulence", "Turbulence", 5.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "bright", "Bright", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "contrast", "Contrast", 1.f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);


		std::vector<std::string> noiseTypeEnum;
		noiseTypeEnum.push_back("soft_noise");
		noiseTypeEnum.push_back("hard_noise");

		std::vector<std::string> stucciTypeEnum;
		stucciTypeEnum.push_back("plastic");
		stucciTypeEnum.push_back("wall_in");
		stucciTypeEnum.push_back("wall_out");

		std::vector<std::string> noiseBasisEnum;
		noiseBasisEnum.push_back("original_perlin");
		noiseBasisEnum.push_back("improved_perlin");
		noiseBasisEnum.push_back("voronoi_f1");
		noiseBasisEnum.push_back("voronoi_f2");
		noiseBasisEnum.push_back("voronoi_f3");
		noiseBasisEnum.push_back("voronoi_f4");
		noiseBasisEnum.push_back("voronoi_f2_f1");
		noiseBasisEnum.push_back("voronoi_crackle");
		noiseBasisEnum.push_back("cell_noise");

		object->rawData<fl::DataObject>()->property("noise_type")->setStringEnumArray(noiseTypeEnum);
		object->rawData<fl::DataObject>()->property("stucci_type")->setStringEnumArray(stucciTypeEnum);
		object->rawData<fl::DataObject>()->property("noise_basis")->setStringEnumArray(noiseBasisEnum);

		return object;
	}

	fl::Any *WoodModel::createModel(fl::Space *space) {
		return wood(space);
	}

	fl::Any *WoodModel::wood(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("wood")
			.setLabel("Wood")
			.setHasModel(true)
			.setModelReference(woodID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_WOOD_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "noise_type", "Noise Type", std::string("soft_noise"))
			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "wood_type", "Wood Type", std::string("bands"))
			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "noise_basis", "Noise Basis", std::string("original_perlin"))
			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "noise_basis2", "Noise Basis 2", std::string("sin"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT,  "noise_size", "Noise Size", .25f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "turbulence", "Turbulence", 5.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT,  "bright", "Bright", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT,  "contrast", "Contrast", 1.f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		std::vector<std::string> woodTypeEnum;
		woodTypeEnum.push_back("bands");
		woodTypeEnum.push_back("rings");
		woodTypeEnum.push_back("bandnoise");
		woodTypeEnum.push_back("ringnoise");

		std::vector<std::string> noiseTypeEnum;
		noiseTypeEnum.push_back("soft_noise");
		noiseTypeEnum.push_back("hard_noise");

		std::vector<std::string> noiseBasisEnum;
		noiseBasisEnum.push_back("original_perlin");
		noiseBasisEnum.push_back("improved_perlin");
		noiseBasisEnum.push_back("voronoi_f1");
		noiseBasisEnum.push_back("voronoi_f2");
		noiseBasisEnum.push_back("voronoi_f3");
		noiseBasisEnum.push_back("voronoi_f4");
		noiseBasisEnum.push_back("voronoi_f2_f1");
		noiseBasisEnum.push_back("voronoi_crackle");
		noiseBasisEnum.push_back("cell_noise");

		std::vector<std::string> noiseBasis2Enum;
		noiseBasis2Enum.push_back("sin");
		noiseBasis2Enum.push_back("saw");
		noiseBasis2Enum.push_back("tri");

		object->rawData<fl::DataObject>()->property("wood_type")->setStringEnumArray(woodTypeEnum);
		object->rawData<fl::DataObject>()->property("noise_type")->setStringEnumArray(noiseTypeEnum);
		object->rawData<fl::DataObject>()->property("noise_basis")->setStringEnumArray(noiseBasisEnum);
		object->rawData<fl::DataObject>()->property("noise_basis2")->setStringEnumArray(noiseBasis2Enum);

		return object;
	}

	fl::Any *VoronoiModel::createModel(fl::Space *space) {
		return voronoi(space);
	}

	fl::Any *VoronoiModel::voronoi(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("voronoi")
			.setLabel("Voronoi")
			.setHasModel(true)
			.setModelReference(voronoiID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_VORONOI_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::HIDDEN_INPUT, "distmetric", "Distmetric", std::string("actual_distance"))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "intensity", "Intensity", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "w00", "W1", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "w01", "W2", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "w02", "W3", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "w03", "W4", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "noise_size", "Noise Size", .25f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "bright", "Bright", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "contrast", "Contrast", 1.f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		std::vector<std::string> distmetricEnum;
		distmetricEnum.push_back("actual_distance");
		distmetricEnum.push_back("distance_squared");
		distmetricEnum.push_back("manhattan");
		distmetricEnum.push_back("chebychev");
		distmetricEnum.push_back("minkowski_half");
		distmetricEnum.push_back("minkowski_four");
		distmetricEnum.push_back("minkowski");

		object->rawData<fl::DataObject>()->property("distmetric")->setStringEnumArray(distmetricEnum);

		return object;
	}

	fl::Any *Fresnel01Model::createModel(fl::Space *space) {
		return fresnel01(space);
	}

	fl::Any *Fresnel01Model::fresnel01(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("fresnel01")
			.setLabel("Fresnel 01")
			.setHasModel(true)
			.setModelReference(fresnel01ID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_FRESNEL01_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "color", "Color", std::string("127:127:127"))

			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "color_r", "Color Red", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "color_g", "Color Green", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::HIDDEN_INPUT, "color_b", "Color Blue", .5f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	// Texture Util
	fl::Any *MixTexModel::createModel(fl::Space *space) {
		return mixTex(space);
	}

	fl::Any *MixTexModel::mixTex(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("mix_tex")
			.setLabel("Mix Texture")
			.setHasModel(true)
			.setModelReference(mixTexID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_MIX_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "amount", "Amount", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "texture1", "Texture 1", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "texture2", "Texture 2", 1.0f)
			
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	fl::Any *ClampModel::createModel(fl::Space *space) {
		return clamp(space);
	}

	fl::Any *ClampModel::clamp(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("clamp")
			.setLabel("Clamp")
			.setHasModel(true)
			.setModelReference(clampID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_CLAMP_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "texture", "Texture", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "min", "min", .0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "max", "max", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

	fl::Any *HsvModel::createModel(fl::Space *space) {
		return hsv(space);
	}

	fl::Any *HsvModel::hsv(fl::Space *space) {

		auto object = fl::DataObject::Builder()
			.setName("hsv")
			.setLabel("Hsv")
			.setHasModel(true)
			.setModelReference(hsvID)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "render_target", "Render Target", (unsigned int)DfxRenderTarget::LUXCORE)
			.addProperty(space, fl::type::STD_STRING, fl::role::NONE, "uid", "Unique Identifier", std::string("LUXCORE_HSV_TEX"))

			.addProperty(space, fl::type::STD_STRING, fl::role::OUTPUT, "color_out", "color", std::string(""))

			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "texture", "Texture", 1.f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "hue", "Hue", .5f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "saturation", "Saturation", 1.0f)
			.addProperty(space, fl::type::FLOAT, fl::role::INPUT, "value", "Value", 1.0f)

			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "Node X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Node Y-Position", 0.f)
			.build(space);

		return object;
	}

}
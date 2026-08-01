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

#include <QSettings>
#include "DfxQMaterialNodeForm.h"

DfxQMaterialNodeForm::DfxQMaterialNodeForm() {

	luxImagemapWrapQControlenums.push_back(std::pair<std::string, unsigned int>("Repeat", LuxImagemapWrap::REPEAT));
	luxImagemapWrapQControlenums.push_back(std::pair<std::string, unsigned int>("Black", LuxImagemapWrap::BLACK));
	luxImagemapWrapQControlenums.push_back(std::pair<std::string, unsigned int>("White", LuxImagemapWrap::WHITE));
	luxImagemapWrapQControlenums.push_back(std::pair<std::string, unsigned int>("Clamp", LuxImagemapWrap::CLAMP));

	luxImagemapChannelQControlenums.push_back(std::pair<std::string, unsigned int>("Default", LuxImagemapChannel::DEFAULT));
	luxImagemapChannelQControlenums.push_back(std::pair<std::string, unsigned int>("Red", LuxImagemapChannel::RED));
	luxImagemapChannelQControlenums.push_back(std::pair<std::string, unsigned int>("Blue", LuxImagemapChannel::BLUE));
	luxImagemapChannelQControlenums.push_back(std::pair<std::string, unsigned int>("Green", LuxImagemapChannel::GREEN));
	luxImagemapChannelQControlenums.push_back(std::pair<std::string, unsigned int>("Alpha", LuxImagemapChannel::ALPHA));
	luxImagemapChannelQControlenums.push_back(std::pair<std::string, unsigned int>("Mean", LuxImagemapChannel::MEAN));
	luxImagemapChannelQControlenums.push_back(std::pair<std::string, unsigned int>("Colored Mean", LuxImagemapChannel::COLORED_MEAN));
	luxImagemapChannelQControlenums.push_back(std::pair<std::string, unsigned int>("RGB", LuxImagemapChannel::RGB));

	luxDisplacementTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Height", LuxDisplacementType::HEIGHT));
	luxDisplacementTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Vector", LuxDisplacementType::VECTOR));

}


DfxQMaterialNodeForm::~DfxQMaterialNodeForm() {

}

bool DfxQMaterialNodeForm::create(DfxQMaterialNode *node) {

	auto space = DfxSpace::getInstance();

	auto any = space->findAbsolute(node->dataObjectPath());

	if (any != nullptr) {
		if (any->data<fl::DataObject>().hasProperty("uid")) {
			std::string uid = any->data<fl::DataObject>().property("uid")->data<std::string>();
	
			// Luxcore
			if (uid == "LUX_OUTPUT") {
				lux_output(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUX_MATTE") {
				lux_matte_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_MATTE_TRANSLUCENT_MAT") {
				lux_matte_translucent_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUX_GLASS_MATERIAL") {
				lux_glass_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_ARCH_GLASS_MAT") {
				lux_arch_glass_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_ROUGH_GLASS_MAT") {
				lux_rough_glass_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_CAR_PAINT_MAT") {
				lux_car_paint_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_CLOTH_MAT") {
				lux_cloth_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_GLOSSY_MAT") {
				lux_glossy_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_GLOSSY_TRANSLUCENT_MAT") {
				lux_glossy_translucent_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_METAL_MAT") {
				lux_metal_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_MIRROR_MAT") {
				lux_mirror_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_VELVET_MAT") {
				lux_velvet_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_ROUGH_MATTE_MAT") {
				lux_rough_matte_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUX_EMISSION") {
				lux_emission_marterial(node->dataObjectPath());
				return true;
			}
			else if(uid == "LUX_DISNEY"){
				lux_disney_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUX_MIX_MATERIAL") {
				lux_mix_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUX_IMAGEMAP") {
				lux_imagemap_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUX_NORMALMAP_TEX") {
				lux_normalmap_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUX_2D_CHECKERBOARD") {
				lux_2d_checkerboard_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUX_FBM") {
				lux_fbm_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_FRESNEL_TEX") {
				lux_fresnel_color_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_BLEND_TEX") {
				lux_blend_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_BLEND_TEX") {
				lux_blend_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_BLEND_TEX") {
				lux_blend_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_BLEND_TEX") {
				lux_blend_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_CLOUD_TEX") {
				lux_cloud_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_DISTORTED_NOISE_TEX") {
				lux_distorted_noise_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_MAGIC_TEX") {
				lux_magic_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_MARBLE_TEX") {
				lux_marble_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_MUSGRAVE_TEX") {
				lux_musgrave_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_NOISE_TEX") {
				lux_noise_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_STUCCI_TEX") {
				lux_stucci_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_WOOD_TEX") {
				lux_wood_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_VORONOI_TEX") {
				lux_voronoi_texture(node->dataObjectPath());
				return true;
			}
			else if (uid == "LUXCORE_MIX_TEX") {
				lux_mix_tex_texture(node->dataObjectPath());
			return true;
			}
			else if (uid == "LUXCORE_CLAMP_TEX") {
				lux_clamp_texture(node->dataObjectPath());
			return true;
			}
			else if (uid == "LUXCORE_HSV_TEX") {
				lux_hsv_texture(node->dataObjectPath());
			return true;
			}
			
			// OSPRay 
			if (uid == "OSPRAY_PRINCIPLED_MAT") {
				osp_principled_marterial(node->dataObjectPath());
				return true;
			}
			else if (uid == "OSPRAY_IMAGE_TEXTURE") {
				osp_image_texture(node->dataObjectPath());
				return true;
			}
		}
	}

	return false;
}

void DfxQMaterialNodeForm::destroy() {

}

// Luxcore Render
void DfxQMaterialNodeForm::lux_output(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();
	auto subdivison = new DfxControlPanel();
	auto displacement = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();
	DfxQControlArray *subdivArray = new DfxQControlArray();
	DfxQControlArray *dispArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto materialID = new DfxQIntegerSpinnerControl(path + "material_id", "", false);
	auto override_smooth_shading = new DfxQCheckBoxControl(path + "smooth_shading_override", "", false);
	auto use_smooth_shading = new DfxQCheckBoxControl(path + "use_smooth_shading", "", false);

	// Sub Division
	auto enable_subdivison = new DfxQCheckBoxControl(path + "enable_subdivison", "", false);
	auto max_level = new DfxQIntegerSpinnerControl(path + "max_level", "", false);
	auto max_edge_screen_size = new DfxQDecimalSpinnerControl(path + "max_edge_screen_size", "", false);

	// Displacement
	auto disp_smooth_normals = new DfxQCheckBoxControl(path + "disp_smooth_normals", "", false);
	auto disp_type = new DfxQComboBoxControl(path + "disp_type", "", luxDisplacementTypeQControlenums, false);
	auto disp_scale = new DfxQDecimalSpinnerControl(path + "disp_scale", "", false);
	auto disp_offset = new DfxQDecimalSpinnerControl(path + "disp_offset", "", false);

	propertiesArray->addControl("Material ID", materialID);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Override Smoothing", override_smooth_shading);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Smooth Shading", use_smooth_shading);

	properties->setTitle("Material");
	properties->setWidget(propertiesArray);

	subdivArray->addControl("Enable Subdivison", enable_subdivison);
	subdivArray->addSpacer(0, 3);
	subdivArray->addControl("Max Level", max_level);
	subdivArray->addSpacer(0, 3);
	subdivArray->addControl("Max Edge Screen Size", max_edge_screen_size);

	subdivison->setTitle("Subdivison");
	subdivison->setWidget(subdivArray);

	dispArray->addControl("Smooth Normals", disp_smooth_normals);
	dispArray->addSpacer(0, 3);
	dispArray->addControl("Type", disp_type);
	dispArray->addSpacer(0, 3);
	dispArray->addControl("Scale", disp_scale);
	dispArray->addSpacer(0, 3);
	dispArray->addControl("Offset", disp_offset);

	displacement->setTitle("Displacement");
	displacement->setWidget(dispArray);

	main_layout->addWidget(properties);
	main_layout->addWidget(subdivison);
	main_layout->addWidget(displacement);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_matte_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();
	
	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto diffuse  = new DfxQColorControl(path + "diffuse_r", path + "diffuse_g", path + "diffuse_b", "", false);
	auto opacity = new DfxQDecimalSpinnerControl(path + "opacity", "", false);
	auto shadow_catcher  = new DfxQCheckBoxControl(path + "shadow_catcher", "", false);

	propertiesArray->addControl("Color", diffuse);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Opacity", opacity);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Shadow Catcher", shadow_catcher);

	properties->setTitle("Matte");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_matte_translucent_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto reflected = new DfxQColorControl(path + "reflected_r", path + "reflected_g", path + "reflected_b", "", false);
	auto transmission = new DfxQColorControl(path + "transmission_r", path + "transmission_g", path + "transmission_b", "", false);
	auto opacity  = new DfxQDecimalSpinnerControl(path + "opacity", "", false);

	propertiesArray->addControl("Reflection", reflected);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Transmission", transmission);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Opacity", opacity);

	properties->setTitle("Matte Translucent");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_glass_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto transmission = new DfxQColorControl(path + "transmission_r", path + "transmission_g", path + "transmission_b", "", false);
	auto reflection = new DfxQColorControl(path + "reflection_r", path + "reflection_g", path + "reflection_b", "", false);
	auto shadow = new DfxQColorControl(path + "shadow_r", path + "shadow_g", path + "shadow_b", "", false);
	auto ior = new DfxQDecimalSpinnerControl(path + "ior", "", false);
	auto dispersion = new DfxQDecimalSpinnerControl(path + "dispersion", "", false);

	//auto opacity  = new DfxQDecimalSpinnerControl(path + "opacity", "", false);

	propertiesArray->addControl("Transmission", transmission);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Reflection", reflection);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Shadow", shadow);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("IOR", ior);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Dispersion", dispersion);
	propertiesArray->addSpacer(0, 3);

	properties->setTitle("Glass");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_arch_glass_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto transmission = new DfxQColorControl(path + "transmission_r", path + "transmission_g", path + "transmission_b", "", false);
	auto reflection = new DfxQColorControl(path + "reflection_r", path + "reflection_g", path + "reflection_b", "", false);
	auto shadow = new DfxQColorControl(path + "shadow_r", path + "shadow_g", path + "shadow_b", "", false);
	auto ior = new DfxQDecimalSpinnerControl(path + "ior", "", false);

	//auto opacity  = new DfxQDecimalSpinnerControl(path + "opacity", "", false);

	propertiesArray->addControl("Transmission", transmission);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Reflection", reflection);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Shadow", shadow);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("IOR", ior);

	properties->setTitle("Arch Glass");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_rough_glass_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto transmission = new DfxQColorControl(path + "transmission_r", path + "transmission_g", path + "transmission_b", "", false);
	auto reflection = new DfxQColorControl(path + "reflection_r", path + "reflection_g", path + "reflection_b", "", false);
	auto shadow = new DfxQColorControl(path + "shadow_r", path + "shadow_g", path + "shadow_b", "", false);
	auto roughness_u = new DfxQDecimalSpinnerControl(path + "roughness_u", "", false);
	auto roughness_v = new DfxQDecimalSpinnerControl(path + "roughness_v", "", false);
	auto ior = new DfxQDecimalSpinnerControl(path + "ior", "", false);

	//auto opacity  = new DfxQDecimalSpinnerControl(path + "opacity", "", false);

	propertiesArray->addControl("Transmission", transmission);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Reflection", reflection);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Shadow", shadow);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Roughness U", roughness_u);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Roughness V", roughness_v);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("IOR", ior);

	properties->setTitle("Rough Glass");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_car_paint_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto diffuse = new DfxQColorControl(path + "diffuse_r", path + "diffuse_g", path + "diffuse_b", "", false);
	auto absorption = new DfxQColorControl(path + "absorption_r", path + "absorption_g", path + "absorption_b", "", false);
	auto absorption_depth = new DfxQDecimalSpinnerControl(path + "absorption_depth", "", false);

	auto spec00 = new DfxQColorControl(path + "specular00_r", path + "specular00_g", path + "specular00_b", "", false);
	auto spec01 = new DfxQColorControl(path + "specular01_r", path + "specular01_g", path + "specular01_b", "", false);
	auto spec02 = new DfxQColorControl(path + "specular02_r", path + "specular02_g", path + "specular02_b", "", false);

	auto fresnel_effect00 = new DfxQDecimalSpinnerControl(path + "fresnel_effect00", "", false);
	auto fresnel_effect01 = new DfxQDecimalSpinnerControl(path + "fresnel_effect01", "", false);
	auto fresnel_effect02 = new DfxQDecimalSpinnerControl(path + "fresnel_effect02", "", false);

	auto roughness00 = new DfxQDecimalSpinnerControl(path + "roughness00", "", false);
	auto roughness01 = new DfxQDecimalSpinnerControl(path + "roughness01", "", false);
	auto roughness02 = new DfxQDecimalSpinnerControl(path + "roughness02", "", false);

	//auto opacity  = new DfxQDecimalSpinnerControl(path + "opacity", "", false);

	propertiesArray->addControl("Diffuse", diffuse);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Absorption", absorption);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Absorption Depth", absorption_depth);
	propertiesArray->addSpacer(0, 5);
	propertiesArray->addControl("Specular Layer 1", spec00);
	propertiesArray->addControl("Specular Layer 2", spec01);
	propertiesArray->addControl("Specular Layer 3", spec02);
	propertiesArray->addSpacer(0, 5);
	propertiesArray->addControl("Fresnel Layer 1", fresnel_effect00);
	propertiesArray->addControl("Fresnel Layer 2", fresnel_effect01);
	propertiesArray->addControl("Fresnel Layer 3", fresnel_effect02);
	propertiesArray->addSpacer(0, 5);
	propertiesArray->addControl("Roughness Layer 1", roughness00);
	propertiesArray->addControl("Roughness Layer 2", roughness01);
	propertiesArray->addControl("Roughness Layer 3", roughness02);

	properties->setTitle("Car Paint");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_cloth_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto warp_diffuse = new DfxQColorControl(path + "warp_diffuse_r", path + "warp_diffuse_g", path + "warp_diffuse_b", "", false);
	auto warp_specular = new DfxQColorControl(path + "warp_specular_r", path + "warp_specular_g", path + "warp_specular_b", "", false);
	auto weft_diffuse = new DfxQColorControl(path + "weft_diffuse_r", path + "weft_diffuse_g", path + "weft_diffuse_b", "", false);
	auto weft_specular = new DfxQColorControl(path + "weft_specular_r", path + "weft_specular_g", path + "weft_specular_b", "", false);
	auto threadu = new DfxQDecimalSpinnerControl(path + "thread_count_u", "", false);
	auto threadv = new DfxQDecimalSpinnerControl(path + "thread_count_v", "", false);
	auto efficency = new DfxQDecimalSpinnerControl(path + "efficency", "", false);
	auto importance = new DfxQDecimalSpinnerControl(path + "importance", "", false);
	auto spread = new DfxQDecimalSpinnerControl(path + "spread", "", false);

	//auto opacity  = new DfxQDecimalSpinnerControl(path + "opacity", "", false);

	propertiesArray->addControl("Warp Diffuse", warp_diffuse);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Warp Specular", warp_specular);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Weft Diffuse", weft_diffuse);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Weft Specular", weft_specular);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Threads U", threadu);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Threads V", threadv);
	
	properties->setTitle("Cloth");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_glossy_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto diffuse = new DfxQColorControl(path + "diffuse_r", path + "diffuse_g", path + "diffuse_b", "", false);
	auto specular = new DfxQColorControl(path + "specular_r", path + "specular_g", path + "specular_b", "", false);
	auto absorption = new DfxQColorControl(path + "absorption_r", path + "absorption_g", path + "absorption_b", "", false);
	auto roughness_u = new DfxQDecimalSpinnerControl(path + "roughness_u", "", false);
	auto roughness_v = new DfxQDecimalSpinnerControl(path + "roughness_v", "", false);
	auto ior = new DfxQDecimalSpinnerControl(path + "ior", "", false);
	auto absorption_depth = new DfxQDecimalSpinnerControl(path + "absorption_depth", "", false);


	//auto opacity  = new DfxQDecimalSpinnerControl(path + "opacity", "", false);

	propertiesArray->addControl("Diffuse", diffuse);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Specular", specular);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Absorption", absorption);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Roughness U", roughness_u);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Roughness v", roughness_v);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("ior", ior);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("absorption_depth", absorption_depth);

	properties->setTitle("Glossy");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_glossy_translucent_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto diffuse = new DfxQColorControl(path + "diffuse_r", path + "diffuse_g", path + "diffuse_b", "", false);
	auto transmission = new DfxQColorControl(path + "transmission_r", path + "transmission_r", path + "transmission_r", "", false);
	auto specular = new DfxQColorControl(path + "specular_r", path + "specular_g", path + "specular_b", "", false);
	auto absorption = new DfxQColorControl(path + "absorption_r", path + "absorption_g", path + "absorption_b", "", false);
	auto roughness_u = new DfxQDecimalSpinnerControl(path + "roughness_u", "", false);
	auto roughness_v = new DfxQDecimalSpinnerControl(path + "roughness_v", "", false);
	auto ior = new DfxQDecimalSpinnerControl(path + "ior", "", false);
	auto absorption_depth = new DfxQDecimalSpinnerControl(path + "absorption_depth", "", false);


	//auto opacity  = new DfxQDecimalSpinnerControl(path + "opacity", "", false);

	propertiesArray->addControl("Diffuse", diffuse);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Transmission", transmission);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Specular", specular);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Absorption", absorption);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Roughness U", roughness_u);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Roughness v", roughness_v);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("ior", ior);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("absorption_depth", absorption_depth);

	properties->setTitle("Glossy");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_metal_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto fresnel = new DfxQColorControl(path + "fresnel_r", path + "fresnel_g", path + "fresnel_b", "", false);
	auto roughness_u = new DfxQDecimalSpinnerControl(path + "roughness_u", "", false);
	auto roughness_v = new DfxQDecimalSpinnerControl(path + "roughness_v", "", false);


	//auto opacity  = new DfxQDecimalSpinnerControl(path + "opacity", "", false);

	propertiesArray->addControl("Diffuse", fresnel);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Roughness U", roughness_u);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Roughness v", roughness_v);

	properties->setTitle("Metal");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_mirror_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto reflection = new DfxQColorControl(path + "reflection_r", path + "reflection_g", path + "reflection_b", "", false);

	//auto opacity  = new DfxQDecimalSpinnerControl(path + "opacity", "", false);

	propertiesArray->addControl("Reflection Color", reflection);

	properties->setTitle("Mirror");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_velvet_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto diffuse = new DfxQColorControl(path + "diffuse_r", path + "diffuse_g", path + "diffuse_b", "", false);
	auto p00 = new DfxQDecimalSpinnerControl(path + "p00", "", false);
	auto p01 = new DfxQDecimalSpinnerControl(path + "p01", "", false);
	auto p02 = new DfxQDecimalSpinnerControl(path + "p02", "", false);
	auto thickness = new DfxQDecimalSpinnerControl(path + "thickness", "", false);


	//auto opacity  = new DfxQDecimalSpinnerControl(path + "opacity", "", false);

	propertiesArray->addControl("Diffuse Color", diffuse);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("P1", p00);
	propertiesArray->addSpacer(0, 3); 
	propertiesArray->addControl("P2", p01);
	propertiesArray->addSpacer(0, 3); 
	propertiesArray->addControl("P3", p02);
	propertiesArray->addSpacer(0, 3); 
	propertiesArray->addControl("Thickness", thickness);

	properties->setTitle("Velvet");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_rough_matte_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto diffuse = new DfxQColorControl(path + "diffuse_r", path + "diffuse_g", path + "diffuse_b", "", false);
	auto sigma = new DfxQDecimalSpinnerControl(path + "sigma", "", false);

	//auto opacity  = new DfxQDecimalSpinnerControl(path + "opacity", "", false);

	propertiesArray->addControl("Diffuse Color", diffuse);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Sigma", sigma);

	properties->setTitle("Rough Matte");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_emission_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto diffuse = new DfxQColorControl(path + "diffuse_r", path + "diffuse_g", path + "diffuse_b", "", false);
	auto gain = new DfxQDecimalSpinnerControl(path + "gain", "", false);
	auto power = new DfxQDecimalSpinnerControl(path + "power", "", false);
	auto efficency = new DfxQDecimalSpinnerControl(path + "efficency", "", false);
	auto importance = new DfxQDecimalSpinnerControl(path + "importance", "", false);
	auto spread = new DfxQDecimalSpinnerControl(path + "spread", "", false);

	//auto opacity  = new DfxQDecimalSpinnerControl(path + "opacity", "", false);

	propertiesArray->addControl("Color", diffuse);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Gain", gain);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Power", power);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Efficency", efficency);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Importance", importance);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Spread", spread);

	properties->setTitle("Emission");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_disney_marterial(std::string mat_path) {

	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto diffuse	= new DfxQColorControl(path + "base_r", path + "base_g", path + "base_b", "", false);
	auto opacity	= new DfxQDecimalSpinnerControl(path + "opacity", "", false);
	auto subsurface = new DfxQDecimalSpinnerControl(path + "subsurface", "", false);
	auto metallic	= new DfxQDecimalSpinnerControl(path + "metallic", "", false);
	auto specular	= new DfxQDecimalSpinnerControl(path + "specular", "", false);
	auto specular_tint	= new DfxQDecimalSpinnerControl(path + "specular_tint", "", false);
	auto roughness		= new DfxQDecimalSpinnerControl(path + "roughness", "", false);
	auto anisotropic	= new DfxQDecimalSpinnerControl(path + "anisotropic", "", false);
	auto sheen			= new DfxQDecimalSpinnerControl(path + "sheen", "", false);
	auto sheen_tint		= new DfxQDecimalSpinnerControl(path + "sheen_tint", "", false);
	auto clear_coat		= new DfxQDecimalSpinnerControl(path + "clear_coat", "", false);
	auto clear_coat_gloss = new DfxQDecimalSpinnerControl(path + "clear_coat_gloss", "", false);

	propertiesArray->addControl("Color", diffuse);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Opacity", opacity);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Sub-Surface", subsurface);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Metallic", metallic);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Specular", specular);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Specular Tint", specular_tint);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Roughness", roughness);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Anisotropic", anisotropic);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Sheen", sheen);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Sheen Tint", sheen_tint);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Clear Coat", clear_coat);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Clear Coat Gloss", clear_coat_gloss);

	properties->setTitle("Disney");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_mix_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto amount = new DfxQDecimalSpinnerControl(path + "amount", "", false);

	propertiesArray->addControl("Mix Amount", amount);

	properties->setTitle("Mix");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

// LuxCore Textures
void DfxQMaterialNodeForm::lux_imagemap_texture(std::string mat_path) {

	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	QSettings settings;
	QString initialDir = settings.value("~/io/lux_imagemap_texture", QDir::homePath()).toString();

	// Controls
	auto file  = new DfxQFilePathControl(path + "file", true, false, "Image", initialDir.toStdString(), "Image Files (*.png *.jpg *.tiff)", "", false);
	auto gamma = new DfxQDecimalSpinnerControl(path + "gamma", "", false);
	auto gain  = new DfxQDecimalSpinnerControl(path + "gain", "", false);
	auto wrap  = new DfxQComboBoxControl(path + "wrap", "", luxImagemapWrapQControlenums, false);
	auto channel = new DfxQComboBoxControl(path + "channel", "", luxImagemapChannelQControlenums, false);
	auto uvrot = new DfxQDecimalSpinnerControl(path + "uv_rotation", "", false);
	auto uvsclx = new DfxQDecimalSpinnerControl(path + "uv_scale_x", "", false);
	auto uvscly = new DfxQDecimalSpinnerControl(path + "uv_scale_y", "", false);
	auto uvtransx = new DfxQDecimalSpinnerControl(path + "uv_translation_x", "", false);
	auto uvtransy = new DfxQDecimalSpinnerControl(path + "uv_translation_y", "", false);

	propertiesArray->addControl("File", file);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Gamma", gamma);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Gain", gain);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Wrap", wrap);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Channel", channel);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("UV Rotation", uvrot);
	propertiesArray->addSpacer(0, 8);
	propertiesArray->addControl("UV Scale X", uvsclx);
	propertiesArray->addControl("Y", uvscly);
	propertiesArray->addSpacer(0, 8);
	propertiesArray->addControl("UV Translation X", uvtransx);
	propertiesArray->addControl("Y", uvtransy);

	properties->setTitle("Imagemap");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);

	QObject::connect(file, &DfxQFilePathControl::rootDirChanged, [file](std::string file) {
		if (file.length() > 0) {
			QSettings settings;
			settings.setValue("~/io/lux_imagemap_texture", file.c_str());
		}
	});
}

void DfxQMaterialNodeForm::lux_normalmap_texture(std::string mat_path) {

	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto texture = new DfxQColorControl(path + "texture_x", path + "texture_y", path + "texture_z", "", false);
	auto scale   = new DfxQDecimalSpinnerControl(path + "scale", "", false);

	propertiesArray->addControl("Texture", texture);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Scale", scale);

	properties->setTitle("Normal Map");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_2d_checkerboard_texture(std::string mat_path) {

	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto color00 = new DfxQColorControl(path + "color00_r", path + "color00_g", path + "color00_b", "", false);
	auto color01 = new DfxQColorControl(path + "color01_r", path + "color01_g", path + "color01_b", "", false);

	propertiesArray->addControl("Color 1", color00);
	propertiesArray->addControl("Color 2", color01);

	properties->setTitle("2D Checkerboard");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_fbm_texture(std::string mat_path) {

	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto octaves   = new DfxQIntegerSpinnerControl(path + "octaves", "", false);
	auto roughness = new DfxQDecimalSpinnerControl(path + "roughness", "", false);

	propertiesArray->addControl("Octaves", octaves);
	propertiesArray->addControl("Roughness", roughness);

	properties->setTitle("FBM");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_fresnel_color_texture(std::string mat_path) {

	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto reflection = new DfxQColorControl(path + "reflection_r", path + "reflection_g", path + "reflection_b", "", false);

	propertiesArray->addControl("Reflection Color", reflection);

	properties->setTitle("Fresnel Color");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_blend_texture(std::string mat_path) {

	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	auto progressEnumm = DfxSpace::getInstance()->findAbsolute(path + "progression")->stringEnumArray();
	auto directionEnumm = DfxSpace::getInstance()->findAbsolute(path + "direction")->stringEnumArray();

	// Controls
	auto progressType = new DfxQStringComboBoxControl(path + "progression", "", progressEnumm, false);
	auto direction = new DfxQStringComboBoxControl(path + "direction", "", directionEnumm, false);
	auto bright = new DfxQDecimalSpinnerControl(path + "bright", "", false);
	auto contrast = new DfxQDecimalSpinnerControl(path + "contrast", "", false);

	propertiesArray->addControl("Progress Type", progressType);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Direction", direction);
	propertiesArray->addSpacer(0, 3); 
	propertiesArray->addControl("Bright", bright);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Contrast", contrast);
	propertiesArray->addSpacer(0, 3);

	properties->setTitle("Blend");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_cloud_texture(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	auto noiseTypeEnumm = DfxSpace::getInstance()->findAbsolute(path + "noise_type")->stringEnumArray();
	auto noiseBasisEnumm = DfxSpace::getInstance()->findAbsolute(path + "noise_basis")->stringEnumArray();

	// Controls
	auto noise_type = new DfxQStringComboBoxControl(path + "noise_type", "", noiseTypeEnumm, false);
	auto noise_basis = new DfxQStringComboBoxControl(path + "noise_basis", "", noiseBasisEnumm, false);
	auto noise_size = new DfxQDecimalSpinnerControl(path + "noise_size", "", false);
	auto noise_depth = new DfxQIntegerSpinnerControl(path + "noise_depth", "", false);
	auto bright = new DfxQDecimalSpinnerControl(path + "bright", "", false);
	auto contrast = new DfxQDecimalSpinnerControl(path + "contrast", "", false);

	propertiesArray->addControl("Noise Type", noise_type);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Basis", noise_basis);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Size", noise_size);
	propertiesArray->addSpacer(0, 3); 
	propertiesArray->addControl("Noise Depth", noise_depth);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Bright", bright);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Contrast", contrast);
	propertiesArray->addSpacer(0, 3);

	properties->setTitle("Cloud");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_distorted_noise_texture(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	auto distortionTypeEnumm = DfxSpace::getInstance()->findAbsolute(path + "noise_distortion")->stringEnumArray();
	auto noiseBasisEnumm = DfxSpace::getInstance()->findAbsolute(path + "noise_basis")->stringEnumArray();

	// Controls
	auto noise_distortion = new DfxQStringComboBoxControl(path + "noise_distortion", "", distortionTypeEnumm, false);
	auto noise_basis = new DfxQStringComboBoxControl(path + "noise_basis", "", noiseBasisEnumm, false);
	auto noise_size = new DfxQDecimalSpinnerControl(path + "noise_size", "", false);
	auto distortion = new DfxQDecimalSpinnerControl(path + "distortion", "", false);
	auto bright = new DfxQDecimalSpinnerControl(path + "bright", "", false);
	auto contrast = new DfxQDecimalSpinnerControl(path + "contrast", "", false);

	propertiesArray->addControl("Noise Distortion", noise_distortion);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Basis", noise_basis);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Size", noise_size);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Distortion", distortion);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Bright", bright);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Contrast", contrast);
	propertiesArray->addSpacer(0, 3);

	properties->setTitle("Noise Distortion");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_magic_texture(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto noise_depth = new DfxQIntegerSpinnerControl(path + "noise_depth", "", false);
	auto turbulence = new DfxQDecimalSpinnerControl(path + "turbulence", "", false);
	auto bright = new DfxQDecimalSpinnerControl(path + "bright", "", false);
	auto contrast = new DfxQDecimalSpinnerControl(path + "contrast", "", false);

	propertiesArray->addControl("Noise Depth", noise_depth);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Turbulence", turbulence);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Bright", bright);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Contrast", contrast);
	propertiesArray->addSpacer(0, 3);

	properties->setTitle("Magic");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_marble_texture(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	auto marbleTypeEnumm = DfxSpace::getInstance()->findAbsolute(path + "marble_type")->stringEnumArray();
	auto noiseTypeEnumm = DfxSpace::getInstance()->findAbsolute(path + "noise_type")->stringEnumArray();
	auto noiseBasisEnumm = DfxSpace::getInstance()->findAbsolute(path + "noise_basis")->stringEnumArray();
	auto noiseBasis2Enumm = DfxSpace::getInstance()->findAbsolute(path + "noise_basis2")->stringEnumArray();

	// Controls
	auto marble_type = new DfxQStringComboBoxControl(path + "marble_type", "", marbleTypeEnumm, false);
	auto noise_type = new DfxQStringComboBoxControl(path + "noise_type", "", noiseTypeEnumm, false);
	auto noise_basis = new DfxQStringComboBoxControl(path + "noise_basis", "", noiseBasis2Enumm, false);
	auto noise_basis2 = new DfxQStringComboBoxControl(path + "noise_basis2", "", noiseBasisEnumm, false);
	auto noise_size = new DfxQDecimalSpinnerControl(path + "noise_size", "", false);
	auto noise_depth = new DfxQIntegerSpinnerControl(path + "noise_depth", "", false);
	auto bright = new DfxQDecimalSpinnerControl(path + "bright", "", false);
	auto contrast = new DfxQDecimalSpinnerControl(path + "contrast", "", false);

	propertiesArray->addControl("Marble Type", marble_type);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Type", noise_type);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Basis", noise_basis);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Basis 2", noise_basis2);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Size", noise_size);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Depth", noise_depth);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Bright", bright);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Contrast", contrast);
	propertiesArray->addSpacer(0, 3);

	properties->setTitle("Marble");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_musgrave_texture(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	auto musgraveTypeEnumm = DfxSpace::getInstance()->findAbsolute(path + "musgrave_type")->stringEnumArray();
	auto noiseTypeEnumm = DfxSpace::getInstance()->findAbsolute(path + "noise_type")->stringEnumArray();
	auto noiseBasisEnumm = DfxSpace::getInstance()->findAbsolute(path + "noise_basis")->stringEnumArray();

	// Controls
	auto musgrave_type = new DfxQStringComboBoxControl(path + "musgrave_type", "", musgraveTypeEnumm, false);
	auto noise_type = new DfxQStringComboBoxControl(path + "noise_type", "", noiseTypeEnumm, false);
	auto noise_basis = new DfxQStringComboBoxControl(path + "noise_basis", "", noiseBasisEnumm, false);
	auto dimension = new DfxQDecimalSpinnerControl(path + "dimension", "", false);
	auto intensity = new DfxQDecimalSpinnerControl(path + "intensity", "", false);
	auto lacunarity = new DfxQDecimalSpinnerControl(path + "lacunarity", "", false);
	auto gain = new DfxQDecimalSpinnerControl(path + "gain", "", false);
	auto octaves = new DfxQDecimalSpinnerControl(path + "octaves", "", false);
	auto noise_size = new DfxQDecimalSpinnerControl(path + "noise_size", "", false);
	auto bright = new DfxQDecimalSpinnerControl(path + "bright", "", false);
	auto contrast = new DfxQDecimalSpinnerControl(path + "contrast", "", false);

	propertiesArray->addControl("Musgrave Type", musgrave_type);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Type", noise_type);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Basis", noise_basis);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Dimension", dimension);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Intensity", intensity);
	propertiesArray->addSpacer(0, 3); 
	propertiesArray->addControl("Lacunarity", lacunarity);
	propertiesArray->addSpacer(0, 3); 
	propertiesArray->addControl("Gain", gain);
	propertiesArray->addSpacer(0, 3); 
	propertiesArray->addControl("Octaves", octaves);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Size", noise_size);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Bright", bright);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Contrast", contrast);
	propertiesArray->addSpacer(0, 3);

	properties->setTitle("Musgrave");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_noise_texture(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto noise_depth = new DfxQIntegerSpinnerControl(path + "noise_depth", "", false);
	auto bright = new DfxQDecimalSpinnerControl(path + "bright", "", false);
	auto contrast = new DfxQDecimalSpinnerControl(path + "contrast", "", false);

	propertiesArray->addControl("Noise Depth", noise_depth);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Bright", bright);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Contrast", contrast);
	propertiesArray->addSpacer(0, 3);

	properties->setTitle("Noise");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_stucci_texture(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	auto stucciTypeEnumm = DfxSpace::getInstance()->findAbsolute(path + "stucci_type")->stringEnumArray();
	auto noiseTypeEnumm = DfxSpace::getInstance()->findAbsolute(path + "noise_type")->stringEnumArray();
	auto noiseBasisEnumm = DfxSpace::getInstance()->findAbsolute(path + "noise_basis")->stringEnumArray();

	// Controls
	auto stucci_type = new DfxQStringComboBoxControl(path + "stucci_type", "", stucciTypeEnumm, false);
	auto noise_type = new DfxQStringComboBoxControl(path + "noise_type", "", noiseTypeEnumm, false);
	auto noise_basis = new DfxQStringComboBoxControl(path + "noise_basis", "", noiseBasisEnumm, false);
	auto noise_size = new DfxQDecimalSpinnerControl(path + "noise_size", "", false);
	auto turbulence = new DfxQDecimalSpinnerControl(path + "turbulence", "", false);
	auto bright = new DfxQDecimalSpinnerControl(path + "bright", "", false);
	auto contrast = new DfxQDecimalSpinnerControl(path + "contrast", "", false);

	propertiesArray->addControl("Stucci Type", stucci_type);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Type", noise_type);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Basis", noise_basis);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Size", noise_size);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Turbulence", turbulence);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Bright", bright);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Contrast", contrast);
	propertiesArray->addSpacer(0, 3);

	properties->setTitle("Stucci");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_wood_texture(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	auto woodTypeEnumm = DfxSpace::getInstance()->findAbsolute(path + "wood_type")->stringEnumArray();
	auto noiseTypeEnumm = DfxSpace::getInstance()->findAbsolute(path + "noise_type")->stringEnumArray();
	auto noiseBasisEnumm = DfxSpace::getInstance()->findAbsolute(path + "noise_basis")->stringEnumArray();
	auto noiseBasis2Enumm = DfxSpace::getInstance()->findAbsolute(path + "noise_basis2")->stringEnumArray();

	// Controls
	auto wood_type = new DfxQStringComboBoxControl(path + "wood_type", "", woodTypeEnumm, false);
	auto noise_type = new DfxQStringComboBoxControl(path + "noise_type", "", noiseTypeEnumm, false);
	auto noise_basis = new DfxQStringComboBoxControl(path + "noise_basis", "", noiseBasisEnumm, false);
	auto noise_basis2 = new DfxQStringComboBoxControl(path + "noise_basis2", "", noiseBasis2Enumm, false);
	auto noise_size = new DfxQDecimalSpinnerControl(path + "noise_size", "", false);
	auto turbulence = new DfxQDecimalSpinnerControl(path + "turbulence", "", false);
	auto bright = new DfxQDecimalSpinnerControl(path + "bright", "", false);
	auto contrast = new DfxQDecimalSpinnerControl(path + "contrast", "", false);

	propertiesArray->addControl("Wood Type", wood_type);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Type", noise_type);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Basis", noise_basis);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Basis 2", noise_basis2);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Size", noise_size);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Turbulence", turbulence);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Bright", bright);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Contrast", contrast);
	propertiesArray->addSpacer(0, 3);

	properties->setTitle("Wood");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_voronoi_texture(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	auto distmetricTypeEnumm = DfxSpace::getInstance()->findAbsolute(path + "distmetric")->stringEnumArray();

	// Controls
	auto distmetric = new DfxQStringComboBoxControl(path + "distmetric", "", distmetricTypeEnumm, false);
	auto intensity = new DfxQDecimalSpinnerControl(path + "intensity", "", false);
	auto w00 = new DfxQDecimalSpinnerControl(path + "w00", "", false);
	auto w01 = new DfxQDecimalSpinnerControl(path + "w01", "", false);
	auto w02 = new DfxQDecimalSpinnerControl(path + "w02", "", false);
	auto w03 = new DfxQDecimalSpinnerControl(path + "w03", "", false);
	auto noise_size = new DfxQDecimalSpinnerControl(path + "noise_size", "", false);
	auto bright = new DfxQDecimalSpinnerControl(path + "bright", "", false);
	auto contrast = new DfxQDecimalSpinnerControl(path + "contrast", "", false);


	propertiesArray->addControl("Distmetric", distmetric);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Intensity", intensity);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("W1", w00);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("W2", w01);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("W3", w02);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("W4", w03);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Noise Size", noise_size);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Bright", bright);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Contrast", contrast);
	propertiesArray->addSpacer(0, 3);

	properties->setTitle("Voronoi");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

// Luxcore Texture Util
void DfxQMaterialNodeForm::lux_mix_tex_texture(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto amount = new DfxQDecimalSpinnerControl(path + "amount", "", false);
	auto texture1 = new DfxQDecimalSpinnerControl(path + "texture1", "", false);
	auto texture2 = new DfxQDecimalSpinnerControl(path + "texture2", "", false);

	propertiesArray->addControl("Amount", amount);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Texture1", texture1);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Texture2", texture2);
	propertiesArray->addSpacer(0, 3);

	properties->setTitle("Mix");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_clamp_texture(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto texture = new DfxQDecimalSpinnerControl(path + "texture", "", false);
	auto min = new DfxQDecimalSpinnerControl(path + "min", "", false);
	auto max = new DfxQDecimalSpinnerControl(path + "max", "", false);

	propertiesArray->addControl("Texture", texture);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Min", min);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Max", max);
	propertiesArray->addSpacer(0, 3);

	properties->setTitle("Clamp");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

void DfxQMaterialNodeForm::lux_hsv_texture(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto texture = new DfxQDecimalSpinnerControl(path + "texture", "", false);
	auto hue = new DfxQDecimalSpinnerControl(path + "hue", "", false);
	auto saturation = new DfxQDecimalSpinnerControl(path + "saturation", "", false);
	auto value = new DfxQDecimalSpinnerControl(path + "value", "", false);

	propertiesArray->addControl("Texture", texture);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Hue", hue);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Saturation", saturation);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Brightness", value);

	properties->setTitle("Hsv");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}

// OSPRay Materials
void DfxQMaterialNodeForm::osp_principled_marterial(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	// Controls
	auto base_color = new DfxQColorControl(path + "base_r", path + "base_g", path + "base_b", "", false);
	auto edge_color = new DfxQColorControl(path + "edge_r", path + "edge_g", path + "edge_b", "", false);
	auto metallic = new DfxQDecimalSpinnerControl(path + "metallic", "", false);
	auto diffuse = new DfxQDecimalSpinnerControl(path + "diffuse", "", false);
	auto specular = new DfxQDecimalSpinnerControl(path + "specular", "", false);
	auto ior = new DfxQDecimalSpinnerControl(path + "ior", "", false);
	auto transmission = new DfxQDecimalSpinnerControl(path + "transmission", "", false);
	auto transmission_color = new DfxQColorControl(path + "transmission_r", path + "transmission_g", path + "transmission_b", "", false);
	auto transmission_depth = new DfxQDecimalSpinnerControl(path + "transmission_depth", "", false);
	auto roughness = new DfxQDecimalSpinnerControl(path + "roughness", "", false);
	auto anisotropy = new DfxQDecimalSpinnerControl(path + "anisotropy", "", false);
	auto rotation = new DfxQDecimalSpinnerControl(path + "rotation", "", false);
	auto normal = new DfxQDecimalSpinnerControl(path + "normal", "", false);
	auto base_normal = new DfxQDecimalSpinnerControl(path + "base_normal", "", false);
	auto thin = new DfxQCheckBoxControl(path + "thin", "", false);
	auto thickness = new DfxQDecimalSpinnerControl(path + "thickness", "", false);
	auto backlight = new DfxQDecimalSpinnerControl(path + "backlight", "", false);
	auto coat = new DfxQDecimalSpinnerControl(path + "coat", "", false);
	auto coat_ior = new DfxQDecimalSpinnerControl(path + "coat_ior", "", false);
	auto coat_color = new DfxQColorControl(path + "coat_r", path + "coat_g", path + "coat_b", "", false);
	auto coat_thickness = new DfxQDecimalSpinnerControl(path + "coat_thickness", "", false);
	auto coat_roughness = new DfxQDecimalSpinnerControl(path + "coat_roughness", "", false);
	auto coat_normal = new DfxQDecimalSpinnerControl(path + "coat_normal", "", false);
	auto sheen = new DfxQDecimalSpinnerControl(path + "sheen", "", false);
	auto sheen_color = new DfxQColorControl(path + "sheen_r", path + "sheen_g", path + "sheen_b", "", false);
	auto shin_tint = new DfxQDecimalSpinnerControl(path + "shin_tint", "", false);
	auto sheen_roughness = new DfxQDecimalSpinnerControl(path + "sheen_roughness", "", false);
	auto opacity = new DfxQDecimalSpinnerControl(path + "opacity", "", false);

	// Set Bounds
	metallic->setMinValue(0.f); 
	metallic->setMaxValue(1.f);

	diffuse->setMinValue(0.f); 
	diffuse->setMaxValue(1.f);

	specular->setMinValue(0.f); 
	specular->setMaxValue(1.f);

	transmission->setMinValue(0.f); 
	transmission->setMaxValue(1.f);

	roughness->setMinValue(0.f); 
	roughness->setMaxValue(1.f);

	anisotropy->setMinValue(0.f); 
	anisotropy->setMaxValue(1.f);

	rotation->setMinValue(0.f); 
	rotation->setMaxValue(1.f);

	backlight->setMinValue(0.f); 
	backlight->setMaxValue(2.f);

	coat->setMinValue(0.f);
	coat->setMaxValue(1.f);

	coat_roughness->setMinValue(0.f);
	coat_roughness->setMaxValue(1.f);

	sheen->setMinValue(0.f);
	sheen->setMaxValue(1.f);

	sheen_roughness->setMinValue(0.f);
	sheen_roughness->setMaxValue(1.f);

	opacity->setMinValue(0.f);
	opacity->setMaxValue(1.f);

	propertiesArray->addControl("Base Color", base_color);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Edge Color", edge_color);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Metallic", metallic);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Diffuse", diffuse);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Specular", specular);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("IOR", ior);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Transmission", transmission);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Transmission Color", transmission_color);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Transmission Depth", transmission_depth);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Roughness", roughness);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Anisotropy", anisotropy);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Rotation", rotation);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Normal", normal);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Base Normal", base_normal);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Thin", thin);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Thickness", thickness);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Backlight", backlight);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Coat", coat);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Coat IOR", coat_ior);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Coat Color", coat_color);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Coat Thickness", coat_thickness);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Coat Roughness", coat_roughness);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Coat Normal", coat_normal);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Sheen", sheen);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Sheen Color", sheen_color);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Shin Tint", shin_tint);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Sheen Roughness", sheen_roughness);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("Opacity", opacity);

	properties->setTitle("Principled");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);
}


void DfxQMaterialNodeForm::osp_image_texture(std::string mat_path) {
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	std::string path = mat_path + "/";

	QSettings settings;
	QString initialDir = settings.value("~/io/osp_imagemap_texture", QDir::homePath()).toString();

	// Controls
	auto file = new DfxQFilePathControl(path + "file", true, false, "Image", initialDir.toStdString(), "Image Files (*.png *.jpg *.tiff)", "", false);
	auto uvrot = new DfxQDecimalSpinnerControl(path + "uv_rotation", "", false);
	auto uvsclx = new DfxQDecimalSpinnerControl(path + "uv_scale_x", "", false);
	auto uvscly = new DfxQDecimalSpinnerControl(path + "uv_scale_y", "", false);
	auto uvtransx = new DfxQDecimalSpinnerControl(path + "uv_translation_x", "", false);
	auto uvtransy = new DfxQDecimalSpinnerControl(path + "uv_translation_y", "", false);

	propertiesArray->addControl("File", file);
	propertiesArray->addSpacer(0, 3);
	propertiesArray->addControl("UV Rotation", uvrot);
	propertiesArray->addSpacer(0, 8);
	propertiesArray->addControl("UV Scale X", uvsclx);
	propertiesArray->addControl("Y", uvscly);
	propertiesArray->addSpacer(0, 8);
	propertiesArray->addControl("UV Translation X", uvtransx);
	propertiesArray->addControl("Y", uvtransy);

	properties->setTitle("Image Texture");
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);

	QObject::connect(file, &DfxQFilePathControl::rootDirChanged, [file](std::string file) {
		if (file.length() > 0) {
			QSettings settings;
			settings.setValue("~/io/osp_imagemap_texture", file.c_str());
		}
	});
}
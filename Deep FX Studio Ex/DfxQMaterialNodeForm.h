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

#include <QWidget>
#include <vector>
#include <SgNode.h>
#include <SgNodeType.h>
#include <QBoxLayout>
#include <QToolbox.h>
#include <QLineEdit>
#include <QTabWidget>
#include <QFormlayout>
#include <QScrollArea>
#include <QSpacerItem>
#include <QGroupBox>
#include <QToolBox>
#include <QTabBar>
#include <QComboBox>
#include "DfxObjectHandler.h"
#include "DfxBaseForm.h"
#include "DfxQDecimalSpinnerControl.h"
#include "DfxQIntegerSpinnerControl.h"
#include "DfxQCheckBoxControl.h"
#include "DfxQColorControl.h"
#include "DfxQComboBoxControl.h"
#include "DfxQStringComboBoxControl.h"
#include "DfxQControlArray.h"
#include "DfxQControlPanel.h"
#include "DfxItemWidgetType.h"
#include "DfxQFilePathControl.h"
#include "DfxSpace.h"
#include "DfxQMaterialNode.h"
#include "DfxBaseForm.h"

class DfxQMaterialNodeForm : public DfxBaseForm {
private:

	std::vector<std::pair<std::string, unsigned int>> luxImagemapWrapQControlenums;
	std::vector<std::pair<std::string, unsigned int>> luxImagemapChannelQControlenums;
	std::vector<std::pair<std::string, unsigned int>> luxDisplacementTypeQControlenums;

public:

	struct LuxImagemapWrap { enum { REPEAT = 0, BLACK = 1, WHITE = 2, CLAMP = 3 }; };
	struct LuxImagemapChannel { enum { DEFAULT = 0, RED = 1, GREEN = 2, BLUE = 3, ALPHA = 4, MEAN = 5, COLORED_MEAN = 6, RGB = 7 }; };
	struct LuxDisplacementType { enum { HEIGHT = 0, VECTOR = 1}; };

private:

	// Luxcore
	void lux_output(std::string path);

	void lux_matte_marterial(std::string path);
	void lux_matte_translucent_marterial(std::string path);
	void lux_emission_marterial(std::string path);
	void lux_disney_marterial(std::string path);
	void lux_glass_marterial(std::string path);
	void lux_arch_glass_marterial(std::string path);
	void lux_rough_glass_marterial(std::string path);
	void lux_car_paint_marterial(std::string path);
	void lux_cloth_marterial(std::string path);
	void lux_glossy_marterial(std::string path);
	void lux_glossy_translucent_marterial(std::string path);
	void lux_metal_marterial(std::string path);
	void lux_mirror_marterial(std::string path);
	void lux_velvet_marterial(std::string path);
	void lux_rough_matte_marterial(std::string path);
	void lux_mix_marterial(std::string path);

	void lux_imagemap_texture(std::string path);
	void lux_normalmap_texture(std::string path);
	void lux_2d_checkerboard_texture(std::string path);
	void lux_fbm_texture(std::string path);
	void lux_fresnel_color_texture(std::string path);
	void lux_blend_texture(std::string path);
	void lux_cloud_texture(std::string path);
	void lux_distorted_noise_texture(std::string path);
	void lux_magic_texture(std::string path);
	void lux_marble_texture(std::string path);
	void lux_musgrave_texture(std::string path);
	void lux_noise_texture(std::string path);
	void lux_stucci_texture(std::string path);
	void lux_wood_texture(std::string path);
	void lux_voronoi_texture(std::string path);

	void lux_mix_tex_texture(std::string path);
	void lux_clamp_texture(std::string path);
	void lux_hsv_texture(std::string path);

	// OSPRay
	void osp_principled_marterial(std::string path);
	
	void osp_image_texture(std::string path);

public:
	DfxQMaterialNodeForm();
	virtual ~DfxQMaterialNodeForm();
	bool create(DfxQMaterialNode *nodes);
	void destroy() override;
};
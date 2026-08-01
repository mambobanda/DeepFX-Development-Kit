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

#include <SgPointCloudNode.h>
#include <random>
#include <numeric>
#include "DfxGroupNode.h"
#include "DfxGeometryNode.h"
#include "DfxPointShapeTool.h"
#include "DfxPointShapeHandler.h"

std::string DfxPointShapeTool::_uniqueName = "UID_POINT_SHAPE_TOOL";

DfxPointShapeTool::DfxPointShapeTool() {

	_dispatcher = nullptr;

	distributionEnums.push_back(std::pair<std::string, unsigned int>("Uniform", Distribution::UNIFORM));
	distributionEnums.push_back(std::pair<std::string, unsigned int>("Random", Distribution::RANDOM));

}

DfxPointShapeTool::~DfxPointShapeTool() {

}

std::string DfxPointShapeTool::uniqueName() {
	return _uniqueName;
}

std::string DfxPointShapeTool::getUniqueName() {
	return _uniqueName;
}

bool DfxPointShapeTool::isReady() {
	return true;
}

void DfxPointShapeTool::onSetReferencePath(std::string path) {
	if (_dispatcher != nullptr) {
		delete _dispatcher;
		_dispatcher = nullptr;
	}

	auto any = DfxSpace::getInstance()->findAbsolute(path);

	if (any != nullptr) {
		_dispatcher = new DfxToolFlDispatcher(this);
		_dispatcher->attachBranch(any);
	}

	DfxToolNode::onSetReferencePath(path);
}

void DfxPointShapeTool::create(SgNode *parent) {
}

void DfxPointShapeTool::destroy() {
}

bool DfxPointShapeTool::evaluateOnUpdate() {
	return false;
}

bool DfxPointShapeTool::evaluateOnNewFrame() {
	return true;
}

bool DfxPointShapeTool::evaluateOnValueChanged() {
	return true;
}

void DfxPointShapeTool::evaluate(SgNode *parent) {
	DfxPointShapeHandler handler;

	if (parent->getType() == deepsg::SgNodeType::POINT_CLOUD) {

		auto pcloud = ((deepsg::SgPointCloudNode*)parent)->geo();

		if (handler.fetch(referencePath())) {

			auto dao = handler.dataObject()->rawData<fl::DataObject>();

			auto group = dao->property("group")->data<std::string>();
			auto distribution = dao->property("distribution")->data<unsigned int>();
			auto seed = dao->property("seed")->data<unsigned int>();
			
			auto min_uscale = dao->property("uniform_scale_min")->data<float>();
			auto max_uscale = dao->property("uniform_scale_max")->data<float>();

			auto min_offset_x = dao->property("min_offset_x")->data<float>();
			auto min_offset_y = dao->property("min_offset_y")->data<float>();
			auto min_offset_z = dao->property("min_offset_z")->data<float>();
			auto max_offset_x = dao->property("max_offset_x")->data<float>();
			auto max_offset_y = dao->property("max_offset_y")->data<float>();
			auto max_offset_z = dao->property("max_offset_z")->data<float>();

			auto min_scale_x = dao->property("min_scale_x")->data<float>();
			auto min_scale_y = dao->property("min_scale_y")->data<float>();
			auto min_scale_z = dao->property("min_scale_z")->data<float>();
			auto max_scale_x = dao->property("max_scale_x")->data<float>();
			auto max_scale_y = dao->property("max_scale_y")->data<float>();
			auto max_scale_z = dao->property("max_scale_z")->data<float>();

			auto min_rotation_x = dao->property("min_rotation_x")->data<float>();
			auto min_rotation_y = dao->property("min_rotation_y")->data<float>();
			auto min_rotation_z = dao->property("min_rotation_z")->data<float>();
			auto max_rotation_x = dao->property("max_rotation_x")->data<float>();
			auto max_rotation_y = dao->property("max_rotation_y")->data<float>();
			auto max_rotation_z = dao->property("max_rotation_z")->data<float>();

			// Position Rand
			std::uniform_real_distribution<float> posXDistr(min_offset_x, std::max(min_offset_x, max_offset_x));
			std::uniform_real_distribution<float> posYDistr(min_offset_y, std::max(min_offset_y, max_offset_y));
			std::uniform_real_distribution<float> posZDistr(min_offset_z, std::max(min_offset_z, max_offset_z));
			
			std::default_random_engine posXGen(std::min(seed + 1, std::numeric_limits<unsigned int>::max()));
			std::default_random_engine posYGen(std::min(seed + 2, std::numeric_limits<unsigned int>::max()));
			std::default_random_engine posZGen(std::min(seed + 3, std::numeric_limits<unsigned int>::max()));

			// Rotation Rand
			std::uniform_real_distribution<float> rotXDistr(min_rotation_x, std::max(min_rotation_x, max_rotation_x));
			std::uniform_real_distribution<float> rotYDistr(min_rotation_y, std::max(min_rotation_y, max_rotation_y));
			std::uniform_real_distribution<float> rotZDistr(min_rotation_z, std::max(min_rotation_z, max_rotation_z));

			std::default_random_engine rotXGen(std::min(seed + 4, std::numeric_limits<unsigned int>::max()));
			std::default_random_engine rotYGen(std::min(seed + 5, std::numeric_limits<unsigned int>::max()));
			std::default_random_engine rotZGen(std::min(seed + 6, std::numeric_limits<unsigned int>::max()));

			// Scale Rand
			std::uniform_real_distribution<float> sclXDistr(min_scale_x, std::max(min_scale_x, max_scale_x));
			std::uniform_real_distribution<float> sclYDistr(min_scale_y, std::max(min_scale_y, max_scale_y));
			std::uniform_real_distribution<float> sclZDistr(min_scale_z, std::max(min_scale_z, max_scale_z));

			std::default_random_engine sclXGen(std::min(seed + 7, std::numeric_limits<unsigned int>::max()));
			std::default_random_engine sclYGen(std::min(seed + 8, std::numeric_limits<unsigned int>::max()));
			std::default_random_engine sclZGen(std::min(seed + 9, std::numeric_limits<unsigned int>::max()));

			// Uniform Scale Rand
			std::uniform_real_distribution<float> usclDistr(min_uscale, std::max(min_uscale, max_uscale));

			std::default_random_engine usclGen(std::min(seed + 10, std::numeric_limits<unsigned int>::max()));


			deepsg::SgNode *snode = nullptr;
			std::vector<std::string> shapes;
			auto any = DfxSpace::getInstance()->findAbsolute(group);

			if (any != nullptr) {

				if (any->type() == fl::type::DATA_OBJECT) {
					auto dao = any->rawData<fl::DataObject>();

					if (dao->hasProperty("sg_group_node")) {
						snode = any->data<fl::DataObject>().property("sg_group_node")->rawData<DfxGroupNode>();

						if (snode != nullptr) {
							for (auto child : snode->children()) {
								if (child->getType() == deepsg::SgNodeType::GEOMETRY) {
									shapes.push_back(child->referencePath());
								}
							}
						}
					}
				}

			}

			std::uniform_int_distribution<unsigned int> randomDistribution(0,  (unsigned int)(shapes.size() - 1));
			std::default_random_engine randomGenerator(std::min(seed, std::numeric_limits<unsigned int>::max()));

			if (pcloud->size() > 0 && !shapes.empty()) {

				auto shapeChannel = pcloud->channel("@instanceShapePath");
				auto originalPosChannel = pcloud->channel("@position");
				auto posChannel = pcloud->channel("@positionOverride");
				auto rotChannel = pcloud->channel("@rotationOverride");
				auto sclChannel = pcloud->channel("@scaleOverride");

				if (shapeChannel == nullptr) {
					pcloud->addAttribute<std::string>("@instanceShapePath");
					shapeChannel = pcloud->channel("@instanceShapePath");
				}

				if (posChannel == nullptr) {
					pcloud->addAttribute<glm::vec3>("@positionOverride");
					posChannel = pcloud->channel("@positionOverride");
				}

				if (rotChannel == nullptr) {
					pcloud->addAttribute<glm::vec3>("@rotationOverride");
					rotChannel = pcloud->channel("@rotationOverride");
				}

				if (sclChannel == nullptr) {
					pcloud->addAttribute<glm::vec3>("@scaleOverride");
					sclChannel = pcloud->channel("@scaleOverride");
				}

				int cyclicCounter = 0;
				int psize = pcloud->size();
				for (size_t inx = 0; inx < pcloud->size(); inx++) {

					if (distribution == Distribution::UNIFORM) {
						shapeChannel->at<std::string>()[inx] = shapes[cyclicCounter];
						if (++cyclicCounter == shapes.size()) cyclicCounter = 0;
					}
					else if(distribution == Distribution::RANDOM){
						auto sindex = randomDistribution(randomGenerator);
						shapeChannel->at<std::string>()[inx] = shapes[sindex];
					}

					auto uniformFactor = usclDistr(usclGen);
					auto npos = originalPosChannel->at<glm::vec3>()[inx] + glm::vec3(posXDistr(posXGen), posYDistr(posYGen), posZDistr(posZGen));
					auto stretch = glm::vec3(sclXDistr(sclXGen), sclYDistr(sclYGen), sclZDistr(sclZGen));
					auto fscale = stretch * (uniformFactor / 100.f);

					posChannel->at<glm::vec3>()[inx] = npos;
					rotChannel->at<glm::vec3>()[inx] = glm::vec3(rotXDistr(rotXGen), rotYDistr(rotYGen), rotZDistr(rotZGen));
					sclChannel->at<glm::vec3>()[inx] = fscale;
				}
			}

			parent->emitSignal(deepsg::SgNode::Signal::UPDATED);
		}
	}
}

std::vector<std::pair<QWidget*, QString>> DfxPointShapeTool::propertyForms() {
	std::pair<QWidget*, QString> grid(form(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> {grid};
}

QWidget *DfxPointShapeTool::form() {

	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto generalControlPanel = new DfxControlPanel();
	auto offsetControlPanel  = new DfxControlPanel();
	auto scaleControlPanel   = new DfxControlPanel();
	auto rotationControlPanel = new DfxControlPanel();

	auto gridControlPanel = new DfxControlPanel();
	/*auto circularControlPanel = new DfxControlPanel();
	auto brickControlPanel = new DfxControlPanel();
	auto helixControlPanel = new DfxControlPanel();*/

	DfxQControlArray *generalControlArray = new DfxQControlArray();
	DfxQControlArray *offsetControlArray = new DfxQControlArray();
	DfxQControlArray *scaleControlArray = new DfxQControlArray();
	DfxQControlArray *rotationControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// General Controls
	auto group = new DfxQFluentPathControl(path + "group", "", false);
	auto distribution = new DfxQComboBoxControl(path + "distribution", "", distributionEnums, false);
	auto seed = new DfxQIntegerSpinnerControl(path + "seed", "", false);

	// Offset Controls
	auto min_offset_x = new DfxQDecimalSpinnerControl(path + "min_offset_x", "", false);
	auto min_offset_y = new DfxQDecimalSpinnerControl(path + "min_offset_y", "", false);
	auto min_offset_z = new DfxQDecimalSpinnerControl(path + "min_offset_z", "", false);

	auto max_offset_x = new DfxQDecimalSpinnerControl(path + "max_offset_x", "", false);
	auto max_offset_y = new DfxQDecimalSpinnerControl(path + "max_offset_y", "", false);
	auto max_offset_z = new DfxQDecimalSpinnerControl(path + "max_offset_z", "", false);

	// Uniform Scale
	auto min_uscale = new DfxQDecimalSpinnerControl(path + "uniform_scale_min", "", false);
	auto max_uscale = new DfxQDecimalSpinnerControl(path + "uniform_scale_max", "", false);

	// Scale Controls
	auto min_scale_x = new DfxQDecimalSpinnerControl(path + "min_scale_x", "", false);
	auto min_scale_y = new DfxQDecimalSpinnerControl(path + "min_scale_y", "", false);
	auto min_scale_z = new DfxQDecimalSpinnerControl(path + "min_scale_z", "", false);

	auto max_scale_x = new DfxQDecimalSpinnerControl(path + "max_scale_x", "", false);
	auto max_scale_y = new DfxQDecimalSpinnerControl(path + "max_scale_y", "", false);
	auto max_scale_z = new DfxQDecimalSpinnerControl(path + "max_scale_z", "", false);

	// Rotation Controls
	auto min_rotation_x = new DfxQDecimalSpinnerControl(path + "min_rotation_x", "", false);
	auto min_rotation_y = new DfxQDecimalSpinnerControl(path + "min_rotation_y", "", false);
	auto min_rotation_z = new DfxQDecimalSpinnerControl(path + "min_rotation_z", "", false);

	auto max_rotation_x = new DfxQDecimalSpinnerControl(path + "max_rotation_x", "", false);
	auto max_rotation_y = new DfxQDecimalSpinnerControl(path + "max_rotation_y", "", false);
	auto max_rotation_z = new DfxQDecimalSpinnerControl(path + "max_rotation_z", "", false);

	seed->setMinValue(1);

	// Distribution
	generalControlArray->addControl("Group", group);
	generalControlArray->addSpacer(0, 3);
	generalControlArray->addControl("Distribution", distribution);
	generalControlArray->addSpacer(0, 3);
	generalControlArray->addControl("Random Seed", seed);

	generalControlPanel->setTitle("General");
	generalControlPanel->setWidget(generalControlArray);

	// Offset
	offsetControlArray->addControl("Min Offset X", min_offset_x);
	offsetControlArray->addControl("Y", min_offset_y);
	offsetControlArray->addControl("Z", min_offset_z);
	offsetControlArray->addSpacer(0, 5);
	offsetControlArray->addControl("Max Offset X", max_offset_x);
	offsetControlArray->addControl("Y", max_offset_y);
	offsetControlArray->addControl("Z", max_offset_z);

	offsetControlPanel->setTitle("Offset");
	offsetControlPanel->setWidget(offsetControlArray);

	// Scale
	scaleControlArray->addControl("Min Scale(%)", min_uscale);
	scaleControlArray->addControl("Max Scale(%)", max_uscale);
	scaleControlArray->addSpacer(0, 5);
	scaleControlArray->addControl("Min Stretch X", min_scale_x);
	scaleControlArray->addControl("Y", min_scale_y);
	scaleControlArray->addControl("Z", min_scale_z);
	scaleControlArray->addSpacer(0, 5);
	scaleControlArray->addControl("Max Stretch X", max_scale_x);
	scaleControlArray->addControl("Y", max_scale_y);
	scaleControlArray->addControl("Z", max_scale_z);

	scaleControlPanel->setTitle("Scale");
	scaleControlPanel->setWidget(scaleControlArray);

	// Rotation
	rotationControlArray->addControl("Min Rotation X", min_rotation_x);
	rotationControlArray->addControl("Y", min_rotation_y);
	rotationControlArray->addControl("Z", min_rotation_z);
	rotationControlArray->addSpacer(0, 5);
	rotationControlArray->addControl("Max Rotation X", max_rotation_x);
	rotationControlArray->addControl("Y", max_rotation_y);
	rotationControlArray->addControl("Z", max_rotation_z);

	rotationControlPanel->setTitle("Rotation");
	rotationControlPanel->setWidget(rotationControlArray);

	// Main layout
	main_layout->addWidget(generalControlPanel);
	main_layout->addWidget(offsetControlPanel);
	main_layout->addWidget(scaleControlPanel);
	main_layout->addWidget(rotationControlPanel);

	widget->setLayout(main_layout);

	return widget;
}
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

#include <SgPointCloudNode.h>
#include "DfxGeneratePointsTool.h"
#include "DfxGeneratePointsHandler.h"

std::string DfxGeneratePointsTool::_uniqueName = "UID_GENERATE_POINTS_TOOL";

DfxGeneratePointsTool::DfxGeneratePointsTool() {

	_dispatcher = nullptr;

	typeQControlenums.push_back(std::pair<std::string, unsigned int>("Grid", GRID));
	//typeQControlenums.push_back(std::pair<std::string, unsigned int>("Circular", CIRCULAR));
	//typeQControlenums.push_back(std::pair<std::string, unsigned int>("Brick", BRICK));
	//typeQControlenums.push_back(std::pair<std::string, unsigned int>("Helix", HELIX));
}

DfxGeneratePointsTool::~DfxGeneratePointsTool() {

}

std::string DfxGeneratePointsTool::uniqueName() {
	return _uniqueName;
}

std::string DfxGeneratePointsTool::getUniqueName() {
	return _uniqueName;
}

bool DfxGeneratePointsTool::isReady() {
	return true;
}

void DfxGeneratePointsTool::onSetReferencePath(std::string path) {
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

void DfxGeneratePointsTool::create(SgNode *parent) {
}

void DfxGeneratePointsTool::destroy() {
}

bool DfxGeneratePointsTool::evaluateOnUpdate() {
	return false;
}

bool DfxGeneratePointsTool::evaluateOnNewFrame() {
	return false;
}

bool DfxGeneratePointsTool::evaluateOnValueChanged() {
	return true;
}

void DfxGeneratePointsTool::evaluate(SgNode *parent) {
	DfxGeneratePointsHandler handler;

	if (parent->getType() == deepsg::SgNodeType::POINT_CLOUD) {

		auto pcloud = ((deepsg::SgPointCloudNode*)parent)->geo();

		if (handler.fetch(referencePath())) {

			auto dao = handler.dataObject()->rawData<fl::DataObject>();
			
			auto reset	  = dao->property("reset")->data<bool>();
			auto gen_type = dao->property("type")->data<size_t>();

			if (reset) pcloud->clear();

			if (gen_type == DfxGeneratePointsTool::GRID) {

				auto xcount = dao->property("grid_count_x")->data<size_t>();
				auto ycount = dao->property("grid_count_y")->data<size_t>();
				auto zcount = dao->property("grid_count_z")->data<size_t>();

				auto xspacing_in = dao->property("grid_spacing_x")->data<float>();
				auto yspacing_in = dao->property("grid_spacing_y")->data<float>();
				auto zspacing_in = dao->property("grid_spacing_z")->data<float>();

				auto rand_seed = dao->property("grid_rand_seed")->data<size_t>();

				auto rand_min = dao->property("grid_rand_pos_min_offset")->data<float>();
				auto rand_max = dao->property("grid_rand_pos_max_offset")->data<float>();

				if (rand_max < rand_min) rand_max = rand_min;

				std::time_t time = std::time(nullptr);
				std::uniform_real_distribution<> distPos(rand_min, rand_max);
				unsigned int k = 10;

				auto xbase = -((xcount - 1) * xspacing_in) * 0.5f;
				auto ybase = -((ycount - 1) * yspacing_in) * 0.5f;
				auto zbase = -((zcount - 1) * zspacing_in) * 0.5f;

				float xspacing = xbase;
				float yspacing = ybase;
				float zspacing = zbase;

				if (xcount > 0.0f || ycount > 0.0f || zcount > 0.0f) {

					std::vector<glm::vec3> output; 

					for (int x = 0; x < xcount; x++) {
						for (int y = 0; y < ycount; y++) {
							for (int z = 0; z < zcount; z++) {

								std::default_random_engine genx((++k)*(unsigned int)rand_seed);
								std::default_random_engine geny((++k)*(unsigned int)rand_seed + 100);
								std::default_random_engine genz((++k)*(unsigned int)rand_seed + 200);

								float posx = (float)distPos(genx);
								float posy = (float)distPos(geny);
								float posz = (float)distPos(genz);

								glm::vec3 newPoint;

								newPoint.x = xspacing + posx;
								newPoint.y = yspacing + posy;
								newPoint.z = zspacing + posz;

								output.push_back(newPoint);

								zspacing += zspacing_in;
							}

							zspacing = zbase;
							yspacing += yspacing_in;
						}

						zspacing = zbase;
						yspacing = ybase;
						xspacing += xspacing_in;
					}

					pcloud->resize(output.size());

					for (int i = 0; i < output.size(); i++) {
						pcloud->at<glm::vec3>("@position")[i] = output[i];
					}
				}
			}

			parent->emitSignal(deepsg::SgNode::Signal::UPDATED);
		}
	}
}

std::vector<std::pair<QWidget*, QString>> DfxGeneratePointsTool::propertyForms() {
	std::pair<QWidget*, QString> grid(form(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> {grid};
}

QWidget *DfxGeneratePointsTool::form() {

	QWidget *widget = new QWidget;

	QWidget *gridWidget = new QWidget;
	//QWidget *circularWidget = new QWidget;
	//QWidget *brickWidget = new QWidget;
	//QWidget *helixWidget = new QWidget;
	
	DfxQStackControl *widgetStack = new DfxQStackControl;

	QVBoxLayout *main_layout = new QVBoxLayout;
	QVBoxLayout *grid_widget_layout = new QVBoxLayout;
	/*QVBoxLayout *circular_widget_layout = new QVBoxLayout;
	QVBoxLayout *brick_widget_layout = new QVBoxLayout;
	QVBoxLayout *helix_widget_layout = new QVBoxLayout;*/

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	grid_widget_layout->setContentsMargins(0, 0, 0, 0);
	grid_widget_layout->setAlignment(Qt::AlignTop);

	/*circular_widget_layout->setContentsMargins(0, 0, 0, 0);
	circular_widget_layout->setAlignment(Qt::AlignTop);

	brick_widget_layout->setContentsMargins(0, 15, 0, 0);
	brick_widget_layout->setAlignment(Qt::AlignTop);

	helix_widget_layout->setContentsMargins(0, 15, 0, 0);
	helix_widget_layout->setAlignment(Qt::AlignTop);*/

	auto typeControlPanel = new DfxControlPanel();

	auto gridControlPanel  = new DfxControlPanel();
	/*auto circularControlPanel = new DfxControlPanel();
	auto brickControlPanel = new DfxControlPanel();
	auto helixControlPanel = new DfxControlPanel();*/

	DfxQControlArray *typeControlArray = new DfxQControlArray();

	DfxQControlArray *gridControlArray = new DfxQControlArray();
	//DfxQControlArray *circularControlArray = new DfxQControlArray();
	//DfxQControlArray *brickControlArray = new DfxQControlArray();
	//DfxQControlArray *helixControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// Type Controls
	auto type = new DfxQComboBoxControl(path + "type", "", typeQControlenums, false);

	// Grid Controls
	auto grid_count_x = new DfxQIntegerSpinnerControl(path + "grid_count_x", "", false, nullptr, DfxQIntegerSpinnerControl::SIZE_T);
	auto grid_count_y = new DfxQIntegerSpinnerControl(path + "grid_count_y", "", false, nullptr, DfxQIntegerSpinnerControl::SIZE_T);
	auto grid_count_z = new DfxQIntegerSpinnerControl(path + "grid_count_z", "", false, nullptr, DfxQIntegerSpinnerControl::SIZE_T);

	auto grid_spacing_x = new DfxQDecimalSpinnerControl(path + "grid_spacing_x", "", false);
	auto grid_spacing_y = new DfxQDecimalSpinnerControl(path + "grid_spacing_y", "", false);
	auto grid_spacing_z = new DfxQDecimalSpinnerControl(path + "grid_spacing_z", "", false);

	auto grid_rand_seed		 = new DfxQIntegerSpinnerControl(path + "grid_rand_seed", "", false, nullptr, DfxQIntegerSpinnerControl::SIZE_T);
	auto grid_pos_min_offset = new DfxQDecimalSpinnerControl(path + "grid_rand_pos_min_offset", "", false);
	auto grid_pos_max_offset = new DfxQDecimalSpinnerControl(path + "grid_rand_pos_max_offset", "", false);
	/*auto grid_rot_offset = new DfxQDecimalSpinnerControl(path + "grid_rand_rot_offset", false);
	auto grid_scl_offset = new DfxQDecimalSpinnerControl(path + "grid_rand_scl_offset", false);*/

	// Min Max Constraints
	grid_count_x->setMinValue(1);
	grid_count_y->setMinValue(1);
	grid_count_z->setMinValue(1);

	grid_rand_seed->setMinValue(1);
	grid_pos_min_offset->setMinValue(0.f);
	grid_pos_min_offset->setMinValue(0.f);

	// Type
	typeControlArray->addControl("Type", type);

	typeControlPanel->setTitle("Generator Type");
	typeControlPanel->setWidget(typeControlArray);

	// Grid
	gridControlArray->addControl("Count X", grid_count_x);
	gridControlArray->addControl("Y", grid_count_y);
	gridControlArray->addControl("Z", grid_count_z);
	gridControlArray->addSpacer(0, 5);
	gridControlArray->addControl("Spacing X", grid_spacing_x);
	gridControlArray->addControl("Y", grid_spacing_y);
	gridControlArray->addControl("Z", grid_spacing_z);
	gridControlArray->addSpacer(0, 5);
	gridControlArray->addControl("Rand Seed", grid_rand_seed);
	gridControlArray->addSpacer(0, 5);
	gridControlArray->addControl("Rand Min Offset", grid_pos_min_offset);
	gridControlArray->addControl("Rand Max Offset", grid_pos_max_offset);

	gridControlPanel->setTitle("Grid");
	gridControlPanel->setWidget(gridControlArray);

	grid_widget_layout->addWidget(gridControlPanel);
	gridWidget->setLayout(grid_widget_layout);

	// Stack
	widgetStack->addPage(gridWidget);
	
	widgetStack->changeCurrentPage(type->comboBox()->currentIndex());

	// Main layout
	main_layout->addWidget(typeControlPanel);
	main_layout->addWidget(widgetStack);

	widget->setLayout(main_layout);

	// Signal  & Slots
	QObject::connect(type->comboBox(), QOverload<int>::of(&QComboBox::activated), widgetStack, &DfxQStackControl::changeCurrentPage);

	return widget;
}


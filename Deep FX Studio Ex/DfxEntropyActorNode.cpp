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


#include "DfxEntropyActorNode.h"

std::string DfxEntropyActorNode::_uniqueName = "UID_ENTROPY_ACTOR_NODE";

DfxEntropyActorNode::DfxEntropyActorNode(deepsg::SgStageNode *st) : DfxSimulationStageActorBase(st, entropy::type::SOURCE) {
	setActorState(DfxEntropyActorNode::ActorState::SPAWN);

	std::string targetPath = "";

	sourceQControlenums.push_back(std::pair<std::string, unsigned int>("Source", Source::SOURCE));
	sourceQControlenums.push_back(std::pair<std::string, unsigned int>("Obstacle", Source::OBSTACLE));

}

DfxEntropyActorNode::~DfxEntropyActorNode() {

}

std::string DfxEntropyActorNode::uniqueName() {
	return _uniqueName;
}

std::string DfxEntropyActorNode::getUniqueName() {
	return _uniqueName;
}

bool DfxEntropyActorNode::isReady() {
	return false;
}

std::string DfxEntropyActorNode::sourcePath() {
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {
		return any->rawData<fl::DataObject>()->property("source_path")->data<std::string>();
	}

	return "";
}

void DfxEntropyActorNode::setSourcePath(std::string ref) {
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {
		any->rawData<fl::DataObject>()->property("source_path")->setData<std::string>(ref);
	}
}

size_t DfxEntropyActorNode::actorState() {
	return _actorState;
}

void DfxEntropyActorNode::setActorState(size_t at) {
	_actorState = at;
}

std::vector<std::pair<QWidget*, QString>> DfxEntropyActorNode::propertyForms() {
	std::pair<QWidget*, QString> properties(actorPropertiesForm(), "Source");

	return std::vector<std::pair<QWidget*, QString>> { properties};
}

QWidget *DfxEntropyActorNode::actorPropertiesForm() {

	QWidget *widget = new QWidget;
	QWidget *sourceWidget = new QWidget;
	QWidget *obstacleWidget = new QWidget;
	DfxQStackControl *widgetStack = new DfxQStackControl;

	QVBoxLayout *main_layout = new QVBoxLayout;
	QVBoxLayout *source_widget_layout = new QVBoxLayout;
	QVBoxLayout *obstacle_widget_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	source_widget_layout->setContentsMargins(0, 0, 0, 0);
	source_widget_layout->setAlignment(Qt::AlignTop);

	obstacle_widget_layout->setContentsMargins(0, 15, 0, 0);
	obstacle_widget_layout->setAlignment(Qt::AlignTop);

	auto objectControlPanel = new DfxControlPanel();

	auto sourceControlPanel = new DfxControlPanel();

	auto obstacleControlPanel = new DfxControlPanel();

	DfxQControlArray *objectControlArray = new DfxQControlArray();

	DfxQControlArray *sourceControlArray = new DfxQControlArray();

	DfxQControlArray *obstacleControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	auto source_path = new DfxQFluentPathControl(path + "source_path", "", false);
	auto actor_type = new DfxQComboBoxControl(path + "actor_type", "", sourceQControlenums, false);

	auto constant_inflow = new DfxQCheckBoxControl(path + "constant_inflow", "", false);
	auto constant_temperature = new DfxQCheckBoxControl(path + "constant_temperature", "", false);
	auto constant_velocity = new DfxQCheckBoxControl(path + "constant_velocity", "", false);

	/*auto inflow_rate_type = new DfxQIntegerSpinnerControl(path + "inflow_rate_type", "", false);
	auto inflow_rate = new DfxQDecimalSpinnerControl(path + "inflow_rate", "", false);*/

	auto inflow_density = new DfxQDecimalSpinnerControl(path + "inflow_density", "", false);
	auto inflow_temperature = new DfxQDecimalSpinnerControl(path + "inflow_temperature", "", false);

	auto inflow_velocity_x = new DfxQDecimalSpinnerControl(path + "inflow_velocity_x", "", false);
	auto inflow_velocity_y = new DfxQDecimalSpinnerControl(path + "inflow_velocity_y", "", false);
	auto inflow_velocity_z = new DfxQDecimalSpinnerControl(path + "inflow_velocity_z", "", false);

	// Actor Panel
	objectControlArray->addControl("Source Path", source_path);
	objectControlArray->addSpacer(0, 3);
	objectControlArray->addControl("Actor", actor_type);

	objectControlPanel->setTitle("Actor");
	objectControlPanel->setWidget(objectControlArray);

	// Source Panel
	sourceControlArray->addControl("Constant Inflow", constant_inflow);
	sourceControlArray->addSpacer(0, 3);
	sourceControlArray->addControl("Constant Temperature", constant_temperature);
	sourceControlArray->addSpacer(0, 3); 
	sourceControlArray->addControl("Constant Velocity", constant_velocity);
	sourceControlArray->addSpacer(0, 6);

	sourceControlArray->addControl("Inflow Density", inflow_density);
	sourceControlArray->addSpacer(0, 3);
	sourceControlArray->addControl("Inflow Temperature", inflow_temperature);
	sourceControlArray->addSpacer(0, 6);
	
	sourceControlArray->addControl("Velocity X", inflow_velocity_x);
	sourceControlArray->addControl("Y", inflow_velocity_y);
	sourceControlArray->addControl("Z", inflow_velocity_z);

	sourceControlPanel->setTitle("Source Settings");
	sourceControlPanel->setWidget(sourceControlArray);

	source_widget_layout->addWidget(sourceControlPanel);
	sourceWidget->setLayout(source_widget_layout);

	// Obstacle
	obstacleControlPanel->setTitle("Obstacle Settings");
	obstacleControlPanel->setWidget(obstacleControlArray);

	obstacle_widget_layout->addWidget(obstacleControlPanel);
	obstacleWidget->setLayout(obstacle_widget_layout);

	// Stack
	widgetStack->addPage(sourceWidget);
	widgetStack->addPage(obstacleWidget);

	widgetStack->changeCurrentPage(actor_type->comboBox()->currentIndex());

	// Layouts
	main_layout->addWidget(objectControlPanel);
	main_layout->addWidget(widgetStack);

	widget->setLayout(main_layout);

	// Signal & Slots
	QObject::connect(actor_type->comboBox(), QOverload<int>::of(&QComboBox::activated), widgetStack, &DfxQStackControl::changeCurrentPage);
	
	return widget;
}
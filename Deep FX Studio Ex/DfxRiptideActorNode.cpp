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
#include "DfxFluentSetManager.h"
#include "DfxFluentStreamCollection.h"
#include "DfxRiptideActorNode.h"
#include "DfxRiptideStageNode.h"

std::string DfxRiptideActorNode::_uniqueName = "UID_RIPTIDE_ACTOR_NODE";

DfxRiptideActorNode::DfxRiptideActorNode(deepsg::SgStageNode *st) : DfxSimulationStageActorBase(st, riptide::type::ACTOR) {
	_isReady = false;

	actorTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Fluid", ActorType::FLUID));
	actorTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Collision Object", ActorType::COLLISION_OBJECT));
}

DfxRiptideActorNode::~DfxRiptideActorNode() {
	
}

size_t DfxRiptideActorNode::actorState() {
	return _actorState;
}

void DfxRiptideActorNode::setActorState(size_t as) {
	_actorState = as;
}

bool DfxRiptideActorNode::isReady() {
	return _isReady;
}

std::string DfxRiptideActorNode::uniqueName() {
	return _uniqueName;
}

std::string DfxRiptideActorNode::getUniqueName() {
	return _uniqueName;
}

std::string DfxRiptideActorNode::sourcePath() {
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {
		return any->rawData<fl::DataObject>()->property("source_path")->data<std::string>();
	}

	return "";
}

void DfxRiptideActorNode::setSourcePath(std::string ref) {
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {
		any->rawData<fl::DataObject>()->property("source_path")->setData<std::string>(ref);
	}
}

void DfxRiptideActorNode::spawn() {
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	auto riptideStage = (DfxRiptideStageNode*)stage();

	if (any != nullptr) {
		auto dao = any->rawData<fl::DataObject>();

		auto spath = dao->property("source_path")->data<std::string>();
		auto samplesPath = dao->property("samples")->data<std::string>();
		auto fluent_set  = dao->property("fluent_set")->data<std::string>();
		auto actor_type  = dao->property("actor_type")->data<int>();
		// auto source_transform = dao->property("use_source_transform")->data<bool>();
		auto source_transform = true;

		auto fluid_group_id = dao->property("fluid_group_id")->data<int>();
		auto fluid_use_viscosity = dao->property("fluid_use_viscosity")->data<bool>();
		auto fluid_viscosity = dao->property("fluid_viscosity")->data<float>();
		auto fluid_use_vorticity = dao->property("fluid_use_vorticity")->data<bool>();
		auto fluid_vorticity = dao->property("fluid_vorticity")->data<float>();
		auto fluid_use_surface_tension = dao->property("fluid_use_surface_tension")->data<bool>();
		auto fluid_surface_tension = dao->property("fluid_surface_tension")->data<float>();

		auto fluid_frozen = dao->property("fluid_freeze")->data<bool>();

		auto fluid_initial_velocity_x = dao->property("fluid_initial_velocity_x")->data<float>();
		auto fluid_initial_velocity_y = dao->property("fluid_initial_velocity_y")->data<float>();
		auto fluid_initial_velocity_z = dao->property("fluid_initial_velocity_z")->data<float>();

		auto fluid_color_r = dao->property("fluid_color_r")->data<float>();
		auto fluid_color_g = dao->property("fluid_color_g")->data<float>();
		auto fluid_color_b = dao->property("fluid_color_b")->data<float>();

		auto collision_kinematic		= dao->property("collision_kinematic")->data<bool>();
		auto collsiion_use_viscosity	= dao->property("collsiion_use_viscosity")->data<bool>();
		auto collision_viscosity		= dao->property("collision_viscosity")->data<float>();

		ds::PointCloud *pcloud = nullptr;
		glm::mat4 xform(1.f);

		if (fluent_set.empty()) {
			DfxGlobal::console::println("Riptide: Actors & Props must have sets, please specify set name.");
			return;
		}

		if (_source == Source::GEOMETRY) {
			DfxObjectHandler handler;

			if (handler.fetch(spath)) {

				auto geoNode = (deepsg::SgGeoNode*)handler.geoNode();

				auto posMat = glm::translate(glm::mat4(1.f), geoNode->position());
				auto rotMat = geoNode->rotationMatrix();
				
				if (source_transform) xform = posMat * rotMat;
			}
			else {
				return;
			}
		}

		auto samplesAny = DfxSpace::getInstance()->findAbsolute(samplesPath);

		if (samplesAny != nullptr) {

			if (samplesAny->type() == fl::type::DATA_OBJECT) {

				auto samplesDao = samplesAny->rawData<fl::DataObject>();

				if (samplesDao->hasProperty("sg_point_cloud_node")) {

					auto pcloudNode = samplesDao->property("sg_point_cloud_node")->rawData<deepsg::SgPointCloudNode>();
					pcloud = ((deepsg::SgPointCloudNode*)pcloudNode)->geo();
				}
			}
		}

		if (pcloud != nullptr) {
			if (pcloud->size() > 0) {

				size_t streamOffset = DfxFluentStreamCollection::getInstance()->growCollection(pcloud->size());
				auto positionStream = DfxFluentStreamCollection::getInstance()->atrributeMap().at<glm::vec3>("@position");
				auto positionAttribute = pcloud->at<glm::vec3>("@position");

				auto d_attributes = riptideStage->fetchDefaultStreamAttributes(true);

				if (!d_attributes.success) return;

				auto agent = new DfxRiptideAgent;
				DfxFluentSet *set = DfxFluentSetManager::getInstance()->getSet(fluent_set);
				
				if (set == nullptr) {
					set = new DfxFluentSet(DfxFluentSet::STREAM);
					DfxFluentSetManager::getInstance()->addSet(fluent_set, set);
				}		

				set->setIsVisible(false);

				glm::vec3 initialVel(fluid_initial_velocity_x, fluid_initial_velocity_y, fluid_initial_velocity_z);
				for (size_t inx = 0; inx < pcloud->size(); inx++) {

					positionStream[streamOffset + inx] = glm::vec3(xform * glm::vec4(positionAttribute[inx], 1.f));
					d_attributes.alive[streamOffset + inx] = 1;
					d_attributes.update[streamOffset + inx] = 0;
					
					if (actor_type == DfxRiptideActorNode::ActorType::FLUID) {
						d_attributes.sph_class[streamOffset + inx] = DfxRiptideActorNode::ActorType::FLUID;

						d_attributes.velocity[streamOffset + inx] = initialVel;

						d_attributes.use_viscosity[streamOffset + inx] = fluid_use_viscosity;
						d_attributes.viscosity[streamOffset + inx] = fluid_viscosity;
						d_attributes.use_vorticity[streamOffset + inx] = fluid_use_vorticity;
						d_attributes.vorticity[streamOffset + inx] = fluid_vorticity;
						d_attributes.use_tension[streamOffset + inx] = fluid_use_surface_tension;
						d_attributes.tension[streamOffset + inx] = fluid_surface_tension;

						d_attributes.frozen[streamOffset + inx] = fluid_frozen;

					}else if (actor_type == DfxRiptideActorNode::ActorType::COLLISION_OBJECT) {
						d_attributes.sph_class[streamOffset + inx] = DfxRiptideActorNode::ActorType::COLLISION_OBJECT;

						d_attributes.use_viscosity[streamOffset + inx] = collsiion_use_viscosity;
						d_attributes.viscosity[streamOffset + inx] = collision_viscosity;				
					}		

					set->indices().push_back(streamOffset + inx);

				}

				if (actor_type == DfxRiptideActorNode::ActorType::FLUID) agent->type = DfxRiptideActorNode::ActorType::FLUID;
				else if (actor_type == DfxRiptideActorNode::ActorType::COLLISION_OBJECT) agent->type = DfxRiptideActorNode::ActorType::COLLISION_OBJECT;
				agent->offset = streamOffset;
				agent->group = fluid_group_id;
				agent->color = glm::vec3(fluid_color_r, fluid_color_g, fluid_color_b);
				agent->size = pcloud->size();

				auto riptide = (DfxRiptideStageNode*)stage();
				riptide->queueStreamAgent(agent);

			}
		}
	}

}

unsigned int DfxRiptideActorNode::source() {
	return _source;
}

void DfxRiptideActorNode::setSource(unsigned int src) {
	_source = src;
}

std::vector<std::pair<QWidget*, QString>> DfxRiptideActorNode::propertyForms() {

	std::pair<QWidget*, QString> properties(actorPropertiesForm(), "Actor");

	return std::vector<std::pair<QWidget*, QString>> { properties};
}

QWidget *DfxRiptideActorNode::actorPropertiesForm() {

	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	DfxQStackControl *actorWidgetStack = new DfxQStackControl;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto generalControlPanel = new DfxControlPanel();

	DfxQControlArray *generalControlArray = new DfxQControlArray();
	DfxQControlArray *fluidControlArray = new DfxQControlArray();
	DfxQControlArray *CollisionObjectControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	auto source_path = new DfxQFluentPathControl(path + "source_path", "", false);
	auto samples     = new DfxQFluentPathControl(path + "samples", "", false);
	auto fluent_set  = new DfxQLineEditControl(path + "fluent_set", "", false);
	auto actor_type  = new DfxQComboBoxControl(path + "actor_type", "", actorTypeQControlenums, false);

	auto fluid_use_viscosity	= new DfxQCheckBoxControl(path + "fluid_use_viscosity", "", false);
	auto fluid_viscosity		= new DfxQDecimalSpinnerControl(path + "fluid_viscosity", "", false);
	auto fluid_use_vorticity	= new DfxQCheckBoxControl(path + "fluid_use_vorticity", "", false);
	auto fluid_vorticity		= new DfxQDecimalSpinnerControl(path + "fluid_vorticity", "", false);
	auto fluid_vorticity_viscosity  = new DfxQDecimalSpinnerControl(path + "fluid_vorticity_viscosity", "", false);
	auto fluid_vorticity_inertia	= new DfxQDecimalSpinnerControl(path + "fluid_vorticity_inertia", "", false);
	auto fluid_use_surface_tension = new DfxQCheckBoxControl(path + "fluid_use_surface_tension", "", false);
	auto fluid_surface_tension = new DfxQDecimalSpinnerControl(path + "fluid_surface_tension", "", false);
	auto fluid_freeze = new DfxQCheckBoxControl(path + "fluid_freeze", "", false);

	auto fluid_group_id = new DfxQIntegerSpinnerControl(path + "fluid_group_id", "", false);

	auto color = new DfxQColorControl(path + "fluid_color_r", path + "fluid_color_g", path + "fluid_color_b", "", false);

	auto initial_velx = new DfxQDecimalSpinnerControl(path + "fluid_initial_velocity_x", "", false);
	auto initial_vely = new DfxQDecimalSpinnerControl(path + "fluid_initial_velocity_y", "", false);
	auto initial_velz = new DfxQDecimalSpinnerControl(path + "fluid_initial_velocity_z", "", false);

	auto collision_kinematic = new DfxQCheckBoxControl(path + "collision_kinematic", "", false);
	auto collsiion_use_viscosity = new DfxQCheckBoxControl(path + "collsiion_use_viscosity", "", false);
	auto collision_viscosity = new DfxQDecimalSpinnerControl(path + "collision_viscosity", "", false);
	auto collsiion_use_surface_tension = new DfxQCheckBoxControl(path + "collsiion_use_surface_tension", "", false);
	auto collision_surface_tension = new DfxQDecimalSpinnerControl(path + "collision_surface_tension", "", false);

	// General
	generalControlArray->addControl("Source Path", source_path);
	generalControlArray->addSpacer(1, 2);
	generalControlArray->addControl("Samples", samples);
	generalControlArray->addSpacer(1, 2);
	generalControlArray->addControl("Actor Type", actor_type);
	generalControlArray->addSpacer(1, 2);
	generalControlArray->addControl(actorWidgetStack);

	// Fluid
	fluidControlArray->addControl("Use Viscosity", fluid_use_viscosity);
	fluidControlArray->addControl("Viscosity Strength", fluid_viscosity);
	fluidControlArray->addSpacer(1, 2);
	fluidControlArray->addControl("Use Vorticity", fluid_use_vorticity);
	fluidControlArray->addControl("Vorticity Strength", fluid_vorticity);
	fluidControlArray->addSpacer(1, 2);
	fluidControlArray->addControl("Use Surface Tension", fluid_use_surface_tension);
	fluidControlArray->addControl("Surface Tension Strength", fluid_surface_tension);
	fluidControlArray->addSpacer(1, 2);
	fluidControlArray->addControl("Color", color);
	fluidControlArray->addSpacer(1, 2);
	fluidControlArray->addControl("Group ID", fluid_group_id);
	fluidControlArray->addSpacer(1, 2);
	fluidControlArray->addControl("Freeze", fluid_freeze);
	fluidControlArray->addSpacer(1, 2);
	fluidControlArray->addControl("Initial Velocity X", initial_velx);
	fluidControlArray->addControl("Y", initial_vely);
	fluidControlArray->addControl("Z", initial_velz);

	// Collision
	CollisionObjectControlArray->addControl("Is Kinematic", collision_kinematic);
	CollisionObjectControlArray->addSpacer(1, 2);
	CollisionObjectControlArray->addControl("Use Viscosity", collsiion_use_viscosity);
	CollisionObjectControlArray->addControl("Viscosity Strength", collision_viscosity);

	actorWidgetStack->addPage(fluidControlArray);
	actorWidgetStack->addPage(CollisionObjectControlArray);

	actorWidgetStack->changeCurrentPage(actor_type->comboBox()->currentIndex());

	generalControlPanel->setTitle("Settings");
	generalControlPanel->setWidget(generalControlArray);

	main_layout->addWidget(generalControlPanel);

	widget->setLayout(main_layout);

	// Signal & Slots
	QObject::connect(actor_type->comboBox(), QOverload<int>::of(&QComboBox::activated), actorWidgetStack, &DfxQStackControl::changeCurrentPage);

	return widget;
}
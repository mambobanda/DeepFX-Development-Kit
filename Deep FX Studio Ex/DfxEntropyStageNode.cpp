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

#include <amp.h>
#include <amp_math.h>
#include "DfxEntropyStageNode.h"
#include "DfxEntropyStageHandler.h"
#include "DfxEntropyDomainPropHandler.h"
#include "DfxEntropyCachePropHandler.h"
#include "DfxEntropyActorHandler.h"
#include "DfxEntropyFlDispatcher.h"
#include "DfxGridHandler.h"

std::string DfxEntropyStageNode::_uniqueName = "UID_ENTROPY_STAGE_NODE";

DfxEntropyStageNode::DfxEntropyStageNode(): DfxSimulationStageBase(DfxStageType::ENTROPY_STAGE) {
	_entropy_ptr = nullptr;

	_flDispatcher = new DfxEntropyFlDispatcher;

	advectionQControlenums.push_back(std::pair<std::string, unsigned int>("Stam", Advection::STAM_ADVECTON));
	advectionQControlenums.push_back(std::pair<std::string, unsigned int>("MacCormack", Advection::MAC_CORMACK_ADVECTION));

	QObject::connect(DfxSimulationManager::getInstance(), &DfxSimulationManager::resetTriggered, this, &DfxEntropyStageNode::dirtySimulation);;
	QObject::connect((DfxBulletFlDispatcher*)_flDispatcher, &DfxBulletFlDispatcher::valueHasChanged, DfxSimulationManager::getInstance(), &DfxSimulationManager::resetSimulationState);

	timestep = 0.0166f;
	substeps = 1;
}

DfxEntropyStageNode::~DfxEntropyStageNode() {

}

std::string DfxEntropyStageNode::uniqueName() {
	return _uniqueName;
}

std::string DfxEntropyStageNode::getUniqueName() {
	return _uniqueName;
}

boom::Entropy *DfxEntropyStageNode::entropy() {
	return _entropy_ptr;
}

void DfxEntropyStageNode::clearEntropy() {
	if (_entropy_ptr != nullptr) {
		delete _entropy_ptr;
		_entropy_ptr = nullptr;
	}
}

void DfxEntropyStageNode::newEntropyInstance(int width, int height, int depth) {
	xamp::accelerator accelerator;
	_entropy_ptr = new boom::Entropy(accelerator, width, height, depth);

	boom::fd::fillFloat(_entropy_ptr->getU(), 0.f);
	boom::fd::fillFloat(_entropy_ptr->getV(), 0.f);
	boom::fd::fillFloat(_entropy_ptr->getW(), 0.f);
	boom::fd::fillFloat(_entropy_ptr->getDens(), 0.f);
	boom::fd::fillFloat(_entropy_ptr->getTemperature(), 0.f);
	boom::fd::fillInt(_entropy_ptr->getCellType(), boom::constants::FLUID);
	boom::fd::fillInt(_entropy_ptr->getFixedEmitters(), -1);

	_entropy_ptr->initBoundary();
}

void DfxEntropyStageNode::initDispatcher() {

}

size_t DfxEntropyStageNode::addActor(fl::Any *any) {

	if (any->type() == DfxFluentTypes::SG_NODE) {

		auto snode = any->rawData<deepsg::SgNode>();

		if (snode->getType() == deepsg::SgNodeType::GRID) {

			DfxEntropyActorNode *actor_node = nullptr;
			DfxEntropyActorHandler handler;
			handler.create();

			auto dao = handler.dataObject()->rawData<fl::DataObject>();

			actor_node = handler.createActorNode();
			actor_node->setStage(this);
			actor_node->setSourcePath(snode->referencePath());

			//_flDispatcher->attachBranch(snode->xFormProperties());
			//_flDispatcher->attachBranch(handler.dataObject());

			DfxQSceneManager::getInstance()->moveNode(actor_node, actorCollectionNode());

		}

	}

	return 0;
}

void DfxEntropyStageNode::removeActor(size_t uid) {

}

void DfxEntropyStageNode::restoreActor(deepsg::SgStageActorNode *) {

}

size_t DfxEntropyStageNode::addProp(fl::Any *any) {

	if (any->type() == DfxFluentTypes::SG_STAGE_PROP_NODE) {

		auto prop_node = any->rawData<deepsg::SgStagePropNode>();

		if (prop_node->propType() == entropy::type::SIM_DOMAIN) {
			bool hasDomain = false;

			for (auto prop : props()) {
				if (prop->propType() == entropy::type::SIM_DOMAIN) {
					hasDomain = true;
					break;
				}
			}

			if (!hasDomain) {
				auto domainProp = (DfxEntropyDomainPropNode*)prop_node;
				domainProp->setStage(this);
				DfxQSceneManager::getInstance()->moveNode(prop_node, propCollectionNode());
			}
			else {
				DfxGlobal::info::messageBox("Failed to create domain: Only one domain is allowed per stage.", "", 500, "");
			}
		}
		else if (prop_node->propType() == entropy::type::CACHE) {
			auto cacheProp = (DfxEntropyCachePropNode*)prop_node;
			cacheProp->setStage(this);
			DfxQSceneManager::getInstance()->moveNode(prop_node, propCollectionNode());
		}
		else {
			DfxGlobal::console::println("Entropy doesn't support prop.");
		}
		
	}
	else {
		DfxGlobal::console::println("Entropy doesn't support prop.");
	}

	return 0;
}

void DfxEntropyStageNode::removeProp(size_t uid) {

}

void DfxEntropyStageNode::restoreProp(deepsg::SgStagePropNode *) {

}

void DfxEntropyStageNode::restoreAll() {

}

void DfxEntropyStageNode::reset() {
	clearEntropy();

	for (auto prop : props()) {
		if (prop->propType() == entropy::type::SIM_DOMAIN) {
			auto entropyDomain = (DfxEntropyDomainPropNode*)prop;

			entropyDomain->reset();
			entropyDomain->setPropState(DfxEntropyDomainPropNode::PropState::SPAWN);
			
		}

		if (prop->propType() == entropy::type::CACHE) {
			auto entropyCache = (DfxEntropyCachePropNode*)prop;
			entropyCache->clear();
		}
	}

	for (auto xactor : actors()) {
		auto actor = (DfxEntropyActorNode*)xactor;
		actor->setActorState(DfxEntropyActorNode::ActorState::SPAWN);
	}

	timestep = 0.0166f;
	substeps = 1;
	_simIsDirty = false;
}

bool DfxEntropyStageNode::needsReset() {
	return _simIsDirty;
}

bool DfxEntropyStageNode::isSimulationEnabled() {
	return true;
}

bool DfxEntropyStageNode::doStep(DfxStageEvaluationInfo info) {
	return true;
}

void DfxEntropyStageNode::updateEntropySettings() {
	DfxEntropyStageHandler handler;

	if (handler.fetch(referencePath())) {

		auto dao = handler.dataObject()->rawData<fl::DataObject>();

		auto timestep = dao->property("timestep")->data<float>();
		substeps = dao->property("substeps")->data<int>();
		auto diffusion_iterations = dao->property("diffusion_iterations")->data<int>();
		auto projection_iterations = dao->property("projection_iterations")->data<int>();

		auto advection_type = dao->property("advection_type")->data<int>();
		auto use_vorticity = dao->property("use_vorticity")->data<bool>();

		auto viscosity = dao->property("viscosity")->data<float>();
		auto vorticity = dao->property("vorticity")->data<float>();
		auto density_dissipation = dao->property("density_dissipation")->data<float>();
		auto buoyancy_rise = dao->property("buoyancy_rise")->data<float>();
		auto buoyancy_fall = dao->property("buoyancy_fall")->data<float>();
		auto ambient_temperature = dao->property("ambient_temperature")->data<float>();

		auto gravity_x = dao->property("gravity_x")->data<float>();
		auto gravity_y = dao->property("gravity_y")->data<float>();
		auto gravity_z = dao->property("gravity_z")->data<float>();

		_entropy_ptr->setTimeStep(timestep);
		_entropy_ptr->setDiffuseIterations(diffusion_iterations);
		_entropy_ptr->setProjectionIterations(projection_iterations);

		if (advection_type == 0) _entropy_ptr->setAdvectionType(boom::constants::STAM_ADVECTON);
		else if (advection_type == 1) _entropy_ptr->setAdvectionType(boom::constants::MAC_CORMACK_ADVECTION);

		if (use_vorticity) _entropy_ptr->setVorticityConfinementFlag(boom::constants::WITH_VORTICITY_CONFINEMENT);
		else _entropy_ptr->setVorticityConfinementFlag(boom::constants::NO_VORTICITY_CONFINEMENT);

		_entropy_ptr->setViscosity(viscosity);
		_entropy_ptr->setVorticity(vorticity);
		_entropy_ptr->setDensityDissipation(density_dissipation);
		_entropy_ptr->setBuoyancyRise(buoyancy_rise);
		_entropy_ptr->setBuoyancyFall(buoyancy_fall);
		_entropy_ptr->setAmbiantTemperature(ambient_temperature);

		_entropy_ptr->setGravity(gravity_x, gravity_y, gravity_z);
	
	}
	
}

void DfxEntropyStageNode::beginStep(DfxStageEvaluationInfo info) {

	for (auto prop : props()) {
		if (prop->propType() == entropy::type::SIM_DOMAIN) {
			auto entropyDomain = (DfxEntropyDomainPropNode*)prop;

			if (entropyDomain->propState() == DfxEntropyDomainPropNode::PropState::SPAWN) {

				auto voxel_size = entropyDomain->voxelSize();
				auto dims = entropyDomain->dimensions();

				if (voxel_size >= 0.001f && dims.x >= 10 && dims.y >= 10 && dims.z >= 10) {
					newEntropyInstance(dims.x, dims.y, dims.z);
				}

				entropyDomain->spawn();
				entropyDomain->setPropState(DfxEntropyDomainPropNode::PropState::NONE);
			}
		}
	}

	if (_entropy_ptr != nullptr) {
		updateEntropySettings();
	}

	if (_entropy_ptr != nullptr) {

		for (auto prop : props()) {
			if (prop->propType() == entropy::type::SIM_DOMAIN) {
				auto entropyDomain = (DfxEntropyDomainPropNode*)prop;

				if (entropyDomain->propState() == DfxEntropyDomainPropNode::PropState::NONE) {

					for (auto xactor : actors()) {
						auto actor = (DfxEntropyActorNode*)xactor;

						if (actor->actorState() == DfxEntropyActorNode::ActorState::SPAWN) {

							auto any = DfxSpace::getInstance()->findAbsolute(actor->referencePath());
							auto sourceAny = DfxSpace::getInstance()->findAbsolute(actor->sourcePath());

							if (any != nullptr && sourceAny != nullptr) {
								auto dao = any->rawData<fl::DataObject>();
								auto sourceDao = sourceAny->rawData<fl::DataObject>();

								if (sourceDao->hasProperty("sg_grid_node")) {
									DfxGridHandler ghandler;

									if (ghandler.fetch(actor->sourcePath())) {
										auto vadapter = ((DfxGridNode*)ghandler.gridNode())->getGridAdapter();

										auto actortype = dao->property("actor_type")->data<int>();

										if (actortype == 0) {

											DfxEntropyDomainPropNode::SourceInfo sinfo;

											auto constant_inflow = dao->property("constant_inflow")->data<bool>();
											auto constant_temperature = dao->property("constant_temperature")->data<bool>();
											auto constant_velocity = dao->property("constant_velocity")->data<bool>();

											auto inflow_rate_type = dao->property("inflow_rate_type")->data<int>();
											auto inflow_rate = dao->property("inflow_rate")->data<float>();

											auto inflow_density = dao->property("inflow_density")->data<float>();
											auto inflow_temperature = dao->property("inflow_temperature")->data<float>();

											auto inflow_velocity_x = dao->property("inflow_velocity_x")->data<float>();
											auto inflow_velocity_y = dao->property("inflow_velocity_y")->data<float>();
											auto inflow_velocity_z = dao->property("inflow_velocity_z")->data<float>();

											sinfo.constant_inflow = constant_inflow;
											sinfo.constant_temperature = constant_temperature;
											sinfo.constant_velocity = constant_velocity;

											sinfo.inflow_rate_type = inflow_rate_type;
											sinfo.inflow_rate = inflow_rate;

											sinfo.inflow_density = inflow_density;
											sinfo.inflow_temperature = inflow_temperature;

											sinfo.inflow_velocity = glm::vec3(inflow_velocity_x, inflow_velocity_y, inflow_velocity_z);

											entropyDomain->addSource(sinfo, vadapter, _entropy_ptr, ghandler.gridNode()->transform4x4());
										}
									}
								}
							}

							actor->setActorState(DfxEntropyActorNode::ActorState::NONE);
						}
					}
				}

				break;
			}
		}

		_entropy_ptr->initBoundary();
	}
}

void DfxEntropyStageNode::step(DfxStageEvaluationInfo info) {
	if (_entropy_ptr != nullptr) {	
		for (int i = 0; i < substeps; i++) {
			_entropy_ptr->step();
		}
	}
}

void DfxEntropyStageNode::endStep(DfxStageEvaluationInfo info) {
	
	if (_entropy_ptr != nullptr) {

		for (auto prop : props()) {

			if (prop->propType() == entropy::type::SIM_DOMAIN) {

				for (auto xprop : props()) {

					auto entropyDomain = (DfxEntropyDomainPropNode*)prop;

					auto voxel_size = entropyDomain->voxelSize();
					auto dims = entropyDomain->dimensions();

					if (xprop->propType() == entropy::type::CACHE) {

						auto cache = (DfxEntropyCachePropNode*)xprop;

						cache->cache(_entropy_ptr, voxel_size, dims);

					}
				}

				break;
			}
		}

	}
}

bool DfxEntropyStageNode::hasFluentSimplexObject(size_t uid) {
	return false;
}

bool DfxEntropyStageNode::hasFluentStreamObject(size_t uid) {
	return false;
}

void DfxEntropyStageNode::clearFluent() {

}

std::vector<DfxTime> DfxEntropyStageNode::subFrames() {
	return std::vector<DfxTime>();
}

fl::Dispatcher *DfxEntropyStageNode::fluentDispacther() {
	return nullptr;
}

void DfxEntropyStageNode::dirtySimulation() {
	_simIsDirty = true;
}

std::vector<std::pair<QWidget*, QString>> DfxEntropyStageNode::propertyForms() {
	std::pair<QWidget*, QString> properties(propertiesForm(), "Stage Properties");

	return std::vector<std::pair<QWidget*, QString>> { properties };
}

QWidget *DfxEntropyStageNode::propertiesForm() {
	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto propertiesControlPanel = new DfxControlPanel();

	DfxQControlArray *propertiesControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	auto timestep = new DfxQDecimalSpinnerControl(path + "timestep", "", false);
	auto substeps = new DfxQIntegerSpinnerControl(path + "substeps", "", false);

	auto diffusion_iterations  = new DfxQIntegerSpinnerControl(path + "diffusion_iterations", "", false);
	auto projection_iterations = new DfxQIntegerSpinnerControl(path + "projection_iterations", "", false);

	auto advection_type = new DfxQComboBoxControl(path + "advection_type", "", advectionQControlenums, false);

	auto use_vorticity = new DfxQCheckBoxControl(path + "use_vorticity", "", false);
	auto viscosity = new DfxQDecimalSpinnerControl(path + "viscosity", "", false);
	auto vorticity = new DfxQDecimalSpinnerControl(path + "vorticity", "", false);
	auto density_dissipation = new DfxQDecimalSpinnerControl(path + "density_dissipation", "", false);
	auto buoyancy_rise = new DfxQDecimalSpinnerControl(path + "buoyancy_rise", "", false);
	auto buoyancy_fall = new DfxQDecimalSpinnerControl(path + "buoyancy_fall", "", false);
	auto ambient_temperature = new DfxQDecimalSpinnerControl(path + "ambient_temperature", "", false);

	auto gx = new DfxQDecimalSpinnerControl(path + "gravity_x", "", false);
	auto gy = new DfxQDecimalSpinnerControl(path + "gravity_y", "", false);
	auto gz = new DfxQDecimalSpinnerControl(path + "gravity_z", "", false);

	// Source
	propertiesControlArray->addControl("Timestep", timestep);
	propertiesControlArray->addSpacer(0, 2);
	propertiesControlArray->addControl("Substeps", substeps);
	propertiesControlArray->addSpacer(0, 6);
	propertiesControlArray->addControl("Diffusion Iterations", diffusion_iterations);
	propertiesControlArray->addSpacer(0, 2);
	propertiesControlArray->addControl("Projection Iterations", projection_iterations);
	propertiesControlArray->addSpacer(0, 6);
	propertiesControlArray->addControl("Advection Type", advection_type);
	propertiesControlArray->addSpacer(0, 6);
	propertiesControlArray->addControl("Use Vorticity", use_vorticity);
	propertiesControlArray->addControl("Vorticity", vorticity);
	propertiesControlArray->addSpacer(0, 2);
	propertiesControlArray->addControl("Viscosity", viscosity);
	propertiesControlArray->addSpacer(0, 2);
	propertiesControlArray->addControl("Density Dissipation", density_dissipation);
	propertiesControlArray->addSpacer(0, 2);
	propertiesControlArray->addControl("Buoyancy Rise", buoyancy_rise);
	propertiesControlArray->addSpacer(0, 2);
	propertiesControlArray->addControl("Buoyancy Fall", buoyancy_fall);
	propertiesControlArray->addSpacer(0, 2);
	propertiesControlArray->addControl("Ambient Temperature", ambient_temperature);
	propertiesControlArray->addSpacer(0, 6);
	propertiesControlArray->addControl("Gravity X", gx);
	propertiesControlArray->addControl("Y", gy);
	propertiesControlArray->addControl("Z", gz);

	propertiesControlPanel->setTitle("Global Settings");
	propertiesControlPanel->setWidget(propertiesControlArray);

	main_layout->addWidget(propertiesControlPanel);

	widget->setLayout(main_layout);

	return widget;
}
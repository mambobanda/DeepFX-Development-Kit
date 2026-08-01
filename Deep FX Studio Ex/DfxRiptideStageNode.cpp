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

#include <glm/geometric.hpp>
#include "DfxRiptideStageNode.h"
#include "DfxRiptideActorHandler.h"
#include "DfxRiptideActorNode.h"
#include "DfxRiptideCachePropNode.h"
#include "DfxRiptideCachePropHandler.h"
#include "DfxRiptideEmitterPropNode.h"
#include "DfxRiptideEmitterPropHandler.h"
#include "DfxRiptideHeaterPropHandler.h"
#include "DfxRiptideHeaterPropNode.h"
#include "DfxRiptideFlDispatcher.h"
#include "DfxPointSampling.h"
#include "RtxSimulator.h"
#include "RtxSPHSystem.h"

std::string DfxRiptideStageNode::_uniqueName = "UID_RIPTIDE_SHAPE_NODE";

DfxRiptideStageNode::DfxRiptideStageNode(): DfxSimulationStageBase(DfxStageType::RIPTIDE_STAGE) {
	DfxSimulationManager::getInstance()->forceReset(true);
	_flDispatcher = new DfxRiptideFlDispatcher;

	_sphSystem = new rtx::SPHSystem;

	_simIsDirty = false;

	devicesQControlenums.push_back(std::pair<std::string, unsigned int>("CPU", Devices::CPU));

	timingQControlenums.push_back(std::pair<std::string, unsigned int>("Adaptive", Timing::ADAPTIVE));
	timingQControlenums.push_back(std::pair<std::string, unsigned int>("FPS", Timing::FPS));
	timingQControlenums.push_back(std::pair<std::string, unsigned int>("Timestep", Timing::TIMESTEP));

	searchQControlenums.push_back(std::pair<std::string, unsigned int>("Compact", Search::COMPACT));
	searchQControlenums.push_back(std::pair<std::string, unsigned int>("Compact+", Search::COMPACT_PLUS));

	QObject::connect(DfxSimulationManager::getInstance(), &DfxSimulationManager::resetTriggered, this, &DfxRiptideStageNode::dirtySimulation);;
	QObject::connect((DfxRiptideFlDispatcher*)_flDispatcher, &DfxRiptideFlDispatcher::valueHasChanged, DfxSimulationManager::getInstance(), &DfxSimulationManager::resetSimulationState);
}

DfxRiptideStageNode::~DfxRiptideStageNode() {

}

std::string DfxRiptideStageNode::uniqueName() {
	return _uniqueName;
}

std::string DfxRiptideStageNode::getUniqueName() {
	return _uniqueName;
}

size_t DfxRiptideStageNode::addActor(fl::Any *any) {
	if (any->type() == DfxFluentTypes::SG_NODE) {

		auto snode = any->rawData<deepsg::SgNode>();

		if (snode->getType() == deepsg::SgNodeType::GEOMETRY) {
		
			DfxRiptideActorNode *actor_node = nullptr;
			DfxRiptideActorHandler handler;
			handler.create();

			auto dao = handler.dataObject()->rawData<fl::DataObject>();

			actor_node = handler.createActorNode();
			actor_node->setStage(this);
			actor_node->setSource(DfxRiptideActorNode::Source::GEOMETRY);
			actor_node->setSourcePath(snode->referencePath());
			actor_node->setActorState(DfxRiptideActorNode::ActorState::SPAWN);

			_flDispatcher->attachBranch(snode->xFormProperties());
			_flDispatcher->attachBranch(handler.dataObject());

			DfxQSceneManager::getInstance()->moveNode(actor_node, actorCollectionNode());
		
		}

	}

	return 0;
}

rtx::SPHSystem *DfxRiptideStageNode::sphSystem() {
	return _sphSystem;
}

void DfxRiptideStageNode::applySphSystemSettings() {
	
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {

		auto dao = any->rawData<fl::DataObject>();

		auto resolution  = dao->property("resolution")->data<float>();
		auto timing_type = dao->property("timing_type")->data<unsigned int>();
		auto search = dao->property("search")->data<unsigned int>();
		auto fixed_timstep = dao->property("fixed_timstep")->data<float>();
		auto fixed_substeps = dao->property("fixed_substeps")->data<int>();
		auto fps_timestep = dao->property("fps_timestep")->data<float>();
		auto fps_fps = dao->property("fps_fps")->data<unsigned int>();
		auto adaptive_min_timestep = dao->property("adaptive_min_timestep")->data<float>();
		auto adaptive_max_timestep = dao->property("adaptive_max_timestep")->data<float>();
		auto adaptive_fps = dao->property("adaptive_fps")->data<unsigned int>();

		auto divergence_free = dao->property("divergence_free")->data<bool>();
		auto divergence_error = dao->property("divergence_error")->data<float>();
		auto density_error = dao->property("density_error")->data<float>();
		auto pressure_relaxation = dao->property("pressure_relaxation")->data<float>();

		auto coupling_stiffness = dao->property("coupling_stiffness")->data<float>();
		auto velocity_dampening = dao->property("velocity_dampening")->data<float>();
		auto gravity_x = dao->property("gravity_x")->data<float>();
		auto gravity_y = dao->property("gravity_y")->data<float>();
		auto gravity_z = dao->property("gravity_z")->data<float>();

		auto particleSize = DfxPointSampling::particleRadiusFromResolution(resolution);

		sphSystem()->setRestDensity(1000.f);
		sphSystem()->setParticleRadius(particleSize);

		rtx::SimulationConfig config;
		config.boundaryMethod = rtx::AKINCI2012;

		if (timing_type == DfxRiptideStageNode::Timing::ADAPTIVE) {
			config.timestepMethod = rtx::TimestepMethod::ADAPTIVE;
			config.min_timestep = adaptive_min_timestep;
			config.max_timestep = adaptive_max_timestep;
			config.fps = adaptive_fps;
		}
		else if (timing_type == DfxRiptideStageNode::Timing::FPS) {
			config.timestepMethod = rtx::TimestepMethod::FPS;
			config.timestep = fps_timestep;
			config.fps = fps_fps;
		}
		else {
			config.timestepMethod = rtx::TimestepMethod::FIXED;
			config.timestep = fixed_timstep;
			config.substeps = fixed_substeps;
		}

		config.searchMethod = search;
		config.density_error = density_error;
		config.coherence_error = divergence_error;
		config.pressure_stiffness = pressure_relaxation;
		config.gravity = glm::vec3(gravity_x, gravity_y, gravity_z);

		_simulationConfig = config;
	}

}

void DfxRiptideStageNode::removeActor(size_t uid) {
	auto node = actorCollectionNode()->findChild(uid);

	if (node != nullptr) {
		auto act = (DfxRiptideActorNode*)node;

		DfxQSceneManager::getInstance()->removeNode(node);

		dirtySimulation();
		DfxSimulationManager::getInstance()->resetSimulationState();
	}
}

void DfxRiptideStageNode::restoreActor(deepsg::SgStageActorNode *node) {

	if (node->uniqueName() == DfxRiptideActorNode::getUniqueName()) {
		DfxRiptideActorHandler handler;

		if (handler.fetch(node->referencePath())) {
			auto actor_node = handler.actorNode();

			actor_node->setStage(this);
			actor_node->setActorState(DfxRiptideActorNode::ActorState::SPAWN);

			_flDispatcher->attachBranch(handler.dataObject());

			auto any = DfxSpace::getInstance()->findAbsolute(actor_node->sourcePath());

			if (any != nullptr) {

				if (any->type() == fl::type::DATA_OBJECT) {
					auto dao = any->rawData<fl::DataObject>();

					if (dao->hasProperty("sg_geo_node")) {
						actor_node->setSource(DfxRiptideActorNode::Source::GEOMETRY);
						auto geoNode = dao->property("sg_geo_node")->rawData<deepsg::SgGeoNode>();
						_flDispatcher->attachBranch(geoNode->xFormProperties());
					}
				}
			}
		}
	}

}

size_t DfxRiptideStageNode::addProp(fl::Any *any) {

	if (any->type() == DfxFluentTypes::SG_STAGE_PROP_NODE) {

		auto prop_node = any->rawData<deepsg::SgStagePropNode>();

		if (prop_node->propType() == riptide::type::CACHE) {
			auto cacheProp = (DfxRiptideCachePropNode*)prop_node;
			cacheProp->setStage(this);
			cacheProp->setPropState(DfxRiptideCachePropNode::PropState::SPAWN);
			DfxQSceneManager::getInstance()->moveNode(prop_node, propCollectionNode());
		}
		else if (prop_node->propType() == riptide::type::EMITTER) {
			auto emitterProp = (DfxRiptideEmitterPropNode*)prop_node;

			DfxRiptideEmitterPropHandler handler;
			handler.fetch(emitterProp->referencePath());

			emitterProp->setStage(this);
			emitterProp->setPropState(DfxRiptideEmitterPropNode::PropState::SPAWN);
			DfxQSceneManager::getInstance()->moveNode(prop_node, propCollectionNode());

			_flDispatcher->attachBranch(prop_node->xFormProperties());
			_flDispatcher->attachBranch(handler.dataObject());
		}
		else if (prop_node->propType() == riptide::type::HEATER) {
			auto heaterProp = (DfxRiptideHeaterPropNode*)prop_node;
			heaterProp->setStage(this);
			heaterProp->setPropState(DfxRiptideCachePropNode::PropState::SPAWN);
			DfxQSceneManager::getInstance()->moveNode(prop_node, propCollectionNode());
		}
	}
	else {
		DfxGlobal::console::println("Riptide doesn't support prop.");
	}

	return 0;
}

void DfxRiptideStageNode::removeProp(size_t uid) {
	auto node = propCollectionNode()->findChild(uid);

	if (node != nullptr) {
		if (node->uniqueName() == DfxRiptideEmitterPropNode::getUniqueName()) {
			auto prop = (DfxRiptideEmitterPropNode*)node;

			DfxQSceneManager::getInstance()->removeNode(node);

			dirtySimulation();
			DfxSimulationManager::getInstance()->resetSimulationState();
		}
		else  if (node->uniqueName() == DfxRiptideCachePropNode::getUniqueName()) {
			auto prop = (DfxRiptideCachePropNode*)node;
			prop->clearCache();

			DfxQSceneManager::getInstance()->removeNode(node);

			dirtySimulation();
			DfxSimulationManager::getInstance()->resetSimulationState();
		}
		else if (node->uniqueName() == DfxRiptideHeaterPropNode::getUniqueName()) {
			auto prop = (DfxRiptideHeaterPropNode*)node;

			DfxQSceneManager::getInstance()->removeNode(node);

			dirtySimulation();
			DfxSimulationManager::getInstance()->resetSimulationState();
		}
	}
}

void DfxRiptideStageNode::restoreProp(deepsg::SgStagePropNode *node) {
	if (node->uniqueName() == DfxRiptideCachePropNode::getUniqueName()) {
		DfxRiptideCachePropHandler handler;

		if (handler.fetch(node->referencePath())) {
			auto prop_node = handler.cacheNode();

			prop_node->setStage(this);
			prop_node->setPropState(DfxRiptideCachePropNode::PropState::SPAWN);

			_flDispatcher->attachBranch(handler.dataObject());

		}
	}
	else if (node->uniqueName() == DfxRiptideEmitterPropNode::getUniqueName()) {
		DfxRiptideEmitterPropHandler handler;

		if (handler.fetch(node->referencePath())) {
			auto prop_node = handler.propNode();

			prop_node->setStage(this);
			prop_node->setPropState(DfxRiptideEmitterPropNode::PropState::SPAWN);

			_flDispatcher->attachBranch(handler.dataObject());

		}
	}
	else if (node->uniqueName() == DfxRiptideHeaterPropNode::getUniqueName()) {
		DfxRiptideHeaterPropHandler handler;

		if (handler.fetch(node->referencePath())) {
			auto prop_node = handler.propNode();

			prop_node->setStage(this);
			prop_node->setPropState(DfxRiptideCachePropNode::PropState::SPAWN);

			_flDispatcher->attachBranch(handler.dataObject());

		}
	}
}

void DfxRiptideStageNode::queueSimplexAgent(DfxRiptideAgent *agent) {
	_fluentSimplexAgentQueue.push_back(agent);
}

void DfxRiptideStageNode::queueStreamAgent(DfxRiptideAgent *agent) {
	_fluentStreamAgentQueue.push_back(agent);
}

void DfxRiptideStageNode::restoreAll() {
	for (auto actor : actorCollectionNode()->children()) {
		if (actor->getType() == deepsg::SgNodeType::STAGE_ACTOR) {
			restoreActor((deepsg::SgStageActorNode*)actor);
		}
	}

	for (auto prop : propCollectionNode()->children()) {
		if (prop->getType() == deepsg::SgNodeType::STAGE_PROP) {
			restoreProp((deepsg::SgStagePropNode*)prop);
		}
	}
}

void DfxRiptideStageNode::reset() {
	sphSystem()->clear();
	sphSystem()->clearOther();

	for (auto agent : _fluentSimplexAgents) {
		delete agent;
	}

	for (auto agent : _fluentStreamAgents) {
		delete agent;
	}

	for (auto prop : props()) {
		if (prop->propType() == riptide::type::CACHE) {
			auto cacheProp = (DfxRiptideCachePropNode*)prop;
			cacheProp->clearCache();
		}
		else if (prop->propType() == riptide::type::EMITTER) {
			auto riptideEmitter = (DfxRiptideEmitterPropNode*)prop;
			riptideEmitter->setPropState(DfxRiptideHeaterPropNode::PropState::SPAWN);
		}
		else if (prop->propType() == riptide::type::HEATER) {
			auto riptideHeater = (DfxRiptideHeaterPropNode*)prop;
			riptideHeater->setPropState(DfxRiptideHeaterPropNode::PropState::SPAWN);
		}
	}

	for (auto actor : actors()) {
		if (actor->actorType() == riptide::type::ACTOR) {
			auto riptideActor = (DfxRiptideActorNode*)actor;
			riptideActor->setActorState(DfxRiptideActorNode::ActorState::SPAWN);	
		}
	}


	_fluentSimplexAgents.clear();
	_fluentStreamAgents.clear();

	_simIsDirty = false;
}

bool DfxRiptideStageNode::needsReset() {
	return _simIsDirty;
}

void DfxRiptideStageNode::dirtySimulation() {
	_simIsDirty = true;
}

bool DfxRiptideStageNode::isSimulationEnabled() {
	return true;
}

bool DfxRiptideStageNode::doStep(DfxStageEvaluationInfo info) {
	return true;
}

DfxPointFrame *DfxRiptideStageNode::getSimulationFrame() {
	DfxPointFrame *frame = nullptr;

	auto d_attributes = fetchDefaultStreamAttributes();

	if (d_attributes.success) {

		auto positionStream = DfxFluentStreamCollection::getInstance()->atrributeMap().at<glm::vec3>("@position");

		std::vector<glm::vec3> pos;
		std::vector<glm::vec3> vel;
		std::vector<float> speed;
		std::vector<int> group;
		std::vector<glm::vec3> color;

		for (auto agent : _fluentSimplexAgents) {
			for (size_t inx = 0; inx < agent->size; inx++) {
				if (d_attributes.sph_class[agent->offset + inx] == DfxRiptideActorNode::ActorType::FLUID &&
					d_attributes.alive[agent->offset + inx]) {

					auto velocity = d_attributes.velocity[agent->offset + inx];
					pos.push_back(positionStream[agent->offset + inx]);
					vel.push_back(velocity);
					speed.push_back(glm::length(velocity));
					group.push_back(agent->group);
					color.push_back(agent->color);
				}
			}
		}

		for (auto agent : _fluentStreamAgents) {
			for (size_t inx = 0; inx < agent->size; inx++) {
				if (d_attributes.sph_class[agent->offset + inx] == DfxRiptideActorNode::ActorType::FLUID &&
					d_attributes.alive[agent->offset + inx]) {

					auto velocity = d_attributes.velocity[agent->offset + inx];
					pos.push_back(positionStream[agent->offset + inx]);
					vel.push_back(velocity);
					speed.push_back(glm::length(velocity));
					group.push_back(agent->group);
					color.push_back(agent->color);
				}
			}
		}

		if (!pos.empty()) {
			frame = new DfxPointFrame();
			frame->initFrame(pos.size());

			for (size_t inx = 0; inx < pos.size(); inx++) {

				frame->getPositions()[inx] = pos[inx];
				frame->getVelocities()[inx] = vel[inx];
				frame->getSpeeds()[inx] = speed[inx];
				frame->getGroup()[inx] = group[inx];
				frame->getColors()[inx] = color[inx];
			}
		}

	}

	return frame;
}

float DfxRiptideStageNode::particleRadius() {

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {

		auto dao = any->rawData<fl::DataObject>();

		auto resolution = dao->property("resolution")->data<float>();

		return DfxPointSampling::particleRadiusFromResolution(resolution);
	}

	return 0.02f;
}

void DfxRiptideStageNode::updateStream() {
	if (DfxFluentStreamCollection::getInstance()->atrributeMap().size() > 0) {
		auto fluentPositions = DfxFluentStreamCollection::getInstance()->atrributeMap().at<glm::vec3>("@position");
		auto d_attributes = fetchDefaultStreamAttributes();

		for (long long int inx = 0; inx < sphSystem()->nbParticles(); inx++) {
			if (sphSystem()->type()[inx] == rtx::FLUID_PARTICLE) {
				fluentPositions[sphSystem()->stream_index()[inx]] = sphSystem()->positions()[inx];

				if (d_attributes.success) {
					d_attributes.velocity[sphSystem()->stream_index()[inx]] = sphSystem()->velocity()[inx];
				}
			}
		}
	}
}

DfxRiptideDefaultAttributes DfxRiptideStageNode::fetchDefaultStreamAttributes(bool create) {

	DfxRiptideDefaultAttributes output;
	output.success = true;
	output.alive = nullptr;
	output.sph_class = nullptr;
	output.velocity = nullptr;
	output.update = nullptr;
	output.use_viscosity = nullptr;
	output.viscosity = nullptr;
	output.use_vorticity = nullptr;
	output.vorticity= nullptr;
	output.use_tension = nullptr;
	output.tension = nullptr;
	output.frozen = nullptr;

	auto ref00 = compute::getAttribute<int>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), "@sph_class", DfxFluenType::INT, create);
	auto ref01 = compute::getAttribute<int>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), "@alive", DfxFluenType::INT, create);
	auto ref02 = compute::getAttribute<glm::vec3>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), "@velocity", DfxFluenType::VEC3, create);
	auto ref03 = compute::getAttribute<int>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), "@update", DfxFluenType::INT, create);
	auto ref04 = compute::getAttribute<int>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), "@use_viscosity", DfxFluenType::INT, create);
	auto ref05 = compute::getAttribute<float>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), "@viscosity", DfxFluenType::FLOAT, create);
	auto ref06 = compute::getAttribute<int>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), "@use_vorticity", DfxFluenType::INT, create);
	auto ref07 = compute::getAttribute<float>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), "@vorticity", DfxFluenType::FLOAT, create);
	auto ref08 = compute::getAttribute<float>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), "@vorticity_viscosity", DfxFluenType::FLOAT, create);
	auto ref09 = compute::getAttribute<float>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), "@vorticity_inertia", DfxFluenType::FLOAT, create);
	auto ref10 = compute::getAttribute<int>(&DfxFluentStreamCollection::getInstance()->atrributeMap(),   "@use_surface_tension", DfxFluenType::INT, create);
	auto ref11 = compute::getAttribute<float>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), "@surface_tension", DfxFluenType::FLOAT, create);
	auto ref12 = compute::getAttribute<int>(&DfxFluentStreamCollection::getInstance()->atrributeMap(), "@frozen", DfxFluenType::INT, create);

	// sph_class
	if (ref00.sameAsRequestedType && ref00.exists && ref00._data != nullptr) {
		output.sph_class = compute::extractAttrRef<int>(ref00);
	}
	else {
		DfxGlobal::console::println("Riptide: Attribute '@sph_class' wrong type, expected int.");
		output.success = false;
	}

	// alive
	if (ref01.sameAsRequestedType && ref01.exists && ref01._data != nullptr) {
		output.alive = compute::extractAttrRef<int>(ref01);
	}
	else {
		DfxGlobal::console::println("Riptide: Attribute '@alive' wrong type, expected bool.");
		output.success = false;
	}

	// velocity
	if (ref02.sameAsRequestedType && ref02.exists && ref02._data != nullptr) {
		output.velocity = compute::extractAttrRef<glm::vec3>(ref02);
	}
	else {
		DfxGlobal::console::println("Riptide: Attribute '@velocity' wrong type, expected bool.");
		output.success = false;
	}

	// update
	if (ref03.sameAsRequestedType && ref03.exists && ref03._data != nullptr) {
		output.update = compute::extractAttrRef<int>(ref03);
	}
	else {
		DfxGlobal::console::println("Riptide: Attribute '@update' wrong type, expected bool.");
		output.success = false;
	}

	// use_viscosity
	if (ref04.sameAsRequestedType && ref04.exists && ref04._data != nullptr) {
		output.use_viscosity = compute::extractAttrRef<int>(ref04);
	}
	else {
		DfxGlobal::console::println("Riptide: Attribute '@use_viscosity' wrong type, expected bool.");
		output.success = false;
	}

	// viscosity
	if (ref05.sameAsRequestedType && ref05.exists && ref05._data != nullptr) {
		output.viscosity = compute::extractAttrRef<float>(ref05);
	}
	else {
		DfxGlobal::console::println("Riptide: Attribute '@viscosity' wrong type, expected bool.");
		output.success = false;
	}

	// use_vorticity
	if (ref06.sameAsRequestedType && ref06.exists && ref06._data != nullptr) {
		output.use_vorticity = compute::extractAttrRef<int>(ref06);
	}
	else {
		DfxGlobal::console::println("Riptide: Attribute '@use_vorticity' wrong type, expected bool.");
		output.success = false;
	}

	// vorticity
	if (ref07.sameAsRequestedType && ref07.exists && ref07._data != nullptr) {
		output.vorticity = compute::extractAttrRef<float>(ref07);
	}
	else {
		DfxGlobal::console::println("Riptide: Attribute '@vorticity' wrong type, expected bool.");
		output.success = false;
	}

	// use_surface_tension
	if (ref10.sameAsRequestedType && ref10.exists && ref10._data != nullptr) {
		output.use_tension = compute::extractAttrRef<int>(ref10);
	}
	else {
		DfxGlobal::console::println("Riptide: Attribute '@use_surface_tension' wrong type, expected bool.");
		output.success = false;
	}

	// surface_tension
	if (ref11.sameAsRequestedType && ref11.exists && ref11._data != nullptr) {
		output.tension = compute::extractAttrRef<float>(ref11);
	}
	else {
		DfxGlobal::console::println("Riptide: Attribute '@surface_tension' wrong type, expected float.");
		output.success = false;
	}

	// frozen
	if (ref12.sameAsRequestedType && ref12.exists && ref12._data != nullptr) {
		output.frozen = compute::extractAttrRef<int>(ref12);
	}
	else {
		DfxGlobal::console::println("Riptide: Attribute '@frozen' wrong type, expected bool.");
		output.success = false;
	}

	return output;
}

void DfxRiptideStageNode::processAgentQueue(std::vector<DfxRiptideAgent*> &queue, size_t type) {
	if (!queue.empty()) {
		auto d_attributes = fetchDefaultStreamAttributes();

		if (d_attributes.success) {
			size_t totalSize = 0;

			for (auto agent : queue) {
				totalSize += agent->size;
			}

			if (totalSize > 0) {

				auto oldSize = sphSystem()->nbParticles();
				sphSystem()->resize(oldSize + totalSize);

				size_t counter = 0;
				auto fluentPositions = DfxFluentStreamCollection::getInstance()->atrributeMap().at<glm::vec3>("@position");

				for (auto agent : queue) {
					for (size_t inx = 0; inx < agent->size; inx++) {

						sphSystem()->positions()[oldSize + counter] = fluentPositions[agent->offset + inx];
						sphSystem()->alive()[oldSize + counter] = d_attributes.alive[agent->offset + inx];
						sphSystem()->stream_index()[oldSize + counter] = agent->offset + inx;
						sphSystem()->velocity()[oldSize + counter] = d_attributes.velocity[agent->offset + inx];
						sphSystem()->useViscosity()[oldSize + counter] = d_attributes.use_viscosity[agent->offset + inx];
						sphSystem()->viscosity()[oldSize + counter] = d_attributes.viscosity[agent->offset + inx];
						sphSystem()->useVorticity()[oldSize + counter] = d_attributes.use_vorticity[agent->offset + inx];
						sphSystem()->vorticity()[oldSize + counter] = d_attributes.vorticity[agent->offset + inx];
						sphSystem()->useSurfaceTension()[oldSize + counter] = d_attributes.use_tension[agent->offset + inx];
						sphSystem()->surfaceTension()[oldSize + counter] = d_attributes.tension[agent->offset + inx];
						sphSystem()->frozen()[oldSize + counter] = d_attributes.frozen[agent->offset + inx];

						if (d_attributes.sph_class[agent->offset + inx] == DfxRiptideActorNode::ActorType::FLUID) {
							sphSystem()->type()[oldSize + counter] = rtx::FLUID_PARTICLE;
						}
						else if (d_attributes.sph_class[agent->offset + inx] == DfxRiptideActorNode::ActorType::COLLISION_OBJECT) {
							sphSystem()->type()[oldSize + counter] = rtx::BOUNDARY_PARTICLE;
						}

						counter++;
					}

					if (type == DfxFluentSet::SIMPLEX) {
						_fluentSimplexAgents.push_back(agent);
					}
					else {
						_fluentStreamAgents.push_back(agent);
					}
				}

				sphSystem()->resetNeighbourSearch();

				if (_simulationConfig.searchMethod == 1) {
					sphSystem()->findNeighbours();
				}

				rtx::Simulator::init(sphSystem(), _simulationConfig);
			}
		}
		queue.clear();
	}
}

void DfxRiptideStageNode::updateSPHSystem() {
	auto d_attributes = fetchDefaultStreamAttributes(false);

	if (!d_attributes.success) return;

	auto positionStream = DfxFluentStreamCollection::getInstance()->atrributeMap().at<glm::vec3>("@position");

	for (long long int inx = 0; inx < sphSystem()->nbParticles(); inx++) {
		auto stream_index = sphSystem()->stream_index()[inx];

		if (d_attributes.update[stream_index]) {
			sphSystem()->positions()[inx] = positionStream[stream_index];
			sphSystem()->alive()[inx] = d_attributes.alive[stream_index];
			sphSystem()->velocity_i()[inx] = d_attributes.velocity[stream_index];
		}

		d_attributes.update[stream_index] = 0;
	}
}

void DfxRiptideStageNode::stepUpdateCallback(int step, float timestep, rtx::SPHSystem *system, void *userdata) {
	auto stage = (DfxRiptideStageNode*)userdata;

	for (auto prop : stage->props()) {
		if (prop->propType() == riptide::type::EMITTER) {
			auto riptideEmitter = (DfxRiptideEmitterPropNode*)prop;

			riptideEmitter->setTimestep(timestep);
			riptideEmitter->updateState();
			riptideEmitter->emit_particles();

			stage->updateSPHSystem();

		}
	}
}

void DfxRiptideStageNode::stepLogCallback(const char* msg) {
	DfxGlobal::console::println(msg);
}

void DfxRiptideStageNode::beginStep(DfxStageEvaluationInfo info) {
	
	for (auto actor : actors()) {
		if (actor->actorType() == riptide::type::ACTOR) {
			auto riptideActor = (DfxRiptideActorNode*)actor;

			if (riptideActor->actorState() == DfxRiptideActorNode::ActorState::SPAWN) {
				riptideActor->spawn();
				riptideActor->setActorState(DfxRiptideActorNode::ActorState::NONE);
			}

		}
	}

	for (auto prop : props()) {
		if (prop->propType() == riptide::type::EMITTER) {
			auto riptideEmitter = (DfxRiptideEmitterPropNode*)prop;

			if (riptideEmitter->propState() == DfxRiptideActorNode::ActorState::SPAWN) {
				riptideEmitter->spawn();
				riptideEmitter->setPropState(DfxRiptideActorNode::ActorState::NONE);
			}

		}

		if (prop->propType() == riptide::type::HEATER) {
			auto riptideHeater = (DfxRiptideHeaterPropNode*)prop;
			
			if (riptideHeater->propState() == DfxRiptideHeaterPropNode::PropState::SPAWN) {
				riptideHeater->spawn();
				riptideHeater->setPropState(DfxRiptideHeaterPropNode::PropState::NONE);
			}

			riptideHeater->updateHeaterState();
		}
	}

	applySphSystemSettings();

	processAgentQueue(_fluentSimplexAgentQueue, DfxFluentSet::SIMPLEX);
	processAgentQueue(_fluentStreamAgentQueue, DfxFluentSet::STREAM);
}

void DfxRiptideStageNode::step(DfxStageEvaluationInfo info) {
	rtx::Simulator::step(sphSystem(), _simulationConfig, stepUpdateCallback, stepLogCallback, this);
}

void DfxRiptideStageNode::endStep(DfxStageEvaluationInfo info) {
	updateStream();
	auto frame = getSimulationFrame();
	//clearPreviewFrame();

	for (auto prop : props()) {
		if (prop->propType() == riptide::type::CACHE) {
			auto cacheProp = (DfxRiptideCachePropNode*)prop;
			cacheProp->addCachedFrame(info.currentTime.frame, frame);
			cacheProp->updatePointCloud(frame, info.currentTime.frame);
		}
	}
}

std::vector<DfxTime> DfxRiptideStageNode::subFrames() {
	return std::vector<DfxTime> {DfxTime()};
}

std::vector<std::pair<QWidget*, QString>> DfxRiptideStageNode::propertyForms() {
	std::pair<QWidget*, QString> properties(form(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> { properties };
}

QWidget *DfxRiptideStageNode::form() {

	QWidget *widget = new QWidget;

	DfxQStackControl *timingStack = new DfxQStackControl;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto riptideControlPanel = new DfxControlPanel();

	DfxQControlArray *riptideControlArray = new DfxQControlArray();

	DfxQControlArray *adaptiveControlArray = new DfxQControlArray();
	DfxQControlArray *fpsControlArray = new DfxQControlArray();
	DfxQControlArray *fixedtideControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// Main
	auto resolution = new DfxQDecimalSpinnerControl(path + "resolution", "", false);
	auto device_type = new DfxQComboBoxControl(path + "device", "", devicesQControlenums, false);
	auto search = new DfxQComboBoxControl(path + "search", "", searchQControlenums, false);

	resolution->setMinValue(0.01f);

	// Timing
	auto timing_type = new DfxQComboBoxControl(path + "timing_type", "", timingQControlenums, false);

	auto fixed_timstep  = new DfxQDecimalSpinnerControl(path + "fixed_timstep", "", false);
	auto fixed_substeps = new DfxQIntegerSpinnerControl(path + "fixed_substeps", "", false);

	auto fps_timstep  = new DfxQDecimalSpinnerControl(path + "fps_timestep", "", false);
	auto fps_substeps = new DfxQIntegerSpinnerControl(path + "fps_fps", "", false);

	auto adaptive_min_timestep = new DfxQDecimalSpinnerControl(path + "adaptive_min_timestep", "", false);
	auto adaptive_max_timestep = new DfxQDecimalSpinnerControl(path + "adaptive_max_timestep", "", false);
	auto adaptive_fps = new DfxQIntegerSpinnerControl(path + "adaptive_fps", "", false);

	// Corrections
	auto divergence_free = new DfxQCheckBoxControl(path + "divergence_free", "", false);
	auto divergence_error = new DfxQDecimalSpinnerControl(path + "divergence_error", "", false);
	auto density_error = new DfxQDecimalSpinnerControl(path + "density_error", "", false);

	auto pressure_relaxation = new DfxQDecimalSpinnerControl(path + "pressure_relaxation", "", false);
	auto coupling_stiffness  = new DfxQDecimalSpinnerControl(path + "coupling_stiffness", "", false);
	auto velocity_dampening  = new DfxQDecimalSpinnerControl(path + "velocity_dampening", "", false);

	auto gravity_x = new DfxQDecimalSpinnerControl(path + "gravity_x", "", false);
	auto gravity_y = new DfxQDecimalSpinnerControl(path + "gravity_y", "", false);
	auto gravity_z = new DfxQDecimalSpinnerControl(path + "gravity_z", "", false);

	// Solvers
	riptideControlArray->addControl("Device", device_type);
	riptideControlArray->addSpacer(0, 2);
	riptideControlArray->addControl("Search Type", search);
	riptideControlArray->addSpacer(0, 2);
	riptideControlArray->addControl("Particle Resolution", resolution);
	riptideControlArray->addSpacer(0, 2);
	riptideControlArray->addControl("Timestep Method", timing_type);
	riptideControlArray->addSpacer(0, 2);
	riptideControlArray->addControl(timingStack);
	riptideControlArray->addSpacer(0, 2);
	riptideControlArray->addControl("Divergence Free", divergence_free);
	riptideControlArray->addSpacer(0, 2);
	riptideControlArray->addControl("Divergence Error", divergence_error);
	riptideControlArray->addSpacer(0, 2);
	riptideControlArray->addControl("Density Error", density_error);
	riptideControlArray->addSpacer(0, 2);
	riptideControlArray->addControl("Pressure Relaxation", pressure_relaxation);
	riptideControlArray->addSpacer(0, 2);
	riptideControlArray->addControl("Gravity X", gravity_x);
	riptideControlArray->addSpacer(0, 2);
	riptideControlArray->addControl("Y", gravity_y);
	riptideControlArray->addSpacer(0, 2);
	riptideControlArray->addControl("Z", gravity_z);
	
	adaptiveControlArray->addControl("Min Timestep", adaptive_min_timestep);
	adaptiveControlArray->addSpacer(0, 2);
	adaptiveControlArray->addControl("Max Timestep", adaptive_max_timestep);
	adaptiveControlArray->addSpacer(0, 2);
	adaptiveControlArray->addControl("Fps", adaptive_fps);

	fpsControlArray->addControl("Timestep", fps_timstep);
	fpsControlArray->addSpacer(0, 2);
	fpsControlArray->addControl("Fps", fps_substeps);

	fixedtideControlArray->addControl("Timestep", fixed_timstep);
	fixedtideControlArray->addSpacer(0, 2);
	fixedtideControlArray->addControl("Substeps", fixed_substeps);

	timingStack->addPage(adaptiveControlArray);
	timingStack->addPage(fpsControlArray);
	timingStack->addPage(fixedtideControlArray);

	timingStack->changeCurrentPage(timing_type->comboBox()->currentIndex());

	riptideControlPanel->setTitle("Riptide");
	riptideControlPanel->setWidget(riptideControlArray);

	main_layout->addWidget(riptideControlPanel);

	widget->setLayout(main_layout);

	QObject::connect(timing_type->comboBox(), QOverload<int>::of(&QComboBox::activated), timingStack, &DfxQStackControl::changeCurrentPage);

	return widget;
}

bool DfxRiptideStageNode::hasFluentSimplexObject(size_t uid) {
	return false;
}

bool DfxRiptideStageNode::hasFluentStreamObject(size_t uid) {
	return false;
}

void DfxRiptideStageNode::clearFluent() {

	while(!_fluentSimplexAgents.empty()) {
		delete _fluentSimplexAgents[0];
		_fluentSimplexAgents.erase(_fluentSimplexAgents.begin());
	}

	while (!_fluentStreamAgents.empty()) {
		delete _fluentStreamAgents[0];
		_fluentStreamAgents.erase(_fluentStreamAgents.begin());
	}
}
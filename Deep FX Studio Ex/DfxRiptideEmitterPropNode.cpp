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

#include <RtxCommon.h>
#include "DfxRiptideActorNode.h"
#include "DfxRiptideEmitterPropNode.h"
#include "DfxRiptideStageNode.h"

std::string DfxRiptideEmitterPropNode::_uniqueName = "UID_RIPTIDE_EMITTER_PROP_NODE";

DfxRiptideEmitterPropNode::DfxRiptideEmitterPropNode(deepsg::SgStageNode *stage) : DfxSimulationStagePropBase(stage, riptide::type::EMITTER) {

	radiusAny = nullptr;

	circleRadius = 1.0f;
	squareHeight = 1.0f;
	squareWidth = 1.0f;
	velocity = glm::vec3(0.0f, 1.0f, 0.0f);
	position = glm::vec3(0.0f);
	rotation = glm::vec3(1.0f);
	xform = glm::mat4(1.f);
	randomnessCounter = 1;
	uQuats = false;
	uXform = false;

	emitterTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Circle", EmitterShape::CIRCLE));
	emitterTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Square", EmitterShape::SQUARE));

}

DfxRiptideEmitterPropNode::~DfxRiptideEmitterPropNode() {
	
}

std::string DfxRiptideEmitterPropNode::uniqueName() {
	return _uniqueName;
}

std::string DfxRiptideEmitterPropNode::getUniqueName() {
	return _uniqueName;
}

size_t DfxRiptideEmitterPropNode::propState() {
	return _propState;
}

void DfxRiptideEmitterPropNode::setPropState(size_t ps) {
	_propState = ps;
}

deepsg::SgShape *DfxRiptideEmitterPropNode::shape() {

	float radius = 0.5;

	if (_shape != nullptr) {
		delete _shape;
		_shape = nullptr;
	}

	_shape = new deepsg::SgShape;
	_shape->setScale(1.f);

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {

		if (any->type() == fl::type::DATA_OBJECT) {
			auto dao = any->rawData<fl::DataObject>();

			auto type = dao->property("type")->data<unsigned int>();

			if (type == EmitterShape::CIRCLE) {
				return drawCircleEmitter(dao);
			}
			else if (type == EmitterShape::SQUARE) {
				return drawSquareEmitter(dao);
			}
		}

	}

	return nullptr;
}

void DfxRiptideEmitterPropNode::emitParticles(glm::vec3 *vi) {
	
}

void DfxRiptideEmitterPropNode::spawn() {

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	auto riptideStage = (DfxRiptideStageNode*)stage();
	auto pradius = riptideStage->particleRadius();
	auto _h = pradius * 4.f;

	if (any != nullptr) {

		auto dao = any->rawData<fl::DataObject>();

		auto fluent_set = dao->property("fluent_set")->data<std::string>();
		auto type = dao->property("type")->data<float>();
		auto particleLimit = dao->property("particle_limit")->data<unsigned int>();
		auto strength = dao->property("strength")->data<float>();
		auto radius = dao->property("radius")->data<float>();
		auto width  = dao->property("width")->data<float>();
		auto height = dao->property("height")->data<float>();
		auto group = dao->property("group_id")->data<int>();
		auto c_r = dao->property("color_r")->data<float>();
		auto c_g = dao->property("color_g")->data<float>();
		auto c_b = dao->property("color_b")->data<float>();

		auto use_viscosity	= dao->property("use_viscosity")->data<bool>();
		auto viscosity		= dao->property("viscosity")->data<float>();
		auto use_vorticity	= dao->property("use_vorticity")->data<bool>();
		auto vorticity		= dao->property("vorticity")->data<float>();
		auto use_surface_tension = dao->property("use_surface_tension")->data<bool>();
		auto surface_tension = dao->property("surface_tension")->data<float>();

		if (fluent_set.empty()) {
			DfxGlobal::console::println("Riptide: Actors & Props must have sets, please specify set name.");
			return;
		}
		
		if (particleLimit > 0) {

			size_t streamOffset = DfxFluentStreamCollection::getInstance()->growCollection(particleLimit);
			auto positionStream = DfxFluentStreamCollection::getInstance()->atrributeMap().at<glm::vec3>("@position");
	
			auto d_attributes = riptideStage->fetchDefaultStreamAttributes(true);

			if (!d_attributes.success) return;

			auto agent = new DfxRiptideAgent;
			DfxFluentSet *set = DfxFluentSetManager::getInstance()->getSet(fluent_set);

			if (set == nullptr) {
				set = new DfxFluentSet(DfxFluentSet::STREAM);
				DfxFluentSetManager::getInstance()->addSet(fluent_set, set);
			}
			set->setIsVisible(false);

			for (size_t inx = 0; inx < particleLimit; inx++) {

				positionStream[streamOffset + inx]	= glm::vec3((inx * _h), 0.f, 0.f);
				d_attributes.sph_class[streamOffset + inx] = DfxRiptideActorNode::ActorType::FLUID;
				d_attributes.alive[streamOffset + inx] = 0;
				d_attributes.update[streamOffset + inx] = 0;
				d_attributes.use_viscosity[streamOffset + inx] = use_viscosity;
				d_attributes.viscosity[streamOffset + inx] = viscosity;
				d_attributes.use_vorticity[streamOffset + inx] = use_vorticity;
				d_attributes.vorticity[streamOffset + inx] = vorticity;
				d_attributes.use_tension[streamOffset + inx] = use_surface_tension;
				d_attributes.tension[streamOffset + inx] = surface_tension;

				set->indices().push_back(streamOffset + inx);
			}

			agent->type = DfxRiptideActorNode::ActorType::FLUID;
			agent->group = group;
			agent->color = glm::vec3(c_r, c_g, c_b);
			agent->offset = streamOffset;
			agent->size = particleLimit;
			_agent = agent;

			auto riptide = (DfxRiptideStageNode*)stage();
			riptide->queueStreamAgent(agent);

			setStrength(strength);
			setParticleDiameter(pradius * 2.1f);
			distanceCounter = pradius * 2.1f;
			uDisplacement = 0.f;
			vDisplacement = 0.f;

			if (type == 0) {
				setRadius(radius);
				emissionPositions.clear();
				generateCirclePositions();
			}
			else {
				setSquareWidth(width); 
				setSquareHeight(height);
				emissionPositions.clear();
				generateSquarePositions();
			}
		}
		
	}

}

deepsg::SgShape *DfxRiptideEmitterPropNode::drawCircleEmitter(fl::DataObject *dao) {

	deepsg::SgShapeBuffer shBuffer;
	shBuffer.setColor(153, 255, 192);

	auto radius = dao->property("radius")->data<float>();

	double x_circ[31];
	double y_circ[31];

	double scale = 0.25 * radius;
	double halfwidth = scale * 0.5f;

	for (int i = 0; i <= 30; i++) {
		double angle = 2 * RTX_PI *i / 30;
		double x = cos(angle)*radius;
		double y = sin(angle)*radius;

		x_circ[i] = x;
		y_circ[i] = y;
	}

	for (int i = 0; i <= 29; i++) {

		double pb1[] = { x_circ[i],   0.0, y_circ[i] };
		double pb2[] = { x_circ[i + 1], 0.0, y_circ[i + 1] };

		shBuffer.drawLine(glm::vec3(pb1[0], pb1[1], pb1[2]), glm::vec3(pb2[0], pb2[1], pb2[2]));
	}

	double v1[] = { 0.0, radius*2.0, 0.0 };
	double v2[] = { 0.0, radius*2.0, 0.0 };
	double v3[] = { 0.0, radius*2.0, 0.0 };
	double v4[] = { 0.0, radius*2.0, 0.0 };

	for (int i = 0; i <= 15; i++) {
		double angle = 2 * RTX_PI*i / 15;
		double x = cos(angle)*halfwidth;
		double y = sin(angle)*halfwidth;

		x_circ[i] = x;
		y_circ[i] = y;

		if (i == 0) {
			v1[0] = x;
			v1[2] = y;
		}

		if (i == 4) {
			v2[0] = x;
			v2[2] = y;
		}

		if (i == 7) {
			v3[0] = x;
			v3[2] = y;
		}

		if (i == 12) {
			v4[0] = x;
			v4[2] = y;
		}
	}

	for (int i = 0; i <= 14; i++) {

		double pb1[] = { x_circ[i],   radius * 2, y_circ[i] };
		double pb2[] = { x_circ[i + 1], radius * 2, y_circ[i + 1] };

		shBuffer.drawLine(glm::vec3(pb1[0], pb1[1], pb1[2]), glm::vec3(pb2[0], pb2[1], pb2[2]));

	}

	double ln1[] = { 0.0, 0.0f, 0.0f };
	double ln2[] = { 0.0, radius*2.0, 0.0 };

	shBuffer.drawLine(glm::vec3(ln1[0], ln1[1], ln1[2]), glm::vec3(ln2[0], ln2[1], ln2[2]));

	double v5[] = { 0.0,(radius*2.0) + scale, 0.0 };

	shBuffer.drawLine(glm::vec3(v1[0], v1[1], v1[2]), glm::vec3(v5[0], v5[1], v5[2]));

	shBuffer.drawLine(glm::vec3(v2[0], v2[1], v2[2]), glm::vec3(v5[0], v5[1], v5[2]));

	shBuffer.drawLine(glm::vec3(v3[0], v3[1], v3[2]), glm::vec3(v5[0], v5[1], v5[2]));

	shBuffer.drawLine(glm::vec3(v4[0], v4[1], v4[2]), glm::vec3(v5[0], v5[1], v5[2]));

	_shape->shapes().push_back(shBuffer);

	return _shape;
}

deepsg::SgShape *DfxRiptideEmitterPropNode::drawSquareEmitter(fl::DataObject *dao) {

	deepsg::SgShapeBuffer shBuffer;
	shBuffer.setColor(153, 255, 192);

	auto height = dao->property("height")->data<float>();
	auto width = dao->property("width")->data<float>();

	float halfwidth = width;
	float halfheight = height;

	float sfactor = 0.25f;

	double verts[][3] = { { -halfwidth, 0.0f, halfheight },
	{ -halfwidth, 0.0f, -halfheight },
	{ halfwidth, 0.0f,  -halfheight  },
	{ halfwidth, 0.0f,  halfheight  } };

	shBuffer.drawLine(glm::vec3(verts[0][0], verts[0][1], verts[0][2]), glm::vec3(verts[1][0], verts[1][1], verts[1][2]));
	shBuffer.drawLine(glm::vec3(verts[1][0], verts[1][1], verts[1][2]), glm::vec3(verts[2][0], verts[2][1], verts[2][2]));
	shBuffer.drawLine(glm::vec3(verts[2][0], verts[2][1], verts[2][2]), glm::vec3(verts[3][0], verts[3][1], verts[3][2]));
	shBuffer.drawLine(glm::vec3(verts[3][0], verts[3][1], verts[3][2]), glm::vec3(verts[0][0], verts[0][1], verts[0][2]));

	double x_circ[31];
	double y_circ[31];

	double v1[] = { 0.0, (width + height), 0.0 };
	double v2[] = { 0.0, (width + height), 0.0 };
	double v3[] = { 0.0, (width + height), 0.0 };
	double v4[] = { 0.0, (width + height), 0.0 };

	for (int i = 0; i <= 15; i++) {
		double angle = 2 * RTX_PI*i / 15;
		double x = cos(angle)*0.0625f*(width + height);
		double y = sin(angle)*0.0625f*(width + height);

		x_circ[i] = x;
		y_circ[i] = y;

		if (i == 0) {
			v1[0] = x;
			v1[2] = y;
		}

		if (i == 4) {
			v2[0] = x;
			v2[2] = y;
		}

		if (i == 7) {
			v3[0] = x;
			v3[2] = y;
		}

		if (i == 12) {
			v4[0] = x;
			v4[2] = y;
		}
	}

	for (int i = 0; i <= 14; i++) {

		double pb1[] = { x_circ[i],   (width + height), y_circ[i] };
		double pb2[] = { x_circ[i + 1], (width + height), y_circ[i + 1] };

		shBuffer.drawLine(glm::vec3(pb1[0], pb1[1], pb1[2]), glm::vec3(pb2[0], pb2[1], pb2[2]));

	}

	double ln1[] = { 0.0, 0.0f, 0.0f };
	double ln2[] = { 0.0, (width + height), 0.0 };

	shBuffer.drawLine(glm::vec3(ln1[0], ln1[1], ln1[2]), glm::vec3(ln2[0], ln2[1], ln2[2]));

	double v5[] = { 0.0,((width + height)) + (width + height)*0.0625f, 0.0 };

	shBuffer.drawLine(glm::vec3(v1[0], v1[1], v1[2]), glm::vec3(v5[0], v5[1], v5[2]));
	shBuffer.drawLine(glm::vec3(v2[0], v2[1], v2[2]), glm::vec3(v5[0], v5[1], v5[2]));
	shBuffer.drawLine(glm::vec3(v3[0], v3[1], v3[2]), glm::vec3(v5[0], v5[1], v5[2]));
	shBuffer.drawLine(glm::vec3(v4[0], v4[1], v4[2]), glm::vec3(v5[0], v5[1], v5[2]));

	_shape->shapes().push_back(shBuffer);

	return _shape;
}

void DfxRiptideEmitterPropNode::setParticleDiameter(double pd) {
	particleDiameter = pd;
}

double DfxRiptideEmitterPropNode::getParticleDiameter() {
	return particleDiameter;
}

void DfxRiptideEmitterPropNode::setEmissionPositions(std::vector<glm::vec3> ep) {
	emissionPositions = ep;
}

std::vector<glm::vec3> DfxRiptideEmitterPropNode::getEmissionPositions() {
	return emissionPositions;
}

void DfxRiptideEmitterPropNode::setStrength(float str) {
	strength = str;
}

void DfxRiptideEmitterPropNode::setVelocity(glm::vec3 vel) {
	velocity = vel;
}

int DfxRiptideEmitterPropNode::getParticleType() {
	return particleType;
}

void DfxRiptideEmitterPropNode::setParticleType(int val) {
	particleType = val;
}

void DfxRiptideEmitterPropNode::intergrateDistanceCounter() {
	distanceCounter += (strength * dt);
}

void DfxRiptideEmitterPropNode::resetDistanceCounter() {
	distanceCounter = 0.0;
}

void DfxRiptideEmitterPropNode::setTimestep(float tmps) {
	dt = tmps;
}

glm::vec3 DfxRiptideEmitterPropNode::getPosition() {
	return position;
}

glm::vec3 DfxRiptideEmitterPropNode::getRotation() {
	return position;
}

void DfxRiptideEmitterPropNode::setPosition(glm::vec3 pos) {
	position = pos;
}

void DfxRiptideEmitterPropNode::setRotation(glm::vec3 rot) {
	rotation = rot;
}

void DfxRiptideEmitterPropNode::setRotation(glm::quat rot) {
	rotationQuat = rot;
}

float DfxRiptideEmitterPropNode::getStrength() {
	return strength;
}

glm::vec3 DfxRiptideEmitterPropNode::getVelocity() {
	return velocity;
}

float DfxRiptideEmitterPropNode::getRadius() {
	return circleRadius;
}

void DfxRiptideEmitterPropNode::setUDisplacement(float disp) {
	if (disp < 0.f)
		uDisplacement = 0.f;
	else if (disp > 1.f)
		uDisplacement = 1.f;
	else
		uDisplacement = disp;
}

float DfxRiptideEmitterPropNode::getUDisplacement() {
	return uDisplacement;
}

void DfxRiptideEmitterPropNode::setVDisplacement(float disp) {
	if (disp < 0.f)
		vDisplacement = 0.f;
	else if (disp > 1.f)
		vDisplacement = 1.f;
	else
		vDisplacement = disp;
}

void DfxRiptideEmitterPropNode::setParticleLimit(int limit) {
	particleLimit = limit;
}

int DfxRiptideEmitterPropNode::getParticleLimit() {
	return particleLimit;
}

float DfxRiptideEmitterPropNode::getVDisplacement() {
	return vDisplacement;
}

void DfxRiptideEmitterPropNode::setEmitterType(int _type) {
	emitterType = _type;
}

int DfxRiptideEmitterPropNode::getEmitterType() {
	return emitterType;
}

glm::mat4 DfxRiptideEmitterPropNode::getFinalTransformation() {
	glm::mat4 transMatrix = glm::mat4(1.0f);

	glm::mat4 rotmat;

	if (useQuaternions()) {
		rotmat = glm::mat4_cast(rotationQuat);
	}

	if (!useXformMatrix()) {
		transMatrix = glm::translate(transMatrix, position);
		transMatrix *= rotmat;
	}
	else {
		transMatrix *= xform;
	}

	return transMatrix;
}


bool DfxRiptideEmitterPropNode::useQuaternions() {
	return uQuats;
}

void DfxRiptideEmitterPropNode::setUseQuaternions(bool uq) {
	uQuats = uq;
}

void DfxRiptideEmitterPropNode::resetRandomnessCounter() {
	if (randomnessCounter == 1000000)
		randomnessCounter = 1;
	else
		randomnessCounter++;
}

void DfxRiptideEmitterPropNode::updateWithParticleArrayIndex() {
	auto riptideStage = (DfxRiptideStageNode*)stage();

	auto positionStream = DfxFluentStreamCollection::getInstance()->atrributeMap().at<glm::vec3>("@position");

	auto d_attributes = riptideStage->fetchDefaultStreamAttributes();

	if (!d_attributes.success) {
		DfxGlobal::console::println("Riptide: Emitter failed");
		return;
	}

	int positionCounter = 0;
	glm::vec3 finalVel = glm::vec3(0.0f);

	if (uDisplacement == 0.f && vDisplacement == 0.f) finalVel = getFinalVelocity();

	glm::mat4 transformation = getFinalTransformation();
	for (int i = 0; i < _agent->size; i++) {

		if (!d_attributes.alive[_agent->offset + i]) {

			if (uDisplacement > 0.f || vDisplacement > 0.f)
				finalVel = getFinalVelocity();

			d_attributes.alive[_agent->offset + i] = 1;
			d_attributes.velocity[_agent->offset + i] = finalVel;
			d_attributes.update[_agent->offset + i] = 1;
			positionStream[_agent->offset + i] = glm::vec3(transformation * glm::vec4(emissionPositions[positionCounter], 1.0f));

			/*auto check = glm::vec3(transformation * glm::vec4(emissionPositions[positionCounter], 1.0f));
			printf("%f, %f, %f\n", check.x, check.y, check.z);*/

			positionCounter++;

		}

		if (positionCounter == emissionPositions.size())
			break;
	}
}

void DfxRiptideEmitterPropNode::emit_particles() {

	if (strength <= 0.f) return;

	if (distanceCounter >= particleDiameter) {

		updateWithParticleArrayIndex();
		resetDistanceCounter();
	}
	else
		intergrateDistanceCounter();
}

void DfxRiptideEmitterPropNode::generateSquarePositions() {
	emissionPositions.clear();

	int wcount = (int)((squareWidth  * 2.0f) / (float)particleDiameter);
	int hcount = (int)((squareHeight * 2.0f) / (float)particleDiameter);

	float hwidth = -squareWidth;
	float hheight = -squareHeight;

	for (int i = 0; i < wcount; i++) {
		for (int j = 0; j < hcount; j++) {
			emissionPositions.push_back(glm::vec3(hwidth, 0.0f, hheight));

			hheight += (float)particleDiameter;
		}

		hheight = -squareHeight;
		hwidth += (float)particleDiameter;
	}
}

float DfxRiptideEmitterPropNode::getSquareWidth() {
	return squareWidth;
}

void DfxRiptideEmitterPropNode::setSquareWidth(float w) {
	if (w < 0.04f)
		squareWidth = 0.04;
	else
		squareWidth = w;
}

float DfxRiptideEmitterPropNode::getSquareHeight() {
	return squareHeight;
}

void DfxRiptideEmitterPropNode::setSquareHeight(float h) {
	if (h < 0.04f)
		squareHeight = 0.04;
	else
		squareHeight = h;
}

void DfxRiptideEmitterPropNode::generateCirclePositions() {
	emissionPositions.clear();

	int count = (int)(circleRadius / (float)particleDiameter);
	int counter = 0;

	for (int i = 0; i < count; i++) {

		if (i == 0) {
			emissionPositions.push_back(glm::vec3(0.0f));
			continue;
		}

		float partialRadius = circleRadius / (float)count;
		float radius = partialRadius * i;

		float circum = 2.0f*(float)RTX_PI*radius;

		int segments = (int)(circum / (float)particleDiameter);

		float *x_circ = new float[segments];
		float *y_circ = new float[segments];

		for (int j = 0; j < segments; j++) {
			double angle = 2.0*RTX_PI*j / segments;
			double x = cos(angle)*radius;
			double y = sin(angle)*radius;

			x_circ[j] = x;
			y_circ[j] = y;
		}

		for (int j = 0; j < segments; j++) {
			glm::vec3 pos(x_circ[j], 0.0f, y_circ[j]);
			emissionPositions.push_back(pos);
		}

		delete[] x_circ;
		delete[] y_circ;
	}
}

void DfxRiptideEmitterPropNode::setRadius(float cradius) {
	if (cradius < 0.045f)
		circleRadius = 0.045;
	else
		circleRadius = cradius;
}

glm::vec3 DfxRiptideEmitterPropNode::uvwRandomness(glm::vec3 pos) {

	/*if (emitterType == SQUARE_EMITTER || emitterType == CIRCLE_EMITTER) {

		if (uDisplacement == 0.f && vDisplacement == 0.f)
			return pos;

		std::time_t time = std::time(nullptr);
		std::uniform_real_distribution<> uDisp(-(90.0f * uDisplacement), (90.0f * uDisplacement));
		std::uniform_real_distribution<> vDisp(-(90.0f * vDisplacement), (90.0f * vDisplacement));

		unsigned int k1 = (unsigned int)(randomnessCounter * time) + 10;
		unsigned int k2 = (unsigned int)(randomnessCounter * time) + 20;

		std::default_random_engine genu(k1);
		std::default_random_engine genv(k2);

		float posu = (float)uDisp(genu);
		float posv = (float)vDisp(genv);

		glm::vec3 newRotation(posu, 0.0, posv);

		glm::mat4 transMatrix = glm::mat4(1.0f);
		glm::mat4 rotmat = glm::mat4_cast(LWDVectorToQuat(newRotation));

		transMatrix *= rotmat;

		resetRandomnessCounter();

		return glm::vec3(transMatrix * glm::vec4(pos, 1.0f));
	}

	return pos;*/
	return glm::vec3(0.f);
}

glm::vec3 DfxRiptideEmitterPropNode::getFinalVelocity(bool useRandomness) {

	//glm::vec3 new_vec = glm::vec3(velocity.x, velocity.y*strength, velocity.z);

	glm::vec3 new_vec = velocity;

	if (useRandomness) new_vec = uvwRandomness(new_vec);

	glm::normalize(new_vec);

	new_vec *= strength;

	glm::mat4 transMatrix = glm::mat4(1.0f);

	glm::mat4 rotmat;

	if (useQuaternions()) {
		rotmat = glm::mat4_cast(rotationQuat);
	}

	if (!useXformMatrix()) {
		transMatrix *= rotmat;
	}
	else {
		transMatrix = xform;
	}

	return glm::vec3(transMatrix * glm::vec4(new_vec, 1.0f));
}

glm::mat4 DfxRiptideEmitterPropNode::xformMatrix() {
	return xform;
}

void DfxRiptideEmitterPropNode::setXformMatrix(glm::mat4 &xf) {
	xform = xf;
}

bool DfxRiptideEmitterPropNode::useXformMatrix() {
	return uXform;
}

void DfxRiptideEmitterPropNode::setUseXformMatrix(bool bl) {
	uXform = bl;
}

DfxRiptideAgent *DfxRiptideEmitterPropNode::agent() {
	return _agent;
}

void DfxRiptideEmitterPropNode::updateState() {
	auto xform = deepsg::SgRootNode::getInstance()->getTransformation4x4(this);

	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(xform, scale, rotation, translation, skew, perspective);

	setRotation(glm::conjugate(rotation));
	setPosition(translation);
	setUseQuaternions(true);
}

std::vector<std::pair<QWidget*, QString>> DfxRiptideEmitterPropNode::propertyForms() {
	std::pair<QWidget*, QString> props(properties(), "General");
	return std::vector<std::pair<QWidget*, QString>> { props };
}

QWidget *DfxRiptideEmitterPropNode::properties() {

	QWidget *widget = new QWidget;

	DfxQStackControl *emitterWidgetStack = new DfxQStackControl;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto propertiesControlPanel = new DfxControlPanel();
	auto forcesControlPanel = new DfxControlPanel();

	DfxQControlArray *propertiesControlArray = new DfxQControlArray();
	DfxQControlArray *circleControlArray = new DfxQControlArray();
	DfxQControlArray *squareControlArray = new DfxQControlArray();

	DfxQControlArray *forcesControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// General
	auto fluent_set = new DfxQLineEditControl(path + "fluent_set", "", false);
	auto type = new DfxQComboBoxControl(path + "type", "", emitterTypeQControlenums, false);
	auto limit = new DfxQIntegerSpinnerControl(path + "particle_limit", "", false);
	auto strength = new DfxQDecimalSpinnerControl(path + "strength", "", false);
	auto radius = new DfxQDecimalSpinnerControl(path + "radius", "", false);
	auto height = new DfxQDecimalSpinnerControl(path + "height", "", false);
	auto width = new DfxQDecimalSpinnerControl(path + "width", "", false);
	auto urandom = new DfxQDecimalSpinnerControl(path + "u_displacement", "", false);
	auto vrandom = new DfxQDecimalSpinnerControl(path + "v_displacement", "", false);
	auto group_id = new DfxQIntegerSpinnerControl(path + "group_id", "", false);
	auto color = new DfxQColorControl(path + "color_r", path + "color_g", path + "color_b", "", false);

	// Fluid 
	auto use_viscosity = new DfxQCheckBoxControl(path + "use_viscosity", "", false);
	auto viscosity = new DfxQDecimalSpinnerControl(path + "viscosity", "", false);
	auto use_vorticity = new DfxQCheckBoxControl(path + "use_vorticity", "", false);
	auto vorticity = new DfxQDecimalSpinnerControl(path + "vorticity", "", false);
	auto vorticity_viscosity = new DfxQDecimalSpinnerControl(path + "vorticity_viscosity", "", false);
	auto vorticity_inertia = new DfxQDecimalSpinnerControl(path + "vorticity_inertia", "", false);
	auto use_surface_tension = new DfxQCheckBoxControl(path + "use_surface_tension", "", false);
	auto surface_tension = new DfxQDecimalSpinnerControl(path + "surface_tension", "", false);

	// General
	propertiesControlArray->addControl("Fluent Set", fluent_set);
	propertiesControlArray->addSpacer(1, 2);
	propertiesControlArray->addControl("Type", type);
	propertiesControlArray->addSpacer(1, 2);
	propertiesControlArray->addControl("Particle Limit", limit);
	propertiesControlArray->addSpacer(1, 2);
	propertiesControlArray->addControl("Strength", strength);
	propertiesControlArray->addSpacer(1, 2);
	propertiesControlArray->addControl("U Randomness", urandom);
	propertiesControlArray->addSpacer(1, 2);
	propertiesControlArray->addControl("V Randomness", vrandom);
	propertiesControlArray->addSpacer(1, 2);
	propertiesControlArray->addControl("Group ID", group_id);
	propertiesControlArray->addSpacer(1, 2);
	propertiesControlArray->addControl("Color", color);
	propertiesControlArray->addSpacer(1, 2);
	propertiesControlArray->addControl(emitterWidgetStack);

	circleControlArray->addControl("Radius", radius);

	squareControlArray->addControl("Height", height);
	squareControlArray->addSpacer(1, 2);
	squareControlArray->addControl("Width", width);

	emitterWidgetStack->addPage(circleControlArray);
	emitterWidgetStack->addPage(squareControlArray);

	emitterWidgetStack->changeCurrentPage(type->comboBox()->currentIndex());

	propertiesControlPanel->setTitle("Emitter Settings");
	propertiesControlPanel->setWidget(propertiesControlArray);

	forcesControlArray->addControl("Use Viscosity", use_viscosity);
	forcesControlArray->addControl("Viscosity Strength", viscosity);
	forcesControlArray->addSpacer(1, 2);
	forcesControlArray->addControl("Use Vorticity", use_vorticity);
	forcesControlArray->addControl("Vorticity Strength", vorticity);
	// forcesControlArray->addControl("Vorticity Viscosity", vorticity_viscosity);
	// forcesControlArray->addControl("Vorticity Inertia", vorticity_inertia);
	forcesControlArray->addSpacer(1, 2);
	forcesControlArray->addControl("Use Surface Tension", use_surface_tension);
	forcesControlArray->addControl("Surface Tension Strength", surface_tension);

	forcesControlPanel->setTitle("Fluid Settings");
	forcesControlPanel->setWidget(forcesControlArray);

	main_layout->addWidget(propertiesControlPanel);
	main_layout->addWidget(forcesControlPanel);

	widget->setLayout(main_layout);

	//// Signal & Slots
	QObject::connect(type->comboBox(), QOverload<int>::of(&QComboBox::activated), emitterWidgetStack, &DfxQStackControl::changeCurrentPage);

	return widget;
}
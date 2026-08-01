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

#include "DfxBulletConstraintPropNode.h"
#include "DfxBulletConstraintPropHandler.h"
#include "DfxBulletActorNode.h"
#include "DfxBulletAgent.h"
#include "DfxBulletConstraintWidget.h"

std::string DfxBulletConstraintPropNode::_uniqueName = "UID_BULLET_CONTRAINT_PROP_NODE";

DfxBulletConstraintPropNode::DfxBulletConstraintPropNode(deepsg::SgStageNode *stage): DfxSimulationStagePropBase(stage, bullet::CONSTRAINT_PROP){
	
	typeQControlenums.push_back(std::pair<std::string, unsigned int>("Point To Point", Type::POINT_TO_POINT));
	typeQControlenums.push_back(std::pair<std::string, unsigned int>("Hinge", Type::HINGE));

	_dynamicsWorld = nullptr;
	_constraint = nullptr;
	_constraint_type = -1;
	agentA = nullptr;
	agentB = nullptr;
}

DfxBulletConstraintPropNode::~DfxBulletConstraintPropNode() {

}

std::string DfxBulletConstraintPropNode::uniqueName() {
	return _uniqueName;
}

std::string DfxBulletConstraintPropNode::getUniqueName() {
	return _uniqueName;
}

size_t DfxBulletConstraintPropNode::propState() {
	return _propState;
}

void DfxBulletConstraintPropNode::setPropState(size_t pst) {
	_propState = pst;
}

btDiscreteDynamicsWorld *DfxBulletConstraintPropNode::dynamicsWorld() {
	return _dynamicsWorld;
}

void DfxBulletConstraintPropNode::setDynamicsWorld(btDiscreteDynamicsWorld *btWorld) {
	_dynamicsWorld = btWorld;
}

deepsg::SgNode *DfxBulletConstraintPropNode::findNode(std::string path) {

	auto any = DfxSpace::getInstance()->findAbsolute(path);

	if (any != nullptr) {

		if (any->type() == fl::type::DATA_OBJECT) {
			auto dao = any->rawData<fl::DataObject>();

			if (dao->hasProperty("sg_geo_node")) {
				return dao->property("sg_geo_node")->rawData<deepsg::SgGeoNode>();
			}
			else if (dao->hasProperty("sg_null_node")) {
				return dao->property("sg_null_node")->rawData<DfxNullNode>();
			}

		}

	}

	return nullptr;
}

std::vector<DfxBulletAgent*> DfxBulletConstraintPropNode::findRigidBodyAgents(std::string path) {
	std::vector<DfxBulletAgent*> output;

	DfxBulletActorHandler handler;

	if (handler.fetch(path)) {
		if (handler.dataObject()->type() == fl::type::DATA_OBJECT) {
			auto dao = handler.dataObject()->rawData<fl::DataObject>();

			if (dao->hasProperty("sg_stage_actor_node")) {
				if (dao->property("sg_stage_actor_node")->type() == DfxFluentTypes::SG_STAGE_ACTOR_NODE) {
					deepsg::SgStageActorNode *anode = dao->property("sg_stage_actor_node")->rawData<deepsg::SgStageActorNode>();

					if (anode->stage()->stageType() == DfxStageType::BULLET_STAGE) {
						if (anode->actorType() == bullet::MAIN_ACTOR) {
							auto agents = ((DfxBulletActorNode*)anode)->agents();

							if (!agents.empty()) {
								for (auto agent : agents) {
									if (agent->rigidBody) {
										output.push_back(agent);
									}
								}
							}

						}
					}

				}
			}
		}
	}

	return output;
}

void DfxBulletConstraintPropNode::preStep() {
	updatePivot();
}

void DfxBulletConstraintPropNode::postStep() {

}

void DfxBulletConstraintPropNode::spawn() {

	DfxBulletConstraintPropHandler handler;

	if (handler.fetch(referencePath())) {

		auto dao = handler.dataObject()->rawData<fl::DataObject>();

		auto type = dao->property("constraint_type")->data<unsigned int>();
		
		DfxBulletActorHandler actorA;
		DfxBulletActorHandler actorB;

		auto ba = dao->property("body_a")->data<std::string>();
		auto bb = dao->property("body_b")->data<std::string>();

		auto p2pPivotPath = dao->property("point_to_point_pivot")->data<std::string>();

		auto hingePivotPath = dao->property("hinge_pivot")->data<std::string>();
		auto hingeEnableMotor = dao->property("hinge_enable_motor")->data<bool>();
		auto hingeTVel = dao->property("hinge_target_velocity")->data<float>();
		auto hingeMImp = dao->property("hinge_max_impulse")->data<float>();

		if (Type::POINT_TO_POINT == type) {
			_constraint_type = type;

			std::vector<DfxBulletAgent*> agents_a = findRigidBodyAgents(ba);
			std::vector<DfxBulletAgent*> agents_b = findRigidBodyAgents(bb);

			glm::vec3 pivPos(0.f);
			auto pivNode = findNode(p2pPivotPath);

			if (pivNode != nullptr) {
				pivPos = pivNode->position();
			}

			btRigidBody *bodyA = nullptr;
			btRigidBody *bodyB = nullptr;

			btTransform transA;
			btTransform transB;
	
			if (!agents_a.empty()) {
				transA = agents_a[0]->worldTransform;
				bodyA = agents_a[0]->body->toRigidBody();
				agentA = agents_a[0];
			}

			if (!agents_b.empty()) {
				transB = agents_b[0]->worldTransform;
				bodyB = agents_b[0]->body->toRigidBody();
				agentB = agents_b[0];
			}

			if (bodyA != nullptr && bodyB != nullptr) {
				auto midPoint = (transA.getOrigin() + transB.getOrigin()) / 2.f;
				auto pivotA = midPoint - transA.getOrigin();
				auto pivotB = midPoint - transB.getOrigin();
				_constraint = new btPoint2PointConstraint(*bodyA, *bodyB, pivotA, pivotB);
				_dynamicsWorld->addConstraint(_constraint);

			}
			else if (bodyA != nullptr) {
				auto pivot = btVector3(pivPos.x, pivPos.y, pivPos.z) - transA.getOrigin();
				_constraint = new btPoint2PointConstraint(*bodyA, pivot);
				_dynamicsWorld->addConstraint(_constraint);
			}
			else if (bodyB != nullptr) {
				auto pivot = btVector3(pivPos.x, pivPos.y, pivPos.z) - transB.getOrigin();
				_constraint = new btPoint2PointConstraint(*bodyB, pivot);
				_dynamicsWorld->addConstraint(_constraint);
			}

		}
		else if (Type::HINGE == type) {

			std::vector<DfxBulletAgent*> agents_a = findRigidBodyAgents(ba);
			std::vector<DfxBulletAgent*> agents_b = findRigidBodyAgents(bb);

			glm::vec3 pivPos(0.f);
			glm::vec3 orient(0.f, 1.f, 0.f);
			auto pivNode = findNode(hingePivotPath);

			if (pivNode != nullptr) {
				pivPos = pivNode->position();
				orient = glm::normalize(glm::vec3(pivNode->rotationMatrix() * glm::vec4(orient, 1.f)));
			}

			btRigidBody *bodyA = nullptr;
			btRigidBody *bodyB = nullptr;

			btTransform transA;
			btTransform transB;

			if (!agents_a.empty()) {
				transA = agents_a[0]->worldTransform;
				bodyA = agents_a[0]->body->toRigidBody();
				agentA = agents_a[0];
			}

			if (!agents_b.empty()) {
				transB = agents_b[0]->worldTransform;
				bodyB = agents_b[0]->body->toRigidBody();
				agentB = agents_b[0];
			}

			if (bodyA != nullptr && bodyB != nullptr) {
				auto pivotA = btVector3(pivPos.x, pivPos.y, pivPos.z) - transA.getOrigin();
				auto pivotB = btVector3(pivPos.x, pivPos.y, pivPos.z) - transB.getOrigin();
				btVector3 axis(orient.x, orient.y, orient.z);

				_constraint = new btHingeConstraint(*bodyA, *bodyB, pivotA, pivotB, axis, axis);
				
				_dynamicsWorld->addConstraint(_constraint);
			}
			else if (bodyA != nullptr) {
				auto pivot = btVector3(pivPos.x, pivPos.y, pivPos.z) - transA.getOrigin();
				btVector3 axis(orient.x, orient.y, orient.z);

				_constraint = new btHingeConstraint(*bodyA, pivot, axis);

				_dynamicsWorld->addConstraint(_constraint);
			}
			else if (bodyB != nullptr) {
				auto pivot = btVector3(pivPos.x, pivPos.y, pivPos.z) - transA.getOrigin();
				btVector3 axis(orient.x, orient.y, orient.z);

				_constraint = new btHingeConstraint(*bodyB, pivot, axis);
				_dynamicsWorld->addConstraint(_constraint);
			}
			
			if (_constraint != nullptr) {
				if (hingeEnableMotor) {
					((btHingeConstraint*)_constraint)->enableAngularMotor(true, hingeTVel, hingeMImp);
				}
			}
		}
		else if (Type::SLIDER == type) {

			//auto ba = dao->property("point_to_point_body_a")->data<std::string>();
			//auto bb = dao->property("point_to_point_body_b")->data<std::string>();

			//std::vector<DfxBulletAgent*> agents_a = findRigidBodyAgents(ba);
			//std::vector<DfxBulletAgent*> agents_b = findRigidBodyAgents(bb);

			//btRigidBody *bodyA = nullptr;
			//btRigidBody *bodyB = nullptr;

			//btTransform transA;
			//btTransform transB;
			//glm::vec3 piv(0.f, 0.f, 0.f);

			//if (!agents_a.empty()) {
			//	transA = agents_a[0]->worldTransform;
			//	bodyA = agents_a[0]->body->toRigidBody();
			//	agentA = agents_a[0];
			//}

			//if (bodyA != nullptr) {
			//	auto pivot = btVector3(piv.x, piv.y, piv.z) - transA.getOrigin();
			//	_constraint = new btSliderConstraint(*bodyA, pivot, btVector3(0.f, 1.f, 0.f));
			//	_dynamicsWorld->addConstraint(_constraint);

			//	//_widget->setBodies(agents_a[0], agents_b[0]);

			//}
		}
	}

	setPropState(DfxBulletConstraintPropNode::PropState::NONE);
}

void DfxBulletConstraintPropNode::updatePivot() {
	//if (_constraint != nullptr) {	

	//		btRigidBody *bodyA = nullptr;
	//		btRigidBody *bodyB = nullptr;

	//		btTransform transA;
	//		btTransform transB;
	//		glm::vec3 piv(0.f, 0.f, 0.f);

	//		if (_widget != nullptr) {
	//			piv = _widget->position();
	//		}

	//		if (agentA != nullptr) {
	//			agentA->body->toRigidBody()->getMotionState()->getWorldTransform(transA);
	//			bodyA = agentA->body->toRigidBody();
	//		}

	//		if (agentB != nullptr) {
	//			agentB->body->toRigidBody()->getMotionState()->getWorldTransform(transB);
	//			bodyB = agentB->body->toRigidBody();
	//		}

	//		if (Type::POINT_TO_POINT == _constraint_type) {

	//			if (bodyA != nullptr && bodyB != nullptr) {
	//				/*((btPoint2PointConstraint*)_constraint)->setPivotA(btVector3(piv.x, piv.y, piv.z) - transA.getOrigin());
	//				((btPoint2PointConstraint*)_constraint)->setPivotB(btVector3(piv.x, piv.y, piv.z) - transB.getOrigin());*/
	//			}
	//			else if (bodyA != nullptr) {
	//				auto f_piv = btVector3(piv.x, piv.y, piv.z) - transA.getOrigin();
	//				((btPoint2PointConstraint*)_constraint)->setPivotA(f_piv);
	//				((btPoint2PointConstraint*)_constraint)->setPivotB(bodyA->getCenterOfMassTransform()(f_piv));
	//			}
	//			else if (bodyB != nullptr) {
	//				((btPoint2PointConstraint*)_constraint)->setPivotA(btVector3(piv.x, piv.y, piv.z) - transB.getOrigin());
	//				((btPoint2PointConstraint*)_constraint)->setPivotB(btVector3(piv.x, piv.y, piv.z) - transB.getOrigin());
	//			}

	//		}
	//	
	//}
}

void DfxBulletConstraintPropNode::clear() {

	if (_constraint != nullptr) {
		_dynamicsWorld->removeConstraint(_constraint);
		delete _constraint;
		_constraint = nullptr;
		_constraint_type = -1;
		agentA = nullptr;
		agentB = nullptr;
	}

	setPropState(DfxBulletConstraintPropNode::PropState::SPAWN);
}

std::vector<std::pair<QWidget*, QString>> DfxBulletConstraintPropNode::propertyForms() {
	std::pair<QWidget*, QString> propertiesForm(properties(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> { propertiesForm };
}

QWidget *DfxBulletConstraintPropNode::properties() {

	QWidget *widget = new QWidget;
	QWidget *pointToPointWidget = new QWidget;
	
	DfxQStackControl *widgetStack = new DfxQStackControl;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto typeControlPanel = new DfxControlPanel();
	auto pointToPointControlPanel = new DfxControlPanel();

	DfxQControlArray *typeControlArray = new DfxQControlArray();
	DfxQControlArray *pointToPointControlArray = new DfxQControlArray();
	DfxQControlArray *hingeControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// Type Controls
	auto type = new DfxQComboBoxControl(path + "constraint_type", "", typeQControlenums, false);

	// Point To Point Controls
	auto body_a = new DfxQFluentPathControl(path + "body_a", "", false);
	auto body_b = new DfxQFluentPathControl(path + "body_b", "", false);

	auto p2p_pivot = new DfxQFluentPathControl(path + "point_to_point_pivot", "", false);

	auto hinge_pivot = new DfxQFluentPathControl(path + "hinge_pivot", "", false);
	auto hinge_enable_motor = new DfxQCheckBoxControl(path + "hinge_enable_motor", "", false);
	auto hinge_t_vel = new DfxQDecimalSpinnerControl(path + "hinge_target_velocity", "", false);
	auto hinge_m_imp = new DfxQDecimalSpinnerControl(path + "hinge_max_impulse", "", false);

	// Type
	typeControlArray->addControl("Rigid Body A", body_a);
	typeControlArray->addSpacer(1, 2);
	typeControlArray->addControl("Rigid Body B", body_b);
	typeControlArray->addSpacer(1, 6);
	typeControlArray->addControl("Type", type);
	typeControlArray->addControl(widgetStack);

	typeControlPanel->setTitle("Constraint");
	typeControlPanel->setWidget(typeControlArray);

	// Point To Point
	pointToPointControlArray->addControl("Pivot", p2p_pivot);

	// Hinge
	hingeControlArray->addControl("Pivot", hinge_pivot);
	hingeControlArray->addSpacer(1, 2);
	hingeControlArray->addControl("Enable Motor", hinge_enable_motor);
	hingeControlArray->addSpacer(1, 2);
	hingeControlArray->addControl("Target Velocity", hinge_t_vel);
	hingeControlArray->addSpacer(1, 2);
	hingeControlArray->addControl("Max Impulse", hinge_m_imp);

	// Stack Pages
	widgetStack->addPage(pointToPointControlArray);
	widgetStack->addPage(hingeControlArray);

	widgetStack->changeCurrentPage(type->comboBox()->currentIndex());

	main_layout->addWidget(typeControlPanel);
	widget->setLayout(main_layout);

	QObject::connect(type->comboBox(), QOverload<int>::of(&QComboBox::activated), widgetStack, &DfxQStackControl::changeCurrentPage);

	return widget;
}
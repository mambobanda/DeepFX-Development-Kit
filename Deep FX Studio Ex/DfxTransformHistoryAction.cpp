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

#include "DfxTransformHistoryAction.h"

DfxTransformHistoryAction::DfxTransformHistoryAction() {

	_posx = nullptr;
	_posy = nullptr;
	_posz = nullptr;

	_rotx = nullptr;
	_roty = nullptr;
	_rotz = nullptr;

	_sclx = nullptr;
	_scly = nullptr;
	_sclz = nullptr;

}

DfxTransformHistoryAction::DfxTransformHistoryAction(
	DfxAnyHistoryAction *px, DfxAnyHistoryAction *py, DfxAnyHistoryAction *pz,
	DfxAnyHistoryAction *rx, DfxAnyHistoryAction *ry, DfxAnyHistoryAction *rz,
	DfxAnyHistoryAction *sx, DfxAnyHistoryAction *sy, DfxAnyHistoryAction *sz) {

	_posx = px;
	_posy = py;
	_posz = pz;

	_rotx = rx;
	_roty = ry;
	_rotz = rz;

	_sclx = sx;
	_scly = sy;
	_sclz = sz;

}

DfxTransformHistoryAction::~DfxTransformHistoryAction() {

	delete _posx;
	delete _posy;
	delete _posz;

	delete _rotx;
	delete _roty;
	delete _rotz;

	delete _sclx;
	delete _scly;
	delete _sclz;

}

bool DfxTransformHistoryAction::execute() {

	if (_posx->execute() &&
		_posy->execute() &&
		_posz->execute() &&

		_rotx->execute() &&
		_roty->execute() &&
		_rotz->execute() &&

		_sclx->execute() &&
		_scly->execute() &&
		_sclz->execute()) return true;

	return false;
}

DfxHistoryAction *DfxTransformHistoryAction::inverse() {
	return new DfxTransformHistoryAction(
		(DfxAnyHistoryAction*)_posx->inverse(), (DfxAnyHistoryAction*)_posy->inverse(), (DfxAnyHistoryAction*)_posz->inverse(),
		(DfxAnyHistoryAction*)_rotx->inverse(), (DfxAnyHistoryAction*)_roty->inverse(), (DfxAnyHistoryAction*)_rotz->inverse(),
		(DfxAnyHistoryAction*)_sclx->inverse(), (DfxAnyHistoryAction*)_scly->inverse(), (DfxAnyHistoryAction*)_sclz->inverse());
}

void DfxTransformHistoryAction::setReference(std::string ref) {
	_transformReference = ref;
}

void DfxTransformHistoryAction::setOldTransform(fl::Any *transform) {

	oldPos = glm::vec3(
		transform->data<fl::DataObject>().property("x_pos")->data<float>(),
		transform->data<fl::DataObject>().property("y_pos")->data<float>(),
		transform->data<fl::DataObject>().property("z_pos")->data<float>());

	oldScl = glm::vec3(
		transform->data<fl::DataObject>().property("x_scl")->data<float>(),
		transform->data<fl::DataObject>().property("y_scl")->data<float>(),
		transform->data<fl::DataObject>().property("z_scl")->data<float>());

	oldRot = glm::vec3(
		transform->data<fl::DataObject>().property("x_rot")->data<float>(),
		transform->data<fl::DataObject>().property("y_rot")->data<float>(),
		transform->data<fl::DataObject>().property("z_rot")->data<float>());

}

void DfxTransformHistoryAction::setNewTransform(fl::Any *transform) {

	newPos = glm::vec3(
		transform->data<fl::DataObject>().property("x_pos")->data<float>(),
		transform->data<fl::DataObject>().property("y_pos")->data<float>(),
		transform->data<fl::DataObject>().property("z_pos")->data<float>());

	newScl = glm::vec3(
		transform->data<fl::DataObject>().property("x_scl")->data<float>(),
		transform->data<fl::DataObject>().property("y_scl")->data<float>(),
		transform->data<fl::DataObject>().property("z_scl")->data<float>());

	newRot = glm::vec3(
		transform->data<fl::DataObject>().property("x_rot")->data<float>(),
		transform->data<fl::DataObject>().property("y_rot")->data<float>(),
		transform->data<fl::DataObject>().property("z_rot")->data<float>());

}

void DfxTransformHistoryAction::buildHistoryActions() {

	// Position
	_posx = new DfxAnyHistoryAction;
	_posx->setValue(fl::type::FLOAT, _transformReference + "/x_pos", newPos.x, oldPos.x);

	_posy = new DfxAnyHistoryAction;
	_posy->setValue(fl::type::FLOAT, _transformReference + "/y_pos", newPos.y, oldPos.y);

	_posz = new DfxAnyHistoryAction;
	_posz->setValue(fl::type::FLOAT, _transformReference + "/z_pos", newPos.z, oldPos.z);

	// Scale
	_sclx = new DfxAnyHistoryAction;
	_sclx->setValue(fl::type::FLOAT, _transformReference + "/x_scl", newScl.x, oldScl.x);

	_scly = new DfxAnyHistoryAction;
	_scly->setValue(fl::type::FLOAT, _transformReference + "/y_scl", newScl.y, oldScl.y);

	_sclz = new DfxAnyHistoryAction;
	_sclz->setValue(fl::type::FLOAT, _transformReference + "/z_scl", newScl.z, oldScl.z);

	// Rot
	_rotx = new DfxAnyHistoryAction;
	_rotx->setValue(fl::type::FLOAT, _transformReference + "/x_rot", newRot.x, oldRot.x);

	_roty = new DfxAnyHistoryAction;
	_roty->setValue(fl::type::FLOAT, _transformReference + "/y_rot", newRot.y, oldRot.y);

	_rotz = new DfxAnyHistoryAction;
	_rotz->setValue(fl::type::FLOAT, _transformReference + "/z_rot", newRot.z, oldRot.z);
}
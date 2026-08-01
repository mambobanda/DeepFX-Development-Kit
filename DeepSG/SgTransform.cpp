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

#include "SgTransform.h"

namespace deepsg {

	SgTransform::SgTransform() {
		_properties = nullptr;

		defualtPos = glm::vec3(0.f);
		defualtRot = glm::vec3(0.f);
		defualtScl = glm::vec3(1.f);

		defaultRotationType = 0;
	}

	SgTransform::~SgTransform() {

	}

	float &SgTransform::posX() {
		if (_properties == nullptr) return defualtPos.x;
		else return _properties->data<fl::DataObject>().property("x_pos")->data<float>();
	}

	float &SgTransform::posY() {
		if (_properties == nullptr) return defualtPos.y;
		else return _properties->data<fl::DataObject>().property("y_pos")->data<float>();
	}

	float &SgTransform::posZ() {
		if (_properties == nullptr) return defualtPos.z;
		else return _properties->data<fl::DataObject>().property("z_pos")->data<float>();
	}

	float &SgTransform::sclX() {
		if (_properties == nullptr) return defualtScl.x;
		else return _properties->data<fl::DataObject>().property("x_scl")->data<float>();
	}

	float &SgTransform::sclY() {
		if (_properties == nullptr) return defualtScl.y;
		else return _properties->data<fl::DataObject>().property("y_scl")->data<float>();
	}

	float &SgTransform::sclZ() {
		if (_properties == nullptr) return defualtScl.z;
		else return _properties->data<fl::DataObject>().property("z_scl")->data<float>();
	}

	float &SgTransform::rotX() {
		if (_properties == nullptr) return defualtRot.x;
		else return _properties->data<fl::DataObject>().property("x_rot")->data<float>();
	}

	float &SgTransform::rotY() {
		if (_properties == nullptr) return defualtRot.y;
		else return _properties->data<fl::DataObject>().property("y_rot")->data<float>();
	}

	float &SgTransform::rotZ() {
		if (_properties == nullptr) return defualtRot.z;
		else return _properties->data<fl::DataObject>().property("z_rot")->data<float>();
	}

	void SgTransform::notifyObservers() {
		if (_properties != nullptr) {
			_properties->data<fl::DataObject>().property("x_pos")->notifyObservers();
			_properties->data<fl::DataObject>().property("x_pos")->invokeDispatchers();

			_properties->data<fl::DataObject>().property("y_pos")->notifyObservers();
			_properties->data<fl::DataObject>().property("y_pos")->invokeDispatchers();

			_properties->data<fl::DataObject>().property("z_pos")->notifyObservers();
			_properties->data<fl::DataObject>().property("z_pos")->invokeDispatchers();

			_properties->data<fl::DataObject>().property("x_scl")->notifyObservers();
			_properties->data<fl::DataObject>().property("x_scl")->invokeDispatchers();

			_properties->data<fl::DataObject>().property("y_scl")->notifyObservers();
			_properties->data<fl::DataObject>().property("y_scl")->invokeDispatchers();

			_properties->data<fl::DataObject>().property("z_scl")->notifyObservers();
			_properties->data<fl::DataObject>().property("z_scl")->invokeDispatchers();

			_properties->data<fl::DataObject>().property("x_rot")->notifyObservers();
			_properties->data<fl::DataObject>().property("x_rot")->invokeDispatchers();

			_properties->data<fl::DataObject>().property("y_rot")->notifyObservers();
			_properties->data<fl::DataObject>().property("y_rot")->invokeDispatchers();

			_properties->data<fl::DataObject>().property("z_rot")->notifyObservers();
			_properties->data<fl::DataObject>().property("z_rot")->invokeDispatchers();

		}
	}

	glm::vec3 SgTransform::position() {
		return glm::vec3(posX(), posY(), posZ());
	}

	glm::vec3 SgTransform::rotation() {
		return glm::vec3(rotX(), rotY(), rotZ());
	}

	glm::vec3 SgTransform::scale() {
		return glm::vec3(sclX(), sclY(), sclZ());
	}

	void SgTransform::setPosition(glm::vec3 pos) {
		posX() = pos.x;
		posY() = pos.y;
		posZ() = pos.z;

		if (_properties != nullptr) {
			_properties->data<fl::DataObject>().property("x_pos")->notifyObservers();
			_properties->data<fl::DataObject>().property("y_pos")->notifyObservers();
			_properties->data<fl::DataObject>().property("z_pos")->notifyObservers();
		}
	}

	void SgTransform::setRotation(glm::vec3 rot) {
		rotX() = rot.x;
		rotY() = rot.y;
		rotZ() = rot.z;

		if (_properties != nullptr) {
			_properties->data<fl::DataObject>().property("x_rot")->notifyObservers();
			_properties->data<fl::DataObject>().property("y_rot")->notifyObservers();
			_properties->data<fl::DataObject>().property("z_rot")->notifyObservers();
		}
	}

	void SgTransform::setScale(glm::vec3 scl) {
		sclX() = scl.x;
		sclY() = scl.y;
		sclZ() = scl.z;

		if (_properties != nullptr) {
			_properties->data<fl::DataObject>().property("x_scl")->notifyObservers();
			_properties->data<fl::DataObject>().property("y_scl")->notifyObservers();
			_properties->data<fl::DataObject>().property("z_scl")->notifyObservers();
		}
	}

	unsigned int &SgTransform::rotationType() {
		if (_properties == nullptr) return defaultRotationType;
		return _properties->data<fl::DataObject>().property("rotation_type")->data<unsigned int>();
	}


	glm::mat4 SgTransform::rotationMatrix() {
		unsigned int _rotationType = 0;
		
		if (_properties == nullptr) _rotationType = defaultRotationType;
		else _rotationType = _properties->data<fl::DataObject>().property("rotation_type")->data<unsigned int>();

		return rotationMatrix(rotation(), _rotationType);
		
	}

	glm::mat4 SgTransform::rotationMatrix(glm::vec3 rotation, unsigned int _rotationType) {

		switch (_rotationType) {
		case XYZ:
		{
			auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, .0f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0.f, 1.f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0.f, .0f, 1.f));

			return transMat;
		}
		break;
		case XZY:
		{
			auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, .0f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0.f, .0f, 1.f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0.f, 1.f, .0f));


			return transMat;
		}
		break;
		case YXZ:
		{
			auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0.f, 1.0f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, 0.f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0.f, .0f, 1.f));

			return transMat;
		}
		break;
		case YZX:
		{
			auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0.f, 1.0f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, .0f, 0.f));

			return transMat;
		}
		break;
		case ZXY:
		{
			auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0.f, .0f, 1.0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, 0.f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0.f, 1.0f, 0.f));

			return transMat;
		}
		break;
		case ZYX:
		{
			auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0.f, .0f, 1.0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0.f, 1.f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, .0f, 0.f));

			return transMat;
		}
		break;
		default:
			return glm::mat4(1.f);
			break;
		}
	}

	glm::mat4 SgTransform::transform4x4() {

		glm::mat4 transMat = glm::translate(glm::mat4(1.f), glm::vec3(position().x, position().y, position().z));

		transMat *= rotationMatrix();

		transMat *= glm::scale(glm::mat4(1.f), glm::vec3(scale().x, scale().y, scale().z));

		return transMat;
	}

	fl::Any *SgTransform::xFormProperties() {
		return _properties;
	}

	void SgTransform::setXFormProperties(fl::Any *prop) {
		_properties = prop;
	}
};
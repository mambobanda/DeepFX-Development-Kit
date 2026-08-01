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

#include "SgXForm.h"

namespace deepsg {

	SgXForm::SgXForm() {
		_position = glm::vec3(0.f);
		_rotation = glm::vec3(0.f);;
		_scale = glm::vec3(1.f);;

		_rotationType = XYZ;
	}

	SgXForm::~SgXForm() {

	}

	glm::vec3 &SgXForm::position() {
		return _position;
	}

	glm::vec3 &SgXForm::rotation() {
		return _rotation;
	}

	glm::vec3 &SgXForm::scale() {
		return _scale;
	}

	unsigned int &SgXForm::rotationType() {
		return _rotationType;
	}

	glm::mat4 SgXForm::rotationMatrix() {
		switch (_rotationType) {
		case XYZ:
		{
			auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(_rotation.x), glm::vec3(1.f, .0f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(_rotation.y), glm::vec3(0.f, 1.f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(_rotation.z), glm::vec3(0.f, .0f, 1.f));

			return transMat;
		}
		break;
		case XZY:
		{
			auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(_rotation.x), glm::vec3(1.f, .0f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(_rotation.z), glm::vec3(0.f, .0f, 1.f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(_rotation.y), glm::vec3(0.f, 1.f, .0f));


			return transMat;
		}
		break;
		case YXZ:
		{
			auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(_rotation.y), glm::vec3(0.f, 1.0f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(_rotation.x), glm::vec3(1.f, 0.f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(_rotation.z), glm::vec3(0.f, .0f, 1.f));

			return transMat;
		}
		break;
		case YZX:
		{
			auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(_rotation.y), glm::vec3(0.f, 1.0f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(_rotation.z), glm::vec3(0.f, 0.f, 1.0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(_rotation.x), glm::vec3(1.f, .0f, 0.f));

			return transMat;
		}
		break;
		case ZXY:
		{
			auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(_rotation.z), glm::vec3(0.f, .0f, 1.0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(_rotation.x), glm::vec3(1.f, 0.f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(_rotation.y), glm::vec3(0.f, 1.0f, 0.f));

			return transMat;
		}
		break;
		case ZYX:
		{
			auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(_rotation.z), glm::vec3(0.f, .0f, 1.0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(_rotation.y), glm::vec3(0.f, 1.f, .0f));
			transMat *= glm::rotate(glm::mat4(1.f), glm::radians(_rotation.x), glm::vec3(1.f, .0f, 0.f));

			return transMat;
		}
		break;
		default:
			return glm::mat4(1.f);
			break;
		}

	}

	glm::mat4 SgXForm::transform4x4() {

		glm::mat4 transMat = glm::translate(glm::mat4(1.f), glm::vec3(_position.x, _position.y, _position.z));

		transMat *= rotationMatrix();

		transMat *= glm::scale(glm::mat4(1.f), glm::vec3(_scale.x, _scale.y, _scale.z));

		return transMat;
	}

};
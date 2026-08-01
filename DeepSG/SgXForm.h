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

#pragma once

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\geometric.hpp>
#include <glm\gtx\transform.hpp>
#include "SgRotationType.h"

namespace deepsg {

	class SgXForm {
	private:
		glm::vec3 _position;
		glm::vec3 _rotation;
		glm::vec3 _scale;

		unsigned int _rotationType;

	private:
		glm::mat4 rotationMatrix();

	public:

		SgXForm();
		virtual ~SgXForm();

		glm::vec3 &position();
		glm::vec3 &rotation();
		glm::vec3 &scale();

		unsigned int &rotationType();
		glm::mat4 transform4x4();
	};

}
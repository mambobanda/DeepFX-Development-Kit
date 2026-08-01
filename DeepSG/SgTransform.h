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
#include <Fluent.h>
#include "SgRotationType.h"

namespace deepsg {

	class SgTransform {
	private:
		fl::Any *_properties;

	private:
		glm::vec3 defualtPos;
		glm::vec3 defualtRot;
		glm::vec3 defualtScl;

		unsigned int defaultRotationType;
	public:

		SgTransform();
		virtual ~SgTransform();

		float &posX();
		float &posY();
		float &posZ();

		float &sclX();
		float &sclY();
		float &sclZ();

		float &rotX();
		float &rotY();
		float &rotZ();

		void notifyObservers();

		void setPosition(glm::vec3 pos);
		glm::vec3 position();

		void setRotation(glm::vec3 rot);
		glm::vec3 rotation();

		void setScale(glm::vec3 scl);
		glm::vec3 scale();

		unsigned int &rotationType();

		glm::mat4 transform4x4();
		glm::mat4 rotationMatrix();
		static glm::mat4 rotationMatrix(glm::vec3 rot, unsigned int type);

		fl::Any *xFormProperties();
		void setXFormProperties(fl::Any *prop);
	};

}
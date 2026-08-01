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

#include <glm/geometric.hpp>
#include <algorithm>
#include "DfxCCollisionResponseInterface.h"
#include "Util.h"

class DfxCDefaultCollisionResponse: public DfxCCollisionResponseInterface {
public:
	DfxCDefaultCollisionResponse();
	~DfxCDefaultCollisionResponse();

	virtual void response(float restitutionCoefficient, float _frictionCoeffient, float radius, glm::vec3 penetrationPoint, glm::vec3 normal, glm::vec3 velocity, glm::vec3 &newPosition, glm::vec3 &newVelocity) override;
};
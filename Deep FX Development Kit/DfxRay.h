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

class DfxRay {
private:
	bool null;
public:
	glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 start;
	glm::vec3 end;

	DfxRay();

	bool isNull();
	void setNull(bool);
};
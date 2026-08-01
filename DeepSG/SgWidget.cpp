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

#include "SgWidget.h"

namespace deepsg {

	SgWidget::SgWidget() {

	}

	SgWidget::~SgWidget() {

	}

	float SgWidget::getScale() {
		return scale;
	}

	void SgWidget::setScale(float s) {
		scale = s;
	}

	int SgWidget::handleCount() {
		return (int)_handles.size();
	}

	std::vector<SgShapeBuffer> SgWidget::handles() {
		return _handles;
	}

	glm::mat4 SgWidget::handleTransform(int handle, deepsg::SgXForm parentTransform) {
		return glm::mat4(1);
	}
}
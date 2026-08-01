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


#include "SgXForm.h"
#include "SgTransform.h"
#include "SgShapeBuffer.h"

namespace deepsg {

	class SgWidget {
	private:
		float scale;
	protected:
		std::vector<SgShapeBuffer> _handles;
	public:

		SgWidget();
		virtual ~SgWidget();

		float getScale();
		void setScale(float);

		int handleCount();
		std::vector<SgShapeBuffer> handles();
		virtual glm::mat4 handleTransform(int handle, deepsg::SgXForm parentTransform);
	};
}
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

#include <QImage>
#include <SgNode.h>
#include "DfxRenderTarget.h"

class DfxRenderSessionInterface {
public:

	virtual bool init() = 0;
	virtual bool prepareSession() = 0;
	virtual void startRenderSession() = 0;
	virtual void stopRenderSession() = 0;
	virtual bool clear() = 0;
	virtual bool loadScene(deepsg::SgNode *branch) = 0;
	virtual bool unloadScene() = 0;
	virtual bool updateTransformations() = 0;
	virtual bool updateGeometry() = 0;
	virtual bool updateGeometryTransform()  = 0;
	virtual bool updateLights() = 0;
	virtual bool updateCamera() = 0;
	virtual bool updateMaterials() = 0;
	virtual bool render() = 0;
	virtual QImage output() = 0;
	virtual bool updateOutput(bool applyDenoising) = 0;
	virtual void setCameraPath(std::string) = 0;
	virtual void enableInteractivePreview() = 0;

	// Testing stuff
	virtual void testMaterials() = 0;
};
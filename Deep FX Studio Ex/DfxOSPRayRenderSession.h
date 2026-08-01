/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <QThread>
#include <QTimer>
#include <QApplication>
#include <DfxGlobal.h>
#include <SgRootNode.h>
#include <luxcore/luxcore.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <set>
#include <glm/gtc/type_ptr.hpp>
#include <QtConcurrent/qtconcurrentrun.h>
#include <mutex>
#include <QPainter>
#include "DfxOSPRayCommon.h"
#include "DfxItemWidgetType.h"
#include "DfxSpace.h"
#include "DfxOSPRayCameraHandler.h"
#include "DfxObjectHandler.h"
#include "DfxGridHandler.h"
#include "DfxOSPRayMaterialBuilder.h"
//#include "DfxOSPRaySunSkyLightHandler.h"
#include "DfxRenderSessionInterface.h"
#include "DfxRenderSessionLightObserver.h"
#include "DfxRenderSessionCameraObserver.h"
#include "DfxUtil.h"
#include "DfxMaterialLinker.h"
#include "DfxRenderCommon.h"
#include "DfxInstanceManager.h"
#include "DfxQRenderInstanceInfo.h"
#include "DfxRenderSessionUpdateListener.h"
#include "DfxLuxObjectSegment.h"
#include "DeepFxStudioLicenseManager.h"

class DfxOSPRayRenderSession : public DfxRenderSessionInterface {
private:
	bool _initialized;

	std::recursive_mutex _updateSessionMutex;
	std::recursive_mutex _updateOutputMutex;
	DfxRenderSessionUpdateListener *_sessionUpdateListener;

private:

	DfxRenderOutput *_renderOutput;

	DfxOSPRayMaterialBuilder matBuilder;

	deepsg::SgNode *activeBranch;
	void addLightObserver(fl::Any *);
	void addCameraObserver(fl::Any *);

	std::vector<fl::Observer*> observerList;
	std::vector<fl::Observer*> cameraObservers;

	int _refreshInterval;
	int _haltTime;
	float _haltVariance;
	int _rtHaltTime;
	float _rtHaltVariance;

	QTimer *rtHaltTimer;
	QTimer *rtBufferRefreshTimer;

	int _renderEngine;
	std::string cameraPath;
	std::string _ospRayRenderInfoPath;

	bool _denoisingEnabled;
	int  _denoisingBuffer;

	std::unique_ptr<ospray::cpp::World> world;
	std::unique_ptr<ospray::cpp::Renderer> renderer;
	std::unique_ptr<ospray::cpp::Camera> camera;
	std::unique_ptr<ospray::cpp::Material> defaultMat;
	std::unique_ptr<ospray::cpp::FrameBuffer> framebuffer;

	int outputWidth;
	int outputHeight;

	std::vector<OSPRayObject*> _ospObjects;
	std::vector<OSPRayVolume*> _ospVolumes;
	std::vector<OSPRayLight*> _ospLights;

private:

	bool isInitialized();
	bool loadMaterials(deepsg::SgNode *branch);
	bool loadObjects(deepsg::SgNode *branch);
	bool loadVolumes(deepsg::SgNode *branch);
	bool loadInstances();
	bool loadFluentSimplicies();
	bool loadFluentSimplexCaches();
	bool loadLights(deepsg::SgNode *branch);
	bool loadCamera(deepsg::SgNode *branch);
	bool loadWorld();

	bool checkSceneIsReady(std::string tag);
	bool checkSessionIsReady(std::string tag);

	glm::mat4 rootTransform();

	void _traverseScene00(deepsg::SgNode *node, deepsg::SgTraversalFunction travFunc, deepsg::SgStateInfo *info, unsigned int layer = 0);
	void traverseSubScene(deepsg::SgNode *node, deepsg::SgTraversalFunction travFunc, glm::mat4 root_transform = glm::mat4(1.f));

	glm::mat4 transform4x4(deepsg::SgNode *node);
	glm::mat4 rotationMatrix(deepsg::SgNode *node);

	void newHaltTimer(int time);
	void newBufferRefreshTimer(int time);

	void printRenderingStats(luxrays::Properties &stats);

	void applyWaterMark(QImage &image);

private:
	
	bool _bufferUpdating;
	QFuture<void> updateBufferFuture;

public:
	enum { PATH = 0, BIDIR, RT };

	DfxOSPRayRenderSession();
	~DfxOSPRayRenderSession();

	void startRenderSession() override;
	void stopRenderSession();
	bool isStarted();

public:

	void setRenderOutput(DfxRenderOutput *output);
	DfxRenderOutput *renderOutput();

	std::string osprayRenderInfoPath();
	void setOsprayRenderInfoPath(std::string path);

	bool _forceStop;
	void forceStop();

public:

	bool init() override;
	bool prepareSession() override;
	bool clear() override;
	bool loadScene(deepsg::SgNode *branch) override;
	bool unloadScene() override;
	bool updateTransformations() override;
	bool updateGeometry() override;
	bool updateGeometryTransform() override;
	bool updateLights() override;
	bool updateCamera() override;
	bool updateMaterials() override;
	bool render() override;
	bool renderExport(std::string dir);
	QImage output() override;
	bool updateOutput(bool applyDenoising = false) override;
	void setCameraPath(std::string) override;
	void enableInteractivePreview() override;
	void findInstancePaths();

	void testMaterials() override;

	void _updateOutput();
};
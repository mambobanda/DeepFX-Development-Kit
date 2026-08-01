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
#include "DfxLuxCoreMaterialBuilder.h"
#include "DfxLxCameraHandler.h"
#include "DfxItemWidgetType.h"
#include "DfxSpace.h"
#include "DfxLxCameraHandler.h"
#include "DfxObjectHandler.h"
#include "DfxLxPointLightHandler.h"
#include "DfxLxEnvironmentLightHandler.h"
#include "DfxLxSunSkyLightHandler.h"
#include "DfxRenderSessionInterface.h"
#include "DfxRenderSessionLightObserver.h"
#include "DfxRenderSessionCameraObserver.h"
#include "DfxLuxCoreSupport.h"
#include "DfxUtil.h"
#include "DfxMaterialLinker.h"
#include "DfxRenderCommon.h"
#include "DfxInstanceManager.h"
#include "DfxQRenderInstanceInfo.h"
#include "DfxRenderSessionUpdateListener.h"
#include "DfxLuxObjectSegment.h"
#include "DeepFxStudioLicenseManager.h"
#include "DfxGridHandler.h"

class DfxLuxCoreRenderSession : public DfxRenderSessionInterface {
private:
	static bool _initialized;

	std::recursive_mutex _updateSessionMutex;
	std::recursive_mutex _updateOutputMutex;
	DfxRenderSessionUpdateListener *_sessionUpdateListener;

private:

	luxcore::Scene *scene;
	luxcore::RenderConfig *render_config;
	luxcore::RenderSession *render_session;

	LuxItemID id_gen;

	std::unordered_map<std::string, std::vector<DfxLuxObjectSegment>> objectSegments;
	std::unordered_map<std::string, glm::mat4> objectTransform;
	std::unordered_map<std::string, std::vector<DfxLuxObjectSegment>> _instanceShapes;

	//std::unordered_map<std::string, std::string> shapeNameToReference;
	//std::unordered_map<std::string, deepsg::SgNode*> shapeNameToNode;
	/*std::unordered_map<std::string, glm::mat4> shapeNameToTransform;
	std::unordered_map<std::string, std::string> luxNameToReference;
	std::unordered_map<std::string, std::string> referenceToLuxName;*/

	std::unordered_map<std::string, std::string> existingLights;

	DfxRenderOutput *_renderOutput;

	deepsg::SgNode *activeBranch;
	void addLightObserver(fl::Any *);
	void addCameraObserver(fl::Any *);

	std::vector<fl::Observer*> observerList;
	std::vector<fl::Observer*> cameraObservers;

	DfxLuxCoreMaterialBuilder matBuilder;
	DfxLuxMaterialChangeDispatch *_materialDispatcher;

	std::string _luxcoreRenderInfoPath;

	int _refreshInterval;
	int _haltTime;
	int _haltSpp;
	int rtHaltTime;
	QTimer *rtHaltTimer;
	QTimer *rtBufferRefreshTimer;

	int _renderEngine;
	std::string cameraPath;

	bool _denoisingEnabled;
	int  _denoisingBuffer;
private:

	bool loadMaterials(deepsg::SgNode *branch);
	bool loadObjects(deepsg::SgNode *branch);
	bool loadInstances();
	bool loadFluentSimplicies();
	bool loadFluentSimplexCaches();
	bool loadLights(deepsg::SgNode *branch);
	bool loadCamera(deepsg::SgNode *branch);
	bool loadVolumes(deepsg::SgNode *branch);
	
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

	void Normalize3(const float *filmPixels, float *pixels, const unsigned int filmWidth, const unsigned int filmHeight) const;
	void AutoLinearToneMap(const float *src, float *dst, const unsigned int filmWidth, const unsigned int filmHeight) const;
	void Normalize1(const float *filmPixels, float *pixels, const unsigned int filmWidth, const unsigned int filmHeight) const;
	void Copy1(const float *filmPixels, float *pixels, const unsigned int filmWidth, const unsigned int filmHeight) const;
	void Copy3(const float *filmPixels, float *pixels, const unsigned int filmWidth, const unsigned int filmHeight) const;

private:
	luxcore::RenderConfig *pathRenderConfig(bool &usingOpencl, bool realtime = false);
	luxcore::RenderConfig *bidirRenderConfig();

	bool setupRenderConfigBuffer(luxrays::Properties &properties);
	std::pair<int, bool> setupImagePipline(luxrays::Properties &properties, fl::DataObject *pipelineObj, fl::DataObject *denoiseObj);
	void setupAovPipeline(int index, luxrays::Properties &properties, std::string channel);
	bool _bufferUpdating;
	QFuture<void> updateBufferFuture;
public:
	enum { PATH = 0, BIDIR, RT };

	DfxLuxCoreRenderSession();
	~DfxLuxCoreRenderSession();

	void startRenderSession() override;
	void stopRenderSession();
	bool isStarted();
	
	void setRenderEngine(int);
	void setRtHaltTime(int);

	float CalcY(const float *c);
	float estimatePathClampingVariance();

	void setRenderOutput(DfxRenderOutput *output);
	DfxRenderOutput *renderOutput();
	
	std::string luxcoreRenderInfoPath();
	void setLuxcoreRenderInfoPath(std::string path);

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
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

#include <QMessageBox>
#include <ospray/ospray_util.h>
#include <ospray/ospray_cpp.h>
#include <ospray/ospray_cpp/ext/rkcommon.h>
#include <rkcommon/math/AffineSpace.h>
//#define OSPRAY_GLM_DEFINITIONS

#include "DfxOSPRayRenderSession.h"
#include "DfxOSPRayCameraHandler.h"
#include "DfxOSPRayRenderHandler.h"
#include "DfxOSPRaySunSkyLightHandler.h"
#include "DfxLxRenderHandler.h"
#include "DfxRenderOutput.h"
#include "DfxMaterialManager.h"


using namespace luxrays;

DfxOSPRayRenderSession::DfxOSPRayRenderSession() {

	_initialized = false;

	_renderOutput = nullptr;

	_renderEngine = PATH;
	activeBranch = nullptr;

	cameraPath = "";

	_refreshInterval = 10;
	_haltTime = 120;
	_haltVariance = 0.1f;
	_rtHaltTime = 10;
	_rtHaltVariance = 1.f;

	_denoisingEnabled = false;
	_denoisingBuffer = 0;

	_bufferUpdating = false;

	world = nullptr;
	defaultMat = nullptr;
	renderer = nullptr;
	framebuffer = nullptr;
	camera = nullptr;
		 
	outputWidth = -1;
	outputHeight = -1;

	_sessionUpdateListener = nullptr;

}

DfxOSPRayRenderSession::~DfxOSPRayRenderSession() {

	
}

void DfxOSPRayRenderSession::enableInteractivePreview() {
	//_renderEngine = DfxLuxCoreRenderSession::RT;
}

bool DfxOSPRayRenderSession::isInitialized() {
	return _initialized;
}

void ospStatCallback(void *userData, const char* messageText) {
	std::cout << "OSPRay Status: " + std::string(messageText) << std::endl;
	DfxGlobal::console::println("OSPRay Status: " + std::string(messageText));
}

void ospErrorCallback(void *userData, OSPError err, const char* errorDetails) {
	std::cout << "OSPRay ERROR (" + std::to_string(err) + " ): " + std::string(errorDetails) << std::endl;
	DfxGlobal::console::println("OSPRay ERROR (" + std::to_string(err) + " ): " + std::string(errorDetails));
}

bool DfxOSPRayRenderSession::init() {

	auto device = ospGetCurrentDevice();

	if (device) {

		//int logLevel = OSP_LOG_DEBUG;
		bool logWarning = true;
		//ospDeviceSetParam(device, "logLevel", OSP_INT, &logLevel);
		//ospDeviceSetParam(device, "warnAsError",OSP_BOOL, &logWarning);

		ospDeviceSetStatusCallback(device, ospStatCallback, nullptr);
		ospDeviceSetErrorCallback(device, ospErrorCallback, nullptr);

		world.reset(new ospray::cpp::World);

		_initialized = true;
		return true;
	}
	else {
		DfxGlobal::console::println("Failed to Init OSPRay Render Session: No Device Found.");
	}
	
	return false;
}

void DfxOSPRayRenderSession::setRenderOutput(DfxRenderOutput *output) {
	_renderOutput = output;
}

DfxRenderOutput *DfxOSPRayRenderSession::renderOutput() {
	return _renderOutput;
}

std::string DfxOSPRayRenderSession::osprayRenderInfoPath() {
	return _ospRayRenderInfoPath;
}

void DfxOSPRayRenderSession::setOsprayRenderInfoPath(std::string path) {
	_ospRayRenderInfoPath = path;
}

glm::mat4 DfxOSPRayRenderSession::rootTransform() {
	return glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
}

glm::mat4 DfxOSPRayRenderSession::transform4x4(deepsg::SgNode *node) {

	glm::mat4 transMat = glm::translate(glm::mat4(1.f), glm::vec3(node->posX(), -node->posZ(), node->posY()));

	transMat *= rotationMatrix(node);

	transMat *= glm::scale(glm::mat4(1.f), glm::vec3(node->sclX(), node->sclZ(), node->sclY()));

	return transMat;
}

glm::mat4 DfxOSPRayRenderSession::rotationMatrix(deepsg::SgNode *node) {

	unsigned int _rotationType = node->rotationType();

	glm::vec3 rotation(node->rotX(), node->rotY(), node->rotZ());

	switch (_rotationType) {
	case deepsg::XYZ:
	{
		auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, .0f, .0f));
		transMat *= glm::rotate(glm::mat4(1.f), glm::radians(-rotation.z), glm::vec3(0.f, 1.f, .0f));
		transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0.f, .0f, 1.f));

		return transMat;
	}
	break;
	case deepsg::XZY:
	{
		auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, .0f, .0f));
		transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0.f, .0f, 1.f));
		transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0.f, 1.f, .0f));


		return transMat;
	}
	break;
	case deepsg::YXZ:
	{
		auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0.f, 1.0f, .0f));
		transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, 0.f, .0f));
		transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0.f, .0f, 1.f));

		return transMat;
	}
	break;
	case deepsg::YZX:
	{
		auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0.f, 1.0f, .0f));
		transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0.f, 0.f, 1.0f));
		transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, .0f, 0.f));

		return transMat;
	}
	break;
	case deepsg::ZXY:
	{
		auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0.f, .0f, 1.0f));
		transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, 0.f, .0f));
		transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0.f, 1.0f, 0.f));

		return transMat;
	}
	break;
	case deepsg::ZYX:
	{
		auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(rotation.y), glm::vec3(0.f, .0f, 1.0f));
		transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.z), glm::vec3(0.f, 1.f, .0f));
		transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, .0f, 0.f));

		return transMat;
	}
	break;
	default:
		return glm::mat4(1.f);
		break;
	}

}

void DfxOSPRayRenderSession::_traverseScene00(deepsg::SgNode *node, deepsg::SgTraversalFunction travFunc, deepsg::SgStateInfo *info, unsigned int layer) {

	if (!node->isIndependent()) {
		if (layer != 0)
			if (node->layer() != layer) return;
	}
	
	deepsg::SgRootNode::StateInfo *_info = new deepsg::SgRootNode::StateInfo;
	_info->setParent(info->parent());

	auto mat = transform4x4(node);
	_info->setMatrix(info->transformMatrix() * mat);

	travFunc(node, _info);

	_info->setParent(node);
	for (auto child : node->children()) {
		_traverseScene00(child, travFunc, _info);
	}

	delete _info;
}

void DfxOSPRayRenderSession::traverseSubScene(deepsg::SgNode *node, deepsg::SgTraversalFunction travFunc, glm::mat4 root_transform) {

	deepsg::SgRootNode::StateInfo *_info = new deepsg::SgRootNode::StateInfo;
	_info->setParent(node);
	_info->setMatrix(root_transform);

	_traverseScene00(node, travFunc, _info);
}

void DfxOSPRayRenderSession::addLightObserver(fl::Any *any) {
	DfxRenderSessionLightObserver *obs = new DfxRenderSessionLightObserver;
	obs->_interface = this;

	observerList.push_back(obs);
	any->addObserver(obs);
}

void DfxOSPRayRenderSession::addCameraObserver(fl::Any *any) {
	DfxRenderSessionCameraObserver *obs = new DfxRenderSessionCameraObserver;
	obs->_interface = this;

	cameraObservers.push_back(obs);
	any->addObserver(obs);
}

void DfxOSPRayRenderSession::newHaltTimer(int time) {
	/*if (!checkSessionIsReady("LuxCore Halt Timer")) return;

	if (rtHaltTimer != nullptr) {
		rtHaltTimer->stop();
		delete rtHaltTimer;
	}

	rtHaltTimer = new QTimer;
	rtHaltTimer->setSingleShot(true);

	QObject::connect(rtHaltTimer, &QTimer::timeout, [this]() {
		if (render_session->IsStarted()) {
			render_session->Pause();

			if (rtBufferRefreshTimer != nullptr) {
				rtBufferRefreshTimer->stop();
				delete rtBufferRefreshTimer;
				rtBufferRefreshTimer = nullptr;
			}

			if (updateBufferFuture.isRunning()) {
				updateBufferFuture.cancel();
				updateBufferFuture.waitForFinished();
				_bufferUpdating = false;
			}

			if (_denoisingEnabled) {
				updateOutput(true);
			}
		}

	});

	if (time < 1000) {
		rtHaltTimer->start(1000);
	}
	else {
		rtHaltTimer->start(time);
	}*/
}

void DfxOSPRayRenderSession::newBufferRefreshTimer(int time) {
	/*if (!checkSessionIsReady("LuxCore Refresh Buffer Timer")) return;

	rtBufferRefreshTimer = new QTimer;
	auto session = this;
	QObject::connect(rtBufferRefreshTimer, &QTimer::timeout, [this, session]() {
		if (_bufferUpdating) return;
		else _bufferUpdating = true;

		updateBufferFuture = QtConcurrent::run(session, &DfxLuxCoreRenderSession::_updateOutput);
		while (updateBufferFuture.isRunning()) QApplication::processEvents();

		_bufferUpdating = false;
	});

	rtBufferRefreshTimer->start(100);*/
}

void DfxOSPRayRenderSession::printRenderingStats(luxrays::Properties &stats) {
	//char buf[512];

	//const double elapsedTime = stats.Get("stats.renderengine.time").Get<double>();
	//const unsigned int pass = stats.Get("stats.renderengine.pass").Get<unsigned int>();

	//// Print some information about the rendering progress
	//sprintf(buf, "[Elapsed time: %3d/%dsec][Samples %4d/%d][Avg. samples/sec % 3.2fM on %.1fK tris]",
	//	int(elapsedTime), int(_haltTime), pass, _haltSpp,
	//	stats.Get("stats.renderengine.total.samplesec").Get<double>() / 1000000.0,
	//	stats.Get("stats.dataset.trianglecount").Get<double>() / 1000.0);

	//LC_LOG(buf);
}

bool DfxOSPRayRenderSession::prepareSession() {
	auto space = DfxSpace::getInstance();

	std::string generalStr = _ospRayRenderInfoPath + "/ospray_general";
	std::string pathStr = _ospRayRenderInfoPath + "/ospray_path";
	std::string sciVisStr = _ospRayRenderInfoPath + "/ospray_scivis";
	std::string aoStr = _ospRayRenderInfoPath + "/ospray_ao";
	std::string toneMappingStr = _ospRayRenderInfoPath + "/ospray_tone_mapping";

	auto ospRenderAny = space->findAbsolute(_ospRayRenderInfoPath);
	auto ospGeneralAny = space->findAbsolute(generalStr);
	auto ospPathAny = space->findAbsolute(pathStr);
	auto ospSciVisAny = space->findAbsolute(sciVisStr);
	auto ospAOAny = space->findAbsolute(aoStr);
	auto toneMappingAny = space->findAbsolute(toneMappingStr);


	if (world == nullptr) {
		DfxGlobal::console::println("OSPRay ERROR: World not initialized.");
		return false;
	}

	if (camera == nullptr) {
		DfxGlobal::console::println("OSPRay ERROR: Camera not initialized.");
		return false;
	}

	if (outputWidth == -1 || outputHeight == -1) {
		DfxGlobal::console::println("OSPRay ERROR: Undefined image WIDTH or HEIGHT.");
		return false;
	}
	
	if (ospRenderAny != nullptr || ospGeneralAny != nullptr || ospPathAny != nullptr || ospSciVisAny != nullptr ||
		ospAOAny != nullptr) {

		auto daoRoot = ospRenderAny->rawData<fl::DataObject>();
		auto engine = daoRoot->property("engine")->data<unsigned int>();

		auto daoGeneral = ospGeneralAny->rawData<fl::DataObject>();
		auto pixel_filter = daoGeneral->property("pixel_filter")->data<int>();
		auto pixel_samples = daoGeneral->property("pixel_samples")->data<int>();
		auto max_path_length = daoGeneral->property("max_path_length")->data<int>();
		auto min_contribution = daoGeneral->property("min_contribution")->data<float>();
		auto variance_threshold = daoGeneral->property("variance_threshold")->data<float>();

		if (engine == 0) {

			// SciVis
			auto daoScivis = ospSciVisAny->rawData<fl::DataObject>();
			auto shadows = daoScivis->property("shadows")->data<bool>();
			auto aoSamples = daoScivis->property("aoSamples")->data<int>();
			auto ao_max_distance = daoScivis->property("ao_max_distance")->data<float>();
			auto volume_sampling_rate = daoScivis->property("volume_sampling_rate")->data<float>();
			auto visible_lights = daoScivis->property("visible_lights")->data<bool>();

			renderer.reset(new ospray::cpp::Renderer("scivis"));
			renderer->setParam("shadows", shadows);
			renderer->setParam("visibleLights", visible_lights);
			renderer->setParam("aoSamples", aoSamples);
			renderer->setParam("aoDistance", ao_max_distance);
			renderer->setParam("volumeSamplingRate", visible_lights);
		}
		else if (engine == 1) {

			// AO
			auto daoAO = ospAOAny->rawData<fl::DataObject>();
			auto aoSamples = daoAO->property("aoSamples")->data<int>();
			auto ao_max_distance = daoAO->property("ao_max_distance")->data<float>();
			auto aoIntensity = daoAO->property("aoIntensity")->data<float>();
			auto volume_sampling_rate = daoAO->property("volume_sampling_rate")->data<float>();

			renderer.reset(new ospray::cpp::Renderer("ao"));
			renderer->setParam("aoSamples", aoSamples);
			renderer->setParam("aoDistance", ao_max_distance);
			renderer->setParam("aoIntensity", aoIntensity);
			renderer->setParam("volumeSamplingRate", volume_sampling_rate);
		}
		else if (engine == 2) {

			// Path
			auto daoPath = ospPathAny->rawData<fl::DataObject>();
			auto roulette_path_length = daoPath->property("roulette_path_length")->data<int>();
			auto background_refraction = daoPath->property("background_refraction")->data<bool>();

			renderer.reset(new ospray::cpp::Renderer("pathtracer"));
			renderer->setParam("roulettePathLength", roulette_path_length);
			renderer->setParam("backgroundRefraction", background_refraction);
		}
		else {
			DfxGlobal::console::println("OSPRay Error: Unknown render engine.");
			return false;
		}

		char pxFilter = OSP_PIXELFILTER_GAUSS;
		if (pixel_filter == 0) {
			pxFilter = OSP_PIXELFILTER_POINT;
		}
		else if (pixel_filter == 1) {
			pxFilter = OSP_PIXELFILTER_BOX;
		}
		else if (pixel_filter == 2) {
			pxFilter = OSP_PIXELFILTER_GAUSS;
		}
		else if (pixel_filter == 3) {
			pxFilter = OSP_PIXELFILTER_MITCHELL;
		}
		else if (pixel_filter == 4) {
			pxFilter = OSP_PIXELFILTER_BLACKMAN_HARRIS;
		}

		renderer->setParam("pixelFilter", OSP_CHAR, &pxFilter);
		renderer->setParam("pixelSamples", pixel_samples);
		renderer->setParam("maxPathLength", max_path_length);
		renderer->setParam("minContribution", min_contribution);
		renderer->setParam("varianceThreshold", variance_threshold);

		renderer->setParam("backgroundColor", rkcommon::math::vec3f(1.0f, 1.f, 1.f));
		renderer->commit();

		framebuffer.reset(new ospray::cpp::FrameBuffer(
			outputWidth,
			outputHeight,
			OSP_FB_RGBA32F,
			OSP_FB_COLOR | OSP_FB_VARIANCE | OSP_FB_ACCUM));
		framebuffer->clear();

		auto renderBuffer = new DfxRenderBuffer;
		renderBuffer->setBufferName("sRGBA");

		_renderOutput->clear();
		QApplication::processEvents();
		_renderOutput->notifyBuffersUpdated();

		_renderOutput->addBuffer(renderBuffer);
		_renderOutput->notifyBuffersUpdated();

		// Set up frame buffer ops 
		{
			auto toneMappingDao = toneMappingAny->rawData<fl::DataObject>();

			auto enable_tone_mapping = toneMappingDao->property("enable_tone_mapping")->data<bool>();
			auto aces_color = toneMappingDao->property("aces_color")->data<bool>();
			auto exposure = toneMappingDao->property("exposure")->data<float>();
			auto contrast = toneMappingDao->property("contrast")->data<float>();
			auto shoulder = toneMappingDao->property("shoulder")->data<float>();
			auto mid_in  = toneMappingDao->property("mid_in")->data<float>();
			auto mid_out = toneMappingDao->property("mid_out")->data<float>();
			auto hdr_max = toneMappingDao->property("hdr_max")->data<float>();
			
			auto enableDenosing = daoGeneral->property("denoising")->data<bool>();

			std::vector<ospray::cpp::ImageOperation> imageOps;
			ospray::cpp::ImageOperation denoiseOp("denoiser");
			ospray::cpp::ImageOperation toneMappingOp("tonemapper");		

			if (enable_tone_mapping) {
				toneMappingOp.setParam("exposure", exposure);
				toneMappingOp.setParam("contrast", contrast);
				toneMappingOp.setParam("shoulder", shoulder);
				toneMappingOp.setParam("midIn", mid_in);
				toneMappingOp.setParam("midOut", mid_out);
				toneMappingOp.setParam("hdrMax", hdr_max);
				toneMappingOp.setParam("acesColor", aces_color);
				imageOps.push_back(toneMappingOp);
			}

			if (enableDenosing) {
				imageOps.push_back(denoiseOp);
			}

			if (!imageOps.empty()) {
				framebuffer->setParam("imageOperation", ospray::cpp::CopiedData(imageOps));
				framebuffer->commit();
			}
		}
	}
	else {
		DfxGlobal::console::println("OSPRay Error: Couldn't find rendering settings.");
		return false;
	}

	return true;
}

bool DfxOSPRayRenderSession::clear() {

	// Clear Scene
	for (auto _light : _ospLights) {
		delete _light;
	}

	for (auto _obj : _ospObjects) {
		delete _obj;
	}

	for (auto _vol : _ospVolumes) {
		delete _vol;
	}

	_ospLights.clear();
	_ospObjects.clear();
	_ospVolumes.clear();

	_initialized = false;

	/*id_gen.reset();
	if (render_session != nullptr) render_session->Stop();
	delete scene;
	delete render_config;
	delete render_session;

	if (rtHaltTimer != nullptr) {
		rtHaltTimer->stop();
		delete rtHaltTimer;
	}

	existingLights.clear();

	for (auto ob : observerList) {
		delete ob;
	}

	for (auto obs : cameraObservers) {
		delete obs;
	}

	cameraObservers.clear();
	observerList.clear();

	scene = nullptr;
	render_config = nullptr;
	render_session = nullptr;
	activeBranch = nullptr;
	rtHaltTimer = nullptr;

	_refreshInterval = 0;
	_haltTime = 0;
	_haltSpp = 0;*/

	return true;
}

bool DfxOSPRayRenderSession::checkSceneIsReady(std::string tag) {

	/*if (!_initialized) {
		DfxGlobal::console::println(tag + " Error: LuxCore not initialized.");
		return false;
	}

	if (scene == nullptr) {
		DfxGlobal::console::println(tag + " Error: Scene not initialized.");
		return false;
	}*/

	return true;
}

bool DfxOSPRayRenderSession::checkSessionIsReady(std::string tag) {
	/*if (render_config == nullptr) {
		DfxGlobal::console::println(tag + " Error: No Render Configuration.");
		return false;
	}

	if (render_session == nullptr) {
		DfxGlobal::console::println(tag + " Error: No Session.");
		return false;
	}*/

	return true;
}

bool DfxOSPRayRenderSession::loadScene(deepsg::SgNode *branch) {

	loadCamera(branch);
	loadMaterials(branch);
	loadObjects(branch);
	loadVolumes(branch);
	/*loadInstances();
	loadFluentSimplicies();
	loadFluentSimplexCaches();*/
	loadLights(branch);

	loadWorld();

	activeBranch = branch;
	return true;
}

bool DfxOSPRayRenderSession::unloadScene() {
	/*if (!checkSceneIsReady("LuxCore Session Prep"))return false;*/

	return true;
}

bool DfxOSPRayRenderSession::updateTransformations() {
	//if (!checkSceneIsReady("LuxCore Session Prep"))return false;

	return true;
}

bool DfxOSPRayRenderSession::updateGeometry() {
	return true;
}

bool DfxOSPRayRenderSession::updateMaterials() {
	/*const std::lock_guard<std::recursive_mutex> lock(_updateSessionMutex);

	if (!checkSessionIsReady("LuxCore Render")) {
		LC_LOG("LuxCore Render Session Update Failed.");
		return false;
	}

	render_session->BeginSceneEdit();
	loadMaterials(nullptr);
	for (auto p : objectSegments) {
		for (auto seginfo : p.second) {
			if (DfxMaterialLinker::getInstance()->hasObjectLink(p.first, seginfo.surf_id)) {

				auto globalMatPath = DfxMaterialLinker::getInstance()->objectLink(p.first, seginfo.surf_id);
				auto luxMat = matBuilder.material(globalMatPath);

				if (!luxMat.empty()) {

					Properties props;
					props
						<< Property("scene.objects." + seginfo.objectName + ".shape")(seginfo.shapeName)
						<< Property("scene.objects." + seginfo.objectName + ".material")(luxMat)
						<< Property("scene.objects." + seginfo.objectName + ".transformation")(seginfo.transform);

					scene->Parse(props);
				}
				else {

					Properties props;
					props
						<< Property("scene.objects." + seginfo.objectName + ".shape")(seginfo.shapeName)
						<< Property("scene.objects." + seginfo.objectName + ".material")("default")
						<< Property("scene.objects." + seginfo.objectName + ".transformation")(seginfo.transform);

					scene->Parse(props);
				}
			}
			else {
				Properties props;
				props
					<< Property("scene.objects." + seginfo.objectName + ".shape")(seginfo.shapeName)
					<< Property("scene.objects." + seginfo.objectName + ".material")("default")
					<< Property("scene.objects." + seginfo.objectName + ".transformation")(seginfo.transform);

				scene->Parse(props);
			}
		}
	}
	render_session->EndSceneEdit();

	if (render_session->IsInPause()) {
		render_session->Resume();
		newBufferRefreshTimer(120);
	}
	newHaltTimer(rtHaltTime);*/

	return true;
}

bool DfxOSPRayRenderSession::updateGeometryTransform() {
	/*const std::lock_guard<std::recursive_mutex> lock(_updateSessionMutex);

	if (!checkSessionIsReady("LuxCore Render")) {
		LC_LOG("LuxCore Render Session Update Failed.");
		return false;
	}

	deepsg::SgConditionalTraversalFunction renderFunc = [this](deepsg::SgNode *node, deepsg::SgStateInfo *info)-> deepsg::TraversalFlag {

		if (node->getType() == deepsg::SgNodeType::GEOMETRY && node->isRenderable()) {

			auto itr00 = objectSegments.find(node->referencePath());
			auto itr01 = objectTransform.find(node->referencePath());

			if (itr00 != objectSegments.end()) {

				glm::mat4 finalTransform(1.f);

				if (!node->transformOverrides().empty()) {

					deepsg::SgTransformOverrideInfo ov_info;

					auto fps = DfxTimelineManager::getInstance()->fps();
					auto current_frame = DfxTimelineManager::getInstance()->currentFrame();
					double time = (double)current_frame / (double)fps;

					ov_info.fps = fps;
					ov_info.frame = current_frame;
					ov_info.time = time;
					ov_info.globalTransform = info->transformMatrix();

					finalTransform = rootTransform();
					for (auto xoverride : node->transformOverrides()) {
						finalTransform *= xoverride->evaluate(ov_info);
					}
				}
				else {
					finalTransform = info->transformMatrix();
				}

				for (auto &segment : itr00->second) {

					auto transformInverse = glm::inverse(itr01->second);
					scene->UpdateObjectTransformation(segment.objectName, &transformInverse[0][0]);
					scene->UpdateObjectTransformation(segment.objectName, &finalTransform[0][0]);
				}

				itr01->second = finalTransform;
			}
			return deepsg::TraversalFlag::CONTINUE;
		}
		else {
			return deepsg::TraversalFlag::IGNORE_CHILDREN;
		}
	};

	render_session->BeginSceneEdit();
	deepsg::SgRootNode::getInstance()->traverseSubScene(deepsg::SgRootNode::getInstance(), renderFunc, rootTransform());
	render_session->EndSceneEdit();

	if (render_session->IsInPause()) {
		render_session->Resume();
		newBufferRefreshTimer(120);
	}
	newHaltTimer(rtHaltTime);*/

	return true;
}

bool DfxOSPRayRenderSession::updateLights() {
	//const std::lock_guard<std::recursive_mutex> lock(_updateSessionMutex);

	//if (auto lock = std::unique_lock<std::recursive_mutex>(_updateSessionMutex, std::try_to_lock)) {
	//	if (!checkSessionIsReady("LuxCore Render")) {
	//		LC_LOG("LuxCore Render Session Update Failed.");
	//		return false;
	//	}

	//	render_session->BeginSceneEdit();
	//	loadLights(activeBranch);
	//	render_session->EndSceneEdit();

	//	if (render_session->IsInPause()) {
	//		render_session->Resume();
	//		newBufferRefreshTimer(120);
	//	}
	//	newHaltTimer(rtHaltTime);

	//}
	return true;
}

bool DfxOSPRayRenderSession::updateCamera() {
	/*if (!checkSessionIsReady("LuxCore Render")) {
		LC_LOG("LuxCore Render Session Update Failed.");
		return false;
	}

	render_session->BeginSceneEdit();
	loadCamera(activeBranch);
	render_session->EndSceneEdit();

	if (render_session->IsInPause()) {
		render_session->Resume();
		newBufferRefreshTimer(120);
	}
	newHaltTimer(rtHaltTime);*/

	return true;
}

void writePPM(
	const char *fileName, int size_x, int size_y, const uint32_t *pixel)
{
	FILE *file = fopen(fileName, "wb");
	if (!file) {
		fprintf(stderr, "fopen('%s', 'wb') failed: %d", fileName, errno);
		return;
	}
	fprintf(file, "P6\n%i %i\n255\n", size_x, size_y);
	unsigned char *out = (unsigned char *)alloca(3 * size_x);
	for (int y = 0; y < size_y; y++) {
		const unsigned char *in =
			(const unsigned char *)&pixel[(size_y - 1 - y) * size_x];
		for (int x = 0; x < size_x; x++) {
			out[3 * x + 0] = in[4 * x + 0];
			out[3 * x + 1] = in[4 * x + 1];
			out[3 * x + 2] = in[4 * x + 2];
		}
		fwrite(out, 3 * size_x, sizeof(char), file);
	}
	fprintf(file, "\n");
	fclose(file);
}

void DfxOSPRayRenderSession::forceStop() {
	_forceStop = true;
}

bool DfxOSPRayRenderSession::render() {
	auto space = DfxSpace::getInstance();

	_forceStop = false;
	auto _printInfo = true;

	bool renderDone = false;
	bool warmStart = true;
	double elapsedTime = 0.0;
	double warmStartElapsed = 0.0;
	double warmStartInterval = 0.2;

	bool flushBuffer = false;
	auto future = framebuffer->renderFrame(*renderer, *camera, *world);
	QTime current_elapsedTime;
	current_elapsedTime.start();
	while (!renderDone && !_forceStop) {
		

		auto _current_elapsedTime = float(current_elapsedTime.elapsed()) / 1000.f;

		if (!_forceStop) QApplication::processEvents();

		if (!_forceStop) {
			
			if (warmStart) {
				if ((int)(_current_elapsedTime - warmStartElapsed) > (warmStartInterval - 1)) {
					warmStartElapsed = float(current_elapsedTime.elapsed()) / 1000.f;
					flushBuffer = true;
				}

				if (warmStartElapsed > 10.0) warmStart = false;
			}
			else {
	
				if ((int)(_current_elapsedTime - elapsedTime) > (_refreshInterval - 1)) {
					elapsedTime = float(current_elapsedTime.elapsed()) / 1000.f;
					flushBuffer = true;
				}
			}

			/*if (_haltTime > 0) {
				auto _current_elapsedTime = float(current_elapsedTime.elapsed()) / 1000.f;
				double complete = current_elapsedTime / (double)_haltTime;
				_renderOutput->setProgress(complete * 100.0);
			}*/

			if (_current_elapsedTime > float(_haltTime)) {
				future.wait();
				renderDone = true;
			}

			/*if (_haltSpp > 0) {
				double complete = (double)current_samples / (double)_haltSpp;
				_renderOutput->setProgress(complete * 100.0);
			}*/
			if (!renderDone && !_forceStop) {
				if (future.isReady()) {
					if (!updateOutput()) DfxGlobal::console::println("Failed to render output.");
					flushBuffer = false;
					future = framebuffer->renderFrame(*renderer, *camera, *world);
				}
			}
			else {
				if (!future.isReady()) {
					future.wait();
				}
			}
			
		}
	}

	//estimatePathClampingVariance();

	if (!updateOutput(true)) DfxGlobal::console::println("Failed to render output.");
	for (int i = 0; i < _renderOutput->bufferCount(); i++) {

		if (_renderOutput->getBuffer(i)->bufferName() == "sRGBA") {
			_renderOutput->getBuffer(i)->getImage().save("c:/test/osp_smoke_test.png", "PNG");
			break;
		}
	}

	_renderOutput->setProgress(100);

	// render_session->GetFilm().SaveOutputs();
	// output().save("c:/test/render_result.png", "PNG");

	return true;
}

bool DfxOSPRayRenderSession::renderExport(std::string dir) {
	//luxrays::Properties properties;

	//properties
	//	<< Property("renderengine.type")("FILESAVER")
	//	<< Property("filesaver.format")("TXT")
	//	<< Property("filesaver.directory")(dir);

	//auto r_config = luxcore::RenderConfig::Create(properties, scene);
	//auto exrender_session = luxcore::RenderSession::Create(r_config);

	//exrender_session->Start();

	//while (!exrender_session->HasDone());

	//delete r_config;
	//delete exrender_session;

	return true;
}

QImage DfxOSPRayRenderSession::output() {
	/*if (!checkSessionIsReady("LuxCore Render"))return QImage(540, 360, QImage::Format::Format_ARGB32);

	DfxLxCameraHandler camHandler;

	if (cameraPath.empty()) {
		LC_LOG("LuxCore Session Ouput RUNTIME ERROR: Empty camera path.");
		return QImage(540, 360, QImage::Format::Format_RGB888);
	}
	else {
		auto rc = camHandler.fetch(cameraPath);

		if (!rc) {
			LC_LOG("LuxCore Session Ouput RUNTIME ERROR: Failed to fetch handle.");
			return QImage(540, 360, QImage::Format::Format_RGB888);
		}
	}

	int camWidth = camHandler.dataObject()->data<fl::DataObject>().property("width")->data<unsigned int>();
	int camHeight = camHandler.dataObject()->data<fl::DataObject>().property("height")->data<unsigned int>();

	try {
		QImage image(camWidth, camHeight, QImage::Format::Format_RGB888);
		int buffer_size = render_session->GetFilm().GetOutputSize(luxcore::Film::FilmOutputType::OUTPUT_RGB_IMAGEPIPELINE);

		std::unique_ptr<float[]> _ibuffer(new float[buffer_size]);

		if (_renderEngine == RT) {
			if (!render_session->IsInPause() && render_session->IsStarted()) {
				render_session->GetFilm().GetOutput<float>(luxcore::Film::FilmOutputType::OUTPUT_RGB_IMAGEPIPELINE, _ibuffer.get(), 0);
			}
			else {
				render_session->GetFilm().GetOutput<float>(luxcore::Film::FilmOutputType::OUTPUT_RGB_IMAGEPIPELINE, _ibuffer.get(), 1);
			}
		}
		else {
			render_session->GetFilm().GetOutput<float>(luxcore::Film::FilmOutputType::OUTPUT_RGB_IMAGEPIPELINE, _ibuffer.get(), 0);
		}

		for (int i = 0; i < buffer_size; i++) image.bits()[i] = (uchar)(_ibuffer.get()[i] * 255.f);

		return image.mirrored();
	}
	catch (std::runtime_error &err) {
		LC_LOG("LuxCore Session Output RUNTIME ERROR: " << err.what());
		return QImage(540, 360, QImage::Format::Format_RGB888);
	}
	catch (std::exception &err) {
		LC_LOG("LuxCore Session Output ERROR: " << err.what());
		return QImage(540, 360, QImage::Format::Format_RGB888);
	}*/

	return QImage(540, 360, QImage::Format::Format_RGB888);
}

void DfxOSPRayRenderSession::_updateOutput() {
	updateOutput(false);
}

void DfxOSPRayRenderSession::applyWaterMark(QImage &image) {

	QImage watermark(QString(":/icon/watermark"));
	QPainter painter;
	painter.begin(&image);
	painter.setOpacity(0.1);
	QRect rect(watermark.rect());
	QRect devRect(0, 0, painter.device()->width(), painter.device()->height());
	painter.drawImage(devRect.topLeft(), watermark);
	painter.drawImage(devRect.topRight().rx() - rect.width(), devRect.topRight().ry(), watermark);
	painter.drawImage(devRect.center().rx() - (rect.width() * 0.5f), devRect.center().ry() - (rect.height() * 0.5f), watermark);
	painter.drawImage(devRect.bottomRight().rx() - rect.width(), devRect.bottomRight().ry() - rect.height(), watermark);
	painter.drawImage(devRect.bottomLeft().rx(), devRect.bottomLeft().ry() - rect.height(), watermark);
	painter.end();

}

bool DfxOSPRayRenderSession::updateOutput(bool applyDenoising) {
	 const std::lock_guard<std::recursive_mutex> lock(_updateOutputMutex);

	if (auto lock = std::unique_lock<std::recursive_mutex>(_updateSessionMutex, std::try_to_lock)) {

		for (int i = 0; i < _renderOutput->bufferCount(); i++) {

			if (_renderOutput->getBuffer(i)->bufferName() == "sRGBA") {

				
				const float *fb = (float*)framebuffer->map(OSP_FB_COLOR);

				QImage image(outputWidth, outputHeight, QImage::Format::Format_RGBA8888);

				for (int j = 0; j < outputWidth * outputHeight * 4; j++) {
					image.bits()[j] = (uchar)std::min(255.f, (fb[j] * 255.f));
				}
	
				image = image.mirrored();
				image = image.convertToFormat(QImage::Format::Format_RGB888);
				
				framebuffer->unmap((void*)fb);

				_renderOutput->getBuffer(i)->setImage(image);

				std::cout << "Variance: " << framebuffer->variance() << std::endl;
			}		
			
		}
	}

	return true;
}

void DfxOSPRayRenderSession::testMaterials() {
	//DfxLuxCoreMaterialBuilder builder;
	//builder.build(scene);
}

bool DfxOSPRayRenderSession::loadMaterials(deepsg::SgNode *branch) {

	defaultMat.reset(new ospray::cpp::Material("pathtracer", "obj"));
	defaultMat->commit();

	matBuilder.build();

	return true;
}

struct MeshSegment {
	size_t surf_id;
	int materialID;
	std::vector<deepsg::SgGeoAdapterInterface::Triangle> triangles;
};

bool DfxOSPRayRenderSession::loadFluentSimplicies() {

	/*auto materialIDS = DfxMaterialManager::getInstance()->materialIDMap();
	for (auto &p : DfxFluentSetManager::getInstance()->setMap()) {
		if (p.second->isVisible()) {

			if (p.second->type() == DfxFluentSet::SIMPLEX) {
				for (auto index : p.second->indices()) {
					auto simplexObj = DfxFluentSimplexCollection::getInstance()->simplexObjects()[index];
					auto adapter = simplexObj->getGeometryAdapter();
					auto xform = rootTransform() * DfxFluentSimplexCollection::getInstance()->atrributeMap().at<glm::mat4>("@xform")[index];

					if (adapter->getPolygonCount() > 0 && adapter->getVertexCount() > 0) {

						std::unordered_map<size_t, MeshSegment*> meshSegments;
						auto tricount = adapter->getTriangleIndexListSize() / 3;
						auto indices = adapter->getTriangleIndexList();
						auto sourceFaces = adapter->getTriangleSourceFaceIndexList();
						for (size_t i = 0; i < tricount; i++) {
							auto offset = i * 3;
							auto sid = adapter->getPolygonSurfaceID(sourceFaces[i]);

							MeshSegment *currentSegment = nullptr;

							auto itr = meshSegments.find(sid);

							if (itr == meshSegments.end()) {
								currentSegment = new MeshSegment;
								currentSegment->materialID = sid;
								meshSegments[sid] = currentSegment;
							}
							else {
								currentSegment = itr->second;
							}

							deepsg::SgGeoAdapterInterface::Triangle triangle;
							triangle.a = indices[offset];
							triangle.b = indices[offset + 1];
							triangle.c = indices[offset + 2];
							triangle.faceIndex = sourceFaces[i];

							currentSegment->triangles.push_back(triangle);

						}

						delete[] indices;
						delete[] sourceFaces;

						std::vector<DfxLuxObjectSegment> segmentInfo;
						for (auto p : meshSegments) {

							auto segment = p.second;

							size_t vertCount = 0;
							size_t polyCount = 0;
							std::unordered_map<size_t, size_t> oldNewIndex;
							std::vector<size_t> newVertexIndices;
							std::string material = "default";

							bool overrideSmoothing = false;
							bool overrideUseSmoothing = false;

							auto matidItr = materialIDS.find(p.second->materialID);

							if (matidItr != materialIDS.end()) {
								auto luxMat = matBuilder.material(matidItr->second);
								if (!luxMat.empty()) {
									material = luxMat;
								}
							}

							vertCount = segment->triangles.size() * 3;
							polyCount = segment->triangles.size();

							if (polyCount > 0) {

								LuxPoint *p = (LuxPoint*)luxcore::Scene::AllocVerticesBuffer(vertCount);
								LuxTriangle *t = (LuxTriangle*)luxcore::Scene::AllocTrianglesBuffer(polyCount);
								LuxUV *uvs = new LuxUV[vertCount];

								float *normals = NULL;

								if (matidItr != materialIDS.end()) {
									auto matAny = DfxSpace::getInstance()->findAbsolute(matidItr->second);

									if (matAny != nullptr) {
										if (matAny->type() == fl::type::DATA_OBJECT) {
											auto matDao = matAny->rawData<fl::DataObject>();

											if (matDao->hasProperty("smooth_shading_override")) {
												overrideSmoothing = matDao->property("smooth_shading_override")->data<bool>();
											}

											if (matDao->hasProperty("use_smooth_shading")) {
												overrideUseSmoothing = matDao->property("use_smooth_shading")->data<bool>();
											}
										}
									}
								}

								if (overrideSmoothing) {
									if (overrideUseSmoothing) {
										normals = new float[vertCount * 3];
									}
								}

								for (int i = 0; i < polyCount; i++) {

									auto offset = i * 3;
									auto norm_offset = i * 9;
									auto uv_offset = i * 3;

									auto uvMap = adapter->getPolygonUVMap(segment->triangles[i].faceIndex);

									size_t a = segment->triangles[i].a;
									size_t b = segment->triangles[i].b;
									size_t c = segment->triangles[i].c;

									p[offset] = LuxPoint(adapter->getVertex(a).x, adapter->getVertex(a).y, adapter->getVertex(a).z);
									p[offset + 1] = LuxPoint(adapter->getVertex(b).x, adapter->getVertex(b).y, adapter->getVertex(b).z);
									p[offset + 2] = LuxPoint(adapter->getVertex(c).x, adapter->getVertex(c).y, adapter->getVertex(c).z);

									auto auv = uvMap[a];
									auto buv = uvMap[b];
									auto cuv = uvMap[c];

									uvs[uv_offset] = LuxUV(auv.x, auv.y);
									uvs[uv_offset + 1] = LuxUV(buv.x, buv.y);
									uvs[uv_offset + 2] = LuxUV(cuv.x, cuv.y);

									if (normals != NULL) {
										normals[norm_offset] = adapter->getSmoothNormal(a).x;
										normals[norm_offset + 1] = adapter->getSmoothNormal(a).y;
										normals[norm_offset + 2] = adapter->getSmoothNormal(a).z;

										normals[norm_offset + 3] = adapter->getSmoothNormal(b).x;
										normals[norm_offset + 4] = adapter->getSmoothNormal(b).y;
										normals[norm_offset + 5] = adapter->getSmoothNormal(b).z;

										normals[norm_offset + 6] = adapter->getSmoothNormal(c).x;
										normals[norm_offset + 7] = adapter->getSmoothNormal(c).y;
										normals[norm_offset + 8] = adapter->getSmoothNormal(c).z;
									}

									t[i] = LuxTriangle(offset, offset + 1, offset + 2);

								}

								std::string id = std::to_string(id_gen.getID());
								std::string shapeName = std::string("shape") + id;
								std::string itemName = std::string("item") + id;

								scene->DefineMesh(shapeName, vertCount, polyCount, (float*)p, (unsigned int*)t, normals, (float*)uvs, NULL, NULL);

								Properties props;
								props.SetFromString(
									"scene.objects." + itemName + ".shape = " + shapeName + "\n"
									"scene.objects." + itemName + ".material = " + material + "\n"
								);
								scene->Parse(props);

								scene->UpdateObjectTransformation(itemName, &xform[0][0]);

								DfxLuxObjectSegment sinfo;
								sinfo.surf_id = segment->surf_id;
								sinfo.id = id;
								sinfo.material = material;
								sinfo.objectName = itemName;
								sinfo.shapeName = shapeName;

								segmentInfo.push_back(sinfo);
							}

							delete segment;
						}

					}
				}
			}

		}
	}*/

	return true;
}

bool DfxOSPRayRenderSession::loadFluentSimplexCaches() {

	/*auto materialIDS = DfxMaterialManager::getInstance()->materialIDMap();
	for (auto track : DfxCacheTrackManager::getInstance()->tracks()) {
		auto snapshot = track->snapshot();

		for (auto set : snapshot->sets()) {
			if (set->isVisible()) {

				if (set->type() == DfxFluentSet::SIMPLEX) {
					for (auto index : set->indices()) {

						auto simplexObj = snapshot->simplexObjects()[index];
						auto adapter = simplexObj->getGeometryAdapter();
						auto xform = rootTransform() * snapshot->simplexAttributes().at<glm::mat4>("@xform")[index];

						if (adapter->getPolygonCount() > 0 && adapter->getVertexCount() > 0) {

							std::unordered_map<size_t, MeshSegment*> meshSegments;
							auto tricount = adapter->getTriangleIndexListSize() / 3;
							auto indices = adapter->getTriangleIndexList();
							auto sourceFaces = adapter->getTriangleSourceFaceIndexList();
							for (size_t i = 0; i < tricount; i++) {
								auto offset = i * 3;
								auto sid = adapter->getPolygonSurfaceID(sourceFaces[i]);

								MeshSegment *currentSegment = nullptr;

								auto itr = meshSegments.find(sid);

								if (itr == meshSegments.end()) {
									currentSegment = new MeshSegment;
									currentSegment->materialID = sid;
									meshSegments[sid] = currentSegment;
								}
								else {
									currentSegment = itr->second;
								}

								deepsg::SgGeoAdapterInterface::Triangle triangle;
								triangle.a = indices[offset];
								triangle.b = indices[offset + 1];
								triangle.c = indices[offset + 2];
								triangle.faceIndex = sourceFaces[i];

								currentSegment->triangles.push_back(triangle);

							}
							delete[] indices;
							delete[] sourceFaces;

							std::vector<DfxLuxObjectSegment> segmentInfo;
							for (auto p : meshSegments) {

								auto segment = p.second;

								size_t vertCount = 0;
								size_t polyCount = 0;
								std::unordered_map<size_t, size_t> oldNewIndex;
								std::vector<size_t> newVertexIndices;
								std::string material = "default";

								bool overrideSmoothing = false;
								bool overrideUseSmoothing = false;

								auto matidItr = materialIDS.find(p.second->materialID);

								if (matidItr != materialIDS.end()) {
									auto luxMat = matBuilder.material(matidItr->second);
									if (!luxMat.empty()) {
										material = luxMat;
									}
								}

								vertCount = segment->triangles.size() * 3;
								polyCount = segment->triangles.size();

								if (polyCount > 0) {

									LuxPoint *p = (LuxPoint*)luxcore::Scene::AllocVerticesBuffer(vertCount);
									LuxTriangle *t = (LuxTriangle*)luxcore::Scene::AllocTrianglesBuffer(polyCount);
									LuxUV *uvs = new LuxUV[vertCount];

									float *normals = NULL;

									if (matidItr != materialIDS.end()) {
										auto matAny = DfxSpace::getInstance()->findAbsolute(matidItr->second);

										if (matAny != nullptr) {
											if (matAny->type() == fl::type::DATA_OBJECT) {
												auto matDao = matAny->rawData<fl::DataObject>();

												if (matDao->hasProperty("smooth_shading_override")) {
													overrideSmoothing = matDao->property("smooth_shading_override")->data<bool>();
												}

												if (matDao->hasProperty("use_smooth_shading")) {
													overrideUseSmoothing = matDao->property("use_smooth_shading")->data<bool>();
												}
											}
										}
									}

									if (overrideSmoothing) {
										if (overrideUseSmoothing) {
											normals = new float[vertCount * 3];
										}
									}

									for (int i = 0; i < polyCount; i++) {

										auto offset = i * 3;
										auto norm_offset = i * 9;
										auto uv_offset = i * 3;

										auto uvMap = adapter->getPolygonUVMap(segment->triangles[i].faceIndex);

										size_t a = segment->triangles[i].a;
										size_t b = segment->triangles[i].b;
										size_t c = segment->triangles[i].c;

										p[offset] = LuxPoint(adapter->getVertex(a).x, adapter->getVertex(a).y, adapter->getVertex(a).z);
										p[offset + 1] = LuxPoint(adapter->getVertex(b).x, adapter->getVertex(b).y, adapter->getVertex(b).z);
										p[offset + 2] = LuxPoint(adapter->getVertex(c).x, adapter->getVertex(c).y, adapter->getVertex(c).z);

										auto auv = uvMap[a];
										auto buv = uvMap[b];
										auto cuv = uvMap[c];

										uvs[uv_offset] = LuxUV(auv.x, auv.y);
										uvs[uv_offset + 1] = LuxUV(buv.x, buv.y);
										uvs[uv_offset + 2] = LuxUV(cuv.x, cuv.y);

										if (normals != NULL) {
											normals[norm_offset] = adapter->getSmoothNormal(a).x;
											normals[norm_offset + 1] = adapter->getSmoothNormal(a).y;
											normals[norm_offset + 2] = adapter->getSmoothNormal(a).z;

											normals[norm_offset + 3] = adapter->getSmoothNormal(b).x;
											normals[norm_offset + 4] = adapter->getSmoothNormal(b).y;
											normals[norm_offset + 5] = adapter->getSmoothNormal(b).z;

											normals[norm_offset + 6] = adapter->getSmoothNormal(c).x;
											normals[norm_offset + 7] = adapter->getSmoothNormal(c).y;
											normals[norm_offset + 8] = adapter->getSmoothNormal(c).z;
										}

										t[i] = LuxTriangle(offset, offset + 1, offset + 2);

									}

									std::string id = std::to_string(id_gen.getID());
									std::string shapeName = std::string("shape") + id;
									std::string itemName = std::string("item") + id;

									scene->DefineMesh(shapeName, vertCount, polyCount, (float*)p, (unsigned int*)t, normals, (float*)uvs, NULL, NULL);

									Properties props;
									props.SetFromString(
										"scene.objects." + itemName + ".shape = " + shapeName + "\n"
										"scene.objects." + itemName + ".material = " + material + "\n"
									);
									scene->Parse(props);

									scene->UpdateObjectTransformation(itemName, &xform[0][0]);

									DfxLuxObjectSegment sinfo;
									sinfo.surf_id = segment->surf_id;
									sinfo.id = id;
									sinfo.material = material;
									sinfo.objectName = itemName;
									sinfo.shapeName = shapeName;

									segmentInfo.push_back(sinfo);
								}

								delete segment;
							}

						}
					}
				}

			}
		}
	}*/
	return true;
}

void DfxOSPRayRenderSession::findInstancePaths() {

	/*_instanceShapes.clear();

	for (auto instancer : DfxInstanceManager::getInstance()->intancers()) {

		std::unordered_map<size_t, DfxQRenderInstanceInfo> instanceData;

		instancer->begin();

		if (instancer->isEnabled()) {

			for (size_t inx = 0; inx < instancer->instanceCount(); inx++) {

				auto node = instancer->instanceShape(inx);

				if (node != nullptr) {
					_instanceShapes[node->referencePath()] = std::vector<DfxLuxObjectSegment>();
				}
			}

		}

		instancer->end();
	}*/

}

bool DfxOSPRayRenderSession::loadObjects(deepsg::SgNode *branch) {	

	deepsg::SgConditionalTraversalFunction renderFunc = [this](deepsg::SgNode *node, deepsg::SgStateInfo *info)-> deepsg::TraversalFlag {

		// auto instItr = _instanceShapes.find(node->referencePath());

		if (node->getType() == deepsg::SgNodeType::GEOMETRY) {

			if (!node->isRenderable()) {
				return deepsg::TraversalFlag::IGNORE_CHILDREN;
			}

			DfxObjectHandler handler;

			auto geoNode = (deepsg::SgGeoNode*)node;
			auto adapter = ((deepsg::SgGeoNode*)node)->getGeometryAdapter();
			handler.fetch(node->referencePath());

			auto tricount = int(adapter->getTriangleIndexListSize() / 3);
			auto vertCount = int(tricount * 3);
			std::unique_ptr<unsigned int[]> triIndices(adapter->getTriangleIndexList());
			std::unique_ptr<unsigned int[]> triFaceIndices(adapter->getTriangleSourceFaceIndexList());
			
			// Calculate Final Transform
			glm::mat4 finalTransform(1.f);

			if (!node->transformOverrides().empty()) {

				deepsg::SgTransformOverrideInfo ov_info;

				auto fps = DfxTimelineManager::getInstance()->fps();
				auto current_frame = DfxTimelineManager::getInstance()->currentFrame();
				double time = (double)current_frame / (double)fps;

				ov_info.fps = fps;
				ov_info.frame = current_frame;
				ov_info.time = time;
				ov_info.globalTransform = info->transformMatrix();

				finalTransform = rootTransform();
				for (auto xoverride : node->transformOverrides()) {
					finalTransform *= xoverride->evaluate(ov_info);
				}
			}
			else {
				finalTransform = info->transformMatrix();
			}

			if (tricount > 0) {
				auto _obj = new OSPRayObject;

				std::vector<rkcommon::math::vec3f> v;
				std::vector<rkcommon::math::vec2f> uv;
				std::vector<rkcommon::math::vec3f> n;
				std::vector<rkcommon::math::vec3ui> inx;
				std::vector<ospray::cpp::Material> materials;

				v.resize(vertCount);
				n.resize(vertCount);
				uv.resize(vertCount);
				inx.resize(tricount);
				materials.resize(tricount);

				for (int i = 0; i < tricount * 3; i++) {	
					auto vert = adapter->getVertex(triIndices.get()[i]);
					auto norm = adapter->getSmoothNormal(triIndices.get()[i]);

					v[i] = rkcommon::math::vec3f(vert.x, vert.y, vert.z);
					n[i] = rkcommon::math::vec3f(norm.x, norm.y, norm.z);

				}

				for (int i = 0; i < tricount; i++) {
					auto offset = i * 3;

					inx[i] = rkcommon::math::vec3ui(offset, offset + 1, offset + 2);

					size_t a = triIndices.get()[offset];
					size_t b = triIndices.get()[offset + 1];
					size_t c = triIndices.get()[offset + 2];

					if (adapter->type() == DfxGeoAdapterType::DYNA_MESH) {

					}
					else {

						auto uvMap = adapter->getPolygonUVMap(triFaceIndices[i]);

						auto auv = uvMap[a];
						auto buv = uvMap[b];
						auto cuv = uvMap[c];

						uv[offset] = rkcommon::math::vec2f(auv.x, auv.y);
						uv[offset + 1] = rkcommon::math::vec2f(buv.x, buv.y);
						uv[offset + 2] = rkcommon::math::vec2f(cuv.x, cuv.y);

					}

					auto globalMatPath = DfxMaterialLinker::getInstance()->objectLink(geoNode->referencePath(), adapter->getPolygonSurfaceID(triFaceIndices[i]));
					auto ospMat = matBuilder.material(globalMatPath);

					if (ospMat != nullptr) {
						materials[i] = *ospMat;
					}
					else {
						materials[i] = *defaultMat;
					}
				}

				rkcommon::math::affine3f xfm = glmMat4ToAffine3(finalTransform);			
				
				// Copy mesh data to OSPRay
				_obj->mesh.reset(new ospray::cpp::Geometry("mesh"));
				_obj->mesh->setParam("vertex.position", ospray::cpp::CopiedData(v));
				_obj->mesh->setParam("vertex.texcoord", ospray::cpp::CopiedData(uv));
				_obj->mesh->setParam("vertex.normal", ospray::cpp::CopiedData(n));
				_obj->mesh->setParam("index", ospray::cpp::CopiedData(inx));
				_obj->mesh->commit();

				_obj->model.reset(new ospray::cpp::GeometricModel(*_obj->mesh));
				_obj->model->setParam("material", ospray::cpp::CopiedData(materials));
				_obj->model->commit();

				_obj->group.reset(new ospray::cpp::Group());
				_obj->group->setParam("geometry", ospray::cpp::CopiedData(*_obj->model));
				_obj->group->commit();

				_obj->instance.reset(new ospray::cpp::Instance(*_obj->group));
				_obj->instance->setParam("transform", xfm);
				_obj->instance->commit();

				_ospObjects.push_back(_obj);

			}
			
			return deepsg::TraversalFlag::CONTINUE;
		}
		else {

			if (node->isRenderable()) {
				return deepsg::TraversalFlag::CONTINUE;
			}
			else {
				return deepsg::TraversalFlag::IGNORE_CHILDREN;
			}

		}
	};

	deepsg::SgRootNode::getInstance()->traverseSubSceneConditionally(branch, renderFunc, glm::mat4(1.f));
	//traverseSubScene(branch, renderFunc);

	return true;
}

bool DfxOSPRayRenderSession::loadInstances() {

	//std::unordered_map<std::string, std::vector<glm::mat4>> _transforms;
	//size_t instID = 0xff;

	//// Collect Transforms
	//for (auto instancer : DfxInstanceManager::getInstance()->intancers()) {

	//	std::unordered_map<size_t, DfxQRenderInstanceInfo> instanceData;

	//	instancer->begin();

	//	if (instancer->isEnabled()) {

	//		for (size_t inx = 0; inx < instancer->instanceCount(); inx++) {

	//			auto node = instancer->instanceShape(inx);

	//			if (node != nullptr) {
	//				auto instItr = _instanceShapes.find(node->referencePath());
	//				auto trans = _transforms.find(node->referencePath());

	//				if (instItr != _instanceShapes.end()) {

	//					glm::mat4 xform(1.f);

	//					if (instancer->useTransformMatrix(inx))
	//						xform = instancer->instanceTransform(inx);
	//					else
	//						xform = instancer->finalTransform(inx);

	//					if (trans == _transforms.end()) {
	//						_transforms[node->referencePath()] = std::vector<glm::mat4>{ xform };
	//					}
	//					else {
	//						trans->second.push_back(xform);
	//					}

	//				}
	//			}
	//		}

	//	}

	//	instancer->end();
	//}

	////Add Instances
	//for (auto instancer : DfxInstanceManager::getInstance()->intancers()) {

	//	std::unordered_map<size_t, DfxQRenderInstanceInfo> instanceData;

	//	instancer->begin();

	//	if (instancer->isEnabled()) {

	//		for (auto &gform : _transforms) {

	//			auto instItr = _instanceShapes.find(gform.first);
	//			auto trans = _transforms.find(gform.first);

	//			if (instItr != _instanceShapes.end() && trans != _transforms.end()) {

	//				if (trans->second.size() > 0) {

	//					auto xform = rootTransform() * trans->second[0];
	//					float *xPtr = &xform[0][0];
	//					std::vector<float> xfinalTransform =
	//					{
	//					xPtr[0], xPtr[1], xPtr[2], xPtr[3],
	//					xPtr[4], xPtr[5], xPtr[6], xPtr[7],
	//					xPtr[8], xPtr[9], xPtr[10], xPtr[11],
	//					xPtr[12], xPtr[13], xPtr[14], xPtr[15]
	//					};

	//					for (auto segment : instItr->second) {

	//						std::string itemName = "instance" + std::to_string(instID++);

	//						Properties prop01;
	//						prop01
	//							<< Property("scene.objects." + itemName + ".shape")(segment.shapeName)
	//							<< Property("scene.objects." + itemName + ".material")(segment.material)
	//							<< Property("scene.objects." + itemName + ".transformation")(xfinalTransform);

	//						scene->Parse(prop01);

	//						if (trans->second.size() > 1) {
	//							float *matArray = new float[(trans->second.size() - 1) * 16];

	//							for (long int i = 0; i < trans->second.size() - 1; i++) {
	//								long int os = i * 16;

	//								auto xf = rootTransform() * trans->second[i + 1];
	//								xPtr = &xf[0][0];
	//								matArray[os] = xPtr[0]; matArray[os + 1] = xPtr[1]; matArray[os + 2] = xPtr[2]; matArray[os + 3] = xPtr[3];
	//								matArray[os + 4] = xPtr[4]; matArray[os + 5] = xPtr[5]; matArray[os + 6] = xPtr[6]; matArray[os + 7] = xPtr[7];
	//								matArray[os + 8] = xPtr[8]; matArray[os + 9] = xPtr[9]; matArray[os + 10] = xPtr[10]; matArray[os + 11] = xPtr[11];
	//								matArray[os + 12] = xPtr[12]; matArray[os + 13] = xPtr[13]; matArray[os + 14] = xPtr[14]; matArray[os + 15] = xPtr[15];
	//							}

	//							scene->DuplicateObject(itemName, itemName + "_copy", trans->second.size() - 1, matArray);
	//							delete[] matArray;
	//						}
	//					}

	//				}
	//			}

	//		}

	//	}

	//	instancer->end();
	//}

	/*for (auto instancer : DfxInstanceManager::getInstance()->intancers()) {

		std::unordered_map<size_t, DfxQRenderInstanceInfo> instanceData;

		instancer->begin();

		if (instancer->isEnabled()) {

			for (size_t inx = 0; inx < instancer->instanceCount(); inx++) {

				auto node = instancer->instanceShape(inx);

				if (node != nullptr) {
					auto instItr = _instanceShapes.find(node->referencePath());

					if (instItr != _instanceShapes.end()) {

						glm::mat4 xform;

						if (instancer->useTransformMatrix(inx))
							xform = instancer->instanceTransform(inx);
						else
							xform = instancer->finalTransform(inx);

						xform = rootTransform() * xform;

						float *xPtr = &xform[0][0];
						std::vector<float> xfinalTransform =
						{
						xPtr[0], xPtr[1], xPtr[2], xPtr[3],
						xPtr[4], xPtr[5], xPtr[6], xPtr[7],
						xPtr[8], xPtr[9], xPtr[10], xPtr[11],
						xPtr[12], xPtr[13], xPtr[14], xPtr[15]
						};

						for (auto segment : instItr->second) {
							std::string itemName = "instance" + std::to_string(instID++);

							Properties prop01;
							prop01
								<< Property("scene.objects." + itemName + ".shape")(segment.shapeName)
								<< Property("scene.objects." + itemName + ".material")(segment.material)
								<< Property("scene.objects." + itemName + ".transformation")(xfinalTransform);

							scene->Parse(prop01);

						}

					}
				}
			}

		}

		instancer->end();
	}*/

	return true;
}

bool DfxOSPRayRenderSession::loadVolumes(deepsg::SgNode *branch) {

	deepsg::SgConditionalTraversalFunction renderFunc = [this](deepsg::SgNode *node, deepsg::SgStateInfo *info)-> deepsg::TraversalFlag {

		if (node->getType() == deepsg::SgNodeType::GRID) {

			if (!node->isRenderable()) {
				return deepsg::TraversalFlag::IGNORE_CHILDREN;
			}

			auto gnode = (DfxGridNode*)node;
			auto adapter = gnode->getGridAdapter();

			if (adapter->channelCount() <= 0)
				return deepsg::TraversalFlag::CONTINUE;

			if (adapter->channel(0)->valueType() != deepsg::SgGridChannel::FLOAT)
				return deepsg::TraversalFlag::CONTINUE;

			// Calculate Final Transform
			glm::mat4 finalTransform(1.f);

			if (!node->transformOverrides().empty()) {

				deepsg::SgTransformOverrideInfo ov_info;

				auto fps = DfxTimelineManager::getInstance()->fps();
				auto current_frame = DfxTimelineManager::getInstance()->currentFrame();
				double time = (double)current_frame / (double)fps;

				ov_info.fps = fps;
				ov_info.frame = current_frame;
				ov_info.time = time;
				ov_info.globalTransform = info->transformMatrix();

				finalTransform = rootTransform();
				for (auto xoverride : node->transformOverrides()) {
					finalTransform *= xoverride->evaluate(ov_info);
				}
			}
			else {
				finalTransform = info->transformMatrix();
			}

			int MAX_DIM = 0;

			auto dim = adapter->channel(0)->dimension();

			MAX_DIM = std::max(dim.x, std::max(dim.y, dim.z));

			auto XDIM = MAX_DIM;
			auto YDIM = MAX_DIM;
			auto ZDIM = MAX_DIM;

			int tRes = 500;
			// read the volume data file
			auto minMax = adapter->channel(0)->floatMinMax();

			float scale = 1.0 / (minMax.second - minMax.first);
			std::unique_ptr<float[]> pData(adapter->channel(0)->flattenGridFloat((XDIM*YDIM*ZDIM), XDIM, YDIM, ZDIM, [=](float value) {
				return (value - minMax.first) * scale;
			}));

			rkcommon::math::affine3f xfm = glmMat4ToAffine3(finalTransform);

			// Copy mesh data to OSPRay
			auto _vol = new OSPRayVolume;
			_vol->volume.reset(new ospray::cpp::Volume("structuredRegular"));
			_vol->volume->setParam("data", ospray::cpp::CopiedData(pData.get(), rkcommon::math::vec3ul(XDIM, YDIM, ZDIM)));
			_vol->volume->setParam("gridSpacing", rkcommon::math::vec3f(0.1f, 0.1f, 0.1f));
			_vol->volume->commit();

			std::vector<rkcommon::math::vec3f> colors;
			std::vector<float> opacities;
			
			colors.emplace_back(1.f, 0.f, 0.f);
			colors.emplace_back(0.f, 0.f, 1.f);
			colors.emplace_back(0.f, 1.f, 0.f);
			
			opacities.emplace_back(0.f);
			//opacities.emplace_back(0.7f);
			opacities.emplace_back(1.f);
		
			ospray::cpp::TransferFunction transferFunction("piecewiseLinear");

			transferFunction.setParam("color", ospray::cpp::CopiedData(colors));
			transferFunction.setParam("opacity", ospray::cpp::CopiedData(opacities));
			transferFunction.setParam("valueRange", rkcommon::math::vec2f(0.f, 1.f));
			transferFunction.commit();

			_vol->model.reset(new ospray::cpp::VolumetricModel(*_vol->volume));
			_vol->model->setParam("transferFunction", transferFunction);
			_vol->model->setParam("densityScale", 5.f);
			_vol->model->commit();

			_vol->group.reset(new ospray::cpp::Group());
			_vol->group->setParam("volume", ospray::cpp::CopiedData(*_vol->model));
			_vol->group->commit();

			_vol->instance.reset(new ospray::cpp::Instance(*_vol->group));
			//_vol->instance->setParam("transform", xfm);
			_vol->instance->commit();

			_ospVolumes.push_back(_vol);

			return deepsg::TraversalFlag::CONTINUE;
		}
		else {

			if (node->isRenderable()) {
				return deepsg::TraversalFlag::CONTINUE;
			}
			else {
				return deepsg::TraversalFlag::IGNORE_CHILDREN;
			}

		}
	};

	deepsg::SgRootNode::getInstance()->traverseSubSceneConditionally(branch, renderFunc, glm::mat4(1.f));

	return true;

	/*if (node->getType() == deepsg::SgNodeType::GRID) {

		DfxGridHandler ghandler;

		if (ghandler.fetch(node->referencePath())) {

			auto dao = ghandler.dataObject()->rawData<fl::DataObject>();
			auto cache = ghandler.volumeCache();

			float voxelPixelSize = dao->property("voxel_pixel_size")->data<float>();

			if (cache != nullptr) {
				glm::mat4 finalTransform(1.f);

				if (cache->isReady()) {
					if (!node->transformOverrides().empty()) {

						deepsg::SgTransformOverrideInfo ov_info;

						auto fps = DfxTimelineManager::getInstance()->fps();
						auto current_frame = DfxTimelineManager::getInstance()->currentFrame();
						double time = (double)current_frame / (double)fps;

						ov_info.fps = fps;
						ov_info.frame = current_frame;
						ov_info.time = time;
						ov_info.globalTransform = info->transformMatrix();

						for (auto xoverride : node->transformOverrides()) {
							finalTransform *= xoverride->evaluate(ov_info);
						}
					}
					else {
						finalTransform = info->transformMatrix();
					}

					glPushMatrix();
					glMultMatrixf(&finalTransform[0][0]);

					if (cache->isReady()) {
						cache->drawVolume(voxelPixelSize);
					}
				}
			}
		}
	}
*/
}

bool DfxOSPRayRenderSession::loadLights(deepsg::SgNode *branch) {

	deepsg::SgTraversalFunction lightFunc = [this](deepsg::SgNode *node, deepsg::SgStateInfo *info) {

		if (node->getType() == deepsg::SgNodeType::SHAPE && node->isRenderable()) {
			auto shapeNode = (deepsg::SgShapeNode*)node;

			glm::mat4 transformation = info->transformMatrix();
			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 pos;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(transformation, scale, rotation, pos, skew, perspective);

			glm::vec3 rot = node->rotation();

			if (node->uniqueName() == DfxOSPRaySunSkyLightWidget::getUniqueName()) {
				DfxOSPRaySunSkyLightHandler handler;
				auto rc = handler.fetch(node->referencePath());

				if (rc) {

					auto dao = handler.dataObject()->rawData<fl::DataObject>();

					auto turbidity = dao->property("turbidity")->data<float>();
					auto albedo = dao->property("albedo")->data<float>();
					auto horizon_extension = dao->property("horizon_extension")->data<float>();
					auto intensity = dao->property("intensity")->data<float>();

					glm::vec3 scale;
					glm::quat rotation;
					glm::vec3 pos;
					glm::vec3 skew;
					glm::vec4 perspective;
					glm::decompose(info->transformMatrix(), scale, rotation, pos, skew, perspective);

					auto dir = glm::normalize(glm::vec3(glm::toMat4(glm::conjugate(rotation)) * glm::vec4(0.f, 1.f, 0.f, 1.f)));

					auto _light = new OSPRayLight;
					_light->light.reset(new ospray::cpp::Light("sunSky"));

					_light->light->setParam("intensity", intensity);
					_light->light->setParam("turbidity", turbidity);
					_light->light->setParam("albedo", albedo);
					_light->light->setParam("horizonExtension", horizon_extension);
					_light->light->setParam("direction", rkcommon::math::vec3f(dir.x, dir.y, dir.z));
					_light->light->commit();
					
					_ospLights.push_back(_light);

					//auto itrExLight = existingLights.find(node->referencePath());

					//if (itrExLight == existingLights.end()) {
					//	existingLights[node->referencePath()] = "";

					//	auto xform = handler.xform()->rawData<fl::DataObject>();;

					//	addLightObserver(dao->property("color_r"));
					//	addLightObserver(dao->property("gain"));
					//	addLightObserver(dao->property("power"));
					//	addLightObserver(dao->property("efficacy"));
					//	/*addLightObserver(xform->property("x_pos"));
					//		addLightObserver(xform->property("y_pos"));
					//		addLightObserver(xform->property("z_pos"));*/
					//}

				}
			}
		
		}
	};

	deepsg::SgRootNode::getInstance()->traverseScene(lightFunc, glm::mat4(1.f));
	
	
	return true;
}

bool DfxOSPRayRenderSession::loadCamera(deepsg::SgNode *branch) {

	glm::vec3 origin(0.f, 0.f, -10.05f);
	glm::vec3 target(0.f, 0.f, .0f);
	glm::vec3 up(0.f, 0.f, 1.0f);

	bool use_dof = false;
	bool architectural = false;
	float fov = 0.f;
	float focalDistance = 0.f;
	float lensSize = 0.f;
	float lensRadius = 0.f;
	int height = 0.f;
	int width = 0.f;
	std::string helper;

	deepsg::SgTraversalFunction renderFunc = [this, &architectural, &height, &width, &helper, &lensRadius, &focalDistance, &lensSize, &fov, &use_dof, &up, &origin, &target](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
		if (node->getType() == deepsg::SgNodeType::SHAPE) {
			auto shapeNode = (deepsg::SgShapeNode*)node;

			auto referencePath = shapeNode->referencePath();

			if (shapeNode->referencePath() == cameraPath) {
				if (shapeNode->shapeType() == DfxItemWidgetType::CAMERA) {
					DfxOSPRayCameraHandler handler;

					glm::vec3 scale;
					glm::quat rotation;
					glm::vec3 pos;
					glm::vec3 skew;
					glm::vec4 perspective;
					glm::decompose(info->transformMatrix(), scale, rotation, pos, skew, perspective);

					auto dir = glm::normalize(glm::vec3(glm::toMat4(glm::conjugate(rotation)) * glm::vec4(0.f, 1.f, 0.f, 1.f)));

					origin = glm::vec3(info->transformMatrix() * glm::vec4(0.f, 0.f, 0.f, 1.f));
					target = glm::normalize(glm::vec3(glm::toMat4(glm::conjugate(rotation)) * glm::vec4(0.f, 0.f, -1.f, 1.f)));
					up = glm::normalize(glm::vec3(glm::toMat4(glm::conjugate(rotation)) * glm::vec4(0.f, 1.f, 0.f, 1.f)));

					if (handler.fetch(referencePath)) {
						auto dao = handler.dataObject()->rawData<fl::DataObject>();
	
						use_dof = dao->property("use_dof")->data<bool>();
						architectural = dao->property("architectural")->data<bool>();
						fov = dao->property("field_of_view")->data<float>();
						focalDistance = dao->property("focal_distance")->data<float>();
						lensSize = dao->property("aperture_radius")->data<float>();
						helper = dao->property("dof_helper")->data<std::string>();
						width = dao->property("width")->data<int>();
						height = dao->property("height")->data<int>();

						auto focalLength = fovToFocalLength(fov, lensSize);
						// lensRadius = (focalLength / 1000.f) / (2.f * fstop);

						if (height > 0 && width > 0) {
							camera.reset( new ospray::cpp::Camera("perspective"));
							camera->setParam("aspect", width / (float)height);
							camera->setParam("fovy", fov);
							camera->setParam("position", rkcommon::math::vec3f(origin.x, origin.y, origin.z));
							camera->setParam("direction", rkcommon::math::vec3f(target.x, target.y, target.z));
							camera->setParam("up", rkcommon::math::vec3f(up.x, up.y, up.z));
							//ospSetVec3f(camera, "position", 0.f, 0.f, 7.f);
							//ospSetVec3f(camera, "direction", 0.f, 0.f, -1.f);
							//ospSetVec3f(camera, "up", 0.f, 1.f, 0.f);
							camera->commit();
							
							outputWidth = width;
							outputHeight = height;
						}
						else {
							outputWidth = -1;
							outputHeight = -1;
						}

					}
				}
			}
		}
	};

	deepsg::SgTraversalFunction helperFunc = [this, &helper, &origin, &focalDistance](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
		if (node->referencePath() == helper) {
			auto helper_pos = glm::vec3(info->transformMatrix() * glm::vec4(0.f, 0.f, 0.f, 1.f));

			glm::vec3 pos = helper_pos;
			focalDistance = std::abs(glm::distance(origin, pos));
		}
	};

	deepsg::SgRootNode::getInstance()->traverseSubScene(branch, renderFunc, glm::mat4(1.f));
	deepsg::SgRootNode::getInstance()->traverseSubScene(branch, helperFunc, glm::mat4(1.f));


	//if (use_dof) {
	//	props
	//		<< Property("scene.camera.focaldistance")(focalDistance)
	//		<< Property("scene.camera.lensradius")(lensRadius);
	//}

	return true;
}

bool DfxOSPRayRenderSession::loadWorld() {
	if (!isInitialized())return false;

	if (_ospObjects.empty()) {
		DfxGlobal::console::println("OSPRay ERROR: No objects to render");
		return false;
	}

	if (_ospLights.empty()) {
		DfxGlobal::console::println("OSPRay ERROR: No lights found");
		return false;
	}

	std::vector<ospray::cpp::Instance> _ospInstanceHandles;
	std::vector<ospray::cpp::Light>_ospLightHandles;

	//_ospInstanceHandles.resize(_ospObjects.size() + _ospVolumes.size());
	_ospLightHandles.resize(_ospLights.size());

	for (int i = 0; i < _ospObjects.size(); i++)
		_ospInstanceHandles.push_back(*(_ospObjects[i]->instance));

	for (int i = 0; i < _ospVolumes.size(); i++)
		_ospInstanceHandles.push_back(*(_ospVolumes[i]->instance));
	
		
	for (int i = 0; i < _ospLights.size(); i++)
		_ospLightHandles[i] = *(_ospLights[i]->light);

	world->setParam("instance", ospray::cpp::CopiedData(_ospInstanceHandles));
	world->setParam("light", ospray::cpp::CopiedData(_ospLightHandles));
	world->commit();

	auto worldBounds = world->getBounds<rkcommon::math::box3f>();
	//printf("world bounds: ({%f, %f, %f}, {%f, %f, %f}\n\n",
	//	worldBounds.lower[0],
	//	worldBounds.lower[1],
	//	worldBounds.lower[2],
	//	worldBounds.upper[0],
	//	worldBounds.upper[1],
	//	worldBounds.upper[2]);

	return true;
}

/// LuxCore Specific functions
void DfxOSPRayRenderSession::startRenderSession() {
	//auto space = DfxSpace::getInstance();

	//try {
	//	render_session->Start();
	//	newBufferRefreshTimer(120);

	//	DfxLxRenderHandler handler;

	//	rtHaltTime = 10000;
	//	if (handler.fetch(_luxcoreRenderInfoPath)) {
	//		DfxLxCameraHandler camHandler;
	//		std::string haltStr = _luxcoreRenderInfoPath + "/luxcore_halt_conditions";
	//		fl::DataObject *haltObj;

	//		auto haltAny = space->findAbsolute(haltStr);

	//		if (haltAny == nullptr) {
	//			DfxGlobal::console::println("LuxCore RT Path Engine Config Invalid path: " + haltStr);
	//			return;
	//		}
	//		else {
	//			haltObj = haltAny->rawData<fl::DataObject>();
	//		}

	//		rtHaltTime = (float)haltObj->property("ipr_timed_samples")->data<int>() * 1000;
	//	}

	//	newHaltTimer(rtHaltTime);

	//	_sessionUpdateListener = new DfxRenderSessionUpdateListener(this);
	//}
	//catch (std::runtime_error &err) {
	//	LC_LOG("LuxCore Camera RUNTIME ERROR: " << err.what());
	//	//return false;
	//}
	//catch (std::exception &err) {
	//	LC_LOG("LuxCore Camera ERROR: " << err.what());
	//	//return false;
	//}
}

void DfxOSPRayRenderSession::stopRenderSession() {
	/*if (rtBufferRefreshTimer != nullptr) {
		rtBufferRefreshTimer->stop();
		delete rtBufferRefreshTimer;
		rtBufferRefreshTimer = nullptr;
	}

	if (updateBufferFuture.isRunning()) {
		updateBufferFuture.cancel();
		updateBufferFuture.waitForFinished();
		_bufferUpdating = false;
	}

	delete _sessionUpdateListener;
	_sessionUpdateListener = nullptr;

	render_session->Stop();*/
}

bool DfxOSPRayRenderSession::isStarted() {
	return false;
}

void DfxOSPRayRenderSession::setCameraPath(std::string path) {
	cameraPath = path;
}
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

#include <QMessageBox>
#include <generator/BoxMesh.hpp>
#include "DfxLuxCoreRenderSession.h"
#include "DfxLxRenderHandler.h"
#include "DfxRenderOutput.h"
#include "DfxMaterialManager.h"

bool DfxLuxCoreRenderSession::_initialized = false;

using namespace luxrays;

DfxLuxCoreRenderSession::DfxLuxCoreRenderSession() {
	scene = nullptr;
	render_config = nullptr;
	render_session = nullptr;

	_renderOutput = nullptr;

	_renderEngine = PATH;
	activeBranch = nullptr;

	cameraPath = "";
	_luxcoreRenderInfoPath = "";

	rtHaltTime = 30;
	rtHaltTimer = nullptr;

	_refreshInterval = 0;
	_haltTime = 0;
	_haltSpp = 0;

	_denoisingEnabled = false;
	_denoisingBuffer  = 0;

	_forceStop = false;

	_bufferUpdating = false;

	_sessionUpdateListener = nullptr;

	_materialDispatcher = nullptr;
}

DfxLuxCoreRenderSession::~DfxLuxCoreRenderSession() {
	
	if (_sessionUpdateListener != nullptr) {
		delete _sessionUpdateListener;
		_sessionUpdateListener = nullptr;
	}

	forceStop();
	clear();

	if (_materialDispatcher != nullptr) delete _materialDispatcher;
}

void DfxLuxCoreRenderSession::forceStop() {
	_forceStop = true;
}

std::string DfxLuxCoreRenderSession::luxcoreRenderInfoPath() {
	return _luxcoreRenderInfoPath;
}

void DfxLuxCoreRenderSession::setLuxcoreRenderInfoPath(std::string path) {
	_luxcoreRenderInfoPath = path;
}

void DfxLuxCoreRenderSession::enableInteractivePreview() {
	_renderEngine = DfxLuxCoreRenderSession::RT;
}

bool DfxLuxCoreRenderSession::init() {
	
	bool _printInfo = false;

	auto anyPrintInfo = DfxSpace::getInstance()->findAbsolute(_luxcoreRenderInfoPath + "/luxcore_general/print_render_info");
	if (anyPrintInfo != nullptr) _printInfo = anyPrintInfo->data<bool>();
	
	try {

		if (!_initialized) {

			if (_printInfo) {
				luxcore::Init(luxcore::LuxCore_LogHandler);
			}
			else {
				luxcore::Init();
			}

			_initialized = true;
		}

		scene = luxcore::Scene::Create();			
	}
	catch (std::runtime_error &err) {
		LC_LOG("LuxCore Init RUNTIME ERROR: " << err.what());
		return EXIT_FAILURE;
	}
	catch (std::exception &err) {
		LC_LOG("LuxCore Init ERROR: " << err.what());
		return EXIT_FAILURE;
	}

	return _initialized;
}

void DfxLuxCoreRenderSession::setRenderOutput(DfxRenderOutput *output) {
	_renderOutput = output;
}

DfxRenderOutput *DfxLuxCoreRenderSession::renderOutput() {
	return _renderOutput;
}

glm::mat4 DfxLuxCoreRenderSession::rootTransform() {

	return glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
}

glm::mat4 DfxLuxCoreRenderSession::transform4x4(deepsg::SgNode *node) {

	glm::mat4 transMat = glm::translate(glm::mat4(1.f), glm::vec3(node->posX(), -node->posZ(), node->posY()));

	transMat *= rotationMatrix(node);

	transMat *= glm::scale(glm::mat4(1.f), glm::vec3(node->sclX(), node->sclZ(), node->sclY()));

	return transMat;
}

glm::mat4 DfxLuxCoreRenderSession::rotationMatrix(deepsg::SgNode *node) {

	unsigned int _rotationType = node->rotationType();

	glm::vec3 rotation(node->rotX(), node->rotY(), node->rotZ());

	switch (_rotationType) {
	case deepsg::XYZ:
	{
		auto transMat = glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, .0f, .0f));
		transMat *= glm::rotate(glm::mat4(1.f), glm::radians(-rotation.z),	  glm::vec3(0.f, 1.f, .0f));
		transMat *= glm::rotate(glm::mat4(1.f), glm::radians(rotation.y),	  glm::vec3(0.f, .0f, 1.f));

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

void DfxLuxCoreRenderSession::_traverseScene00(deepsg::SgNode *node, deepsg::SgTraversalFunction travFunc, deepsg::SgStateInfo *info, unsigned int layer) {

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

void DfxLuxCoreRenderSession::traverseSubScene(deepsg::SgNode *node, deepsg::SgTraversalFunction travFunc, glm::mat4 root_transform) {

	deepsg::SgRootNode::StateInfo *_info = new deepsg::SgRootNode::StateInfo;
	_info->setParent(node);
	_info->setMatrix(root_transform);

	_traverseScene00(node, travFunc, _info);
}

void DfxLuxCoreRenderSession::addLightObserver(fl::Any *any) {
	DfxRenderSessionLightObserver *obs = new DfxRenderSessionLightObserver;
	obs->_interface = this;

	observerList.push_back(obs);
	any->addObserver(obs);
}

void DfxLuxCoreRenderSession::addCameraObserver(fl::Any *any) {
	DfxRenderSessionCameraObserver *obs = new DfxRenderSessionCameraObserver;
	obs->_interface = this;

	cameraObservers.push_back(obs);
	any->addObserver(obs);
}

void DfxLuxCoreRenderSession::setRtHaltTime(int ht) {
	rtHaltTime = ht;
}

void DfxLuxCoreRenderSession::newHaltTimer(int time) {
	if (!checkSessionIsReady("LuxCore Halt Timer")) return;

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
	}
}

void DfxLuxCoreRenderSession::newBufferRefreshTimer(int time) {
	if (!checkSessionIsReady("LuxCore Refresh Buffer Timer")) return;

	rtBufferRefreshTimer = new QTimer;
	auto session = this;
	QObject::connect(rtBufferRefreshTimer, &QTimer::timeout, [this, session]() {
		if (_bufferUpdating) return;
		else _bufferUpdating = true;

		updateBufferFuture = QtConcurrent::run(session, &DfxLuxCoreRenderSession::_updateOutput);
		while (updateBufferFuture.isRunning()) QApplication::processEvents();

		_bufferUpdating = false;
	});

	rtBufferRefreshTimer->start(100);
}

void DfxLuxCoreRenderSession::printRenderingStats(luxrays::Properties &stats) {
	char buf[512];

	const double elapsedTime = stats.Get("stats.renderengine.time").Get<double>();
	const unsigned int pass = stats.Get("stats.renderengine.pass").Get<unsigned int>();

	// Print some information about the rendering progress
	sprintf(buf, "[Elapsed time: %3d/%dsec][Samples %4d/%d][Avg. samples/sec % 3.2fM on %.1fK tris]",
		int(elapsedTime), int(_haltTime), pass, _haltSpp,
		stats.Get("stats.renderengine.total.samplesec").Get<double>() / 1000000.0,
		stats.Get("stats.dataset.trianglecount").Get<double>() / 1000.0);

	LC_LOG(buf);
}

std::pair<int, bool> DfxLuxCoreRenderSession::setupImagePipline(luxrays::Properties &properties, fl::DataObject *pipelineObj, fl::DataObject *denoiseObj) {
	int line = 0;
	if(denoiseObj != nullptr) 
		if (denoiseObj->property("enable")->data<bool>()) line = 1;
		else line = -1;

	int piplineCnt = 0;
	bool enablePipeline = false;
	bool enableDenoising = false;
	bool addNOP = true;
	if (denoiseObj != nullptr) {
		if (denoiseObj->property("enable")->data<bool>()) {

			properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt) + ".type")("INTEL_OIDN");
			properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt++) + ".oidnmemory")(denoiseObj->property("oidn_max_memory")->data<int>());

			enablePipeline = true;
			enableDenoising = true;

			if (addNOP) {
				properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt++) + ".type")("NOP");
				addNOP = false;
			}
		}
	}

	if (line == 0 || enableDenoising) {
		if (pipelineObj->property("tonemap_enable")->data<bool>()) {

			if (addNOP) {
				properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt++) + ".type")("NOP");
				addNOP = false;
			}

			if (pipelineObj->property("tonemap_lin_auto")->data<bool>()) {
				properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt++) + ".type")("TONEMAP_AUTOLINEAR");
			}

			auto tonemapper = pipelineObj->property("tonemap_type")->data<int>();

			auto linScale = pipelineObj->property("tonemap_lin_gain")->data<float>();

			auto tonemap_rein_pre = pipelineObj->property("tonemap_rein_pre")->data<float>();
			auto tonemap_rein_post = pipelineObj->property("tonemap_rein_post")->data<float>();
			auto tonemap_rein_burn = pipelineObj->property("tonemap_rein_burn")->data<float>();

			auto tonemap_camera_fstop = pipelineObj->property("tonemap_camera_fstop")->data<float>();
			auto tonemap_camera_shutter = pipelineObj->property("tonemap_camera_shutter")->data<float>();
			auto tonemap_camera_iso = pipelineObj->property("tonemap_camera_iso")->data<float>();

			if (tonemapper == 0) {
				properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt) + ".type")("TONEMAP_LINEAR");
				properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt++) + ".scale")(linScale);
			}
			else if (tonemapper == 1) {
				properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt) + ".type")("TONEMAP_LUXLINEAR");
				properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt) + ".fstop")(tonemap_camera_fstop);
				properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt) + ".exposure")(tonemap_camera_shutter);
				properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt++) + ".sensitivity")(tonemap_camera_iso);
			}
			else {
				properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt) + ".type")("TONEMAP_REINHARD02");
				properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt) + ".prescale")(tonemap_rein_pre);
				properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt) + ".postscale")(tonemap_rein_post);
				properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt++) + ".burn")(tonemap_rein_burn);
			}

			enablePipeline = true;
		}
	}

	if (pipelineObj->property("bloom_enable")->data<bool>()) {
		if (addNOP) {
			properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt++) + ".type")("NOP");
			addNOP = false;
		}

		auto radius = pipelineObj->property("bloom_radius")->data<float>();
		auto strength = pipelineObj->property("bloom_strength")->data<float>();

		properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt) + ".type")("BLOOM");
		properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt) + ".radius")(radius);
		properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt++) + ".weight")(strength);

		enablePipeline = true;
	}
	
	if (enablePipeline) {
		properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt) + ".type")("GAMMA_CORRECTION");
		properties << Property("film.imagepipelines." + std::to_string(line) + "." + std::to_string(piplineCnt++) + ".value")(2.2000000000000002);
	}

	return std::pair<int, bool>(line, enablePipeline);
}

void DfxLuxCoreRenderSession::setupAovPipeline(int index, luxrays::Properties &properties, std::string channel) {

	properties << Property("film.imagepipelines." + std::to_string(index) + ".0.type")("OUTPUT_SWITCHER");
	properties << Property("film.imagepipelines." + std::to_string(index) + ".0.channel")(channel);
	properties << Property("film.imagepipelines." + std::to_string(index) + ".1.type")("NOP");
	properties << Property("film.imagepipelines." + std::to_string(index) + ".2.type")("TONEMAP_LINEAR");
	properties << Property("film.imagepipelines." + std::to_string(index) + ".2.scale")(0.77999997138977051);
	properties << Property("film.imagepipelines." + std::to_string(index) + ".3.type")("GAMMA_CORRECTION");
	properties << Property("film.imagepipelines." + std::to_string(index) + ".3.scale")(2.2000000000000002);

	properties << Property("film.imagepipelines." + std::to_string(index) + ".radiancescales.0.globalscale")(1);
	properties << Property("film.imagepipelines." + std::to_string(index) + ".radiancescales.0.rgbscale")(1, 1, 1);

}

bool DfxLuxCoreRenderSession::setupRenderConfigBuffer(luxrays::Properties &properties) {
	auto space = DfxSpace::getInstance();

	_denoisingEnabled = false;
	_denoisingBuffer  = true;

	_renderOutput->clear();
	_renderOutput->notifyBuffersUpdated();

	std::string aovStr = _luxcoreRenderInfoPath + "/luxcore_aovs";
	std::string pipelinetr = _luxcoreRenderInfoPath + "/luxcore_image_pipeline";
	std::string denoiseStr = _luxcoreRenderInfoPath + "/luxcore_denoising";

	fl::DataObject *aovObj;
	fl::DataObject *pipelineObj;
	fl::DataObject *denoiseObj;

	auto aovAny = space->findAbsolute(aovStr);
	auto pipelineAny = space->findAbsolute(pipelinetr);
	auto denoiseAny = space->findAbsolute(denoiseStr);

	if (aovAny == nullptr) {
		DfxGlobal::console::println("LuxCore Path Engine Config Invalid path: " + aovStr);
		return false;
	}
	else {
		aovObj = aovAny->rawData<fl::DataObject>();
	}

	if (pipelineAny == nullptr) {
		DfxGlobal::console::println("LuxCore Path Engine Config Invalid path: " + pipelinetr);
		return false;
	}
	else {
		pipelineObj = pipelineAny->rawData<fl::DataObject>();
	}

	if (denoiseAny == nullptr) {
		DfxGlobal::console::println("LuxCore Path Engine Config Invalid path: " + denoiseStr);
		return false;
	}
	else {
		denoiseObj = denoiseAny->rawData<fl::DataObject>();
	}

	int aovCounter = 0;
	int imagePipelineCounter = 0;
	for (auto aovProp : aovObj->propertiesOriginalOrder()) {
		
		std::string aovName = aovProp->name();
		bool useAov = aovProp->data<bool>();

		if (useAov) {

			if (aovName == "rgb_imagepipeline") {
				auto outputIndex = aovCounter;
				aovCounter++;
				
				bool nop = false;
				bool usePipeline = false;

				auto rc00 = setupImagePipline(properties, pipelineObj, nullptr);
				auto rc01 = setupImagePipline(properties, pipelineObj, denoiseObj);

				usePipeline = rc00.second;
				_denoisingEnabled = rc01.second;
				
				imagePipelineCounter = rc01.first;
				if (imagePipelineCounter == -1) imagePipelineCounter = 0;

				auto renderBuffer = new DfxRenderBuffer;
				renderBuffer->setBufferName("RGB Image Pipeline");
				renderBuffer->setIndex(outputIndex);
				_renderOutput->addBuffer(renderBuffer);

				auto prefix = std::string("film.outputs.") + std::to_string(outputIndex);
				properties << Property(prefix + ".type")("RGB_IMAGEPIPELINE");
				if(usePipeline) properties << Property(prefix + ".index")(0);

				if (_denoisingEnabled) {
					auto albedoIndex = aovCounter++;
					auto avgIndex = aovCounter++;
					auto _denoisingBuffer = aovCounter++;

					properties
						<< Property("film.outputs." + std::to_string(albedoIndex) + ".type")("ALBEDO")
						<< Property("film.outputs." + std::to_string(albedoIndex) + ".filename")("denoise_ALBEDO.exr")
						<< Property("film.outputs." + std::to_string(avgIndex) + ".type")("AVG_SHADING_NORMAL")
						<< Property("film.outputs." + std::to_string(avgIndex) + ".filename")("denoise_AVG_SHADING_NORMAL.exr")
						<< Property("film.outputs." + std::to_string(_denoisingBuffer) + ".type")("RGB_IMAGEPIPELINE")
						<< Property("film.outputs." + std::to_string(_denoisingBuffer) + ".index")(1)
						<< Property("film.outputs." + std::to_string(_denoisingBuffer) + ".filename")("RGB_IMAGEPIPELINE_1.png");
				}
			}
			else if (aovName == "rgb") {
				std::stringstream ss;
				ss << "film.outputs." << aovCounter << ".type";
				properties << Property(ss.str())("RGB");
				properties << Property(std::string("film.outputs.") + std::to_string(aovCounter) + ".filename")("rgb.exr");

				auto renderBuffer = new DfxRenderBuffer;
				renderBuffer->setBufferName("RGB");

				_renderOutput->addBuffer(renderBuffer);

				aovCounter++;
			}
			else if (aovName == "alpha") {
				std::stringstream ss;
				ss << "film.outputs." << aovCounter << ".type";
				properties << Property(ss.str())("ALPHA");
				properties << Property(std::string("film.outputs.") + std::to_string(aovCounter) + ".filename")("alpha.exr");

				auto renderBuffer = new DfxRenderBuffer;
				renderBuffer->setBufferName("Alpha");

				_renderOutput->addBuffer(renderBuffer);

				aovCounter++;
			}
			else if (aovName == "depth") {
				std::stringstream ss;
				ss << "film.outputs." << aovCounter << ".type";
				properties << Property(ss.str())("DEPTH");
				properties << Property(std::string("film.outputs.") + std::to_string(aovCounter) + ".filename")("depth.exr");

				auto renderBuffer = new DfxRenderBuffer;
				renderBuffer->setBufferName("Depth");

				_renderOutput->addBuffer(renderBuffer);

				aovCounter++;
			}
			else if (aovName == "emission") {
				std::stringstream ss;
				ss << "film.outputs." << aovCounter << ".type";
				properties << Property(ss.str())("EMISSION");
				properties << Property(std::string("film.outputs.") + std::to_string(aovCounter) + ".filename")("emission.exr");

				auto renderBuffer = new DfxRenderBuffer;
				renderBuffer->setBufferName("Emission");

				_renderOutput->addBuffer(renderBuffer);

				aovCounter++;
			}
			else if (aovName == "direct_diffuse"){
				std::stringstream ss;
				ss << "film.outputs." << aovCounter << ".type";
				properties << Property(ss.str())("DIRECT_DIFFUSE");
				properties << Property(std::string("film.outputs.") + std::to_string(aovCounter) + ".filename")("DIRECT_DIFFUSE.exr");

				auto renderBuffer = new DfxRenderBuffer;
				renderBuffer->setBufferName("Direct Diffuse");

				_renderOutput->addBuffer(renderBuffer);

				aovCounter++;
			}
			else if (aovName == "direct_glossy") {
				std::stringstream ss;
				ss << "film.outputs." << aovCounter << ".type";
				properties << Property(ss.str())("DIRECT_GLOSSY");
				properties << Property(std::string("film.outputs.") + std::to_string(aovCounter) + ".filename")("direct_glossy.exr");

				// setupAovPipeline(++imagePipelineCounter, properties, "DIRECT_GLOSSY");
				/*properties << Property(std::string("film.outputs.") + std::to_string(aovCounter) + ".index")(imagePipelineCounter);
				imagePipelineCounter++;*/

				auto renderBuffer = new DfxRenderBuffer;
				renderBuffer->setBufferName("Direct Glossy");

				_renderOutput->addBuffer(renderBuffer);

				aovCounter++;
			}
			else if (aovName == "indirect_diffuse") {
				std::stringstream ss;
				ss << "film.outputs." << aovCounter << ".type";
				properties << Property(ss.str())("INDIRECT_DIFFUSE");
				properties << Property(std::string("film.outputs.") + std::to_string(aovCounter) + ".filename")("indirect_diffuse.exr");

				// setupAovPipeline(++imagePipelineCounter, properties, "INDIRECT_DIFFUSE");

				auto renderBuffer = new DfxRenderBuffer;
				renderBuffer->setBufferName("Indirect Diffuse");

				_renderOutput->addBuffer(renderBuffer);

				aovCounter++;
			}
			else if (aovName == "indirect_specular") {
				std::stringstream ss;
				ss << "film.outputs." << aovCounter << ".type";
				properties << Property(ss.str())("INDIRECT_SPECULAR");
				properties << Property(std::string("film.outputs.") + std::to_string(aovCounter) + ".filename")("DIRECT_DIFFUSE.exr");

				auto renderBuffer = new DfxRenderBuffer;
				renderBuffer->setBufferName("Indirect Specular");

				_renderOutput->addBuffer(renderBuffer);

				aovCounter++;
			}
			else if (aovName == "indirect_glossy") {
				std::stringstream ss;
				ss << "film.outputs." << aovCounter << ".type";
				properties << Property(ss.str())("INDIRECT_GLOSSY");
				properties << Property(std::string("film.outputs.") + std::to_string(aovCounter) + ".filename")("DIRECT_DIFFUSE.exr");

				auto renderBuffer = new DfxRenderBuffer;
				renderBuffer->setBufferName("Indirect Glossy");

				_renderOutput->addBuffer(renderBuffer);

				aovCounter++;
			}
			else if (aovName == "irradiance") {
				std::stringstream ss;
				ss << "film.outputs." << aovCounter << ".type";
				properties << Property(ss.str())("IRRADIANCE");
				properties << Property(std::string("film.outputs.") + std::to_string(aovCounter) + ".filename")("DIRECT_DIFFUSE.exr");

				auto renderBuffer = new DfxRenderBuffer;
				renderBuffer->setBufferName("Irradiance");

				_renderOutput->addBuffer(renderBuffer);

				aovCounter++;
			}
					
		}
	}

	_renderOutput->notifyBuffersUpdated();

	return true;
}

luxcore::RenderConfig *DfxLuxCoreRenderSession::pathRenderConfig(bool &usingOpencl, bool realtime) {
	if (!checkSceneIsReady("LuxCore Session Prep")) return nullptr;

	DfxLxRenderHandler handler;
	
	if (handler.fetch(_luxcoreRenderInfoPath)) {
		DfxLxCameraHandler camHandler;

		std::string luxStr = _luxcoreRenderInfoPath;
		std::string samplerStr = _luxcoreRenderInfoPath + "/luxcore_sampling";
		std::string pathStr = _luxcoreRenderInfoPath + "/luxcore_path";
		std::string pixFilterStr = _luxcoreRenderInfoPath + "/luxcore_pixel_filter";
		std::string haltStr = _luxcoreRenderInfoPath + "/luxcore_halt_conditions";
		std::string refreshStr = _luxcoreRenderInfoPath + "/luxcore_refresh_interval";
		std::string photonGiStr = _luxcoreRenderInfoPath + "/luxcore_photon_gi_cache";
		std::string dlsStr = _luxcoreRenderInfoPath + "/luxcore_direct_light_cache";

		auto space = DfxSpace::getInstance();

		bool useCpu = false;
		bool useOpencl = false;

		fl::DataObject *luxObj;
		fl::DataObject *samplerObj;
		fl::DataObject *pathObj;
		fl::DataObject *pixFilterObj;
		fl::DataObject *haltObj;
		fl::DataObject *refreshObj;
		fl::DataObject *photonGiObj;
		fl::DataObject *dlsObj;

		auto luxAny = space->findAbsolute(luxStr);
		auto samplerAny  = space->findAbsolute(samplerStr);
		auto pathAny = space->findAbsolute(pathStr);
		auto pixFilterAny = space->findAbsolute(pixFilterStr);
		auto haltAny = space->findAbsolute(haltStr);
		auto refreshAny = space->findAbsolute(refreshStr);
		auto photonGiAny = space->findAbsolute(photonGiStr);
		auto dlsAny = space->findAbsolute(dlsStr);

		if (luxAny == nullptr) {
			DfxGlobal::console::println("LuxCore Path Engine Config Invalid path: " + luxStr);
			return nullptr;
		}
		else {
			luxObj = luxAny->rawData<fl::DataObject>();
		}

		if (samplerAny == nullptr) {
			DfxGlobal::console::println("LuxCore Path Engine Config Invalid path: " + samplerStr);
			return nullptr;
		}
		else {
			samplerObj = samplerAny->rawData<fl::DataObject>();
		}

		if (pathAny == nullptr) {
			DfxGlobal::console::println("LuxCore Path Engine Config Invalid path: " + pathStr);
			return nullptr;
		}
		else {
			pathObj = pathAny->rawData<fl::DataObject>();
		}

		if (pixFilterAny == nullptr) {
			DfxGlobal::console::println("LuxCore Path Engine Config Invalid path: " + pixFilterStr);
			return nullptr;
		}
		else {
			pixFilterObj = pixFilterAny->rawData<fl::DataObject>();
		}

		if (haltAny == nullptr) {
			DfxGlobal::console::println("LuxCore Path Engine Config Invalid path: " + haltStr);
			return nullptr;
		}
		else {
			haltObj = haltAny->rawData<fl::DataObject>();
		}

		if (refreshAny == nullptr) {
			DfxGlobal::console::println("LuxCore Path Engine Config Invalid path: " + refreshStr);
			return nullptr;
		}
		else {
			refreshObj = refreshAny->rawData<fl::DataObject>();
		}

		if (photonGiAny == nullptr) {
			DfxGlobal::console::println("LuxCore Path Engine Config Invalid path: " + photonGiStr);
			return nullptr;
		}
		else {
			photonGiObj = photonGiAny->rawData<fl::DataObject>();
		}

		if (dlsAny == nullptr) {
			DfxGlobal::console::println("LuxCore Path Engine Config Invalid path: " + dlsStr);
			return nullptr;
		}
		else {
			dlsObj = dlsAny->rawData<fl::DataObject>();
		}

		// Retrieve Camera settings
		if (cameraPath.empty()) {
			LC_LOG("LuxCore Camera RUNTIME ERROR: Empty camera path.");
			return false;
		}
		else {
			auto rc = camHandler.fetch(cameraPath);

			if (!rc) {
				LC_LOG("LuxCore Camera RUNTIME ERROR: Failed to fetch handle.");
				return false;
			}
		}

		/// Path Engine Properties
		int deviceID = luxObj->property("device")->data<int>();
		std::string device = "";

		if (!realtime) {
			if (deviceID == DfxLuxCoreRenderNode::Device::CPU) {
				useCpu = true;
				device = "PATHCPU";
			}
			else if (deviceID == DfxLuxCoreRenderNode::Device::OPENCL) {
				useOpencl = true;
				device = "PATHOCL";
			}
		}
		else {
			useCpu = true;
			useOpencl = false;
			device = "RTPATHCPU";
			/*if (deviceID == DfxLuxCoreRenderNode::Device::CPU) {
				useCpu = true;
				useOpencl = false;
				device = "RTPATHCPU";
			}
			else if (deviceID == DfxLuxCoreRenderNode::Device::OPENCL) {
				useOpencl = true;
				useCpu = false;
				device = "RTPATHOCL";
			}*/
		}
		
		// Sampler
		int samplerID = samplerObj->property("sampler")->data<unsigned int>();
		std::string sampler = ""; 

		if (samplerID == DfxLuxCoreRenderNode::Sampler::SOBOL) {
			sampler = "SOBOL";
		}
		else if (samplerID == DfxLuxCoreRenderNode::Sampler::METROPOLIS) {
			sampler = "METROPOLIS";
		}
		else if (samplerID == DfxLuxCoreRenderNode::Sampler::RANDOM) {
			sampler = "RANDOM";
		}

		float sobol_adaptive_strength = samplerObj->property("sobol_adaptive_strength")->data<float>();
		float metropolis_mutation_rate = samplerObj->property("metropolis_mutation_rate")->data<float>();
		int metropolis_mutation_rejection_rate = samplerObj->property("metropolis_mutation_rejection_rate")->data<int>();
		float metropolis_image_mutation_rate = samplerObj->property("metropolis_image_mutation_rate")->data<float>();
		float random_adaptive_strength = samplerObj->property("random_adaptive_strength")->data<float>();
		unsigned int russian_roulette_depth = samplerObj->property("russian_roulette_depth")->data<unsigned int>();
		float russian_roulette_cap = samplerObj->property("russian_roulette_cap")->data<float>();

		// Path
		unsigned int max_recursion_depth = pathObj->property("max_recursion_depth")->data<unsigned int>();
		unsigned int diffuse = pathObj->property("diffuse")->data<unsigned int>();
		unsigned int glossy = pathObj->property("glossy")->data<unsigned int>();
		unsigned int specular = pathObj->property("specular")->data<unsigned int>();
		float variance_clamp = pathObj->property("variance_clamp")->data<float>();
		bool l_tracing = pathObj->property("use_light_tracing")->data<bool>();
		float l_tracing_glossiness = pathObj->property("light_tracing_glossiness")->data<float>();

		// Pixel Filter
		auto pixFilterType = pixFilterObj->property("type")->data<int>();
		auto pixFilterWidth = pixFilterObj->property("width")->data<float>();
		auto pixFilterGaussianAlpha = pixFilterObj->property("gaussian_alpha")->data<float>();
		std::string pixFilterTag = "";
		if (pixFilterType == 0) {
			pixFilterTag = "BLACKMANHARRIS";
		}
		else if (pixFilterType == 1) {
			pixFilterTag = "GAUSSIAN";
		}
		else if (pixFilterType == 2) {
			pixFilterTag = "MITCHELL";
		}
		// Photon Gi
		auto enablePhotonGi = photonGiObj->property("enable")->data<bool>();
		auto photon_count = photonGiObj->property("photon_count")->data<int>();
		auto photon_depth = photonGiObj->property("photon_depth")->data<int>();

		auto enable_indirect_light_cache = photonGiObj->property("enable_indirect_light_cache")->data<bool>();
		auto indirect_halt_threshold = photonGiObj->property("indirect_halt_threshold")->data<int>();
		auto indirect_brute_force_radius_scale = photonGiObj->property("indirect_brute_force_radius_scale")->data<float>();
		auto indirect_normal_angle = photonGiObj->property("indirect_normal_angle")->data<int>();
		auto indirect_glossiness_threshold = photonGiObj->property("indirect_glossiness_threshold")->data<float>();
		auto indirect_auto_lookup_radius = photonGiObj->property("indirect_auto_lookup_radius")->data<bool>();
		auto indirect_lookup_radius = photonGiObj->property("indirect_lookup_radius")->data<float>();

		auto enable_caustic_light_cache = photonGiObj->property("enable_caustic_light_cache")->data<bool>();
		auto caustic_merge_photons = photonGiObj->property("caustic_merge_photons")->data<bool>();
		auto caustic_periodic_update = photonGiObj->property("caustic_periodic_update")->data<bool>();
		auto caustic_max_size = photonGiObj->property("caustic_max_size")->data<int>();
		auto caustic_lookup_radius = photonGiObj->property("caustic_lookup_radius")->data<float>();
		auto caustic_normal_angle = photonGiObj->property("caustic_normal_angle")->data<int>();
		auto caustic_step_samples = photonGiObj->property("caustic_step_samples")->data<int>();
		auto caustic_step_radius_reduction = photonGiObj->property("caustic_step_radius_reduction")->data<float>();
		auto caustic_step_minimum_radius = photonGiObj->property("caustic_step_minimum_radius")->data<float>();

		// DLS Cache
		auto dlsEnable = dlsObj->property("enable")->data<bool>();
		auto automatic_entry_radius = dlsObj->property("automatic_entry_radius")->data<bool>();
		auto entry_radius = dlsObj->property("entry_radius")->data<float>();
		auto warmup_samples = dlsObj->property("warmup_samples")->data<int>();
		auto normal_angle = dlsObj->property("normal_angle")->data<int>();
		auto max_passes = dlsObj->property("max_passes")->data<int>();
		auto convergence_threshold = dlsObj->property("convergence_threshold")->data<float>();
		auto place_entries_in_volumes = dlsObj->property("place_entries_in_volumes")->data<bool>();
		auto light_threshold = dlsObj->property("light_threshold")->data<float>();
		auto target_cache_hit_ratio = dlsObj->property("target_cache_hit_ratio")->data<float>();
		auto max_depth = dlsObj->property("max_depth")->data<int>();
		auto max_samples = dlsObj->property("max_samples")->data<int>();

		/// Samples Properties
		unsigned int halt_conditionID = 0;
		int samples = 0;
		float timed_samples = 0.f;

		if (!realtime) {
			halt_conditionID = haltObj->property("halt_condition")->data<unsigned int>();
			samples = haltObj->property("samples")->data<int>();
			timed_samples = (float)haltObj->property("timed_samples")->data<int>();
			_refreshInterval = refreshObj->property("refresh_interval")->data<int>();
		}
		else {
			halt_conditionID = haltObj->property("ipr_halt_condition")->data<unsigned int>();
			samples = haltObj->property("ipr_samples")->data<int>();
			timed_samples = (float)haltObj->property("ipr_timed_samples")->data<int>();
		}

		int camWidth = camHandler.dataObject()->data<fl::DataObject>().property("width")->data<unsigned int>();
		int camHeight = camHandler.dataObject()->data<fl::DataObject>().property("height")->data<unsigned int>();

		float haltTime = 0.0;
		unsigned haltSPP = 0;

		if (halt_conditionID == DfxLuxCoreRenderNode::Halt::TIME) {
			haltTime = timed_samples;
			_haltTime = (int)timed_samples;
		}
		else {
			haltSPP = samples;
			_haltSpp = samples;
		}

		// Retrieve Camera settings
		if (cameraPath.empty()) {
			LC_LOG("LuxCore Camera RUNTIME ERROR: Empty camera path.");
			return false;
		}
		else {
			auto rc = camHandler.fetch(cameraPath);

			if (!rc) {
				LC_LOG("LuxCore Camera RUNTIME ERROR: Failed to fetch handle.");
				return false;
			}
		}

		usingOpencl = useOpencl;

		try {

			luxrays::Properties properties;
			
			/// Engine / Device
			properties
				<< Property("renderengine.type")(device)
				<< Property("opencl.cpu.use")(true)
				<< Property("opencl.gpu.use")(useOpencl);
				
			/// Path
			properties 
				<< Property("path.pathdepth.total")(max_recursion_depth)
				<< Property("path.pathdepth.diffuse")(diffuse)
				<< Property("path.pathdepth.glossy")(glossy)
				<< Property("path.pathdepth.specular")(specular)
				<< Property("path.russianroulette.depth")(russian_roulette_depth)
				<< Property("path.russianroulette.cap")(russian_roulette_cap)
				<< Property("path.clamping.variance.maxvalue")(variance_clamp)
				<< Property("path.hybridbackforward.enable")(l_tracing)
				<< Property("path.hybridbackforward.partition ")(0)
				<< Property("path.hybridbackforward.glossinessthreshold")(l_tracing_glossiness);
				// << Property("path.forceblackbackground.enable")(force_black_background) <<

			/// Sampler
			if (!realtime) {
				properties
					<< Property("sampler.type")(sampler.c_str())
					<< Property("sampler.sobol.adaptive.strength")(sobol_adaptive_strength)
					<< Property("sampler.random.adaptive.strength")(random_adaptive_strength)
					<< Property("sampler.metropolis.largesteprate")(metropolis_mutation_rate)
					<< Property("sampler.metropolis.maxconsecutivereject")(metropolis_mutation_rejection_rate)
					<< Property("sampler.metropolis.imagemutationrate")(metropolis_image_mutation_rate);
			}
			else {		
				if (device == "RTPATHCPU") {
					properties
						<< Property("sampler.type")("RTPATHCPUSAMPLER");
				}
				else if (device == "RTPATHOCL"){
					properties
						<< Property("sampler.type")("TILEPATHSAMPLER");
				}
			}

			/// Halt
			properties
				<< Property("batch.halttime")(haltTime)
				<< Property("batch.haltspp")(haltSPP);

			/// Film
			properties
				// << Property("film.opencl.enable")(false)
				<< Property("film.width")(camWidth)
				<< Property("film.height")(camHeight);

			// Pixel Filter
			if (pixFilterType < 3) {
				properties 
					<< Property("film.filter.type")(pixFilterTag)
					<< Property("film.filter.width")(pixFilterWidth)
					<< Property("film.filter.gaussian.alpha")(pixFilterGaussianAlpha);
			}

			// Photon Gi
			if (enablePhotonGi) {
				properties
					<< Property("path.photongi.photon.maxcount")(photon_count * 1000000)
					<< Property("path.photongi.photon.maxdepth")(photon_depth)
					<< Property("path.photongi.photon.glossinessusagethreshold")(indirect_glossiness_threshold);

				if (enable_indirect_light_cache) {
					properties
						<< Property("path.photongi.indirect.enabled")(true)
						<< Property("path.photongi.indirect.maxsize")(0)
						<< Property("path.photongi.indirect.haltthreshold")(indirect_halt_threshold)
						<< Property("path.photongi.indirect.usagethresholdscale")(indirect_brute_force_radius_scale)
						<< Property("path.photongi.indirect.lookup.normalangle ")(indirect_normal_angle)
						<< Property("path.photongi.indirect.haltthreshold")(indirect_halt_threshold);

					if (indirect_auto_lookup_radius) {
						properties
							<< Property("path.photongi.indirect.lookup.radius")(0);
					}
					else {
						properties
							<< Property("path.photongi.indirect.lookup.radius")(indirect_lookup_radius);
					}
				}
				else {
					properties
						<< Property("path.photongi.indirect.enabled")(false);
				}

				if (enable_caustic_light_cache) {
					properties
						<< Property("path.photongi.caustic.enabled")(true)
						<< Property("path.photongi.caustic.maxsize")(caustic_max_size * 1000000)
						<< Property("path.photongi.caustic.lookup.radius")(caustic_lookup_radius)
						<< Property("path.photongi.caustic.lookup.normalangle")(caustic_normal_angle);
					

					if (caustic_periodic_update) {
						properties
							<< Property("path.photongi.caustic.updatespp")(caustic_step_samples)
							<< Property("path.photongi.caustic.updatespp.radiusreduction")(caustic_step_radius_reduction)
							<< Property("path.photongi.caustic.updatespp.minradius")(caustic_step_minimum_radius);
					}
					else {
						properties
							<< Property("path.photongi.caustic.updatespp")(0);
					}
				}
				else {
					properties
						<< Property("path.photongi.caustic.enabled")(false);
				}
			}
			
			// Dls Cache
			if (dlsEnable) {
				properties
					<< Property("lightstrategy.type")("DLS_CACHE")
					<< Property("lightstrategy.entry.normalangle")(normal_angle)
					<< Property("lightstrategy.entry.maxpasses")(max_passes)
					<< Property("lightstrategy.entry.convergencethreshold")(convergence_threshold)
					<< Property("lightstrategy.entry.warmupsamples")(warmup_samples)
					<< Property("lightstrategy.entry.volumes.enable")(place_entries_in_volumes)
					<< Property("lightstrategy.lightthreshold ")(light_threshold)
					<< Property("lightstrategy.targetcachehitratio")(target_cache_hit_ratio)
					<< Property("lightstrategy.maxdepth")(max_depth)
					<< Property("lightstrategy.maxsamplescount ")(max_samples);
			}	

			// writeToFile("c:/test/lux_render_config.txt", properties.ToString());
			if(!setupRenderConfigBuffer(properties)) return nullptr;

			return luxcore::RenderConfig::Create( properties, scene );
		}
		catch (std::runtime_error &err) {
			LC_LOG("LuxCore Prepare Session (Path Engine) RUNTIME ERROR: " << err.what());
			return nullptr;
		}
		catch (std::exception &err) {
			LC_LOG("LuxCore (Path Engine) ERROR: " << err.what());
			return nullptr;
		}

	}

	return nullptr;
}

luxcore::RenderConfig *DfxLuxCoreRenderSession::bidirRenderConfig() {
	return nullptr;
}

bool DfxLuxCoreRenderSession::prepareSession() {

#ifndef _DEBUG
	bool useOpenCL = false;

	if (_renderEngine == PATH) {
		render_config = pathRenderConfig(useOpenCL);
	}
	else if (_renderEngine == RT) {
		render_config = pathRenderConfig(useOpenCL, true);
	}

	if (render_config != nullptr) {
		//writeToFile("c:/test/render_config.txt", render_config->GetProperties().ToString());

		if (useOpenCL) {
			if (!render_config->HasCachedKernels()) {
				QMessageBox msgBox;
				msgBox.setText("LuxCoreRender is going to compile OpenCL Kernels (just once, may take a few minutes).");
				msgBox.setStandardButtons(QMessageBox::Ok);
				msgBox.setDefaultButton(QMessageBox::Ok);
				msgBox.exec();
			}
		}

		render_session = luxcore::RenderSession::Create(render_config);

		return true;
	}
	else {
		LC_LOG("LuxCore failed to prepare render session.");
	}
#endif // _DEBUG

	return false;
}

bool DfxLuxCoreRenderSession::clear() {
	id_gen.reset();
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
	_haltSpp = 0;

	return true;
}

bool DfxLuxCoreRenderSession::checkSceneIsReady(std::string tag) {

	if (!_initialized) {
		DfxGlobal::console::println(tag + " Error: LuxCore not initialized.");
		return false;
	}

	if (scene == nullptr) {
		DfxGlobal::console::println(tag + " Error: Scene not initialized.");
		return false;
	}

	return true;
}

bool DfxLuxCoreRenderSession::checkSessionIsReady(std::string tag) {
	if (render_config == nullptr) {
		DfxGlobal::console::println(tag + " Error: No Render Configuration.");
		return false;
	}

	if (render_session == nullptr) {
		DfxGlobal::console::println(tag + " Error: No Session.");
		return false;
	}

	return true;
}

bool DfxLuxCoreRenderSession::loadScene(deepsg::SgNode *branch) {
	if (!checkSceneIsReady("LuxCore Session Prep"))return false;

	loadCamera(branch);
	loadMaterials(branch);
	loadObjects(branch);
	loadInstances();
	loadVolumes(branch);
	loadFluentSimplicies();
	loadFluentSimplexCaches();
	loadLights(branch);
	
	activeBranch = branch;
	return true;
}

bool DfxLuxCoreRenderSession::unloadScene() {
	if (!checkSceneIsReady("LuxCore Session Prep"))return false;

	return true;
}

bool DfxLuxCoreRenderSession::updateTransformations() {
	if (!checkSceneIsReady("LuxCore Session Prep"))return false;

	return true;
}

bool DfxLuxCoreRenderSession::updateGeometry() {
	return true;
}

bool DfxLuxCoreRenderSession::updateMaterials() {
	const std::lock_guard<std::recursive_mutex> lock(_updateSessionMutex);

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
	newHaltTimer(rtHaltTime);

	return true;
}

bool DfxLuxCoreRenderSession::updateGeometryTransform() {
	const std::lock_guard<std::recursive_mutex> lock(_updateSessionMutex);

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
		
				for (auto &segment: itr00->second) {

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
	newHaltTimer(rtHaltTime);

	return true;
}

bool DfxLuxCoreRenderSession::updateLights() {
	//const std::lock_guard<std::recursive_mutex> lock(_updateSessionMutex);

	if (auto lock = std::unique_lock<std::recursive_mutex>(_updateSessionMutex, std::try_to_lock)) {
		if (!checkSessionIsReady("LuxCore Render")) {
			LC_LOG("LuxCore Render Session Update Failed.");
			return false;
		}

		render_session->BeginSceneEdit();
		loadLights(activeBranch);
		render_session->EndSceneEdit();

		if (render_session->IsInPause()) {
			render_session->Resume();
			newBufferRefreshTimer(120);
		}
		newHaltTimer(rtHaltTime);

	}
	return true;
}

bool DfxLuxCoreRenderSession::updateCamera() {
	if (!checkSessionIsReady("LuxCore Render")) {
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
	newHaltTimer(rtHaltTime);
	
	return true;
}

bool DfxLuxCoreRenderSession::render() {
	auto space = DfxSpace::getInstance();

	if (!checkSceneIsReady("LuxCore Render"))return false;
	if (!checkSessionIsReady("LuxCore Render"))return false;
	
	_forceStop = false;
	auto _printInfo = true;

	auto anyPrintInfo = space->findAbsolute(_luxcoreRenderInfoPath + "/luxcore_general/print_render_info");
	if (anyPrintInfo != nullptr) _printInfo = anyPrintInfo->data<bool>();

	try {
		render_session->Start();
		
		bool warmStart = true;;
		double elapsedTime = 0.0;
		double warmStartElapsed = 0.0;
		double warmStartInterval = 0.2;

		while (!render_session->HasDone() && !_forceStop) {

			if(!_forceStop) QApplication::processEvents();

			if (!_forceStop) {
				render_session->UpdateStats();
				Properties stats = render_session->GetStats();

				// if(_printInfo) printRenderingStats(stats);

				auto current_elapsedTime = stats.Get("stats.renderengine.time").Get<double>();
				auto current_samples = stats.Get("stats.renderengine.pass").Get<unsigned int>();

				if (warmStart) {
					if ((int)(current_elapsedTime - warmStartElapsed) > (warmStartInterval - 1)) {
						warmStartElapsed = current_elapsedTime;
						if (!updateOutput()) DfxGlobal::console::println("Failed to render output.");
					}

					if (warmStartElapsed > 10.0) warmStart = false;
				}
				else {
					if ((int)(current_elapsedTime - elapsedTime) > (_refreshInterval - 1)) {
						elapsedTime = current_elapsedTime;
						if (!updateOutput()) DfxGlobal::console::println("Failed to render output.");
					}
				}

				if (_haltTime > 0) {
					double complete = current_elapsedTime / (double)_haltTime;
					_renderOutput->setProgress(complete * 100.0);
				}
				
				if (_haltSpp > 0) {
					double complete = (double)current_samples / (double)_haltSpp;
					_renderOutput->setProgress(complete * 100.0);
				}
			}
		}

		if (_forceStop) render_session->Stop();
		
		estimatePathClampingVariance();

		if (_denoisingEnabled) {
			if (!updateOutput(true)) DfxGlobal::console::println("Failed to render output.");
		}
		else {
			if (!updateOutput()) DfxGlobal::console::println("Failed to render output.");
		}

		_renderOutput->setProgress(100);

		// render_session->GetFilm().SaveOutputs();
		// output().save("c:/test/render_result.png", "PNG");
	}
	catch (std::runtime_error &err) {
		LC_LOG("LuxCore Render ERROR: " << err.what());
		return false;
	}
	catch (std::exception &err) {
		LC_LOG("LuxCore: " << err.what());
		return false;
	}

	return true;
}

bool DfxLuxCoreRenderSession::renderExport(std::string dir) {
	luxrays::Properties properties;

	properties
		<< Property("renderengine.type")("FILESAVER")
		<< Property("filesaver.format")("TXT")
		<< Property("filesaver.directory")(dir);

	auto r_config = luxcore::RenderConfig::Create(properties, scene);
	auto exrender_session = luxcore::RenderSession::Create(r_config);

	exrender_session->Start();

	while (!exrender_session->HasDone());

	delete r_config;
	delete exrender_session;

	return true;
}

QImage DfxLuxCoreRenderSession::output() {
	if (!checkSessionIsReady("LuxCore Render"))return QImage(540, 360, QImage::Format::Format_ARGB32);
	
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
	}
}


float DfxLuxCoreRenderSession::CalcY(const float *c) {
	return 0.212671f * c[0] + 0.715160f * c[1] + 0.072169f * c[2];
}

float DfxLuxCoreRenderSession::estimatePathClampingVariance() {
	for (int i = 0; i < _renderOutput->bufferCount(); i++) {
		try {
			if (_renderOutput->getBuffer(i)->bufferName() == "RGB Image Pipeline") {
			
				const unsigned int filmWidth = render_session->GetFilm().GetWidth();
				const unsigned int filmHeight = render_session->GetFilm().GetHeight();

				std::unique_ptr<float[]> _ibuffer(new float[filmWidth * filmHeight * 3]);

				render_session->GetFilm().GetOutput<float>(luxcore::Film::FilmOutputType::OUTPUT_RGB_IMAGEPIPELINE, _ibuffer.get(), _renderOutput->getBuffer(i)->getIndex());
				
				float Y = 0.f;
				for (unsigned int i = 0; i < filmWidth * filmHeight * 3; i += 3) {
					const float y = CalcY(&_ibuffer.get()[i]);
					if ((y <= 0.f) || isinf(y))
						continue;

					Y += y;
				}

				const unsigned int pixelCount = filmWidth * filmHeight;
				Y /= pixelCount;

				if (Y <= 0.f)
					return 0.f;
				else {
					const float v = Y * 10.f;
					auto suggestedVerianceClampingValue = v * v;
					return suggestedVerianceClampingValue;
				}

			}
		}
		catch (std::runtime_error &err) {
			LC_LOG("LuxCore Session Estimate Path Clamping Variance RUNTIME ERROR: " << err.what());
			return 0.f;
		}
		catch (std::exception &err) {
			LC_LOG("LuxCore Session Estimate Path Clamping Variance ERROR: " << err.what());
			return 0.f;
		}
	}

	return 0.f;
}

void DfxLuxCoreRenderSession::_updateOutput() {
	updateOutput(false);
}

void DfxLuxCoreRenderSession::applyWaterMark(QImage &image) {

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

void DfxLuxCoreRenderSession::Normalize3(const float *filmPixels, float *pixels, const unsigned int filmWidth, const unsigned int filmHeight) const {
	for (unsigned int y = 0; y < filmHeight; ++y) {
		for (unsigned int x = 0; x < filmWidth; ++x) {
			const unsigned int index = x + y * filmWidth;
			const float *src = &filmPixels[index * 4];
			float *dst = &pixels[index * 3];

			if (src[3] > 0.f) {
				const float k = 1.f / src[3];
				dst[0] = src[0] * k;
				dst[1] = src[1] * k;
				dst[2] = src[2] * k;
			}
			else {
				dst[0] = 0.f;
				dst[1] = 0.f;
				dst[2] = 0.f;
			}
		}
	}
}

void DfxLuxCoreRenderSession::AutoLinearToneMap(const float *src, float *dst, const unsigned int filmWidth, const unsigned int filmHeight) const {

	float minVal[3] = {
		std::numeric_limits<float>::min(),
		std::numeric_limits<float>::min(),
		std::numeric_limits<float>::min()
	};

	for (unsigned int yy = 0; yy < filmHeight; ++yy) {
		for (unsigned int xx = 0; xx < filmWidth; ++xx) {
			const unsigned int index = xx + yy * filmWidth;
			const float *p = &src[index * 3];

			minVal[0] = std::min(minVal[0], p[0]);
			minVal[1] = std::min(minVal[1], p[1]);
			minVal[2] = std::min(minVal[2], p[2]);
		}
	}

	float Y = 0.f;
	for (unsigned int yy = 0; yy < filmHeight; ++yy) {
		for (unsigned int xx = 0; xx < filmWidth; ++xx) {
			const unsigned int index = (xx + yy * filmWidth) * 3;
			const float *s = &src[index];
			float *d = &dst[index];

			d[0] = s[0] - minVal[0];
			d[1] = s[1] - minVal[1];
			d[2] = s[2] - minVal[2];

			// Spectrum::Y()
			const float y = 0.212671f * d[0] + 0.715160f * d[1] + 0.072169f * d[2];
			if ((y <= 0.f) || isinf(y))
				continue;

			Y += y;
		}
	}

	const unsigned int pixelCount = filmWidth * filmHeight;
	Y /= pixelCount;
	if (Y <= 0.f)
		return;

	const float scale = (1.25f / Y * powf(118.f / 255.f, 2.2f));

	for (unsigned int i = 0; i < pixelCount * 3; ++i)
		dst[i] = scale * src[i];
}

void DfxLuxCoreRenderSession::Normalize1(const float *filmPixels, float *pixels, const unsigned int filmWidth, const unsigned int filmHeight) const {
	for (unsigned int y = 0; y < filmHeight; ++y) {
		for (unsigned int x = 0; x < filmWidth; ++x) {
			const unsigned int index = x + y * filmWidth;
			const float *src = &filmPixels[index * 2];
			float *dst = &pixels[index * 3];

			if (src[1] > 0.f) {
				const float v = src[0] / src[1];
				dst[0] = v;
				dst[1] = v;
				dst[2] = v;
			}
			else {
				dst[0] = 0.f;
				dst[1] = 0.f;
				dst[2] = 0.f;
			}
		}
	}
}

void DfxLuxCoreRenderSession::Copy1(const float *filmPixels, float *pixels, const unsigned int filmWidth, const unsigned int filmHeight) const {
	for (unsigned int y = 0; y < filmHeight; ++y) {
		for (unsigned int x = 0; x < filmWidth; ++x) {
			const unsigned int index = x + y * filmWidth;
			const float *src = &filmPixels[index];
			float *dst = &pixels[index * 3];

			const float v = src[0];
			dst[0] = v;
			dst[1] = v;
			dst[2] = v;
		}
	}
}

void DfxLuxCoreRenderSession::Copy3(const float *filmPixels, float *pixels, const unsigned int filmWidth, const unsigned int filmHeight) const {
	std::copy(&filmPixels[0], &filmPixels[filmWidth * filmHeight * 3], pixels);
}

bool DfxLuxCoreRenderSession::updateOutput(bool applyDenoising) {
	// const std::lock_guard<std::recursive_mutex> lock(_updateOutputMutex);

	if (auto lock = std::unique_lock<std::recursive_mutex>(_updateSessionMutex, std::try_to_lock)) {
		if (!checkSessionIsReady("LuxCore Render"))return false;

		DfxLxCameraHandler camHandler;

		if (cameraPath.empty()) {
			LC_LOG("LuxCore Session Ouput RUNTIME ERROR: Empty camera path.");
			return false;
		}
		else {
			auto rc = camHandler.fetch(cameraPath);

			if (!rc) {
				LC_LOG("LuxCore Session Ouput RUNTIME ERROR: Failed to fetch handle.");
				return false;
			}
		}

		int camWidth = camHandler.dataObject()->data<fl::DataObject>().property("width")->data<unsigned int>();
		int camHeight = camHandler.dataObject()->data<fl::DataObject>().property("height")->data<unsigned int>();

		for (int i = 0; i < _renderOutput->bufferCount(); i++) {

			try {

				if (_renderOutput->getBuffer(i)->bufferName() == "RGB Image Pipeline") {

					QImage image(camWidth, camHeight, QImage::Format::Format_RGB888);
					int buffer_size = render_session->GetFilm().GetOutputSize(luxcore::Film::FilmOutputType::OUTPUT_RGB_IMAGEPIPELINE);

					std::unique_ptr<float[]> _ibuffer(new float[buffer_size]);

					if (!applyDenoising || !_denoisingEnabled) {
						render_session->GetFilm().GetOutput<float>(luxcore::Film::FilmOutputType::OUTPUT_RGB_IMAGEPIPELINE, _ibuffer.get(), _renderOutput->getBuffer(i)->getIndex());
					}
					else {
						render_session->GetFilm().GetOutput<float>(luxcore::Film::FilmOutputType::OUTPUT_RGB_IMAGEPIPELINE, _ibuffer.get(), _denoisingBuffer);
					}

					for (int j = 0; j < camWidth * camHeight * 3; j++) {
						image.bits()[j] = (uchar)(std::min(1.f, _ibuffer.get()[j]) * 255.f);
					}

					image = image.mirrored();

					if (DeepFxStudioLicenseManager::getState() != DeepFxStudioLicenseManager::NODE_LOCKED_STATE) applyWaterMark(image);

					_renderOutput->getBuffer(i)->setImage(image);

				}
				else if (_renderOutput->getBuffer(i)->bufferName() == "Alpha") {

					QImage image(camWidth, camHeight, QImage::Format::Format_RGB888);

					const float *filmPixels = render_session->GetFilm().GetChannel<float>(luxcore::Film::FilmChannelType::CHANNEL_ALPHA, _renderOutput->getBuffer(i)->getIndex());
					std::unique_ptr<float[]> _ibuffer(new float[camWidth * camHeight * 3]);

					Normalize1(filmPixels, _ibuffer.get(), camWidth, camHeight);
					//AutoLinearToneMap(_ibuffer.get(), _ibuffer.get(), camWidth, camHeight);

					for (int j = 0; j < camWidth * camHeight * 3; j++) {
						image.bits()[j] = (uchar)(std::min(1.f, _ibuffer.get()[j]) * 255.f);
					}

					if (DeepFxStudioLicenseManager::getState() != DeepFxStudioLicenseManager::NODE_LOCKED_STATE) applyWaterMark(image);

					_renderOutput->getBuffer(i)->setImage(image.mirrored());
				}
				else if (_renderOutput->getBuffer(i)->bufferName() == "Depth") {

					QImage image(camWidth, camHeight, QImage::Format::Format_RGB888);

					const float *filmPixels = render_session->GetFilm().GetChannel<float>(luxcore::Film::FilmChannelType::CHANNEL_DEPTH, _renderOutput->getBuffer(i)->getIndex());
					std::unique_ptr<float[]> _ibuffer(new float[camWidth * camHeight * 3]);

					Copy1(filmPixels, _ibuffer.get(), camWidth, camHeight);
					AutoLinearToneMap(_ibuffer.get(), _ibuffer.get(), camWidth, camHeight);

					for (int j = 0; j < camWidth * camHeight * 3; j++) {
						image.bits()[j] = (uchar)(std::min(1.f, _ibuffer.get()[j]) * 255.f);
					}

					if (DeepFxStudioLicenseManager::getState() != DeepFxStudioLicenseManager::NODE_LOCKED_STATE) applyWaterMark(image);

					_renderOutput->getBuffer(i)->setImage(image.mirrored());
				}
				else if (_renderOutput->getBuffer(i)->bufferName() == "RGB") {
					QImage image(camWidth, camHeight, QImage::Format::Format_RGB888);

					const float *filmPixels = render_session->GetFilm().GetChannel<float>(luxcore::Film::FilmChannelType::CHANNEL_IMAGEPIPELINE, _renderOutput->getBuffer(i)->getIndex());
					std::unique_ptr<float[]> _ibuffer(new float[camWidth * camHeight * 3]);

					Copy3(filmPixels, _ibuffer.get(), camWidth, camHeight);

					for (int j = 0; j < camWidth * camHeight * 3; j++) {
						image.bits()[j] = (uchar)(std::min(1.f, _ibuffer.get()[j]) * 255.f);
					}

					if (DeepFxStudioLicenseManager::getState() != DeepFxStudioLicenseManager::NODE_LOCKED_STATE) applyWaterMark(image);

					_renderOutput->getBuffer(i)->setImage(image.mirrored());
				}
				else if (_renderOutput->getBuffer(i)->bufferName() == "Direct Diffuse" || _renderOutput->getBuffer(i)->bufferName() == "Indirect Diffuse" ||
					     _renderOutput->getBuffer(i)->bufferName() == "Direct Glossy"  || _renderOutput->getBuffer(i)->bufferName() == "Indirect Glossy" ||
						 _renderOutput->getBuffer(i)->bufferName() == "Indirect Specular" || _renderOutput->getBuffer(i)->bufferName() == "Emission" ||
						 _renderOutput->getBuffer(i)->bufferName() == "Irradiance") {

					QImage image(camWidth, camHeight, QImage::Format::Format_RGB888);

					const float *filmPixels = render_session->GetFilm().GetChannel<float>(luxcore::Film::FilmChannelType::CHANNEL_DIRECT_DIFFUSE, _renderOutput->getBuffer(i)->getIndex());
					
					if (_renderOutput->getBuffer(i)->bufferName() == "Direct Diffuse") {
						filmPixels = render_session->GetFilm().GetChannel<float>(luxcore::Film::FilmChannelType::CHANNEL_DIRECT_DIFFUSE, _renderOutput->getBuffer(i)->getIndex());
					}
					else if (_renderOutput->getBuffer(i)->bufferName() == "Indirect Diffuse") {
						filmPixels = render_session->GetFilm().GetChannel<float>(luxcore::Film::FilmChannelType::CHANNEL_INDIRECT_DIFFUSE, _renderOutput->getBuffer(i)->getIndex());
					}
					else if (_renderOutput->getBuffer(i)->bufferName() == "Direct Glossy") {
						filmPixels = render_session->GetFilm().GetChannel<float>(luxcore::Film::FilmChannelType::CHANNEL_DIRECT_GLOSSY, _renderOutput->getBuffer(i)->getIndex());
					}
					else if (_renderOutput->getBuffer(i)->bufferName() == "Indirect Glossy") {
						filmPixels = render_session->GetFilm().GetChannel<float>(luxcore::Film::FilmChannelType::CHANNEL_INDIRECT_GLOSSY, _renderOutput->getBuffer(i)->getIndex());
					}
					else if (_renderOutput->getBuffer(i)->bufferName() == "Indirect Specular") {
						filmPixels = render_session->GetFilm().GetChannel<float>(luxcore::Film::FilmChannelType::CHANNEL_INDIRECT_SPECULAR, _renderOutput->getBuffer(i)->getIndex());
					}
					else if (_renderOutput->getBuffer(i)->bufferName() == "Emission") {
						filmPixels = render_session->GetFilm().GetChannel<float>(luxcore::Film::FilmChannelType::CHANNEL_EMISSION, _renderOutput->getBuffer(i)->getIndex());
					}
					else if (_renderOutput->getBuffer(i)->bufferName() == "Irradiance") {
						filmPixels = render_session->GetFilm().GetChannel<float>(luxcore::Film::FilmChannelType::CHANNEL_IRRADIANCE, _renderOutput->getBuffer(i)->getIndex());
					}
					
					std::unique_ptr<float[]> _ibuffer(new float[camWidth * camHeight * 3]);

					Normalize3(filmPixels, _ibuffer.get(), camWidth, camHeight);
					//AutoLinearToneMap(_ibuffer.get(), _ibuffer.get(), camWidth, camHeight);

					for (int j = 0; j < camWidth * camHeight * 3; j++) {					
						image.bits()[j] = (uchar)(std::min(1.f, _ibuffer.get()[j]) * 255.f);
					}

					if (DeepFxStudioLicenseManager::getState() != DeepFxStudioLicenseManager::NODE_LOCKED_STATE) applyWaterMark(image);

					_renderOutput->getBuffer(i)->setImage(image.mirrored());
				}
			}
			catch (std::runtime_error &err) {
				LC_LOG("LuxCore Session Output RUNTIME ERROR: " << err.what());
				return false;
			}
			catch (std::exception &err) {
				LC_LOG("LuxCore Session Output ERROR: " << err.what());
				return false;
			}
		}
	}

	return true;
}

void DfxLuxCoreRenderSession::testMaterials() {
	DfxLuxCoreMaterialBuilder builder;
	builder.build(scene);
}

bool DfxLuxCoreRenderSession::loadMaterials(deepsg::SgNode *branch) {

	try {

		scene->Parse(
			Property("scene.materials.default.type")("matte") <<
			Property("scene.materials.default.preset")("matte") <<
			Property("scene.materials.transparent.type")("null")
		);

		matBuilder = DfxLuxCoreMaterialBuilder();
		matBuilder.build(scene);

		if (_materialDispatcher != nullptr) delete _materialDispatcher;
		_materialDispatcher = DfxLuxCoreMaterialBuilder::createMatrialDispatcher();
	}
	catch (std::runtime_error &err) {
		LC_LOG("LuxCore Material RUNTIME ERROR: " << err.what());
		return false;
	}
	catch (std::exception &err) {
		LC_LOG("LuxCore Material ERROR: " << err.what());
		return false;
	}

	return true;
}

struct MeshSegment {
	size_t surf_id;
	int materialID;
	std::vector<deepsg::SgGeoAdapterInterface::Triangle> triangles;
};

bool DfxLuxCoreRenderSession::loadFluentSimplicies() {

	auto materialIDS = DfxMaterialManager::getInstance()->materialIDMap();
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
	}

	return true;
}

bool DfxLuxCoreRenderSession::loadFluentSimplexCaches() {

	auto materialIDS = DfxMaterialManager::getInstance()->materialIDMap();
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
	}
	return true;
}

void DfxLuxCoreRenderSession::findInstancePaths() {

	_instanceShapes.clear();

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
	}

}

bool DfxLuxCoreRenderSession::loadObjects(deepsg::SgNode *branch) {

	try {

		deepsg::SgConditionalTraversalFunction renderFunc = [this](deepsg::SgNode *node, deepsg::SgStateInfo *info)-> deepsg::TraversalFlag {
			
			auto instItr = _instanceShapes.find(node->referencePath());

			if ( node->getType() == deepsg::SgNodeType::GEOMETRY) {

				if (instItr == _instanceShapes.end() && !node->isRenderable()) {
					return deepsg::TraversalFlag::IGNORE_CHILDREN;
				}

				DfxObjectHandler handler;

				std::vector<deepsg::SgGeoAdapterInterface::Triangle> tris;
				auto geoNode = (deepsg::SgGeoNode*)node;
				auto geo = ((deepsg::SgGeoNode*)node)->getGeometryAdapter();
				handler.fetch(node->referencePath());

				std::set<unsigned int> surface_ids;

				//for (size_t i = 0; i < geo->getPolygonCount(); i++) surface_ids.insert(geo->getPolygonSurfaceID(i));

				std::unordered_map<size_t, MeshSegment*> meshSegments;
				auto tricount = geo->getTriangleIndexListSize() / 3;
				auto indices = geo->getTriangleIndexList();
				auto sourceFaces = geo->getTriangleSourceFaceIndexList();

				for (size_t i = 0; i < tricount; i++) {
					auto offset = i * 3;
					auto sid = geo->getPolygonSurfaceID(sourceFaces[i]);

					MeshSegment *currentSegment = nullptr;

					auto itr = meshSegments.find(sid);

					if (itr == meshSegments.end()) {
						currentSegment = new MeshSegment;
						currentSegment->surf_id = sid;
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

					auto uvMap = geo->getPolygonUVMap(sourceFaces[i]);

					currentSegment->triangles.push_back(triangle);
									
				}	

				delete[] indices;
				delete[] sourceFaces;

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

				// Load segements into Luxcore
				std::vector<DfxLuxObjectSegment> segmentInfo;
				for (auto p : meshSegments) {

					auto segment = p.second;

					size_t vertCount = 0;
					size_t polyCount = 0;
					std::unordered_map<size_t, size_t> oldNewIndex;
					std::vector<size_t> newVertexIndices;
					std::string material = "default";
					std::string displacement = "";

					bool overrideSmoothing = false;
					bool overrideUseSmoothing = false;
					
					bool enableSubdivsion = false;
					int  subdivMaxLevel = 2;
					float subdivMaxEdge = 0.f;

					bool useNormalSmooothing = true;
					int  disp_type = 0;
					float disp_scale = 1.f;
					float disp_offset = 0.f;

					if(DfxMaterialLinker::getInstance()->hasObjectLink(geoNode->referencePath(), segment->surf_id)) {

						auto globalMatPath = DfxMaterialLinker::getInstance()->objectLink(geoNode->referencePath(), segment->surf_id);
						auto luxMat = matBuilder.material(globalMatPath);
						displacement = matBuilder.displacement(globalMatPath);

						auto matAny = DfxSpace::getInstance()->findAbsolute(globalMatPath);

						if (matAny != nullptr) {
							if (matAny->type() == fl::type::DATA_OBJECT) {
								auto matDao = matAny->rawData<fl::DataObject>();

								if (matDao->hasProperty("smooth_shading_override")) {
									overrideSmoothing = matDao->property("smooth_shading_override")->data<bool>();
								}

								if (matDao->hasProperty("use_smooth_shading")) {
									overrideUseSmoothing = matDao->property("use_smooth_shading")->data<bool>();
								}

								if (matDao->hasProperty("enable_subdivison")) {
									enableSubdivsion = matDao->property("enable_subdivison")->data<bool>();
								}

								if (matDao->hasProperty("max_level")) {
									subdivMaxLevel = matDao->property("max_level")->data<int>();
								}

								if (matDao->hasProperty("max_edge_screen_size")) {
									subdivMaxEdge = matDao->property("max_edge_screen_size")->data<float>();
								}

								if (matDao->hasProperty("disp_smooth_normals")) {
									useNormalSmooothing = matDao->property("disp_smooth_normals")->data<bool>();
								}

								if (matDao->hasProperty("disp_type")) {
									disp_type = matDao->property("disp_type")->data<unsigned int>();
								}

								if (matDao->hasProperty("disp_scale")) {
									disp_scale = matDao->property("disp_scale")->data<float>();
								}

								if (matDao->hasProperty("disp_offset")) {
									disp_offset = matDao->property("disp_offset")->data<float>();
								}

							}
						}

						if (!luxMat.empty()) {
							material = luxMat;
						}
						else {
							DfxGlobal::console::println("Cant find linked material");
						}
					}
					else {
						DfxGlobal::console::println("No Material Linked " + geoNode->referencePath());
					}

					vertCount = segment->triangles.size() * 3;
					polyCount = segment->triangles.size();
					
					if (polyCount > 0) {
				
						LuxPoint *p = (LuxPoint*)luxcore::Scene::AllocVerticesBuffer(vertCount);
						LuxTriangle *t = (LuxTriangle*)luxcore::Scene::AllocTrianglesBuffer(polyCount);
						LuxUV *uvs = new LuxUV[vertCount];

						float *normals = NULL;
						
						if (!overrideSmoothing) {
							if (!handler.isNull()) {
								if (handler.useSmoothShading()) {
									normals = new float[vertCount * 3];
								}
							}
						}
						else {
							if (overrideUseSmoothing) {
								normals = new float[vertCount * 3];
							}
						}

						std::vector<glm::vec3> recalc_normals; 

						if (geoNode->vertexBufferSize() > 0 && geoNode->vertexBuffer() != nullptr) {
							recalc_normals = geo->calculateSmoothNormals(geoNode->vertexBuffer());
						}
						else {
							recalc_normals = geo->calculateSmoothNormals();
						}

						for (int i = 0; i < polyCount; i++) {

							auto offset = i * 3;
							auto norm_offset = i * 9;
							auto uv_offset = i * 3;

							auto uvMap = geo->getPolygonUVMap(segment->triangles[i].faceIndex);

							size_t a = segment->triangles[i].a;
							size_t b = segment->triangles[i].b;
							size_t c = segment->triangles[i].c;

							
							if (geoNode->vertexBufferSize() > 0 && geoNode->vertexBuffer() != nullptr) {
								
								size_t offset_a = a * 3;
								size_t offset_b = b * 3;
								size_t offset_c = c * 3;

								p[offset] = LuxPoint(geoNode->vertexBuffer()[offset_a], geoNode->vertexBuffer()[offset_a + 1], geoNode->vertexBuffer()[offset_a + 2]);
								p[offset + 1] = LuxPoint(geoNode->vertexBuffer()[offset_b], geoNode->vertexBuffer()[offset_b + 1], geoNode->vertexBuffer()[offset_b + 2]);
								p[offset + 2] = LuxPoint(geoNode->vertexBuffer()[offset_c], geoNode->vertexBuffer()[offset_c + 1], geoNode->vertexBuffer()[offset_c + 2]);
							}
							else {
								p[offset] = LuxPoint(geo->getVertex(a).x, geo->getVertex(a).y, geo->getVertex(a).z);
								p[offset + 1] = LuxPoint(geo->getVertex(b).x, geo->getVertex(b).y, geo->getVertex(b).z);
								p[offset + 2] = LuxPoint(geo->getVertex(c).x, geo->getVertex(c).y, geo->getVertex(c).z);
							}

							auto auv = uvMap[a];
							auto buv = uvMap[b];
							auto cuv = uvMap[c];
							
							uvs[uv_offset] = LuxUV(auv.x, auv.y);
							uvs[uv_offset + 1] = LuxUV(buv.x, buv.y);
							uvs[uv_offset + 2] = LuxUV(cuv.x, cuv.y);

							/*if (i < 100) {
								std::stringstream ss;
								ss << std::fixed << a << ": " << auv.x << ", " << auv.y << std::endl;
								ss << std::fixed << b << ": " << buv.x << ", " << buv.y << std::endl;
								ss << std::fixed << c << ": " << cuv.x << ", " << cuv.y << std::endl << std::endl;

								appendToFile("c:/test/uv_test.txt", ss.str());
							}*/

							if (normals != NULL) {
								normals[norm_offset] = recalc_normals[a].x;
								normals[norm_offset + 1] = recalc_normals[a].y;
								normals[norm_offset + 2] = recalc_normals[a].z;

								normals[norm_offset + 3] = recalc_normals[b].x;
								normals[norm_offset + 4] = recalc_normals[b].y;
								normals[norm_offset + 5] = recalc_normals[b].z;

								normals[norm_offset + 6] = recalc_normals[c].x;
								normals[norm_offset + 7] = recalc_normals[c].y;
								normals[norm_offset + 8] = recalc_normals[c].z;
							}

							t[i] = LuxTriangle(offset, offset + 1, offset + 2);

						}

						std::string id = std::to_string(id_gen.getID());
						std::string shapeName = std::string("shape") + id;
						std::string subdivShapeName = std::string("subdiv_shape") + id;
						std::string displacementShapementName = std::string("displacement_shape") + id;
						std::string itemName  = std::string("item")  + id;

						scene->DefineMesh(shapeName, vertCount, polyCount, (float*)p, (unsigned int*)t, normals, (float*)uvs, NULL, NULL);
					
						Properties props;

						std::string instanceShape = shapeName;
						float *xPtr = &finalTransform[0][0];
						std::vector<float> xfinalTransform =
							{ 
							xPtr[0], xPtr[1], xPtr[2], xPtr[3], 
							xPtr[4], xPtr[5], xPtr[6], xPtr[7], 
							xPtr[8], xPtr[9], xPtr[10], xPtr[11], 
							xPtr[12], xPtr[13], xPtr[14], xPtr[15] 
						};

						if (enableSubdivsion) {

							scene->Parse(Properties().SetFromString(
								"scene.shapes." + subdivShapeName + ".type = subdiv\n"
								"scene.shapes." + subdivShapeName + ".source = " + shapeName + "\n"
								"scene.shapes." + subdivShapeName + ".maxlevel = " + std::to_string(subdivMaxLevel) + "\n"
								"scene.shapes." + subdivShapeName + ".maxedgescreensize = " + std::to_string(subdivMaxEdge) + "\n"
							));

							if (!displacement.empty()) {
								std::string _dtype = "height";
								if (disp_type == 1) _dtype = "vector";

								Properties prop00;
								prop00
									<< Property("scene.shapes." + displacementShapementName + ".type")("displacement")
									<< Property("scene.shapes." + displacementShapementName + ".source")(subdivShapeName)
									<< Property("scene.shapes." + displacementShapementName + ".map")(displacement)
									<< Property("scene.shapes." + displacementShapementName + ".map.type")(_dtype)
									<< Property("scene.shapes." + displacementShapementName + ".scale")(disp_scale)
									<< Property("scene.shapes." + displacementShapementName + ".offset")(disp_offset)
									<< Property("scene.shapes." + displacementShapementName + ".normalsmooth")(useNormalSmooothing);

								scene->Parse(prop00);
								
								instanceShape = displacementShapementName;

								if (node->isRenderable()) {

									Properties prop01;
									prop01
										<< Property("scene.objects." + itemName + ".shape")(displacementShapementName)
										<< Property("scene.objects." + itemName + ".material")(material)
										<< Property("scene.objects." + itemName + ".transformation")(xfinalTransform);
									
									scene->Parse(prop01);
								}
							}
							else {

								instanceShape = displacementShapementName;

								if (node->isRenderable()) {

									Properties prop01;
									prop01
										<< Property("scene.objects." + itemName + ".shape")(subdivShapeName)
										<< Property("scene.objects." + itemName + ".material")(material)
										<< Property("scene.objects." + itemName + ".transformation")(xfinalTransform);

									scene->Parse(prop01);
								}
							}
						}
						else {

							if (node->isRenderable()) {
								Properties prop01;
								prop01
									<< Property("scene.objects." + itemName + ".shape")(shapeName)
									<< Property("scene.objects." + itemName + ".material")(material)
									<< Property("scene.objects." + itemName + ".transformation")(xfinalTransform);

								scene->Parse(prop01);
							}
						}					

						if (node->isRenderable()) {
							scene->Parse(props);
						}
											
						DfxLuxObjectSegment sinfo;
						sinfo.surf_id = segment->surf_id;
						sinfo.id = id;
						sinfo.material = material;
						sinfo.objectName = itemName;
						sinfo.shapeName = shapeName;
						sinfo.transform = xfinalTransform;

						segmentInfo.push_back(sinfo);

						if (instItr != _instanceShapes.end()) {
							DfxLuxObjectSegment sinfoInstance = sinfo;
							sinfoInstance.shapeName = instanceShape;
							instItr->second.push_back(sinfoInstance);
						}
					}
					
					delete segment;
				}
				objectSegments[node->referencePath()] = segmentInfo;
				objectTransform[node->referencePath()] = finalTransform;

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

		findInstancePaths();
		deepsg::SgRootNode::getInstance()->traverseSubSceneConditionally(branch, renderFunc, rootTransform());
		//traverseSubScene(branch, renderFunc);
	}
	catch (std::runtime_error &err) {
		LC_LOG("LuxCore Object RUNTIME ERROR: " << err.what());
		return false;
	}
	catch (std::exception &err) {
		LC_LOG("LuxCore Object ERROR: " << err.what());
		return false;
	}

	return true;
}

bool DfxLuxCoreRenderSession::loadInstances() {

	std::unordered_map<std::string, std::vector<glm::mat4>> _transforms;
	size_t instID = 0xff;

	// Collect Transforms
	for (auto instancer : DfxInstanceManager::getInstance()->intancers()) {

		std::unordered_map<size_t, DfxQRenderInstanceInfo> instanceData;

		instancer->begin();

		if (instancer->isEnabled()) {

			for (size_t inx = 0; inx < instancer->instanceCount(); inx++) {

				auto node = instancer->instanceShape(inx);
				
				if (node != nullptr) {
					auto instItr = _instanceShapes.find(node->referencePath());
					auto trans = _transforms.find(node->referencePath());

					if (instItr != _instanceShapes.end()) {

						glm::mat4 xform(1.f);

						if (instancer->useTransformMatrix(inx))
							xform = instancer->instanceTransform(inx);
						else
							xform = instancer->finalTransform(inx);

						if (trans == _transforms.end()) {
							_transforms[node->referencePath()] = std::vector<glm::mat4>{ xform };
						}else{
							trans->second.push_back(xform);
						}				
					
					}
				}
			}

		}

		instancer->end();
	}

	//Add Instances
	for (auto instancer : DfxInstanceManager::getInstance()->intancers()) {

		std::unordered_map<size_t, DfxQRenderInstanceInfo> instanceData;

		instancer->begin();

		if (instancer->isEnabled()) {

			for (auto &gform: _transforms) {

				auto instItr = _instanceShapes.find(gform.first);
				auto trans = _transforms.find(gform.first);

				if (instItr != _instanceShapes.end() && trans != _transforms.end()) {

					if (trans->second.size() > 0) {

						auto xform = rootTransform() * trans->second[0];
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

							if (trans->second.size() > 1) {
								float *matArray = new float[(trans->second.size() - 1) * 16];

								for (long int i = 0; i < trans->second.size() - 1; i++) {
									long int os = i * 16;

									auto xf = rootTransform() * trans->second[i + 1];
									xPtr = &xf[0][0];
									matArray[os] = xPtr[0]; matArray[os + 1] = xPtr[1]; matArray[os + 2] = xPtr[2]; matArray[os + 3] = xPtr[3];
									matArray[os + 4] = xPtr[4]; matArray[os + 5] = xPtr[5]; matArray[os + 6] = xPtr[6]; matArray[os + 7] = xPtr[7];
									matArray[os + 8] = xPtr[8]; matArray[os + 9] = xPtr[9]; matArray[os + 10] = xPtr[10]; matArray[os + 11] = xPtr[11];
									matArray[os + 12] = xPtr[12]; matArray[os + 13] = xPtr[13]; matArray[os + 14] = xPtr[14]; matArray[os + 15] = xPtr[15];
								}

								scene->DuplicateObject(itemName, itemName + "_copy", trans->second.size() - 1, matArray);
								delete[] matArray;
							}
						}

					}
				}
				
			}

		}

		instancer->end();
	}

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

bool DfxLuxCoreRenderSession::loadLights(deepsg::SgNode *branch) {
	try {
	
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

				if (node->uniqueName() == DfxLxPointLightWidget::getUniqueName()) {
					DfxLxPointLightHandler handler;
					auto rc = handler.fetch(node->referencePath());

					if (rc) {

						std::string propStr = std::string("scene.lights.") + handler.dataObject()->name() + ".";
						auto dao = handler.dataObject()->rawData<fl::DataObject>();

						float r = dao->property("color_r")->data<float>();
						float g = dao->property("color_g")->data<float>();
						float b = dao->property("color_b")->data<float>();
						float gain = dao->property("gain")->data<float>();
						float power = dao->property("power")->data<float>();
						float efficacy = dao->property("efficacy")->data<float>();

						scene->Parse(
							Property(propStr + "type")("point") <<
							Property(propStr + "gain")(gain, gain, gain) <<
							Property(propStr + "color")(r, g, b) <<
							Property(propStr + "power")(power) <<
							Property(propStr + "efficency")(efficacy) <<
							Property(propStr + "position")(pos.x, pos.y, pos.z)
						);

						auto itrExLight = existingLights.find(node->referencePath());

						if (itrExLight == existingLights.end()) {
							existingLights[node->referencePath()] = "";

							auto xform = handler.xform()->rawData<fl::DataObject>();;

							addLightObserver(dao->property("color_r"));
							addLightObserver(dao->property("gain"));
							addLightObserver(dao->property("power"));
							addLightObserver(dao->property("efficacy"));
			               /*addLightObserver(xform->property("x_pos"));
							addLightObserver(xform->property("y_pos"));
							addLightObserver(xform->property("z_pos"));*/
						}

					}
				}
				else if (node->uniqueName() == DfxLxSunSkyLightWidget::getUniqueName()) {
					DfxLxSunSkyLightHandler handler;
					auto rc = handler.fetch(node->referencePath());

					if (rc) {

						std::string propSunStr = std::string("scene.lights.sun_") + handler.dataObject()->name() + ".";
						std::string propSkyStr = std::string("scene.lights.sky_") + handler.dataObject()->name() + ".";

						auto dao = handler.dataObject()->rawData<fl::DataObject>();

						bool use_sun = dao->property("use_sun")->data<bool>();
						bool use_sky = dao->property("use_sky")->data<bool>();
						float turbidity = dao->property("turbidity")->data<float>();
						float dir_x = dao->property("dir_x")->data<float>();
						float dir_y = dao->property("dir_y")->data<float>();
						float dir_z = dao->property("dir_z")->data<float>();
						bool diff = dao->property("diffuse")->data<bool>();
						bool glos = dao->property("glossy")->data<bool>();
						bool spec = dao->property("specular")->data<bool>();
						float gain = dao->property("gain")->data<float>();
						float sky_gain = dao->property("sky_gain")->data<float>();
						float albedo_r = dao->property("sky_ground_albedo_r")->data<float>();
						float albedo_g = dao->property("sky_ground_albedo_g")->data<float>();
						float albedo_b = dao->property("sky_ground_albedo_b")->data<float>();
						float sun_relative_size = dao->property("sun_relative_size")->data<float>();

						glm::vec3 scale;
						glm::quat rotation;
						glm::vec3 pos;
						glm::vec3 skew;
						glm::vec4 perspective;
						glm::decompose(info->transformMatrix(), scale, rotation, pos, skew, perspective);

						auto dir = glm::normalize(glm::vec3(glm::toMat4(glm::conjugate(rotation)) * glm::vec4(0.f, 1.f, 0.f, 1.f)));
						
						Properties props;
						if (use_sky) {
							props <<
								Property(propSkyStr + "type")("sky2") <<
								Property(propSkyStr + "gain")(sky_gain, sky_gain, sky_gain)   <<
								Property(propSkyStr + "turbidity")(turbidity) <<
								Property(propSkyStr + "relsize")(sun_relative_size) <<
								Property(propSkyStr + "dir")(dir.x, dir.y, dir.z)   <<
								Property(propSkyStr + "visibility.indirect.diffuse.enable")(diff) <<
								Property(propSkyStr + "visibility.indirect.glossy.enable")(glos)  <<
								Property(propSkyStr + "visibility.indirect.specular.enable")(spec) <<
								Property(propSkyStr + "groundalbedo")(albedo_r, albedo_g, albedo_b);
						}

						if (use_sun) {
							props <<
								Property(propSunStr + "type")("sun") <<
								Property(propSunStr + "gain")(gain, gain, gain) <<
								Property(propSunStr + "turbidity")(turbidity) <<
								Property(propSunStr + "relsize")(sun_relative_size) <<
								Property(propSunStr + "dir")(dir.x, dir.y, dir.z) <<
								Property(propSunStr + "visibility.indirect.diffuse.enable")(diff) <<
								Property(propSunStr + "visibility.indirect.glossy.enable")(glos)  <<
								Property(propSunStr + "visibility.indirect.specular.enable")(spec);
						}

						DfxLxRenderHandler lxhandler;
						if (lxhandler.fetch(_luxcoreRenderInfoPath)) {

							std::string cache_enableStr = _luxcoreRenderInfoPath + "/luxcore_enviroment_light_cache/enable";
							std::string cache_qualityStr = _luxcoreRenderInfoPath + "/luxcore_enviroment_light_cache/quality";

							auto cache_enable = DfxSpace::getInstance()->findAbsolute(cache_enableStr)->data<bool>();
							auto cache_quality = DfxSpace::getInstance()->findAbsolute(cache_qualityStr)->data<float>();

							if (cache_enable) {
								props
									<< Property(propSkyStr + "visibilitymapcache.enable")(true)
									<< Property(propSkyStr + "visibilitymapcache.map.quality")(cache_quality)
									<< Property(propSkyStr + "visibilitymapcache.map.tilewidth")(0)
									<< Property(propSkyStr + "visibilitymapcache.map.tileheight")(0)
									<< Property(propSkyStr + "visibilitymapcache.map.tilesamplecount")(1)
									<< Property(propSkyStr + "visibilitymapcache.map.sampleupperhemisphereonly")(false)
									<< Property(propSkyStr + "visibilitymapcache.visibility.maxsamplecount")(11048576)
									<< Property(propSkyStr + "visibilitymapcache.visibility.maxdepth")(4)
									<< Property(propSkyStr + "visibilitymapcache.visibility.targethitrate ")(0.99000001)
									<< Property(propSkyStr + "visibilitymapcache.visibility.radius")(0)
									<< Property(propSkyStr + "visibilitymapcache.visibility.normalangle")(25);
							}
						}

						scene->Parse(props);

						//writeToFile("c:/test/ptest.txt", props.ToString());

						auto itrExLight = existingLights.find(node->referencePath());

						if (itrExLight == existingLights.end()) {
							existingLights[node->referencePath()] = "";

							auto xform = handler.xform()->rawData<fl::DataObject>();

							addLightObserver(dao->property("use_sun"));
							addLightObserver(dao->property("use_sky"));
							addLightObserver(dao->property("turbidity"));
							addLightObserver(dao->property("dir_x"));
							addLightObserver(dao->property("dir_y"));
							addLightObserver(dao->property("dir_z"));
							addLightObserver(dao->property("diffuse"));
							addLightObserver(dao->property("glossy"));
							addLightObserver(dao->property("specular"));
							addLightObserver(dao->property("gain"));
							addLightObserver(dao->property("sky_ground_albedo_r"));
							addLightObserver(dao->property("sun_relative_size"));
							//addLightObserver(xform->property("x_pos"));
							//addLightObserver(xform->property("y_pos"));
							//addLightObserver(xform->property("z_pos"));
						}
					}
				}
				else if (shapeNode->shapeType() == DfxItemWidgetType::ENVIRONMENT_LIGHT) {

					DfxLxEnvironmentLightHandler handler;
					auto rc = handler.fetch(node->referencePath());

					if (rc) {
						std::string propStr = std::string("scene.lights.") + handler.dataObject()->name() + ".";
						auto dao = handler.dataObject()->rawData<fl::DataObject>();

						auto gain  = dao->property("gain")->data<float>();
						auto gamma = dao->property("gamma")->data<float>();
						auto image = dao->property("image")->data<std::string>();
						auto diffuse = dao->property("diffuse")->data<bool>();
						auto glossy   = dao->property("glossy")->data<bool>();
						auto specular = dao->property("specular")->data<bool>();
						auto upper    = dao->property("upper_hemisphere")->data<bool>();

						std::vector<float> matArray;
						auto rotmat = glm::rotate(glm::mat4(1.f), glm::radians(rot.y), glm::vec3(0.f, 0.f, 1.f));
						float *pmat = (float*)glm::value_ptr(rotmat);
						for (int i = 0; i < 16; i++) matArray.push_back(pmat[i]);

						Properties props;
						props <<
							Property(propStr + "type")("infinite") <<
							Property(propStr + "gain")(gain, gain, gain) <<
							Property(propStr + "gamma")(gamma) <<
							Property(propStr + "file")(image) <<
							Property(propStr + "diffuse.enable")(diffuse) <<
							Property(propStr + "glossy.enable")(glossy) <<
							Property(propStr + "specular.enable")(specular) <<
							Property(propStr + "sampleupperhemisphereonly")(upper) <<
							Property(propStr + "transformation")(matArray);

						DfxLxRenderHandler lxhandler;
						if (lxhandler.fetch(_luxcoreRenderInfoPath)) {
	
							std::string cache_enableStr = _luxcoreRenderInfoPath + "/luxcore_enviroment_light_cache/enable";
							std::string cache_qualityStr = _luxcoreRenderInfoPath + "/luxcore_enviroment_light_cache/quality";

							auto cache_enable = DfxSpace::getInstance()->findAbsolute(cache_enableStr)->data<bool>();
							auto cache_quality = DfxSpace::getInstance()->findAbsolute(cache_qualityStr)->data<float>();

							if (cache_enable) {
								props
									<< Property(propStr + "visibilitymapcache.enable")(true)
									<< Property(propStr + "visibilitymapcache.map.quality")(cache_quality)
									<< Property(propStr + "visibilitymapcache.map.tilewidth")(0)
									<< Property(propStr + "visibilitymapcache.map.tileheight")(0)
									<< Property(propStr + "visibilitymapcache.map.tilesamplecount")(1)
									<< Property(propStr + "visibilitymapcache.map.sampleupperhemisphereonly")(false)
									<< Property(propStr + "visibilitymapcache.visibility.maxsamplecount")(11048576)
									<< Property(propStr + "visibilitymapcache.visibility.maxdepth")(4)
									<< Property(propStr + "visibilitymapcache.visibility.targethitrate ")(0.99000001)
									<< Property(propStr + "visibilitymapcache.visibility.radius")(0)
									<< Property(propStr + "visibilitymapcache.visibility.normalangle")(25);
							}
						}

						scene->Parse(props);

						auto itrExLight = existingLights.find(node->referencePath());

						if (itrExLight == existingLights.end()) {
							existingLights[node->referencePath()] = "";

							auto xform = handler.xform()->rawData<fl::DataObject>();;

							addLightObserver(dao->property("gain"));
							addLightObserver(dao->property("image"));
							addLightObserver(dao->property("diffuse"));
							addLightObserver(dao->property("glossy"));
							addLightObserver(dao->property("specular"));
							addLightObserver(dao->property("upper_hemisphere"));
							//addLightObserver(xform->property("x_pos"));
							//addLightObserver(xform->property("y_pos"));
							//addLightObserver(xform->property("z_pos"));
						}

					}
				}
			}
		};
		
		deepsg::SgRootNode::getInstance()->traverseScene(lightFunc, rootTransform());
	}
	catch (std::runtime_error &err) {
		LC_LOG("RUNTIME ERROR: " << err.what());
		return false;
	}
	catch (std::exception &err) {
		LC_LOG("ERROR: " << err.what());
		return false;
	}

	return true;
}

bool DfxLuxCoreRenderSession::loadCamera(deepsg::SgNode *branch) {
	
	try {

		glm::vec3 origin(0.f, 0.f, -10.05f);
		glm::vec3 target(0.f, 0.f, .0f);
		glm::vec3 up(0.f, 0.f, 1.0f);
		
		bool use_dof = false;
		float fov = 0.f;
		float focalDistance = 0.f;
		float lensSize = 0.f;
		float lensRadius = 0.f;
		std::string helper;

		deepsg::SgTraversalFunction renderFunc = [this, &helper, &lensRadius, &focalDistance, &lensSize, &fov, &use_dof, &up, &origin, &target](deepsg::SgNode *node, deepsg::SgStateInfo *info) {
			if (node->getType() == deepsg::SgNodeType::SHAPE) {
				auto shapeNode = (deepsg::SgShapeNode*)node;

				auto referencePath = shapeNode->referencePath();

				if (shapeNode->referencePath() == cameraPath) {
					if (shapeNode->shapeType() == DfxItemWidgetType::CAMERA) {
						DfxLxCameraHandler handler;
						
						glm::vec3 scale;
						glm::quat rotation;
						glm::vec3 pos;
						glm::vec3 skew;
						glm::vec4 perspective;
						glm::decompose(info->transformMatrix(), scale, rotation, pos, skew, perspective);

						auto dir = glm::normalize(glm::vec3(glm::toMat4(glm::conjugate(rotation)) * glm::vec4(0.f, 1.f, 0.f, 1.f)));

						origin = glm::vec3(info->transformMatrix() * glm::vec4(0.f, 0.f, 0.f, 1.f));
						target = glm::vec3(info->transformMatrix() * glm::vec4(0.f, 0.f, -1.f, 1.f));
						up = glm::normalize(glm::vec3(glm::toMat4(glm::conjugate(rotation)) * glm::vec4(0.f, 1.f, 0.f, 1.f)));

						if (handler.fetch(referencePath)) {
							auto dao = handler.dataObject()->rawData<fl::DataObject>();

							use_dof = dao->property("use_dof")->data<bool>();
							fov = dao->property("field_of_view")->data<float>();
							focalDistance = dao->property("focal_distance")->data<float>();
							lensSize = dao->property("lens_size")->data<float>();
							helper = dao->property("dof_helper")->data<std::string>();

							auto fstop = dao->property("dof_fstop")->data<float>();
							auto focalLength = fovToFocalLength(fov, lensSize);
							lensRadius = (focalLength / 1000.f) / (2.f * fstop);

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
	
		deepsg::SgRootNode::getInstance()->traverseSubScene(branch, renderFunc, rootTransform());
		deepsg::SgRootNode::getInstance()->traverseSubScene(branch, helperFunc, rootTransform());

		Properties props;

	
		if (use_dof) {
			props 
				<< Property("scene.camera.focaldistance")(focalDistance) 
				<< Property("scene.camera.lensradius")(lensRadius);
		}
		
		props <<
			Property("scene.camera.lookat.orig")(origin.x, origin.y, origin.z) <<
			Property("scene.camera.lookat.target")(target.x, target.y, target.z) <<
			Property("scene.camera.up")(up.x, up.y, up.z) <<
			Property("scene.camera.fieldofview")(fov);

		scene->Parse(props);

		for (auto obs : cameraObservers) {
			delete obs;
		}
		cameraObservers.clear();	

		DfxLxCameraHandler handler;
		handler.fetch(cameraPath);

		auto xform = handler.xform()->rawData<fl::DataObject>();

		/*addCameraObserver(xform->property("x_pos"));
		addCameraObserver(xform->property("y_pos"));
		addCameraObserver(xform->property("z_pos"));
		addCameraObserver(xform->property("x_rot"));
		addCameraObserver(xform->property("y_rot"));
		addCameraObserver(xform->property("z_rot"));*/
	}
	catch (std::runtime_error &err) {
		LC_LOG("LuxCore Camera RUNTIME ERROR: " << err.what());
		return false;
	}
	catch (std::exception &err) {
		LC_LOG("LuxCore Camera ERROR: " << err.what());
		return false;
	}

	return true;
}

bool DfxLuxCoreRenderSession::loadVolumes(deepsg::SgNode *branch) {
	int vcounter = 0;
	deepsg::SgConditionalTraversalFunction renderFunc = [this, &vcounter](deepsg::SgNode *node, deepsg::SgStateInfo *info)-> deepsg::TraversalFlag {

		if (node->getType() == deepsg::SgNodeType::GRID ) {
			DfxGridHandler handler;

			if (!node->isRenderable()) {
				return deepsg::TraversalFlag::IGNORE_CHILDREN;
			}

			if (handler.fetch(node->referencePath())) {
				auto dao = handler.dataObject()->rawData<fl::DataObject>();

				auto use_render_hint = dao->property("use_render_hint")->data<bool>();
				auto cull_out_of_bounds = dao->property("cull_out_of_bounds")->data<bool>();

				auto material_id = dao->property("material_id")->data<unsigned int>();

				auto voxel_size = dao->property("voxel_size")->data<float>();
				auto width  = dao->property("width")->data<unsigned int>();
				auto height = dao->property("height")->data<unsigned int>();
				auto depth  = dao->property("depth")->data<unsigned int>();

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
				voxel_size = adapter->channel(0)->voxelSize();

				MAX_DIM = std::max(dim.x, std::max(dim.y, dim.z));

			/*	auto XDIM = dim.x;
				auto YDIM = dim.y;
				auto ZDIM = dim.z;*/

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

				std::vector<float> pDataVec(pData.get(), pData.get() + (XDIM*YDIM*ZDIM));

				auto wmax = voxel_size * XDIM;
				auto hmax = voxel_size * YDIM;
				auto dmax = voxel_size * ZDIM;

				glm::ivec3 seg(1, 1, 1);

				glm::dvec3 si(wmax, hmax, dmax);

				generator::BoxMesh box(si, seg);
				auto vertices = box.vertices();
				auto faces = box.triangles();

				std::vector<LuxPoint> vertArray;
				std::vector<LuxTriangle> faceArray;
				std::vector<LuxUV> texCoord;

				while (!vertices.done()) {
					auto pt = vertices.generate();

					pt.position += glm::vec3(wmax, hmax, dmax);
					vertArray.push_back(LuxPoint(pt.position.x, pt.position.y, pt.position.z));
					
					texCoord.push_back(LuxUV(pt.texCoord.x, pt.texCoord.y));
					
					vertices.next();
				}		

				while (!faces.done()) {
					auto triangle = faces.generate();
					faceArray.push_back(LuxTriangle(triangle.vertices.x, triangle.vertices.y, triangle.vertices.z));
					faces.next();
				}

				std::string id = std::to_string(id_gen.getID());
				std::string shapeName = std::string("volumeShape") + id;
				std::string objectName = std::string("volumeObject") + id;
				std::string gridName = std::string("volumeGrid") + id;
				std::string volumeName = std::string("volume") + id;
				std::string volumeMatName = std::string("volumeMatName") + id;

				LuxPoint *p = (LuxPoint*)luxcore::Scene::AllocVerticesBuffer(vertArray.size());
				LuxTriangle *t = (LuxTriangle*)luxcore::Scene::AllocTrianglesBuffer(faceArray.size());
				LuxUV *uvs = new LuxUV[texCoord.size()];

				for (int i = 0; i < vertArray.size(); i++) {
					p[i] = vertArray[i];
					uvs[i] = texCoord[i];
				}

				for (int i = 0; i < faceArray.size(); i++) {
					t[i] = faceArray[i];
				}

				float *xPtr = &finalTransform[0][0];
				std::vector<float> xfinalTransform =
				{
				xPtr[0], xPtr[1], xPtr[2], xPtr[3],
				xPtr[4], xPtr[5], xPtr[6], xPtr[7],
				xPtr[8], xPtr[9], xPtr[10], xPtr[11],
				xPtr[12], xPtr[13], xPtr[14], xPtr[15]
				};
				
				auto mapXform = glm::rotate(glm::mat4(1.0f), glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
				mapXform *= glm::scale(glm::vec3(1/wmax, 1/hmax, 1/dmax));
				xPtr = &mapXform[0][0];
				std::vector<float> xmapTransform =
				{
				xPtr[0], xPtr[1], xPtr[2], xPtr[3],
				xPtr[4], xPtr[5], xPtr[6], xPtr[7],
				xPtr[8], xPtr[9], xPtr[10], xPtr[11],
				xPtr[12], xPtr[13], xPtr[14], xPtr[15]
				};

				scene->DefineMesh(shapeName, vertArray.size(), faceArray.size(), (float*)p, (unsigned int*)t, nullptr, (float*)uvs, NULL, NULL);

				Properties gridProperties;
				gridProperties
					<< Property("scene.textures." + gridName + ".type")("densitygrid")
					<< Property("scene.textures." + gridName + ".nx")(XDIM)
					<< Property("scene.textures." + gridName + ".ny")(YDIM)
					<< Property("scene.textures." + gridName + ".nz")(ZDIM)
					<< Property("scene.textures." + gridName + ".wrap")("black")
					<< Property("scene.textures." + gridName + ".data")(pDataVec)
					<< Property("scene.textures." + gridName + ".mapping.type")("globalmapping3d")
					<< Property("scene.textures." + gridName + ".mapping.transformation")(xmapTransform);
				scene->Parse(gridProperties);

				Properties gridScaleProperties;
				gridScaleProperties
					<< Property("scene.textures." + id + "_scale" + ".type")("scale")
					<< Property("scene.textures." + id + "_scale" + ".texture1")(10.0f)
					<< Property("scene.textures." + id + "_scale" + ".texture2")(gridName);
				scene->Parse(gridScaleProperties);

				Properties volumeProperties;
				volumeProperties
					<< Property("scene.volumes." + volumeName + ".type")("heterogeneous")
					<< Property("scene.volumes." + volumeName + ".absorption")(0, 0, 0)
					<< Property("scene.volumes." + volumeName + ".scattering")(id + "_scale")
					<< Property("scene.volumes." + volumeName + ".asymmetry")(0, 0, 0)
					<< Property("scene.volumes." + volumeName + ".multiscattering")(0)
					<< Property("scene.volumes." + volumeName + ".steps.size")(0.01)
					<< Property("scene.volumes." + volumeName + ".steps.maxcount")(2048)
					<< Property("scene.volumes." + volumeName + ".priority")(vcounter++)
					<< Property("scene.volumes." + volumeName + ".photongi.enable")(0)
					<< Property("scene.volumes." + volumeName + ".ior")(1.5f);
				scene->Parse(volumeProperties);

				Properties volumeMatProperties;
				volumeMatProperties
					<< Property("scene.materials." + volumeMatName + ".type")("null")
					<< Property("scene.materials." + volumeMatName + ".kd")(0.7, 0.7, 0.7)
					<< Property("scene.materials." + volumeMatName + ".transparency.shadow")(0, 0, 0)
					<< Property("scene.materials." + volumeMatName + ".emission.gain")(1, 1, 1)
					<< Property("scene.materials." + volumeMatName + ".emission.power")(0)
					<< Property("scene.materials." + volumeMatName + ".emission.normalizebycolor")(1)
					<< Property("scene.materials." + volumeMatName + ".emission.efficency")(0)
					<< Property("scene.materials." + volumeMatName + ".emission.theta ")(90)
					<< Property("scene.materials." + volumeMatName + ".emission.importance")(1)
					<< Property("scene.materials." + volumeMatName + ".emission.temperature")(-1)
					<< Property("scene.materials." + volumeMatName + ".emission.temperature.normalize")(0)
					<< Property("scene.materials." + volumeMatName + ".emission.directlightsampling.type")("AUTO")
					<< Property("scene.materials." + volumeMatName + ".bumpsamplingdistance")(0.001)
					<< Property("scene.materials." + volumeMatName + ".volume.interior")(volumeName)
					<< Property("scene.materials." + volumeMatName + ".photongi.enable")(1);
				scene->Parse(volumeMatProperties);

				Properties objectProperties;
				objectProperties
					<< Property("scene.objects." + objectName + ".shape")(shapeName)
					<< Property("scene.objects." + objectName + ".material")(volumeMatName)
					<< Property("scene.objects." + objectName + ".transformation")(xfinalTransform);

				scene->Parse(objectProperties);

			
				//rkcommon::math::affine3f xfm = glmMat4ToAffine3(finalTransform);

				//// Copy mesh data to OSPRay
				//auto _vol = new OSPRayVolume;
				//_vol->volume.reset(new ospray::cpp::Volume("structuredRegular"));
				//_vol->volume->setParam("data", ospray::cpp::CopiedData(pData.get(), rkcommon::math::vec3ul(XDIM, YDIM, ZDIM)));
				//_vol->volume->setParam("gridSpacing", rkcommon::math::vec3f(0.1f, 0.1f, 0.1f));
				//_vol->volume->commit();

				//std::vector<rkcommon::math::vec3f> colors;
				//std::vector<float> opacities;

				//colors.emplace_back(1.f, 0.f, 0.f);
				//colors.emplace_back(0.f, 0.f, 1.f);
				//colors.emplace_back(0.f, 1.f, 0.f);

				//opacities.emplace_back(0.f);
				////opacities.emplace_back(0.7f);
				//opacities.emplace_back(1.f);

				//ospray::cpp::TransferFunction transferFunction("piecewiseLinear");

				//transferFunction.setParam("color", ospray::cpp::CopiedData(colors));
				//transferFunction.setParam("opacity", ospray::cpp::CopiedData(opacities));
				//transferFunction.setParam("valueRange", rkcommon::math::vec2f(0.f, 1.f));
				//transferFunction.commit();

				//_vol->model.reset(new ospray::cpp::VolumetricModel(*_vol->volume));
				//_vol->model->setParam("transferFunction", transferFunction);
				//_vol->model->setParam("densityScale", 5.f);
				//_vol->model->commit();

				//_vol->group.reset(new ospray::cpp::Group());
				//_vol->group->setParam("volume", ospray::cpp::CopiedData(*_vol->model));
				//_vol->group->commit();

				//_vol->instance.reset(new ospray::cpp::Instance(*_vol->group));
				////_vol->instance->setParam("transform", xfm);
				//_vol->instance->commit();

				//_ospVolumes.push_back(_vol);

				return deepsg::TraversalFlag::CONTINUE;
			}
			else {
				return deepsg::TraversalFlag::CONTINUE;
			}
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

	deepsg::SgRootNode::getInstance()->traverseSubSceneConditionally(branch, renderFunc, rootTransform());

	return true;
}

/// LuxCore Specific functions
void DfxLuxCoreRenderSession::startRenderSession() {
	auto space = DfxSpace::getInstance();

	try {
		render_session->Start();
		newBufferRefreshTimer(120);

		DfxLxRenderHandler handler;

		rtHaltTime = 10000;
		if (handler.fetch(_luxcoreRenderInfoPath)) {
			DfxLxCameraHandler camHandler;
			std::string haltStr = _luxcoreRenderInfoPath + "/luxcore_halt_conditions";
			fl::DataObject *haltObj;

			auto haltAny = space->findAbsolute(haltStr);

			if (haltAny == nullptr) {
				DfxGlobal::console::println("LuxCore RT Path Engine Config Invalid path: " + haltStr);
				return;
			}
			else {
				haltObj = haltAny->rawData<fl::DataObject>();
			}

			rtHaltTime = (float)haltObj->property("ipr_timed_samples")->data<int>() * 1000;
		}

		newHaltTimer(rtHaltTime);

		_sessionUpdateListener = new DfxRenderSessionUpdateListener(this);
	}
	catch (std::runtime_error &err) {
		LC_LOG("LuxCore Camera RUNTIME ERROR: " << err.what());
		//return false;
	}
	catch (std::exception &err) {
		LC_LOG("LuxCore Camera ERROR: " << err.what());
		//return false;
	}
}

void DfxLuxCoreRenderSession::stopRenderSession() {
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

	delete _sessionUpdateListener;
	_sessionUpdateListener = nullptr;

	render_session->Stop();
}

bool DfxLuxCoreRenderSession::isStarted() {
	return render_session->IsStarted();
}

void DfxLuxCoreRenderSession::setRenderEngine(int rengine) {
	_renderEngine = rengine;
}

void DfxLuxCoreRenderSession::setCameraPath(std::string path) {
	cameraPath = path;
}
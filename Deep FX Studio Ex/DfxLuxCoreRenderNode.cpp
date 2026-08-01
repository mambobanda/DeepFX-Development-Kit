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
 /**
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/


#include "DfxLuxCoreRenderNode.h"
#include "DfxRenderOutput.h"

std::string DfxLuxCoreRenderNode::_uniqueName = "UID_LUXCORE_RENDER_NODE";

DfxLuxCoreRenderNode::DfxLuxCoreRenderNode():DfxRenderNodeBase(DfxRenderTarget::LUXCORE) {

	_renderSession = nullptr;

	_renderingStill    = false;
	_renderingSequence = false;

	_varianceEstimate = 0.f;

	deviceQControlenums.push_back(std::pair<std::string, unsigned int>("CPU", Device::CPU));
	deviceQControlenums.push_back(std::pair<std::string, unsigned int>("OpenCl", Device::OPENCL));

	engineQControlenums.push_back(std::pair<std::string, unsigned int>("Path", Engine::PATH));
	//engineQControlenums.push_back(std::pair<std::string, unsigned int>("Bidir", Engine::BIDIR));

	samplerQControlenums.push_back(std::pair<std::string, unsigned int>("Sobal", Sampler::SOBOL));
	samplerQControlenums.push_back(std::pair<std::string, unsigned int>("Metropolis", Sampler::METROPOLIS));
	samplerQControlenums.push_back(std::pair<std::string, unsigned int>("Random", Sampler::RANDOM));

	haltQControlenums.push_back(std::pair<std::string, unsigned int>("Samples", Halt::SAMPLES));
	haltQControlenums.push_back(std::pair<std::string, unsigned int>("Time", Halt::TIME));

	pixelFilterQControlenums.push_back(std::pair<std::string, unsigned int>("Blackman-Harris", PixelFilter::BLACKMAN));
	pixelFilterQControlenums.push_back(std::pair<std::string, unsigned int>("Gaussian", PixelFilter::GAUSSIAN));
	pixelFilterQControlenums.push_back(std::pair<std::string, unsigned int>("Mitchell", PixelFilter::MITCHELL));
	pixelFilterQControlenums.push_back(std::pair<std::string, unsigned int>("None", PixelFilter::NONE));

	denoiserQControlenums.push_back(std::pair<std::string, unsigned int>("OIDN", Denoiser::OIDN));

	tonemapTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Linear", TonemapType::LINEAR));
	tonemapTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Camera Settings", TonemapType::CAMERA_SETTINGS));
	tonemapTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Reinhard", TonemapType::REINHARD));

	imageNamingConventionTypeQControlenums.push_back(std::pair<std::string, unsigned int>("prefix_buffername_0000", ImageNamingConvention::PREFIX_BUFFER_FRAME));
	imageNamingConventionTypeQControlenums.push_back(std::pair<std::string, unsigned int>("buffername_0000", ImageNamingConvention::BUFFER_FRAME));

	imageFileFormatTypeQControlenums.push_back(std::pair<std::string, unsigned int>(".PNG", ImageFileFormat::PNG));
	imageFileFormatTypeQControlenums.push_back(std::pair<std::string, unsigned int>(".JPEG", ImageFileFormat::JPEG));

}

DfxLuxCoreRenderNode::~DfxLuxCoreRenderNode() {
	DfxRenderHub::getInstance()->removeRenderNode(this);
	delete renderOutput();
	
	if (_renderSession != nullptr) delete _renderSession;
}

void DfxLuxCoreRenderNode::initRenderNode() {
	_renderOutput = new DfxRenderOutput(this);
	DfxRenderHub::getInstance()->addRenderNode(this);
}

std::string DfxLuxCoreRenderNode::uniqueName() {
	return _uniqueName;
}

std::string DfxLuxCoreRenderNode::getUniqueName() {
	return _uniqueName;
}

void DfxLuxCoreRenderNode::stopRendering() {
	if (_renderSession != nullptr) {
		_renderSession->forceStop();
		_renderingStill = false;
		_renderingSequence = false;
	}
}

void DfxLuxCoreRenderNode::stopAllRendering() {
	_renderingSequence = false;
	_renderingStill = false;
	stopRendering();
}

DfxLxCameraHandler DfxLuxCoreRenderNode::getCamera() {
	DfxLxCameraHandler handler;

	auto cameraAttr = DfxSpace::getInstance()->findAbsolute(referencePath() + "/luxcore_general/camera");
	auto cameraPath = cameraAttr->data<std::string>();

	if (handler.fetch(cameraPath)) {

		if (handler.dataObject()->rawData<fl::DataObject>()->hasProperty("item_type")) {
			
			if (handler.dataObject()->rawData<fl::DataObject>()->property("item_type")->data<unsigned int>() == DfxItemWidgetType::CAMERA) {
				if (handler.dataObject()->rawData<fl::DataObject>()->hasProperty("render_target")) {

					if (handler.dataObject()->rawData<fl::DataObject>()->property("render_target")->data<unsigned int>() == DfxRenderTarget::LUXCORE) {

						return handler;
					}
				}
			}
		}
	}

	return DfxLxCameraHandler();
}

void DfxLuxCoreRenderNode::renderFrame() {
	auto space = DfxSpace::getInstance();

	auto camHandler = getCamera();

	if (!camHandler.isNull()) {
	
		if (DeepFxStudioLicenseManager::getState() != DeepFxStudioLicenseManager::NODE_LOCKED_STATE) {
			auto w = camHandler.width();
			auto h = camHandler.height();

			if (w > 1280) {
				DfxLicenseInfoDialog::trialResolutionExceeded();
				return;
			}

			if (h > 720) {
				DfxLicenseInfoDialog::trialResolutionExceeded();
				return;
			}
		}

		auto lxRenderAny = space->findAbsolute(referencePath());

		auto device  = lxRenderAny->rawData<fl::DataObject>()->property("device")->data<unsigned int>();
		auto rengine = lxRenderAny->rawData<fl::DataObject>()->property("engine")->data<unsigned int>();

		int camWidth  = camHandler.dataObject()->data<fl::DataObject>().property("width")->data<unsigned int>();
		int camHeight = camHandler.dataObject()->data<fl::DataObject>().property("height")->data<unsigned int>();

		_renderSession = new DfxLuxCoreRenderSession;
		_renderSession->setCameraPath(camHandler.dataObject()->absolutePath());
		_renderSession->setLuxcoreRenderInfoPath(referencePath());
		_renderSession->setRenderOutput(_renderOutput);

		((DfxLuxCoreRenderSession*)_renderSession)->setRenderEngine(DfxLuxCoreRenderSession::PATH);

		if (!_renderSession->init()) {
			_renderSession->clear();
			return;
		}

		if (!_renderSession->loadScene(deepsg::SgRootNode::getInstance())) {
			_renderSession->clear();
			return;
		}

		if (!_renderSession->prepareSession()) {

			_renderSession->clear();
			return;
		}

		_renderSession->render();

		_varianceEstimate = _renderSession->estimatePathClampingVariance();
		emit varianceEstimateUpdated(QString("*Estimate: ") + QString(std::to_string(_varianceEstimate).c_str()));

		_renderSession->clear();
		delete _renderSession;
		_renderSession = nullptr;
	
		DfxGlobal::console::println("Luxcore Render Done!");

	}
	else {
		DfxGlobal::console::println("No Camera Assigned. Please assign a camera.");
	}

}

DfxRenderSessionInterface *DfxLuxCoreRenderNode::renderSession() {
	auto camHandler = getCamera();

	if (!camHandler.isNull()) {

		if (DeepFxStudioLicenseManager::getState() != DeepFxStudioLicenseManager::NODE_LOCKED_STATE) {
			auto w = camHandler.width();
			auto h = camHandler.height();

			if (w > 1280) {
				DfxLicenseInfoDialog::trialResolutionExceeded();
				return nullptr;
			}

			if (h > 720) {
				DfxLicenseInfoDialog::trialResolutionExceeded();
				return nullptr;
			}
		}

		auto rndSession = new DfxLuxCoreRenderSession;
		rndSession->setCameraPath(camHandler.dataObject()->absolutePath());
		rndSession->setLuxcoreRenderInfoPath(referencePath());
		rndSession->setRenderOutput(_renderOutput);
		return rndSession;
	}
	else {
		DfxGlobal::console::println("No Camera Assigned. Please assign a camera.");
	}

	return nullptr;
}

void DfxLuxCoreRenderNode::renderStill() {
	if (!_renderingStill && !_renderingSequence) {
		_renderingStill = true;
		_renderingSequence = false;

		renderFrame();
		saveStill();

		_renderingStill = false;
		_renderingSequence = false;
	}
}

void DfxLuxCoreRenderNode::debugExport() {
	auto space = DfxSpace::getInstance();

	auto camHandler = getCamera();

	if (!camHandler.isNull()) {

		auto lxRenderAny = space->findAbsolute(referencePath());

		int camWidth = camHandler.dataObject()->data<fl::DataObject>().property("width")->data<unsigned int>();
		int camHeight = camHandler.dataObject()->data<fl::DataObject>().property("height")->data<unsigned int>();

		_renderSession = new DfxLuxCoreRenderSession;
		_renderSession->setCameraPath(camHandler.dataObject()->absolutePath());
		_renderSession->setLuxcoreRenderInfoPath(referencePath());
		_renderSession->setRenderOutput(_renderOutput);

		if (!_renderSession->init()) {
			_renderSession->clear();
			return;
		}

		if (!_renderSession->loadScene(deepsg::SgRootNode::getInstance())) {
			_renderSession->clear();
			return;
		}

		_renderSession->renderExport("c:/test/lux_export");

		_renderSession->clear();
		delete _renderSession;
		_renderSession = nullptr;

		DfxGlobal::console::println("Luxcore Render Done!");

	}
	else {
		DfxGlobal::console::println("No Camera Assigned. Please assign a camera.");
	}
}

void DfxLuxCoreRenderNode::renderSequence() {
	if (!_renderingStill && !_renderingSequence) {
		auto space = DfxSpace::getInstance();

		_renderingStill = false;
		_renderingSequence = true;

		auto lxGeneralAny = space->findAbsolute(referencePath() + "/" + "luxcore_general");

		if (lxGeneralAny != nullptr) {

			auto dao = lxGeneralAny->rawData<fl::DataObject>();

			auto start_frame = dao->property("start_frame")->data<int>();
			auto end_frame  = dao->property("end_frame")->data<int>();
			auto frame_step = dao->property("frame_step")->data<int>();

			if (end_frame < start_frame) {
				DfxGlobal::console::println("Start Frame cannot be smaller than end frame.");
				return;
			}

			if (frame_step <= 0) {
				DfxGlobal::console::println("Frame step has to be bigger than 0.");
				return;
			}

			for (int cframe = start_frame; cframe <= end_frame; cframe += frame_step) {
				if (_renderingSequence) {
					DfxTimelineManager::getInstance()->goToFrame(cframe);
					QApplication::processEvents();

					renderFrame();
					saveSequence(cframe);
				}
				else {
					break;
				}
			}
		}

		_renderingSequence = false;
		_renderingStill = false;
	}
}

DfxRenderOutput *DfxLuxCoreRenderNode::renderOutput() {
	return _renderOutput;
}

void DfxLuxCoreRenderNode::saveStill() {
	auto space = DfxSpace::getInstance();

	auto lxOutAny = space->findAbsolute(referencePath() + "/" + "luxcore_image_output");

	
	if (lxOutAny != nullptr) {
		auto dao = lxOutAny->rawData<fl::DataObject>();

		auto dir = dao->property("base_stills_dir")->data<std::string>();
		auto prefix = dao->property("prefix")->data<std::string>();
		auto naming = dao->property("naming_convention")->data<unsigned int>();
		auto format = dao->property("file_format")->data<unsigned int>();
		auto save = dao->property("save_still_to_disk")->data<bool>();
		auto mkdir = dao->property("create_intermediate_directories")->data<bool>();
		auto aovInDir = dao->property("save_aov_in_seperate_folder")->data<bool>();
		auto overwrite = dao->property("overwrite_existing_file")->data<bool>();

		if (save) {

			if (!dir.empty()) {
				filesystem::path dirPath(dir);

				if (!filesystem::exists(dirPath)) {
					if (mkdir) {
						bool created = filesystem::create_directories(dirPath);

						if (!created) {
							DfxGlobal::console::println("Failed to create directory/directories");
							return;
						}
					}
					else {
						DfxGlobal::console::println("Failed to save render: Unknown directory");
						return;
					}
				}		

				if (!filesystem::is_directory(dirPath)) {
					DfxGlobal::console::println("Failed to save render: Select a directory");
					return;
				}

				for (int i = 0; i < _renderOutput->bufferCount(); i++) {
					auto finalDir = dir;

					if (aovInDir) {
						finalDir = finalDir + "/" + _renderOutput->getBuffer(i)->bufferName();
						filesystem::path aovPath(finalDir);

						if (!filesystem::exists(aovPath)) filesystem::create_directory(finalDir);
					}

					auto finalPath = finalDir + "/";

					if (naming == 0) {
						finalPath = finalPath + prefix + "_";
					}
					
					QString buffname(_renderOutput->getBuffer(i)->bufferName().c_str());

					buffname = buffname.toLower();
					buffname = buffname.replace(' ', "_");

					finalPath = finalPath + buffname.toStdString() + "_";
					
					auto image = _renderOutput->getBuffer(i)->getImage();

					if (overwrite) {
						finalPath = finalPath + "0000";

						if (format == 0) {
							finalPath = finalPath + ".png";
							image.save(finalPath.c_str(), "PNG", 100);
						}
						else {
							finalPath = finalPath + ".jpeg";
							image.save(finalPath.c_str(), "JPEG");
						}
		
					}
					else{
					
						int counter = 0;
						bool condition = true;
						int limit = 100000;

						while (condition && counter < limit) {
							QString number = QStringLiteral("%1").arg(counter, 4, 10, QLatin1Char('0'));
							auto tempPath = finalPath + number.toStdString();

							if (format == 0) {
								tempPath = tempPath + ".png";
							}
							else {
								tempPath = tempPath + ".jpeg";
							}

							filesystem::path tpath(tempPath);

							if (!filesystem::exists(tpath)) {
								finalPath = tempPath;
								condition = false;
							}

							counter++;
						}

						if (format == 0) {
							image.save(finalPath.c_str(), "PNG", 100);
						}
						else {
							image.save(finalPath.c_str(), "JPEG");
						}
					}
				}
			}
			else {
				DfxGlobal::console::println("Failed to save render: Specify destination");
				return;
			}
		}
	}

}

void DfxLuxCoreRenderNode::saveSequence(int frame) {
	auto space = DfxSpace::getInstance();

	auto lxOutAny = space->findAbsolute(referencePath() + "/" + "luxcore_image_output");

	if (lxOutAny != nullptr) {
		auto dao = lxOutAny->rawData<fl::DataObject>();

		auto dir = dao->property("base_sequence_dir")->data<std::string>();
		auto prefix = dao->property("prefix")->data<std::string>();
		auto naming = dao->property("naming_convention")->data<unsigned int>();
		auto format = dao->property("file_format")->data<unsigned int>();
		auto save = dao->property("save_sequence_to_disk")->data<bool>();
		auto mkdir = dao->property("create_intermediate_directories")->data<bool>();
		auto aovInDir = dao->property("save_aov_in_seperate_folder")->data<bool>();
		auto overwrite = dao->property("overwrite_existing_file")->data<bool>();

		if (save) {

			if (!dir.empty()) {
				filesystem::path dirPath(dir);

				if (!filesystem::exists(dirPath)) {
					if (mkdir) {
						bool created = filesystem::create_directories(dirPath);

						if (!created) {
							DfxGlobal::console::println("Failed to create directory/directories");
							return;
						}
					}
					else {
						DfxGlobal::console::println("Failed to save render: Unknown directory");
						return;
					}
				}

				if (!filesystem::is_directory(dirPath)) {
					DfxGlobal::console::println("Failed to save render: Select a directory");
					return;
				}

				for (int i = 0; i < _renderOutput->bufferCount(); i++) {
					auto finalDir = dir;

					if (aovInDir) {
						finalDir = finalDir + "/" + _renderOutput->getBuffer(i)->bufferName();
						filesystem::path aovPath(finalDir);

						if (!filesystem::exists(aovPath)) filesystem::create_directory(finalDir);
					}

					auto finalPath = finalDir + "/";

					if (naming == 0) {
						finalPath = finalPath + prefix + "_";
					}

					QString buffname(_renderOutput->getBuffer(i)->bufferName().c_str());

					buffname = buffname.toLower();
					buffname = buffname.replace(' ', "_");

					finalPath = finalPath + buffname.toStdString() + "_";

					auto image = _renderOutput->getBuffer(i)->getImage();

					QString number = QStringLiteral("%1").arg(frame, 4, 10, QLatin1Char('0'));
					finalPath = finalPath + number.toStdString();

					if (format == 0) {
						finalPath = finalPath + ".png";
					}
					else {
						finalPath = finalPath + ".jpeg";
					}

					filesystem::path tpath(finalPath);
					bool alreadyExists = filesystem::exists(tpath);
					bool save = false;

					if (overwrite) {
						save = true;
					}
					else {
						if (!alreadyExists) {
							save = true;
						}
					}

					if (save) {
						if (format == 0) {
							image.save(finalPath.c_str(), "PNG", 100);
						}
						else {
							image.save(finalPath.c_str(), "JPEG");
						}
					}

				}
			}
			else {
				DfxGlobal::console::println("Failed to save render: Specify destination");
				return;
			}
		}
	}
}

std::vector<std::pair<QWidget*, QString>> DfxLuxCoreRenderNode::propertyForms() {
	std::pair<QWidget*, QString> properties(propertiesForm(), "LuxCoreRender");

	return std::vector<std::pair<QWidget*, QString>> { properties };
}

QWidget *DfxLuxCoreRenderNode::propertiesForm() {

	QWidget *widget = new QWidget;
	
	QWidget *renderingWidget = new QWidget;
	QWidget *renderingSettingsWidget = new QWidget;
	QWidget *aovsWidget = new QWidget;

	QWidget *cacheWidget = new QWidget;
	QWidget *cachePhotonGiWidget = new QWidget;
	QWidget *cacheEnvLightWidget = new QWidget;
	QWidget *cacheDirectLightWidget = new QWidget;

	QWidget *denoiserWidget = new QWidget;

	QWidget *ImageWidget = new QWidget;
	QWidget *ImagePipelineWidget = new QWidget;
	QWidget *ImageOutputWidget = new QWidget;

	DfxQStackControl *samplingWidgetStack = new DfxQStackControl;
	DfxQStackControl *imageToneWidgetStack = new DfxQStackControl;

	QTabWidget *mainTabWidget = new QTabWidget;
	QTabWidget *renderingTabWidget = new QTabWidget;
	QTabWidget *cacheTabWidget = new QTabWidget;
	QTabWidget *imageTabWidget = new QTabWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	QVBoxLayout *rendering_main_layout = new QVBoxLayout;
	QVBoxLayout *rendering_settings_layout = new QVBoxLayout;
	QVBoxLayout *rendering_aovs_layout = new QVBoxLayout;

	QVBoxLayout *cache_main_layout = new QVBoxLayout;
	QVBoxLayout *cache_photon_gi_main_layout = new QVBoxLayout;
	QVBoxLayout *cache_env_light_main_layout = new QVBoxLayout;
	QVBoxLayout *cache_direct_light_main_layout = new QVBoxLayout;

	QVBoxLayout *denoiser_main_layout = new QVBoxLayout;

	QVBoxLayout *image_main_layout = new QVBoxLayout;
	QVBoxLayout *image_pipeline_main_layout = new QVBoxLayout;
	QVBoxLayout *image_output_main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	rendering_main_layout->setContentsMargins(0, 15, 0, 0);
	rendering_main_layout->setAlignment(Qt::AlignTop);

	rendering_settings_layout->setContentsMargins(0, 15, 0, 0);
	rendering_settings_layout->setAlignment(Qt::AlignTop);

	rendering_aovs_layout->setContentsMargins(0, 15, 0, 0);
	rendering_aovs_layout->setAlignment(Qt::AlignTop);

	cache_main_layout->setContentsMargins(0, 15, 0, 0);
	cache_main_layout->setAlignment(Qt::AlignTop);

	cache_photon_gi_main_layout->setContentsMargins(0, 15, 0, 0);
	cache_photon_gi_main_layout->setAlignment(Qt::AlignTop);

	cache_env_light_main_layout->setContentsMargins(0, 15, 0, 0);
	cache_env_light_main_layout->setAlignment(Qt::AlignTop);

	cache_direct_light_main_layout->setContentsMargins(0, 15, 0, 0);
	cache_direct_light_main_layout->setAlignment(Qt::AlignTop);

	denoiser_main_layout->setContentsMargins(0, 15, 0, 0);
	denoiser_main_layout->setAlignment(Qt::AlignTop);

	image_main_layout->setContentsMargins(0, 15, 0, 0);
	image_main_layout->setAlignment(Qt::AlignTop);

	image_pipeline_main_layout->setContentsMargins(0, 15, 0, 0);
	image_pipeline_main_layout->setAlignment(Qt::AlignTop);

	image_output_main_layout->setContentsMargins(0, 15, 0, 0);
	image_output_main_layout->setAlignment(Qt::AlignTop);

	auto generalControlPanel = new DfxControlPanel();

	auto renderingControlPanel  = new DfxControlPanel();
	auto samplingControlPanel = new DfxControlPanel();
	auto pathControlPanel   = new DfxControlPanel();
	auto bidirControlPanel = new DfxControlPanel();
	auto pixelFilterControlPanel  = new DfxControlPanel();
	auto haltControlPanel   = new DfxControlPanel();
	auto iprHaltControlPanel = new DfxControlPanel();
	auto refreshControlPanel = new DfxControlPanel();
	auto aovsControlPanel = new DfxControlPanel();
	auto cachePhotonGiControlPanel = new DfxControlPanel();
	auto cachePhotonGiIndirectControlPanel = new DfxControlPanel();
	auto cachePhotonGiCausticControlPanel = new DfxControlPanel();
	auto cacheEnvLightControlPanel = new DfxControlPanel();
	auto cacheDirectLightControlPanel = new DfxControlPanel();
	auto denoiserControlPanel = new DfxControlPanel();

	auto imageToneControlPanel = new DfxControlPanel();
	auto imageBoomControlPanel = new DfxControlPanel();
	auto imageFileOutputControlPanel = new DfxControlPanel();

	/*auto imageMistControlPanel = new DfxControlPanel();
	auto imageVignettControlPanel = new DfxControlPanel();
	auto imageAberrControlPanel = new DfxControlPanel();
	auto imageAnalogControlPanel = new DfxControlPanel();
	auto imageIrradianceControlPanel = new DfxControlPanel();*/
	
	DfxQControlArray *generalControlArray = new DfxQControlArray();

	DfxQControlArray *renderingControlArray = new DfxQControlArray();

	DfxQControlArray *samplingControlArray = new DfxQControlArray();
	DfxQControlArray *sobolControlArray = new DfxQControlArray();
	DfxQControlArray *metropolisControlArray = new DfxQControlArray();
	DfxQControlArray *randomControlArray = new DfxQControlArray();

	DfxQControlArray *pathControlArray = new DfxQControlArray();

	DfxQControlArray *bidirControlArray = new DfxQControlArray();

	DfxQControlArray *pixelFilterControlArray = new DfxQControlArray();

	DfxQControlArray *haltControlArray = new DfxQControlArray();
	
	DfxQControlArray *iprHaltControlArray = new DfxQControlArray();

	DfxQControlArray *refreshControlArray = new DfxQControlArray();

	DfxQControlArray *aovsControlArray = new DfxQControlArray();

	DfxQControlArray *cachePhotonGiControlArray = new DfxQControlArray();
	DfxQControlArray *cachePhotonGiIndirectControlArray = new DfxQControlArray();
	DfxQControlArray *cachePhotonGiCausticControlArray = new DfxQControlArray();
	
	DfxQControlArray *cacheEnvLightControlArray = new DfxQControlArray();

	DfxQControlArray *cacheDirectLightControlArray = new DfxQControlArray();

	DfxQControlArray *denoiserControlArray = new DfxQControlArray();

	DfxQControlArray *imageToneControlArray = new DfxQControlArray();
	DfxQControlArray *imageToneLinControlArray = new DfxQControlArray();
	DfxQControlArray *imageToneCamtrolArray = new DfxQControlArray();
	DfxQControlArray *imageToneReintrolArray = new DfxQControlArray();
	DfxQControlArray *imageBoomControlArray = new DfxQControlArray();
	DfxQControlArray *imageFileoutputControlArray = new DfxQControlArray();

	/*DfxQControlArray *imageMistControlArray = new DfxQControlArray();
	DfxQControlArray *imageVignettControlArray = new DfxQControlArray();
	DfxQControlArray *imageAberrControlArray = new DfxQControlArray();
	DfxQControlArray *imageAnalogControlArray = new DfxQControlArray();
	DfxQControlArray *imageIrradianceControlArray = new DfxQControlArray();*/

	std::string path = referencePath() + "/";

	QPushButton *renderFrame = new QPushButton("Render Frame");
	QPushButton *renderSequence = new QPushButton("Render Sequence");
	QPushButton *debugExportButton = new QPushButton("Debug Export");
	QPushButton *renderStop  = new QPushButton("Stop");
	QLabel *varianceEstimateLabel = new QLabel(QString("*Estimate: ") + QString(std::to_string(_varianceEstimate).c_str()));

	// AOVS
	auto aov_rgb = new DfxQCheckBoxControl(path + "luxcore_aovs/rgb", "", false);
	auto aov_rgba = new DfxQCheckBoxControl(path + "luxcore_aovs/rgba", "", false);
	auto aov_rgb_pipeline = new DfxQCheckBoxControl(path + "luxcore_aovs/rgb_imagepipeline", "", false);
	auto aov_rgba_pipeline = new DfxQCheckBoxControl(path + "luxcore_aovs/rgba_imagepipeline", "", false);
	auto aov_alpha = new DfxQCheckBoxControl(path + "luxcore_aovs/alpha", "", false);
	auto aov_depth = new DfxQCheckBoxControl(path + "luxcore_aovs/depth", "", false);
	auto aov_position = new DfxQCheckBoxControl(path + "luxcore_aovs/position", "", false);
	auto aov_geometry_normal = new DfxQCheckBoxControl(path + "luxcore_aovs/geometry_normal", "", false);
	auto aov_shading_normal = new DfxQCheckBoxControl(path + "luxcore_aovs/shading_normal", "", false);
	auto aov_material_id = new DfxQCheckBoxControl(path + "luxcore_aovs/material_id", "", false);
	auto aov_direct_diffuse = new DfxQCheckBoxControl(path + "luxcore_aovs/direct_diffuse", "", false);
	auto aov_direct_glossy = new DfxQCheckBoxControl(path + "luxcore_aovs/direct_glossy", "", false);
	auto aov_indirect_diffuse = new DfxQCheckBoxControl(path + "luxcore_aovs/indirect_diffuse", "", false);
	auto aov_indirect_glossy = new DfxQCheckBoxControl(path + "luxcore_aovs/indirect_glossy", "", false);
	auto aov_indirect_specular = new DfxQCheckBoxControl(path + "luxcore_aovs/indirect_specular", "", false);
	auto aov_material_id_mask = new DfxQCheckBoxControl(path + "luxcore_aovs/material_id_mask", "", false);
	auto aov_direct_shadow_mask = new DfxQCheckBoxControl(path + "luxcore_aovs/direct_shadow_mask", "", false);
	auto aov_indirect_shadow_mask = new DfxQCheckBoxControl(path + "luxcore_aovs/indirect_shadow_mask", "", false);
	auto aov_radiance_group = new DfxQCheckBoxControl(path + "luxcore_aovs/radiance_group", "", false);
	auto aov_uv = new DfxQCheckBoxControl(path + "luxcore_aovs/uv", "", false);
	auto aov_ray_count = new DfxQCheckBoxControl(path + "luxcore_aovs/ray_count", "", false);
	auto aov_by_material_id = new DfxQCheckBoxControl(path + "luxcore_aovs/by_material_id", "", false);
	auto aov_irradiance= new DfxQCheckBoxControl(path + "luxcore_aovs/irradiance", "", false);
	auto aov_object_id = new DfxQCheckBoxControl(path + "luxcore_aovs/object_id", "", false);
	auto aov_object_id_mask = new DfxQCheckBoxControl(path + "luxcore_aovs/object_id_mask", "", false);
	auto aov_by_object_id = new DfxQCheckBoxControl(path + "luxcore_aovs/by_object_id", "", false);
	auto aov_sample_count = new DfxQCheckBoxControl(path + "luxcore_aovs/samplecount", "", false);
	auto aov_convergence = new DfxQCheckBoxControl(path + "luxcore_aovs/convergence", "", false);
	auto aov_noise = new DfxQCheckBoxControl(path + "luxcore_aovs/noise", "", false);
	auto aov_serialized_film = new DfxQCheckBoxControl(path + "luxcore_aovs/serialized_film", "", false);
	auto aov_material_id_color = new DfxQCheckBoxControl(path + "luxcore_aovs/material_id_color", "", false);
	auto aov_emission = new DfxQCheckBoxControl(path + "luxcore_aovs/emission", "", false);

	// General Controls
	auto startFrame = new DfxQIntegerSpinnerControl(path + "luxcore_general/start_frame", "", false);
	auto endFrame   = new DfxQIntegerSpinnerControl(path + "luxcore_general/end_frame", "", false);
	auto frameStep  = new DfxQIntegerSpinnerControl(path + "luxcore_general/frame_step", "", false);
	auto camera     = new DfxQFluentPathControl(path + "luxcore_general/camera", "", false);
	auto print_info = new DfxQCheckBoxControl(path + "luxcore_general/print_render_info", "", false);

	// Rendering Controls
	auto device_type = new DfxQComboBoxControl(path + "device", "", deviceQControlenums, false);
	auto engine_type = new DfxQComboBoxControl(path + "engine", "", engineQControlenums, false);

	// Sampling
	auto russianRouletteDepth = new DfxQIntegerSpinnerControl(path + "luxcore_sampling/russian_roulette_depth", "", false);
	auto russianRouletteCap   = new DfxQDecimalSpinnerControl(path + "luxcore_sampling/russian_roulette_cap", "", false);

	auto sampler_type		   = new DfxQComboBoxControl(path + "luxcore_sampling/sampler", "", samplerQControlenums, false);
	auto sobolAdaptiveStrength = new DfxQDecimalSpinnerControl(path + "luxcore_sampling/sobol_adaptive_strength", "", false);
	auto metroMutationRate     = new DfxQDecimalSpinnerControl(path + "luxcore_sampling/metropolis_mutation_rate", "", false);
	auto metroMutationRejectionRate = new DfxQIntegerSpinnerControl(path + "luxcore_sampling/metropolis_mutation_rejection_rate", "", false);
	auto metroImageMutationRate = new DfxQDecimalSpinnerControl(path + "luxcore_sampling/metropolis_image_mutation_rate", "", false);
	auto randomAdaptiveStrength = new DfxQDecimalSpinnerControl(path + "luxcore_sampling/random_adaptive_strength", "", false);

	renderFrame->setStyleSheet(renderFrame->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());
	renderSequence->setStyleSheet(renderSequence->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());
	renderStop->setStyleSheet(renderSequence->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());
	debugExportButton->setStyleSheet(debugExportButton->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());

	// Path
	auto totalPathDepth = new DfxQIntegerSpinnerControl(path + "luxcore_path/max_recursion_depth", "", false);
	auto diffuse		= new DfxQIntegerSpinnerControl(path + "luxcore_path/diffuse", "", false);
	auto glossy			= new DfxQIntegerSpinnerControl(path + "luxcore_path/glossy", "", false);
	auto specular		= new DfxQIntegerSpinnerControl(path + "luxcore_path/specular", "", false);
	auto variance		= new DfxQDecimalSpinnerControl(path + "luxcore_path/variance_clamp", "", false);
	auto lightTracing	= new DfxQCheckBoxControl(path + "luxcore_path/use_light_tracing", "", false);
	auto lightTracingGloss = new DfxQDecimalSpinnerControl(path + "luxcore_path/light_tracing_glossiness", "", false);

	// Bidir
	auto maxEyeDepth = new DfxQIntegerSpinnerControl(path + "luxcore_bidir/max_eye_depth", "", false);
	auto maxLIghtDepth = new DfxQIntegerSpinnerControl(path + "luxcore_bidir/max_light_depth", "", false);

	// Pixel Filter
	auto pfilter_type = new DfxQComboBoxControl(path + "luxcore_pixel_filter/type", "", pixelFilterQControlenums, false);
	auto pfilterWidth = new DfxQDecimalSpinnerControl(path + "luxcore_pixel_filter/width", "", false);
	auto pfilterGaussianAlpha = new DfxQDecimalSpinnerControl(path + "luxcore_pixel_filter/gaussian_alpha", "", false);

	// Halt conditions
	auto halt_type = new DfxQComboBoxControl(path + "luxcore_halt_conditions/halt_condition", "", haltQControlenums, false);
	auto halt_samples = new DfxQIntegerSpinnerControl(path + "luxcore_halt_conditions/samples", "", false);
	auto halt_time = new DfxQIntegerSpinnerControl(path + "luxcore_halt_conditions/timed_samples", "", false);

	// IPR Halt conditions
	auto ipr_halt_type = new DfxQComboBoxControl(path + "luxcore_halt_conditions/ipr_halt_condition", "", haltQControlenums, false);
	auto ipr_halt_samples = new DfxQIntegerSpinnerControl(path + "luxcore_halt_conditions/ipr_samples", "", false);
	auto ipr_halt_time = new DfxQIntegerSpinnerControl(path + "luxcore_halt_conditions/ipr_timed_samples", "", false);

	// Refresh
	auto refesh_time = new DfxQIntegerSpinnerControl(path + "luxcore_refresh_interval/refresh_interval", "", false);
	auto ipr_refesh_time = new DfxQIntegerSpinnerControl(path + "luxcore_refresh_interval/ipr_refresh_interval", "", false);

	// Photon GI Cache
	auto cache_pgi_enable = new DfxQCheckBoxControl(path + "luxcore_photon_gi_cache/enable", "", false);
	auto cache_pgi_count = new DfxQIntegerSpinnerControl(path + "luxcore_photon_gi_cache/photon_count", "", false);
	auto cache_pgi_depth = new DfxQIntegerSpinnerControl(path + "luxcore_photon_gi_cache/photon_depth", "", false);

	// Photon GI Cache Indirect Light
	auto cache_pgi_indirect_enable = new DfxQCheckBoxControl(path + "luxcore_photon_gi_cache/enable_indirect_light_cache", "", false);
	auto cache_pgi_indirect_halt = new DfxQIntegerSpinnerControl(path + "luxcore_photon_gi_cache/indirect_halt_threshold", "", false);
	auto cache_pgi_indirect_rscale = new DfxQDecimalSpinnerControl(path + "luxcore_photon_gi_cache/indirect_brute_force_radius_scale", "", false);
	auto cache_pgi_indirect_normal = new DfxQIntegerSpinnerControl(path + "luxcore_photon_gi_cache/indirect_normal_angle", "", false);
	auto cache_pgi_indirect_gloss = new DfxQDecimalSpinnerControl(path + "luxcore_photon_gi_cache/indirect_glossiness_threshold", "", false);
	auto cache_pgi_indirect_auto_lookup = new DfxQCheckBoxControl(path + "luxcore_photon_gi_cache/indirect_auto_lookup_radius", "", false);
	auto cache_pgi_indirect_lookup_radius = new DfxQDecimalSpinnerControl(path + "luxcore_photon_gi_cache/indirect_lookup_radius", "", false);

	// Photon GI Cache Caustic Light
	auto cache_pgi_caustic_enable = new DfxQCheckBoxControl(path + "luxcore_photon_gi_cache/enable_caustic_light_cache", "", false);
	auto cache_pgi_caustic_periodic = new DfxQCheckBoxControl(path + "luxcore_photon_gi_cache/caustic_periodic_update", "", false);
	auto cache_pgi_caustic_maxs = new DfxQIntegerSpinnerControl(path + "luxcore_photon_gi_cache/caustic_max_size", "", false);
	auto cache_pgi_caustic_lookupr = new DfxQDecimalSpinnerControl(path + "luxcore_photon_gi_cache/caustic_lookup_radius", "", false);
	auto cache_pgi_caustic_normal = new DfxQIntegerSpinnerControl(path + "luxcore_photon_gi_cache/caustic_normal_angle", "", false);
	auto cache_pgi_caustic_radius = new DfxQDecimalSpinnerControl(path + "luxcore_photon_gi_cache/caustic_radius_Scale", "", false);
	auto cache_pgi_caustic_step = new DfxQIntegerSpinnerControl(path + "luxcore_photon_gi_cache/caustic_step_samples", "", false);
	auto cache_pgi_caustic_step_radius_reduction = new DfxQDecimalSpinnerControl(path + "luxcore_photon_gi_cache/caustic_step_radius_reduction", "", false);
	auto cache_pgi_caustic_step_min_radius = new DfxQDecimalSpinnerControl(path + "luxcore_photon_gi_cache/caustic_step_minimum_radius", "", false);

	// Environment Light Cache
	auto cache_env_light_enable = new DfxQCheckBoxControl(path + "luxcore_enviroment_light_cache/enable", "", false);
	auto cache_env_light_quality = new DfxQDecimalSpinnerControl(path + "luxcore_enviroment_light_cache/quality", "", false);
	/*auto cache_env_light_width = new DfxQIntegerSpinnerControl(path + "luxcore_enviroment_light_cache/map_width", "", false);
	auto cache_env_light_samples = new DfxQIntegerSpinnerControl(path + "luxcore_enviroment_light_cache/samples", "", false);*/

	// Direct Light Path
	auto cache_direct_light_enable	= new DfxQCheckBoxControl(path + "luxcore_direct_light_cache/enable", "", false);
	auto cache_direct_light_auto_rad = new DfxQCheckBoxControl(path + "luxcore_direct_light_cache/automatic_entry_radius", "", false);
	auto cache_direct_light_entry_rad = new DfxQDecimalSpinnerControl(path + "luxcore_direct_light_cache/entry_radius", "", false);
	auto cache_direct_light_w_samples = new DfxQIntegerSpinnerControl(path + "luxcore_direct_light_cache/warmup_samples", "", false);
	auto cache_direct_light_n_angle = new DfxQIntegerSpinnerControl(path + "luxcore_direct_light_cache/normal_angle", "", false);
	auto cache_direct_light_m_passes = new DfxQIntegerSpinnerControl(path + "luxcore_direct_light_cache/max_passes", "", false);
	auto cache_direct_light_c_threshold = new DfxQDecimalSpinnerControl(path + "luxcore_direct_light_cache/convergence_threshold", "", false);
	auto cache_direct_light_entries_vol = new DfxQCheckBoxControl(path + "luxcore_direct_light_cache/place_entries_in_volumes", "", false);
	auto cache_direct_light_l_threshold = new DfxQDecimalSpinnerControl(path + "luxcore_direct_light_cache/light_threshold", "", false);
	auto cache_direct_light_cache_ratio = new DfxQDecimalSpinnerControl(path + "luxcore_direct_light_cache/target_cache_hit_ratio", "", false);
	auto cache_direct_light_m_depth = new DfxQIntegerSpinnerControl(path + "luxcore_direct_light_cache/max_depth", "", false);
	auto cache_direct_light_m_samples = new DfxQIntegerSpinnerControl(path + "luxcore_direct_light_cache/max_samples", "", false);

	// Denoiser
	auto denoise_enable = new DfxQCheckBoxControl(path + "luxcore_denoising/enable", "", false);
	auto denoise_type = new DfxQComboBoxControl(path + "luxcore_denoising/type", "", denoiserQControlenums, false); 
	auto denoise_oidn_mem = new DfxQIntegerSpinnerControl(path + "luxcore_denoising/oidn_max_memory", "", false);

	// Pipleline Tonemapper
	auto imageToneEnable = new DfxQCheckBoxControl(path + "luxcore_image_pipeline/tonemap_enable", "", false);
	auto imageToneType   = new DfxQComboBoxControl(path + "luxcore_image_pipeline/tonemap_type", "", tonemapTypeQControlenums, false);
	auto imageToneLinAuto = new DfxQCheckBoxControl(path + "luxcore_image_pipeline/tonemap_lin_auto", "", false);
	auto imageToneLinGain = new DfxQDecimalSpinnerControl(path + "luxcore_image_pipeline/tonemap_lin_gain", "", false);
	auto imageToneCamFstop = new DfxQDecimalSpinnerControl(path + "luxcore_image_pipeline/tonemap_camera_fstop", "", false);
	auto imageToneCamShutter = new DfxQDecimalSpinnerControl(path + "luxcore_image_pipeline/tonemap_camera_shutter", "", false);
	auto imageToneCamISO = new DfxQDecimalSpinnerControl(path + "luxcore_image_pipeline/tonemap_camera_iso", "", false);
	auto imageToneReinPre = new DfxQDecimalSpinnerControl(path + "luxcore_image_pipeline/tonemap_rein_pre", "", false);
	auto imageToneReinPost = new DfxQDecimalSpinnerControl(path + "luxcore_image_pipeline/tonemap_rein_post", "", false);
	auto imageToneReinBurn = new DfxQDecimalSpinnerControl(path + "luxcore_image_pipeline/tonemap_rein_burn", "", false);

	// Pipleline Bloom
	auto imageBloomEnable = new DfxQCheckBoxControl(path + "luxcore_image_pipeline/bloom_enable", "", false);
	auto imageBloomRadius = new DfxQDecimalSpinnerControl(path + "luxcore_image_pipeline/bloom_radius", "", false);
	auto imageBloomStrength = new DfxQDecimalSpinnerControl(path + "luxcore_image_pipeline/bloom_strength", "", false);

	// Image Output 
	auto imageOutBaseStDir = new DfxQFilePathControl(path + "luxcore_image_output/base_stills_dir", true, true, "Image Stills Directory", "c:/", "", "", false);
	auto imageOutBaseSqDir = new DfxQFilePathControl(path + "luxcore_image_output/base_sequence_dir", true, true, "Image Sequence Directory", "c:/", "", "", false);
	auto imageOutPrefix = new DfxQLineEditControl(path + "luxcore_image_output/prefix", "", false);
	auto imageOutNConven = new DfxQComboBoxControl(path + "luxcore_image_output/naming_convention", "", imageNamingConventionTypeQControlenums, false);
	auto imageOutFormat = new DfxQComboBoxControl(path + "luxcore_image_output/file_format", "", imageFileFormatTypeQControlenums, false);
	auto imageOutStToDist = new DfxQCheckBoxControl(path + "luxcore_image_output/save_still_to_disk", "", false);
	auto imageOutSqToDisk = new DfxQCheckBoxControl(path + "luxcore_image_output/save_sequence_to_disk", "", false);
	auto imageOutToMemory = new DfxQCheckBoxControl(path + "luxcore_image_output/save_to_memory", "", false);
	auto imageOutCtImDirs = new DfxQCheckBoxControl(path + "luxcore_image_output/create_intermediate_directories", "", false);
	auto imageOutAovsSepDir = new DfxQCheckBoxControl(path + "luxcore_image_output/save_aov_in_seperate_folder", "", false);
	auto imageOutOverwriteFile = new DfxQCheckBoxControl(path + "luxcore_image_output/overwrite_existing_file", "", false);

	// AOVS
	aovsControlArray->addControl("RGB Image Pipeline", aov_rgb_pipeline);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("RGBA Image Pipeline", aov_rgba_pipeline);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("RGB", aov_rgb);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("RGBA", aov_rgba);
	aovsControlArray->addSpacer(0, 2);
	aovsControlArray->addControl("Alpha", aov_alpha);
	aovsControlArray->addSpacer(0, 2);
	aovsControlArray->addControl("Depth", aov_depth);
	aovsControlArray->addSpacer(0, 2);
	aovsControlArray->addControl("Emission", aov_emission);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Position", aov_position);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Geometry Normal", aov_geometry_normal);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Shading Normal", aov_shading_normal);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Material ID", aov_material_id);
	aovsControlArray->addSpacer(0, 2);
	aovsControlArray->addControl("Direct Diffuse", aov_direct_diffuse);
	aovsControlArray->addSpacer(0, 2);
	aovsControlArray->addControl("Direct Glossy", aov_direct_glossy);
	aovsControlArray->addSpacer(0, 2);
	aovsControlArray->addControl("Indirect Diffuse", aov_indirect_diffuse);
	aovsControlArray->addSpacer(0, 2);
	aovsControlArray->addControl("Indirect Glossy", aov_indirect_glossy);
	aovsControlArray->addSpacer(0, 2);
	aovsControlArray->addControl("Indirect Specular", aov_indirect_specular);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Material ID Mask", aov_material_id_mask);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Direct Shadow Mask", aov_direct_shadow_mask);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Indirect Shadow Mask", aov_indirect_shadow_mask);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Radiance Group", aov_radiance_group);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("UV", aov_uv);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Ray Count", aov_ray_count);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("By Material ID", aov_by_material_id);
	aovsControlArray->addSpacer(0, 2);
	aovsControlArray->addControl("Irradiance", aov_irradiance);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Object ID", aov_object_id);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Object ID Mask", aov_object_id_mask);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("By Object ID", aov_by_object_id);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Sample Count", aov_sample_count);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Convergence", aov_convergence);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Noise", aov_noise);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Serialized Film", aov_serialized_film);
	//aovsControlArray->addSpacer(0, 2);
	//aovsControlArray->addControl("Material ID Color", aov_material_id_color);
	
	aovsControlPanel->setTitle("Buffers");
	aovsControlPanel->setWidget(aovsControlArray);

	rendering_aovs_layout->addWidget(aovsControlPanel);
	aovsWidget->setLayout(rendering_aovs_layout);

	// General
	generalControlArray->addControl("", renderFrame);
	generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("", renderSequence);
	generalControlArray->addSpacer(0, 2);
	//generalControlArray->addControl("", debugExportButton);
	//generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("", renderStop);
	generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("Start Frame", startFrame);
	generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("End Frame", endFrame);
	generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("Frame Step", frameStep);
	generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("Camera", camera);
	/*generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("Print Info", print_info);*/

	generalControlPanel->setTitle("General");
	generalControlPanel->setWidget(generalControlArray);

	// Rendering
	renderingControlArray->addControl("Device", device_type);
	renderingControlArray->addSpacer(0, 2);
	renderingControlArray->addControl("Engine", engine_type);

	renderingControlPanel->setTitle("Targets");
	renderingControlPanel->setWidget(renderingControlArray);

	rendering_main_layout->addWidget(renderingControlPanel);
	rendering_main_layout->addWidget(renderingTabWidget);
	renderingWidget->setLayout(rendering_main_layout);

	renderingSettingsWidget->setLayout(rendering_settings_layout);

	// Sampling
	samplingControlArray->addControl("Russian Roulette Depth", russianRouletteDepth);
	samplingControlArray->addSpacer(0, 2);
	samplingControlArray->addControl("Russian Roulette Cap", russianRouletteCap);
	samplingControlArray->addSpacer(0, 2);
	samplingControlArray->addControl("Sampler", sampler_type);
	samplingControlArray->addControl(samplingWidgetStack);

	samplingControlPanel->setTitle("Sampling");
	samplingControlPanel->setWidget(samplingControlArray);

	sobolControlArray->addControl("Adaptive Strength", sobolAdaptiveStrength);

	metropolisControlArray->addControl("Mutation Rate", metroMutationRate);
	metropolisControlArray->addSpacer(0, 2);
	metropolisControlArray->addControl("Mutation Rejection Rate", metroMutationRejectionRate);
	metropolisControlArray->addSpacer(0, 2);
	metropolisControlArray->addControl("Image Mutation Rate", metroImageMutationRate);

	randomControlArray->addControl("Adaptive Strength", randomAdaptiveStrength);

	samplingWidgetStack->addPage(sobolControlArray);
	samplingWidgetStack->addPage(metropolisControlArray);
	samplingWidgetStack->addPage(randomControlArray);

	samplingWidgetStack->changeCurrentPage(sampler_type->comboBox()->currentIndex());

	rendering_settings_layout->addWidget(samplingControlPanel);

	// Path
	pathControlArray->addControl("Total Path Depth", totalPathDepth);
	pathControlArray->addSpacer(0, 2);
	pathControlArray->addControl("Diffuse", diffuse);
	pathControlArray->addSpacer(0, 2);
	pathControlArray->addControl("Glossy", glossy);
	pathControlArray->addSpacer(0, 2);
	pathControlArray->addControl("Specular", specular);
	pathControlArray->addSpacer(0, 2);
	pathControlArray->addControl("Clamping Variance", variance);
	pathControlArray->addControl("", varianceEstimateLabel);
	pathControlArray->addSpacer(0, 2);
	pathControlArray->addControl("Use Light Tracing", lightTracing);
	pathControlArray->addSpacer(0, 2);
	pathControlArray->addControl("Light Tracing Glossiness", lightTracingGloss);

	pathControlPanel->setTitle("Path Engine");
	pathControlPanel->setWidget(pathControlArray);

	rendering_settings_layout->addWidget(pathControlPanel);

	// Bidir
	bidirControlArray->addControl("Max Eye Depth", maxEyeDepth);
	bidirControlArray->addSpacer(0, 2);
	bidirControlArray->addControl("Max Light Depth", maxLIghtDepth);

	bidirControlPanel->setTitle("Bidir Engine");
	bidirControlPanel->setWidget(bidirControlArray);

	//rendering_settings_layout->addWidget(bidirControlPanel);

	// Pixel Filter
	pixelFilterControlArray->addControl("Type", pfilter_type);
	pixelFilterControlArray->addSpacer(0, 2);
	pixelFilterControlArray->addControl("Width", pfilterWidth);
	pixelFilterControlArray->addSpacer(0, 2);
	pixelFilterControlArray->addControl("Gaussian Filter Alpha", pfilterGaussianAlpha);

	pixelFilterControlPanel->setTitle("Pixel Filter");
	pixelFilterControlPanel->setWidget(pixelFilterControlArray);

	rendering_settings_layout->addWidget(pixelFilterControlPanel);

	// Halt Conditions
	haltControlArray->addControl("Type", halt_type);
	haltControlArray->addSpacer(0, 2);
	haltControlArray->addControl("Max Samples", halt_samples);
	haltControlArray->addSpacer(0, 2);
	haltControlArray->addControl("Time Limit (Seconds)", halt_time);

	haltControlPanel->setTitle("Halt Conditions");
	haltControlPanel->setWidget(haltControlArray);

	rendering_settings_layout->addWidget(haltControlPanel);

	// IPR Halt Conditions
	//iprHaltControlArray->addControl("Type", ipr_halt_type);
	//iprHaltControlArray->addSpacer(0, 2);
	//iprHaltControlArray->addControl("Max Samples", ipr_halt_samples);
	//iprHaltControlArray->addSpacer(0, 2);
	iprHaltControlArray->addControl("Time Limit (Seconds)", ipr_halt_time);

	iprHaltControlPanel->setTitle("IPR Halt Conditions");
	iprHaltControlPanel->setWidget(iprHaltControlArray);

	rendering_settings_layout->addWidget(iprHaltControlPanel);

	// Refresh
	refreshControlArray->addControl("Interval (Seconds)", refesh_time);
	refreshControlArray->addSpacer(0, 2);
	refreshControlArray->addControl("IPR Interval (Seconds)", ipr_refesh_time);

	refreshControlPanel->setTitle("Rendering Display Refresh");
	refreshControlPanel->setWidget(refreshControlArray);

	rendering_settings_layout->addWidget(refreshControlPanel);

	// Caches
	cache_main_layout->addWidget(cacheTabWidget);
	cacheWidget->setLayout(cache_main_layout);

	cachePhotonGiWidget->setLayout(cache_photon_gi_main_layout);
	
	// Photon Gi
	cachePhotonGiControlArray->addControl("Enable Caching", cache_pgi_enable);
	cachePhotonGiControlArray->addSpacer(0, 2);
	cachePhotonGiControlArray->addControl("Photon Count(Millions)", cache_pgi_count);
	cachePhotonGiControlArray->addSpacer(0, 2);
	cachePhotonGiControlArray->addControl("Photon Depth", cache_pgi_depth);
	cachePhotonGiControlArray->addSpacer(0, 2);
	cachePhotonGiControlArray->addControl("Glossiness Threshold", cache_pgi_indirect_gloss);

	cachePhotonGiControlPanel->setTitle("PhotonGi Cache");
	cachePhotonGiControlPanel->setWidget(cachePhotonGiControlArray);

	cache_photon_gi_main_layout->addWidget(cachePhotonGiControlPanel);

	// Photon Gi Indirect
	cachePhotonGiIndirectControlArray->addControl("Enable", cache_pgi_indirect_enable);
	cachePhotonGiIndirectControlArray->addSpacer(0, 2);
	cachePhotonGiIndirectControlArray->addControl("Halt Threshold", cache_pgi_indirect_halt);
	cachePhotonGiIndirectControlArray->addSpacer(0, 2);
	cachePhotonGiIndirectControlArray->addControl("Brute Fore Radius Scale", cache_pgi_indirect_rscale);
	cachePhotonGiIndirectControlArray->addSpacer(0, 2);
	cachePhotonGiIndirectControlArray->addControl("Normal Angle", cache_pgi_indirect_normal);
	cachePhotonGiIndirectControlArray->addSpacer(0, 2);
	cachePhotonGiIndirectControlArray->addControl("Automatic Lookup Radius", cache_pgi_indirect_auto_lookup);
	cachePhotonGiIndirectControlArray->addSpacer(0, 2);
	cachePhotonGiIndirectControlArray->addControl("Lookup Radius", cache_pgi_indirect_lookup_radius);

	cachePhotonGiIndirectControlPanel->setTitle("Indirect Light Cache");
	cachePhotonGiIndirectControlPanel->setWidget(cachePhotonGiIndirectControlArray);

	cache_photon_gi_main_layout->addWidget(cachePhotonGiIndirectControlPanel);

	// Photon Gi Caustic
	cachePhotonGiCausticControlArray->addControl("Enable", cache_pgi_caustic_enable);
	cachePhotonGiCausticControlArray->addSpacer(0, 2);
	cachePhotonGiCausticControlArray->addControl("Max Size(Millions)", cache_pgi_caustic_maxs);
	cachePhotonGiCausticControlArray->addSpacer(0, 2);
	cachePhotonGiCausticControlArray->addControl("Lookup Radius", cache_pgi_caustic_lookupr);
	cachePhotonGiCausticControlArray->addSpacer(0, 2);
	cachePhotonGiCausticControlArray->addControl("Normal Angle", cache_pgi_caustic_normal);
	cachePhotonGiCausticControlArray->addSpacer(0, 2);
	cachePhotonGiCausticControlArray->addControl("Radius Scale", cache_pgi_caustic_radius);
	cachePhotonGiCausticControlArray->addSpacer(0, 2);
	cachePhotonGiCausticControlArray->addControl("Periodic Update", cache_pgi_caustic_periodic);
	cachePhotonGiCausticControlArray->addSpacer(0, 2);
	cachePhotonGiCausticControlArray->addControl("Step Samples", cache_pgi_caustic_step);
	cachePhotonGiCausticControlArray->addSpacer(0, 2);
	cachePhotonGiCausticControlArray->addControl("Step Radius Reduction", cache_pgi_caustic_step_radius_reduction);
	cachePhotonGiCausticControlArray->addSpacer(0, 2);
	cachePhotonGiCausticControlArray->addControl("Step Min Radius", cache_pgi_caustic_step_min_radius);

	cachePhotonGiCausticControlPanel->setTitle("Caustic Cache");
	cachePhotonGiCausticControlPanel->setWidget(cachePhotonGiCausticControlArray);

	cache_photon_gi_main_layout->addWidget(cachePhotonGiCausticControlPanel);
	
	// Enviroment Light Cache 
	cacheEnvLightWidget->setLayout(cache_env_light_main_layout);

	cacheEnvLightControlArray->addControl("Enable Cache", cache_env_light_enable);
	cacheEnvLightControlArray->addSpacer(0, 2);
	cacheEnvLightControlArray->addControl("Quality", cache_env_light_quality);

	cacheEnvLightControlPanel->setTitle("Environment Light");
	cacheEnvLightControlPanel->setWidget(cacheEnvLightControlArray);

	cache_env_light_main_layout->addWidget(cacheEnvLightControlPanel);

	// Direct Light Cache
	cacheDirectLightWidget->setLayout(cache_direct_light_main_layout);

	cacheDirectLightControlArray->addControl("Enable Cache", cache_direct_light_enable);
	cacheDirectLightControlArray->addSpacer(0, 2);
	cacheDirectLightControlArray->addControl("Automatic Entry radius", cache_direct_light_auto_rad);
	cacheDirectLightControlArray->addSpacer(0, 2);
	cacheDirectLightControlArray->addControl("Entry radius", cache_direct_light_entry_rad);
	cacheDirectLightControlArray->addSpacer(0, 2);
	cacheDirectLightControlArray->addControl("Warmup Samples", cache_direct_light_w_samples);
	cacheDirectLightControlArray->addSpacer(0, 2);
	cacheDirectLightControlArray->addControl("Normal Angle", cache_direct_light_n_angle);
	cacheDirectLightControlArray->addSpacer(0, 2);
	cacheDirectLightControlArray->addControl("Max Passes", cache_direct_light_m_passes);
	cacheDirectLightControlArray->addSpacer(0, 2);
	cacheDirectLightControlArray->addControl("Convergence Threshold", cache_direct_light_c_threshold);
	cacheDirectLightControlArray->addSpacer(0, 2);
	cacheDirectLightControlArray->addControl("Place Entries In Volume", cache_direct_light_entries_vol);
	cacheDirectLightControlArray->addSpacer(0, 2);
	cacheDirectLightControlArray->addControl("Light Threshold", cache_direct_light_l_threshold);
	cacheDirectLightControlArray->addSpacer(0, 2);
	cacheDirectLightControlArray->addControl("Cache Hit Ratio", cache_direct_light_cache_ratio);
	cacheDirectLightControlArray->addSpacer(0, 2);
	cacheDirectLightControlArray->addControl("Max Depth", cache_direct_light_m_depth);
	cacheDirectLightControlArray->addSpacer(0, 2);
	cacheDirectLightControlArray->addControl("Max Samples", cache_direct_light_m_samples);

	cacheDirectLightControlPanel->setTitle("Direct Light");
	cacheDirectLightControlPanel->setWidget(cacheDirectLightControlArray);

	cache_direct_light_main_layout->addWidget(cacheDirectLightControlPanel);

	// Denoiser
	denoiserWidget->setLayout(denoiser_main_layout);

	denoiserControlArray->addControl("Enable Denoising", denoise_enable);
	denoiserControlArray->addSpacer(0, 2);
	denoiserControlArray->addControl("Denoiser", denoise_type);
	denoiserControlArray->addSpacer(0, 2);
	denoiserControlArray->addControl("OIDN Max Memory", denoise_oidn_mem);

	denoiserControlPanel->setTitle("Denoising");
	denoiserControlPanel->setWidget(denoiserControlArray);

	denoiser_main_layout->addWidget(denoiserControlPanel);

	// Image
	ImageWidget->setLayout(image_main_layout);
	image_main_layout->addWidget(imageTabWidget);

	ImagePipelineWidget->setLayout(image_pipeline_main_layout);
	
	// Tonemapper
	imageToneControlArray->addControl("Enable", imageToneEnable);
	imageToneControlArray->addControl("Type", imageToneType);
	imageToneControlArray->addControl(imageToneWidgetStack);

	imageToneLinControlArray->addControl("Gain",imageToneLinGain);
	imageToneLinControlArray->addSpacer(0, 2);
	imageToneLinControlArray->addControl("Auto Brightness", imageToneLinAuto);

	imageToneCamtrolArray->addControl("F-stop", imageToneCamFstop);
	imageToneCamtrolArray->addSpacer(0, 2);
	imageToneCamtrolArray->addControl("Shutter(s)", imageToneCamShutter);
	imageToneCamtrolArray->addSpacer(0, 2);
	imageToneCamtrolArray->addControl("ISO", imageToneCamISO);

	imageToneReintrolArray->addControl("Pre", imageToneReinPre);
	imageToneReintrolArray->addSpacer(0, 2);
	imageToneReintrolArray->addControl("Post", imageToneReinPost);
	imageToneReintrolArray->addSpacer(0, 2);
	imageToneReintrolArray->addControl("Burn", imageToneReinBurn);

	imageToneWidgetStack->addPage(imageToneLinControlArray);
	imageToneWidgetStack->addPage(imageToneCamtrolArray);
	imageToneWidgetStack->addPage(imageToneReintrolArray);

	imageToneWidgetStack->changeCurrentPage(imageToneType->comboBox()->currentIndex());

	imageToneControlPanel->setTitle("Tonemapper");
	imageToneControlPanel->setWidget(imageToneControlArray);

	image_pipeline_main_layout->addWidget(imageToneControlPanel);

	// Bloom
	imageBoomControlArray->addControl("Enable", imageBloomEnable);
	imageBoomControlArray->addControl("Radius", imageBloomRadius);
	imageBoomControlArray->addSpacer(0, 2);
	imageBoomControlArray->addControl("Strength", imageBloomStrength);

	imageBoomControlPanel->setTitle("Bloom");
	imageBoomControlPanel->setWidget(imageBoomControlArray);

	image_pipeline_main_layout->addWidget(imageBoomControlPanel);

	// Image Output
	ImageOutputWidget->setLayout(image_output_main_layout);

	imageFileoutputControlArray->addControl("Stills Directory", imageOutBaseStDir);
	imageFileoutputControlArray->addSpacer(0, 2);
	imageFileoutputControlArray->addControl("Sequence Directory", imageOutBaseSqDir);
	imageFileoutputControlArray->addSpacer(0, 2);
	imageFileoutputControlArray->addControl("Prefix", imageOutPrefix);
	imageFileoutputControlArray->addSpacer(0, 2);
	imageFileoutputControlArray->addControl("naming Convention", imageOutNConven);
	imageFileoutputControlArray->addSpacer(0, 2);
	imageFileoutputControlArray->addControl("File Format", imageOutFormat);
	imageFileoutputControlArray->addSpacer(0, 2);
	imageFileoutputControlArray->addControl(imageOutStToDist, "Save Still to Disk");
	imageFileoutputControlArray->addSpacer(0, 2);
	imageFileoutputControlArray->addControl(imageOutSqToDisk, "Save Sequence to Disk ");
	imageFileoutputControlArray->addSpacer(0, 2);
	/*imageFileoutputControlArray->addControl(imageOutToMemory, "Save To Memory");
	imageFileoutputControlArray->addSpacer(0, 2);*/
	imageFileoutputControlArray->addControl(imageOutCtImDirs, "Create Intermediate Directories");
	imageFileoutputControlArray->addSpacer(0, 2);
	imageFileoutputControlArray->addControl(imageOutAovsSepDir, "Save AOVs in Seperate Directory");
	imageFileoutputControlArray->addSpacer(0, 2);
	imageFileoutputControlArray->addControl(imageOutOverwriteFile, "Overwite Existing File");

	imageFileOutputControlPanel->setTitle("Image Output");
	imageFileOutputControlPanel->setWidget(imageFileoutputControlArray);

	image_output_main_layout->addWidget(imageFileOutputControlPanel);

	// Tabs
	mainTabWidget->addTab(renderingWidget, "Rendering");
	mainTabWidget->addTab(denoiserWidget, "Denoising");
	mainTabWidget->addTab(aovsWidget, "AOVS");
	mainTabWidget->addTab(ImageWidget, "Image");

	renderingTabWidget->addTab(renderingSettingsWidget, "Renderer");
	renderingTabWidget->addTab(cacheWidget, "Caches");
	// renderingTabWidget->addTab(new DfxQEmptyWidget, "Light Groups");

	cacheTabWidget->addTab(cachePhotonGiWidget, "PhotonGI");
	cacheTabWidget->addTab(cacheEnvLightWidget, "Environment Light");
	cacheTabWidget->addTab(cacheDirectLightWidget, "Direct Light Sampling");

	imageTabWidget->addTab(ImagePipelineWidget, "Pipeline");
	imageTabWidget->addTab(ImageOutputWidget, "Output");

	main_layout->addWidget(generalControlPanel);
	main_layout->addWidget(mainTabWidget );

	widget->setLayout(main_layout);

	QObject::connect(sampler_type->comboBox(), QOverload<int>::of(&QComboBox::activated), samplingWidgetStack, &DfxQStackControl::changeCurrentPage);
	QObject::connect(imageToneType->comboBox(), QOverload<int>::of(&QComboBox::activated), imageToneWidgetStack, &DfxQStackControl::changeCurrentPage);

	QObject::connect(renderFrame, &QPushButton::clicked, this, &DfxLuxCoreRenderNode::renderStill);
	QObject::connect(renderSequence, &QPushButton::clicked, this, &DfxLuxCoreRenderNode::renderSequence);
	QObject::connect(debugExportButton, &QPushButton::clicked, this, &DfxLuxCoreRenderNode::debugExport);
	QObject::connect(renderStop, &QPushButton::clicked, this, &DfxLuxCoreRenderNode::stopAllRendering);

	QObject::connect(this, &DfxLuxCoreRenderNode::varianceEstimateUpdated, varianceEstimateLabel, &QLabel::setText);

	return widget;
}

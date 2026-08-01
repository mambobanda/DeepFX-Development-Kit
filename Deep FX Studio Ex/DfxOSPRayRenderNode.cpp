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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxOSPRayRenderNode.h"
#include "DfxRenderOutput.h"

std::string DfxOSPRayRenderNode::_uniqueName = "UID_OSPRAY_RENDER_NODE";

DfxOSPRayRenderNode::DfxOSPRayRenderNode() :DfxRenderNodeBase(DfxRenderTarget::OSPRAY) {

	_renderSession = nullptr;

	_renderingStill = false;
	_renderingSequence = false;

	_varianceEstimate = 0.f;

	engineQControlenums.push_back(std::pair<std::string, unsigned int>("Path Tracer", Engine::PATH_TRACER));
	engineQControlenums.push_back(std::pair<std::string, unsigned int>("SciVi", Engine::SCI_VIS));
	engineQControlenums.push_back(std::pair<std::string, unsigned int>("AO", Engine::AO));

	pixelFilterQControlenums.push_back(std::pair<std::string, unsigned int>("Point", PixelFilter::POINT));
	pixelFilterQControlenums.push_back(std::pair<std::string, unsigned int>("Box", PixelFilter::BOX));
	pixelFilterQControlenums.push_back(std::pair<std::string, unsigned int>("Gaussian ", PixelFilter::GAUSS));
	pixelFilterQControlenums.push_back(std::pair<std::string, unsigned int>("Mitchell", PixelFilter::MITCHELL));
	pixelFilterQControlenums.push_back(std::pair<std::string, unsigned int>("Blackman-Harris", PixelFilter::BLACKMAN));

	imageNamingConventionTypeQControlenums.push_back(std::pair<std::string, unsigned int>("prefix_buffername_0000", ImageNamingConvention::PREFIX_BUFFER_FRAME));
	imageNamingConventionTypeQControlenums.push_back(std::pair<std::string, unsigned int>("buffername_0000", ImageNamingConvention::BUFFER_FRAME));

	imageFileFormatTypeQControlenums.push_back(std::pair<std::string, unsigned int>(".PNG", ImageFileFormat::PNG));
	imageFileFormatTypeQControlenums.push_back(std::pair<std::string, unsigned int>(".JPEG", ImageFileFormat::JPEG));

	haltQControlenums.push_back(std::pair<std::string, unsigned int>("Variance", Halt::VARIANCE));
	haltQControlenums.push_back(std::pair<std::string, unsigned int>("Time", Halt::TIME));
}

DfxOSPRayRenderNode::~DfxOSPRayRenderNode() {
	DfxRenderHub::getInstance()->removeRenderNode(this);
	delete renderOutput();

	if (_renderSession != nullptr) delete _renderSession;
}

void DfxOSPRayRenderNode::initRenderNode() {
	_renderOutput = new DfxRenderOutput(this);
	DfxRenderHub::getInstance()->addRenderNode(this);
}

std::string DfxOSPRayRenderNode::uniqueName() {
	return _uniqueName;
}

std::string DfxOSPRayRenderNode::getUniqueName() {
	return _uniqueName;
}

void DfxOSPRayRenderNode::stopRendering() {
	if (_renderSession != nullptr) {
		_renderSession->forceStop();
		_renderingStill = false;
		_renderingSequence = false;
	}
}

void DfxOSPRayRenderNode::stopAllRendering() {
	_renderingSequence = false;
	_renderingStill = false;
	stopRendering();
}

DfxOSPRayCameraHandler DfxOSPRayRenderNode::getCamera() {
	DfxOSPRayCameraHandler handler;

	auto cameraAttr = DfxSpace::getInstance()->findAbsolute(referencePath() + "/ospray_general/camera");
	auto cameraPath = cameraAttr->data<std::string>();

	if (handler.fetch(cameraPath)) {

		if (handler.dataObject()->rawData<fl::DataObject>()->hasProperty("item_type")) {

			if (handler.dataObject()->rawData<fl::DataObject>()->property("item_type")->data<unsigned int>() == DfxItemWidgetType::CAMERA) {
				if (handler.dataObject()->rawData<fl::DataObject>()->hasProperty("render_target")) {

					if (handler.dataObject()->rawData<fl::DataObject>()->property("render_target")->data<unsigned int>() == DfxRenderTarget::OSPRAY) {

						return handler;
					}
				}
			}
		}
	}

	return DfxOSPRayCameraHandler();
}

void DfxOSPRayRenderNode::renderFrame() {
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

		auto ospRenderAny = space->findAbsolute(referencePath());

		int camWidth = camHandler.dataObject()->data<fl::DataObject>().property("width")->data<unsigned int>();
		int camHeight = camHandler.dataObject()->data<fl::DataObject>().property("height")->data<unsigned int>();

		_renderSession = new DfxOSPRayRenderSession;
		_renderSession->setCameraPath(camHandler.dataObject()->absolutePath());
		_renderSession->setOsprayRenderInfoPath(referencePath());
		_renderSession->setRenderOutput(_renderOutput);

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

		_renderSession->clear();
		delete _renderSession;
		_renderSession = nullptr;

		DfxGlobal::console::println("OSPRay Render Done!");

	}
	else {
		DfxGlobal::console::println("No Camera Assigned. Please assign a camera.");
	}

}

DfxRenderSessionInterface *DfxOSPRayRenderNode::renderSession() {
	/*auto camHandler = getCamera();

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
	*/

	return nullptr;
}

void DfxOSPRayRenderNode::renderStill() {
	if (!_renderingStill && !_renderingSequence) {
		_renderingStill = true;
		_renderingSequence = false;

		renderFrame();
		saveStill();

		_renderingStill = false;
		_renderingSequence = false;
	}
}

void DfxOSPRayRenderNode::renderSequence() {
	if (!_renderingStill && !_renderingSequence) {
		auto space = DfxSpace::getInstance();

		_renderingStill = false;
		_renderingSequence = true;

		auto ospGeneralAny = space->findAbsolute(referencePath() + "/" + "ospray_general");

		if (ospGeneralAny != nullptr) {

			auto dao = ospGeneralAny->rawData<fl::DataObject>();

			auto start_frame = dao->property("start_frame")->data<int>();
			auto end_frame = dao->property("end_frame")->data<int>();
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

DfxRenderOutput *DfxOSPRayRenderNode::renderOutput() {
	return _renderOutput;
}

void DfxOSPRayRenderNode::saveStill() {
	auto space = DfxSpace::getInstance();

	auto ospOutAny = space->findAbsolute(referencePath() + "/" + "ospray_image_output");

	if (ospOutAny != nullptr) {
		auto dao = ospOutAny->rawData<fl::DataObject>();

		auto dir = dao->property("base_stills_dir")->data<std::string>();
		auto prefix = dao->property("prefix")->data<std::string>();
		auto naming = dao->property("naming_convention")->data<unsigned int>();
		auto format = dao->property("file_format")->data<unsigned int>();
		auto save = dao->property("save_still_to_disk")->data<bool>();
		auto mkdir = dao->property("create_intermediate_directories")->data<bool>();
		// auto aovInDir = dao->property("save_aov_in_seperate_folder")->data<bool>();
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

					/*if (aovInDir) {
						finalDir = finalDir + "/" + _renderOutput->getBuffer(i)->bufferName();
						filesystem::path aovPath(finalDir);

						if (!filesystem::exists(aovPath)) filesystem::create_directory(finalDir);
					}*/

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
					else {

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

void DfxOSPRayRenderNode::saveSequence(int frame) {
	auto space = DfxSpace::getInstance();

	auto ospOutAny = space->findAbsolute(referencePath() + "/" + "ospray_image_output");

	if (ospOutAny != nullptr) {
		auto dao = ospOutAny->rawData<fl::DataObject>();

		auto dir = dao->property("base_sequence_dir")->data<std::string>();
		auto prefix = dao->property("prefix")->data<std::string>();
		auto naming = dao->property("naming_convention")->data<unsigned int>();
		auto format = dao->property("file_format")->data<unsigned int>();
		auto save = dao->property("save_sequence_to_disk")->data<bool>();
		auto mkdir = dao->property("create_intermediate_directories")->data<bool>();
		// auto aovInDir = dao->property("save_aov_in_seperate_folder")->data<bool>();
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

					/*if (aovInDir) {
						finalDir = finalDir + "/" + _renderOutput->getBuffer(i)->bufferName();
						filesystem::path aovPath(finalDir);

						if (!filesystem::exists(aovPath)) filesystem::create_directory(finalDir);
					}*/

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

std::vector<std::pair<QWidget*, QString>> DfxOSPRayRenderNode::propertyForms() {
	std::pair<QWidget*, QString> properties(propertiesForm(), "OSPRay Render");

	return std::vector<std::pair<QWidget*, QString>> { properties };
}

QWidget *DfxOSPRayRenderNode::propertiesForm() {

	QWidget *widget = new QWidget;
	QWidget *renderingSettingsWidget = new QWidget;
	QWidget *outputSettingsWidget = new QWidget;
	QWidget *toneMappingSettingsWidget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;
	QVBoxLayout *rendering_layout = new QVBoxLayout;
	QVBoxLayout *output_layout = new QVBoxLayout;
	QVBoxLayout *tone_mapping_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	rendering_layout->setContentsMargins(0, 15, 0, 0);
	rendering_layout->setAlignment(Qt::AlignTop);

	output_layout->setContentsMargins(0, 15, 0, 0);
	output_layout->setAlignment(Qt::AlignTop);

	tone_mapping_layout->setContentsMargins(0, 15, 0, 0);
	tone_mapping_layout->setAlignment(Qt::AlignTop);

	QTabWidget *mainTabWidget = new QTabWidget;

	auto generalControlPanel = new DfxControlPanel();
	auto globalControlPanel = new DfxControlPanel();
	auto pathControlPanel = new DfxControlPanel();
	auto sciVisControlPanel = new DfxControlPanel();
	auto aoControlPanel = new DfxControlPanel();
	auto haltControlPanel = new DfxControlPanel();

	auto outputControlPanel = new DfxControlPanel();

	auto toneMappingControlPanel = new DfxControlPanel();

	DfxQControlArray *generalControlArray = new DfxQControlArray();
	DfxQControlArray *globalControlArray = new DfxQControlArray();
	DfxQControlArray *pathControlArray = new DfxQControlArray();
	DfxQControlArray *sciVisControlArray = new DfxQControlArray();
	DfxQControlArray *aoControlArray = new DfxQControlArray();
	DfxQControlArray *haltControlArray = new DfxQControlArray();

	DfxQControlArray *outputControlArray = new DfxQControlArray();

	DfxQControlArray *toneMappingControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	QPushButton *renderFrame = new QPushButton("Render Frame");
	QPushButton *renderSequence = new QPushButton("Render Sequence");
	QPushButton *renderStop = new QPushButton("Stop");

	renderFrame->setStyleSheet(renderFrame->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());
	renderSequence->setStyleSheet(renderSequence->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());
	renderStop->setStyleSheet(renderSequence->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());

	// General Controls
	auto startFrame = new DfxQIntegerSpinnerControl(path + "ospray_general/start_frame", "", false);
	auto endFrame = new DfxQIntegerSpinnerControl(path + "ospray_general/end_frame", "", false);
	auto frameStep = new DfxQIntegerSpinnerControl(path + "ospray_general/frame_step", "", false);
	auto camera = new DfxQFluentPathControl(path + "ospray_general/camera", "", false);
	auto print_info = new DfxQCheckBoxControl(path + "ospray_general/print_render_info", "", false);

	// Global Settings
	auto engine = new DfxQComboBoxControl(path + "engine", "", engineQControlenums, false);
	auto pixel_filter = new DfxQComboBoxControl(path + "ospray_general/pixel_filter", "", pixelFilterQControlenums, false);
	auto pixel_samples = new DfxQIntegerSpinnerControl(path + "ospray_general/pixel_samples", "", false);
	auto max_path_length = new DfxQIntegerSpinnerControl(path + "ospray_general/max_path_length", "", false);
	auto min_contribution = new DfxQDecimalSpinnerControl(path + "ospray_general/min_contribution", "", false);
	auto variance_threshold = new DfxQDecimalSpinnerControl(path + "ospray_general/variance_threshold", "", false);
	auto denoising = new DfxQCheckBoxControl(path + "ospray_general/denoising", "", false);

	// Path Settings 
	auto roulette_path_length = new DfxQIntegerSpinnerControl(path + "ospray_path/roulette_path_length", "", false);
	auto background_refraction = new DfxQCheckBoxControl(path + "ospray_path/background_refraction", "", false);

	// SciVis
	auto shadows = new DfxQCheckBoxControl(path + "ospray_scivis/shadows", "", false);
	auto s_aoSamples = new DfxQIntegerSpinnerControl(path + "ospray_scivis/aoSamples", "", false);
	auto s_ao_max_distance = new DfxQDecimalSpinnerControl(path + "ospray_scivis/ao_max_distance", "", false);
	auto s_volume_sampling_rate = new DfxQDecimalSpinnerControl(path + "ospray_scivis/volume_sampling_rate", "", false);
	auto visible_lights = new DfxQCheckBoxControl(path + "ospray_scivis/visible_lights", "", false);

	// AO
	auto aoSamples = new DfxQIntegerSpinnerControl(path + "ospray_ao/aoSamples", "", false);
	auto ao_max_distance = new DfxQDecimalSpinnerControl(path + "ospray_ao/ao_max_distance", "", false);
	auto aoIntensity = new DfxQDecimalSpinnerControl(path + "ospray_ao/aoIntensity", "", false);
	auto volume_sampling_rate = new DfxQDecimalSpinnerControl(path + "ospray_ao/volume_sampling_rate", "", false);

	// Halt Conditions
	auto halt_condition = new DfxQComboBoxControl(path + "ospray_halt_conditions/halt_condition", "", haltQControlenums, false);
	auto ipr_halt_condition = new DfxQComboBoxControl(path + "ospray_halt_conditions/ipr_halt_condition", "", haltQControlenums, false);
	auto variance = new DfxQDecimalSpinnerControl(path + "ospray_halt_conditions/variance", "", false);
	auto timed_samples = new DfxQIntegerSpinnerControl(path + "ospray_halt_conditions/timed_samples", "", false);
	auto ipr_variance = new DfxQDecimalSpinnerControl(path + "ospray_halt_conditions/ipr_variance", "", false);
	auto ipr_timed_samples = new DfxQIntegerSpinnerControl(path + "ospray_halt_conditions/ipr_timed_samples", "", false);

	// Image Output 
	auto imageOutBaseStDir = new DfxQFilePathControl(path + "ospray_image_output/base_stills_dir", true, true, "Image Stills Directory", "c:/", "", "", false);
	auto imageOutBaseSqDir = new DfxQFilePathControl(path + "ospray_image_output/base_sequence_dir", true, true, "Image Sequence Directory", "c:/", "", "", false);
	auto imageOutPrefix = new DfxQLineEditControl(path + "ospray_image_output/prefix", "", false);
	auto imageOutNConven = new DfxQComboBoxControl(path + "ospray_image_output/naming_convention", "", imageNamingConventionTypeQControlenums, false);
	auto imageOutFormat = new DfxQComboBoxControl(path + "ospray_image_output/file_format", "", imageFileFormatTypeQControlenums, false);
	auto imageOutStToDist = new DfxQCheckBoxControl(path + "ospray_image_output/save_still_to_disk", "", false);
	auto imageOutSqToDisk = new DfxQCheckBoxControl(path + "ospray_image_output/save_sequence_to_disk", "", false);
	auto imageOutToMemory = new DfxQCheckBoxControl(path + "ospray_image_output/save_to_memory", "", false);
	auto imageOutCtImDirs = new DfxQCheckBoxControl(path + "ospray_image_output/create_intermediate_directories", "", false);
	auto imageOutSaveAlpha = new DfxQCheckBoxControl(path + "ospray_image_output/save_alpha", "", false);
	auto imageOutOverwriteFile = new DfxQCheckBoxControl(path + "ospray_image_output/overwrite_existing_file", "", false);

	// Tone Mapping
	auto enable_tone_mapping = new DfxQCheckBoxControl(path + "ospray_tone_mapping/enable_tone_mapping", "", false);
	auto aces_color = new DfxQCheckBoxControl(path + "ospray_tone_mapping/aces_color", "", false);
	auto exposure = new DfxQDecimalSpinnerControl(path + "ospray_tone_mapping/exposure", "", false);
	auto contrast = new DfxQDecimalSpinnerControl(path + "ospray_tone_mapping/contrast", "", false);
	auto shoulder = new DfxQDecimalSpinnerControl(path + "ospray_tone_mapping/shoulder", "", false);
	auto mid_in = new DfxQDecimalSpinnerControl(path + "ospray_tone_mapping/mid_in", "", false);
	auto mid_out = new DfxQDecimalSpinnerControl(path + "ospray_tone_mapping/mid_out", "", false);
	auto hdr_max = new DfxQDecimalSpinnerControl(path + "ospray_tone_mapping/hdr_max", "", false);


	// General
	generalControlArray->addControl("", renderFrame);
	generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("", renderSequence);
	generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("", renderStop);
	generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("Start Frame", startFrame);
	generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("End Frame", endFrame);
	generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("Frame Step", frameStep);
	generalControlArray->addSpacer(0, 2);
	generalControlArray->addControl("Camera", camera);

	generalControlPanel->setTitle("General");
	generalControlPanel->setWidget(generalControlArray);

	// Global
	globalControlArray->addControl("Rendering Engine", engine);
	globalControlArray->addSpacer(0, 2);
	globalControlArray->addControl("Pixel Filter", pixel_filter);
	globalControlArray->addSpacer(0, 2);
	globalControlArray->addControl("Pixel Samples", pixel_samples);
	globalControlArray->addSpacer(0, 2);
	globalControlArray->addControl("Max Path Length", max_path_length);
	globalControlArray->addSpacer(0, 2);
	globalControlArray->addControl("Min Contribution", min_contribution);
	globalControlArray->addSpacer(0, 2);
	globalControlArray->addControl("Variance Threshold", variance_threshold);
	globalControlArray->addSpacer(0, 2);
	globalControlArray->addControl("Enable Denoising", denoising);

	globalControlPanel->setTitle("Global Settings");
	globalControlPanel->setWidget(globalControlArray);

	// Path
	pathControlArray->addControl("Roulette Path Length", roulette_path_length);
	pathControlArray->addSpacer(0, 2);
	pathControlArray->addControl("background Refraction", background_refraction);

	pathControlPanel->setTitle("Path Tracing");
	pathControlPanel->setWidget(pathControlArray);

	// Sci Vis
	sciVisControlArray->addControl("AO Samples", s_aoSamples);
	sciVisControlArray->addSpacer(0, 2);
	sciVisControlArray->addControl("AO Max Distance", s_ao_max_distance);
	sciVisControlArray->addSpacer(0, 2);
	sciVisControlArray->addControl("Volume Sample rate", s_volume_sampling_rate);
	sciVisControlArray->addSpacer(0, 2);
	sciVisControlArray->addControl("Render Lights", visible_lights);
	sciVisControlArray->addSpacer(0, 2);
	sciVisControlArray->addControl("Enable Shadows", shadows);

	sciVisControlPanel->setTitle("SciVis");
	sciVisControlPanel->setWidget(sciVisControlArray);

	// AO
	aoControlArray->addControl("Samples", aoSamples);
	aoControlArray->addSpacer(0, 2);
	aoControlArray->addControl("Max Distance", ao_max_distance);
	aoControlArray->addSpacer(0, 2);
	aoControlArray->addControl("Intensity", aoIntensity);
	aoControlArray->addSpacer(0, 2);
	aoControlArray->addControl("Volume Sampling Rate", volume_sampling_rate);

	aoControlPanel->setTitle("Ambient Occlusion");
	aoControlPanel->setWidget(aoControlArray);

	// Halt Conditions
	haltControlArray->addControl("Halt Condition", halt_condition);
	haltControlArray->addSpacer(0, 2);
	haltControlArray->addControl("Variance", variance);
	haltControlArray->addSpacer(0, 2);
	haltControlArray->addControl("Timed", timed_samples);
	haltControlArray->addSpacer(0, 4);
	haltControlArray->addControl("IPR Halt Condition", ipr_halt_condition);
	haltControlArray->addSpacer(0, 2);
	haltControlArray->addControl("IPR Variance", ipr_variance);
	haltControlArray->addSpacer(0, 2);
	haltControlArray->addControl("IPR Timed", ipr_timed_samples);

	haltControlPanel->setTitle("Halt Conditions");
	haltControlPanel->setWidget(haltControlArray);

	// Tone Mapping
	toneMappingControlArray->addControl(enable_tone_mapping, "Enable Tone Mapping");
	toneMappingControlArray->addSpacer(0, 2);
	toneMappingControlArray->addControl(aces_color, "ACES Color");
	toneMappingControlArray->addSpacer(0, 2);
	toneMappingControlArray->addControl("Exposure", exposure);
	toneMappingControlArray->addSpacer(0, 2);
	toneMappingControlArray->addControl("Contrast", contrast);
	toneMappingControlArray->addSpacer(0, 2);
	toneMappingControlArray->addControl("Shoulder", shoulder);
	toneMappingControlArray->addSpacer(0, 2);
	toneMappingControlArray->addControl("Mid In", mid_in);
	toneMappingControlArray->addSpacer(0, 2);
	toneMappingControlArray->addControl("Mid Out", mid_out);
	toneMappingControlArray->addSpacer(0, 2);
	toneMappingControlArray->addControl("HDRi Max", hdr_max);

	toneMappingControlPanel->setTitle("Tone Mapping");
	toneMappingControlPanel->setWidget(toneMappingControlArray);

	// Image Output
	outputControlArray->addControl("Stills Directory", imageOutBaseStDir);
	outputControlArray->addSpacer(0, 2);
	outputControlArray->addControl("Sequence Directory", imageOutBaseSqDir);
	outputControlArray->addSpacer(0, 2);
	outputControlArray->addControl("Prefix", imageOutPrefix);
	outputControlArray->addSpacer(0, 2);
	outputControlArray->addControl("naming Convention", imageOutNConven);
	outputControlArray->addSpacer(0, 2);
	outputControlArray->addControl("File Format", imageOutFormat);
	outputControlArray->addSpacer(0, 2);
	outputControlArray->addControl(imageOutStToDist, "Save Still to Disk");
	outputControlArray->addSpacer(0, 2);
	outputControlArray->addControl(imageOutSqToDisk, "Save Sequence to Disk ");
	outputControlArray->addSpacer(0, 2);
	outputControlArray->addControl(imageOutCtImDirs, "Create Intermediate Directories");
	outputControlArray->addSpacer(0, 2);
	outputControlArray->addControl(imageOutSaveAlpha, "Save Alpha Channel");
	outputControlArray->addSpacer(0, 2);
	outputControlArray->addControl(imageOutOverwriteFile, "Overwite Existing File");

	outputControlPanel->setTitle("Image Output");
	outputControlPanel->setWidget(outputControlArray);

	output_layout->addWidget(outputControlPanel);

	tone_mapping_layout->addWidget(toneMappingControlPanel);

	rendering_layout->addWidget(globalControlPanel);
	rendering_layout->addWidget(haltControlPanel);
	rendering_layout->addWidget(pathControlPanel);
	rendering_layout->addWidget(sciVisControlPanel);
	rendering_layout->addWidget(aoControlPanel);

	mainTabWidget->addTab(renderingSettingsWidget, "Rendering");
	// mainTabWidget->addTab(toneMappingSettingsWidget, "Tone Mapping");
	mainTabWidget->addTab(outputSettingsWidget, "Output");

	main_layout->addWidget(generalControlPanel);
	main_layout->addWidget(mainTabWidget);

	widget->setLayout(main_layout);
	renderingSettingsWidget->setLayout(rendering_layout);
	outputSettingsWidget->setLayout(output_layout);
	toneMappingSettingsWidget->setLayout(tone_mapping_layout);

	QObject::connect(renderFrame, &QPushButton::clicked, this, &DfxOSPRayRenderNode::renderStill);
	QObject::connect(renderSequence, &QPushButton::clicked, this, &DfxOSPRayRenderNode::renderSequence);
	QObject::connect(renderStop, &QPushButton::clicked, this, &DfxOSPRayRenderNode::stopAllRendering);

	return widget;
}
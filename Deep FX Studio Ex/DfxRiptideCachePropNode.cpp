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

#include "DfxRiptideCachePropNode.h"
#include "DfxRiptideStageNode.h"

std::string DfxRiptideCachePropNode::_uniqueName = "UID_RIPTIDE_CACHE_PROP_NODE";

DfxRiptideCachePropNode::DfxRiptideCachePropNode(deepsg::SgStageNode *stage) : DfxSimulationStagePropBase(stage, riptide::type::CACHE) {

	currentFrameObserver = new CurrentFrameObserver(this);
	DfxTimelineManager::getInstance()->addCurrentFrameObserver(currentFrameObserver);

	locationTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Memory", Location::MEMORY));
	locationTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Disk", Location::DISK));

	paticleFormatTypeQControlenums.push_back(std::pair<std::string, unsigned int>(".BGEO", ParticleFormat::BGEO));

}

DfxRiptideCachePropNode::~DfxRiptideCachePropNode() {
	delete currentFrameObserver;
}

std::string DfxRiptideCachePropNode::uniqueName() {
	return _uniqueName;
}

std::string DfxRiptideCachePropNode::getUniqueName() {
	return _uniqueName;
}

size_t DfxRiptideCachePropNode::propState() {
	return _propState;
}

void DfxRiptideCachePropNode::setPropState(size_t ps) {
	_propState = ps;
}


DfxPointCloudNode *DfxRiptideCachePropNode::pointCloud() {
	DfxPointCloudHandler handler;

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {
		auto pcloudReference = any->rawData<fl::DataObject>()->property("point_cloud")->data<std::string>();

		if (handler.fetch(pcloudReference)) {
			return (DfxPointCloudNode*)handler.pointCloudNode();
		}
	}

	return nullptr;
}

void DfxRiptideCachePropNode::saveToFile(DfxPointFrame *frame, int frameNumber) {
	using namespace Partio;

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {
		auto dao = any->rawData<fl::DataObject>();

		auto directory = dao->property("directory")->data<std::string>();
		auto fname = dao->property("filename")->data<std::string>();

		ParticlesDataMutable* p = create();
		ParticleAttribute positionAttr = p->addAttribute("position", Partio::VECTOR, 3);
		ParticleAttribute velocityAttr = p->addAttribute("velocity", Partio::VECTOR, 3);
		ParticleAttribute colorAttr = p->addAttribute("color", Partio::VECTOR, 3);
		ParticleAttribute densityAttr = p->addAttribute("density", Partio::FLOAT, 1);
		ParticleAttribute groupAttr = p->addAttribute("group_id", Partio::INT, 1);
		ParticleAttribute speedAttr = p->addAttribute("speed", Partio::FLOAT, 1);

		for (int j = 0; j < frame->size(); j++) {

			glm::vec3 _vel = frame->getVelocities()[j];
			glm::normalize(_vel);
			float speed = glm::length(_vel);

			Partio::ParticleIndex particle = p->addParticle();

			float* pos = p->dataWrite<float>(positionAttr, particle);
			float* vel = p->dataWrite<float>(velocityAttr, particle);
			float* col = p->dataWrite<float>(colorAttr, particle);
			float* spd = p->dataWrite<float>(speedAttr, particle);
			int*   gid = p->dataWrite<int>(groupAttr, particle);

			pos[0] = frame->getPositions()[j].x;
			pos[1] = frame->getPositions()[j].y;
			pos[2] = frame->getPositions()[j].z;

			vel[0] = frame->getVelocities()[j].x;
			vel[1] = frame->getVelocities()[j].y;
			vel[2] = frame->getVelocities()[j].z;

			col[0] = frame->getColors()[j].x;
			col[1] = frame->getColors()[j].y;
			col[2] = frame->getColors()[j].z;

			*spd = speed;
			*gid = frame->getGroup()[j];

		}

		std::string ftype = "";
		std::stringstream fullpath;

		if (directory.length() > 0) {

			if (fname.length() == 0)
				fname = "no_name";

			if (ftype.length() == 0)
				ftype = ".bgeo";

			fullpath << directory << "/" << fname << ".@" << frameNumber << ftype;

			filesystem::create_directories(directory);

			Partio::write(fullpath.str().c_str(), *p);

		}

		p->release();

	}
}

void DfxRiptideCachePropNode::getPreviewColor(float speed, float limit, glm::vec3 primary, glm::vec3 secondary, glm::vec3 &out) {

	if (speed > limit)
		speed = limit;

	if (speed < 0)
		speed = 0;

	float alpha = speed / limit;

	out.x = primary.x + (alpha * (secondary.x - primary.x));
	out.y = primary.y + (alpha * (secondary.y - primary.y));
	out.z = primary.z + (alpha * (secondary.z - primary.z));
}

void DfxRiptideCachePropNode::updatePointCloud(DfxPointFrame *frame, int frameNumber) {

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());
	auto dao = any->rawData<fl::DataObject>();
	glm::vec3 secColor(0.f);
	float pVelocity = 10.f;

	auto location = dao->property("location")->data<unsigned int>();
	auto filename = dao->property("filename")->data<std::string>();
	auto directory = dao->property("directory")->data<std::string>();
	auto format = dao->property("format")->data<unsigned int>();

	auto r = dao->property("secondary_color_r")->data<float>();
	auto g = dao->property("secondary_color_g")->data<float>();
	auto b = dao->property("secondary_color_b")->data<float>();

	pVelocity = any->rawData<fl::DataObject>()->property("peak_preview_velocity")->data<float>();

	secColor = glm::vec3(r, g, b);

	if (location == DfxRiptideCachePropNode::Location::MEMORY) {
		updatePointCloud(frame, pointCloud(), pVelocity, secColor);
	}
	else if (location == DfxRiptideCachePropNode::Location::DISK) {
		updatePointCloud(frame, pointCloud(), pVelocity, secColor);
		saveToFile(frame, frameNumber);
	}

}

void DfxRiptideCachePropNode::updatePointCloud(DfxPointFrame *frame, DfxPointCloudNode *pcloud, float pVelocity, glm::vec3 secColor) {

	if (frame != NULL) {
		if (frame->size() > 0) {

			if (pcloud != nullptr) {
				auto geo = pcloud->geo();
				geo->clear();
				geo->removeAllAttributes();

				geo->resize(frame->size());
				geo->addAttribute<glm::vec3>("@color");
				geo->addAttribute<int>("@group_id");

				auto parray = geo->at<glm::vec3>("@position");
				auto carray = geo->at<glm::vec3>("@color");
				auto garray = geo->at<int>("@group_id");

				#pragma omp parallel default(shared)
				{
					#pragma omp for schedule(static)
					for (long long int i = 0; i < frame->size(); i++) {
						auto speed = frame->getSpeeds()[i];
						glm::vec3 out;

						getPreviewColor(speed, pVelocity, frame->getColors()[i], secColor, out);

						parray[i] = frame->getPositions()[i];
						carray[i] = out;
						garray[i] = frame->getGroup()[i];
					}
				}

				pcloud->emitSignal(deepsg::SgNode::Signal::UPDATED);
			}

		}
	}
}

std::vector<std::pair<QWidget*, QString>> DfxRiptideCachePropNode::propertyForms() {
	std::pair<QWidget*, QString> props(properties(), "General");
	std::pair<QWidget*, QString> viz(visualisation(), "Visualization");

	return std::vector<std::pair<QWidget*, QString>> { props, viz};
}

QWidget *DfxRiptideCachePropNode::properties() {

	QWidget *widget = new QWidget;

	DfxQStackControl *propertiesWidgetStack = new DfxQStackControl;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto generalControlPanel = new DfxControlPanel();
	auto memoryControlPanel = new DfxControlPanel();
	auto outputControlPanel = new DfxControlPanel();

	DfxQControlArray *generalControlArray = new DfxQControlArray();
	DfxQControlArray *memoryControlArray = new DfxQControlArray();
	DfxQControlArray *outputControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	QPushButton *clear_memory = new QPushButton("Clear Memory");
	auto location = new DfxQComboBoxControl(path + "location", "", locationTypeQControlenums, false);

	auto filename = new DfxQLineEditControl(path + "filename", "", false);
	auto dir = new DfxQFilePathControl(path + "directory", true, true, "Output Directory", "c:/", "", "", false);
	auto format = new DfxQComboBoxControl(path + "format", "", paticleFormatTypeQControlenums, false);

	clear_memory->setStyleSheet(clear_memory->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());

	// General
	generalControlArray->addControl("Save To", location);
	generalControlArray->addSpacer(1, 2);
	generalControlArray->addControl(propertiesWidgetStack);

	memoryControlArray->addControl(clear_memory);

	outputControlArray->addControl("Filename", filename);
	outputControlArray->addSpacer(1, 2);
	outputControlArray->addControl("Directory", dir);
	outputControlArray->addSpacer(1, 2);
	outputControlArray->addControl("Format", format);

	propertiesWidgetStack->addPage(memoryControlArray);
	propertiesWidgetStack->addPage(outputControlArray);

	propertiesWidgetStack->changeCurrentPage(location->comboBox()->currentIndex());

	generalControlPanel->setTitle("Settings");
	generalControlPanel->setWidget(generalControlArray);

	main_layout->addWidget(generalControlPanel);

	widget->setLayout(main_layout);

	// Signal & Slots
	QObject::connect(location->comboBox(), QOverload<int>::of(&QComboBox::activated), propertiesWidgetStack, &DfxQStackControl::changeCurrentPage);

	return widget;
}

QWidget *DfxRiptideCachePropNode::visualisation() {

	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto visualizationControlPanel = new DfxControlPanel();

	DfxQControlArray *visualizationControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	auto pcloud = new DfxQFluentPathControl(path + "point_cloud", "", false);

	auto psize = new DfxQDecimalSpinnerControl(path + "preview_particle_size", "", false);
	auto pvelocity = new DfxQDecimalSpinnerControl(path + "peak_preview_velocity", "", false);
	auto scolor = new DfxQColorControl(path + "secondary_color_r", path + "secondary_color_g", path + "secondary_color_b", "", false);

	// General
	visualizationControlArray->addControl("Point Cloud", pcloud);
	visualizationControlArray->addSpacer(1, 2);
	visualizationControlArray->addControl("Preview Particle Size", psize);
	visualizationControlArray->addSpacer(1, 2);
	visualizationControlArray->addControl("Peak Preview Velocity", pvelocity);
	visualizationControlArray->addSpacer(1, 2);
	visualizationControlArray->addControl("Secondary Color", scolor);

	visualizationControlPanel->setTitle("Settings");
	visualizationControlPanel->setWidget(visualizationControlArray);

	main_layout->addWidget(visualizationControlPanel);

	widget->setLayout(main_layout);

	return widget;
}

void DfxRiptideCachePropNode::clearCache() {
	_cache.clear();
}

DfxPointFrame *DfxRiptideCachePropNode::getCachedFrame(int cframe) {
	return _cache.getFrame(cframe);
}

void DfxRiptideCachePropNode::addCachedFrame(int cframe, DfxPointFrame *frame) {
	_cache.insertFrame(cframe, frame);
}

DfxRiptideCachePropNode::CurrentFrameObserver::CurrentFrameObserver(DfxRiptideCachePropNode *cn) {
	_cacheNode = cn;
}

void DfxRiptideCachePropNode::CurrentFrameObserver::update(fl::Any *any) {
	if (any != nullptr) {
		auto  val = any->data<int>();

		if (!DfxTimelineManager::getInstance()->isSimulating()) {
			auto frame = _cacheNode->getCachedFrame(val);
			if(frame != nullptr) _cacheNode->updatePointCloud(frame, val);
		}

	}
}
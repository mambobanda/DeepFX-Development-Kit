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

#include <SgPointCloudNode.h>
#include "DfxPointReaderTool.h"
#include "DfxPointReaderHandler.h"

std::string DfxPointReaderTool::_uniqueName = "UID_POINT_READER_TOOL";

DfxPointReaderTool::DfxPointReaderTool() {

	_dispatcher = nullptr;
	current_path = "";
}

DfxPointReaderTool::~DfxPointReaderTool() {

}

std::string DfxPointReaderTool::uniqueName() {
	return _uniqueName;
}

std::string DfxPointReaderTool::getUniqueName() {
	return _uniqueName;
}

bool DfxPointReaderTool::isReady() {
	return true;
}

void DfxPointReaderTool::onSetReferencePath(std::string path) {
	if (_dispatcher != nullptr) {
		delete _dispatcher;
		_dispatcher = nullptr;
	}

	auto any = DfxSpace::getInstance()->findAbsolute(path);

	if (any != nullptr) {
		_dispatcher = new DfxToolFlDispatcher(this);
		_dispatcher->attachBranch(any);
	}

	DfxToolNode::onSetReferencePath(path);
}

void DfxPointReaderTool::create(SgNode *parent) {
}

void DfxPointReaderTool::destroy() {
}

bool DfxPointReaderTool::evaluateOnUpdate() {
	return false;
}

bool DfxPointReaderTool::evaluateOnNewFrame() {
	return true;
}

bool DfxPointReaderTool::evaluateOnValueChanged() {
	return true;
}

void DfxPointReaderTool::evaluate(SgNode *parent) {
	DfxPointReaderHandler handler;

	if (parent->getType() == deepsg::SgNodeType::POINT_CLOUD) {

		auto pcloud = ((deepsg::SgPointCloudNode*)parent)->geo();

		if (handler.fetch(referencePath())) {

			auto dao = handler.dataObject()->rawData<fl::DataObject>();

			auto path = dao->property("cache_path")->data<std::string>();
			auto offset = dao->property("offset")->data<unsigned int>();
			auto frame = dao->property("frame")->data<unsigned int>();
			auto fixed_frame = dao->property("fixed_frame")->data<bool>();

			if (true) {
				pcloud->clear();
				pcloud->removeAllAttributes();
			}

			if (!path.empty()) {
				filesystem::path pth(path);

				if (filesystem::exists(path) && filesystem::is_regular_file(pth)) {

					if (path != current_path) {
						current_path = path;
						if (!loadCache()) {
							DfxGlobal::console::println("Failed to load Point Cache: " + current_path);
						}
					}

					if (!fileCacheMap.empty()) {
						auto current_frame = DfxTimelineManager::getInstance()->currentFrame();

						if (fixed_frame)
							current_frame = frame;

						current_frame -= offset;

						auto *frame = getFileCacheFrame(current_frame);

						if (frame != nullptr) {

							if (pcloud != nullptr) {

								pcloud->resize(frame->size());
								pcloud->addAttribute<glm::vec3>("@color");
								pcloud->addAttribute<int>("@group_id");

								auto parray = pcloud->at<glm::vec3>("@position");
								auto carray = pcloud->at<glm::vec3>("@color");
								auto garray = pcloud->at<int>("@group_id");

								#pragma omp parallel default(shared)
								{
									#pragma omp for schedule(static)
									for (long long int i = 0; i < frame->size(); i++) {
										auto speed = frame->getSpeeds()[i];
										glm::vec3 out;

										getPreviewColor(speed, 10.f, frame->getColors()[i], glm::vec3(1.f), out);

										parray[i] = frame->getPositions()[i];
										carray[i] = out;
										garray[i] = frame->getGroup()[i];
									}
								}
							}

							frame->clear();
							delete frame;
						}
					}
				}
			}

			parent->emitSignal(deepsg::SgNode::Signal::UPDATED);
		}
	}
}

DfxPointFrame *DfxPointReaderTool::getFileCacheFrame(int frm) {
	using namespace Partio;

	if (fileCacheMap.size() > 0) {
		std::stringstream key;
		key << "frame_" << frm;

		std::unordered_map<std::string, std::string>::iterator it = fileCacheMap.find(key.str());

		if (it == fileCacheMap.end()) {
			return NULL;
		}
		else {
			std::string path = it->second;

			Partio::ParticlesDataMutable* p = Partio::read(path.c_str());

			if (p) {
				ParticleAttribute positionAttr;
				ParticleAttribute speedAttr;
				ParticleAttribute velocityAttr;
				ParticleAttribute colorAttr;
				ParticleAttribute gidAttr;

				bool hasSpeedAttr = true;
				bool hasVelocityAttr = true;
				bool hasColorAttr = true;
				bool hasGidAttr = true;

				if (!p->attributeInfo("position", positionAttr))
					return NULL;

				if (!p->attributeInfo("speed", speedAttr))
					hasSpeedAttr = false;

				if (!p->attributeInfo("velocity", velocityAttr))
					hasSpeedAttr = false;

				if (!p->attributeInfo("color", colorAttr))
					hasColorAttr = false;

				if (!p->attributeInfo("group_id", gidAttr))
					hasGidAttr = false;

				auto *frame = new DfxPointFrame;
				int pcount = p->numParticles();

				frame->initFrame(pcount);

				#pragma omp parallel default(shared)
				{
					#pragma omp for schedule(static)
					for (int i = 0; i < pcount; i++) {
						const float* pos = p->data<float>(positionAttr, i);

						frame->getPositions()[i].x = pos[0];
						frame->getPositions()[i].y = pos[1];
						frame->getPositions()[i].z = pos[2];

						if (hasSpeedAttr) {
							const float* pos = p->data<float>(speedAttr, i);
							frame->getSpeeds()[i] = *pos;
						}

						if (hasVelocityAttr) {
							const float* pos = p->data<float>(velocityAttr, i);
							frame->getVelocities()[i].x = pos[0];
							frame->getVelocities()[i].y = pos[1];
							frame->getVelocities()[i].z = pos[2];
						}

						if (hasColorAttr) {
							const float* col = p->data<float>(colorAttr, i);
							frame->getColors()[i].x = col[0];
							frame->getColors()[i].y = col[1];
							frame->getColors()[i].z = col[2];
						}

						if (hasGidAttr) {
							const int* gid = p->data<int>(gidAttr, i);
							frame->getGroup()[i] = *gid;
						}

					}
				}

				p->release();

				return frame;

			}
			else {
				return NULL;
			}

			return NULL;
		}
	}
	else {
		return NULL;
	}

}

bool DfxPointReaderTool::loadCache() {

	fileCacheMap.clear();
	
	Partio::ParticlesDataMutable* p = Partio::read(current_path.c_str());

	if (p) {
		filesystem::path pth(current_path);

		std::string dir = pth.parent_path().string();
		std::vector<std::string> files = getDirectoryFilenames(dir);

		if (files.size() == 0)
			return false;

		for (std::string pth : files) {

			int inc = getFileIncrement(pth);

			if (inc > -1) {

				std::stringstream key;
				key << "frame_" << inc;

				std::pair<std::string, std::string> pair(key.str(), dir + "\\" + pth);
				fileCacheMap.insert(pair);
			}
		}

		if (fileCacheMap.size() <= 0)
			return false;
		else
			return true;
	}
	else {
		return false;
	}

	p->release();
}

std::vector<std::string> DfxPointReaderTool::getDirectoryFilenames(std::string dirpath) {

	std::vector <std::string> names;

	for (auto & p : filesystem::directory_iterator(dirpath)) {
		names.push_back(p.path().filename().string());
	}

	return names;
}

int DfxPointReaderTool::getFileIncrement(const std::string s) {

	std::regex reg("@\\d+.");
	std::smatch match;

	if (regex_search(s.begin(), s.end(), match, reg)) {
		std::string str = match[0];
		unsigned int val = str.find('@');
		unsigned int val2 = str.find('.');
		std::string sub = str.substr(val + 1, val2 - 1);

		return std::stoi(sub);
	}
	else
		return -1;
}

void DfxPointReaderTool::getPreviewColor(float speed, float limit, glm::vec3 primary, glm::vec3 secondary, glm::vec3 &out) {

	if (speed > limit)
		speed = limit;

	if (speed < 0)
		speed = 0;

	float alpha = speed / limit;

	out.x = primary.x + (alpha * (secondary.x - primary.x));
	out.y = primary.y + (alpha * (secondary.y - primary.y));
	out.z = primary.z + (alpha * (secondary.z - primary.z));
}

std::vector<std::pair<QWidget*, QString>> DfxPointReaderTool::propertyForms() {
	std::pair<QWidget*, QString> grid(form(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> {grid};
}

QWidget *DfxPointReaderTool::form() {

	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto propertiesControlPanel = new DfxControlPanel();

	DfxQControlArray *propertiesControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// Type Controls
	auto file_path = new DfxQFilePathControl(path + "cache_path", true, false, "Open Cache File", "c:/", "(*.bgeo)", "", false);
	auto offset = new DfxQIntegerSpinnerControl(path + "offset", "", false);
	auto frame = new DfxQIntegerSpinnerControl(path + "frame", "", false);
	auto fixed_frame = new DfxQCheckBoxControl(path + "fixed_frame", "", false);

	propertiesControlArray->addControl("File", file_path);
	propertiesControlArray->addControl("Offset", offset);
	propertiesControlArray->addControl("Frame", frame);
	propertiesControlArray->addControl("Fixed Frame", fixed_frame);

	propertiesControlPanel->setTitle("Properties");
	propertiesControlPanel->setWidget(propertiesControlArray);

	// Main layout
	main_layout->addWidget(propertiesControlPanel);

	widget->setLayout(main_layout);

	return widget;
}
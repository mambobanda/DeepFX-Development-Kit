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
#include <Partio.h>
#include "DfxPointCache.h"

// Point Frame
DfxPointFrame::DfxPointFrame() {

	pcloud = new ds::PointCloud;
	pcloud->addAttribute<glm::vec3>("@velocity");
	pcloud->addAttribute<glm::vec3>("@color");
	pcloud->addAttribute<int>("@group_id");
	pcloud->addAttribute<float>("@speed");

	pos = pcloud->channel("@position");
	vel = pcloud->channel("@velocity");
	speed = pcloud->channel("@speed");
	color = pcloud->channel("@color");
	group = pcloud->channel("@group_id");

	cached = false;
}

DfxPointFrame::~DfxPointFrame() {
	pcloud->clear();
	delete pcloud;
}

void DfxPointFrame::clear() {
	pcloud->clear();
}

ds::PointCloud *DfxPointFrame::pointCloud() {
	return pcloud;
}

void DfxPointFrame::initFrame(int count) {
	pcloud->clear();
	if (count > 0) {
		pcloud->resize(count);
		pcloud->fill<glm::vec3>("@position", glm::vec3(0.f));
		pcloud->fill<glm::vec3>("@velocity", glm::vec3(0.f));
		pcloud->fill<glm::vec3>("@color", glm::vec3(0.f));
		pcloud->fill<float>("@speed", 0.f);
		pcloud->fill<int>("@group", -1);
	}
}

glm::vec3  *DfxPointFrame::getPositions() {
	return pos->at<glm::vec3>();
}

glm::vec3  *DfxPointFrame::getVelocities() {
	return vel->at<glm::vec3>();
}

float *DfxPointFrame::getSpeeds() {
	return speed->at<float>();
}

glm::vec3 *DfxPointFrame::getColors() {
	return color->at<glm::vec3>();
}

int *DfxPointFrame::getGroup() {
	return group->at<int>();
}

int DfxPointFrame::size() {
	return pcloud->size();
}

bool DfxPointFrame::isCached() {
	return cached;
}

void DfxPointFrame::setIsCached(bool ch) {
	cached = ch;
}

// Point Cache
DfxPointCache::DfxPointCache(): DfxSimulationCacheBase(DfxSimulationCacheBase::POINT_CACHE) {
}

DfxPointCache::~DfxPointCache() {
	clear();
}

int DfxPointCache::getFrameCount() {
	return frames.size();
}

void DfxPointCache::insertFrame(int frame, DfxPointFrame *sframe) {
	std::pair<std::string, DfxPointFrame*> p(utils::indexToKey(frame), sframe);
	frames.insert(p);
}

DfxPointFrame* DfxPointCache::getFrame(int frame) {

	std::unordered_map<std::string, DfxPointFrame*>::iterator fr = frames.find(utils::indexToKey(frame));

	if (fr == frames.end()) {
		return nullptr;
	}
	else {
		DfxPointFrame* ptr = fr->second;
		return ptr;
	}

}

bool DfxPointCache::hasFrame(int frame) {
	std::unordered_map<std::string, DfxPointFrame*>::iterator fr = frames.find(utils::indexToKey(frame));

	if (fr != frames.end()) {
		return true;
	}

	return false;
}

void DfxPointCache::clear() {
	std::unordered_map<std::string, DfxPointFrame*>::iterator it = frames.begin();

	for (; it != frames.end(); it++) {
		if (it->second != nullptr) {
			it->second->clear();
			delete it->second;
			it->second = nullptr;
		}
	}

	frames.clear();
}

// 
bool DfxPointCache::toMemory() {
	return cacheToMemory;
}

bool DfxPointCache::fromFile() {
	return loadCacheFromFile;
}

void DfxPointCache::saveToMemory(bool val) {
	cacheToMemory = val;
}

bool DfxPointCache::isSaveToMemory() {
	return cacheToMemory;
}

void DfxPointCache::loadFromFile(bool val) {
	loadCacheFromFile = val;
}

bool DfxPointCache::isLoadFromFile() {
	return loadCacheFromFile;
}

std::string DfxPointCache::getFilename() {
	return filename;
}

char *DfxPointCache::getFilename_c_str() {
	return _strdup(filename.c_str());
}

void DfxPointCache::setFilename(std::string fn) {
	filename = fn;
}

std::string DfxPointCache::getDirectory() {
	return directory;
}

char *DfxPointCache::getDirectory_c_str() {
	return _strdup(directory.c_str());
}

void DfxPointCache::setDirectory(std::string dir) {
	directory = dir;
}

void DfxPointCache::setDirectory_c_str(char *dir) {
	directory = dir;
}

std::string DfxPointCache::getCacheFilepath() {
	return cacheFilepath;
}

char *DfxPointCache::getCacheFilepath_c_str() {
	return _strdup(cacheFilepath.c_str());
}

void DfxPointCache::setCacheFilepath(std::string cp) {
	cacheFilepath = cp;
}

void DfxPointCache::setCacheFilepath_c_str(char *cp) {
	cacheFilepath = cp;
}

std::string DfxPointCache::getCacheFiletype() {
	return filetype;
}

char *DfxPointCache::getCacheFiletype_c_str() {
	return _strdup(filetype.c_str());
}

void DfxPointCache::setCacheFiletype(std::string ft) {
	filetype = ft;
}

bool DfxPointCache::loadPartioFileCache() {
	fileCacheMap.clear();

	if (cacheFilepath.length() > 0) {
		Partio::ParticlesDataMutable* p = Partio::read(cacheFilepath.c_str());

		if (p) {

			std::string dir = getFileDirName(cacheFilepath);
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
		}
		else {
			return false;
		}

		p->release();
		return true;
	}

	return false;
}

DfxPointFrame *DfxPointCache::getPartioFileCacheFrame(int frm) {

	if (fileCacheMap.size() > 0) {
		std::stringstream key;
		key << "frame_" << frm;

		std::unordered_map<std::string, std::string>::iterator it = fileCacheMap.find(key.str());

		if (it == fileCacheMap.end()) {
			return nullptr;
		}
		else {
			std::string path = it->second;

			Partio::ParticlesDataMutable* p = Partio::read(path.c_str());

			if (p) {
				Partio::ParticleAttribute positionAttr;
				Partio::ParticleAttribute speedAttr;
				Partio::ParticleAttribute velocityAttr;
				Partio::ParticleAttribute colorAttr;
				Partio::ParticleAttribute gidAttr;

				bool hasSpeedAttr = true;
				bool hasVelocityAttr = true;
				bool hasColorAttr = true;
				bool hasGidAttr = true;

				if (!p->attributeInfo("position", positionAttr))
					return nullptr;

				if (!p->attributeInfo("speed", speedAttr))
					hasSpeedAttr = false;

				if (!p->attributeInfo("velocity", velocityAttr))
					hasSpeedAttr = false;

				if (!p->attributeInfo("color", colorAttr))
					hasColorAttr = false;

				if (!p->attributeInfo("group_id", gidAttr))
					hasGidAttr = false;

				DfxPointFrame *frame = new DfxPointFrame;
				int pcount = p->numParticles();

				frame->initFrame(pcount);

				for (int i = 0; i < pcount; i++) {
					const float* pos = p->data<float>(positionAttr, i);

					frame->getPositions()[i].x = pos[0];
					frame->getPositions()[i].y = pos[1];
					frame->getPositions()[i].z = pos[2];

					if (hasSpeedAttr) {
						const float* pos = p->data<float>(speedAttr, i);
						frame->getSpeeds()[i] = *pos;
					}
					else {
						frame->getSpeeds()[i] = 1.0f;
					}

					if (hasVelocityAttr) {
						const float* pos = p->data<float>(velocityAttr, i);
						frame->getVelocities()[i].x = pos[0];
						frame->getVelocities()[i].y = pos[1];
						frame->getVelocities()[i].z = pos[2];
					}
					else {
						frame->getVelocities()[i] = glm::vec3(0.0f);
					}

					if (hasColorAttr) {
						const float* col = p->data<float>(colorAttr, i);
						frame->getColors()[i].x = col[0];
						frame->getColors()[i].y = col[1];
						frame->getColors()[i].z = col[2];
					}
					else {
						frame->getColors()[i] = glm::vec3(0.0f, 0.f, 1.0f);
					}

					if (hasGidAttr) {
						const int* gid = p->data<int>(gidAttr, i);
						frame->getGroup()[i] = *gid;
					}
					else {
						frame->getGroup()[i] = -1;
					}
				}

				p->release();

				return frame;

			}
			else {
				return nullptr;
			}

			return nullptr;
		}
	}
	else {
		return nullptr;
	}
}

size_t DfxPointCache::fileCacheMapSize() {
	return fileCacheMap.size();
}

int DfxPointCache::getFileIncrement(const std::string s) {

	std::regex reg("@\\d+.");
	std::smatch match;

	if (regex_search(s.begin(), s.end(), match, reg)) {
		std::string str = match[0];
		size_t val = str.find('@');
		size_t val2 = str.find('.');
		std::string sub = str.substr(val + 1, val2 - 1);

		return stoi(sub);
	}
	else
		return -1;
}

void DfxPointCache::makeDirectory(std::string path) {
	filesystem::create_directories(path.c_str());
}

std::string DfxPointCache::getFileDirName(std::string path) {

	bool useBackSlash = true;

	std::size_t found = path.rfind("\\");

	if (found == std::string::npos)
		useBackSlash = false;

	std::size_t index = -1;
	if (useBackSlash) {
		index = path.rfind("\\");
	}
	else {
		index = path.rfind("/");
	}

	if (std::string::npos != index) {
		return path.substr(0, index);
	}
	else {
		return path;
	}
}

std::vector<std::string> DfxPointCache::getDirectoryFilenames(std::string dirpath) {

	std::vector<std::string> names;

	for (auto &file : filesystem::directory_iterator(dirpath)) {
		names.push_back(file.path().stem().string() + file.path().extension().string());
	}

	return names;
}

void DfxPointCache::savePartioFileCache(DfxPointFrame *frame, int cframe, std::string directory, std::string filename) {

	Partio::ParticlesDataMutable* p = Partio::create();
	Partio::ParticleAttribute positionAttr = p->addAttribute("position", Partio::VECTOR, 3);
	Partio::ParticleAttribute velocityAttr = p->addAttribute("velocity", Partio::VECTOR, 3);
	Partio::ParticleAttribute colorAttr = p->addAttribute("color", Partio::VECTOR, 3);
	Partio::ParticleAttribute groupAttr = p->addAttribute("group_id", Partio::INT, 1);
	Partio::ParticleAttribute speedAttr = p->addAttribute("speed", Partio::FLOAT, 1);

	for (size_t i = 0; i < frame->size(); i++) {

		glm::vec3 _vel = frame->getPositions()[i];
		glm::normalize(_vel);
		float speed = glm::length(_vel);

		Partio::ParticleIndex particle = p->addParticle();

		float* pos = p->dataWrite<float>(positionAttr, particle);
		float* vel = p->dataWrite<float>(velocityAttr, particle);
		float* col = p->dataWrite<float>(colorAttr, particle);
		float* spd = p->dataWrite<float>(speedAttr, particle);
		int*   gid = p->dataWrite<int>(groupAttr, particle);

		pos[0] = frame->getPositions()[i].x;
		pos[1] = frame->getPositions()[i].y;
		pos[2] = frame->getPositions()[i].z;

		vel[0] = frame->getVelocities()[i].x;
		vel[1] = frame->getVelocities()[i].y;
		vel[2] = frame->getVelocities()[i].z;

		col[0] = frame->getColors()[i].x;
		col[1] = frame->getColors()[i].y;
		col[2] = frame->getColors()[i].z;

		*spd = frame->getSpeeds()[i];
		*gid = frame->getGroup()[i];

	}

	std::string fname = filename;
	std::stringstream fullpath;
	std::string ftype = ".bgeo";

	if (directory.length() > 0) {

		if (fname.length() == 0)
			fname = "no_name";

		fullpath << directory << "/" << fname << ".@" << cframe << ftype;

		makeDirectory(directory);

		Partio::write(fullpath.str().c_str(), *p);

	}

	p->release();
}
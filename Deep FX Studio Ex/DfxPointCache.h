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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <regex>
#include <filesystem>
#include <DsPointCloud.h>
#include "DfxSimulationCacheBase.h"
#include "DfxCacheUtils.h"

namespace filesystem = std::experimental::filesystem;

class DfxPointFrame {
private:

	ds::PointCloud *pcloud;
	ds::Channel *pos;
	ds::Channel *vel;
	ds::Channel *speed;
	ds::Channel *color;
	ds::Channel *group;

	bool cached;

public:

	DfxPointFrame();
	virtual ~DfxPointFrame();

	void clear();
	void initFrame(int count);

	glm::vec3  *getPositions();
	glm::vec3  *getVelocities();
	glm::vec3  *getColors();
	int		   *getGroup();
	float *getSpeeds();
	int size();
	bool isCached();
	void setIsCached(bool);

	ds::PointCloud *pointCloud();
};

class DfxPointCache: public DfxSimulationCacheBase {
private:
	std::unordered_map< std::string, DfxPointFrame*> frames;

private:
	std::unordered_map<std::string, std::string> fileCacheMap;

	bool save;
	bool cacheToMemory;
	bool loadCacheFromFile;

	std::string filename;
	std::string directory;
	std::string cacheFilepath;
	std::string filetype;

public:
	DfxPointCache();
	virtual ~DfxPointCache();
	int getFrameCount();
	void insertFrame(int frame, DfxPointFrame *);
	
	DfxPointFrame* getFrame(int frame);
		
	bool hasFrame(int) override;
	void clear() override;

public:

	size_t fileCacheMapSize();

	int getFileIncrement(const std::string s);

	void makeDirectory(std::string path);

	std::string getFileDirName(std::string path);

	std::vector<std::string> DfxPointCache::getDirectoryFilenames(std::string dirpath);

public:

	bool toMemory();
	bool fromFile();

	void saveToMemory(bool);
	bool isSaveToMemory();

	void loadFromFile(bool);
	bool isLoadFromFile();

	std::string getFilename();
	char *getFilename_c_str();
	void setFilename(std::string);

	char *getDirectory_c_str();
	std::string getDirectory();
	void setDirectory(std::string);
	void setDirectory_c_str(char *);

	std::string getCacheFilepath();
	char *getCacheFilepath_c_str();
	void setCacheFilepath(std::string);
	void setCacheFilepath_c_str(char *);

	std::string getCacheFiletype();
	char *getCacheFiletype_c_str();
	void setCacheFiletype(std::string);

	bool loadPartioFileCache();

	DfxPointFrame *getPartioFileCacheFrame(int);

	void savePartioFileCache(DfxPointFrame *frame, int cframe, std::string directory, std::string filename);
};


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

#include <QObject>
#include <unordered_map>
#include "DfxPropertyConduitType.h"
#include "DfxKeyFrameManager.h"


class DfxPropertyConduit: public QObject{
	Q_OBJECT
private:
	static DfxPropertyConduit *_conduitInstance;
	std::unordered_map<std::string, int> _conduitMap;

	DfxPropertyConduit();
public:

	virtual ~DfxPropertyConduit();

	static DfxPropertyConduit *getInstance();

	// Float Values
	void createKeyframeFloatOverride(std::string path, float originalValue);

	void destroyframeFloatOverride(std::string path);

	size_t floatKeyframeOverrideSize(std::string path);

	void removeKeyframeFloatOverride(std::string path, int key);

	void updateKeyframeFloatOverride(std::string path, int key, float value);

	void updateKeyframeFloatInterpolationType(std::string path, int key, unsigned int type);

	void updateKeyframeFloatControlPoints(std::string path, int key, std::pair<glm::vec2, glm::vec2> points);

	float evaluateKeyframeFloatOverride(std::string path, int key);

	void applyKeyframeFloatOverride(std::string path, int key);

	fl::AnyMap<int, float> *findFloatKeyFrameMap(std::string path);

	static void clearKeyframeFloatOverrideCallback(fl::Any *any);

	// Utility
	std::vector<std::string> allConduitPaths();

	bool hasKeyframeOverride(std::string path);

	/*int keyframeOverrideType(std::string path);*/

	void announceNewKeyframeCreated();

	void announceKeyFrameUpdated(std::string path);

signals:

	void newKeyFrameCreated();
	void keyFrameUpdated(std::string path);
};
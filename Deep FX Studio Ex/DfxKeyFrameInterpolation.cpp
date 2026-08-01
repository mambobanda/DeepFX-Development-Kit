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

#include "DfxKeyFrameInterpolation.h"

float DfxKeyFrameInterpolation::interpolateFloat(int key, fl::AnyMap<int, float> *_map) {

	if (_map->hasKey(key)) {
		return _map->value(key);
	}
	else {

		int smallest;
		bool hasSmallest = _map->smallestBounds(key, smallest);

		int highest;
		bool hasBiggest = _map->highestBounds(key, highest);

		unsigned int interpol = fl::InterpolationType::CONSTANT;

		if (hasBiggest) interpol = _map->interpolationType(highest);

		if (interpol == fl::InterpolationType::CONSTANT) {

			if (!hasSmallest) {
				if (hasBiggest) {
					return _map->value(highest);
				}
				else {
					return _map->originalValue();
				}
			}
			else {
				return _map->value(smallest);
			}
		}
		else if (interpol == fl::InterpolationType::LINEAR) {

			if (hasSmallest && hasBiggest) {
				float a = _map->value(smallest);
				float b = _map->value(highest);
				float ratio = float((highest - smallest) - (highest - key)) / float(highest - smallest);
				return glm::lerp(a, b, ratio);
			}
			else if (hasSmallest) {
				return _map->value(smallest);
			}
			else if (hasBiggest) {
				return _map->value(highest);
			}
			else {
				return _map->originalValue();
			}
		}
		else if (interpol == fl::InterpolationType::BEZIER) {

			if (hasSmallest && hasBiggest) {
				float a = _map->value(smallest);
				float b = _map->value(highest);

				auto cpointsA = _map->controlPoints(smallest);
				auto cpointsB = _map->controlPoints(highest);

				float ratio = float((highest - smallest) - (highest - key)) / float(highest - smallest);

				glm::vec2 p0((float)key, (float)a);
				glm::vec2 p1((float)key, (float)b);
				glm::vec2 c0(cpointsA.second.x, cpointsA.second.y);
				glm::vec2 c1(cpointsB.first.x, cpointsB.first.y);

				auto val = util::cubicBezierInterpolation(ratio, p0, p1, c0, c1);
				return val.y;
			}
			else if (hasSmallest) {
				return _map->value(smallest);
			}
			else if (hasBiggest) {
				return _map->value(highest);
			}
			else {
				return _map->originalValue();
			}
		}
	}

	return _map->originalValue();
}

int DfxKeyFrameInterpolation::interpolateInt(int key, fl::AnyMap<int, int> *_map) {

	if (_map->hasKey(key)) {
		return _map->value(key);
	}
	else {

		int smallest;
		bool hasSmallest = _map->smallestBounds(key, smallest);

		int highest;
		bool hasBiggest = _map->highestBounds(key, highest);

		if (!hasSmallest) {
			if (hasBiggest) {
				return _map->value(highest);
			}
			else {
				return _map->originalValue();
			}
		}
		else {
			return _map->value(smallest);
		}

	}
}

std::string DfxKeyFrameInterpolation::interpolateString(int key, fl::AnyMap<int, std::string> *_map) {

	if (_map->hasKey(key)) {
		return _map->value(key);
	}
	else {

		int smallest;
		bool hasSmallest = _map->smallestBounds(key, smallest);

		int highest;
		bool hasBiggest = _map->highestBounds(key, highest);

		if (!hasSmallest) {
			if (hasBiggest) {
				return _map->value(highest);
			}
			else {
				return _map->originalValue();
			}
		}
		else {
			return _map->value(smallest);
		}
	
	}
}
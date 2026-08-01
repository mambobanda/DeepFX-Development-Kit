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
   * @file FlAnyMap.h
   *
   * @brief Fl::AnyMap maps variable data to a specific Fl::Any in a space using a path
   *		it also supports interpolation of floating point values using linear and
   *		beizer interpolation.
   *
   * @author Mambo Banda
   *
   */

#pragma once


#include <map>
#include <vector>
#include <limits>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/spline.hpp>
#include <glm/vec3.hpp>
#include <type_traits>
#include "FlInterpolation.h"
#include "FlCommon.h"


namespace fl {

	namespace  InterpolationType { enum { CONSTANT = 0, LINEAR = 1, BEZIER = 2 }; };

	float toFloat(float value);

	float toFloat(int value);

	float toFloat(std::string value);

	template<typename Key, typename Compare, typename Value>
	bool _smallestBounds(std::map<Key, Value> arr, Compare val, Key &result) {
		bool found = false;
		Key base = std::numeric_limits<Key>::min();

		for (auto v : arr) {
			if (v.first < val) {
				if (v.first > base) base = v.first;
				found = true;
			}
		}

		if (found) result = base;

		return found;
	}

	template<typename Key, typename Compare, typename Value>
	bool _highestBounds(std::map<Key, Value> arr, Compare val, Key &result) {
		bool found = false;
		int base = std::numeric_limits<int>::max();

		for (auto v: arr) {
			if (v.first > val) {
				if (v.first < base) base = v.first;
				found = true;
			}
		}

		if (found) result = base;

		return found;
	}

	template<typename Key, typename Value>
	class AnyMap {
	private:
		std::map<Key, Value> _map;
		std::map<Key, unsigned int> _mapInterpolationType;
		std::map<Key, std::pair<glm::vec2, glm::vec2>> _mapControlPoints;
		Value _originalValue;
		std::string _path;

	private:

		typename std::map<Key, Value>::iterator min_value(Value v);
	public:

		AnyMap(std::string path, Value originalValue);
		virtual ~AnyMap();

		void insertValue(Key key, Value value);
		void removeValue(Key key);
		void moveKey(Key from, Key to);
		Value value(Key key);
		bool hasKey(Key key);
		std::pair<glm::vec2, glm::vec2> controlPoints(Key key);
		void updateControlPoints(Key key, std::pair<glm::vec2, glm::vec2> pts);
		unsigned int interpolationType(Key key);
		void changeInterpolationType(Key key, unsigned int type);

		// Value interpolate(Key key);
		
		Value originalValue();

		std::vector<Key> keys();
		
		size_t size();

		std::string path();
		void updatePath(std::string str);

		bool smallestBounds(Key val, Key &result);
		bool highestBounds(Key val, Key &result);
	};

	template<typename Key, typename Value>
	AnyMap<Key, Value>::AnyMap(std::string path, Value originalValue) {
		_path = path;
		_originalValue = originalValue;
	}

	template<typename Key, typename Value>
	AnyMap<Key, Value>::~AnyMap() {

	}

	template<typename Key, typename Value>
	typename std::map<Key, Value>::iterator AnyMap<Key, Value>::min_value(Value wantedvalue) {

		return std::min_element(_map.begin(), _map.end(),
			[&](const auto &p1, const auto &p2)
		{
			return (p1.first - wantedvalue) < (p2.first - wantedvalue);
		});
	}

	template<typename Key, typename Value>
	unsigned int AnyMap<Key, Value>::interpolationType(Key key) {
		auto itr = _map.find(key);

		if (itr != _map.end()) {
			return _mapInterpolationType[key];
		}

		return InterpolationType::CONSTANT;
	}

	template<typename Key, typename Value>
	void AnyMap<Key, Value>::changeInterpolationType(Key key, unsigned int type) {
		
		auto itr = _map.find(key);

		if (itr != _map.end()) {
			_mapInterpolationType[key] = type;		
		}
	}

	template<typename Key, typename Value>
	void AnyMap<Key, Value>::insertValue(Key key, Value value) {

		auto itr = _map.find(key);
		if (itr == _map.end()) {
			_mapInterpolationType[key] = InterpolationType::BEZIER;
			_mapControlPoints[key] = std::pair<glm::vec2, glm::vec2>(glm::vec2(((float)key) - 5.f, toFloat(value)), glm::vec2(((float)key) + 5.f, toFloat(value)));
		}

		_map[key] = value;
	}

	template<typename Key, typename Value>
	void AnyMap<Key, Value>::removeValue(Key key) {
		_map.erase(key);
		_mapInterpolationType.erase(key);
		_mapControlPoints.erase(key);
	}

	template<typename Key, typename Value>
	void AnyMap<Key, Value>::moveKey(Key from, Key to) {
		auto itr = _map.find(from);

		if (itr != _map.end()) {
			_map[to] = itr->second;
			_map.erase(itr);
			_mapInterpolationType[to] = _mapInterpolationType[from];
			_mapInterpolationType.erase(from);
			_mapControlPoints[to] = _mapControlPoints[from];
			_mapControlPoints.erase(from);
		}
	}

	template<typename Key, typename Value>
	Value AnyMap<Key, Value>::value(Key key) {
		return _map[key];
	}

	/*template<typename Key, typename Value>
	Value AnyMap<Key, Value>::interpolate(Key key) {

		static_assert(std::is_integral<Key>::value, "Fl::AnyMap::interpolate requires numeric keys.");
		static_assert(
			std::is_floating_point<Value>::value ||  std::is_integral<Value>::value || std::is_same<std::string, Value>::value,
			"Fl::AnyMap::interpolate requires float / double values.");

		auto itr00 = _map.find(key);

		if (itr00 != _map.end()) {
			
			unsigned int interpol = _mapInterpolationType.find(key)->second;
			if (interpol == InterpolationType::CONSTANT) {
				return itr00->second;
			}
			else if (interpol == InterpolationType::LINEAR) {
				return itr00->second;
			}
			else if (interpol == InterpolationType::BEZIER) {
				return itr00->second;
			}
		}
		else {
			
			Key smallest;
			bool hasSmallest = smallestBounds(_map, key, smallest);

			Key highest;
			bool hasBiggest = highestBounds(_map, key, highest);

			unsigned int interpol = InterpolationType::CONSTANT;

			if(hasBiggest) interpol = _mapInterpolationType.find(highest)->second;
			 
			if (interpol == InterpolationType::CONSTANT) {

				if (!hasSmallest) {
					if (hasBiggest) {
						return _map[highest];
					}
					else {
						return _originalValue;
					}
				}
				else {
					return _map[smallest];
				}
			}
			else if (interpol == InterpolationType::LINEAR) {
				
				if (hasSmallest && hasBiggest) {
					Value a = _map[smallest];
					Value b = _map[highest];
					Value ratio = (Value)((highest - smallest)-(highest - key)) / (Value)(highest - smallest);
					return glm::lerp(a, b, ratio);
				}
				else if (hasSmallest) {
					return _map[smallest];
				}
				else if (hasBiggest) {
					return _map[highest];
				}
				else {
					return _originalValue;
				}
			}
			else if (interpol == InterpolationType::BEZIER) {

				if (hasSmallest && hasBiggest) {
					Value a = _map[smallest];
					Value b = _map[highest];
					
					auto cpointsA = _mapControlPoints[smallest];
					auto cpointsB = _mapControlPoints[highest];

					Value ratio = (Value)((highest - smallest) - (highest - key)) / (Value)(highest - smallest);
					
					glm::vec2 p0((float)key, (float)a);
					glm::vec2 p1((float)key, (float)b);
					glm::vec2 c0(cpointsA.second.x, cpointsA.second.y);
					glm::vec2 c1(cpointsB.first.x, cpointsB.first.y);

					auto val = util::cubicBezierInterpolation(ratio, p0, p1, c0, c1);
					return val.y;
				}
				else if (hasSmallest) {
					return _map[smallest];
				}
				else if (hasBiggest) {
					return _map[highest];
				}
				else {
					return _originalValue;
				}
			}
		}

		return _originalValue;
	}*/

	template<typename Key, typename Value>
	bool AnyMap<Key, Value>::hasKey(Key key) {
		auto itr = _map.find(key);

		if (itr == _map.end()) return false;
		else return true;
	}

	template<typename Key, typename Value>
	std::pair<glm::vec2, glm::vec2> AnyMap<Key, Value>::controlPoints(Key key) {
		auto itr = _mapControlPoints.find(key);

		if (itr != _mapControlPoints.end()) {
			return itr->second;
		}

		return std::pair<glm::vec2, glm::vec2>(glm::vec2(0.f), glm::vec2(0.f));
	}

	template<typename Key, typename Value>
	void AnyMap<Key, Value>::updateControlPoints(Key key, std::pair<glm::vec2, glm::vec2> pts) {
		auto itr = _mapControlPoints.find(key);

		if (itr != _mapControlPoints.end()) {
			itr->second = pts;
		}
	}

	template<typename Key, typename Value>
	Value AnyMap<Key, Value>::originalValue() {
		return _originalValue;
	}

	template<typename Key, typename Value>
	std::string AnyMap<Key, Value>::path() {
		return _path;
	}

	template<typename Key, typename Value>
	void AnyMap<Key, Value>::updatePath(std::string str) {
		_path = str;
	}

	template<typename Key, typename Value>
	std::vector<Key> AnyMap<Key, Value>::keys() {
		std::vector<Key> ks;

		for (auto k : _map) {
			ks.push_back(k.first);
		}

		return ks;
	}

	template<typename Key, typename Value>
	size_t AnyMap<Key, Value>::size() {
		return _map.size();
	}

	template<typename Key, typename Value>
	bool AnyMap<Key, Value>::smallestBounds(Key val, Key &result) {
		return _smallestBounds(_map, val, result);
	}

	template<typename Key, typename Value>
	bool AnyMap<Key, Value>::highestBounds(Key val, Key &result) {
		return _highestBounds(_map, val, result);
	}
}
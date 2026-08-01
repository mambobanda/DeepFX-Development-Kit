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

#pragma once


#include <map>
#include <chrono>
#include "FlCommon.h"
#include "FlObserver.h"

namespace fl {

	namespace util {

		template<typename T, typename S>
		std::vector<T> unordered_map_keys(std::unordered_map<T, S> map) {
			std::vector<T> keys;

			for (auto p : map) {
				keys.push_back(p.first);
			}

			return keys;
		}

		template<typename T, typename S>
		std::vector<T> map_keys(std::map<T, S> map) {
			std::vector<T> keys;

			for (auto p : map) {
				keys.push_back(p.first);
			}

			return keys;
		}

		template<typename T, typename S>
		std::vector<S> unordered_map_values(std::unordered_map<T, S> map) {
			std::vector<S> values;

			for (auto p : map) {
				values.push_back(p.second);
			}

			return values;
		}

		template<typename T, typename S>
		std::vector<S> map_values(std::map<T, S> map) {
			std::vector<S> values;

			for (auto p : map) {
				values.push_back(p.second);
			}

			return values;
		}

		std::string makeStringUnique(std::string str, std::vector<std::string> lst);

		// Splitter Class
		class StringSplitter {
		private:
			std::string _str;
			std::vector<std::string> words;
			char token;
		public:

			StringSplitter(std::string str, char token = '/');

			std::vector<std::string> split(char token);

			std::string first();

			std::string next();

			size_t count();

			std::vector<std::string> &wordList();

			bool isSplittable();
		};

		// Random Integer classes
		class RandInt {
		private:
			std::default_random_engine generator;
			int f;
			int t;
		public:
			RandInt();

			RandInt(int from, int to);

			void setFrom(int from);

			void setTo(int to);

			int operator()();
		};

		// Random Float classes
		class RandFloat {
		private:
			std::default_random_engine generator;
			float f;
			float t;
		public:

			RandFloat();

			RandFloat(float from, float to);

			void setFrom(float from);

			void setTo(float to);

			float operator()();
		};

		// Fluent Any Observer Soft Link
		class ObserverSoftLink {
		private:
			std::pair<std::string, Observer *> _link;
			Space *_space;
		public:
			ObserverSoftLink(Space *space, std::string, Observer *);
			virtual ~ObserverSoftLink();
		};

		class xTimer {
		private:
			std::chrono::system_clock::time_point start;
			double finalTime;

		public:

			inline void startTimer() {
				start = std::chrono::system_clock::now();
			}

			inline void stopTimer() {
				auto diff = std::chrono::system_clock::now() - start;
				finalTime = std::chrono::duration <double, std::milli>(diff).count();
			}

			inline double getTime() {
				return finalTime;
			}
		};
	}
}
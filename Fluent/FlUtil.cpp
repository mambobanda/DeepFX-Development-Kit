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

#include "FlSpace.h"
#include "FlUtil.h"

namespace fl {

	namespace util {

		std::string makeStringUnique(std::string str, std::vector<std::string> lst) {
			bool done = false;
			std::string uniqueString = str;

			size_t counter = 1;
			while (!done) {
				done = true;
				for (auto in_str : lst) {
					if (in_str == uniqueString) {
						uniqueString = str + std::to_string(counter++);
						done = false;
					}
				}
			}

			return uniqueString;
		}

		// String Splitter
		StringSplitter::StringSplitter(std::string str, char _token) {
			_str = str;
			token = _token;
			words = split(_token);
		}

		std::vector<std::string> StringSplitter::split(char token) {

			std::vector<std::string> cont;
			std::stringstream ss(_str);
			std::string output;

			while (std::getline(ss, output, token)) {
				cont.push_back(output);
			}

			return cont;
		}

		std::string StringSplitter::first() {

			if (!words.empty()) {
				return words[0];
			}
			else {
				return "";
			}
		}

		std::string StringSplitter::next() {

			if (!words.empty()) {
				
				if (words.size() >= 2) {
					std::stringstream ss;
					for (int i = 1; i < words.size(); i++) {
						ss << words[i];

						if ((i + 1) < words.size()) ss << "/";
					}
					return ss.str();
				}
			}
			
			return "";
		}

		std::vector<std::string> &StringSplitter::wordList() {
			return words;
		}

		size_t StringSplitter::count() {
			return words.size();
		}

		bool StringSplitter::isSplittable() {
			return (words.size() > 1);
		}
		
		RandInt::RandInt() {
			f = 1;
			t = std::numeric_limits<int>::max();
		}

		RandInt::RandInt(int from, int to) {
			f = from;
			t = to;
		}

		void RandInt::setFrom(int from) {
			f = from;
		}

		void RandInt::setTo(int to) {
			t = to;
		}

		int RandInt::operator()() {
			std::uniform_int_distribution<int> distribution(f, t);
			return distribution(generator);
		}
				
		RandFloat::RandFloat() {
			f = 1.f;
			t = std::numeric_limits<float>::max();
		}

		RandFloat::RandFloat(float from, float to) {
			f = from;
			t = to;
		}

		void RandFloat::setFrom(float from) {
			f = from;
		}

		void RandFloat::setTo(float to) {
			t = to;
		}

		float RandFloat::operator()() {
			std::uniform_real_distribution<float> distribution(f, t);
			return distribution(generator);
		}
	
		ObserverSoftLink::ObserverSoftLink(Space *space, std::string path, Observer *obs) {
			_link = std::pair<std::string, Observer *>(path, obs);
			_space;
		}
		
		ObserverSoftLink::~ObserverSoftLink() {
			auto any = _space->findAbsolute(_link.first);
			if (any != nullptr) delete _link.second;
		}
		
	}
}
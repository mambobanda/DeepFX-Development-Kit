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

#include "FlInterpreterScope.h"
#include "FlSpace.h"

namespace fl {

	InterpreterScope::InterpreterScope(Space *space) {
		_space = space;
	}

	InterpreterScope::~InterpreterScope() {
		clear();
	}

	void InterpreterScope::addOutput(std::string path, Any* any) {
		output_map[path] = any;
	}

	Any *InterpreterScope::findOutput(std::string path) {
		auto itr = output_map.find(path);

		if (itr != output_map.end()) return itr->second;

		return nullptr;
	}

	std::vector<Any*> InterpreterScope::fetchInput(std::string path) {
		std::vector<Any*> inputs;
		std::vector<Any*> compoundPorts;

		if (_space->hasBinding(path)) {
			auto links = _space->findLinks(path);

			for (auto link: links) {
				auto l_o_any = _space->findAbsolute(link.output_path);

				if (l_o_any != nullptr) {
					if (l_o_any->type() == fl::type::COMPOUND_PORT) {
						compoundPorts.push_back(l_o_any);
					}
					else {
						auto itr = output_map.find(link.output_path);

						if (itr != output_map.end()) {
							inputs.push_back(itr->second);
						}
						else {
							auto any = _space->findAbsolute(link.output_path);

							if (any != nullptr) {
								inputs.push_back(any);
							}
						}
					}
				}
			}

			for (auto any : compoundPorts) {
				auto hardLink = findHardLink(any);

				if (hardLink != nullptr) {
					auto itr = output_map.find(hardLink->absolutePath());

					if (itr != output_map.end()) {
						inputs.push_back(itr->second);
					}
					else {
						inputs.push_back(hardLink);
					}
				}
			}
		}
		else {
			auto itr = output_map.find(path);

			if (itr != output_map.end()) {
				inputs.push_back(itr->second);
			}
			else {
				auto any = _space->findAbsolute(path);

				if (any != nullptr) {
					inputs.push_back(any);
				}
			}
		}

		return inputs;
	}

	Any *InterpreterScope::findHardLink(Any *any) {
		Any *output = nullptr;

		if (any == nullptr) return nullptr;
		else if (any->type() != fl::type::COMPOUND_PORT) return any;
		else {
			auto links = _space->findLinks(any->absolutePath());

			if (links.empty()) return nullptr;
			else {
				for (auto link: links) {
					auto l_o_any = _space->findAbsolute(link.output_path);

					if (l_o_any != nullptr) {
						output = findHardLink(l_o_any);
					}
				}
			}
		}

		return output;
	}

	void InterpreterScope::clear() {
		for (auto val : output_map) {
			delete val.second;
		}

		output_map.clear();
	}

}

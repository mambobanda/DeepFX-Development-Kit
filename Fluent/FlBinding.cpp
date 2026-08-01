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

#include "FlBinding.h"


namespace fl {

	Binding::Binding() {

	}

	Binding::~Binding() {

	}

	void Binding::addLink(std::string output_path, std::string output_node, std::string input_path, std::string input_node, int stamp) {
		Link lk;
		lk.output_path = output_path;
		lk.output_node  = output_node;
		lk.input_path  = input_path;
		lk.input_node  = input_node;
		lk.stamp = stamp;

		_links[output_path] = lk;
	}

	Link Binding::link(std::string output_path) {
		auto itr = _links.find(output_path);

		if (itr != _links.end()) return itr->second;
		else return Link();
	}

	std::vector<Link> Binding::links() {
		return util::unordered_map_values<std::string, Link>(_links);
	}

	std::unordered_map<std::string, Link> &Binding::mapOflinks() {
		return _links;
	}

	bool Binding::hasLink(std::string output_path) {
		auto itr = _links.find(output_path);

		if (itr != _links.end()) return true;
		else return false;
	}

	void Binding::updateStamp(std::string output_path, int stamp) {
		auto itr = _links.find(output_path);

		if (itr != _links.end()) itr->second.stamp = stamp;
	}

	int Binding::linkStamp(std::string output_path) {
		auto itr = _links.find(output_path);

		if (itr != _links.end()) return itr->second.stamp;
		else return -1;
	}

	void Binding::removeLink(std::string output_path) {
		auto itr = _links.find(output_path);

		if (itr != _links.end()) _links.erase(itr);
	}
}
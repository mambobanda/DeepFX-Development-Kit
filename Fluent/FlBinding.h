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

#include "FlCommon.h"
#include "FlLink.h"
#include "FlUtil.h"

namespace fl {

	class Binding {
	public:

	private:
		std::unordered_map<std::string, Link> _links;

	public:

		Binding();

		virtual ~Binding();

		void addLink(std::string output_path, std::string output_node, std::string input_path, std::string input_node, int stamp = -1);

		Link link(std::string output_path);

		std::vector<Link> links();

		std::unordered_map<std::string, Link> &mapOflinks();

		bool hasLink(std::string output_path);

		void updateStamp(std::string output_path, int stamp);

		int linkStamp(std::string output_path);

		void removeLink(std::string output_path);
	};
}

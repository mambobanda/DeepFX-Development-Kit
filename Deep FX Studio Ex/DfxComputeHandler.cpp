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


#include "DfxComputeHandler.h"

DfxComputeHandler::DfxComputeHandler() {
	_any = nullptr;
}

DfxComputeHandler::~DfxComputeHandler() {

}

bool DfxComputeHandler::fetch(std::string path) {
	auto space = DfxSpace::getInstance();

	auto any = space->findAbsolute(path);

	if (any != nullptr) {
		_any = any;
		return true;
	}
	else {
		return false;
	}
}

fl::Any *DfxComputeHandler::any() {
	return _any;
}

bool DfxComputeHandler::create(std::string label) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto temp_branch = space->findAbsolute("~/COMPUTE");

	/// Proceed only if it exists
	if (temp_branch != nullptr) {

		auto compute = fl::DataObject::Builder()
			.setName("compute_graph")
			.setLabel(label)
			.build(space);

		/// Store reference for further use
		_any = compute;
		space->move(compute, temp_branch);

		/// Return success
		return true;
	}

	return false;
}

bool DfxComputeHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_any != nullptr) {
		space->remove(_any);
		_any = nullptr;

		return true;
	}

	return false;
}

bool DfxComputeHandler::clearAllComputationGraphs() {

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Listener Branch
	auto object_branch = space->findAbsolute("~/COMPUTE");

	/// Proceed only if it exists
	if (object_branch != nullptr) {
		for (auto any : object_branch->data<fl::DataObject>().properties()) {

			DfxComputeHandler handler;
			bool rc = handler.fetch(space->absolutePath(any));

			if (rc) {
				if (!handler.destroy()) {
					DfxGlobal::console::println("Compute Graph Handler: Clear all failed to destroy object.");
				}
			}
			else {
				DfxGlobal::console::println("Compute Graph Handler: Clear all failed to fetch object.");
			}
		}
	}
	else {
		DfxGlobal::console::println("Compute Graph Handler: Clear all failed to find branch.");
		return false;
	}

	return true;
}

std::vector<DfxComputeHandler> DfxComputeHandler::fetchAllFlowHandlers() {
	std::vector<DfxComputeHandler> output;

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Listener Branch
	auto listener_branch = space->findAbsolute("~/COMPUTE");

	/// Proceed only if it exists
	if (listener_branch != nullptr) {
		for (auto any : listener_branch->data<fl::DataObject>().properties()) {

			DfxComputeHandler handler;
			bool rc = handler.fetch(space->absolutePath(any));

			if (rc) {
				output.push_back(handler);
			}
			else {
				DfxGlobal::console::println("Compute Graph Handler: fetch all failed to fetch object.");
			}
		}
	}
	else {
		DfxGlobal::console::println("Compute Graph Handler: fetch all failed to find branch.");
	}

	return output;
}
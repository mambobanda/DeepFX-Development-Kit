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

#include "DfxListenerHandler.h"

DfxListenerHandler::DfxListenerHandler() {
	_dataObject = nullptr;
}

DfxListenerHandler::~DfxListenerHandler() {

}

bool DfxListenerHandler::fetch(std::string path) {
	auto space = DfxSpace::getInstance();

	auto any = space->findAbsolute(path);

	if (any != nullptr) {
		_dataObject = any;
		return true;
	}
	else {
		return false;
	}
}

fl::Any *DfxListenerHandler::dataObject() {
	return _dataObject;
}

bool DfxListenerHandler::create(unsigned int evt) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto temp_branch = space->findAbsolute("~/FL_WORKSPACE/listeners");

	auto eventname = event::enumToString(evt);

	/// Proceed only if it exists
	if (temp_branch != nullptr) {

		/// Object Reference
		auto listener = fl::DataObject::Builder()
			.setName("event_listener")
			.setLabel(eventname)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
			.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
			.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "event", "Event", evt)
			.build(space);

		auto flow_container = fl::DataObject::Builder()
			.setName("paths")
			.setLabel("Paths")
			.build(space);

		space->move(flow_container, listener);
		space->move(listener, temp_branch);

		/// Store reference for further use
		_dataObject = listener;

		/// Return success
		return true;
	}

	return false;
}

bool DfxListenerHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {
		space->remove(_dataObject);
		_dataObject = nullptr;

		return true;
	}

	return false;
}

bool DfxListenerHandler::removeFlowPath(std::string path) {
	auto space = DfxSpace::getInstance();
	auto paths = _dataObject->rawData<fl::DataObject>()->property("paths");

	if (paths != nullptr) {
		for (auto p_any : paths->rawData<fl::DataObject>()->properties()) {
			std::string pstr = p_any->data<std::string>();

			if (path == pstr) {
				space->remove(p_any);
				return true;
			}
		}
	}

	return false;
}

bool DfxListenerHandler::clearAllListeners() {
	
	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Listener Branch
	auto object_branch = space->findAbsolute("~/FL_WORKSPACE/listeners");

	/// Proceed only if it exists
	if (object_branch != nullptr) {
		for (auto any : object_branch->data<fl::DataObject>().properties()) {

			DfxListenerHandler handler;
			bool rc = handler.fetch(space->absolutePath(any));

			if (rc) {
				if (!handler.destroy()) {
					DfxGlobal::console::println("Event Listener Handler: Clear all failed to destroy object.");
				}
			}
			else {
				DfxGlobal::console::println("Event Listener: Clear all failed to fetch object.");
			}
		}
	}
	else {
		DfxGlobal::console::println("Event Listener: Clear all failed to find branch.");
		return false;
	}

	return true;
}

std::vector<DfxListenerHandler> DfxListenerHandler::fetchAllListenerHandlers() {
	std::vector<DfxListenerHandler> output;

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Listener Branch
	auto listener_branch = space->findAbsolute("~/FL_WORKSPACE/listeners");

	/// Proceed only if it exists
	if (listener_branch != nullptr) {
		for (auto any : listener_branch->data<fl::DataObject>().properties()) {

			DfxListenerHandler handler;
			bool rc = handler.fetch(space->absolutePath(any));

			if (rc) {
				output.push_back(handler);
			}
			else {
				DfxGlobal::console::println("Event Listener: fetch all failed to fetch object.");
			}
		}
	}
	else {
		DfxGlobal::console::println("Event Listener: fetch all failed to find branch.");
	}

	return output;
}

std::vector<DfxFlowHandler> DfxListenerHandler::fetchAllRelatedFlowGraphs() {
	std::vector<DfxFlowHandler> output;

	if (_dataObject != nullptr) {
		auto paths = _dataObject->rawData<fl::DataObject>()->property("paths");

		if (paths != nullptr) {
			for (auto p : paths->data<fl::DataObject>().propertiesOriginalOrder()) {
				if (p->type() == fl::type::STD_STRING) {
					DfxFlowHandler fhandler;
					
					if (fhandler.fetch(p->data<std::string>())) {
						output.push_back(fhandler);
					}
				}
			}
		}
	}

	return output;
}
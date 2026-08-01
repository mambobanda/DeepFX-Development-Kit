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

#include "DfxFlowHandler.h"

std::string DfxFlowHandler::model_id = "/FLOW/FLOW_INFO";
size_t DfxFlowHandler::ID_GEN = 1;

DfxFlowHandler::DfxFlowHandler() {

}

DfxFlowHandler::~DfxFlowHandler() {

}

fl::Flow *DfxFlowHandler::flow() {
	auto any = _dataObject->data<fl::DataObject>().property("flow_id");

	if (any != nullptr) {
		auto flw = DfxFlowDB::getInstance()->findFlow(any->data<size_t>());
		return flw;
	}
	else {
		return nullptr;
	}
}

fl::Any *DfxFlowHandler::createModel(fl::Space *space) {
	size_t id_g = ID_GEN++;
	std::string str_id = std::string(2 - std::to_string(id_g).length(), '0') + std::to_string(id_g);

	auto any = fl::DataObject::Builder()
		.setName("flow")
		.setLabel("Flow" + str_id)
		.setHasModel(true)
		.setModelReference(model_id)
		.addProperty(space, fl::type::SIZE_T, fl::role::NONE, "flow_id", "Flow ID", (size_t)0)
		.build(space);

	return any;
}

bool DfxFlowHandler::create(std::string label) {

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Lights Branch
	auto temp_branch = space->findAbsolute("~/FL_WORKSPACE/flows");

	/// Proceed only if it exists
	if (temp_branch != nullptr) {

		auto object = createModel(space);
		object->setLabel(label);

		auto fid = DfxFlowDB::getInstance()->newID();
		DfxFlowDB::getInstance()->addFlow(fid, new fl::Flow(space));

		object->rawData<fl::DataObject>()->property("flow_id")->setData<size_t>(fid);

		space->move(object, temp_branch);
		
		/// Store reference for further use
		_dataObject = object;

		/// Return success
		return true;
	}

	return false;
}

bool DfxFlowHandler::destroy() {
	auto space = DfxSpace::getInstance();

	if (_dataObject != nullptr) {
		auto flowPtr = flow();
		auto fid = _dataObject->rawData<fl::DataObject>()->property("flow_id")->data<size_t>();

		for (auto nd : flowPtr->nodes()) {
			if (nd->type() == fl::Flow::PROCESS) {
				for (auto f : nd->paths()) {
					auto any = space->findAbsolute(f);

					if (any != nullptr) 
						space->remove(any);
				}
			}
			else if (nd->type() == fl::Flow::DECISION) {
				if (!nd->conditionPath().empty()) {
					auto any = space->findAbsolute(nd->conditionPath());
					space->remove(any);
				}
			}
		}

		DfxFlowDB::getInstance()->removeFlow(fid);
		delete flowPtr;

		space->remove(_dataObject);
		_dataObject = nullptr;

		return true;
	}

	return false;
}

bool DfxFlowHandler::clearAllFlows() {

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Listener Branch
	auto object_branch = space->findAbsolute("~/FL_WORKSPACE/flows");

	/// Proceed only if it exists
	if (object_branch != nullptr) {
		for (auto any : object_branch->data<fl::DataObject>().properties()) {

			DfxFlowHandler handler;
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

std::vector<DfxFlowHandler> DfxFlowHandler::fetchAllFlowHandlers() {
	std::vector<DfxFlowHandler> output;

	/// Master Space
	auto space = DfxSpace::getInstance();

	/// Find Listener Branch
	auto listener_branch = space->findAbsolute("~/FL_WORKSPACE/flows");

	/// Proceed only if it exists
	if (listener_branch != nullptr) {
		for (auto any : listener_branch->data<fl::DataObject>().properties()) {

			DfxFlowHandler handler;
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
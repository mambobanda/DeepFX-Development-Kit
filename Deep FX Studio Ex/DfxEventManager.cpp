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

#include "DfxEventManager.h"
#include "DfxFluentSetManager.h"

DfxEventManager *DfxEventManager::_instance = nullptr;

DfxEventManager::DfxEventManager() {

}

DfxEventManager::~DfxEventManager() {

}

DfxEventManager *DfxEventManager::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxEventManager;
	}

	return _instance;
}

void DfxEventManager::annouceToFluentEventListeners(unsigned int event_id) {
	auto space = DfxSpace::getInstance();

	auto branch = space->findAbsolute("~/FL_WORKSPACE/listeners");

	if (branch != nullptr) {
		for (auto listener: branch->rawData<fl::DataObject>()->properties()) {
			
			if (listener->rawData<fl::DataObject>()->hasProperty("event")) {
				auto l_event_id = listener->rawData<fl::DataObject>()->property("event")->data<unsigned int>();

				if (event_id == l_event_id) {
					
					if (listener->rawData<fl::DataObject>()->hasProperty("paths")) {
						auto paths = listener->rawData<fl::DataObject>()->property("paths")->rawData<fl::DataObject>();

						for (auto p : paths->propertiesOriginalOrder()) {
							auto p_str = p->data<std::string>();
							DfxFlowHandler handler;

							if (handler.fetch(p_str)) {
								//DfxGlobal::console::println(std::string("Listener Found: ") + p_str);
								auto flow = handler.flow();

								fl::Interpreter interpreter(space, flow);
								interpreter.setPrintDebugInfo(true);
								interpreter.run();

								DfxFluentSetManager::getInstance()->clearSoftSets();
								/*QFuture<void> future = QtConcurrent::run([space, flow]() {
									fl::Interpreter interpreter(space, flow);
									interpreter.setPrintDebugInfo(true);
									interpreter.run();
								});
								
								while (future.isRunning()) {
									QApplication::processEvents();
								}*/
							}
						}
					}
				}
			}
		}
	}
}

void DfxEventManager::announceEvent(unsigned int event_id) {
	annouceToFluentEventListeners(event_id);
	emit announceEventSignal(event_id);
}
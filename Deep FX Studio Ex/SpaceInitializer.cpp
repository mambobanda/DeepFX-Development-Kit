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

#include "SpaceInitializer.h"
#include "DfxPropertyConduit.h"

namespace space {

	void initialize() {
		auto spc = DfxSpace::getInstance();

		auto root_data = fl::DataObject::Builder()
			.setName("ROOT")
			.setLabel("Root")
			.build(spc);

		auto fluent = fl::DataObject::Builder()
			.setName("FL_WORKSPACE")
			.setLabel("Fluent Work Space")
			.build(spc);

		auto fluent_listener = fl::DataObject::Builder()
			.setName("listeners")
			.setLabel("Listeners")
			.build(spc);

		auto fluent_flow = fl::DataObject::Builder()
			.setName("flows")
			.setLabel("Flows")
			.build(spc);

		fl::DataObject::Builder()
			.setName("NULLS")
			.setLabel("Nulls")
			.build(spc);

		fl::DataObject::Builder()
			.setName("COMPUTE")
			.setLabel("Computation Graphs")
			.build(spc);

		fl::DataObject::Builder()
			.setName("GRIDS")
			.setLabel("Grids")
			.build(spc);

		fl::DataObject::Builder()
			.setName("OBJS")
			.setLabel("Objects")
			.build(spc);

		fl::DataObject::Builder()
			.setName("PTCS")
			.setLabel("Point Clouds")
			.build(spc);

		fl::DataObject::Builder()
			.setName("TOOLS")
			.setLabel("Tools")
			.build(spc);

		fl::DataObject::Builder()
			.setName("GROUPS")
			.setLabel("Groups")
			.build(spc);

		fl::DataObject::Builder()
			.setName("MATS")
			.setLabel("Materials")
			.build(spc);

		fl::DataObject::Builder()
			.setName("IMGS")
			.setLabel("Images")
			.build(spc);

		fl::DataObject::Builder()
			.setName("LIGS")
			.setLabel("Lights")
			.build(spc);

		fl::DataObject::Builder()
			.setName("CAMS")
			.setLabel("Cameras")
			.build(spc);

		fl::DataObject::Builder()
			.setName("RENDER_GLOBAL")
			.setLabel("Render Global")
			.build(spc);

		fl::DataObject::Builder()
			.setName("RENDER")
			.setLabel("Render")
			.build(spc);

		fl::DataObject::Builder()
			.setName("TIME")
			.setLabel("Time")
			.addProperty(DfxSpace::getInstance(), fl::type::INT, fl::role::NONE, "fps", "Frames Per Second", 24)
			.addProperty(DfxSpace::getInstance(), fl::type::INT, fl::role::NONE, "current_frame", "Current Frame", 0)
			.addProperty(DfxSpace::getInstance(), fl::type::INT, fl::role::NONE, "last_frame", "Last Frame", 0)
			.addProperty(DfxSpace::getInstance(), fl::type::INT, fl::role::NONE, "frame_start", "Frame Start", 0)
			.addProperty(DfxSpace::getInstance(), fl::type::INT, fl::role::NONE, "frame_end", "Frame End", 250)
			.build(spc);

		fl::DataObject::Builder()
			.setName("KEY_OPTIONS")
			.setLabel("Key Options")
			.addProperty(DfxSpace::getInstance(), fl::type::INT, fl::role::NONE, "keyable", "Keyable Properties", 0)
			.build(spc);

		fl::DataObject::Builder()
			.setName("TEMP")
			.setLabel("Temporary")
			.build(spc);

		fl::DataObject::Builder()
			.setName("MISC")
			.setLabel("Miscellaneous")
			.build(spc);

		fl::DataObject::Builder()
			.setName("STAGES")
			.setLabel("Stage")
			.build(spc);

		fl::DataObject::Builder()
			.setName("STAGE_ACTORS")
			.setLabel("Stage Actors")
			.build(spc);

		fl::DataObject::Builder()
			.setName("STAGE_PROPS")
			.setLabel("Stage Props")
			.build(spc);

		// Setup Root Properties
		spc->move(fluent_listener, fluent);
		spc->move(fluent_flow, fluent);

	
		spc->addRemoveCallback(DfxPropertyConduit::clearKeyframeFloatOverrideCallback);
	}

	void reset() {

		auto space = DfxSpace::getInstance();

		auto temp = space->findAbsolute("~/NULLS");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/TEMP");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/MISC");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/FL_WORKSPACE/listeners");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/FL_WORKSPACE/flows");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/FLUENT/flow/decisions");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/CAMS");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/GRIDS");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/COMPUTE");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/STAGES");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/PTCS");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/TOOLS");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/GROUPS");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/MATS");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/IMGS");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/LIGS");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/RENDER");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/STAGE_ACTORS");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		temp = space->findAbsolute("~/STAGE_PROPS");

		if (temp != nullptr) {
			temp->rawData<fl::DataObject>()->removeAndDeleteAll();
		}

		auto time = space->findAbsolute("~/TIME");

		if (time != nullptr) {

			time->rawData<fl::DataObject>()->property("fps")->setData<int>(24);
			time->rawData<fl::DataObject>()->property("fps")->notifyObservers();

			time->rawData<fl::DataObject>()->property("current_frame")->setData<int>(0);
			time->rawData<fl::DataObject>()->property("current_frame")->notifyObservers();

			time->rawData<fl::DataObject>()->property("last_frame")->setData<int>(0);
			time->rawData<fl::DataObject>()->property("last_frame")->notifyObservers();

			time->rawData<fl::DataObject>()->property("frame_start")->setData<int>(0);
			time->rawData<fl::DataObject>()->property("frame_start")->notifyObservers();

			time->rawData<fl::DataObject>()->property("frame_end")->setData<int>(250);
			time->rawData<fl::DataObject>()->property("frame_end")->notifyObservers();

		}		
	}

	void registerCleanCallbacks() {
		DfxSceneCleaner::getInstance()->addCallback(reset);
	}
}
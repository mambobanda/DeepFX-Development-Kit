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

#include "DfxEventEnums.h"

namespace event {

	std::string enumToString(unsigned int en) {
		switch (en) {

		// Key Frame Events
		case INITIAL_KEYFRAME:
			return "Initial Keyframe";
			break;
		case STARTING_KEYFRAME_UPDATE:
			return "Starting Keyframe Update";
			break;
		case KEYFRAME_UPDATE_COMPLETE:
			return "Keyframe Update Complete";
			break;
		case FIRST_FRAME_BEFORE_KEYFRAME_UPDATE:
			return "First Frame Before Keyframe Update";
			break;
		case FIRST_FRAME_AFTER_KEYFRAME_UPDATE:
			return "First Frame After Keyframe Update";
			break;
		case LAST_FRAME_BEFORE_KEYFRAME_UPDATE:
			return "Last Frame Before Keyframe Update";
			break;
		case LAST_FRAME_AFTER_KEYFRAME_UPDATE:
			return "Last Frame After Keyframe Update";
			break;

		// Bullet Events
		case BT_STARTING_SIMULATION_STEP:
			return "Starting Bullet Simulation Step";
			break;
		case BT_SIMULATION_STEP_COMPLETE:
			return "Bullet Simulation Step Complete";
			break;
		default:
			return "N/A";
		}
	}

	unsigned int stringToEnum(std::string en) {

		// Key Frame Events
		if (en == "Starting Keyframe Update") {
			return STARTING_KEYFRAME_UPDATE;
		}
		else if (en == "Initial Keyframe") {
			return INITIAL_KEYFRAME;
		}
		else if (en == "Keyframe Update Complete") {
			return KEYFRAME_UPDATE_COMPLETE;
		}
		else if (en == "First Frame Before Keyframe Update") {
			return FIRST_FRAME_BEFORE_KEYFRAME_UPDATE;
		}
		else if (en == "First Frame After Keyframe Update") {
			return FIRST_FRAME_AFTER_KEYFRAME_UPDATE;
		}
		else if (en == "Last Frame Before Keyframe Update") {
			return LAST_FRAME_BEFORE_KEYFRAME_UPDATE;
		}
		else if (en == "Last Frame After Keyframe Upfate") {
			return LAST_FRAME_AFTER_KEYFRAME_UPDATE;
		}

		// Bullet Events
		else if (en == "Starting Bullet Simulation Step") {
			return BT_STARTING_SIMULATION_STEP;
		}
		else if (en == "Bullet Simulation Step Complete") {
			return BT_SIMULATION_STEP_COMPLETE;
		}
		else {
			return 0;
		}
	}

	std::vector<std::pair<unsigned int, std::string>> enumList() {
		std::vector<std::pair<unsigned int, std::string>> lst;

		// Key Frame Events
		lst.push_back(std::pair<unsigned int, std::string>(INITIAL_KEYFRAME, "Keyframe Listeners/Initial Keyframe"));
		lst.push_back(std::pair<unsigned int, std::string>(STARTING_KEYFRAME_UPDATE, "Keyframe Listeners/Starting Keyframe Update"));
		lst.push_back(std::pair<unsigned int, std::string>(KEYFRAME_UPDATE_COMPLETE, "Keyframe Listeners/Keyframe Update Complete"));
		lst.push_back(std::pair<unsigned int, std::string>(FIRST_FRAME_BEFORE_KEYFRAME_UPDATE, "Keyframe Listeners/Initial Frame Before Keyframe Update"));
		lst.push_back(std::pair<unsigned int, std::string>(FIRST_FRAME_AFTER_KEYFRAME_UPDATE, "Keyframe Listeners/Initial Frame After Keyframe Update"));
		lst.push_back(std::pair<unsigned int, std::string>(LAST_FRAME_BEFORE_KEYFRAME_UPDATE, "Keyframe Listeners/Last Frame Before Keyframe Update"));
		lst.push_back(std::pair<unsigned int, std::string>(LAST_FRAME_AFTER_KEYFRAME_UPDATE, "Keyframe Listeners/Last Frame After Keyframe Update"));

		// Bullet Events
		lst.push_back(std::pair<unsigned int, std::string>(BT_STARTING_SIMULATION_STEP, "Bullet/Starting Bullet Simulation Step"));
		lst.push_back(std::pair<unsigned int, std::string>(BT_SIMULATION_STEP_COMPLETE, "Bullet/Bullet Simulation Step Complete"));

		return lst;

	}
}
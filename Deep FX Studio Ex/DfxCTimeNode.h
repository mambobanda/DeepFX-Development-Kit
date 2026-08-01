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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <FlSpace.h>
#include <FlInterpreterScope.h>
#include <DfxSpace.h>
#include <FlModel.h>
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"

namespace compute_node {

	static const std::string time_model_id = "/COMPUTE/TIME_NODE";

	class TimeNodeHandler : public fl::Model {
	public:

		/// Time Node
		static inline void timeMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {

			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			error->hasError = false;

			auto _startFrame = DfxSpace::getInstance()->findAbsolute("~/TIME/frame_start")->data<int>();
			auto _endFrame = DfxSpace::getInstance()->findAbsolute("~/TIME/frame_end")->data<int>();
			auto _currentFrame = DfxSpace::getInstance()->findAbsolute("~/TIME/current_frame")->data<int>();
			auto _fps = DfxSpace::getInstance()->findAbsolute("~/TIME/fps")->data<int>();
			float _time = 0.f;

			if (_fps > 0)
				_time = (float)_currentFrame / (float)_fps;
			else
				_time = 0.f;

			auto startFrame = new fl::Any();
			startFrame->newData<float>(fl::type::FLOAT);
			startFrame->setData<float>((float)_startFrame);

			auto endFrame = new fl::Any();
			endFrame->newData<float>(fl::type::FLOAT);
			endFrame->setData<float>((float)_endFrame);

			auto currentFrame = new fl::Any();
			currentFrame->newData<float>(fl::type::FLOAT);
			currentFrame->setData<float>((float)_currentFrame);

			auto fps = new fl::Any();
			fps->newData<float>(fl::type::FLOAT);
			fps->setData<float>((float)_fps);

			auto time = new fl::Any();
			time->newData<float>(fl::type::FLOAT);
			time->setData<float>((float)_time);

			returnValues.push_back(currentFrame);
			returnValues.push_back(time);
			returnValues.push_back(fps);
			returnValues.push_back(startFrame);
			returnValues.push_back(endFrame);
		}

		static inline fl::Any *timeNode(fl::SpaceInterface *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("time")
				.setLabel("Time")
				.setMode(fl::DataObject::NODE)
				.setHasModel(true)
				.setModelReference(time_model_id)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "frame", "Frame", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "time", "Time(sec)", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "fps", "FPS", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "first_frame", "Initial Frame", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::OUTPUT, "last_frame", "Last Frame", 0.f)

				.addStaticFunction(fl::DataObject::STATIC_MAIN, timeMethod)
				.build(space);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return timeNode(space);
		}
	};

	inline void registerTimeNode() {

		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = time_model_id;
		desc->name = "Time";
		desc->description = "Different time values";
		desc->category = "Time";
		desc->placement = "Time/Time";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = TimeNodeHandler::timeNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}
}
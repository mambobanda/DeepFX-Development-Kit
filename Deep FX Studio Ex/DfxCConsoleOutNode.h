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
#include <FlModel.h>
#include <FlInterpreterScope.h>
#include <DfxGlobal.h>
#include "DfxComputeNodeDB.h"
#include "DfxComputeNodeDescription.h"

namespace compute_node {

	static const std::string consoleOut_model_id = "/COMPUTE/CONSOLE_OUT_NODE";

	class ConsoleOutNodeHandler : public fl::Model {
	public:

		/// Scalar Node
		static inline void consoleOutMethod(fl::SpaceInterface *space, fl::DataObject *dao, std::vector<fl::Any*> &args, std::vector<fl::Any*> &returnValues) {
			auto _anyError = new fl::Any();
			auto error = new fl::InterpreterError;
			_anyError->newData<fl::InterpreterError>(fl::type::INTERPRETER_ERROR, error);
			returnValues.push_back(_anyError);

			if (!args.empty()) {

				if (args[0]->type() != fl::type::STD_STRING) {
					error->hasError = true;
					error->errorMsg = "Unknown Input Type";

					return;
				}
				else if (args[0]->isArray()) {
					error->hasError = true;
					error->errorMsg = "Input 'a' does not accept arrays.";

					return;
				}

				std::string str = args[0]->data<std::string>();

				DfxGlobal::console::println(str);

				error->hasError = false;
			}
		}

		static inline fl::Any *consoleOutNode(fl::SpaceInterface *space) {

			auto mnode = fl::DataObject::Builder()
				.setName("console_out")
				.setLabel("Console Out")
				.setHasModel(true)
				.setModelReference(consoleOut_model_id)
				.setMode(fl::DataObject::NODE)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "xpos", "X-Position", 0.f)
				.addProperty(space, fl::type::FLOAT, fl::role::NONE, "ypos", "Y-Position", 0.f)
				.addProperty(space, fl::type::STD_STRING, fl::role::INPUT, "string", "String", std::string("< text out >"))
				.addStaticFunction(fl::DataObject::STATIC_MAIN, consoleOutMethod)
				.build(space);

			return mnode;
		}

		fl::Any *createModel(fl::Space *space) override {
			return consoleOutNode(space);
		}
	};

	inline void registerConsoleOutNode() {
		DfxComputeNodeDescription *desc = new DfxComputeNodeDescription;
		desc->identifier = "DFX_CONSOLE_OUT";
		desc->name = "Console Out";
		desc->description = "Print text to output.";
		desc->category  = "I/O";
		desc->placement = "Input Output/Console Out";
		desc->major = 1;
		desc->minor = 0;
		desc->instance = ConsoleOutNodeHandler::consoleOutNode;

		DfxComputeNodeDB::getInstance()->registerComputeNode(desc);
	}
}
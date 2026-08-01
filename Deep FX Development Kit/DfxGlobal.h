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

#include <QString>
#include <QStringList>
#include "DfxCommandDatabase.h"
#include "DfxAbstractCommand.h"
#include "DfxCmdArguments.h"
#include "DfxString.h"
#include "DfxEntityUtil.h"
#include "DfxConsoleBuffer.h"

namespace DfxGlobal {

	namespace console {

		void println(DfxString);
		void println(std::string);
		void println(const char *);
		void python_println(const char *);

		void print(DfxString);
		void print(std::string);
		void print(const char *);
		void python_print(const char *);
	}

	namespace info {

		void message(DfxString);
		void message(std::string);
		void message(const char *);

		int messageBox(std::string text, std::string infor_text, int width, std::string cltext);
		int messageBox(const char *text, const char *infor_text, int width, const char *cltext);
		bool python_messageBox(const char *text, const char *infor_text, int width, const char *cltext);
	}

	namespace input {

		float getFloatValue(const char *title, const char *label, float default_value);
		const char *getStringValue(const char *title, const char *label, const char *default_value);
	}

	namespace command {

		void exec(DfxString);
		void exec(std::string);
		void exec(const char *);
		void python_exec(const char *);
	}
}
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

#include <vector>
#include <sstream>
#include <fstream>
#include <vector>
#include <regex>
#include <stack>
#include <experimental\filesystem>
#include <DfxGlobal.h>

namespace filesystem = std::experimental::filesystem;

namespace dfxsceneformat {

	enum {SCALAR_VALUE=0xff, VECTOR_VALUE};

	enum class STATE{ _ERROR, NONE, 
				      OBJECT_DECLARATION, OBJECT_DEFINITION, OBJECT_BEGIN, OBJECT_END,
					  VALUE_DECLARATION, VALUE_DEFINITION, VALUE_ARRAY_BEGIN, VALUE, VALUE_ARRAY_END, VALUE_BEGIN, VALUE_END,
					  ARRAY_SIZE, ARRAY_DECLARATION_BEGIN, ARRAY_DECLARATION_END};

	enum class _ERROR_ { SUCCESS, IS_DIRECTORY, FILE_DOES_NOT_EXIST };

	static char characterTokens[] = { ':', ';', '{', '}', ',', '[', ']' };
	static char ghostTokens[] = { ' ', '\t', '\n' };

	namespace regexstr {
		static std::string object       = "[a-zA-Z][_0-9a-zA-Z]*";
		static std::string objectName   = "[#][/_a-zA-Z0-9]*";
		static std::string objectBegin  = "[{]";
		static std::string objectEnd    = "[}]";
		static std::string valueType    = "[$][_a-zA-Z]*";
		static std::string valueName    = "[_a-zA-Z][_0-9a-zA-Z]*";
		static std::string valueBegin   = "[:]";
		static std::string valueEnd     = "[;]";
		static std::string arrayDeclarationBegin = "[\\[]";
		static std::string arrayDeclarationEnd   = "[\\]]";
		static std::string integer      = "[1-9][0-9]*";
		static std::string arrayBegin   = "[{]";
		static std::string arrayEnd     = "[}]";
		static std::string arraySeperator = "[,]";
		static std::string value        = "[_#\"a-zA-Z0-9/~-][_~/\\:a-zA-Z0-9\\.\"-]*";
	}

	class DfxSceneFormatStateListener{
	public:
		virtual void setFStream(std::ifstream *) = 0;
		virtual void newObject(std::string objectTypename, std::string objectName) = 0;
		virtual void newValue(std::string objectTypename, std::string objectName, size_t type, size_t size) = 0;
		virtual void setValue(std::string val) = 0;
		virtual void endValue() = 0;
		virtual void endArray() = 0;
		virtual void endObject() = 0;
		virtual void error(STATE st, std::string tok) = 0;
	};

	class DfxSceneFormatReader {
	private:
		char BUFFER[8196];
		filesystem::path filepath;
		std::stringstream ss;
		std::stack<std::string> stack;
		STATE errorState;
		std::string errorToken;
		STATE state;
		size_t depth;
		bool array;
		size_t arraySize;
	private:
		bool isCharacterToken(char val);
		bool isGhostToken(char val);
		void parseFStream(std::ifstream &is);
		void clearStream();
		void errorStateFunc(STATE st, std::string tok);

		void step(std::string token);
	private:
		DfxSceneFormatStateListener *listener;
	public:
		DfxSceneFormatReader(std::string path);
		_ERROR_ read();

		void setStateListener(DfxSceneFormatStateListener *lst);
	};
}

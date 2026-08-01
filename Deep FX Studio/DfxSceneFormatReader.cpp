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

#include "DfxSceneFormatReader.h"


namespace dfxsceneformat {

	DfxSceneFormatReader::DfxSceneFormatReader(std::string pth) {
		filepath = filesystem::path(pth);
		state = STATE::NONE;
		errorState = STATE::NONE;
		errorToken = "N/A";
		listener = nullptr;
		depth = 0;
		array = false;
		arraySize = 0;
	}

	bool DfxSceneFormatReader::isCharacterToken(char val) {

		for (int i = 0; i < sizeof(characterTokens); i++) {
			if (val == characterTokens[i]) return true;
		}

		return false;
	}

	bool DfxSceneFormatReader::isGhostToken(char val) {

		for (int i = 0; i < sizeof(ghostTokens); i++) {
			if (val == ghostTokens[i]) return true;
		}

		return false;
	}

	void DfxSceneFormatReader::clearStream() {
		ss.str("");
	}

	void DfxSceneFormatReader::errorStateFunc(STATE st, std::string tok) {
		state = STATE::_ERROR;
		errorToken = tok;
		errorState = st;
		if (listener != nullptr) listener->error(st, tok);
	}

	void DfxSceneFormatReader::parseFStream(std::ifstream &is) {

		is.read(BUFFER, sizeof(BUFFER));
		while (is.gcount() > 0)
		{
			for (int i = 0; i < is.gcount(); i++) {

				if (!isGhostToken(BUFFER[i])) {

					if (isCharacterToken(BUFFER[i])) {

						if (ss.str().size() > 0) {
							step(ss.str());
							clearStream();
						}

						ss << BUFFER[i];
						step(ss.str());
						clearStream();
					}
					else {
						ss << BUFFER[i];
					}
				}
				else {
					if (ss.str().length() > 0) {
						step(ss.str());
						clearStream();
					}
				}
			}

			is.read(BUFFER, sizeof(BUFFER));
		}

	}

	_ERROR_ DfxSceneFormatReader::read() {

		if (!filepath.has_extension()) {

			if (filesystem::is_directory(filepath)) {
				return _ERROR_::IS_DIRECTORY;
			}
		}

		if (filesystem::exists(filepath)) {

			std::ifstream is;
			is.open(filepath.string());

			listener->setFStream(&is);
			parseFStream(is);

			is.close();
		}
		else {
			return _ERROR_::FILE_DOES_NOT_EXIST;
		}
		
		return _ERROR_::SUCCESS;
	}

	void DfxSceneFormatReader::setStateListener(DfxSceneFormatStateListener *lst) {
		listener = lst;
	}

	void DfxSceneFormatReader::step(std::string token) {

		switch (state)
		{
		case STATE::_ERROR:
		{

		}
		break;
		case STATE::NONE:
		{
			std::regex r(regexstr::object);
			if (std::regex_match(token, r)) {
				state = STATE::OBJECT_DECLARATION;
				stack.push(token);
			}
			else {
				errorStateFunc(state, token);
			}
		}
			break;
		case STATE::OBJECT_DECLARATION:
		{
			std::regex r(regexstr::objectName);
			if (std::regex_match(token, r)) {
				state = STATE::OBJECT_DEFINITION;
				stack.push(token);
				
			}
			else {
				errorStateFunc(state, token);
			}
		}
			break;
		case STATE::OBJECT_DEFINITION:
		{
			std::regex r(regexstr::objectBegin);
			if (std::regex_match(token, r)) {
				state = STATE::OBJECT_BEGIN;

				depth++;
	
				std::string objectName = stack.top();
				stack.pop();
				std::string objectTypeName = stack.top();

				if (listener != nullptr) listener->newObject(objectTypeName, objectName);
			}
			else {
				errorStateFunc(state, token);
			}
		}
			break;
		case STATE::OBJECT_BEGIN:
		{
			std::regex r1(regexstr::object);
			std::regex r2(regexstr::valueType);
			std::regex r3(regexstr::objectEnd);

			if (std::regex_match(token, r1)) {
				state = STATE::OBJECT_DECLARATION;
				stack.push(token);
			}
			else if (std::regex_match(token, r2)) {
				state = STATE::VALUE_DECLARATION;
				stack.push(token);
			}
			else if (std::regex_match(token, r3)) {
				depth--;
				
				if (depth == 0) {
					state = STATE::NONE;
				}
				else {
					state = STATE::OBJECT_BEGIN;
				}

				if (listener != nullptr) listener->endObject();
			}
			else {
				errorStateFunc(state, token);
			}
		}
			break;
		case STATE::VALUE_DECLARATION:
		{
			std::regex r1(regexstr::valueName);
			std::regex r2(regexstr::arrayDeclarationBegin);

			if (std::regex_match(token, r1)) {
				state = STATE::VALUE_DEFINITION;
				stack.push(token);
			}
			else if (std::regex_match(token, r2)) {
				state = STATE::ARRAY_DECLARATION_BEGIN;
				array = true;
			}
			else {
				errorStateFunc(state, token);
			}
		}
			break;
		case STATE::ARRAY_DECLARATION_BEGIN:
		{
			std::regex r1(regexstr::integer);

			if (std::regex_match(token, r1)) {
				state = STATE::ARRAY_SIZE;
				arraySize = std::stoi(token.c_str());
			}
			else {
				errorStateFunc(state, token);
			}
		}
			break;
		case STATE::ARRAY_SIZE:
		{
			std::regex r1(regexstr::arrayDeclarationEnd);

			if (std::regex_match(token, r1)) {
				state = STATE::VALUE_DECLARATION;
			}
			else {
				errorStateFunc(state, token);
			}
		}
		break;
		case STATE::VALUE_DEFINITION:
		{
			std::regex r(regexstr::valueBegin);
			if (std::regex_match(token, r)) {
				state = STATE::VALUE_BEGIN;

				std::string valueName = stack.top();
				stack.pop();
				std::string valueTypeName = stack.top();

				if (array) {
					if (listener != nullptr) listener->newValue(valueTypeName, valueName, VECTOR_VALUE, arraySize);
				}
				else {
					if (listener != nullptr) listener->newValue(valueTypeName, valueName, SCALAR_VALUE, 0);
				}
			}
			else {
				errorStateFunc(state, token);
			}
		}
			break;
		case STATE::VALUE_BEGIN:
		{
			std::regex r1(regexstr::value);
			std::regex r2(regexstr::arrayBegin);
			std::regex r3(regexstr::arrayEnd);
			std::regex r4(regexstr::arraySeperator);

			if (array) {
				if (std::regex_match(token, r2)) {
					state = STATE::VALUE_BEGIN;
				}
				else if (std::regex_match(token, r3)) {
					array = false;
					arraySize = 0;
					state = STATE::OBJECT_BEGIN;

					if (listener != nullptr) listener->endArray();
				}
				else if(std::regex_match(token, r4)){
					state = STATE::VALUE_BEGIN;
				}
				else if(std::regex_match(token, r1))
				{
					if (listener != nullptr) listener->setValue(token);
				}
				else {
					errorStateFunc(state, token);
				}
			}
			else if(std::regex_match(token, r1)) {
				state = STATE::VALUE;

				if (listener != nullptr) listener->setValue(token);
			}
			else
			{
				state = STATE::VALUE;
				if (listener != nullptr) listener->setValue("\'?\'");
			}
		}
			break;
		case STATE::VALUE:
		{
			std::regex r(regexstr::valueEnd);
			if (std::regex_match(token, r)) {
				state = STATE::OBJECT_BEGIN;

				if (listener != nullptr) listener->endValue();
			}
			else
			{
				errorStateFunc(state, token);
			}
		}
			break;
		case STATE::VALUE_END:
		{

		}
			break;
		default:
			break;
		}
	}
}


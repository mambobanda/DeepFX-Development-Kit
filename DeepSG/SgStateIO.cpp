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

#include "SgStateIO.h"

namespace deepsg {

	SgStateIO::SgStateIO() {
		sbuffer = new rapidjson::StringBuffer;
		writer  = new rapidjson::PrettyWriter<rapidjson::StringBuffer>(*sbuffer);

		document = nullptr;
		current_value = nullptr;
	}

	SgStateIO::~SgStateIO() {
		delete writer;
		delete sbuffer;
	}

	bool SgStateIO::isDocumentValid() {
		if (document == nullptr) return false;
		else return true;
	}

	void SgStateIO::saveBufferToFile(std::string) {

	}

	void SgStateIO::loadFileToDOM(std::string fstr) {

		std::ifstream file(fstr);

		std::string line = "";
		std::stringstream ss;

		if (file.is_open()) {
			while (std::getline(file, line))
			{
				ss << line << '\n';
			}
			file.close();
		}

		if (document != nullptr) {
			delete document;
			document = nullptr;
		}

		std::string content = ss.str();
		if (!content.empty()) {
			document = new rapidjson::Document;
			if (document->Parse(ss.str().c_str()).HasParseError()) {
				delete document;
				document = nullptr;
			}
		}
	}

	void SgStateIO::addIndent(std::string key) {
		_indentMap[key] = true;
	}

	rapidjson::Value::Object SgStateIO::rootObject() {
		return document->GetObject();
	}

	void SgStateIO::beginSaveBlock(std::string key) {
		writer->String(key.c_str());
		writer->StartObject();
	}

	void SgStateIO::endSaveBlock() {
		writer->EndObject();
	}

	void SgStateIO::beginSave() {
		writer->StartObject();
	}

	void SgStateIO::endSave() {
		writer->EndObject();
	}

	void SgStateIO::saveInt(std::string key, int val) {
		writer->String(key.c_str());
		writer->Int(val);
	}

	void SgStateIO::saveBool(std::string key, bool val) {
		writer->String(key.c_str());
		writer->Bool(val);
	}

	void SgStateIO::saveUInt(std::string key, unsigned int val) {
		writer->String(key.c_str());
		writer->Uint(val);
	}

	void SgStateIO::saveDouble(std::string key, double val) {
		writer->String(key.c_str());
		writer->Double(val);
	}

	void SgStateIO::saveDouble3(std::string key, double val_a, double val_b, double val_c) {
		writer->String(key.c_str());
		writer->StartArray();
		writer->Double(val_a);
		writer->Double(val_b);
		writer->Double(val_c);
		writer->EndArray();
	}

	void SgStateIO::saveString(std::string key, std::string val) {
		writer->String(key.c_str());
		writer->String(val.c_str());
	}

	void SgStateIO::saveIntArray(std::string key, int *arr, size_t size) {
		writer->String(key.c_str());
		writer->StartArray();
		for (size_t i = 0; i < size; i++) writer->Int(arr[i]);
		writer->EndArray();
	}

	void SgStateIO::saveBoolArray(std::string key, bool *arr, size_t size) {
		writer->String(key.c_str());
		writer->StartArray();
		for (size_t i = 0; i < size; i++) writer->Bool(arr[i]);
		writer->EndArray();
	}

	void SgStateIO::saveUIntArray(std::string key, unsigned int *arr, size_t size) {
		writer->String(key.c_str());
		writer->StartArray();
		for (size_t i = 0; i < size; i++) writer->Uint(arr[i]);
		writer->EndArray();
	}

	void SgStateIO::saveDoubleArray(std::string key, double *arr, size_t size) {
		writer->String(key.c_str());
		writer->StartArray();
		for (size_t i = 0; i < size; i++) writer->Double(arr[i]);
		writer->EndArray();
	}

	void SgStateIO::saveStringArray(std::string key, std::string *arr, size_t size) {
		writer->String(key.c_str());
		writer->StartArray();
		for (size_t i = 0; i < size; i++) writer->String(arr[i].c_str());
		writer->EndArray();
	}

	int SgStateIO::loadInt(rapidjson::Value::Member &m) {
		return m.value.GetInt();
	}

	bool SgStateIO::loadBool(rapidjson::Value::Member &m) {
		return m.value.GetBool();
	}

	unsigned int SgStateIO::loadUInt(rapidjson::Value::Member &m) {
		return m.value.GetUint();
	}

	double SgStateIO::loadDouble(rapidjson::Value::Member &m) {
		return m.value.GetDouble();
	}

	std::array<double, 3> SgStateIO::loadDouble3(rapidjson::Value::Member &m) {
		auto array = m.value.GetArray();
		std::array<double, 3> outArray;

		outArray[0] = array[0].GetDouble();
		outArray[1] = array[1].GetDouble();
		outArray[2] = array[2].GetDouble();
	
		return outArray;
	}

	std::string SgStateIO::loadString(rapidjson::Value::Member &m) {
		return m.value.GetString();
	}

	std::vector<int> SgStateIO::loadIntArray(rapidjson::Value::Member &m) {
		auto array = m.value.GetArray();
		std::vector<int> outArray;

		for (unsigned int inx = 0; inx < array.Size(); inx++) {
			outArray.push_back(array[inx].GetInt());
		}

		return outArray;
	}

	std::vector<bool> SgStateIO::loadBoolArray(rapidjson::Value::Member &m) {
		auto array = m.value.GetArray();
		std::vector<bool> outArray;

		for (unsigned int inx = 0; inx < array.Size(); inx++) {
			outArray.push_back(array[inx].GetBool());
		}

		return outArray;
	}

	std::vector<unsigned int> SgStateIO::loadUIntArray(rapidjson::Value::Member &m) {
		auto array = m.value.GetArray();
		std::vector<unsigned int> outArray;

		for (unsigned int inx = 0; inx < array.Size(); inx++) {
			outArray.push_back(array[inx].GetUint());
		}

		return outArray;
	}

	std::vector<double> SgStateIO::loadDoubleArray(rapidjson::Value::Member &m) {
		auto array = m.value.GetArray();
		std::vector<double> outArray;

		for (unsigned int inx = 0; inx < array.Size(); inx++) {
			outArray.push_back(array[inx].GetDouble());
		}

		return outArray;
	}

	std::vector<std::string> SgStateIO::loadStringArray(rapidjson::Value::Member &m) {
		auto array = m.value.GetArray();
		std::vector<std::string> outArray;

		for (unsigned int inx = 0; inx < array.Size(); inx++) {
			outArray.push_back(array[inx].GetString());
		}

		return outArray;
	}

	std::string SgStateIO::bufferToString() {
		return sbuffer->GetString();
	}

	std::string SgStateIO::DOMToString() {
		rapidjson::StringBuffer sb;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
		document->Accept(writer);
		
		return sb.GetString();
	}

	SgStateIO::BlockReader::BlockReader() {
	}

	SgStateIO::BlockReader::~BlockReader() {

	}

	void SgStateIO::BlockReader::addBlock(std::string indent, BlockCallback cc) {
		_callbacks[indent] = cc;
	}

	void SgStateIO::BlockReader::readObject(rapidjson::Value::Object obj) {
		for (auto& m : obj) {
			auto itr = _callbacks.find(m.name.GetString());

			if (itr != _callbacks.end()) {
				itr->second(m);
			}	
		}
	}

}
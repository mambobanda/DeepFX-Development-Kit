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

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <functional>
#include <unordered_map>
#include <vector>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

class DeepRisingStateIO;


namespace deepsg {

	using BlockCallback = std::function<void(rapidjson::Value::Member &)>;

	class SgStateIO {
	protected:
		class BlockReader {
		private:
			std::unordered_map<std::string, BlockCallback> _callbacks;

		public: 
			BlockReader();
			virtual ~BlockReader();

			void addBlock(std::string indent, BlockCallback);

			void readObject(rapidjson::Value::Object obj);
		};
	private:
		std::unordered_map<std::string, bool> _indentMap;

		rapidjson::StringBuffer *sbuffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> *writer;

		rapidjson::Document *document;
		rapidjson::Value *current_value;
	private:

	public:

		SgStateIO();
		virtual ~SgStateIO();

		void addIndent(std::string);

		void beginSave();
		void endSave();

		void beginSaveBlock(std::string);
		void endSaveBlock();

		rapidjson::Value::Object rootObject();

		bool isDocumentValid();

		void saveInt(std::string, int);
		void saveBool(std::string, bool);
		void saveUInt(std::string, unsigned int);
		void saveDouble(std::string, double);
		void saveDouble3(std::string, double, double, double);
		void saveString(std::string, std::string);

		void saveIntArray(std::string, int *, size_t size);
		void saveBoolArray(std::string, bool*, size_t size);
		void saveUIntArray(std::string, unsigned int *, size_t size);
		void saveDoubleArray(std::string, double *, size_t size);
		void saveStringArray(std::string, std::string *, size_t size);

		static int loadInt(rapidjson::Value::Member &m);
		static bool loadBool(rapidjson::Value::Member &m);
		static unsigned int loadUInt(rapidjson::Value::Member &m);
		static double loadDouble(rapidjson::Value::Member &m);
		static std::array<double, 3> loadDouble3(rapidjson::Value::Member &m);
		static std::string loadString(rapidjson::Value::Member &m);

		static std::vector<int> loadIntArray(rapidjson::Value::Member &m);
		static std::vector<bool> loadBoolArray(rapidjson::Value::Member &m);
		static std::vector<unsigned int> loadUIntArray(rapidjson::Value::Member &m);
		static std::vector<double> loadDoubleArray(rapidjson::Value::Member &m);
		static std::vector<std::string> loadStringArray(rapidjson::Value::Member &m);

		void saveBufferToFile(std::string);
		void loadFileToDOM(std::string);

		virtual void save(std::string) = 0;
		virtual void load(std::string) = 0;

		std::string bufferToString();
		std::string DOMToString();
	};

}
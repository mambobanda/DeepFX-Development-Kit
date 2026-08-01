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

#include <algorithm>
#include <string>
#include <unordered_map>


class DeepStudioRegistry {
public:

	class Entry {
	private:

	public:
		int Int;
		float Float;
		std::string String;
		bool Bool;

		std::unordered_map<std::string, Entry> entry;

		Entry();

		void copyValues(Entry &other);
	};

private:

	static Entry root;

	static std::vector<std::string> splitStr(std::string str);
public:

	DeepStudioRegistry();

	static void insertEntry(std::string path, Entry ent);

	static bool findEntry(std::string path, Entry &out);
};
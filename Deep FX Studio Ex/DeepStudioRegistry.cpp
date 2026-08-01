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

#include "DeepStudioRegistry.h"

DeepStudioRegistry::Entry DeepStudioRegistry::root;

DeepStudioRegistry::Entry::Entry() { Int = 0;  Float = 0; String = "<Empty>"; Bool = false; }

void DeepStudioRegistry::Entry::copyValues(DeepStudioRegistry::Entry &other) {
	Int = other.Int;
	Float = other.Float;
	String = other.String;
	Bool = other.Bool;
}

std::vector<std::string> DeepStudioRegistry::splitStr(std::string str) {
	std::vector<std::string> lst;

	std::string stand;
	for (int i = 0; i < str.length(); i++) {

		if ((str[i] == '/' || i == str.length() - 1) && stand.length() > 0) {

			if (i == str.length() - 1) stand += str[i];

			lst.push_back(stand);
			stand = "";
		}
		else {
			if (str[i] != '/') {
				stand += str[i];
			}
		}
	}

	return lst;
}

void DeepStudioRegistry::insertEntry(std::string path, Entry ent) {

	std::transform(path.begin(), path.end(), path.begin(), ::tolower);

	auto lst = splitStr(path);

	Entry *ref = &root;

	for (int i = 0; i < lst.size(); i++) {
		auto it = ref->entry.find(lst[i]);

		if (it == ref->entry.end()) {

			if (i == lst.size() - 1) {
				ref->entry[lst[i]] = ent;
			}
			else {
				Entry newEntry;
				ref->entry[lst[i]] = newEntry;

				ref = &ref->entry[lst[i]];
			}
		}
		else {

			if (i == lst.size() - 1) {
				ref->entry[lst[i]].copyValues(ent);
			}
			else
			{
				ref = &ref->entry[lst[i]];
			}
		}
	}
}

bool DeepStudioRegistry::findEntry(std::string path, Entry &out) {

	std::transform(path.begin(), path.end(), path.begin(), ::tolower);

	auto lst = splitStr(path);

	Entry *ref = &root;

	for (int i = 0; i < lst.size(); i++) {
		auto it = ref->entry.find(lst[i]);

		if (it != ref->entry.end()) {
			if (i == lst.size() - 1) {
				out = it->second;
				return true;
			}
			else {
				ref = &ref->entry[lst[i]];
			}
		}
	}

	return false;
}
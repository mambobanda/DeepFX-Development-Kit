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

#include "DfxIOUtil.h"


namespace util {

	/// Save Text File
	bool writeTextFile(std::string contents, std::string path) {
		std::ofstream file(path);

		if (file.is_open()) {
			file << contents.c_str();
			file.close();

			return true;
		}
		
		return false;
	}

	/// Read Text File
	std::string readTextFile(std::string path) {
		std::string line;
		std::ifstream myfile(path);

		std::stringstream ss;
		if (myfile.is_open())
		{
			while (std::getline(myfile, line))
			{
				ss << line << '\n';
			}
			myfile.close();
		}

		return ss.str();
	}
}
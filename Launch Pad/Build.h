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

#include <fstream>
#include <QString>
#include <QDate>
#include <QProcess> 
#include <QProgressDialog>
#include <sstream>
#include "ResourceManager.h"

namespace filesystem = std::experimental::filesystem;

namespace build {

	void writeToFile(std::string path, std::string content);
	std::string moveFiles(std::string dir, Resource *rsc, std::string configDir = "", std::string metaDir = "", std::string versionStr = "");
	std::string build_repo();
	void build_executable();

}
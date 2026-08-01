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

#include "VCommon.h"

namespace xvault {

	class File {
	public:
		virtual ~File();
		virtual bool open(std::string path, unsigned int iomode) = 0;
		virtual bool isOpen() = 0;
		virtual bool close() = 0;
		virtual bool writeResource(std::string resource_path, oResource &) = 0;
		virtual bool readResource(std::string resource_path, iResource &) = 0;
		virtual void walkFile(WalkCallback callback, void *userdata) = 0;
		virtual std::string errorMsg() = 0;
	};

}
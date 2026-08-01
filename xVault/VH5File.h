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

#include <hdf5.h>
#include <H5Cpp.h>
#include <iostream>
#include <filesystem>

#include "VFile.h"

namespace filesystem = std::experimental::filesystem;

namespace xvault {

	namespace h5 {

		class h5File : public File {
		private:

			H5::H5File  *_file;
			H5::StrType *_attrStrType;
			std::string  _error;
		
			void walkFile(H5::Group &group, std::string path, WalkCallback callback, void *userdata);
		private:

			bool openGroup(std::string path, H5::Group &grp);

		public:

			h5File();
			virtual ~h5File();

			bool open(std::string path, unsigned int iomode) override;
			bool isOpen() override;
			bool close() override;
			bool writeResource(std::string resource_path, oResource &) override;
			bool readResource(std::string resource_path, iResource &) override;
			void walkFile(WalkCallback callback, void *userdata) override;
			std::string errorMsg() override;

		};

	}
}
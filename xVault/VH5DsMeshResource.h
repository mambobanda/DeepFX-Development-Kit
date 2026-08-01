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

#include <vector>
#include <DsMesh.h>
#include "VH5OResource.h"

namespace xvault {

	namespace h5 {

		class h5DsMeshResource : public h5OResource {
		private:
			std::string _name;
		public:

			h5DsMeshResource(std::string nme = ".geometry");
			virtual ~h5DsMeshResource();

			void setMesh(ds::Mesh *msh);
			void setResourceID(size_t uid);
			void setName(std::string name);

			ds::Mesh *toDsMesh();
			static ds::Mesh *toDsMesh(iResource *);
		};
	}

}
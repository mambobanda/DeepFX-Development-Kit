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
#include "VResource.h"

namespace xvault {

	namespace h5 {

		class h5IResource : public iResource {
		protected:
			std::vector<Attribute*> _attributes;
			std::vector<Buffer*> _buffers;
		public:

			h5IResource();
			virtual ~h5IResource();

			std::vector<Attribute*> &attributes();
			std::vector<Buffer*> &buffers();

			void addAttribute(Attribute*) override;
			void addBuffer(Buffer*) override;

			Attribute *findAttribute(std::string) override;
			Buffer *findBuffer(std::string) override;
		};

	}

}
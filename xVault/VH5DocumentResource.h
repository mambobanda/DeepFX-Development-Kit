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
#include "VH5OResource.h"


namespace xvault {

	namespace h5 {
	
		class h5ODocumentResource : public h5OResource {
		private:

			Attribute *_contentAttribute;
			Buffer *_buffer;
			std::string _name;
		public:

			h5ODocumentResource(std::string nme = "document");
			virtual ~h5ODocumentResource();

			int attributeCount() override;
			Attribute *attribute(size_t) override;

			int bufferCount() override;
			Buffer *buffer(size_t) override;

			void setText(std::string str);

		};
	}

}
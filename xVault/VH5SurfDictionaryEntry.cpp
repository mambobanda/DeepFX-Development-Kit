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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "VH5SurfDictionaryEntry.h"
#include "VH5Attribute.h"
#include "VH5Buffer.h"

namespace xvault {

	namespace h5 {

		VH5SurfDictionaryEntry::VH5SurfDictionaryEntry() {

			auto _contentAttribute = new h5Attribute;
			_contentAttribute->setName("__content__");
			_contentAttribute->setType(type::STRING);
			_contentAttribute->setValueString("surf_dict_ent");

			addAttribute(_contentAttribute);

			counter = 0;
		}

		VH5SurfDictionaryEntry::~VH5SurfDictionaryEntry() {

		}

		void VH5SurfDictionaryEntry::setPath(std::string path) {

			auto nameAttribute = new h5Attribute;
			nameAttribute->setName("__path__");
			nameAttribute->setType(type::STRING);
			nameAttribute->setValueString(path);

			addAttribute(nameAttribute);
		}

		void VH5SurfDictionaryEntry::addEntries(std::vector<long long unsigned int> uid, std::vector<std::string> name) {

			if (uid.size() == name.size()) {

				auto vbuffer = new h5Buffer();
				vbuffer->setName("uids");
				vbuffer->setType(type::UNSIGNED_LONG_LONG);
				vbuffer->setBufferSize(uid.size());

			}
		}
	}
}
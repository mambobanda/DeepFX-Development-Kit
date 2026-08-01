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

#include <iostream>
#include <string>
#include <filesystem>

#include "VAttribute.h"
#include "VH5File.h"
#include "VH5Attribute.h"
#include "VH5IResource.h"
#include "VH5DocumentResource.h"
#include "VH5Buffer.h"

#include "RJDocument.h"

int main(void) {
	
	xvault::rapid::rjDocument xdoc;

	int _ibuff[] = { 1,2,3,4,5,6,7,8,9,10 };
	auto ibuffer = new xvault::h5::h5Buffer(false);
	ibuffer->setName("numbers");
	ibuffer->setType(xvault::type::INT);
	ibuffer->setBufferSize(10);
	ibuffer->setBufferInt(_ibuff);

	float _fbuff[] = { 0.1f, 2.3f, 33.3f, 0.14f, 5.f,6.f,7.f, 1.8f, 9.f, 10.f };
	auto fbuffer = new xvault::h5::h5Buffer(false);
	fbuffer->setName("decimals");
	fbuffer->setType(xvault::type::FLOAT);
	fbuffer->setBufferSize(10);
	fbuffer->setBufferFloat(_fbuff);

	auto name = new xvault::h5::h5Attribute;
	name->setName("name");
	name->setType(xvault::type::STRING);
	name->setValueString("Mambo Banda");

	auto age = new xvault::h5::h5Attribute;
	age->setName("age");
	age->setType(xvault::type::INT);
	age->setValueInt(40);

	auto gender = new xvault::h5::h5Attribute;
	gender->setName("sex");
	gender->setType(xvault::type::STRING);
	gender->setValueString("Male");

	auto height = new xvault::h5::h5Attribute;
	height->setName("height");
	height->setType(xvault::type::FLOAT);
	height->setValueFloat(1.87f);

	auto location = new xvault::rapid::rjNode("Location");

	double _dbuff[] = { 0.1, 2.3, 33.3, 0.14, 5.0, 6.0, 7.0, 1.8, 9.0, 10.0 };
	auto dbuffer = new xvault::h5::h5Buffer(false);
	dbuffer->setName("area codes");
	dbuffer->setType(xvault::type::DOUBLE);
	dbuffer->setBufferSize(10);
	dbuffer->setBufferDouble(_dbuff);

	auto street = new xvault::h5::h5Attribute;
	street->setName("street");
	street->setType(xvault::type::STRING);
	street->setValueString("Kwacha Road");

	auto country = new xvault::h5::h5Attribute;
	country->setName("country");
	country->setType(xvault::type::STRING);
	country->setValueString("Zambia");

	auto pobox = new xvault::h5::h5Attribute;
	pobox->setName("PO Box");
	pobox->setType(xvault::type::INT);
	pobox->setValueInt(12467);

	location->addBuffer(dbuffer);
	location->addAttribute(pobox);
	location->addAttribute(street);
	location->addAttribute(country);

	xdoc.addBuffer(ibuffer);
	xdoc.addBuffer(fbuffer);
	xdoc.addAttribute(name);
	xdoc.addAttribute(age);
	xdoc.addAttribute(gender);
	xdoc.addAttribute(height);
	xdoc.addNode(location);

	//xdoc.addBuffer(buffer);

	std::cout << xdoc.toString() << std::endl;
	/*std::unique_ptr<xvault::File> ofile(new xvault::h5::h5File);
	std::unique_ptr<xvault::File> ifile(new xvault::h5::h5File);

	xvault::h5::h5ODocumentResource rsc(".properties");
	xvault::h5::h5IResource generic_rsc;
	xvault::h5::h5OResource generic_rsc2;

	int int_array[] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 10 };
	auto *intBuffer = new xvault::h5::h5Buffer(false);
	intBuffer->setName("intArray");
	intBuffer->setType(xvault::type::INT);
	intBuffer->setBufferInt(int_array);
	intBuffer->setBufferSize(11);
	generic_rsc2.addBuffer(intBuffer);

	rsc.setText("Hello World!!!");

	filesystem::path path("/my_path/.ll");
	std::cout << "Path: " << path.stem() << std::endl;

	if (ofile->open("c:/test/data_format.h5", xvault::io::OPEN)) {
		ofile->writeResource("/Documents/Books/Magazines", rsc);
		ofile->writeResource("/Documents/Books/Magazines", generic_rsc2);
	}
	else {
		std::cout << "Failed to create file...\n";
		std::cout << ofile->errorMsg();
	}

	if (ifile->open("c:/test/data_format.h5", xvault::io::OPEN_READ_ONLY)) {

		ifile->readResource("/Documents/Books/Magazines", generic_rsc);

		for (auto attr : generic_rsc.attributes()) {
			std::cout << attr->name() << std::endl;

			if (attr->type() == xvault::type::STRING) std::cout << attr->valueString() << std::endl;
		}

		for (auto buff : generic_rsc.buffers()) {
			std::cout << buff->name() << std::endl;
			if (buff->type() == xvault::type::INT) {
				for (int i = 0; i < buff->bufferSize(); i++) {
					std::cout << "Val: " << buff->bufferInt()[i] <<std::endl;
				}
			}
		}

		ifile->walkFile([](std::string, xvault::iResource *, void *) {std::cout << "Found A Resource !\n"; }, nullptr);
	}
	else {
		std::cout << "Failed to open file...\n";
		std::cout << ifile->errorMsg();
	}
*/
	return 0;  // successfully terminated
}
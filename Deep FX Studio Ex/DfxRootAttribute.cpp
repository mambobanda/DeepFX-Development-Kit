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
 
#include "DfxRootAttribute.h"

DfxRootAttribute::DfxRootAttribute() {

}

DfxRootAttribute::~DfxRootAttribute() {

}

void DfxRootAttribute::setName(std::string nm) {
	_name = nm;
}

std::string DfxRootAttribute::name() {
	return _name;
}

unsigned int DfxRootAttribute::type() {
	return fl::type::NONE;
}

bool DfxRootAttribute::isReadable() {
	return false;
}

bool DfxRootAttribute::isWritable() {
	return false;
}

bool DfxRootAttribute::isArray() {
	return false;
}

fl::Any *DfxRootAttribute::getValue() {
	return nullptr;
}

//std::unique_ptr<fl::Any> DfxRootAttribute::getValueAt(size_t) {
//	std::unique_ptr<fl::Any>(new fl::Any);
//}

size_t DfxRootAttribute::getValueCount() {
	return 0;
}

void DfxRootAttribute::setValue(fl::Any *val) {
	// pass
}

//void DfxRootAttribute::setValueAt(size_t, std::unique_ptr<fl::Any> val) {
//	// pass
//}
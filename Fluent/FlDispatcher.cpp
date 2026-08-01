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

#include "FlAny.h"
#include "FlDataObject.h"
#include "FlDispatcher.h"
#include "FlDispatchManager.h"

namespace fl {

	size_t Dispatcher::ID_GEN = 0xff;

	Dispatcher::Dispatcher() {
		_uid = ID_GEN++;
		DispatchManager::getInstance()->addDispatcher(this);
	}

	Dispatcher::~Dispatcher() {
		DispatchManager::getInstance()->removeDispatcher(id());
	}

	size_t Dispatcher::id() {
		return _uid;
	}

	void Dispatcher::attachAny(Any *any) {
		any->addDispatcher(id());
	}

	void Dispatcher::attachBranch(Any *any, std::vector<std::string> exclude) {
		if(std::find(exclude.begin(), exclude.end(), any->name()) == exclude.end()) attachAny(any);

		if (any->type() == type::DATA_OBJECT) {
			auto dao = any->rawData<DataObject>();

			for (auto p : dao->properties()) {
				attachBranch(p, exclude);
			}
		}
	}
}
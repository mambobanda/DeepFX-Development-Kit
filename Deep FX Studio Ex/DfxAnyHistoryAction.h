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

#include <FlCommon.h>
#include <FlAny.h>
#include <DfxSpace.h>
#include "DfxHistoryAction.h"

class DfxAnyHistoryAction : public DfxHistoryAction {
private:
	fl::Any *_old;
	fl::Any *_new;
	std::string _referencePath;
public:

	DfxAnyHistoryAction();
	virtual ~DfxAnyHistoryAction();

	bool execute() override;
	DfxHistoryAction *inverse() override;

	template<typename T>
	void setValue(unsigned int type, std::string referencePath, T newVal, T oldVal) {
		_referencePath = referencePath;

		if (type == fl::type::FLOAT) {
			_old = new fl::Any;
			_old->newData<float>(fl::type::FLOAT);
			_old->setData<float>(oldVal);

			_new = new fl::Any;
			_new->newData<float>(fl::type::FLOAT);
			_new->setData<float>(newVal);
		}
	}
};
/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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
#include <DfxGridTypes.h>
#include "SgGridAdapterInterface.h"

class DfxOpenVDBGridAdapter: public deepsg::SgGridAdapterInterface {
private:
	unsigned int grid_type;
private:
	std::vector<deepsg::SgGridChannel*> _channels;

public:

	DfxOpenVDBGridAdapter();
	virtual ~DfxOpenVDBGridAdapter();

	void addChannel(deepsg::SgGridChannel *);
	void removeChannel(unsigned index);
	void removeChannel(std::string channelName);
	void clearAll();

	unsigned int gridImplementation() override;

	unsigned int channelCount() override;
	std::string channelName(unsigned int index) override;

	deepsg::SgGridChannel *channel(unsigned index) override;
	deepsg::SgGridChannel *channel(std::string channelName) override;

	void clear() override;

};
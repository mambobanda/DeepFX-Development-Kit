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

#pragma once

#include <functional>
#include "DfxTimeActionInfo.h"

class DfxTimeActionListener {
public:
	enum { NEW_FRAME_ACTION = 0xff,
		   NEW_FPS_ACTION,
		   NEW_FRAME_COUNT_ACTION};

	typedef std::function<void(DfxTimeActionInfo)> ActionCallback;
public:
	DfxTimeActionListener(ActionCallback);
	virtual void response(DfxTimeActionInfo info);

private:

	ActionCallback _response;
};
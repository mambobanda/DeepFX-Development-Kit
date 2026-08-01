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
#include "DfxSceneActionInfo.h"

class DfxSceneActionListener{
public:
	enum { ADD_ITEM_ACTION=0xff, ITEM_SELECTED_ACTION};

	typedef std::function<void(DfxSceneActionInfo)> ActionCallback;
public:
	DfxSceneActionListener(ActionCallback);
	virtual void response(DfxSceneActionInfo info);

private:

	ActionCallback _response;
};

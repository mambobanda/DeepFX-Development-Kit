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

#include "MainEventLoopInitializer.h"
#include "DfxMainEventStack.h"
#include "DfxKeyFrameOverrideMainEventUpdate.h"
#include "DfxNodeFilterMainEventUpdate.h"
#include "DfxVertexFilterMainEventUpdate.h"
#include "DfxToolsMainEventUpdate.h"

namespace eventloop {
	void initialize() {
		auto any = DfxSpace::getInstance()->findAbsolute("~/TIME/current_frame");
		any->addObserver(new DfxMainEventCurrentTimeObserever);

		DfxMainEventStack::getInstance()->addProcess(new DfxVertexFilterMainEventUpdate);
		DfxMainEventStack::getInstance()->addProcess(new DfxNodeFilterMainEventUpdate);
		DfxMainEventStack::getInstance()->addProcess(new DfxToolsMainEventUpdate);
		DfxMainEventStack::getInstance()->addProcess(new DfxKeyFrameOverrideMainEventUpdate);
	}
}
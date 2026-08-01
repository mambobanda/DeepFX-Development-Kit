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

#include "DfxViewportSynchronizer.h"
#include "DfxQSceneViewport.h"

std::vector<DfxQSceneViewport*> DfxViewportSynchronizer::_viewports;
std::atomic<int> DfxViewportSynchronizer::counter(0);

void DfxViewportSynchronizer::addViewport(DfxQSceneViewport *vp) {
	_viewports.push_back(vp);
}

void DfxViewportSynchronizer::removeViewport(DfxQSceneViewport *vp) {
	auto itr = _viewports.begin();

	while (itr != _viewports.end()) {
		if((*itr)->id() == vp->id()) {
			_viewports.erase(itr);
			if (!synchronized()) counter--;
			return;
		}

		itr++;
	}
}

void DfxViewportSynchronizer::resetCounter() {
	counter = 0;
}

void DfxViewportSynchronizer::prepCounter() {
	counter = _viewports.size();
}

void DfxViewportSynchronizer::decrementCounter() {
	counter--;
}

bool DfxViewportSynchronizer::synchronized() {
	return (counter == 0);
}
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

#include "DfxMainEventLoop.h"
#include "DfxMainEventStack.h"
#include "DfxOpenGLCacheQueue.h"

DfxMainEventLoop *DfxMainEventLoop::_instance = nullptr; 

DfxMainEventLoop::DfxMainEventLoop() {
	_initialKeyframeProcessed = false;
}

DfxMainEventLoop::~DfxMainEventLoop() {

}

DfxMainEventLoop *DfxMainEventLoop::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxMainEventLoop;
	}

	return _instance;
}

void DfxMainEventLoop::emitfinshedProcessingEventsSignal() {
	emit finshedProcessingEvents();
}

void DfxMainEventLoop::resetInitialState() {
	DfxFluentCleaner::cleanAll();
	_initialKeyframeProcessed = false;
}

void DfxMainEventLoop::processEvents(int frame, unsigned int type, std::vector<unsigned int> ignore_list) {
	
	auto _startFrame = DfxTimelineManager::getInstance()->firstFrame();
	auto _endFrame	 = DfxTimelineManager::getInstance()->lastFrame();
	auto _currentFrame = DfxTimelineManager::getInstance()->currentFrame();
	
	for (auto track: DfxCacheTrackManager::getInstance()->tracks()) {
		if (track->isActive() && !track->emptyPath()) {
			track->takeSnapshot(frame);
		}
	}

	for (auto simplex : DfxFluentSimplexCollection::getInstance()->simplexObjects()) {
		if (!simplex->cache()->isCached()) {
			DfxOpenGLCacheQueue::getInstance()->queueCache(simplex->cache());
		}
	}
	
	// Update Tracked Objects
	for (auto ref: DfxFluentTracker::getInstance()->sceneNodeTracker()) {
		auto node = util::fetchNode(ref.first);
		
		if (node.first != nullptr) {
			auto xform = deepsg::SgRootNode::getInstance()->getTransformation4x4(node.first);
			auto pos = node.first->position();

			glm::mat4 *xformAttr = nullptr;
			glm::vec3 *positions = nullptr;

			if (DfxFluentSimplexCollection::getInstance()->size() > 0) {
				xformAttr = DfxFluentSimplexCollection::getInstance()->atrributeMap().at<glm::mat4>("@xform");
			}

			if (DfxFluentStreamCollection::getInstance()->size() > 0) {
				positions = DfxFluentStreamCollection::getInstance()->atrributeMap().at<glm::vec3>("@position");
			}

			for (auto &trk : ref.second) {
		
				if (DfxFluentSimplexCollection::getInstance()->size() > 0) {

					if (trk.second == DfxFluentSet::SIMPLEX) {
						xformAttr[trk.first] = xform;
					}
					
				}

				if (DfxFluentStreamCollection::getInstance()->size() > 0) {
					if (trk.second == DfxFluentSet::STREAM) {
						positions[trk.first] = pos;
					}
				}
			}
		}
	}

	// Pre Initial and Last Frame Events
	if (type == KEY_FRAME_UPDATE) {

		if (!_initialKeyframeProcessed && _startFrame != frame) {
			DfxEventManager::getInstance()->announceEvent(event::INITIAL_KEYFRAME);
			_initialKeyframeProcessed = true;
		}

		if (_startFrame == frame) DfxEventManager::getInstance()->announceEvent(event::FIRST_FRAME_BEFORE_KEYFRAME_UPDATE);
		
		if (_endFrame == frame) DfxEventManager::getInstance()->announceEvent(event::LAST_FRAME_BEFORE_KEYFRAME_UPDATE);

		// Starting Keyframe update event
		DfxEventManager::getInstance()->announceEvent(event::STARTING_KEYFRAME_UPDATE);
	}

	DfxStageEvaluationInfo info;
	info.currentTime.fps = DfxTimelineManager::getInstance()->fps();
	info.currentTime.frame = _currentFrame;
	info.currentTime.seconds = (double)_currentFrame / (double)info.currentTime.fps;

	DfxMainEventStack::getInstance()->processUpdates(frame, type, ignore_list);

	if (type == KEY_FRAME_UPDATE) {
		// Keyframe Update Complete
		DfxEventManager::getInstance()->announceEvent(event::KEYFRAME_UPDATE_COMPLETE);

		// Post Initial and Last Frame Events
		if (_startFrame == frame) DfxEventManager::getInstance()->announceEvent(event::FIRST_FRAME_AFTER_KEYFRAME_UPDATE);
		if (_endFrame == frame) DfxEventManager::getInstance()->announceEvent(event::LAST_FRAME_AFTER_KEYFRAME_UPDATE);
	}
}
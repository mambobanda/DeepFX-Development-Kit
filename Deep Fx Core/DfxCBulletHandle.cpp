/*
* The Mailo Today Solutions Software License
*
* Copyright 2016 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#include "DfxCBulletHandle.h"

namespace dfxcore {

	namespace bullet {
		size_t DfxCBulletHandle::ID_GEN = 0;
		size_t DfxCBulletCollisionAction::ID_GEN = 0;

		DfxCBulletCollisionAction::DfxCBulletCollisionAction() {
			id = ++ID_GEN;
		}

		DfxCBulletCollisionAction::~DfxCBulletCollisionAction() {

		}

		size_t DfxCBulletCollisionAction::getID() {
			return id;
		}

		void DfxCBulletHandleState::reset() {
			STATIC = false;
			IS_SLEEPING = false;
			KINEMATIC = false;
		}

		DfxCBulletHandle::DfxCBulletHandle(size_t typ) {
			id = ++ID_GEN;
			type = typ;

			state = new DfxCBulletHandleState;
			state->reset();
		}

		DfxCBulletHandle::~DfxCBulletHandle() {
			delete state;
		}

		size_t DfxCBulletHandle::getID() {
			return id;
		}

		size_t DfxCBulletHandle::getType() {
			return type;
		}

		void DfxCBulletHandle::addCollisionListener(DfxCBulletCollisionAction *lst) {
			collisionListeners.push_back(lst);
		}

		void DfxCBulletHandle::removeCollisionListener(DfxCBulletCollisionAction *lst) {

			for (auto l : collisionListeners) {

				size_t counter = 0;
				if (l->getID() == lst->getID()) {
					delete l;
					collisionListeners.erase(collisionListeners.begin() + counter);
					break;
				}

				counter++;
			}
		}

		DfxCBulletHandleState *DfxCBulletHandle::getState() {
			return state;
		}
	}
}
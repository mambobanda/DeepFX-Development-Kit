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

#include "DfxCBulletRigidHandle.h"
#include "DfxCBulletHandleType.h"
#include "DfxCBulletCollisionActions.h"

namespace dfxcore {
	namespace bullet {

		void DfxCAwake_CollisionAction::response(btDiscreteDynamicsWorld *dynamicsWorld, DfxCBulletHandle *handle, DfxCBulletCollisionPacket &packet) {

			if (!packet.other->getState()->IS_SLEEPING) {

				if (handle->getState()->IS_SLEEPING) {

					if (handle->getType() == BULLET_RIGID_OBJECT) {
						
						auto rgd_h = (DfxCBulletRigidHandle*)handle;
		
						dynamicsWorld->removeRigidBody(rgd_h->getBody());
						rgd_h->getShape()->calculateLocalInertia(rgd_h->getMass(), btVector3(0, 0, 0));
						rgd_h->getBody()->setMassProps(rgd_h->getMass(), btVector3(0, 0, 0));
						dynamicsWorld->addRigidBody(rgd_h->getBody());

						handle->getState()->IS_SLEEPING = false;
					}
				}
			}
		}
		
	}
}
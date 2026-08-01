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

#pragma once

#include <vector>
#include <btBulletDynamicsCommon.h>
#include "DfxCBulletCollisionPacket.h"

namespace dfxcore {

	namespace bullet {

		class DfxCBulletHandle;
		class DfxCBulletHandleStates;
		class DfxCBulletCollisionAction;
		class DfxCBulletCollisionPacket;

		class DfxCBulletCollisionPacket {
		public:
			glm::vec3 collisionPointA;
			glm::vec3 collisionPointB;
			DfxCBulletHandle *other;

		};

		class DfxCBulletCollisionAction {
		private:
			static size_t ID_GEN;
			size_t id;
		public:
			DfxCBulletCollisionAction();
			virtual ~DfxCBulletCollisionAction();

			size_t getID();
			virtual void response(btDiscreteDynamicsWorld *dynamicsWorld, DfxCBulletHandle *, DfxCBulletCollisionPacket &) = 0;
		};

		class DfxCBulletHandleState{
		public:
			bool STATIC;
			bool KINEMATIC;
			bool IS_SLEEPING;

			void reset();
		};

		class DfxCBulletHandle {
		private:
			DfxCBulletHandleState *state;

			static size_t ID_GEN;
			size_t id;
			size_t type;
		
		protected:
			std::vector<DfxCBulletCollisionAction*> collisionListeners;
		public:

			DfxCBulletHandle(size_t);
			virtual ~DfxCBulletHandle();

			size_t getID();

			size_t getType();

			virtual void collisionResponse(btDiscreteDynamicsWorld *dynamicsWorld, DfxCBulletCollisionPacket &cpacket) = 0;

			void addCollisionListener(DfxCBulletCollisionAction *);

			void removeCollisionListener(DfxCBulletCollisionAction *);

			DfxCBulletHandleState *getState();
		};
	}
}
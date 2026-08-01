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


class DfxBulletActorNode;
class DfxBulletStageNode;
class DfxBulletConstraintPropNode;
class DfxBulletConstraintWidget;
class DfxBulletRigidBody;
class DfxBulletSoftBody;
class DfxBulletAgent;

namespace bullet {
	enum {
		MAIN_ACTOR = 0,

		CONSTRAINT_PROP = 1000, 
	};
}
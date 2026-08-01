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

#include <DfxGlobal.h>
#include "DfxQToolShelfWidget.h"

class DfxQToolShelfConstructor {
public:

	static void construct(DfxQToolShelf *shelf);

public:
	// Object Tools
	static void planeSlot();
	static void boxSlot();
	static void sphereSlot();
	static void torusSlot();

	// Rigid Body tools
	static void newRigidBody();
	static void rigidToStatic();
	static void rigidToDynamic();
	static void rigidGlueConstraint();

	//SPH Tools
	static void newSPHBody();
	static void newSPHFluid();
	static void newSPHEmitter();
	static void addSPHViscosity();
	static void addSPHVorticity();
	static void addSPHAdhesion();

	//Fracture Tools
	static void voronoiFracture();
	static void sliceFracture();
	static void cutoutFracture();
};
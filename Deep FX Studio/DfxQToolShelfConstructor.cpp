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

#include <QFile>
#include "DfxQToolShelfConstructor.h"

#undef slots
#include <DfxPythonGlobal.h>
#define slots

void DfxQToolShelfConstructor::construct(DfxQToolShelf *shelf) {

	DfxQToolShelf::getInstance()->newSelf("Objects");
	DfxQToolShelf::getInstance()->newSelf("Physics");
	DfxQToolShelf::getInstance()->newSelf("Fluids");
	// DfxQToolShelf::getInstance()->newSelf("Grains");
	DfxQToolShelf::getInstance()->newSelf("Fracture");
	// DfxQToolShelf::getInstance()->newSelf("Volumes");

	// Object Tool
	QAction* planePrimitiveAction = new QAction(QIcon(":/viewport_tb/plane"), "Plane");
	QAction* boxPrimitiveAction = new QAction(QIcon(":/viewport_tb/box"), "Box");
	QAction* spherePrimitiveAction = new QAction(QIcon(":/viewport_tb/sphere"), "Sphere");
	QAction* torusPrimitiveAction = new QAction(QIcon(":/viewport_tb/torus"), "Torus");

	DfxQToolShelf::getInstance()->addToolToShelf("Objects", planePrimitiveAction);
	DfxQToolShelf::getInstance()->addToolToShelf("Objects", boxPrimitiveAction);
	DfxQToolShelf::getInstance()->addToolToShelf("Objects", spherePrimitiveAction);
	DfxQToolShelf::getInstance()->addToolToShelf("Objects", torusPrimitiveAction);

	// Rigid Body Tools
	QAction* newRigidBodyAction = new QAction(QIcon(":/layers_tb/add_layer"), "Rigid Body");
	QAction* rigidBodyToStaticAction = new QAction(QIcon(":/layers_tb/add_layer"), "Static");
	QAction* rigidBodyToDynamicAction = new QAction(QIcon(":/layers_tb/add_layer"), "Dynamic");
	QAction* glueConstraintAction = new QAction(QIcon(":/layers_tb/add_layer"), "Glue");

	DfxQToolShelf::getInstance()->addToolToShelf("Physics", newRigidBodyAction);
	DfxQToolShelf::getInstance()->addToolToShelf("Physics", rigidBodyToStaticAction);
	DfxQToolShelf::getInstance()->addToolToShelf("Physics", rigidBodyToDynamicAction);
	DfxQToolShelf::getInstance()->addToolToShelf("Physics", glueConstraintAction);

	// SPH Tool
	QAction* newSPHBodyAction = new QAction(QIcon(":/layers_tb/add_layer"), "SPH Body");
	QAction* newSPHFluidAction = new QAction(QIcon(":/layers_tb/add_layer"), "SPH Fluid");
	QAction* newSPHEmitterAction = new QAction(QIcon(":/layers_tb/add_layer"), "SPH Emitter");
	QAction* addSPHViscosityAction = new QAction(QIcon(":/layers_tb/add_layer"), "SPH Viscosity");
	QAction* addSPHVorticityAction = new QAction(QIcon(":/layers_tb/add_layer"), "SPH Vorticity");
	QAction* addSPHAdhesionAction = new QAction(QIcon(":/layers_tb/add_layer"), "SPH Adhesion");

	DfxQToolShelf::getInstance()->addToolToShelf("Fluids", newSPHBodyAction);
	DfxQToolShelf::getInstance()->addToolToShelf("Fluids", newSPHFluidAction);
	DfxQToolShelf::getInstance()->addToolToShelf("Fluids", newSPHEmitterAction);
	DfxQToolShelf::getInstance()->addToolToShelf("Fluids", addSPHViscosityAction);
	DfxQToolShelf::getInstance()->addToolToShelf("Fluids", addSPHVorticityAction);
	DfxQToolShelf::getInstance()->addToolToShelf("Fluids", addSPHAdhesionAction);

	// Fracture Tools
	QAction* voronoiFractureAction = new QAction(QIcon(":/layers_tb/add_layer"), "Voronoi");
	QAction* sliceFractureAction   = new QAction(QIcon(":/layers_tb/add_layer"), "Slice");
	QAction* cutoutFractureAction  = new QAction(QIcon(":/layers_tb/add_layer"), "Cutout");

	DfxQToolShelf::getInstance()->addToolToShelf("Fracture", voronoiFractureAction);
	DfxQToolShelf::getInstance()->addToolToShelf("Fracture", sliceFractureAction);
	DfxQToolShelf::getInstance()->addToolToShelf("Fracture", cutoutFractureAction);

	// Connections
	QObject::connect(planePrimitiveAction, &QAction::triggered, &DfxQToolShelfConstructor::planeSlot);
	QObject::connect(boxPrimitiveAction, &QAction::triggered, &DfxQToolShelfConstructor::boxSlot);
	QObject::connect(spherePrimitiveAction, &QAction::triggered, &DfxQToolShelfConstructor::sphereSlot);
	QObject::connect(torusPrimitiveAction, &QAction::triggered, &DfxQToolShelfConstructor::torusSlot);

	QObject::connect(newRigidBodyAction, &QAction::triggered, &DfxQToolShelfConstructor::newRigidBody);
	QObject::connect(rigidBodyToStaticAction, &QAction::triggered, &DfxQToolShelfConstructor::rigidToStatic);
	QObject::connect(rigidBodyToDynamicAction, &QAction::triggered, &DfxQToolShelfConstructor::rigidToDynamic);
	QObject::connect(glueConstraintAction, &QAction::triggered, &DfxQToolShelfConstructor::rigidGlueConstraint);

	QObject::connect(newSPHBodyAction, &QAction::triggered, &DfxQToolShelfConstructor::newSPHBody);
	QObject::connect(newSPHFluidAction, &QAction::triggered, &DfxQToolShelfConstructor::newSPHFluid);
	QObject::connect(newSPHEmitterAction, &QAction::triggered, &DfxQToolShelfConstructor::newSPHEmitter);
	QObject::connect(addSPHViscosityAction, &QAction::triggered, &DfxQToolShelfConstructor::addSPHViscosity);
	QObject::connect(addSPHVorticityAction, &QAction::triggered, &DfxQToolShelfConstructor::addSPHVorticity);
	QObject::connect(addSPHAdhesionAction, &QAction::triggered, &DfxQToolShelfConstructor::addSPHAdhesion);

	QObject::connect(voronoiFractureAction, &QAction::triggered, &DfxQToolShelfConstructor::voronoiFracture);
	QObject::connect(sliceFractureAction, &QAction::triggered, &DfxQToolShelfConstructor::sliceFracture);
	QObject::connect(cutoutFractureAction, &QAction::triggered, &DfxQToolShelfConstructor::cutoutFracture);
}

// Object Tools
void DfxQToolShelfConstructor::planeSlot() {
	DfxGlobal::command::exec("createPlane");
}

void DfxQToolShelfConstructor::boxSlot() {
	DfxGlobal::command::exec("createBox");
}

void DfxQToolShelfConstructor::sphereSlot() {
	DfxGlobal::command::exec("createSphere");
}

void DfxQToolShelfConstructor::torusSlot() {
	DfxGlobal::command::exec("createTorus");
}

// Rigid Body Tools
void DfxQToolShelfConstructor::newRigidBody() {
	QFile file(":/shelf_scripts/new_rigid_obj");

	if (file.open(QIODevice::ReadOnly)) {
		QString script = file.readAll();
		DfxPythonGlobal::getInstance()->exec(script.toStdString());
	}
	else {
		DfxGlobal::console::println("Failed to load new rigid body script");
	}
}

void DfxQToolShelfConstructor::rigidToStatic() {
	QFile file(":/shelf_scripts/static_rigid_body");

	if (file.open(QIODevice::ReadOnly)) {
		QString script = file.readAll();
		DfxPythonGlobal::getInstance()->exec(script.toStdString());
	}
	else {
		DfxGlobal::console::println("Failed to load static rigid body script");
	}
}

void DfxQToolShelfConstructor::rigidToDynamic() {
	QFile file(":/shelf_scripts/dynamic_rigid_body");

	if (file.open(QIODevice::ReadOnly)) {
		QString script = file.readAll();
		DfxPythonGlobal::getInstance()->exec(script.toStdString());
	}
	else {
		DfxGlobal::console::println("Failed to load new dynamic rigid body script");
	}
}

void DfxQToolShelfConstructor::rigidGlueConstraint() {
	QFile file(":/shelf_scripts/glue_constraint");

	if (file.open(QIODevice::ReadOnly)) {
		QString script = file.readAll();
		DfxPythonGlobal::getInstance()->exec(script.toStdString());
	}
	else {
		DfxGlobal::console::println("Failed to load glue constraint script");
	}
}

//SPH Tools
void DfxQToolShelfConstructor::newSPHBody() {
	QFile file(":/shelf_scripts/new_sph_body");

	if (file.open(QIODevice::ReadOnly)) {
		QString script = file.readAll();
		DfxPythonGlobal::getInstance()->exec(script.toStdString());
	}
	else {
		DfxGlobal::console::println("Failed to load glue constraint script");
	}
}

void DfxQToolShelfConstructor::newSPHFluid() {
	QFile file(":/shelf_scripts/new_sph_fluid");

	if (file.open(QIODevice::ReadOnly)) {
		QString script = file.readAll();
		DfxPythonGlobal::getInstance()->exec(script.toStdString());
	}
	else {
		DfxGlobal::console::println("Failed to load glue constraint script");
	}
}

void DfxQToolShelfConstructor::newSPHEmitter() {
	QFile file(":/shelf_scripts/new_sph_emitter");

	if (file.open(QIODevice::ReadOnly)) {
		QString script = file.readAll();
		DfxPythonGlobal::getInstance()->exec(script.toStdString());
	}
	else {
		DfxGlobal::console::println("Failed to load glue constraint script");
	}
}

void DfxQToolShelfConstructor::addSPHViscosity() {
	QFile file(":/shelf_scripts/add_sph_viscosity");

	if (file.open(QIODevice::ReadOnly)) {
		QString script = file.readAll();
		DfxPythonGlobal::getInstance()->exec(script.toStdString());
	}
	else {
		DfxGlobal::console::println("Failed to load glue constraint script");
	}
}

void DfxQToolShelfConstructor::addSPHVorticity() {
	QFile file(":/shelf_scripts/add_sph_vorticity");

	if (file.open(QIODevice::ReadOnly)) {
		QString script = file.readAll();
		DfxPythonGlobal::getInstance()->exec(script.toStdString());
	}
	else {
		DfxGlobal::console::println("Failed to load glue constraint script");
	}
}

void DfxQToolShelfConstructor::addSPHAdhesion() {
	QFile file(":/shelf_scripts/add_sph_adhesion");

	if (file.open(QIODevice::ReadOnly)) {
		QString script = file.readAll();
		DfxPythonGlobal::getInstance()->exec(script.toStdString());
	}
	else {
		DfxGlobal::console::println("Failed to load glue constraint script");
	}
}

void DfxQToolShelfConstructor::voronoiFracture() {
	QFile file(":/shelf_scripts/voronoi_fracture");

	if (file.open(QIODevice::ReadOnly)) {
		QString script = file.readAll();
		DfxPythonGlobal::getInstance()->exec(script.toStdString());
	}
	else {
		DfxGlobal::console::println("Failed to load glue constraint script");
	}
}

void DfxQToolShelfConstructor::sliceFracture() {
	QFile file(":/shelf_scripts/slice_fracture");

	if (file.open(QIODevice::ReadOnly)) {
		QString script = file.readAll();
		DfxPythonGlobal::getInstance()->exec(script.toStdString());
	}
	else {
		DfxGlobal::console::println("Failed to load glue constraint script");
	}
}

void DfxQToolShelfConstructor::cutoutFracture() {
	QFile file(":/shelf_scripts/cutout_fracture");

	if (file.open(QIODevice::ReadOnly)) {
		QString script = file.readAll();
		DfxPythonGlobal::getInstance()->exec(script.toStdString());
	}
	else {
		DfxGlobal::console::println("Failed to load glue constraint script");
	}
}
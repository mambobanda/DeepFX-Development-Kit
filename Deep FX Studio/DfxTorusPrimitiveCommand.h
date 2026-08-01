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

#include <generator/TorusMesh.hpp>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinbox>
#include "DfxGlobal.h"
#include "DfxScene.h"
#include "DfxAbstractCommand.h"
#include "DfxDefaultGlobalIDs.h"
#include "DfxObjectItem.h"
#include "DfxCoreApplication.h"

namespace DfxCommands {

	// Torus Primitive
	class TorusPrimitiveCommand : public DfxAbstractCommand {
	private:
		static size_t id_gen;
	public:
		virtual void execute();

		virtual void execute(DfxCmdArguments);

		static void *create();
	};

}
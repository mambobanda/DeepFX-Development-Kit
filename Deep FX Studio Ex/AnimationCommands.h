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

#include <DfxAbstractCommand.h>
#include "DfxSpace.h"
#include "DfxUpdater.h"
#include "DfxProject.h"
#include "DfxQSceneManager.h"
#include "DfxMode.h"
#include "DfxKeyableTypes.h"
#include "DfxSceneSelectionModel.h"
#include "DfxPropertyConduit.h"

namespace command {
	class CreateKey : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};

	class GoToFrame : public DfxAbstractCommand {
	public:
		virtual void execute();
		virtual void execute(DfxCmdArguments);
		static void *create();
	};
}

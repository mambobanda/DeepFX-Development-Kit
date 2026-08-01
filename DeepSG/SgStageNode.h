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


#include "SgNode.h"
#include "SgStageActorCollectionNode.h"
#include "SgStageActorNode.h"
#include "SgStagePropCollectionNode.h"
#include "SgStagePropNode.h"

namespace deepsg {

	class SgStageNode : public SgNode {
	private:
		size_t _stageType;
	private:
		SgStageActorCollectionNode *_actorCollection; 
		SgStagePropCollectionNode  *_propCollection;

	public:
		SgStageNode(size_t stageT);
		virtual ~SgStageNode();

		std::vector<SgStageActorNode*> actors();
		std::vector<SgStagePropNode*>  props();

		SgStageActorCollectionNode *actorCollectionNode();
		SgStagePropCollectionNode  *propCollectionNode();

		size_t stageType();
	};

};
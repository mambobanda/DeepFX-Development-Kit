/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include <string>
#include <FlAny.h>
#include <SgNode.h>

class GlobalState {
public:

	class Space {
	public:
		fl::Space *(*root)();
		fl::Any *(*findAbsolute)(std::string path);
		void (*remove)(fl::Any *);
		void (*move)(fl::Any *any, fl::Any *dest, fl::SpaceObserver *source);
		fl::Any *(*any)(std::string name, std::string label);
	};

	class SceneGraph {
	public:
		void (*moveNode)(deepsg::SgNode *node, deepsg::SgNode *to);
		void (*removeNode)(deepsg::SgNode *node);
		deepsg::SgNode *(*rootNode)();
	};

private:

	Space _space;
	SceneGraph _sgraph;

public:
	static void g_sceneUpdated();

	static fl::Any *spc_findAbsolute(std::string path);
	static void spc_remove(fl::Any *any);
	static void spc_move(fl::Any *any, fl::Any *dest, fl::SpaceObserver *source);
	static fl::Any *spc_any(std::string name, std::string label);
	static fl::Space *spc_root();
	
	static void sg_moveNode(deepsg::SgNode *node, deepsg::SgNode *to);
	static void sg_removeNode(deepsg::SgNode *node);
	static deepsg::SgNode *sg_rootNode();

public:

	// Console & Input
	void (*println)(std::string);
	int  (*messageBox)(std::string text, std::string infor_text, int width, std::string cltext);
	void (*exec)(std::string);
	void (*sceneUpdated)();

	Space *space();
	SceneGraph *sceneGraph();

public:
	static GlobalState *createGlobalState();
};
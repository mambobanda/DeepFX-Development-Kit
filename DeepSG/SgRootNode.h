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

#include <stack>
#include <functional>
#include "SgStateInfo.h"
#include "SgNode.h"
#include "SgShapeNode.h"
#include "SgGroupNode.h"
#include "SgGeoNode.h"
#include "SgTransformNode.h"
#include "SgWidgetNode.h"
#include "SgTransform.h"

namespace deepsg {

	enum class TraversalFlag { END, CONTINUE, IGNORE_CHILDREN };

	using SgTraversalFunction = std::function<void (SgNode *node, SgStateInfo *info)>;
	using SgConditionalTraversalFunction = std::function<TraversalFlag(SgNode *node, SgStateInfo *info)>;

	class SgRootNode : public SgNode {
	public:

		class StateInfo : public SgStateInfo {
		private:
			SgNode *p;
			glm::mat4 mat;
		public:
			void setParent(SgNode *parent) { p = parent; }

			void setMatrix(glm::mat4 mt) { mat = mt; }

			SgNode *parent() { return p; }
			glm::mat4 transformMatrix() { return mat; };
		};

	private:
		SgRootNode *_root;
		SgRootNode *_fluentRoot;
		static SgRootNode *_instance;
		static std::string _uniqueName;
		SgRootNode();
		SgRootNode(SgRootNode *rt, SgRootNode *sgFluent);

	private:
		void _traverseScene00(SgNode *node, SgTraversalFunction travFunc, SgStateInfo *info, unsigned int layer=0);
		TraversalFlag _traverseScene01(SgNode *node, SgConditionalTraversalFunction travFunc, SgStateInfo *info, unsigned int layer = 0);

	public:
		
		virtual ~SgRootNode();

		static SgRootNode *getInstance();

		std::string uniqueName();

		void traverseScene(SgTraversalFunction travFunc, glm::mat4 root_transform = glm::mat4(1.f));
		void traverseSubScene(SgNode *node, SgTraversalFunction travFunc, glm::mat4 root_transform = glm::mat4(1.f));
		void traverseSceneConditionally(SgConditionalTraversalFunction travFunc);
		void traverseSubSceneConditionally(SgNode *node, SgConditionalTraversalFunction travFunc, glm::mat4 root_transform);
		void traverseFluentScene(SgTraversalFunction travFunc, glm::mat4 root_transform = glm::mat4(1.f));
		void traverseFluentSceneConditionally(SgConditionalTraversalFunction travFunc);

		SgRootNode *root();
		SgRootNode *fluentRoot();

		glm::mat4 getTransformation4x4(SgNode *node);

		std::vector<SgNode*> findAllNodeTypes(SgNodeType type);
	};
}


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

#include <vector>
#include <Fluent.h>
#include <queue>
#include "SgCommon.h"
#include "SgNodeType.h"
#include "SgNodeID.h"
#include "SgTransform.h"
#include "SgUtil.h"

namespace deepsg {

	class SgNode: public SgTransform {
	public:
		struct Signal { enum { NONE = 0, UPDATED = 1 }; };

	private:
		std::unordered_map<size_t, SgNode*> _listeners;
		std::unordered_map<size_t, SgNode*> _emitters;

		friend class SgNode;
	private:
		SgNode *_parent;
		std::vector<SgNode*> _children;
		std::string _referencePath;

	private:
		std::vector<SgTransformOverride*> _transformOverrides;
		std::vector<SgNodeFilter*> _filters;

	private:
		size_t nid;
		SgNodeType type;
	
	private:
		unsigned int _layer;

	protected:
		bool independent;
		bool _protected;
		bool _moveable;

	protected:
		bool _visible;
		bool _renderable;

	protected:
		virtual void onSetReferencePath(std::string path);

	public:

		SgNode(SgNodeType nt);
		virtual ~SgNode();

		virtual std::string uniqueName() = 0;

		size_t getID();
		SgNodeType getType();

		void setParent(SgNode *p);
		SgNode *parent();
		std::vector<SgNode*> &children();

		size_t size();

		bool isIndependent();

		bool isProtected();
		void setIsProtected(bool);

		bool isMovable();
		void setIsMovable(bool);

		bool isVisible();
		void setIsVisible(bool);

		bool isRenderable();
		void setIsRenderable(bool);

		unsigned int layer();
		void setLayer(unsigned int);

		void addChild(SgNode *);
		void removeChild(SgNode *);
		void removeChild(size_t);
		void moveChild(size_t src, size_t dest);
		SgNode *childAt(size_t inx);

		size_t relativeIndex();

		SgNode *findChild(SgNode *);
		SgNode *findChild(size_t);

		std::string referencePath();
		void setReferencePath(std::string path);

		void addTransformOverride(SgTransformOverride*);
		void removeTransformOverride(SgTransformOverride*);
		std::vector<SgTransformOverride*> transformOverrides();
		void clearTransformOverrides();

		virtual void beginningEvaluatingTools();
		void evaluateTools();
		virtual void finishedEvaluatingTools();

		void addNodeFilter(SgNodeFilter *);
		void removeNodeFilter(SgNodeFilter *);

		virtual void beginningEvaluatingNodeFilters();
		void evaluateNodeFilters(SgStateInfo *info);
		virtual void finishedEvaluatingNodeFilters();

		void listenToNode(SgNode *);
		void ignoreNode(SgNode *);
		void ignoreAll();
		void unsubscribeListener(SgNode *);
		void unsubscribeAll();
		void emitSignal(size_t);
		virtual void processSignal(size_t signal);

	};

}
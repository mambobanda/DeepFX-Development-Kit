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
#include <limits>
#include <set>
#include "DfxSceneEvent.h"
#include "DfxSceneActionListener.h"
#include "DfxObjectItem.h"
#include "DfxPointItem.h"
#include "DfxAbstractGizmo.h"
#include "DfxTranslateGizmo.h"
#include "DfxScaleGizmo.h"
#include "DfxRotateGizmo.h"
#include "DfxRay.h"

class DfxScene {
public:
	enum {
		TRANSLATE_TRANSFORM = 0xff,
		ROTATE_TRANSFORM,
		SCALE_TRANSFORM
	};
private:
	static DfxScene *instance;
	DfxScene();

private:

	class SelectionListComparator{
		public: 
			bool operator() (const std::pair<size_t, size_t> &lhs, const std::pair<size_t, size_t> &rhs);
	};
private:
	DfxAbstractGizmo *activeGizmo;

	std::vector<DfxViewItem*> itemList;
	
	std::set<std::pair<size_t, size_t>, SelectionListComparator> selectionList;

	std::vector<DfxSceneActionListener*> actionListnerList;
	
	size_t transformMode;

	void itemSelectionEvent(DfxSceneEvent evt);
	void itemSelectionEventColorMode(DfxSceneEvent evt);
	void gizmoSelectionEvent(DfxSceneEvent evt);

	void announceAction(DfxSceneActionInfo info);
public:

	virtual ~DfxScene();

	static DfxScene *getInstance();

	void addItem(DfxViewItem*);
	void addItemNoAnnounce(DfxViewItem* obj);
	bool hasItem(DfxViewItem*);
	size_t itemCount();
	DfxViewItem *getItem(size_t index);
	DfxViewItem *getItemByID(size_t index);
	void removeItemByID(size_t inst_id);

	void newSceneEvent(DfxSceneEvent evt);

	void clearSelected();

	void clearSelectionIfSelected(size_t);

	size_t getTransformMode();
	void setTransformMode(size_t);

	DfxAbstractGizmo *getActiveGizmo();
	void setActiveGizmo(DfxAbstractGizmo *);
	void clearActiveGizmo();

	std::vector<DfxViewItem*> getAllItems();
	std::vector<DfxViewItem*> getSelectedItems();
	std::vector<DfxViewItem*> getUnSelectedItems();
	void clearSelection();

	void addActionListener(DfxSceneActionListener *);

};
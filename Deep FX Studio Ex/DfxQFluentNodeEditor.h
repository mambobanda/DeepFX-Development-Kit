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


#include <QMainWindow>
#include <QSplitter>
#include <QListView>
#include <QMenu>
#include <QBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QToolbar>
#include <QSplitter>
#include <stack>
#include "DfxProject.h"
#include "DfxMainEventLoop.h"
#include "DfxQMenuEditor.h"
#include "DfxComputeNodeDB.h"

#include "DfxQFluentEditorCrumb.h"
#include "DfxQListenerEnums.h"
#include "DfxListenerHandler.h"
#include "DfxQListenerNodeView.h"
#include "DfxQListenerCollection.h"
#include "DfxQListenerNode.h"

#include "DfxQFlowNodeProperties.h"
#include "DfxQFlowNodeView.h"
#include "DfxQFlowCollection.h"
#include "DfxQFlowProcessNode.h"
#include "DfxQFlowBreakNode.h"
#include "DfxQFlowStartNode.h"
#include "DfxQFlowDecisionNode.h"

#include "DfxQComputeNodeView.h"
#include "DfxQComputeNodeCollection.h"
#include "DfxQComputeNodeProperties.h"

#include "DfxCompoundNodeHandler.h"

#include "DfxQKeyBuffer.h"

class DfxFluentNodeEditor : public QMainWindow, public DfxQKeyBuffer {
private:

	QToolBar *_navToolbar;
	QAction  *_navBackward;
	QAction  *_navHome;

	QBoxLayout *_listenerMainLayout;
	QBoxLayout *_listenerMenuLayout;

	QWidget *_listenerMainWidget;
	QPushButton *_listenerAddMenuButton;
	QMenu *_listenerAddMenu;

	DfxQListenerNodeView *_listenerNodeView;
	DfxQListenerCollection *_listenerNodeCollection;

	QBoxLayout *_flowMainLayout;
	QBoxLayout *_flowMenuLayout;
	QSplitter  *_flowViewLayout;

	QWidget *_flowMainWidget;
	QPushButton *_flowAddMenuButton;
	QMenu   *_flowAddMenu;
	QAction *_addProcessAction;
	QAction *_addDecisionAction;
	QAction *_addBreakAction;

	DfxQFlowNodeProperties *_flowNodeProperties;
	DfxQFlowNodeView *_flowNodeView;
	DfxQFlowCollection *_flowNodeCollection;

	QBoxLayout *_computeMainLayout;
	QBoxLayout *_computeMenuLayout;
	QSplitter  *_computeViewLayout;

	QWidget *_computeMainWidget;
	QPushButton *_computeAddMenuButton;
	QPushButton *_computeEditMenuButton;
	QPushButton *_computeLayoutMenuButton;

	QMenu   *_computeAddMenu;
	QMenu   *_computeEditMenu;
	QMenu   *_computeLayoutMenu;

	QAction *editCopy;
	QAction *editPaste;

	QAction *layoutViewAll;

	DfxQComputeNodeProperties *_computeNodeProperties;
	DfxQComputeNodeView *_computeNodeView;
	DfxQComputeNodeCollection *_computeNodeCollection;

	DfxQFluentCollectionBase *_currentCollection;

	void addNewListener(unsigned int);
	void populateAddMenu();

	void styleMenuButtons();

private:

	std::stack<DfxQFluentEditorCrumb> _viewstack;

	std::vector<std::pair<std::string, std::string>> _copyBuffer;

protected:

	bool eventFilter(QObject *watched, QEvent *event) override;
public:

	DfxFluentNodeEditor();
	~DfxFluentNodeEditor();

	void unloadCurrentView();
	void loadCrumb(DfxQFluentEditorCrumb);
	void pushView(DfxQFluentEditorCrumb);
	void popView();

	void populateComputeAddMenu();

	DfxQComputeNodeCollection *computeNodeCollection();
public slots:
	
	void addFlowProccessNodeSlot();
	void addFlowDecisionNodeSlot();
	void addFlowBreakNodeSlot();

	void flowSelectionChanged();

	void computeNodesSelected(std::vector<DfxQComputeBaseNode*> nodes);

	void redrawComputeNodes();

	void home();

	void copySelectedNodes();
	void pasteNodes();

	void computeViewAll();

	void reset();
};
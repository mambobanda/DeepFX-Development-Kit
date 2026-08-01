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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxQFluentNodeEditor.h"
#include "DfxQComputeNode.h"
#include "DfxQComputeCompoundNode.h"
#include "DfxNodeUtils.h"
#include "DfxSceneCleaner.h"

DfxFluentNodeEditor::DfxFluentNodeEditor() {

	_listenerMainLayout = new QVBoxLayout();
	_listenerMenuLayout = new QHBoxLayout();

	_listenerAddMenuButton = new QPushButton("Add Listener");
	_listenerAddMenu = new QMenu();
	
	_listenerNodeView = new DfxQListenerNodeView;
	_listenerNodeCollection = new DfxQListenerCollection;

	_listenerMainWidget = new QWidget();

	_navToolbar  = addToolBar("");
	_navBackward = new QAction(QPixmap(":/icon/back_icon").scaledToWidth(100), "");
	_navHome	 = new QAction(QPixmap(":/icon/home_icon").scaledToWidth(100), "");

	_flowMainLayout = new QVBoxLayout;
	_flowMenuLayout = new QHBoxLayout;
	_flowViewLayout = new QSplitter();

	_flowMainWidget = new QWidget;
	
	_flowAddMenuButton  = new QPushButton("Add");
	_flowAddMenu		= new QMenu("");
	_addProcessAction	= new QAction(QPixmap(":/icon/flow_process_icon"),"Process");
	_addDecisionAction	= new QAction(QPixmap(":/icon/flow_decision_icon"), "Decision");
	_addBreakAction		= new QAction(QPixmap(":/icon/flow_break_icon"), "Break");

	_flowNodeView = new DfxQFlowNodeView();
	_flowNodeCollection = new DfxQFlowCollection();

	_flowNodeProperties = new DfxQFlowNodeProperties;

	_computeAddMenuButton = new QPushButton("Add");
	_computeAddMenu       = new QMenu("");

	_computeEditMenuButton = new QPushButton("Edit");
	_computeEditMenu = new QMenu("");

	_computeLayoutMenuButton = new QPushButton("Layout");
	_computeLayoutMenu = new QMenu("");

	editCopy	= new QAction("Copy");
	editPaste	= new QAction("Paste");

	layoutViewAll = new QAction("View All");

	layoutViewAll;

	_computeNodeView = new DfxQComputeNodeView(this);
	_computeNodeCollection = new DfxQComputeNodeCollection();

	_computeNodeProperties = new DfxQComputeNodeProperties;

	_computeMainLayout = new QVBoxLayout;
	_computeMenuLayout = new QHBoxLayout;
	_computeViewLayout = new QSplitter();

	_computeMainWidget = new QWidget;

	_flowNodeProperties->setEditor(this);

	_listenerNodeCollection->setEditorPtr(this);
	_flowNodeCollection->setEditorPtr(this);
	_computeNodeCollection->setEditor(this);

	_listenerNodeCollection->installEventFilter(_listenerNodeView);
	_flowNodeCollection->installEventFilter(_flowNodeView);
	_computeNodeCollection->installEventFilter(_computeNodeView);

	_currentCollection = _listenerNodeCollection;

	_listenerMainLayout->setContentsMargins(0, 0, 0, 0);

	_listenerMenuLayout->setContentsMargins(5, 5, 0, 0);
	_listenerMenuLayout->setAlignment(Qt::AlignLeft);

	_flowMainLayout->setContentsMargins(0, 0, 0, 0);

	_flowMenuLayout->setContentsMargins(5, 5, 0, 0);
	_flowMenuLayout->setAlignment(Qt::AlignLeft);

	_flowViewLayout->setContentsMargins(0, 0, 0, 0);

	_computeMainLayout->setContentsMargins(0, 0, 0, 0);

	_computeMenuLayout->setContentsMargins(5, 5, 0, 0);
	_computeMenuLayout->setAlignment(Qt::AlignLeft);

	_computeViewLayout->setContentsMargins(0, 0, 0, 0);


	_listenerAddMenuButton->setMenu(_listenerAddMenu);
	_listenerAddMenuButton->setFixedWidth(80);

	_listenerMenuLayout->addWidget(_listenerAddMenuButton);

	_listenerMainLayout->addLayout(_listenerMenuLayout);
	_listenerMainLayout->addWidget(_listenerNodeView);

	_listenerMainWidget->setLayout(_listenerMainLayout);

	_listenerNodeView->setScene(_listenerNodeCollection);

	_flowAddMenu->addAction(_addProcessAction);
	_flowAddMenu->addAction(_addDecisionAction);
	_flowAddMenu->addAction(_addBreakAction);

	_flowAddMenuButton->setMenu(_flowAddMenu);
	_flowAddMenuButton->setFixedWidth(80);

	_flowMenuLayout->addWidget(_flowAddMenuButton);

	_flowViewLayout->addWidget(_flowNodeView);
	_flowViewLayout->addWidget(_flowNodeProperties);
	_flowViewLayout->setSizes({1000, 200});

	_flowMainLayout->addLayout(_flowMenuLayout);
	_flowMainLayout->addWidget(_flowViewLayout);

	_flowMainWidget->setLayout(_flowMainLayout);

	_flowNodeView->setScene(_flowNodeCollection);
	
	populateComputeAddMenu();

	_computeEditMenu->addAction(editCopy);
	_computeEditMenu->addAction(editPaste);

	_computeLayoutMenu->addAction(layoutViewAll);

	_computeAddMenuButton->setMenu(_computeAddMenu);
	_computeAddMenuButton->setFixedWidth(80);

	_computeEditMenuButton->setMenu(_computeEditMenu);
	_computeEditMenuButton->setFixedWidth(80);

	_computeLayoutMenuButton->setMenu(_computeLayoutMenu);
	_computeLayoutMenuButton->setFixedWidth(80);

	_computeMenuLayout->addWidget(_computeAddMenuButton);
	_computeMenuLayout->addWidget(_computeEditMenuButton);
	_computeMenuLayout->addWidget(_computeLayoutMenuButton);

	_computeViewLayout->addWidget(_computeNodeView);
	_computeViewLayout->addWidget(_computeNodeProperties);
	_computeViewLayout->setSizes({ 1000, 200 });

	_computeMainLayout->addLayout(_computeMenuLayout);
	_computeMainLayout->addWidget(_computeViewLayout);

	_computeMainWidget->setLayout(_computeMainLayout);

	_computeNodeView->setScene(_computeNodeCollection);

	_navToolbar->addAction(_navBackward);
	_navToolbar->addAction(_navHome);
	_navToolbar->setMovable(false);

	setCentralWidget(_listenerMainWidget);

	styleMenuButtons();

	populateAddMenu();

	_listenerNodeCollection->addExisting("");

	installEventFilter(this);

	QObject::connect(_navBackward, &QAction::triggered, [this]() {
		popView();
	});

	QObject::connect(_navHome, &QAction::triggered, [this]() {
		home();
	});

	QObject::connect(_addProcessAction, &QAction::triggered, this, &DfxFluentNodeEditor::addFlowProccessNodeSlot);
	QObject::connect(_addDecisionAction, &QAction::triggered, this, &DfxFluentNodeEditor::addFlowDecisionNodeSlot);
	QObject::connect(_addBreakAction, &QAction::triggered, this, &DfxFluentNodeEditor::addFlowBreakNodeSlot);

	QObject::connect(_flowNodeCollection, &QGraphicsScene::selectionChanged, this, &DfxFluentNodeEditor::flowSelectionChanged);

	QObject::connect(_computeNodeView, &DfxQComputeNodeView::nodesSelected, this, &DfxFluentNodeEditor::computeNodesSelected);

	QObject::connect(DfxMainEventLoop::getInstance(), &DfxMainEventLoop::finshedProcessingEvents, this, &DfxFluentNodeEditor::redrawComputeNodes);

	QObject::connect(DfxProject::getInstance(), &DfxProject::finishedLoadingNewProject, this, &DfxFluentNodeEditor::home);

	QObject::connect(editCopy, &QAction::triggered, this, &DfxFluentNodeEditor::copySelectedNodes);
	QObject::connect(editPaste, &QAction::triggered, this, &DfxFluentNodeEditor::pasteNodes);

	QObject::connect(layoutViewAll, &QAction::triggered, this, &DfxFluentNodeEditor::computeViewAll);

	QObject::connect(DfxSceneCleaner::getInstance(), &DfxSceneCleaner::resetSignal, this, &DfxFluentNodeEditor::reset);

}

DfxFluentNodeEditor::~DfxFluentNodeEditor() {

}

DfxQComputeNodeCollection *DfxFluentNodeEditor::computeNodeCollection() {
	return _computeNodeCollection;
}

void DfxFluentNodeEditor::addNewListener(unsigned int lst) {

	DfxListenerHandler handler;
	auto nlst = handler.create(lst);

	if (nlst) {
		auto lst = _listenerNodeCollection->addListener(handler);
		lst->setEditorPtr(this);
	}
	else {
		DfxGlobal::console::println("Listener is not created.");
	}
}

void DfxFluentNodeEditor::populateAddMenu() {

	for (auto p : event::enumList()) {
		DfxQMenuEditor::addAction(_listenerAddMenu, p.second.c_str(), [this, p]() { addNewListener(p.first); });
	}

}

void DfxFluentNodeEditor::styleMenuButtons() {

	QString menuButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"padding-left: 2px;"
		"height: 13px;"
		"}";

	QString navButtonSty =
		"QToolButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"padding-left: 2px;"
		"border-top-left-radius: 6px;"
		"border-bottom-right-radius: 6px;"
		"}";

	QString hoverSty =
		"QPushButton:hover { "
		"background-color: rgb(100,100,100);"
		"}";

	QString hoverPressdSty =
		"QPushButton:hover:pressed { "
		"background-color: rgb(255, 153, 83);"
		"}";

	QString hoverNavSty =
		"QToolButton:hover { "
		"background-color: rgb(100,100,100);"
		"}";

	QString hoverNavPressdSty =
		"QToolButton:hover:pressed { "
		"background-color: rgb(255, 153, 83);"
		"}";

	_listenerAddMenuButton->setStyleSheet(menuButtonSty + hoverSty + hoverPressdSty);
	_flowAddMenuButton->setStyleSheet(menuButtonSty + hoverSty + hoverPressdSty);
	_computeEditMenuButton->setStyleSheet(menuButtonSty + hoverSty + hoverPressdSty);
	_computeLayoutMenuButton->setStyleSheet(menuButtonSty + hoverSty + hoverPressdSty);
	_computeAddMenuButton->setStyleSheet(menuButtonSty + hoverSty + hoverPressdSty);
	_navToolbar->setStyleSheet(navButtonSty + hoverNavSty + hoverNavPressdSty);
}

void DfxFluentNodeEditor::pushView(DfxQFluentEditorCrumb crumb) {

	auto info = _currentCollection->typeInfo();
	DfxQFluentEditorCrumb cm;
	cm.type = info.second;
	cm.path = info.first;
	_viewstack.push(cm);

	loadCrumb(crumb);
}

void DfxFluentNodeEditor::popView() {

	if (!_viewstack.empty()) {

		auto cm = _viewstack.top();
		_viewstack.pop();

		loadCrumb(cm);
	}
}

void DfxFluentNodeEditor::home() {
	while (!_viewstack.empty()) {
		_viewstack.pop();
	}

	DfxQFluentEditorCrumb crumb;
	crumb.path = "";
	crumb.type = QFluentViewType::LISTENER;
	loadCrumb(crumb);
}

void DfxFluentNodeEditor::loadCrumb(DfxQFluentEditorCrumb crumb) {

	if (crumb.type == QFluentViewType::LISTENER) {

		unloadCurrentView();
		setCentralWidget(_listenerMainWidget);

		_listenerNodeCollection->clearAll();
		_listenerNodeCollection->addExisting(crumb.path);

		_currentCollection = _listenerNodeCollection;
	}
	else if (crumb.type == QFluentViewType::FLOW) {

		unloadCurrentView();
		setCentralWidget(_flowMainWidget);

		_flowNodeCollection->clearAll();
		_flowNodeCollection->addExisting(crumb.path);
	
		_currentCollection = _flowNodeCollection;

		_flowNodeView->updatePositions();

	}
	else if (crumb.type == QFluentViewType::NODE ||
		     crumb.type == QFluentViewType::COMPOUND) {

		unloadCurrentView();
		setCentralWidget(_computeMainWidget);

		_computeNodeCollection->clearAll();
		_computeNodeCollection->addExisting(crumb.path);

		if(crumb.type == QFluentViewType::COMPOUND) {

		}

		_currentCollection = _computeNodeCollection;
	}
}

void DfxFluentNodeEditor::unloadCurrentView() {
	auto info = _currentCollection->typeInfo();

	if (info.second == QFluentViewType::LISTENER) {
		_listenerMainWidget->setParent(nullptr);
	}
	else if (info.second == QFluentViewType::FLOW) {
		_flowMainWidget->setParent(nullptr);
	}
	else if (info.second == QFluentViewType::NODE ||
			 info.second == QFluentViewType::COMPOUND) {
		_computeMainWidget->setParent(nullptr);
	}
}

void DfxFluentNodeEditor::addFlowProccessNodeSlot() {
	_flowNodeCollection->addNewProcess();
}

void DfxFluentNodeEditor::addFlowDecisionNodeSlot() {
	_flowNodeCollection->addNewDecision();
}

void DfxFluentNodeEditor::addFlowBreakNodeSlot() {
	_flowNodeCollection->addNewBreak();
}

void DfxFluentNodeEditor::flowSelectionChanged() {
	QList<QGraphicsItem*> items = _flowNodeCollection->selectedItems();

	if (!items.empty()) {

		if (items[0]->type() == DfxQNodeEnums::FLOW_PROCESS_NODE) {

			auto pnode = (DfxQFlowProcessNode*)items[0];
			DfxFlowHandler handler;

			if (handler.fetch(pnode->flowPath())) {
				auto flow = handler.flow();
				_flowNodeProperties->showProcessProperties(pnode, pnode->fid(), flow);
			}
			else {
				_flowNodeProperties->clear();
			}
		}
		else if (items[0]->type() == DfxQNodeEnums::FLOW_DECISION_NODE) {

			auto dnode = (DfxQFlowDecisionNode*)items[0];
			DfxFlowHandler handler;

			if (handler.fetch(dnode->flowPath())) {
				auto flow = handler.flow();
				_flowNodeProperties->showDecisionProperties(dnode, dnode->fid(), flow);
			}
			else {
				_flowNodeProperties->clear();
			}
		}
		else {
			_flowNodeProperties->clear();
		}
				
	}
	else {
		_flowNodeProperties->clear();
	}
}

void DfxFluentNodeEditor::computeNodesSelected(std::vector<DfxQComputeBaseNode*> nodes) {

	if (!nodes.empty()) {
		_computeNodeProperties->newNodePropertiesForm(nodes[0]);
	}
	else {
		_computeNodeProperties->clear();
	}
}

void DfxFluentNodeEditor::redrawComputeNodes() {
	for (auto item : _computeNodeCollection->items()) {
		if (item->type() == DfxQNodeEnums::COMPUTE_NODE) {
			item->update();
		}
	}
}

void DfxFluentNodeEditor::populateComputeAddMenu() {
	auto computeDesc = DfxComputeNodeDB::getInstance()->findAll();

	DfxQMenuEditor::addAction(_computeAddMenu, "Compound/Compound Node", [this]() {
		auto space = DfxSpace::getInstance();
		auto compute_graph = space->findAbsolute(_computeNodeCollection->computeGraphPath());

		if (compute_graph != nullptr) {
			DfxCompoundNodeHandler handler;
			auto cnode = handler.createModel(space);

			space->move(cnode, compute_graph);

			auto qnode = new DfxQComputeCompoundNode();
			qnode->setAny(cnode);
			qnode->setEditor(this);
			qnode->buildNode();

			_computeNodeCollection->addItem(qnode);

		}
	});

	for (auto desc : computeDesc) {
		
		DfxQMenuEditor::addAction(_computeAddMenu, desc->placement.c_str(), [desc, this]() {
			auto space = DfxSpace::getInstance();
			auto compute_graph = space->findAbsolute(_computeNodeCollection->computeGraphPath());

			if (compute_graph != nullptr) {
				auto any = desc->instance(space);
				space->move(any, compute_graph);

				auto qnode = new DfxQComputeNode();
				qnode->buildNode(any);
				qnode->setDataObjectPath(any->absolutePath());

				_computeNodeCollection->addItem(qnode);

			}
		});
	}
}

void DfxFluentNodeEditor::copySelectedNodes() {

	_copyBuffer.clear();

	if (_computeNodeCollection != nullptr) {
		for (auto node : _computeNodeCollection->selectedItems()) {

			if (node->type() == DfxQNodeEnums::COMPUTE_NODE) {
				auto bnode = (DfxQComputeBaseNode*)node;
				_copyBuffer.push_back(std::pair<std::string, std::string>(_computeNodeCollection->computeGraphPath(), bnode->any()->absolutePath()));
			}
		}
	}

}

void DfxFluentNodeEditor::pasteNodes() {

	if (_computeNodeCollection != nullptr) {

		if (!_copyBuffer.empty()) {

			for (auto cp : _copyBuffer) {
				auto any = DfxSpace::getInstance()->findAbsolute(cp.second);
				auto collectionAny = DfxSpace::getInstance()->findAbsolute(cp.first);

				if (any != nullptr) {
					if (any->type() == fl::type::DATA_OBJECT) {
						auto dao = any->rawData<fl::DataObject>();

						auto desc = DfxComputeNodeDB::getInstance()->findComputeNodeDescription(dao->modelReference());
						auto anyComputeNode = desc->instance(DfxSpace::getInstance());

						if (anyComputeNode != nullptr) {

							auto computeDao = anyComputeNode->rawData<fl::DataObject>();

							util::copyDataObjectProperties(*dao, *computeDao);

							DfxSpace::getInstance()->move(anyComputeNode, collectionAny);

							auto qnode = new DfxQComputeNode();
							
							qnode->setDataObjectPath(anyComputeNode->absolutePath());					

							auto xpos = computeDao->property("xpos")->data<float>();
							auto ypos = computeDao->property("ypos")->data<float>();

							computeDao->property("xpos")->setData<float>(xpos + -100.f);
							computeDao->property("ypos")->setData<float>(ypos + -100.f);

							qnode->buildNode(anyComputeNode);
							_computeNodeCollection->addItem(qnode);
						}
						
					}
				}
			}

		}
	
	}
}

void DfxFluentNodeEditor::computeViewAll() {
	_computeNodeView->viewFitAll();
}

bool DfxFluentNodeEditor::eventFilter(QObject *watched, QEvent *event) {

	if (event->type() == QEvent::KeyPress) {

		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		addKey(keyEvent->key());

		if (keys().size() > 0) {
			auto shrtc = keysToShortcut();

			if (_currentCollection->typeInfo().second == QFluentViewType::NODE || _currentCollection->typeInfo().second == QFluentViewType::COMPOUND) {

				if (DfxShortcutsDB::getInstance()->match("Compute Node Editor", "Copy Nodes", DfxShortcutsDB::KEYBOARD, shrtc.k00, shrtc.k01, shrtc.k02, shrtc.k03, shrtc.mouseButton)) {
					copySelectedNodes();
					return true;
				}
				else if (DfxShortcutsDB::getInstance()->match("Compute Node Editor", "Paste Nodes", DfxShortcutsDB::KEYBOARD, shrtc.k00, shrtc.k01, shrtc.k02, shrtc.k03, shrtc.mouseButton)) {
					pasteNodes();
					return true;
				}
			}
		}

	}
	else if (event->type() == QEvent::KeyRelease) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		removeKey(filter(keyEvent->key()));
	}

	return QMainWindow::eventFilter(watched, event);
}

void DfxFluentNodeEditor::reset() {
	home();
}
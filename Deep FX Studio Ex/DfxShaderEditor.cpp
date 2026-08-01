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

#include "DfxShaderEditor.h"
#include "DfxLuxcoreMaterials.h"
#include "DfxOSPrayMaterials.h"
#include "DfxSceneCleaner.h"

DfxShaderEditor::DfxShaderEditor(QWidget *parent):QMainWindow(parent) {
	nodeCollection = nullptr;

	nodeView = new DfxQMaterialNodeView();
	propertiesView = new DfxQMaterialNodeProperties;
	materialsView  = new QListView;

	materialViewLayout = new QVBoxLayout();
	nodeMenuLayout	   = new QHBoxLayout();
	nodeViewLayout	   = new QVBoxLayout();
	materialListMenuLayout = new QHBoxLayout();

	addNodeMenuBtn = new QPushButton("Add");
	editNodeMenuBtn = new QPushButton("Edit");
	viewMenuBtn	   = new QPushButton("View");
	layoutMenuBtn  = new QPushButton("Layout");
	
	newMaterial		= new QPushButton("New Material");
	deleteMaterial	= new QPushButton(QPixmap(":/icon/view_close").scaled(8,8),"");

	addNodeMenu		= new QMenu("");
	editNodeMenu	= new QMenu("");
	viewNodeMenu	= new QMenu("");
	layoutNodeMenu	= new QMenu("");;

	luxMenu = new QMenu("LuxCoreRender");
	luxMaterials	= new QMenu("Materials");
	luxTextures		= new QMenu("Textures");
	luxTextureUtility = new QMenu("Texture Utility");

	ospMenu = new QMenu("OSPRay");
	ospMaterials = new QMenu("Materials");
	ospTextures = new QMenu("Textures");

	editCutAction = new QAction("Cut");
	editCopyAction = new QAction("Copy");
	editPasteAction = new QAction("Paste");

	viewViewAll = new QAction("View All");

	nodeViewWidget	= new QWidget;
	materialViewWidget	= new QWidget;
	propertiesViewWidget = new QWidget;

	mainSplitter = new QSplitter(Qt::Orientation::Horizontal);
	listViewPropertiesSplitter = new QSplitter(Qt::Orientation::Vertical);
	
	materialsHeader = new QLabel("Materials");

	materialsHeader->setAlignment(Qt::AlignCenter);

	materialsView->setAlternatingRowColors(true);
	materialsView->setDragEnabled(true);

	style::styleQScrollBar(materialsView->horizontalScrollBar());
	style::styleQScrollBar(materialsView->verticalScrollBar());

	nodeMenuLayout->setContentsMargins(5, 0, 0, 0);
	nodeMenuLayout->setAlignment(Qt::AlignLeft);

	nodeViewLayout->setMargin(0);

	materialListMenuLayout->setContentsMargins(5, 20, 0, 3);
	materialListMenuLayout->setAlignment(Qt::AlignLeft);
	materialListMenuLayout->setSpacing(1);

	materialViewLayout->setMargin(0);
	materialViewLayout->setSpacing(0);

	addNodeMenuBtn->setMenu(addNodeMenu);
	addNodeMenuBtn->setFixedWidth(60);

	editNodeMenuBtn->setMenu(editNodeMenu);
	editNodeMenuBtn->setFixedWidth(60);

	layoutMenuBtn->setMenu(layoutNodeMenu);
	layoutMenuBtn->setFixedWidth(60);

	viewMenuBtn->setMenu(viewNodeMenu);
	viewMenuBtn->setFixedWidth(60);

	newMaterial->setFixedWidth(90);

	deleteMaterial->setFixedWidth(15);

	nodeMenuLayout->addWidget(addNodeMenuBtn);
	nodeMenuLayout->addWidget(editNodeMenuBtn);
	nodeMenuLayout->addWidget(viewMenuBtn);
	// nodeMenuLayout->addWidget(layoutMenuBtn);

	nodeViewLayout->addLayout(nodeMenuLayout);
	nodeViewLayout->addWidget(nodeView);

	nodeViewWidget->setLayout(nodeViewLayout);

	materialListMenuLayout->addWidget(newMaterial);
	materialListMenuLayout->addWidget(deleteMaterial);

	materialViewLayout->addLayout(materialListMenuLayout);
	materialViewLayout->addSpacing(5);
	materialViewLayout->addWidget(materialsHeader);
	materialViewLayout->addWidget(materialsView);

	materialViewWidget->setLayout(materialViewLayout);

	listViewPropertiesSplitter->addWidget(materialViewWidget);
	listViewPropertiesSplitter->addWidget(propertiesView);

	mainSplitter->addWidget(nodeViewWidget);
	mainSplitter->addWidget(listViewPropertiesSplitter);

	listViewPropertiesSplitter->setSizes({500,500});
	mainSplitter->setSizes({ 1000, 400 });

	setupMenus();

	nodeView->setStyleSheet("background-color: rgb(33, 43, 59);");
	
	styleMenuButtons();
		
	setCentralWidget(mainSplitter);

	installEventFilter(this);

	QObject::connect(newMaterial, &QPushButton::clicked, []() { DfxGlobal::command::exec("newMaterial"); });
	QObject::connect(deleteMaterial, &QPushButton::clicked, this, &DfxShaderEditor::removeSelectedMaterial);
	QObject::connect(nodeView, &DfxQMaterialNodeView::nodesSelected, this, &DfxShaderEditor::nodesSelected);

	QObject::connect(editCutAction, &QAction::triggered, this, &DfxShaderEditor::cutSelectedNodes);
	QObject::connect(editCopyAction, &QAction::triggered, this, &DfxShaderEditor::copySelectedNodes);
	QObject::connect(editPasteAction, &QAction::triggered, this, &DfxShaderEditor::pasteNodes);

	QObject::connect(viewViewAll, &QAction::triggered, this, &DfxShaderEditor::viewAll);

	QObject::connect(DfxProject::getInstance(), &DfxProject::finishedLoadingNewProject, this, &DfxShaderEditor::resetSelectedMaterial);

	QObject::connect(DfxSceneCleaner::getInstance(), &DfxSceneCleaner::resetSignal, this, &DfxShaderEditor::reset);


	resetSelectedMaterial();
}

DfxShaderEditor::~DfxShaderEditor() {

}

void DfxShaderEditor::setupMenus() {

	// Edit Menu
	// editNodeMenu->addAction(editCutAction);
	editNodeMenu->addAction(editCopyAction);
	editNodeMenu->addAction(editPasteAction);

	// View Menu
	viewNodeMenu->addAction(viewViewAll);

	// Add Menu
	addNodeMenu->addMenu(luxMenu);
	addNodeMenu->addMenu(ospMenu);

	// Luxcore
	luxMenu->addMenu(luxMaterials);
	luxMaterials->addAction(createAddNodeAction("Null", luxmat::nullID));
	luxMaterials->addAction(createAddNodeAction("Matte", luxmat::matteID));
	luxMaterials->addAction(createAddNodeAction("Rough Matte", luxmat::roughMatteID));
	luxMaterials->addAction(createAddNodeAction("Matte Translucent", luxmat::matteTranslucentID));
	luxMaterials->addAction(createAddNodeAction("Emission", luxmat::emissionID));
	luxMaterials->addAction(createAddNodeAction("Disney", luxmat::disneyID));
	luxMaterials->addAction(createAddNodeAction("Glass", luxmat::glassID));
	luxMaterials->addAction(createAddNodeAction("Arch Glass", luxmat::archGlassID));
	luxMaterials->addAction(createAddNodeAction("Rough Glass", luxmat::roughGlassID));
	luxMaterials->addAction(createAddNodeAction("Car Paint", luxmat::carPaintID));
	luxMaterials->addAction(createAddNodeAction("Cloth", luxmat::clothID));
	luxMaterials->addAction(createAddNodeAction("Glossy", luxmat::glossyID));
	luxMaterials->addAction(createAddNodeAction("Glossy Translucent", luxmat::glossyTranslucentID));
	luxMaterials->addAction(createAddNodeAction("Metal", luxmat::metalID));
	luxMaterials->addAction(createAddNodeAction("Mirror", luxmat::mirrorID));
	luxMaterials->addAction(createAddNodeAction("Velvet", luxmat::velvetID));
	luxMaterials->addAction(createAddNodeAction("Mix", luxmat::mixID));
	
	luxMenu->addMenu(luxTextures);
	luxTextures->addAction(createAddNodeAction("Blend", luxmat::blendID));
	luxTextures->addAction(createAddNodeAction("Cloud", luxmat::cloudID));
	luxTextures->addAction(createAddNodeAction("Distorted Noise", luxmat::distortedNoiseID));
	luxTextures->addAction(createAddNodeAction("Magic", luxmat::magicID));
	luxTextures->addAction(createAddNodeAction("Marble", luxmat::marbleID));
	luxTextures->addAction(createAddNodeAction("Musgrave", luxmat::musgraveID));
	luxTextures->addAction(createAddNodeAction("Noise", luxmat::noiseID));
	luxTextures->addAction(createAddNodeAction("Stucci", luxmat::stucciID));
	luxTextures->addAction(createAddNodeAction("Wood", luxmat::woodID));
	luxTextures->addAction(createAddNodeAction("Voronoi", luxmat::voronoiID));
	luxTextures->addAction(createAddNodeAction("Imagemap", luxmat::imagemapID));
	luxTextures->addAction(createAddNodeAction("Normalmap", luxmat::normalmapID));
	luxTextures->addAction(createAddNodeAction("2D Checkerboard", luxmat::checkerboard2dID));
	luxTextures->addAction(createAddNodeAction("FBM", luxmat::fbmID));
	luxTextures->addAction(createAddNodeAction("Fresnel Color", luxmat::fresnelColorID));

	luxMenu->addMenu(luxTextureUtility);
	luxTextureUtility->addAction(createAddNodeAction("Mix", luxmat::mixTexID));
	luxTextureUtility->addAction(createAddNodeAction("Clamp", luxmat::clampID));
	luxTextureUtility->addAction(createAddNodeAction("Hsv", luxmat::hsvID));

	// OSPRay
	ospMenu->addMenu(ospMaterials);
	ospMaterials->addAction(createAddNodeAction("Principled", ospmat::principledID));

	ospMenu->addMenu(ospTextures);
	ospTextures->addAction(createAddNodeAction("Image Texture", ospmat::imageTextureID));

}

void DfxShaderEditor::styleMenuButtons() {

	QString menuButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"padding-left: 2px;"
		"height: 13px;"
		"}";

	QString hoverSty =
		"QPushButton:hover { "
		"background-color: rgb(100,100,100);"
		"}";

	QString hoverPressdSty =
		"QPushButton:hover:pressed { "
		"background-color: rgb(255, 153, 83);"
		"}";

	QString newMatButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"border-bottom-left-radius: 4px;"
		"border-top-left-radius: 4px;"
		"padding-left: 2px;"
		"height: 13px;"
		"}";

	QString deleteMatButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"border-bottom-right-radius: 4px;"
		"border-top-right-radius: 4px;"
		"padding-left: 0px;"
		"height: 13px;"
		"}";

	QString materialHeaderSty =
		"QLabel { "
		"color: white;"
		"background-color: rgb(20,20,20);"
		"border: 0px solid;"
		"padding-left: 0px;"
		"height: 18px;"
		"}";

	addNodeMenuBtn->setStyleSheet(menuButtonSty + hoverSty + hoverPressdSty);
	editNodeMenuBtn->setStyleSheet(menuButtonSty + hoverSty + hoverPressdSty);
	viewMenuBtn->setStyleSheet(menuButtonSty + hoverSty + hoverPressdSty);
	layoutMenuBtn->setStyleSheet(menuButtonSty + hoverSty + hoverPressdSty);
	newMaterial->setStyleSheet(newMatButtonSty + hoverSty + hoverPressdSty);
	deleteMaterial->setStyleSheet(deleteMatButtonSty + hoverSty + hoverPressdSty);
	materialsHeader->setStyleSheet(materialHeaderSty);
}

void DfxShaderEditor::matetrialViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
	auto indexes = selected.indexes();
	propertiesView->clear();
	if (!indexes.empty()) {
		auto space = DfxSpace::getInstance();

		auto any = space->findAbsolute("~/MATS");

		if (any != nullptr) {
			if (nodeCollection != nullptr) {
				nodeView->setScene(nullptr);
				nodeCollection->deleteAllLinks();
				delete nodeCollection;
				nodeCollection = nullptr;
			}

			auto matAny = any->data<fl::DataObject>().properties()[indexes[0].row()];

			nodeCollection = new DfxQMaterialNodeCollection;
			nodeCollection->installEventFilter(nodeView);
			nodeCollection->setMaterialPath(space->absolutePath(matAny));
			nodeView->setScene(nodeCollection);

			DfxQMaterialNode *rootNode = new DfxQMaterialNode;
			rootNode->buildNode(matAny);
			rootNode->setLabel("Material Output");
			rootNode->setHeaderColor(QColor(0, 85, 0));
			rootNode->setDataObjectPath(space->absolutePath(matAny));

			nodeCollection->addNode(rootNode);

			for (auto p : matAny->data<fl::DataObject>().properties()) {
				if (p->type() == fl::type::DATA_OBJECT) {

					if (p->data<fl::DataObject>().hasProperty("render_target")) {
						addNodeToCollection(p);
					}
				}
			}

			nodeCollection->constructLinks();
		}
	}
}

void DfxShaderEditor::newMaterialInserted(const QModelIndex &parent, int first, int last) {

	if (!materialsView->selectionModel()->currentIndex().isValid()) {
		materialsView->setCurrentIndex(materialsView->model()->index(0,0));
	}
}

size_t DfxShaderEditor::addNodeToCollection(fl::Any *any) {

	if (nodeCollection != nullptr) {
		auto space = DfxSpace::getInstance();

		DfxQMaterialNode *nnode = new DfxQMaterialNode;
		nnode->buildNode(any);
		nnode->setHeaderColor(QColor(251, 101, 57));
		nnode->setDataObjectPath(space->absolutePath(any));

		nodeCollection->addNode(nnode);

		return nnode->nodeID();
	}

	return 0;
}

QAction *DfxShaderEditor::createAddNodeAction(QString actionLabel, std::string mat_name) {
	QAction *action = nullptr;
	
	auto response = [this, mat_name]() {

		if (nodeCollection != nullptr) {
			auto space = DfxSpace::getInstance();
			auto any = space->findAbsolute(nodeCollection->materialPath());

			if (any != nullptr) {

				auto anyMatNode = DfxMaterialManager::getInstance()->createMaterialInstance(mat_name);

				if (anyMatNode != nullptr) {
					space->move(anyMatNode, any);
					addNodeToCollection(anyMatNode);
				}
			}
		}
	};

	action = new QAction(actionLabel);
	QObject::connect(action, &QAction::triggered, response);

	return action;
}

void DfxShaderEditor::nodesSelected(std::vector<DfxQMaterialBaseNode*> nodes) {

	if(!nodes.empty()) {
		propertiesView->newNodePropertiesForm((DfxQMaterialNode*)nodes[0]);
	}
	else {
		propertiesView->clear();
	}
}

void DfxShaderEditor::resetSelectedMaterial() {

	materialsView->setModel(nullptr);
	materialsView->setModel(DfxMaterialManager::getInstance()->materialModel());
	QObject::connect(materialsView->model(), &QAbstractItemModel::rowsInserted, this, &DfxShaderEditor::newMaterialInserted);
	QObject::connect(materialsView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DfxShaderEditor::matetrialViewSelectionChanged);

	if (!materialsView->selectionModel()->currentIndex().isValid()) {
		if (materialsView->model()->index(0, 0).isValid()) {
			materialsView->setCurrentIndex(materialsView->model()->index(0, 0));
		}
		else {
			if (nodeCollection != nullptr) {
				nodeView->setScene(nullptr);
				nodeCollection->deleteAllLinks();
				delete nodeCollection;
				nodeCollection = nullptr;
			}
		}
	}
}

void DfxShaderEditor::removeSelectedMaterial() {

	if (materialsView->selectionModel()->currentIndex().isValid()) {
		auto any = DfxSpace::getInstance()->findAbsolute("~/MATS");

		if (materialsView->selectionModel()->currentIndex().row() < any->data<fl::DataObject>().properties().size()) {
			auto matAny = any->data<fl::DataObject>().properties()[materialsView->selectionModel()->currentIndex().row()];
			DfxMaterialManager::getInstance()->removeMaterial(matAny->absolutePath());

			resetSelectedMaterial();
			
			DfxUpdater::getInstance()->materialUpdated();
		}
	}
}

void DfxShaderEditor::cutSelectedNodes() {

	_copyBuffer.clear();
	_cutBuffer.clear();
}

void DfxShaderEditor::copySelectedNodes() {

	_copyBuffer.clear();
	_cutBuffer.clear();
	std::string selectedMaterial = "";

	if (materialsView->selectionModel()->currentIndex().isValid()) {
		auto any = DfxSpace::getInstance()->findAbsolute("~/MATS");

		if (materialsView->selectionModel()->currentIndex().row() < any->data<fl::DataObject>().properties().size()) {
			auto matAny = any->data<fl::DataObject>().properties()[materialsView->selectionModel()->currentIndex().row()];
			
			selectedMaterial = matAny->absolutePath();
		}
		else {
			return;
		}
	}
	else {
		return;
	}

	if (nodeCollection != nullptr) {
		for (auto node : nodeCollection->selectedItems()) {

			if (node->type() == DfxQMaterialNodeEnums::BASE_NODE) {
				auto bnode = (DfxQMaterialNode*)node;
				_copyBuffer.push_back(std::pair<std::string, std::string>(selectedMaterial, bnode->dataObjectPath()));
			}
		}
	}
	
}

void DfxShaderEditor::pasteNodes() {

	if (nodeCollection != nullptr) {

		if (!_copyBuffer.empty()) {

			for (auto cp : _copyBuffer) {
				auto any = DfxSpace::getInstance()->findAbsolute(cp.second);
				auto collectionAny = DfxSpace::getInstance()->findAbsolute(nodeCollection->materialPath());

				if (any != nullptr) {
					if (any->type() == fl::type::DATA_OBJECT) {
						auto dao = any->rawData<fl::DataObject>();

						if (dao->modelReference() != "LUX_OUTPUT") {

							auto anyMatNode = DfxMaterialManager::getInstance()->createMaterialInstance(dao->modelReference());

							if (anyMatNode != nullptr) {

								auto matDao = anyMatNode->rawData<fl::DataObject>();

								util::copyDataObjectProperties(*dao, *anyMatNode->rawData<fl::DataObject>());
								DfxSpace::getInstance()->move(anyMatNode, collectionAny);

								auto xpos = matDao->property("xpos")->data<float>();
								auto ypos = matDao->property("ypos")->data<float>();

								matDao->property("xpos")->setData<float>(xpos + -100.f);
								matDao->property("ypos")->setData<float>(ypos + -100.f);

								addNodeToCollection(anyMatNode);
							}
						}
					}
				}
			}

		}
		else if (!_cutBuffer.empty()) {
			
		}

	}
}

void DfxShaderEditor::viewAll() {
	nodeView->viewFitAll();
}

bool DfxShaderEditor::eventFilter(QObject *watched, QEvent *event) {

	if (event->type() == QEvent::KeyPress) {

		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		addKey(keyEvent->key());

		if (keys().size() > 0) {
			auto shrtc = keysToShortcut();

			if (DfxShortcutsDB::getInstance()->match("Material Editor", "Copy Nodes", DfxShortcutsDB::KEYBOARD, shrtc.k00, shrtc.k01, shrtc.k02, shrtc.k03, shrtc.mouseButton)) {
				copySelectedNodes();
				return true;
			}
			else if (DfxShortcutsDB::getInstance()->match("Material Editor", "Paste Nodes", DfxShortcutsDB::KEYBOARD, shrtc.k00, shrtc.k01, shrtc.k02, shrtc.k03, shrtc.mouseButton)) {
				pasteNodes();
				return true;
			}
		}

	}
	else if (event->type() == QEvent::KeyRelease) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		removeKey(filter(keyEvent->key()));
	}

	return QMainWindow::eventFilter(watched, event);
}

void DfxShaderEditor::reset() {
	resetSelectedMaterial();
}
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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include <FlAnyMap.h>
#include "DfxChannelGraphView.h"
#include "DfxGraphEditorManager.h"
#include "DfxGraphEditor.h"
#include "DfxQPushButtonStyles.h"

DfxGraphEditor::DfxGraphEditor() {

	int icon_btn_sz = 28;
	int icon_sz = 18;

	_mainLayout = new QVBoxLayout;
	_toolbarLayout = new QHBoxLayout;

	_frame = new QSpinBox;
	_value = new QDoubleSpinBox;

	_manipulator = new QPushButton(QPixmap(":/icon/graph_editor_manipulator_icon"), "");
	_createKey = new QPushButton(QPixmap(":/icon/graph_editor_add_key_icon"), "");
	_removeKey = new QPushButton(QPixmap(":/icon/graph_editor_remove_key_icon"), "");

	_steppedInterpolation = new QPushButton(QPixmap(":/icon/graph_editor_stepped_icon"), "");
	_linearInterpolation  = new QPushButton(QPixmap(":/icon/graph_editor_linear_icon"), "");
	_beizerInterpolation = new QPushButton(QPixmap(":/icon/graph_editor_beizer_icon"), "");
	_resetTangent  = new QPushButton(QPixmap(":/icon/graph_editor_reset_tangent_icon"), "");

	_mainSplitter = new QSplitter;
	_graphView = new DfxChannelGraphView;
	_channels  = new QTreeView;

	_toolbarLayout->setAlignment(Qt::AlignLeft);

	_frame->setRange(0, std::numeric_limits<int>::max());
	_frame->setAlignment(Qt::AlignCenter);

	_value->setRange(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	_value->setDecimals(6);
	_value->setAlignment(Qt::AlignCenter);

	_frame->setFixedWidth(100);
	_value->setFixedWidth(100);

	_manipulator->setFixedSize(QSize(icon_btn_sz, icon_btn_sz));
	_manipulator->setIconSize(QSize(icon_sz, icon_sz));

	_createKey->setFixedSize(QSize(icon_btn_sz, icon_btn_sz));
	_createKey->setIconSize(QSize(icon_sz, icon_sz));

	_removeKey->setFixedSize(QSize(icon_btn_sz, icon_btn_sz));
	_removeKey->setIconSize(QSize(icon_sz, icon_sz));

	_steppedInterpolation->setFixedSize(QSize(icon_btn_sz, icon_btn_sz));
	_steppedInterpolation->setIconSize(QSize(icon_sz, icon_sz));

	_linearInterpolation->setFixedSize(QSize(icon_btn_sz, icon_btn_sz));
	_linearInterpolation->setIconSize(QSize(icon_sz, icon_sz));

	_beizerInterpolation->setFixedSize(QSize(icon_btn_sz, icon_btn_sz));
	_beizerInterpolation->setIconSize(QSize(icon_sz, icon_sz));

	_resetTangent->setFixedSize(QSize(icon_btn_sz, icon_btn_sz));
	_resetTangent->setIconSize(QSize(icon_sz, icon_sz));

	_channels->setModel(DfxGraphEditorManager::getInstance()->model());
	_channels->setSelectionMode(QAbstractItemView::MultiSelection);

	//_toolbarLayout->addWidget(_frame);
	//_toolbarLayout->addWidget(_value);
	//_toolbarLayout->addWidget(new Seperator());
	_toolbarLayout->addWidget(_manipulator);
	_toolbarLayout->addWidget(_createKey);
	_toolbarLayout->addWidget(_removeKey);
	_toolbarLayout->addWidget(new Seperator());
	_toolbarLayout->addWidget(_steppedInterpolation);
	_toolbarLayout->addWidget(_linearInterpolation);
	_toolbarLayout->addWidget(_beizerInterpolation);
	_toolbarLayout->addWidget(new Seperator());
	_toolbarLayout->addWidget(_resetTangent);

	styleButton(_manipulator, true);
	styleButton(_createKey, true);
	styleButton(_removeKey, true);
	styleButton(_steppedInterpolation);
	styleButton(_linearInterpolation);
	styleButton(_beizerInterpolation);
	styleButton(_resetTangent);

	_mainSplitter->addWidget(_channels);
	_mainSplitter->addWidget(_graphView);
	_mainSplitter->setSizes({200, 1000});

	_mainLayout->addLayout(_toolbarLayout);
	_mainLayout->addWidget(_mainSplitter);

	_manipulator->setAutoExclusive(true);
	_createKey->setAutoExclusive(true);
	_removeKey->setAutoExclusive(true);

	_manipulator->setCheckable(true);
	_createKey->setCheckable(true);
	_removeKey->setCheckable(true);

	_manipulator->setChecked(true);

	setLayout(_mainLayout);

	styleSpinners();

	QObject::connect(DfxGraphEditorManager::getInstance(), &DfxGraphEditorManager::modelUpdated, this, &DfxGraphEditor::expandAllTree);
	QObject::connect(DfxGraphEditorManager::getInstance(), &DfxGraphEditorManager::modelUpdated, this, &DfxGraphEditor::channelModelChanged);
	QObject::connect(DfxGraphEditorManager::getInstance(), &DfxGraphEditorManager::newChannelCreated, this, &DfxGraphEditor::newChannelSelection);
	QObject::connect(_channels->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DfxGraphEditor::channelSelectionChanged);
	QObject::connect(this, &DfxGraphEditor::selectionChanged, _graphView, &DfxChannelGraphView::updateCurves);

	QObject::connect(_steppedInterpolation, &QPushButton::clicked, this, &DfxGraphEditor::useConstantInterpolation);
	QObject::connect(_linearInterpolation, &QPushButton::clicked, this, &DfxGraphEditor::useLinearInterpolation);
	QObject::connect(_beizerInterpolation, &QPushButton::clicked, this, &DfxGraphEditor::useBeizerInterpolation);
	QObject::connect(_resetTangent, &QPushButton::clicked, _graphView, &DfxChannelGraphView::resetTangents);

	QObject::connect(_manipulator, &QPushButton::clicked, this, &DfxGraphEditor::switchToSelectionMode);
	QObject::connect(_createKey, &QPushButton::clicked, this, &DfxGraphEditor::switchToAddKeyMode);
	QObject::connect(_removeKey, &QPushButton::clicked, this, &DfxGraphEditor::switchToRemoveKeyMode);
}

DfxGraphEditor::~DfxGraphEditor() {

}

DfxGraphEditor::Seperator::Seperator() {
	setFrameShape(QFrame::VLine);
	setFrameShadow(QFrame::Plain);
	setLineWidth(0);
	setMidLineWidth(2);
	setContentsMargins(0, 0, 0, 0);
	setStyleSheet("border:1px solid rgba(255,255,255, 0); background-color: rgba(255,255,255, 20);");
}

void DfxGraphEditor::styleButton(QPushButton *pbutton, bool checkedCss) {

	pbutton->setStyleSheet(pbutton->styleSheet() + style::buttonSty() + style::hoverSty() + style::hoverPressdSty());

	if (checkedCss) {
		pbutton->setStyleSheet(pbutton->styleSheet() +
			"QPushButton:checked{"
			"background-color: rgb(0, 150, 0);"
			"}");
	}
}

void DfxGraphEditor::expandAllTree() {
	_channels->expandAll();
}

void DfxGraphEditor::channelSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
	
	std::vector<std::pair<std::string, QColor>> paths;
	for (auto &item : _channels->selectionModel()->selectedIndexes()) {
		if (item.isValid()) {
			auto channel = static_cast<DfxGraphEditorChannel*>(item.internalPointer());

			if (channel->type() == DfxGraphEditorChannel::ANY) {
				paths.push_back(std::pair<std::string, QColor>(channel->path(), channel->color()));
			}
		}
	}
	emit selectionChanged(paths);
}

void DfxGraphEditor::channelModelChanged() {
	std::vector<std::pair<std::string, QColor>> paths;
	emit selectionChanged(paths);
}

void DfxGraphEditor::newChannelSelection() {
	/*std::vector<std::pair<std::string, QColor>> paths;
	for (auto &item : _channels->selectionModel()->selectedIndexes()) {
		if (item.isValid()) {
			auto channel = static_cast<DfxGraphEditorChannel*>(item.internalPointer());

			if (channel->type() == DfxGraphEditorChannel::ANY) {
				paths.push_back(std::pair<std::string, QColor>(channel->path(), channel->color()));
			}
		}
	}
	emit selectionChanged(paths);*/
}

void DfxGraphEditor::styleSpinners() {

	_value->setStyleSheet(
		"QDoubleSpinBox { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"border-radius: 5px;"
		"padding-left: 2px;"
		"padding-top: 2px;"
		"}");

	_value->setStyleSheet(_value->styleSheet() +
		"QDoubleSpinBox:hover { "
		"border-width: 1px;"
		"border-style: solid;"
		"border-color: rgb(255,127,42);"
		"}");

	_value->setStyleSheet(_value->styleSheet() +
		"QDoubleSpinBox:focus { "
		"border-width: 1px;"
		"border-style: solid;"
		"border-color: rgb(255,127,42);"
		"}");

	_value->setStyleSheet(_value->styleSheet() +
		"QDoubleSpinBox::down-button {"
		"subcontrol-origin: margin;"
		"subcontrol-position: center left;"
		"image: url(:/icon/spinner_left);"
		"padding-left: 6px;"
		"height: 8px;"
		"width: 5px;"
		"}");

	_value->setStyleSheet(_value->styleSheet() +
		"QDoubleSpinBox::up-button {"
		"subcontrol-origin: margin;"
		"subcontrol-position: center right;"
		"image: url(:/icon/spinner_right);"
		"padding-right: 6px;"
		"height: 8px;"
		"width: 5px;"
		"}");

	// 
	_frame->setStyleSheet(
		"QSpinBox { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"border-radius: 5px;"
		"padding-left: 2px;"
		"}");

	_frame->setStyleSheet(_frame->styleSheet() +
		"QSpinBox:hover { "
		"border-width: 1px;"
		"border-style: solid;"
		"border-color: rgb(255,127,42);"
		"}");

	_frame->setStyleSheet(_frame->styleSheet() +
		"QSpinBox:focus { "
		"border-width: 1px;"
		"border-style: solid;"
		"border-color: rgb(255,127,42);"
		"}");

	_frame->setStyleSheet(_frame->styleSheet() +
		"QSpinBox::down-button {"
		"subcontrol-origin: margin;"
		"subcontrol-position: center left;"
		"image: url(:/icon/spinner_left);"
		"padding-left: 6px;"
		"height: 8px;"
		"width: 5px;"
		"}");

	_frame->setStyleSheet(_frame->styleSheet() +
		"QSpinBox::up-button {"
		"subcontrol-origin: margin;"
		"subcontrol-position: center right;"
		"image: url(:/icon/spinner_right);"
		"padding-right: 6px;"
		"height: 8px;"
		"width: 5px;"
		"}");
}

void DfxGraphEditor::useConstantInterpolation() {
	_graphView->updateInterpolationType(fl::InterpolationType::CONSTANT);
}

void DfxGraphEditor::useLinearInterpolation() {
	_graphView->updateInterpolationType(fl::InterpolationType::LINEAR);
}

void DfxGraphEditor::useBeizerInterpolation() {
	_graphView->updateInterpolationType(fl::InterpolationType::BEZIER);
}

void DfxGraphEditor::uncheckButtons() {
	_manipulator->setChecked(false);
	_createKey->setChecked(false);
}

void DfxGraphEditor::switchToSelectionMode() {
	_graphView->switchMode(DfxChannelGraphView::SEL_MODE);
}

void DfxGraphEditor::switchToAddKeyMode() {
	_graphView->switchMode(DfxChannelGraphView::ADD_KEY_MODE);
}

void DfxGraphEditor::switchToRemoveKeyMode() {
	_graphView->switchMode(DfxChannelGraphView::REMOVE_KEY_MODE);
}

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

#include "DfxQFlowProcessForm.h"
#include "DfxQStyleLineEdit.h"
#include "DfxQStyleListView.h"
#include "DfxQPushButtonStyles.h"
#include "DfxComputeHandler.h"
#include "DfxQFlowProcessNode.h"
#include "DfxQFluentEditorCrumb.h"
#include "DfxQFluentNodeEditor.h"

DfxQFlowProcessForm::~DfxQFlowProcessForm() {

}

void DfxQFlowProcessForm::create(DfxQFlowProcessNode *pnode, int fid, fl::Flow *fnode) {
	_flow = fnode;
	_fid  = fid;
	_pnode = pnode;

	QVBoxLayout *main_layout = new QVBoxLayout;

	auto namePanel = new DfxControlPanel();
	auto processListPanel = new DfxControlPanel();

	auto listWidget = new QWidget();
	auto btnLayout  = new QHBoxLayout();
	auto listLayout = new QVBoxLayout();

	DfxQControlArray *nameControlArray = new DfxQControlArray();

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto name = new QLineEdit();
	processList = new QListWidget();
	newProcess  = new QPushButton("New");
	renameProcess   = new QPushButton(QPixmap(":/icon/rename_icon").scaledToHeight(12), "");
	moveProcessUp   = new QPushButton(QPixmap(":/icon/triangle_arrow_up").scaledToHeight(8), "");
	moveProcessDown = new QPushButton(QPixmap(":/icon/triangle_arrow_down").scaledToHeight(8), "");
	removeProcess   = new QPushButton(QPixmap(":/icon/view_close").scaledToHeight(8), "");

	newProcess->setToolTip("Add New Process");
	renameProcess->setToolTip("Rename Process");
	moveProcessUp->setToolTip("Move Up");
	moveProcessDown->setToolTip("Move Down");
	removeProcess->setToolTip("Remove Process");

	newProcess->setStyleSheet(style::buttonSty() + style::hoverPressdSty() + style::hoverSty());
	renameProcess->setStyleSheet(style::buttonSty() + style::hoverPressdSty() + style::hoverSty());
	moveProcessUp->setStyleSheet(style::buttonSty() + style::hoverPressdSty() + style::hoverSty());
	moveProcessDown->setStyleSheet(style::buttonSty() + style::hoverPressdSty() + style::hoverSty());
	removeProcess->setStyleSheet(style::rightButtonSty() + style::hoverPressdSty() + style::hoverSty());

	style::styleQLineEdit(name);
	style::styleQListView(processList);

	processList->setAlternatingRowColors(true);

	processList->setFixedHeight(200);
	renameProcess->setFixedWidth(20);
	removeProcess->setFixedWidth(20);
	moveProcessUp->setFixedWidth(20);
	moveProcessDown->setFixedWidth(20);

	btnLayout->addWidget(newProcess);
	btnLayout->addWidget(renameProcess);
	btnLayout->addWidget(moveProcessUp);
	btnLayout->addWidget(moveProcessDown);
	btnLayout->addWidget(removeProcess);

	listLayout->addLayout(btnLayout);
	listLayout->addWidget(processList);

	listLayout->setAlignment(Qt::AlignTop);
	listLayout->setSpacing(1);
	listWidget->setLayout(listLayout);

	nameControlArray->addControl("Description:", name);

	namePanel->setTitle("Name");
	namePanel->setWidget(nameControlArray);

	processListPanel->setTitle("Processes");
	processListPanel->setWidget(listWidget);

	main_layout->addWidget(namePanel);
	main_layout->addWidget(processListPanel);

	setLayout(main_layout);

	QObject::connect(name, &QLineEdit::textChanged, this, &DfxQFlowProcessForm::nameChanged);

	QObject::connect(newProcess, &QPushButton::clicked, this, &DfxQFlowProcessForm::addNewProcess);
	QObject::connect(moveProcessUp, &QPushButton::clicked, this, &DfxQFlowProcessForm::moveExisitngProcessUp);
	QObject::connect(moveProcessDown, &QPushButton::clicked, this, &DfxQFlowProcessForm::moveExisitngProcessDown);
	QObject::connect(removeProcess, &QPushButton::clicked, this, &DfxQFlowProcessForm::removeSelectedProcess);
	QObject::connect(renameProcess, &QPushButton::clicked, this, &DfxQFlowProcessForm::renameSelectedProcess);

	QObject::connect(processList, &QListWidget::itemDoubleClicked, this, &DfxQFlowProcessForm::editComputeGraph);

	name->setText(_flow->node(fid)->text().c_str());
	restoreList();
}

void DfxQFlowProcessForm::clearList() {
	processList->clear();
}

void DfxQFlowProcessForm::restoreList() {
	clearList();

	int counter = 0;
	for (auto path : _flow->node(_fid)->paths()) {

		auto any = DfxSpace::getInstance()->findAbsolute(path);

		if (any != nullptr) {
			QListWidgetItem *newItem = new QListWidgetItem;
			newItem->setText(any->label().c_str());
			processList->insertItem(counter, newItem);

			counter++;
		}
	}
}

void DfxQFlowProcessForm::destroy() {

}

void DfxQFlowProcessForm::addNewProcess() {
	bool ok;
	QString text = QInputDialog::getText(nullptr, "New Process", "Description:", QLineEdit::Normal, "< New Process >", &ok);

	if (ok && !text.isEmpty()) {
		DfxComputeHandler handler;

		if (handler.create(text.toStdString())) {
			_flow->addProcessAction(_fid, handler.any()->absolutePath());
			restoreList();
		}
		else {
			DfxGlobal::console::println("Failed to add new compute graph process.");
		}
	}
}

void DfxQFlowProcessForm::moveExisitngProcessUp() {
	auto index = processList->selectionModel()->currentIndex();
	
	if (index.row() != -1) {
		int next = index.row() - 1;

		if (next >= 0) {
			auto fnode = _flow->node(_fid);
			std::swap(fnode->paths()[index.row()], fnode->paths()[next]);
			
			restoreList();

			auto current_index = processList->model()->index(next, 0);
			processList->selectionModel()->setCurrentIndex(current_index, QItemSelectionModel::SelectCurrent);
		}
	}
}

void DfxQFlowProcessForm::moveExisitngProcessDown() {
	auto index = processList->selectionModel()->currentIndex();

	if (index.row() != -1) {
		int next = index.row() + 1;

		if (next < processList->model()->rowCount()) {
			auto fnode = _flow->node(_fid);
			std::swap(fnode->paths()[index.row()], fnode->paths()[next]);

			restoreList();

			auto current_index = processList->model()->index(next, 0);
			processList->selectionModel()->setCurrentIndex(current_index, QItemSelectionModel::SelectCurrent);
		}
	}
}

void DfxQFlowProcessForm::removeSelectedProcess() {
	auto index = processList->selectionModel()->currentIndex();

	if (index.row() != -1) {
		auto fnode = _flow->node(_fid);
		auto path  = fnode->paths()[index.row()];

		DfxComputeHandler handle;

		if (handle.fetch(path)) {
			handle.destroy();

			_flow->removeProcessAction(_fid, path);

			restoreList();

			if (!fnode->paths().empty()) {
				auto current_index = processList->model()->index(0, 0);
				processList->selectionModel()->setCurrentIndex(current_index, QItemSelectionModel::SelectCurrent);
			}
		}
	}
}

void DfxQFlowProcessForm::renameSelectedProcess() {
	auto index = processList->selectionModel()->currentIndex();

	if (index.row() != -1) {
		auto fnode = _flow->node(_fid);
		auto any = DfxSpace::getInstance()->findAbsolute(fnode->paths()[index.row()]);

		if (any != nullptr) {
			bool ok;
			QString text = QInputDialog::getText(nullptr, "New Process", "Description:", QLineEdit::Normal, any->label().c_str(), &ok);

			if (ok && !text.isEmpty()) {
				any->setLabel(text.toStdString());
				restoreList();
			}
		}
	}
}

void DfxQFlowProcessForm::nameChanged(const QString &text) {

	auto fnode = _flow->node(_fid);
	fnode->setText(text.toStdString());
	_pnode->setLabel(text);
}

DfxFluentNodeEditor *DfxQFlowProcessForm::editor() {
	return _editor;
}

void DfxQFlowProcessForm::setEditor(DfxFluentNodeEditor *editor) {
	_editor = editor;
}

void DfxQFlowProcessForm::editComputeGraph(QListWidgetItem *item) {
	auto index = processList->selectionModel()->currentIndex();

	if (index.row() != -1) {
		auto fnode = _flow->node(_fid);
		auto path = fnode->paths()[index.row()];

		DfxQFluentEditorCrumb crumb;
		crumb.type = QFluentViewType::NODE;
		crumb.path = path;
		_editor->pushView(crumb);

	}
}
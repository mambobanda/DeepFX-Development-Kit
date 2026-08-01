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

#include "DfxQFlowDecisionForm.h"
#include "DfxQStyleLineEdit.h"
#include "DfxQStyleListView.h"
#include "DfxQPushButtonStyles.h"
#include "DfxQFlowDecisionNode.h"
#include "DfxComputeHandler.h"
#include "DfxQFluentEditorCrumb.h"
#include "DfxQFluentNodeEditor.h"

DfxQFlowDecisionForm::~DfxQFlowDecisionForm() {

}

void DfxQFlowDecisionForm::create(DfxQFlowDecisionNode *dnode, int fid, fl::Flow *fnode) {
	_flow = fnode;
	_fid = fid;
	_dnode = dnode;

	QVBoxLayout *main_layout = new QVBoxLayout;

	auto namePanel = new DfxControlPanel();
	auto processListPanel = new DfxControlPanel();

	auto listWidget = new QWidget();
	auto btnLayout = new QHBoxLayout();
	auto listLayout = new QVBoxLayout();

	DfxQControlArray *nameControlArray = new DfxQControlArray();

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto name = new QLineEdit();
	processList = new QListWidget();
	newProcess = new QPushButton("New");
	removeProcess = new QPushButton(QPixmap(":/icon/view_close").scaledToHeight(8), "");

	newProcess->setStyleSheet(style::buttonSty() + style::hoverPressdSty() + style::hoverSty());
	removeProcess->setStyleSheet(style::rightButtonSty() + style::hoverPressdSty() + style::hoverSty());

	style::styleQLineEdit(name);
	style::styleQListView(processList);

	processList->setFixedHeight(200);
	removeProcess->setFixedWidth(20);

	btnLayout->addWidget(newProcess);
	btnLayout->addWidget(removeProcess);

	listLayout->addLayout(btnLayout);
	listLayout->addWidget(processList);

	listLayout->setAlignment(Qt::AlignTop);
	listLayout->setSpacing(1);
	listWidget->setLayout(listLayout);

	nameControlArray->addControl("Description:", name);

	namePanel->setTitle("Name");
	namePanel->setWidget(nameControlArray);

	processListPanel->setTitle("Condition");
	processListPanel->setWidget(listWidget);

	main_layout->addWidget(namePanel);
	main_layout->addWidget(processListPanel);

	name->setText(_flow->node(fid)->text().c_str());
	setLayout(main_layout);

	QObject::connect(name, &QLineEdit::textChanged, this, &DfxQFlowDecisionForm::nameChanged);

	QObject::connect(newProcess, &QPushButton::clicked, this, &DfxQFlowDecisionForm::addNewProcess);
	QObject::connect(removeProcess, &QPushButton::clicked, this, &DfxQFlowDecisionForm::removeSelectedProcess);

	QObject::connect(processList, &QListWidget::itemDoubleClicked, this, &DfxQFlowDecisionForm::editComputeGraph);

	restoreList();
}

void DfxQFlowDecisionForm::destroy() {

}

void DfxQFlowDecisionForm::nameChanged(const QString &text) {
	auto fnode = _flow->node(_fid);
	fnode->setText(text.toStdString());
	_dnode->setLabel(text);
}

DfxFluentNodeEditor *DfxQFlowDecisionForm::editor() {
	return _editor;
}

void DfxQFlowDecisionForm::setEditor(DfxFluentNodeEditor *editor) {
	_editor = editor;
}

void DfxQFlowDecisionForm::addNewProcess() {

	if (_flow->node(_fid)->conditionPath().empty()) {
		DfxComputeHandler handler;

		if (handler.create("Condition Process")) {

			auto any = _flow->decisionOutputNode(DfxSpace::getInstance());
			DfxSpace::getInstance()->move(any, handler.any());

			_flow->setDecisionOuputNodePath(_fid, any->absolutePath());
			_flow->setDecisionConditionPath(_fid, handler.any()->absolutePath());

			restoreList();
		}
		else {
			DfxGlobal::console::println("Failed to add new compute graph process.");
		}
	}
	else {
		QMessageBox msgBox;
		msgBox.setText("Only one process is allowed for decisions.");
		msgBox.exec();
	}
}

void DfxQFlowDecisionForm::removeSelectedProcess() {
	auto index = processList->selectionModel()->currentIndex();


	if (!_flow->node(_fid)->conditionPath().empty()) {

		DfxComputeHandler handle;

		if (handle.fetch(_flow->node(_fid)->conditionPath())) {
			handle.destroy();
			_flow->setDecisionConditionPath(_fid, "");
		}
	}

	clearList();
	restoreList();
}

void DfxQFlowDecisionForm::editComputeGraph(QListWidgetItem *item) {
	

	if (!_flow->node(_fid)->conditionPath().empty()) {
		auto fnode = _flow->node(_fid);
		auto path = _flow->node(_fid)->conditionPath();

		DfxQFluentEditorCrumb crumb;
		crumb.type = QFluentViewType::NODE;
		crumb.path = path;
		_editor->pushView(crumb);

	}
}

void DfxQFlowDecisionForm::clearList() {
	processList->clear();
}

void DfxQFlowDecisionForm::restoreList() {
	clearList();

	if (!_flow->node(_fid)->conditionPath().empty()) {

		auto any = DfxSpace::getInstance()->findAbsolute(_flow->node(_fid)->conditionPath());

		if (any != nullptr) {
			QListWidgetItem *newItem = new QListWidgetItem;
			newItem->setText(any->label().c_str());
			processList->insertItem(0, newItem);
		}
	}
	
}
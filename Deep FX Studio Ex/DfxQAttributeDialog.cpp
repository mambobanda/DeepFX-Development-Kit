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


#include "DfxQAttributeDialog.h"


DfxQAttributeDialog::DfxQAttributeDialog() {
	
	mainLayout = new QVBoxLayout();
	_tree = new QTreeWidget();

	mainLayout->setContentsMargins(2, 0, 2, 2);
	mainLayout->setSpacing(2);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
	QPushButton  *ok     = new QPushButton("Ok");
	QPushButton  *cancel = new QPushButton("Cancel");

	styleButtons(ok);
	styleButtons(cancel);

	_attributePath = "";

	style::styleQScrollBar(_tree->horizontalScrollBar());
	style::styleQScrollBar(_tree->verticalScrollBar());

	_tree->setHeaderLabel("");
	_tree->setAlternatingRowColors(true);
	//styleTree();

	setWindowTitle("Attributes");

	buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);
	buttonBox->addButton(cancel, QDialogButtonBox::RejectRole);

	mainLayout->addWidget(_tree);
	mainLayout->addWidget(buttonBox);

	setLayout(mainLayout);

	populateTree();

	QObject::connect(ok, &QPushButton::clicked, this, &DfxQAttributeDialog::accept);
	QObject::connect(cancel, &QPushButton::clicked, this, &DfxQAttributeDialog::reject);
	QObject::connect(this, &DfxQAttributeDialog::accepted, this, &DfxQAttributeDialog::okBtnCkicked);
}

DfxQAttributeDialog::~DfxQAttributeDialog() {

}

std::string DfxQAttributeDialog::attributePath() {
	return _attributePath;
}

void DfxQAttributeDialog::populateTree() {

	auto attributes_list = DfxAttributeTree::getInstance()->attributes();

	if (!attributes_list.empty()) {

		QTreeWidgetItem *root = new QTreeWidgetItem();
		root->setText(0, "");

		_tree->addTopLevelItem(root);

		for (auto attr : DfxAttributeTree::getInstance()->attributes()) {
			root->addChild(addAttribute(attr));
		}

		_tree->setExpanded(_tree->model()->index(0, 0), true);
	}


}

QTreeWidgetItem *DfxQAttributeDialog::addAttribute(DfxAttribute *attr) {

	QTreeWidgetItem *item = new QTreeWidgetItem();
	item->setText(0, attr->name().c_str());
	item->setData(0, Qt::UserRole, QString(attr->attributePath().c_str()));

	for (auto atrr_chd : attr->attributes()) {
		auto chd_item = addAttribute(atrr_chd);
		item->addChild(chd_item);
	}

	return item;
}

void DfxQAttributeDialog::okBtnCkicked() {
	auto selected = _tree->selectedItems();

	if (!selected.empty()) {
		_attributePath = selected[0]->data(0, Qt::UserRole).toString().toStdString();
	}
}

void DfxQAttributeDialog::styleTree() {
	_tree->setStyleSheet(
		"QTreeView{ "
		"color: white;"
		"background-color: rgb(20,20,20);"
		"border: 0px solid;"
		"border-color: rgb(50,50,50);"
		"border-width: 2px;"
		"border-style: solid;"
		"border-radius: 4px;"
		"padding-left: 5px;"
		"}");

	_tree->setStyleSheet(_tree->styleSheet() +
		"QTreeView:hover { "
		"border-width: 1px;"
		"border-style: solid;"
		"border-color: rgb(255,127,42);"
		"}");

	_tree->setStyleSheet(_tree->styleSheet() +
		"QTreeView::down-arrow{"
		"image: url(:/icon/triangle_arrow_down);"
		"width: 7px;"
		"height: 7px;"
		"}");

	_tree->setStyleSheet(_tree->styleSheet() +
		"QTreeView::drop-down{"
		"border-width: 0px;"
		"}");

	_tree->setStyleSheet(_tree->styleSheet() +
		"QTreeView:item:selected{"
		"color: black;"
		"padding-left: 20px;"
		"font-weight: bold; "
		"background: rgb(255,127,42);"
		"}");
}

void DfxQAttributeDialog::styleButtons(QPushButton *btn) {
	QString leftButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"border-bottom-left-radius: 4px;"
		"border-top-left-radius: 4px;"
		"padding-left: 2px;"
		"height: 20px;"
		"}";

	QString ButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"padding-left: 5px;"
		"padding-right: 5px;"
		"height: 20px;"
		"}";

	QString rightButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"border-bottom-right-radius: 4px;"
		"border-top-right-radius: 4px;"
		"padding-left: 2px;"
		"height: 20px;"
		"}";

	QString hoverSty =
		"QPushButton:hover { "
		"background-color: rgb(100,100,100);"
		"}";

	QString hoverPressdSty =
		"QPushButton:hover:pressed { "
		"background-color: rgb(255, 153, 83);"
		"}";

	btn->setStyleSheet(ButtonSty + hoverSty + hoverPressdSty);
}
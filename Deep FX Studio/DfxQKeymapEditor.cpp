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

#include "DfxQKeymapEditor.h"

DfxQKeymapEditor::DfxQKeymapEditor(QWidget *parent): QWidget(parent) {

	QStringList modifers;
	modifers.append("CTRL");
	modifers.append("ALT");
	modifers.append("SHIFT");

	QStringList mouseButtons;
	mouseButtons.append("Left Mouse Button");
	mouseButtons.append("Middle Mouse Button");
	mouseButtons.append("Right Mouse Button");

	QWidget *central = new QWidget;
	scrollArea = new QScrollArea();

	groupLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom, central);
	mainLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);

	viewportNavigationMap = new QGroupBox("Viewport Navigation");
	viewportNavigationLayout = new QGridLayout();

	panLabel = new QLabel("Pan");
	panModiferList = new QComboBox();
	panMouseButtonList = new QComboBox();
	
	panModiferList->addItems(modifers);
	panMouseButtonList->addItems(mouseButtons);
	
	zoomLabel = new QLabel("Zoom");
	zoomModiferList = new QComboBox();
	zoomMouseButtonList = new QComboBox();

	zoomModiferList->addItems(modifers);
	zoomMouseButtonList->addItems(mouseButtons);

	rotateLabel = new QLabel("Rotate");
	rotateModiferList = new QComboBox();
	rotateMouseButtonList = new QComboBox();

	rotateModiferList->addItems(modifers);
	rotateMouseButtonList->addItems(mouseButtons);

	viewportNavigationLayout->addWidget(panLabel, 0, 0);
	viewportNavigationLayout->addWidget(panModiferList, 0, 1);
	viewportNavigationLayout->addWidget(panMouseButtonList, 0, 2);

	viewportNavigationLayout->addWidget(zoomLabel, 1, 0);
	viewportNavigationLayout->addWidget(zoomModiferList, 1, 1);
	viewportNavigationLayout->addWidget(zoomMouseButtonList, 1, 2);

	viewportNavigationLayout->addWidget(rotateLabel, 2, 0);
	viewportNavigationLayout->addWidget(rotateModiferList, 2, 1);
	viewportNavigationLayout->addWidget(rotateMouseButtonList, 2, 2);

	viewportNavigationMap->setLayout(viewportNavigationLayout);
	groupLayout->addWidget(viewportNavigationMap);

	initViewportkeyMap();
	generateKeymapUI();

	scrollArea->setWidget(central);
	scrollArea->setWidgetResizable(true);
	//scrollArea->setLayout(groupLayout);

	mainLayout->addWidget(scrollArea);

	setLayout(mainLayout);

	QObject::connect(panModiferList, qOverload<int>(&QComboBox::currentIndexChanged),
		             this, &DfxQKeymapEditor::panModifierUpdate);

	QObject::connect(panMouseButtonList, qOverload<int>(&QComboBox::currentIndexChanged),
					 this, &DfxQKeymapEditor::panMouseButtonUpdate);

	QObject::connect(zoomModiferList, qOverload<int>(&QComboBox::currentIndexChanged),
					 this, &DfxQKeymapEditor::zoomModifierUpdate);

	QObject::connect(zoomMouseButtonList, qOverload<int>(&QComboBox::currentIndexChanged),
					 this, &DfxQKeymapEditor::zoomMouseButtonUpdate);

	QObject::connect(rotateModiferList, qOverload<int>(&QComboBox::currentIndexChanged),
					 this, &DfxQKeymapEditor::rotateModifierUpdate);

	QObject::connect(rotateMouseButtonList, qOverload<int>(&QComboBox::currentIndexChanged),
					 this, &DfxQKeymapEditor::rotateMouseButtonUpdate);
}

DfxQKeymapEditor::~DfxQKeymapEditor() {

}

void DfxQKeymapEditor::generateKeymapUI() {

	QSettings settings;

	for (auto group : DfxShortcutManager::getInstance()->getAllGroupNames()) {

		auto gbox = new QGroupBox(group.c_str());
		auto layout = new QFormLayout();

		for (auto action: DfxShortcutManager::getInstance()->getGroupShortcutActions(group)) {
			
			DfxIntegerTyple tup;
			QString title(action->getActionName().c_str());
			QPushButton *btn;

			auto rc = settings.value(action->getName().c_str(), "");

			tup.fromString(rc.value<QString>().toStdString());

			bool two_key = false;
			QString key_a;
			QString key_b;

			if (tup.get(1) == -1) {
				QKeySequence sq(tup.get(0));
				key_a = sq.toString();
			}
			else {
				QKeySequence sq01(tup.get(0));
				QKeySequence sq02(tup.get(1));

				key_a = sq01.toString();
				key_b = sq02.toString();

				two_key = true;
			}

			if (two_key) {
				btn = new DfxQKeymapEditorShortcutButton(key_a + " " + key_b, action);
			}
			else {
				btn = new DfxQKeymapEditorShortcutButton(key_a, action);
			}

			layout->addRow(title, btn);
		}

		gbox->setLayout(layout);
		groupLayout->addWidget(gbox);
	}
}

void DfxQKeymapEditor::initViewportkeyMap() {

	QSettings settings;
	QVariant rc;

	rc = settings.value("viewport/navigation/pan", "");
	DfxIntegerTyple tup(rc.value<QString>().toStdString());

	if (tup.get(0) == Qt::Key_Control) {
		panModiferList->setCurrentIndex(0);
	}
	else if (tup.get(0) == Qt::Key_Alt) {
		panModiferList->setCurrentIndex(1);
	}
	else {
		panModiferList->setCurrentIndex(2);
	}

	if (tup.get(1) == Qt::LeftButton) {
		panMouseButtonList->setCurrentIndex(0);
	}
	else if (tup.get(1) == Qt::MiddleButton) {
		panMouseButtonList->setCurrentIndex(1);
	}
	else {
		panMouseButtonList->setCurrentIndex(2);
	}

	rc = settings.value("viewport/navigation/zoom", "");
	tup  = DfxIntegerTyple(rc.value<QString>().toStdString());

	if (tup.get(0) == Qt::Key_Control) {
		zoomModiferList->setCurrentIndex(0);
	}
	else if (tup.get(0) == Qt::Key_Alt) {
		zoomModiferList->setCurrentIndex(1);
	}
	else {
		zoomModiferList->setCurrentIndex(2);
	}

	if (tup.get(1) == Qt::LeftButton) {
		zoomMouseButtonList->setCurrentIndex(0);
	}
	else if (tup.get(1) == Qt::MiddleButton) {
		zoomMouseButtonList->setCurrentIndex(1);
	}
	else {
		zoomMouseButtonList->setCurrentIndex(2);
	}

	rc = settings.value("viewport/navigation/rotate", "");
	tup = DfxIntegerTyple(rc.value<QString>().toStdString());

	if (tup.get(0) == Qt::Key_Control) {
		rotateModiferList->setCurrentIndex(0);
	}
	else if (tup.get(0) == Qt::Key_Alt) {
		rotateModiferList->setCurrentIndex(1);
	}
	else {
		rotateModiferList->setCurrentIndex(2);
	}

	if (tup.get(1) == Qt::LeftButton) {
		rotateMouseButtonList->setCurrentIndex(0);
	}
	else if (tup.get(1) == Qt::MiddleButton) {
		rotateMouseButtonList->setCurrentIndex(1);
	}
	else {
		rotateMouseButtonList->setCurrentIndex(2);
	}
}

int DfxQKeymapEditor::getComboxModifierID(int index) {

	if (index == 0) return Qt::Key_Control;
	else if (index == 1) return Qt::Key_Alt;
	else return Qt::Key_Shift;
}

int DfxQKeymapEditor::getComboxMouseButtonID(int index) {

	if (index == 0) return Qt::LeftButton;
	else if (index == 1) return Qt::MiddleButton;
	else return Qt::RightButton;
}

void DfxQKeymapEditor::panModifierUpdate(int index) {

	QSettings settings;

	int n01 = getComboxModifierID(index);
	int n02 = getComboxMouseButtonID(panMouseButtonList->currentIndex());

	DfxIntegerTyple tup(n01, n02);
	settings.setValue("viewport/navigation/pan", QString(tup.toString().c_str()));
	DfxShortcutManager::getInstance()->setViewportPanSequence(std::pair<int, int>(n01, n02));

}

void DfxQKeymapEditor::panMouseButtonUpdate(int index) {

	QSettings settings;

	int n01 = getComboxModifierID(panModiferList->currentIndex());
	int n02 = getComboxMouseButtonID(index);

	DfxIntegerTyple tup(n01, n02);
	settings.setValue("viewport/navigation/pan", QString(tup.toString().c_str()));
	DfxShortcutManager::getInstance()->setViewportPanSequence(std::pair<int, int>(n01, n02));

}

void DfxQKeymapEditor::zoomModifierUpdate(int index) {

	QSettings settings;

	int n01 = getComboxModifierID(index);
	int n02 = getComboxMouseButtonID(zoomMouseButtonList->currentIndex());

	DfxIntegerTyple tup(n01, n02);
	settings.setValue("viewport/navigation/zoom", QString(tup.toString().c_str()));
	DfxShortcutManager::getInstance()->setViewportZoomSequence(std::pair<int, int>(n01, n02));

}

void DfxQKeymapEditor::zoomMouseButtonUpdate(int index) {

	QSettings settings;

	int n01 = getComboxModifierID(zoomModiferList->currentIndex());
	int n02 = getComboxMouseButtonID(index);

	DfxIntegerTyple tup(n01, n02);
	settings.setValue("viewport/navigation/zoom", QString(tup.toString().c_str()));
	DfxShortcutManager::getInstance()->setViewportZoomSequence(std::pair<int, int>(n01, n02));

}

void DfxQKeymapEditor::rotateModifierUpdate(int index) {

	QSettings settings;

	int n01 = getComboxModifierID(index);
	int n02 = getComboxMouseButtonID(rotateMouseButtonList->currentIndex());

	DfxIntegerTyple tup(n01, n02);
	settings.setValue("viewport/navigation/rotate", QString(tup.toString().c_str()));
	DfxShortcutManager::getInstance()->setViewportRotateSequence(std::pair<int, int>(n01, n02));

}

void DfxQKeymapEditor::rotateMouseButtonUpdate(int index) {

	QSettings settings;

	int n01 = getComboxModifierID(rotateModiferList->currentIndex());
	int n02 = getComboxMouseButtonID(index);

	DfxIntegerTyple tup(n01, n02);
	settings.setValue("viewport/navigation/rotate", QString(tup.toString().c_str()));
	DfxShortcutManager::getInstance()->setViewportRotateSequence(std::pair<int, int>(n01, n02));

}

/////////////////////////////////////////////
///DfxQKeymapEditorShortcutButton
/////////////////////////////////////////////
DfxQKeymapEditorShortcutButton::DfxQKeymapEditorShortcutButton(QString title, DfxShortcutAction *act, QWidget *parent)
	: QPushButton(title, parent) {

	updateShortcutMode = false;
	modifierKey = -1;

	QObject::connect(this, &DfxQKeymapEditorShortcutButton::clicked,
		this, &DfxQKeymapEditorShortcutButton::_clicked);

	oldTitle = title;

	setCheckable(true);

	daction = act;
}

void DfxQKeymapEditorShortcutButton::keyPressEvent(QKeyEvent *e) {

	if (e->key() == Qt::Key_Escape) {
		reset();
	}

	int t = e->key();
	if (e->key() == Qt::Key_Control ||
		e->key() == Qt::Key_Alt ||
		e->key() == Qt::Key_Shift) {

		if (modifierKey == -1) {
			modifierKey = e->key();

			if (e->key() == Qt::Key_Control) {
				setText("CTRL + ...");
			}

			if (e->key() == Qt::Key_Alt) {
				setText("ALT + ...");
			}

			if (e->key() == Qt::Key_Shift) {
				setText("SHIFT + ...");
			}
		}
	}
	else if ((0x21 <= e->key() && e->key() <= 0x0ff) ||
		(0x01000030 <= e->key() && e->key() <= 0x01000052)) {

		QSettings settings;
		if (modifierKey == -1) {

			DfxIntegerTyple tup(e->key(), -1);
			settings.setValue(daction->getName().c_str(), tup.toString().c_str());

			QKeySequence sq01(tup.get(0));
			daction->updateShorcut(sq01);

			oldTitle = sq01.toString();
		}
		else {

			int _m = -1;

			if (modifierKey == Qt::Key_Control) _m = Qt::CTRL;
			if (modifierKey == Qt::Key_Alt)     _m = Qt::ALT;
			if (modifierKey == Qt::Key_Shift)   _m = Qt::SHIFT;

			DfxIntegerTyple tup(modifierKey, e->key());
			settings.setValue(daction->getName().c_str(), tup.toString().c_str());

			QKeySequence sq01(_m);
			QKeySequence sq02(tup.get(0));
			QKeySequence sq03(_m + tup.get(0));

			daction->updateShorcut(sq03);

			oldTitle = sq01.toString() + " " + sq02.toString();
		}

		reset();
	}
}

void DfxQKeymapEditorShortcutButton::keyReleaseEvent(QKeyEvent *e) {

	if (modifierKey != -1) {

		if (e->key() == modifierKey) {

			setText("<< Enter Shorcut >>");
			modifierKey = -1;
		}
	}
}

void DfxQKeymapEditorShortcutButton::focusOutEvent(QFocusEvent *event){

	reset();

	QPushButton::focusOutEvent(event);
}

void DfxQKeymapEditorShortcutButton::reset() {
	if (updateShortcutMode) {
		updateShortcutMode = false;
		modifierKey = -1;

		setText(oldTitle);

		setChecked(false);
		//QApplication::setOverrideCursor(Qt::ArrowCursor);

	}
}


void DfxQKeymapEditorShortcutButton::_clicked() {
	updateShortcutMode = true;
	oldTitle = text();
	setText("<< Enter Shorcut >>");
	//QApplication::setOverrideCursor(Qt::BlankCursor);
}




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

#include "DfxQHotkeyEditor.h"

DfxQHotkeyEditor::DfxQHotkeyEditor():QDialog(0, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint){

	setWindowTitle("Hotkey Editor");

	QDockWidget *leftDock = new QDockWidget(this);
	
	layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	frame  = new QMainWindow();
	shortcuts = new QTreeWidget(this);
	sections = new QTabWidget(this);
	centralWidget = new QWidget;
	centrallayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
	editLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
	buttonLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	formLayout = new QFormLayout;
	hotkeyField = new HotKeyButton("", this);
	assignRemoveWidget = new QWidget;
	// assign = new QPushButton("Assign ", this);
	remove = new QPushButton("Remove", this);
	currentCommand = new QLabel("");
	ioWidget = new QWidget;
	iolayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	load = new QPushButton("Load...", this);
	save = new QPushButton("Save...", this);
	reset = new QPushButton("Reset...", this);
	viewportNavigationWidget = new QWidget();
	viewportNavigationLayout = new QGridLayout();
	panLabel = new QLabel("Pan:");
	panModiferList = new QComboBox();
	panMouseButtonList = new QComboBox();
	zoomLabel = new QLabel("Zoom:");
	zoomModiferList = new QComboBox();
	zoomMouseButtonList = new QComboBox();
	rotateLabel = new QLabel("Rotate:");
	rotateModiferList = new QComboBox();
	rotateMouseButtonList = new QComboBox();

	QStringList modifers;
	modifers.append("CTRL");
	modifers.append("ALT");
	modifers.append("SHIFT");

	QStringList mouseButtons;
	mouseButtons.append("Left Mouse Button");
	mouseButtons.append("Middle Mouse Button");
	mouseButtons.append("Right Mouse Button");

	panModiferList->addItems(modifers);
	panMouseButtonList->addItems(mouseButtons);

	zoomModiferList->addItems(modifers);
	zoomMouseButtonList->addItems(mouseButtons);

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
	viewportNavigationWidget->setLayout(viewportNavigationLayout);

	layout->setMargin(0);
	iolayout->setMargin(0);
	editLayout->setMargin(0);
	buttonLayout->setMargin(0);
	centrallayout->setContentsMargins(0, 50, 0, 0);
	//viewportNavigationLayout->setMargin(0);

	hotkeyField->setFixedWidth(250);
	assignRemoveWidget->setFixedWidth(250);
	centralWidget->setFixedWidth(350);
	viewportNavigationWidget->setFixedSize(QSize(250, 100));

	//buttonLayout->addWidget(assign);
	buttonLayout->addWidget(remove);

	assignRemoveWidget->setLayout(buttonLayout);

	formLayout->addRow("Command:", currentCommand);
	formLayout->addRow("Hotkey:", hotkeyField);
	formLayout->addRow("", assignRemoveWidget);
	
	//buttonLayout->addWidget(assign);
	buttonLayout->addWidget(remove);

	//editLayout->addLayout(formLayout);

	iolayout->addWidget(save);
	iolayout->addWidget(load);
	iolayout->addWidget(reset);
	ioWidget->setLayout(iolayout);

	sections->addTab(shortcuts, "Keyboard");
	sections->addTab(viewportNavigationWidget, "Viewport Navigation");

	leftDock->setWidget(sections);
	leftDock->setFeatures(0);

	centrallayout->addLayout(formLayout);
	centrallayout->addWidget(ioWidget);
	centralWidget->setLayout(centrallayout);

	frame->setCentralWidget(centralWidget);
	frame->addDockWidget(Qt::LeftDockWidgetArea, leftDock, Qt::Vertical);

	layout->addWidget(frame);
	setLayout(layout);

	shortcuts->setColumnCount(2);
	shortcuts->setHeaderLabels(QStringList() << "Command Name" << "Hotkey");
	shortcuts->setColumnWidth(0, 250);

	initNavigation();
	populateList();

	resize(QSize(750, 500));

	QObject::connect(shortcuts, &QTreeWidget::itemClicked, this, &DfxQHotkeyEditor::commandSelected);
	QObject::connect(remove, &QPushButton::clicked, this, &DfxQHotkeyEditor::removedSelectedCommand);

	QObject::connect(save, &QPushButton::clicked, this, &DfxQHotkeyEditor::saveToFile);
	QObject::connect(load, &QPushButton::clicked, this, &DfxQHotkeyEditor::loadFromFile);
	QObject::connect(reset, &QPushButton::clicked, this, &DfxQHotkeyEditor::resetShortcuts);

	QObject::connect(panModiferList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DfxQHotkeyEditor::updateNavigationHotkeys);
	QObject::connect(zoomModiferList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DfxQHotkeyEditor::updateNavigationHotkeys);
	QObject::connect(rotateModiferList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DfxQHotkeyEditor::updateNavigationHotkeys);
	QObject::connect(panMouseButtonList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DfxQHotkeyEditor::updateNavigationHotkeys);
	QObject::connect(zoomMouseButtonList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DfxQHotkeyEditor::updateNavigationHotkeys);
	QObject::connect(rotateMouseButtonList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DfxQHotkeyEditor::updateNavigationHotkeys);

}

DfxQHotkeyEditor::~DfxQHotkeyEditor() {
	clearList();
}


void DfxQHotkeyEditor::clearList() {
	shortcuts->clear();
}

void DfxQHotkeyEditor::populateList() {

	for (auto group : DfxShortcutsDB::getInstance()->findAllGroups()) {

		if (group.name.compare("Viewport") == 0) continue;

		QTreeWidgetItem *groupItem= new QTreeWidgetItem(shortcuts);
		groupItem->setText(0, group.name.c_str());

		QPair<int, QPair<QString, QString>> data(0, QPair<QString, QString>("", ""));
		QVariant variant = QVariant::fromValue(data);
		groupItem->setData(0, Qt::UserRole, variant);
		groupItem->setData(1, Qt::UserRole, variant);

		for (auto shortcut : group.shortcuts) {
			QTreeWidgetItem *shortcutItem = new QTreeWidgetItem();
			shortcutItem->setText(0, shortcut.second.name.c_str());
			shortcutItem->setText(1, shortcut.second.toString().c_str());

			QPair<int, QPair<QString, QString>> data(1, QPair<QString, QString>(group.name.c_str(), shortcut.second.name.c_str()));
			QVariant variant = QVariant::fromValue(data);
			shortcutItem->setData(0, Qt::UserRole, variant);
			shortcutItem->setData(1, Qt::UserRole, variant);

			groupItem->addChild(shortcutItem);
		}

		groupItem->setExpanded(true);
	}
}

void DfxQHotkeyEditor::commandSelected(QTreeWidgetItem *item, int column) {

	QVariant variant;
	HotkeyItemValue pair_out = item->data(column, Qt::UserRole).value< HotkeyItemValue>();

	if (pair_out.first == 1) {
		DfxShortcutsDB::Group grp;

		if (DfxShortcutsDB::getInstance()->findGroup(pair_out.second.first.toStdString(), grp)) {
			DfxShortcutsDB::Shortcut shrt;
			if (DfxShortcutsDB::getInstance()->findShortcut(grp, pair_out.second.second.toStdString(), shrt)) {

				currentCommand->setText(pair_out.second.second);
				hotkeyField->setText(shrt.toString().c_str());
				hotkeyField->setCommandName(pair_out.second.second);
				hotkeyField->setGroupName(pair_out.second.first);
				hotkeyField->setTreeItem(item);
			}
		}
	}
}

int DfxQHotkeyEditor::filterModifier(int k) {
	if (k != Qt::Key_Alt && k != Qt::Key_Control && k != Qt::Key_Shift)
		return Qt::Key_Alt;
	else
		return k;
}

int DfxQHotkeyEditor::filterMouse(int m) {
	if (m != Qt::MiddleButton && m != Qt::LeftButton && m != Qt::RightButton)
		return Qt::LeftButton;
	else
		return m;
}

void DfxQHotkeyEditor::setModifierIndex(int k, QComboBox *combo) {

	if (k == Qt::Key_Control) {
		combo->setCurrentIndex(0);
	}
	else if (k == Qt::Key_Alt ) {
		combo->setCurrentIndex(1);
	}
	else if (k == Qt::Key_Shift) {
		combo->setCurrentIndex(2);
	}
}

void DfxQHotkeyEditor::setMouseIndex(int m, QComboBox *combo) {

	if (m == Qt::LeftButton) {
		combo->setCurrentIndex(0);
	}
	else if (m == Qt::MiddleButton) {
		combo->setCurrentIndex(1);
	}
	else if (m == Qt::RightButton) {
		combo->setCurrentIndex(2);
	}
}

void DfxQHotkeyEditor::initNavigation() {

	DfxShortcutsDB::Group grp;

	if (DfxShortcutsDB::getInstance()->findGroup("Viewport", grp)) {
		
		DfxShortcutsDB::Shortcut pan;
		DfxShortcutsDB::Shortcut zoom;
		DfxShortcutsDB::Shortcut rotate;

		DfxShortcutsDB::getInstance()->findShortcut(grp, "Pan", pan);
		DfxShortcutsDB::getInstance()->findShortcut(grp, "Zoom", zoom);
		DfxShortcutsDB::getInstance()->findShortcut(grp, "Rotate", rotate);

		setModifierIndex(filterModifier(pan.k00), panModiferList);
		setMouseIndex(filterMouse(pan.mouseButton), panMouseButtonList);

		setModifierIndex(filterModifier(zoom.k00), zoomModiferList);
		setMouseIndex(filterMouse(zoom.mouseButton), zoomMouseButtonList);

		setModifierIndex(filterModifier(rotate.k00), rotateModiferList);
		setMouseIndex(filterMouse(rotate.mouseButton), rotateMouseButtonList);
	}
}

void DfxQHotkeyEditor::removedSelectedCommand(bool) {
	hotkeyField->remove();
}

void DfxQHotkeyEditor::updateNavigationHotkeys(int index) {

	int panMod = 0;
	int panMouse = 0;

	int zoomMod = 0;
	int zoomMouse = 0;

	int rotateMod = 0;
	int rotateMouse = 0;

	// Pan
	if (panModiferList->currentIndex() == 0) panMod = Qt::Key_Control;
	else if (panModiferList->currentIndex() == 1) panMod = Qt::Key_Alt;
	else panMod = Qt::Key_Shift;

	if (panMouseButtonList->currentIndex() == 0) panMouse = Qt::LeftButton;
	else if (panMouseButtonList->currentIndex() ==1) panMouse = Qt::MiddleButton;
	else panMouse = Qt::RightButton;

	// Zoom
	if (zoomModiferList->currentIndex() == 0) zoomMod = Qt::Key_Control;
	else if (zoomModiferList->currentIndex() == 1) zoomMod = Qt::Key_Alt;
	else zoomMod = Qt::Key_Shift;

	if (zoomMouseButtonList->currentIndex() == 0) zoomMouse = Qt::LeftButton;
	else if (zoomMouseButtonList->currentIndex() == 1) zoomMouse = Qt::MiddleButton;
	else zoomMouse = Qt::RightButton;

	// Rotate
	if (rotateModiferList->currentIndex() == 0) rotateMod = Qt::Key_Control;
	else if (rotateModiferList->currentIndex() == 1) rotateMod = Qt::Key_Alt;
	else rotateMod = Qt::Key_Shift;

	if (rotateMouseButtonList->currentIndex() == 0) rotateMouse = Qt::LeftButton;
	else if (rotateMouseButtonList->currentIndex() == 1) rotateMouse = Qt::MiddleButton;
	else rotateMouse = Qt::RightButton;

	if(!DfxShortcutsDB::getInstance()->setHotkey("Viewport", "Pan", DfxShortcutsDB::KEYBOARD_MOUSE, panMod, 0, 0, 0, panMouse)) DfxGlobal::console::println("Failed to set Pan Hotkey");
	if(!DfxShortcutsDB::getInstance()->setHotkey("Viewport", "Zoom", DfxShortcutsDB::KEYBOARD_MOUSE, zoomMod, 0, 0, 0, zoomMouse)) DfxGlobal::console::println("Failed to set Zoom Hotkey");
	if(!DfxShortcutsDB::getInstance()->setHotkey("Viewport", "Rotate", DfxShortcutsDB::KEYBOARD_MOUSE, rotateMod, 0, 0, 0, rotateMouse)) DfxGlobal::console::println("Failed to set Rotate Hotkey");

}

void DfxQHotkeyEditor::saveToFile(bool) {
	QString appdata = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];

	QDir dir(appdata + "/configuration");
	QFile file(appdata + "/configuration/shortcuts.cfg");

	if (!dir.exists()) filesystem::create_directories(filesystem::path((appdata + "/configuration").toStdString()));

	DfxHotkeysSaveLoad::save(appdata + "/configuration/shortcuts.cfg");
}

void DfxQHotkeyEditor::loadFromFile(bool) {

	QSettings settings;
	QString initialDir = settings.value("io/loadHotkeys", QDir::homePath()).toString();

	QString fileName
		= QFileDialog::getOpenFileName(nullptr, QString("Load Hotkeys"), initialDir, QString("Files (*.cfg;)"));

	if (fileName.size() > 0) {

		DfxHotkeysSaveLoad::load(fileName);
		shortcuts->clear();
		populateList();

		settings.setValue("io/loadHotkeys", fileName);
	}
}

void DfxQHotkeyEditor::resetShortcuts(bool) {
	DfxHotkeysSaveLoad::load(":/config/shortcuts");
	shortcuts->clear();
	populateList();
}

// HotKeyButton Definition
HotKeyButton::HotKeyButton(QString title,  QWidget *parent)
	: QPushButton(title, parent) {

	updateShortcutMode = false;
	modifierKey = -1;

	QObject::connect(this, &HotKeyButton::clicked,
		this, &HotKeyButton::_clicked);

	oldTitle = title;

	setCheckable(true);

	treeItem = nullptr;

	groupName   = "";
	commandName = "";
}

void HotKeyButton::setTreeItem(QTreeWidgetItem *titem) {
	treeItem = titem;
}

void HotKeyButton::setCommandName(QString str) {
	commandName = str.toStdString();
}

void HotKeyButton::setGroupName(QString str) {
	groupName = str.toStdString();
}

void HotKeyButton::keyPressEvent(QKeyEvent *e) {

	if (e->key() == Qt::Key_Escape) {
		reset();
	}

	if (e->key() == Qt::Key_Control ||
		e->key() == Qt::Key_Alt ||
		e->key() == Qt::Key_Shift) {

		addKey(e->key());

		updateText();
	}
	else if ((0x21 <= e->key() && e->key() <= 0x0ff) ||
		(0x01000000 <= e->key() && e->key() <= 0x01000052)) {

		addKey(e->key());

		auto _keys = keys4();
		DfxShortcutsDB::getInstance()->setHotkey(groupName, commandName, DfxShortcutsDB::KEYBOARD, _keys[0], _keys[1], _keys[2], _keys[3]);
		treeItem->setText(1, keysToString().c_str());
		
		updateText();
		reset();
	}
}

void HotKeyButton::keyReleaseEvent(QKeyEvent *e) {
	removeKey(e->key());

	if(updateShortcutMode) updateText();
}

void HotKeyButton::focusOutEvent(QFocusEvent *event) {

	reset();

	QPushButton::focusOutEvent(event);
}

void HotKeyButton::reset() {
	if (updateShortcutMode) {
		updateShortcutMode = false;
		modifierKey = -1;

		//setText(oldTitle);

		setChecked(false);
		//QApplication::setOverrideCursor(Qt::ArrowCursor);

	}
}

void HotKeyButton::remove() {
	if (commandName.size() > 0 || groupName.size() > 0) {
		DfxShortcutsDB::getInstance()->setHotkey(groupName, commandName, DfxShortcutsDB::KEYBOARD, 0, 0, 0, 0);
		treeItem->setText(1, "");
		setText("");
	}
}

void HotKeyButton::updateText() {
	setText(keysToString().c_str());
}

void HotKeyButton::_clicked() {
	if (commandName.size() > 0 || groupName.size() > 0) {
		updateShortcutMode = true;
		oldTitle = text();
		setText("<< Enter Shorcut >>");
	}
	else {
		setChecked(false);
	}
	//QApplication::setOverrideCursor(Qt::BlankCursor);
}
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

#include <QDialog>
#include <QMetaType>
#include <QVariant>
#include <QTreeWidget>
#include <QMainWindow.h>
#include <QBoxLayout.h>
#include <QFormLayout>
#include <QLineEdit>
#include <QTabWidget>
#include <QLabel>
#include <QComboBox>
#include <QDockWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QKeyEvent>
#include <DfxQKeyBuffer.h>
#include <DfxCommon.h>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include <filesystem>
#include "DfxShortcutsDB.h"
#include "DfxHotkeysSaveLoad.h"

namespace filesystem = std::experimental::filesystem;

using HotkeyItemValue = QPair<int, QPair<QString, QString>>;
Q_DECLARE_METATYPE(HotkeyItemValue);

class HotKeyButton : public QPushButton, public DfxQKeyBuffer {
	Q_OBJECT
private:

	bool updateShortcutMode;
	int modifierKey;
	QString oldTitle;
	QTreeWidgetItem *treeItem;
	std::string commandName;
	std::string groupName;
protected:

	void keyPressEvent(QKeyEvent *e) override;

	void keyReleaseEvent(QKeyEvent *e) override;

	void focusOutEvent(QFocusEvent *event) override;

	void updateText();

	void reset();
public slots:

	void _clicked();

public:

	//HotKeyButton(QString title, DfxShortcutAction *act, QWidget *parent = nullptr);
	HotKeyButton(QString title, QWidget *parent = nullptr);
	void setTreeItem(QTreeWidgetItem *titem);
	void setCommandName(QString);
	void setGroupName(QString);
	void remove();
};

class DfxQHotkeyEditor: public QDialog {
	Q_OBJECT
private:

	QBoxLayout *layout;
	QMainWindow *frame;
	QTreeWidget *shortcuts;

	QWidget *centralWidget;
	QBoxLayout  *centrallayout;

	QTabWidget *sections;

	QBoxLayout *editLayout;
	QWidget *assignRemoveWidget;
	QBoxLayout *buttonLayout;
	QFormLayout *formLayout;
	HotKeyButton* hotkeyField;
	QPushButton *assign;
	QPushButton *remove;
	QLabel *currentCommand;

	QWidget *ioWidget;
	QBoxLayout  *iolayout;
	QPushButton *load;
	QPushButton *save;
	QPushButton *reset;

	QWidget *viewportNavigationWidget;
	QGridLayout *viewportNavigationLayout;

	QLabel *panLabel;
	QComboBox *panModiferList;
	QComboBox *panMouseButtonList;

	QLabel *zoomLabel;
	QComboBox *zoomModiferList;
	QComboBox *zoomMouseButtonList;

	QLabel *rotateLabel;
	QComboBox *rotateModiferList;
	QComboBox *rotateMouseButtonList;

public:
	DfxQHotkeyEditor();
	virtual ~DfxQHotkeyEditor();

	void clearList();
	void populateList();
	void initNavigation();

	int filterModifier(int k);
	int filterMouse(int m);

	void setModifierIndex(int mod, QComboBox *combo);
	void setMouseIndex(int mouse, QComboBox *combo);

public slots:

	void commandSelected(QTreeWidgetItem *item, int column);
	void removedSelectedCommand(bool);
	void updateNavigationHotkeys(int index);
	void saveToFile(bool);
	void loadFromFile(bool);
	void resetShortcuts(bool);
};
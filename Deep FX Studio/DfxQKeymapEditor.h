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

#include <QWidget>
#include <QBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QCombobox>
#include <QKeysequence>
#include <QGroupBox>
#include <QApplication>
#include <QLabel>
#include <DfxIntegerTuple.h>
#include <QPushButton>
#include <QScrollArea>
#include <QKeyEvent>
#include <DfxShortcutManager.h>
#include <QSettings>
#include <DfxGlobal.h>

class DfxQKeymapEditorShortcutButton : public QPushButton {
	Q_OBJECT
private:

	bool updateShortcutMode;
	int modifierKey;
	QString oldTitle;
	DfxShortcutAction *daction;
protected:

	void keyPressEvent(QKeyEvent *e) override;

	void keyReleaseEvent(QKeyEvent *e) override;

	void focusOutEvent(QFocusEvent *event) override;

	void reset();
public slots:

	void _clicked();

public:

	DfxQKeymapEditorShortcutButton(QString title, DfxShortcutAction *act, QWidget *parent = nullptr);
};

class DfxQKeymapEditor: public QWidget {
	Q_OBJECT
private:
	QScrollArea *scrollArea;

	QBoxLayout *mainLayout;

	QBoxLayout *groupLayout;

	QGroupBox *viewportNavigationMap;
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

private:

	void generateKeymapUI();
	void initViewportkeyMap();

	int getComboxModifierID(int );
	int getComboxMouseButtonID(int );

public:

	DfxQKeymapEditor(QWidget *parent = nullptr);
	~DfxQKeymapEditor();

public slots:

	void panModifierUpdate(int);
	void panMouseButtonUpdate(int);

	void zoomModifierUpdate(int);
	void zoomMouseButtonUpdate(int);

	void rotateModifierUpdate(int);
	void rotateMouseButtonUpdate(int);
};


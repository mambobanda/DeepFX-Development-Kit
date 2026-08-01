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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <QObject>
#include <QMenu>
#include <QComboBox>
#include <QBoxLayout>
#include <QCompleter>
#include "DfxSpace.h"
#include "DfxQNodeEditorCommon.h"
#include "DfxComputeNodeDB.h"


class DfxQComputePopupMenu: public QMenu{
	Q_OBJECT
private:
	QComboBox *_combobox;
	QCompleter *_completer;
	QBoxLayout *_layout;
	DfxFluentNodeEditor *_nodeEditor;
	bool _done;
protected:
	void keyPressEvent(QKeyEvent *e) override;

	bool eventFilter(QObject *obj, QEvent *event) override;
public:

	DfxQComputePopupMenu(QWidget *parent = nullptr, DfxFluentNodeEditor *neditor = nullptr);
	virtual ~DfxQComputePopupMenu();

	void init();
	void showMenuPopup();

public slots:
	void addNode(int);
};
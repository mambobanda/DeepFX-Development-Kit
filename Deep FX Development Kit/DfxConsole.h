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

#include <QtWidgets\QDialog>
#include <QtWidgets\QTextEdit>
#include <QtWidgets\QPlainTextEdit>
#include <QtWidgets\QLineEdit>
#include <QtWidgets\QToolbar>
#include <QtWidgets\QBoxLayout>
#include <QtWidgets\QTabWidget>
#include <QtWidgets\QDockWidget>
#include <QtWidgets\QMainWindow>
#include <QtWidgets\QMenubar>
#include <QtWidgets\QMenu>
#include <QtWidgets\QAction>
#include <QtWidgets\QScrollbar>
#include <QKeyEvent>
#include <vector>
#include <mutex>
#include "DfxQPythonSyntaxHighlighter.h"
#include "DfxGlobal.h"

class DfxConsole: public QMainWindow {
	Q_OBJECT
private:
	static std::mutex _guard;
private slots:
	void loadBatchScript();
	void saveConsoleOutput();
	void close();
	void executeInput();
private:
	std::vector<QString> commandLst;
	unsigned int commandLstIndex;
	
protected:
	virtual void keyPressEvent(QKeyEvent *event) override;

private:
	QWidget *view;
	//QMainWindow *container;

	QBoxLayout boxLayout;
	QPlainTextEdit output;
	QLineEdit input;

	QMenuBar mainMenu;

	QMenu fileMenu;
	QAction actionLoadBatchScript;
	QAction actionSaveConsoleOutput;
	QAction actionClose;
public:
	DfxConsole(QWidget *parent = Q_NULLPTR);
	virtual ~DfxConsole();

	QPlainTextEdit *textArea();
public slots:
	void updateTextOuput(const QString txt);
};
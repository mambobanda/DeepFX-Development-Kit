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
#include <QtWidgets\QLineEdit>
#include <QtWidgets\QToolbar>
#include <QtWidgets\QLabel>
#include <QtWidgets\QBoxLayout>
#include <QtWidgets\QTabWidget>
#include <QtWidgets\QDockWidget>
#include <QtWidgets\QMainWindow>
#include <QtWidgets\QMenubar>
#include <QtWidgets\QMenu>
#include <QtWidgets\QAction>
#include <QtWidgets\QScrollbar>
#include <QKeyEvent>
#include <QStringList>
#include <vector>
#include "DfxTinyPythonTextEditor.h"
#include "DfxQPythonSyntaxHighlighter.h"
#include "DfxGlobal.h"


class DfxTinyPythonIDE : public QDockWidget {
	Q_OBJECT
private slots:
	void loadScript();
	void saveScript();
	void saveScriptAs();
	void executeScript();
	void installScript();
	void close();

protected:
	virtual void keyPressEvent(QKeyEvent *event) override;

private:
	QWidget *view;

	QWidget *pythonWidget;
	QBoxLayout pythonLayout;
	QToolBar pythonToolBar;
	DfxTinyPythonTextEditor pythonEdit;
	QAction *executePythonScript;
	QAction *installPythonScript;

	QMenuBar mainMenu;

	QLabel *statusInfo;

	QMenu fileMenu;
	QAction *loadScriptAction;
	QAction *saveScriptAction;
	QAction *saveAsScriptAction;

	bool (*save_script)(std::string, std::string &, std::string, bool);
	bool (*save_script_as)(std::string, std::string &);
	void (*install_script)(std::string);
	bool (*load_script)(std::string&, std::string&);

	bool hasFilePath;
	std::string filePath;
public:
	DfxTinyPythonIDE(QWidget *parent = Q_NULLPTR);
	virtual ~DfxTinyPythonIDE();

	void setSaveScriptCallback(bool(*callback)(std::string, std::string&, std::string, bool));
	void setSaveAsScriptCallback(bool(*callback)(std::string, std::string&));
	void setLoadScriptCallback(bool(*callback)(std::string&,std::string&));
	void setInstallScriptCallback(void(*callback)(std::string));
	
public slots:
	void updateTextOuput(const QString txt);
	void dockTopLevelChange(bool);
};
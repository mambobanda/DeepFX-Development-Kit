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

#include "DfxPythonGlobal.h"
#include "DfxTinyPythonIDE.h"

DfxTinyPythonIDE::DfxTinyPythonIDE(QWidget *parent) : QDockWidget(parent),
pythonLayout(QBoxLayout::Direction::TopToBottom) {

	install_script = nullptr;
	save_script = nullptr;
	save_script_as = nullptr;
	install_script = nullptr;

	hasFilePath = false;
	filePath = "";

	view = new QWidget(parent);

	// Init Widgets 
	resize(800, 600);
	setWindowTitle("Python IDE");

	statusInfo = new QLabel("Ready...");

	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

	QFont font;
	font.setFamily("Courier");
	font.setStyleHint(QFont::Monospace);
	font.setFixedPitch(true);
	font.setPointSize(8);

	QFontMetrics metrics(font);

	pythonEdit.setReadOnly(false);
	pythonEdit.setWindowTitle("Console");
	pythonEdit.setStyleSheet("QTextEdit { background-color: rgb(43, 43, 43) }");
	pythonEdit.setTextColor(QColor::fromRgb(255, 255, 255));
	pythonEdit.setFont(font);
	pythonEdit.setTabStopWidth(4 * metrics.width(' '));

	executePythonScript = new QAction(QIcon(":/python_tb/execute"), "Execute");
	installPythonScript = new QAction(QIcon(":/python_tb/install"), "Install Script");
	loadScriptAction = new QAction(QIcon(":/python_tb/load"), "Load Script");
	saveScriptAction = new QAction(QIcon(":/python_tb/save"), "Save Script");
	saveAsScriptAction = new QAction(QIcon(":/python_tb/save_as"), "Save Script As");

	pythonToolBar.addAction(saveScriptAction);
	pythonToolBar.addAction(saveAsScriptAction);
	pythonToolBar.addAction(loadScriptAction);
	pythonToolBar.addAction(executePythonScript);
	pythonToolBar.addAction(executePythonScript);
	pythonToolBar.addAction(installPythonScript);

	pythonLayout.addWidget(&pythonToolBar);
	pythonLayout.addWidget(&pythonEdit);

	pythonLayout.setMargin(0);

	QWidget *pythonWidget = new QWidget(this);
	pythonWidget->setLayout(&pythonLayout);

	DfxQPythonSyntaxHighlighter pythonHighlighter;

	view->setLayout(&pythonLayout);
	view->layout()->setContentsMargins(0, 0, 0, 0);

	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	setWidget(view);

	pythonEdit.setPlainText("import xglobal");

	QObject::connect(executePythonScript, &QAction::triggered,
		this, &DfxTinyPythonIDE::executeScript);

	QObject::connect(saveScriptAction, &QAction::triggered,
		this, &DfxTinyPythonIDE::saveScript);

	QObject::connect(saveAsScriptAction, &QAction::triggered,
		this, &DfxTinyPythonIDE::saveScriptAs);

	QObject::connect(loadScriptAction, &QAction::triggered,
		this, &DfxTinyPythonIDE::loadScript);

	QObject::connect(installPythonScript, &QAction::triggered,
		this, &DfxTinyPythonIDE::installScript);

	QObject::connect(this, &QDockWidget::topLevelChanged, 
		this, &DfxTinyPythonIDE::dockTopLevelChange);
}

DfxTinyPythonIDE::~DfxTinyPythonIDE() {

}


void DfxTinyPythonIDE::close() {

}

void DfxTinyPythonIDE::keyPressEvent(QKeyEvent *evt) {

	
}

void DfxTinyPythonIDE::updateTextOuput(const QString txt) {

}

void DfxTinyPythonIDE::executeScript() {

	std::string script = pythonEdit.toPlainText().toStdString();

	if (script.length() > 0) {
		DfxPythonGlobal::getInstance()->exec(script);
	}
}

void DfxTinyPythonIDE::installScript() {
	if (install_script != nullptr) {
		install_script(pythonEdit.toPlainText().toStdString());
	}
	else
	{
		DfxGlobal::console::println("No Callback Installed.");
	}
}

void DfxTinyPythonIDE::loadScript() {
	if (load_script != nullptr) {
		std::string script;
		bool ret = load_script(script, filePath);

		if (ret) {
			hasFilePath = true;

			pythonEdit.setPlainText(script.c_str());

			DfxGlobal::console::println("< Script Opened >");
		}
		else {
			hasFilePath = false;
		}
	}
	else
	{
		DfxGlobal::console::println("No Callback Installed.");
	}
}

void DfxTinyPythonIDE::saveScriptAs() {
	if (save_script_as != nullptr) {
	
		bool  ret = save_script_as(pythonEdit.toPlainText().toStdString(), filePath);

		if (!ret) {
			filePath = "";
			hasFilePath = false;
		}
		else {
			hasFilePath = true;
			DfxGlobal::console::println("< Script Saved >");
		}
		
	}
	else
	{
		DfxGlobal::console::println("No Callback Installed.");
	}
}

void DfxTinyPythonIDE::saveScript() {
	if (save_script != nullptr) {

		if (hasFilePath) {
			bool  ret = save_script(pythonEdit.toPlainText().toStdString(), filePath, filePath, true);

			if (!ret) {
				filePath = "";
				hasFilePath = false;
			}
			else {
				DfxGlobal::console::println("< Script Saved >");
			}
		}
		else {
			bool  ret = save_script(pythonEdit.toPlainText().toStdString(), filePath, "", false);

			if (ret) {
				hasFilePath = true;
				DfxGlobal::console::println("< Script Saved >");
			}
		}
	}
	else
	{
		DfxGlobal::console::println("No Callback Installed.");
	}
}

void DfxTinyPythonIDE::setSaveScriptCallback(bool(*callback)(std::string, std::string&, std::string, bool)) {
	save_script = callback;
}

void DfxTinyPythonIDE::setSaveAsScriptCallback(bool(*callback)(std::string, std::string&)) {
	save_script_as = callback;
}

void DfxTinyPythonIDE::setLoadScriptCallback(bool(*callback)(std::string&, std::string&)) {
	load_script = callback;
}

void DfxTinyPythonIDE::setInstallScriptCallback(void(*callback)(std::string)) {
	install_script = callback;
}

void DfxTinyPythonIDE::dockTopLevelChange(bool) {
	if (isFloating()) {
		setWindowFlags(Qt::CustomizeWindowHint | Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
		show();
	}
}
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

#include "DfxConsole.h"

std::mutex DfxConsole::_guard;

DfxConsole::DfxConsole(QWidget *parent): QMainWindow(parent), boxLayout(QBoxLayout::Direction::TopToBottom) {
	
	view = new QWidget(parent);
	//container = new QMainWindow(parent);
	// Init Members
	commandLstIndex = 0;

	// Init Widgets 
	//resize(800, 600);
	//setWindowTitle("Console");

	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

	QFont inputFont;
	inputFont.setWeight(1);
	inputFont.setPointSize(10);
	input.setFont(inputFont);
	input.setMinimumHeight(30);
	input.setContentsMargins(5, 0, 5, 6);

	output.setReadOnly(true);
	output.setStyleSheet("QPlainTextEdit { background-color: rgb(47, 47, 47); color: rgb(255, 255, 255); }");
	//output.setTextColor(QColor::fromRgb(255, 255, 255));

	fileMenu.setTitle("File");

	actionLoadBatchScript.setText("Load Batch Script");
	actionSaveConsoleOutput.setText("Save Output");
	actionClose.setText("Close");

	fileMenu.addAction(&actionLoadBatchScript);
	fileMenu.addAction(&actionSaveConsoleOutput);
	fileMenu.addAction(&actionClose);

	mainMenu.addMenu(&fileMenu);
	boxLayout.addWidget(&output);
	boxLayout.addWidget(&input);

	view->setLayout(&boxLayout);
	view->layout()->setContentsMargins(0, 0, 0, 0);

	setCentralWidget(view);
	//setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	//setWidget(container);

	/*setLayout(&boxLayout);

	this->layout()->setContentsMargins(0, 0, 0, 0);
	this->layout()->setMenuBar(&mainMenu);*/

	updateTextOuput(DfxConsoleBuffer::getInstance()->toString().c_str());

	QObject::connect(&actionLoadBatchScript, &QAction::triggered,
		this, &DfxConsole::loadBatchScript);

	QObject::connect(&actionSaveConsoleOutput, &QAction::triggered,
		this, &DfxConsole::saveConsoleOutput);

	QObject::connect(&actionClose, &QAction::triggered,
		this, &DfxConsole::close);

	QObject::connect(&input, &QLineEdit::returnPressed,
		this, &DfxConsole::executeInput);

	QObject::connect(DfxConsoleBuffer::getInstance(), &DfxConsoleBuffer::bufferUpdateEvent,
		this, &DfxConsole::updateTextOuput);
}

DfxConsole::~DfxConsole() {

}

void DfxConsole::loadBatchScript() {

}

void DfxConsole::saveConsoleOutput() {

}

void DfxConsole::close() {

}

void DfxConsole::executeInput() {

	DfxString cmdstr = input.text().toUtf8().constData();

	if (cmdstr.size() > 0) {
		DfxGlobal::command::exec(cmdstr);
	}

	commandLst.push_back(QString(cmdstr.c_str()));
	commandLstIndex = commandLst.size() - 1;

	input.clear();
}

void DfxConsole::keyPressEvent(QKeyEvent *evt) {

	if (evt->key() == Qt::Key::Key_Up) {

		if (commandLst.size() <= 0) { return; }
		if (commandLstIndex == -1) { return; }

		input.setText(commandLst[commandLstIndex]);

		commandLstIndex--;
	}

	if (evt->key() == Qt::Key::Key_Down) {

		if (commandLst.size() <= 0) { return; }
		if (commandLstIndex == -1) { commandLstIndex++; }
		if (commandLstIndex == commandLst.size() - 1) { 
			input.setText("");
			return; 
		}

		input.setText(commandLst[commandLstIndex]);

		commandLstIndex++;
	}
}

void DfxConsole::updateTextOuput(const QString txt) {
	const std::lock_guard<std::mutex> lock(_guard);

	output.setPlainText(txt);
	auto sb = output.verticalScrollBar();
	sb->setValue(sb->maximum());
}

QPlainTextEdit *DfxConsole::textArea() {
	return &output;
}
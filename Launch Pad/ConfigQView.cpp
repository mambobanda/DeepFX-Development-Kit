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

#include "ConfigQView.h"
#include "ProjectManager.h"

ConfigQView::ConfigQView() {
	layout = new QVBoxLayout;
	editor = new QTextEdit;

	QFont font;
	font.setFamily("Courier");
	font.setStyleHint(QFont::Monospace);
	font.setFixedPitch(true);
	font.setPointSize(10);

	const int tabStop = 4;  // 4 characters

	QFontMetrics metrics(font);
	
	editor->setText(ResourceManager::getInstance()->configXML().c_str());

	editor->setFont(font);
	editor->setTabStopWidth(tabStop * metrics.width(' '));

	highliter = new QSourceHighlite::QSourceHighliter(editor->document());
	highliter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeXML);

	layout->addWidget(editor);

	setLayout(layout);

	QObject::connect(editor, &QTextEdit::textChanged, this, &ConfigQView::textUpdated);
	QObject::connect(ProjectManager::getInstance(), &ProjectManager::projectChanged, this, &ConfigQView::updateText);

}

ConfigQView::~ConfigQView() {

}

void ConfigQView::textUpdated() {
	ResourceManager::getInstance()->setConfigXML(editor->toPlainText().toStdString());
}

void ConfigQView::updateText() {
	editor->setPlainText(ResourceManager::getInstance()->configXML().c_str());
}
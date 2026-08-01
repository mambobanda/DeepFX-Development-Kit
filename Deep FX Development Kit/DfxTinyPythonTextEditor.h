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

#include <sstream>
#include <DfxGlobal.h>
#include <QTextCursor>
#include <QtWidgets\QTextEdit>
#include "DfxQPythonSyntaxHighlighter.h"

class DfxTinyPythonTextEditor : public QTextEdit {
	Q_OBJECT

private slots:
	void textFormattingAction();
protected:
	void keyPressEvent(QKeyEvent *) override;
	int countTabs(QString str);
private:
	int lastKey;
	QString lastSentence;
	DfxQPythonSyntaxHighlighter *pythonSyntaxHighlighter;
	size_t lineNumber();
public:

	DfxTinyPythonTextEditor(QWidget *parent = Q_NULLPTR);
};
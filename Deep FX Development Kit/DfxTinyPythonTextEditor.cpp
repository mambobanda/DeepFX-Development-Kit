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


#include "DfxTinyPythonTextEditor.h"

DfxTinyPythonTextEditor::DfxTinyPythonTextEditor(QWidget *parent): QTextEdit(parent)
{
	lastKey = 0;
	pythonSyntaxHighlighter = new DfxQPythonSyntaxHighlighter(this->document());

	QObject::connect(this, &QTextEdit::textChanged,
		this, &DfxTinyPythonTextEditor::textFormattingAction);

	lastSentence = "";
}

void DfxTinyPythonTextEditor::textFormattingAction() {
	if (lastKey == Qt::Key_Return) {

		if (toPlainText().endsWith(":\n")) {
			int tabs = countTabs(lastSentence) + 1;
			std::stringstream ss;

			for (int i = 0; i < tabs; i++) ss << "\t";

			insertPlainText(ss.str().c_str());
		}
		else if (toPlainText().endsWith("\n")) {
			int tabs = countTabs(lastSentence);
			std::stringstream ss;

			for (int i = 0; i < tabs; i++) ss << "\t";
			insertPlainText(ss.str().c_str());
		}
	}

	lastKey = 0;
}

void DfxTinyPythonTextEditor::keyPressEvent(QKeyEvent *e) {
	
	if (e->key() == Qt::Key_Return) {
		QStringList lines = toPlainText().split("\n");
		if (lines.size() > 0) {
			lastSentence = lines[lines.size() - 1];
		}
		else {
			lastSentence = "";
		}
	}

	lastKey = e->key();
	QTextEdit::keyPressEvent(e);
}

int DfxTinyPythonTextEditor::countTabs(QString str) {
	
	int count = 0;
	for (int inx = 0; inx < str.length(); inx++) {
		if (str[inx] == '\t') {
			count++;
		}
		else {
			return count;
		}
	}

	return count;
}
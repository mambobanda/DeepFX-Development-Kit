/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include <QBoxLayout>
#include <QSplitter>
#include <QTreeView>
#include <vector>
#include <QColor>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "DfxGraphEditorCommon.h"

class DfxGraphEditor: public QWidget {
	Q_OBJECT
private:

	class Seperator : public QFrame {
	public:
		Seperator();
	};


private:

	QBoxLayout *_mainLayout;
	QBoxLayout *_toolbarLayout;

	QTreeView *_channels;
	DfxChannelGraphView *_graphView;
	QSplitter *_mainSplitter;

	QSpinBox *_frame;
	QDoubleSpinBox *_value;

	QPushButton *_manipulator;
	QPushButton *_createKey;
	QPushButton *_removeKey;

	QPushButton *_steppedInterpolation;
	QPushButton *_linearInterpolation;
	QPushButton *_beizerInterpolation;
	QPushButton *_resetTangent;

	void styleSpinners();
	void styleButton(QPushButton *pbutton, bool checkedCss = false);

	int _cv_point_size;
	int _legend_border;
	int _bar_h;

private:
	void uncheckButtons();
public:

	DfxGraphEditor();
	virtual ~DfxGraphEditor();

signals:
	void selectionChanged(std::vector<std::pair<std::string, QColor>> paths);

public slots:
	void expandAllTree();
	void channelSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
	void newChannelSelection();
	void channelModelChanged();
	void useConstantInterpolation();
	void useLinearInterpolation();
	void useBeizerInterpolation();
	void switchToSelectionMode();
	void switchToAddKeyMode();
	void switchToRemoveKeyMode();
};
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

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPainter>
#include <DfxGlobal.h>
#include <QPropertyAnimation>


class DfxControlPanel: public QWidget {
	Q_OBJECT
private:
	class HeaderWidget : public QWidget {
	private:
		DfxControlPanel *controlPanelPtr;
		bool mousePressed;
	protected:
		void mouseReleaseEvent(QMouseEvent *event) override;
		void mousePressEvent(QMouseEvent *event) override;
		void paintEvent(QPaintEvent *event);
	public:
		QColor bgColor;
		HeaderWidget(DfxControlPanel *cpanel, QWidget *parent = nullptr);
	};

private:
	QBoxLayout *mainLayout;
	QBoxLayout *headerLayout;
	QBoxLayout *bodyLayout;

	HeaderWidget *header;
	QWidget *body;

	QLabel  *header_icon;
	QLabel  *header_title;

	QWidget *_body_widget;

	int preferredWidth;
	int preferredHeight;

public:

	DfxControlPanel(QWidget *parent=nullptr);
	~DfxControlPanel();

	void setWidget(QWidget *body_widget);

	void setTitle(QString title);

	void setHeaderWidth(int wdth);
	void setHeaderHeight(int heght);

	void setHeaderColor(QColor color);

	void hideBody(bool);

	bool isBodyHidden();

	QWidget *bodyWidget();
	QLabel  *headerIcon();
};
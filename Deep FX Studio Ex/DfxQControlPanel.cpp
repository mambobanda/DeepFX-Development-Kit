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

#include "DfxQControlPanel.h"

DfxControlPanel::DfxControlPanel(QWidget *parent):QWidget(parent) {
	_body_widget = nullptr;

	mainLayout = new QVBoxLayout;
	headerLayout = new QHBoxLayout;
	bodyLayout = new QVBoxLayout;

	mainLayout->setMargin(0);
	headerLayout->setMargin(0);
	bodyLayout->setMargin(0);

	mainLayout->setAlignment(Qt::AlignTop);
	bodyLayout->setAlignment(Qt::AlignTop);

	header = new HeaderWidget(this);
	body = new QWidget();

	header_icon = new QLabel;
	header_title = new QLabel;

	header_icon->setPixmap(QPixmap(":/icon/triangle_arrow_down").scaled(7,7));

	headerLayout->setAlignment(Qt::AlignLeft);
	headerLayout->addSpacing(5);
	headerLayout->addWidget(header_icon);
	headerLayout->addWidget(header_title);

	body->setLayout(bodyLayout);
	header->setLayout(headerLayout);

	mainLayout->addWidget(header);
	mainLayout->addWidget(body);

	preferredWidth  = 0;
	preferredHeight = 0;

	header_title->setText("Default Header");

	header_icon->setStyleSheet(
		"QLabel{"
		"background-color:transparent ;"
		"border-style: solid;"
		"border-width: 0px;"
		"}"
	);

	header_title->setStyleSheet(
		"QLabel{"
		"background-color:transparent ;"
		"border-style: solid;"
		"border-width: 0px;"
		"}"
	);

	setLayout(mainLayout);
}

DfxControlPanel::~DfxControlPanel() {

}

void DfxControlPanel::setWidget(QWidget *body_widget) {
	
	if (_body_widget != nullptr) {
		bodyLayout->removeWidget(_body_widget);
		delete _body_widget;
		_body_widget = nullptr;
	}

	_body_widget = body_widget;
	bodyLayout->addWidget(body_widget);
}

void DfxControlPanel::setTitle(QString title) {
	header_title->setText(title);
}

void DfxControlPanel::setHeaderWidth(int wdth) {
	preferredWidth = wdth;
}

void DfxControlPanel::setHeaderHeight(int heght) {
	preferredHeight = heght;
}

void DfxControlPanel::setHeaderColor(QColor color) {
	header->bgColor = color;
}

void DfxControlPanel::hideBody(bool val) {
	if (val) 
		body->hide();
	else 
		body->show();
}

bool  DfxControlPanel::isBodyHidden() {
	return (!body->isVisible());
}

QWidget *DfxControlPanel::bodyWidget() {
	return _body_widget;
}

QLabel *DfxControlPanel::headerIcon() {
	return header_icon;
}

DfxControlPanel::HeaderWidget::HeaderWidget(DfxControlPanel *cpanel, QWidget *parent) {
	controlPanelPtr = cpanel;

	setStyleSheet(
		"QWidget{"
		"background-color:rgb(44, 44, 44);"
		"border-color:rgb(60, 60, 60);"
		"border-style: solid;"
		"border-width: 1px;"
		"}");

	setFixedHeight(18);
}

void DfxControlPanel::HeaderWidget::mouseReleaseEvent(QMouseEvent *event) {
	
	if (mousePressed) {
		if (controlPanelPtr->isBodyHidden()) {
			controlPanelPtr->bodyWidget()->setMaximumHeight(16777215);
			controlPanelPtr->hideBody(false);

			controlPanelPtr->headerIcon()->setPixmap(QPixmap(":/icon/triangle_arrow_down").scaled(7, 7));
		}
		else {
			QVariantAnimation *animation = new QVariantAnimation();
			animation->setDuration(200);
			animation->setStartValue(controlPanelPtr->bodyWidget()->height() + 2);
			animation->setEndValue(0);

			controlPanelPtr->bodyWidget()->setMaximumHeight(controlPanelPtr->bodyWidget()->height() + 2);
			connect(animation, &QVariantAnimation::valueChanged, [=](const QVariant &value) {
				controlPanelPtr->bodyWidget()->setMaximumHeight(value.toInt());
			});

			connect(animation, &QVariantAnimation::finished, [=]() {
				controlPanelPtr->hideBody(true);
				controlPanelPtr->headerIcon()->setPixmap(QPixmap(":/icon/triangle_arrow_right").scaled(7, 7));
			});

			animation->start();
		}

		mousePressed = false;
	}
}

void DfxControlPanel::HeaderWidget::mousePressEvent(QMouseEvent *event) {
	mousePressed = true;
}

void DfxControlPanel::HeaderWidget::paintEvent(QPaintEvent *event) {
	//QPainter painter(this);
	//QBrush brush(bgColor, Qt::SolidPattern);

	//int w = width();
	//int h = height();

	//painter.drawRect(0, 0, w, h);
}
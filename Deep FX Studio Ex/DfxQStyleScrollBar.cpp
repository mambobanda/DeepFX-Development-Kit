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


#include "DfxQStyleScrollBar.h"

namespace style {

	void styleQScrollBar(QScrollBar *scrollbar) {
		scrollbar->setStyleSheet(
			"QScrollBar:horizontal{"
			"border: 0px solid grey;"
			"background: rgba(0,0,0,0);"
			"height: 10px;"
			"margin: 0px 0px 0 0px;"
			"}");

		scrollbar->setStyleSheet(scrollbar->styleSheet() +
			"QScrollBar:vertical{"
			"border: 0px solid grey;"
			"background: rgba(0,0,0,0);"
			"width: 10px;"
			"margin: 0px 0px 0 0px;"
			"}");

		scrollbar->setStyleSheet(scrollbar->styleSheet() +
			"QScrollBar::handle:horizontal{"
			"background: rgba(150,150,150);"
			"border-radius: 5px;"
			"min-width: 30px;"
			"}");

		scrollbar->setStyleSheet(scrollbar->styleSheet() +
			"QScrollBar::handle:vertical{"
			"background: rgba(150,150,150);"
			"border-radius: 5px;"
			"min-height: 30px;"
			"}");

		scrollbar->setStyleSheet(scrollbar->styleSheet() +
			"QScrollBar::add-line:horizontal{"
			"border: none;"
			"background: none;"
			"height: 0px;"
			"}");

		scrollbar->setStyleSheet(scrollbar->styleSheet() +
			"QScrollBar::add-line:vertical{"
			"border: none;"
			"background: none;"
			"height: 0px;"
			"}");

		scrollbar->setStyleSheet(scrollbar->styleSheet() +
			"QScrollBar::sub-line:horizontal{"
			"border: none;"
			"background: none;"
			"height: 0px;"
			"}");

		scrollbar->setStyleSheet(scrollbar->styleSheet() +
			"QScrollBar::sub-line:vertical{"
			"border: none;"
			"background: none;"
			"height: 0px;"
			"}");

		scrollbar->setStyleSheet(scrollbar->styleSheet() +
			"QScrollBar:left-arrow:horizontal,QScrollBar::right-arrow:horizontal{"
			"border: none;"
			"width: 0px;"
			"background: none;"
			"height: 0px;"
			"}");

		scrollbar->setStyleSheet(scrollbar->styleSheet() +
			"QScrollBar:left-arrow:vertical,QScrollBar::right-arrow:vertical{"
			"border: none;"
			"width: 0px;"
			"background: none;"
			"height: 0px;"
			"}");

		scrollbar->setStyleSheet(scrollbar->styleSheet() +
			"QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{"
			"height: 0px;"
			"}");

		scrollbar->setStyleSheet(scrollbar->styleSheet() +
			"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{"
			"height: 0px;"
			"}");
	}

}
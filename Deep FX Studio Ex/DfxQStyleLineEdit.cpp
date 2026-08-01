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

#include "DfxQStyleLineEdit.h"

namespace style {
	void styleQLineEdit(QLineEdit *ledit) {
		ledit->setStyleSheet(
			"QLineEdit{ "
			"color: white;"
			"background-color: rgb(20,20,20);"
			"border: 0px solid;"
			"border-color: rgb(50,50,50);"
			"border-width: 2px;"
			"border-style: solid;"
			"border-radius: 4px;"
			"padding-left: 5px;"
			"}");

		ledit->setStyleSheet(ledit->styleSheet() +
			"QLineEdit:hover { "
			"border-width: 1px;"
			"border-style: solid;"
			"border-color: rgb(255,127,42);"
			"}");


		ledit->setStyleSheet(ledit->styleSheet() +
			"QLineEdit:selected{"
			"color: black;"
			"padding-left: 20px;"
			"font-weight: bold; "
			"background: rgb(255,127,42);"
			"}");

	}
}
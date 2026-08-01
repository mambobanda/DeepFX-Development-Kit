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

#include <QString>


namespace style {

	inline QString leftButtonSty() {
		return "QPushButton { "
			"color: white;"
			"background-color: rgb(70,70,70);"
			"border: 0px solid;"
			"border-bottom-left-radius: 4px;"
			"border-top-left-radius: 4px;"
			"padding-left: 2px;"
			"height: 16px;"
			"}";
	}

	inline QString buttonSty() {
		return "QPushButton { "
			"color: white;"
			"background-color: rgb(70,70,70);"
			"border: 0px solid;"
			"padding-left: 2px;"
			"height: 16px;"
			"}";
	}

	inline QString rightButtonSty() {
		return "QPushButton { "
			"color: white;"
			"background-color: rgb(70,70,70);"
			"border: 0px solid;"
			"border-bottom-right-radius: 4px;"
			"border-top-right-radius: 4px;"
			"padding-left: 2px;"
			"height: 16px;"
			"}";
	}

	inline QString hoverSty() {
		return "QPushButton:hover { "
			"background-color: rgb(100,100,100);"
			"}";
	}

	inline QString hoverPressdSty() {
		return "QPushButton:hover:pressed { "
			"background-color: rgb(255, 153, 83);"
			"}";
	}
}
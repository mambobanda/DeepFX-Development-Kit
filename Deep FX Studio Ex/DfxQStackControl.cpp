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

#include "DfxQStackControl.h"


DfxQStackControl::DfxQStackControl() {
	_currentPage = nullptr;
	_layout = new QVBoxLayout();

	_layout->setContentsMargins(0, 0, 0, 0);

	setLayout(_layout);
}

DfxQStackControl::~DfxQStackControl() {

}

void DfxQStackControl::addPage(QWidget *page) {
	_pages.push_back(page);
}

void DfxQStackControl::removePage(QWidget *page) {
	auto itr = _pages.begin();

	while (itr != _pages.end()) {
		if ((*itr) == page) {
			if (page == _currentPage) _currentPage = nullptr;
			_pages.erase(itr);
			return;
		}
	}
}

QWidget *DfxQStackControl::currentPage() {
	return _currentPage;
}

void DfxQStackControl::changeCurrentPage(int index) {

	if (_currentPage != nullptr) {
		_layout->removeWidget(_currentPage);
		_currentPage->hide();
		_currentPage = nullptr;
	}

	_currentPage = _pages[index];
	_layout->addWidget(_currentPage);
	
	_currentPage->show();


	//adjustSize();

}
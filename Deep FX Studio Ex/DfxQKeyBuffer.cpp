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

#include "DfxQKeyBuffer.h"

void DfxQKeyBuffer::addKey(unsigned int key) {
	_keys.push_back(key);
}

void DfxQKeyBuffer::removeKey(unsigned int key) {
	_keys.erase(std::remove(_keys.begin(), _keys.end(), key), _keys.end());
}

bool DfxQKeyBuffer::hasKey(unsigned int key) {
	auto itr = std::find(_keys.begin(), _keys.end(), key);

	if (itr == _keys.end()) return false;
	else return true;
}

std::vector<unsigned int> &DfxQKeyBuffer::keys() {
	return _keys;
}

DfxShortcutsDB::Shortcut DfxQKeyBuffer::keysToShortcut() {
	DfxShortcutsDB::Shortcut scut;

	if (_keys.size() == 1) {
		scut.k00 = _keys[0];
	}
	else if (_keys.size() == 2) {
		scut.k00 = _keys[0];
		scut.k01 = _keys[1];
	}
	else if (_keys.size() == 3) {
		scut.k00 = _keys[0];
		scut.k01 = _keys[1];
		scut.k02 = _keys[2];
	}
	else if (_keys.size() == 4) {
		scut.k00 = _keys[0];
		scut.k01 = _keys[1];
		scut.k02 = _keys[2];
		scut.k03 = _keys[3];
	}

	return scut;
}

std::vector<unsigned int> DfxQKeyBuffer::keys4() {

	std::vector<unsigned int> f;
	f.push_back(0);
	f.push_back(0);
	f.push_back(0);
	f.push_back(0);

	if (_keys.size() == 1) {
		f[0] = _keys[0];
	}
	else if (_keys.size() == 2) {
		f[0] = _keys[0];
		f[1] = _keys[1];
	}
	else if (_keys.size() == 3) {
		f[0] = _keys[0];
		f[1] = _keys[1];
		f[2] = _keys[2];
	}
	else if (_keys.size() == 4) {
		f[0] = _keys[0];
		f[1] = _keys[1];
		f[2] = _keys[2];
		f[3] = _keys[3];
	}
	
	return f;
}

int DfxQKeyBuffer::filter(int k) {

	if (k == Qt::Key_Control) {
		return Qt::CTRL;
	}
	else if (k == Qt::Key_Alt) {
		return Qt::ALT;
	}
	else if (k == Qt::Key_Shift) {
		return Qt::SHIFT;
	}
	else {
		return k;
	}
}

std::string DfxQKeyBuffer::keysToString() {

	if (_keys.size() == 1) {
		return QKeySequence(filter(_keys[0])).toString().toStdString();
	}
	else if (_keys.size() == 2) {
		return QKeySequence(filter(_keys[0]) + filter(_keys[1])).toString().toStdString();
	}
	else if (_keys.size() == 3) {
		return QKeySequence(filter(_keys[0]) + filter(_keys[1]) + filter(_keys[2])).toString().toStdString();
	}
	else if (_keys.size() == 4) {
		return QKeySequence(filter(_keys[0]) + filter(_keys[1]) + filter(_keys[2]) + filter(_keys[3])).toString().toStdString();
	}
	else {
		return QKeySequence().toString().toStdString();
	}
}
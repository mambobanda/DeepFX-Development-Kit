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

#include "DfxShortcutsDB.h"


DfxShortcutsDB *DfxShortcutsDB::_instance = nullptr;


DfxShortcutsDB::Shortcut::Shortcut() {
	k00 = 0;
	k01 = 0;
	k02 = 0;
	k03 = 0;
	type = 0;
	mouseButton = 0;
}

std::string DfxShortcutsDB::Shortcut::toString() {
	return QKeySequence(filter(k00) + filter(k01) + filter(k02) + filter(k03)).toString().toStdString();
}

bool DfxShortcutsDB::Shortcut::operator==(Shortcut &scut) {
	if (k00 == scut.k00 && k01 == scut.k01 && k02 == scut.k02 && k03 == scut.k03) return true;
	return false;
}

int DfxShortcutsDB::Shortcut::filter(int k) {

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

DfxShortcutsDB::DfxShortcutsDB() {

}

DfxShortcutsDB *DfxShortcutsDB::getInstance() {

	if (_instance == nullptr) {
		_instance = new DfxShortcutsDB;
	}

	return _instance;
}

void DfxShortcutsDB::addGroup(std::string name, std::string category) {
	DfxShortcutsDB::Group group;
	group.name = name;
	group.category = category;
	std::pair<std::string, DfxShortcutsDB::Group> p(name, group);
	groups.insert(p);
}

void DfxShortcutsDB::addShortcut(std::string group, std::string name, Command command) {

	auto itrGroup = groups.find(group);

	if (itrGroup != groups.end()) {

		DfxShortcutsDB::Shortcut scut;
		scut.name = name;
		scut.command = command;

		std::pair<std::string, DfxShortcutsDB::Shortcut> p(name, scut);
		itrGroup->second.shortcuts.insert(p);
	}
}
bool DfxShortcutsDB::getShortcut(std::string group, std::string name, DfxShortcutsDB::Shortcut &shortcut) {

	auto itrGroup = groups.find(group);

	if (itrGroup != groups.end()) {
		auto itrShortcut = itrGroup->second.shortcuts.find(name);

		if (itrShortcut != itrGroup->second.shortcuts.end()) {
			shortcut = itrShortcut->second;
			return true;
		}
	}

	return false;
}

bool DfxShortcutsDB::setHotkey(std::string group, std::string name, unsigned int type, int t00, int t01, int t02, int t03, unsigned int mouseButton) {
	auto itrGroup = groups.find(group);

	if (itrGroup != groups.end()) {
		auto itrShortcut = itrGroup->second.shortcuts.find(name);

		if (itrShortcut != itrGroup->second.shortcuts.end()) {
			itrShortcut->second.type = type;
			itrShortcut->second.k00 = t00;
			itrShortcut->second.k01 = t01;
			itrShortcut->second.k02 = t02;
			itrShortcut->second.k03 = t03;
			itrShortcut->second.mouseButton = mouseButton;
			return true;
		}
	}

	return false;
}

bool DfxShortcutsDB::executeCommand(std::string group, Shortcut keyboardSequence, unsigned int mouseButton) {
	auto itrGroup = groups.find(group);

	if (itrGroup != groups.end()) {

		for (auto &scut : itrGroup->second.shortcuts) {

			if (scut.second == keyboardSequence &&
				scut.second.mouseButton == mouseButton) {

				scut.second.command();
				return true;
			}
		}
	}

	return false;
}

bool DfxShortcutsDB::findGroup(std::string name, DfxShortcutsDB::Group &group) {
	auto itrGroup = groups.find(name);

	if (itrGroup != groups.end()) {
		group = itrGroup->second;
		return true;
	}

	return false;
}

bool DfxShortcutsDB::findShortcut(DfxShortcutsDB::Group group, std::string name, DfxShortcutsDB::Shortcut &shortcut) {
	auto itrShortcut = group.shortcuts.find(name);

	if (itrShortcut != group.shortcuts.end()) {
		shortcut = itrShortcut->second;
		return true;
	}

	return false;
}

std::set<std::string> DfxShortcutsDB::findAllCategories() {
	std::set<std::string> ctgs;

	for (auto group : groups) {
		ctgs.insert(group.second.category);
	}

	return ctgs;
}

std::vector<DfxShortcutsDB::Group> DfxShortcutsDB::findGroupsInCategory(std::string category) {
	std::vector<DfxShortcutsDB::Group> gps;

	for (auto group : groups) {
		QString ctgs(category.c_str());

		if (ctgs.compare(group.second.category.c_str(), Qt::CaseSensitivity::CaseInsensitive) == 0) {
			gps.push_back(group.second);
		}
	}

	return gps;
}

std::vector<DfxShortcutsDB::Group> DfxShortcutsDB::findAllGroups(){
	std::vector<DfxShortcutsDB::Group> gps;

	for (auto grp : groups)
		gps.push_back(grp.second);

	return gps;
}

bool DfxShortcutsDB::match(std::string group, std::string name, unsigned int type, int t00, int t01, int t02, int t03, unsigned int mouseButton) {
	
	DfxShortcutsDB::Group grp;

	if (findGroup(group, grp)) {
		DfxShortcutsDB::Shortcut shrt;

		if (findShortcut(grp, name, shrt)) {

			if (shrt.type == type &&
				shrt.k00 == t00 &&
				shrt.k01 == t01 &&
				shrt.k02 == t02 &&
				shrt.k03 == t03 &&
				shrt.mouseButton == mouseButton) {
				return true;
			}
		}
	}

	return false;
}
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

#pragma once

#include <string>
#include <vector>
#include <set>
#include <QKeySequence>
#include <unordered_map>
#include <functional>
#include <DfxGlobal.h>

class DfxShortcutsDB {
public:
	using Command = std::function<void()>;

	enum {MOUSE=0x60, KEYBOARD=0x61, KEYBOARD_MOUSE=0x62};

	class Shortcut {
	public:
		unsigned int type;
		std::string name;
		Command command;
		int k00;
		int k01;
		int k02;
		int k03;
		unsigned int mouseButton;
		
		Shortcut();
		std::string toString();
		static int filter(int k);

		bool operator==(Shortcut &scut);
	};

	class Group {
	public:
		std::string name;
		std::string category;
		std::unordered_map<std::string, Shortcut> shortcuts;
	};

private:
	static DfxShortcutsDB *_instance;
	std::unordered_map<std::string, Group> groups;

	DfxShortcutsDB();
public:

	static DfxShortcutsDB *getInstance();

	void addGroup(std::string name, std::string category);
	void addShortcut(std::string group, std::string name, Command command);
	bool getShortcut(std::string group, std::string name, DfxShortcutsDB::Shortcut &shortcut);

	bool setHotkey(std::string group, std::string name, unsigned int type, int t00 = 0, int t01 = 0, int t02 = 0, int t03 = 0, unsigned int mouseButton = 0);

	bool executeCommand(std::string group, Shortcut keyboardSequence, unsigned int mouseButton = 0);
	
	bool findGroup(std::string name, DfxShortcutsDB::Group &group);
	bool findShortcut(DfxShortcutsDB::Group group, std::string name, DfxShortcutsDB::Shortcut &shortcut);

	bool match(std::string group, std::string name, unsigned int type, int t00 = 0, int t01 = 0, int t02 = 0, int t03 = 0, unsigned int mouseButton = 0);

	std::set<std::string> findAllCategories();
	std::vector<DfxShortcutsDB::Group> findGroupsInCategory(std::string category);
	std::vector<DfxShortcutsDB::Group> findAllGroups();
};
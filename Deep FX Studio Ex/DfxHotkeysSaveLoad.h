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

#include <QDomDocument> 
#include <String>
#include <SStream>
#include <QFile>
#include <QTextStream>
#include "DfxCommon.h"
#include "DfxShortcutsDB.h"

class DfxHotkeysSaveLoad {
public:
	static void load(QString path);
	static void save(QString path);
};
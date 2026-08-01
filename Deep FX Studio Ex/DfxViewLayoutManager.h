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

#include <QXmlStreamWriter>
#include <QTextStream> 
#include <QFile> 
#include <vector>
#include <QDir>
#include <QFile>
#include <QDomDocument>
#include <QMessageBox>
#include <QStandardPaths>
#include "DfxQViewManager.h"
#include "DfxQViewID.h"
#include "DfxQViewMap.h"


class DfxViewLayoutManager {
public:

	class DfxViewLayoutItem {
	public:
		DfxQViewID viewid;
		bool hasView;
		int direction;
		int sizeA;
		int sizeB;
		DfxViewLayoutItem *a;
		DfxViewLayoutItem *b;
		std::string path;
	};

	class DfxViewLayout {
	public:
		DfxViewLayoutItem *root;

		DfxViewLayout();
		virtual ~DfxViewLayout();

		void clear(DfxViewLayoutItem *item);
		void mapCurrentLayout(DfxQViewContainer *cnt, DfxViewLayoutItem *item);
	};

private:

	static DfxViewLayoutItem *loadCurrentContainer(QDomNode &node);
	static void applyLayout(DfxQViewContainer *cnt, DfxViewLayoutItem *item);

public:

	static std::vector<std::pair<std::string, DfxViewLayout*>> _layouts;

public:

	static void clear();
	static void switchLayout(DfxViewLayout &);
	static void switchLayout(std::string);
	static std::pair<std::string, DfxViewLayout*> loadLayout(std::string path);
	static void saveLayout(std::string name, DfxViewLayout *layout, std::string path);
	static std::string currentLayoutToString();
	static void findLayoutConfigs();
	static bool removeLayout(int index);
	static void writeCurrentLayout(DfxViewLayoutItem *item, QXmlStreamWriter &stream);
};
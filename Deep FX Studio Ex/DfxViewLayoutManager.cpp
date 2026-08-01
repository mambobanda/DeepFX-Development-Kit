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

#include "DfxViewLayoutManager.h"

std::vector<std::pair<std::string, DfxViewLayoutManager::DfxViewLayout*>> DfxViewLayoutManager::_layouts;

DfxViewLayoutManager::DfxViewLayout::DfxViewLayout() {
	root = nullptr;
}

DfxViewLayoutManager::DfxViewLayout::~DfxViewLayout() {
}

void DfxViewLayoutManager::DfxViewLayout::clear(DfxViewLayoutItem *item) {

	if (!item->hasView) {
		clear(item->a);
		clear(item->b);
	}

	delete item;
}

void DfxViewLayoutManager::DfxViewLayout::mapCurrentLayout(DfxQViewContainer *cnt, DfxViewLayoutItem *item) {

	if (cnt->getView() != nullptr) {
		item->hasView = true;
		item->a = nullptr;
		item->b = nullptr;
		item->viewid = cnt->getViewID();
		item->direction = 0;
	}
	else {
		item->hasView = false;
		item->a = new DfxViewLayoutItem;
		item->b = new DfxViewLayoutItem;
		item->direction = cnt->getSplitter()->orientation();

		auto sizes = cnt->getSplitter()->sizes();

		item->sizeA = sizes[0];
		item->sizeB = sizes[1];

		mapCurrentLayout(cnt->getA(), item->a);
		mapCurrentLayout(cnt->getB(), item->b);
	}
}

void DfxViewLayoutManager::writeCurrentLayout(DfxViewLayoutItem *item, QXmlStreamWriter &stream) {
	stream.writeStartElement("container");
	if (item->hasView) {
		stream.writeTextElement("view", "yes");
		stream.writeTextElement("viewid", viewIDToString(item->viewid).c_str());
	}
	else {
		stream.writeTextElement("view", "no");
		stream.writeTextElement("viewid", viewIDToString(DfxQViewID::NONE).c_str());
		stream.writeTextElement("size00", std::to_string(item->sizeA).c_str());
		stream.writeTextElement("size01", std::to_string(item->sizeB).c_str());
		stream.writeTextElement("direction", std::to_string(item->direction).c_str());

		writeCurrentLayout(item->a, stream);
		writeCurrentLayout(item->b, stream);
	}
	stream.writeEndElement();
}

DfxViewLayoutManager::DfxViewLayoutItem *DfxViewLayoutManager::loadCurrentContainer(QDomNode &node) {

	bool hasView = false;
	std::string view = "no";
	DfxQViewID viewid = DfxQViewID::NONE;
	int size00 = 0;
	int size01 = 0;
	int direction = 0;
	DfxViewLayoutManager::DfxViewLayoutItem *a = nullptr;
	DfxViewLayoutManager::DfxViewLayoutItem *b = nullptr;
 
	if (node.isNull()) 
		return nullptr;

	auto nodeElement = node.toElement();

	if (nodeElement.isNull()) 
		return nullptr;

	if (nodeElement.tagName().compare("container", Qt::CaseInsensitive) != 0) 
		return nullptr;

	auto childNode = node.firstChild();

	int cnt = 0;
	while (!childNode.isNull())
	{
		auto childElement = childNode.toElement();
		
		if (childElement.isNull()) {
			if (a != nullptr) delete a;
			if (b != nullptr) delete b;
			return nullptr;
		}

		if (childElement.tagName().compare("view", Qt::CaseInsensitive) == 0) {
			view = childElement.text().toStdString();

			if (view == "yes")
				hasView = true;
		}
		else if (childElement.tagName().compare("viewid", Qt::CaseInsensitive) == 0) {
			viewid = stringToViewID(childElement.text().toStdString());
		}
		else if (childElement.tagName().compare("size00", Qt::CaseInsensitive) == 0) {
			size00 = childElement.text().toInt();
		}
		else if (childElement.tagName().compare("size01", Qt::CaseInsensitive) == 0) {
			size01 = childElement.text().toInt();
		}
		else if (childElement.tagName().compare("direction", Qt::CaseInsensitive) == 0) {
			direction = childElement.text().toInt();
		}
		else if (childElement.tagName().compare("container", Qt::CaseInsensitive) == 0) {

			if (!hasView) {
				if (cnt == 0) {
					a = loadCurrentContainer(childNode);
				}
				else if (cnt == 1) {
					b = loadCurrentContainer(childNode);
				}
				else {
					if (a != nullptr) delete a;
					if (b != nullptr) delete b;
					return nullptr;
				}
			}
			cnt++;
		}

		childNode = childNode.nextSibling();
	}
	
	auto *item = new DfxViewLayoutManager::DfxViewLayoutItem;
	item->hasView = hasView;
	item->viewid = viewid;
	item->sizeA = size00;
	item->sizeB = size01;
	item->direction = direction;
	item->a = a;
	item->b = b;

	return item;
}

std::pair<std::string, DfxViewLayoutManager::DfxViewLayout*> DfxViewLayoutManager::loadLayout(std::string path) {

	std::string layoutName = "";
	QDomDocument doc("doc");
	QFile file(path.c_str());

	if (!file.open(QIODevice::ReadOnly)) return std::pair<std::string, DfxViewLayoutManager::DfxViewLayout*>("", nullptr);

	if (!doc.setContent(&file)) {
		file.close();
		return std::pair<std::string, DfxViewLayoutManager::DfxViewLayout*>("", nullptr);;
	}

	QDomElement layoutElement = doc.documentElement();
	if (layoutElement.isNull()) {
		file.close();
		return std::pair<std::string, DfxViewLayoutManager::DfxViewLayout*>("", nullptr);;
	}

	layoutName = layoutElement.attribute("name", "N/A").toStdString();

	auto item = loadCurrentContainer(layoutElement.firstChild());

	file.close();

	DfxViewLayout *litem = new DfxViewLayout;
	litem->root = item;
	litem->root->path = path;

	return std::pair<std::string, DfxViewLayoutManager::DfxViewLayout*>(layoutName, litem);
}

void DfxViewLayoutManager::saveLayout(std::string name, DfxViewLayoutManager::DfxViewLayout *layout, std::string path) {
	
	QFile file(path.c_str());
	
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&file);

	QXmlStreamWriter stream(out.device());
	stream.setAutoFormatting(true);
	stream.writeStartDocument();

	stream.writeStartElement("layout");
	stream.writeAttribute("name", name.c_str());
	writeCurrentLayout(layout->root, stream);
	stream.writeEndElement();
	
	stream.writeEndDocument();

	file.close();
}

std::string DfxViewLayoutManager::currentLayoutToString() {
	DfxViewLayoutManager::DfxViewLayout lt;
	lt.root = new DfxViewLayoutManager::DfxViewLayoutItem;
	lt.mapCurrentLayout(DfxQViewManager::getInstance()->root(), lt.root);

	QString output;
	QXmlStreamWriter stream(&output);
	stream.setAutoFormatting(true);
	stream.writeStartDocument();

	stream.writeStartElement("layout");
	stream.writeAttribute("name", "default");
	DfxViewLayoutManager::writeCurrentLayout(lt.root, stream);
	stream.writeEndElement();

	return output.toStdString();
}

void DfxViewLayoutManager::clear() {
	
	for (auto lt: _layouts) {
		lt.second->clear(lt.second->root);
		delete lt.second;
	}

	_layouts.clear();
}

void DfxViewLayoutManager::applyLayout(DfxQViewContainer *cnt, DfxViewLayoutManager::DfxViewLayoutItem *item) {

	if (item->hasView) {
		bool result = false;
		auto vptr = DfxQViewMap::getInstance()->findEntry(item->viewid, result);

		if (!result) return;

		auto name = DfxQViewMap::getInstance()->findEntryName(item->viewid);

		DfxQView *view = new DfxQView();
		view->setTitle(name.c_str());
		view->setCentralWidget(vptr());

		cnt->setView(view);
		cnt->setViewID(item->viewid);
	}
	else {
		Qt::Orientation dir = (item->direction == 1) ? Qt::Orientation::Horizontal : Qt::Orientation::Vertical;

		DfxQViewContainer *containerA = new DfxQViewContainer;
		DfxQViewContainer *containerB = new DfxQViewContainer;

		QSize ratio(item->sizeA, item->sizeB);

		cnt->newSplitter(dir, containerA, containerB, ratio);

		applyLayout(containerA, item->a);
		applyLayout(containerB, item->b);
	}
}

void DfxViewLayoutManager::switchLayout(DfxViewLayout &layout) {

	DfxQViewManager::getInstance()->clear(DfxQViewManager::getInstance()->root());

	applyLayout(DfxQViewManager::getInstance()->root(), layout.root);
}

void DfxViewLayoutManager::switchLayout(std::string str) {
	QDomDocument doc("doc");
	doc.setContent(QString(str.c_str()));

	QDomElement layoutElement = doc.documentElement();
	if (!layoutElement.isNull()) {
		auto item = loadCurrentContainer(layoutElement.firstChild());

		DfxQViewManager::getInstance()->clear(DfxQViewManager::getInstance()->root());

		applyLayout(DfxQViewManager::getInstance()->root(), item);
	}
}

void DfxViewLayoutManager::findLayoutConfigs() {
	clear();

	QString appdata = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];

	QDir directory(appdata + "/layout_config");
	QStringList configs = directory.entryList(QStringList() << "*.lt" , QDir::Files);

	for (auto str : configs) {
		auto rc = loadLayout(std::string((appdata + "/layout_config/").toStdString()) + str.toStdString().c_str());

		if (rc.second != nullptr) {
			_layouts.push_back(rc);
		}
	}
}

bool DfxViewLayoutManager::removeLayout(int index) {
	QString appdata = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];

	QString path = QString(appdata + "/layout_config/") + _layouts[index].first.c_str() + ".lt";
	QFile file(path);

	if (!file.exists()) {
		return false;
	}

	file.remove();

	return true;
}
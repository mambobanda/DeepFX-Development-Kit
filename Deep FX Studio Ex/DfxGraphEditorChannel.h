/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include <string>
#include <vector>
#include <QColor>
#include <glm/vec3.hpp>

class DfxGraphEditorChannel {
private:

	std::string _path;
	std::string _name;
	QColor _color;
	unsigned int _type;

	DfxGraphEditorChannel *_parent;
	std::vector<DfxGraphEditorChannel*> _channels;

public:
	
	enum  { NONE=0, DATA_OBJECT=1, ANY=2 };

	DfxGraphEditorChannel();
	virtual ~DfxGraphEditorChannel();

	DfxGraphEditorChannel *parent();
	void setParent(DfxGraphEditorChannel *prt);

	std::vector<DfxGraphEditorChannel*> &channels();
	void addChannel(DfxGraphEditorChannel *channel);
	DfxGraphEditorChannel *findChannelByIndex(int index);
	void removeChannel(std::string pth);
	void removeAllChannels(std::string pth);
	void clear();

	int channelCount();
	int index(std::string pth);
	int relativeIndex();

	void setPath(std::string pth);
	std::string path();

	void setColor(QColor clr);
	QColor color();

	std::string name();
	void setName(std::string name);

	void setType(int typ);
	int type();

};
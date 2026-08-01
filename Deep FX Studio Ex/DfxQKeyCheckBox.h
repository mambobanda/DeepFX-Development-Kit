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

#include <QCheckBox>
#include "DfxSpace.h"

class DfxQKeyCheckBox: public QCheckBox {
	Q_OBJECT
private:
	
	class TimeObserver : public fl::Observer {
	public:
		DfxQKeyCheckBox *ptr;
		TimeObserver(DfxQKeyCheckBox *pt);
		void update(fl::Any *any) override;
	};

	class PathObserver : public fl::Observer {
	public:
		DfxQKeyCheckBox *ptr;
		PathObserver(DfxQKeyCheckBox *pt);
		void update(fl::Any *any) override;
	};

	bool _ignoreSignals;
private:
	std::string _path;

	TimeObserver *_timeObserver;
	PathObserver *_pathObserver;
public:

	DfxQKeyCheckBox(std::string pth, QWidget *parent = Q_NULLPTR);
	virtual ~DfxQKeyCheckBox();

	std::string path();
	void setPath(std::string pth);

	void updateState();

	bool ignoreSignals();
public slots:
	void pathUpdated(std::string pth);
};
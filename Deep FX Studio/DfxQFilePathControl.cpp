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

#include <string>
#include <QFileDialog>
#include "DfxQFilePathControl.h"

DfxQFilePathControl::DfxQFilePathControl(DfxEntity *ent, size_t propID, std::string initial_value,
	float width, QWidget *parent) {

	entity = ent;
	propertyID = propID;

	layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
	layout->setMargin(0);
	layout->setSpacing(0);

	path = new QLineEdit(this);
	selectFile = new QPushButton(QIcon(":/misc/filepath"), "");

	path->setText(((std::string*)ent->getProperty(propID))->c_str());
	layout->addWidget(path);
	layout->addWidget(selectFile);

	setFixedWidth(width);

	setLayout(layout);

	QObject::connect(selectFile, &QPushButton::clicked, this, &DfxQFilePathControl::updateEntity);
	QObject::connect(path, &QLineEdit::editingFinished, this, &DfxQFilePathControl::updateEdited);
}

void DfxQFilePathControl::updateEntity() {

	QSettings settings;
	QString initialDir = settings.value(entity->UniqueName().c_str(), QDir::homePath()).toString();

	QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Choose Image"), initialDir, tr("Image Files (*.png *.jpg *.bmp)"));
	
	if (fileName.size() > 0) {

		if (!entity->getDataInstance()->getPropertyInstance(propertyID)->hasBinding()) {

			if (entity->getDataInstance()->getPropertyInstance(propertyID)->getType() == DfxEntityPropertyTypes::iFILE_PATH_TYPE) {
				*((std::string*)entity->getProperty(propertyID)) = fileName.toStdString();
				path->setText(fileName);

				entity->makeDirty();
				entity->getDataInstance()->getPropertyInstance(propertyID)->dirtyParent();
				DfxGlobal::command::exec("updateViewport");

				settings.value(entity->UniqueName().c_str(), fileName).toString();
			}
		}
	}
}

void DfxQFilePathControl::updateEdited() {

	std::string val = path->text().toStdString();

	if (!entity->getDataInstance()->getPropertyInstance(propertyID)->hasBinding()) {

		if (entity->getDataInstance()->getPropertyInstance(propertyID)->getType() == DfxEntityPropertyTypes::iFILE_PATH_TYPE) {
			*((std::string*)entity->getProperty(propertyID)) = val;

			entity->makeDirty();
			entity->getDataInstance()->getPropertyInstance(propertyID)->dirtyParent();
			DfxGlobal::command::exec("updateViewport");
		}
	}
}
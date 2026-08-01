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

#include "ResourceQForm.h"


ResourceQForm::ResourceQForm() {
	_currentSelection = nullptr;

	_layout = new QVBoxLayout;
	_layout->setAlignment(Qt::AlignTop);

	setLayout(_layout);
}

ResourceQForm::~ResourceQForm() {

}

void ResourceQForm::resourceChanged(const QItemSelection &selected, const QItemSelection &deselected) {
	
	if (_currentSelection != nullptr) {
		_currentSelection->setParent(nullptr);
		delete _currentSelection;
		_currentSelection = nullptr;
	}

	if (!selected.indexes().isEmpty()) {
		auto resource = static_cast<Resource*>(selected.indexes()[0].internalPointer());

		if (resource->type() == Resource::ROOT) {
			_currentSelection = rootForm(resource);
			_layout->addWidget(_currentSelection);
		}
		else if (resource->type() == Resource::FILE) {
			_currentSelection = fileForm(resource);
			_layout->addWidget(_currentSelection);
		}
	}
	
}

QWidget *ResourceQForm::fileForm(Resource *resource) {
	auto wgt = new QWidget;
	auto layout = new QFormLayout;

	auto resourcePath = new FileQWidget(&resource->resourcePath, true, false, QDir::homePath().toStdString(), "*.*");

	layout->addRow("Path: ",  resourcePath);
	layout->setAlignment(Qt::AlignTop);

	wgt->setLayout(layout);

	return wgt;
}

QWidget *ResourceQForm::rootForm(Resource *resource) {
	auto wgt = new QWidget;
	auto layout = new QFormLayout;

	auto installationDir = new FileQWidget(&resource->installation_dir, false, true, QDir::homePath().toStdString(), "*.*");
	auto main_executable = new FileQWidget(&resource->main_executable, true, false, QDir::homePath().toStdString(), "*.*");
	auto executableName = new QLineEdit(resource->executable_name.c_str());
	auto component_name = new QLineEdit(resource->component_name.c_str());
	auto eula = new FileQWidget(&resource->eula, true, false, QDir::homePath().toStdString(), "*.*");

	layout->addRow("Path: ", installationDir);
	layout->addRow("Main Executable: ", main_executable);
	layout->addRow("Executable Name: ", executableName);
	layout->addRow("Component Name: ", component_name);
	layout->addRow("EULA: ", eula);
	layout->setAlignment(Qt::AlignTop);

	wgt->setLayout(layout);

	QObject::connect(executableName, &QLineEdit::textChanged, [resource](const QString &text) {
		resource->executable_name = text.toStdString();
	});

	QObject::connect(component_name, &QLineEdit::textChanged, [resource](const QString &text) {
		resource->component_name = text.toStdString();
	});

	return wgt;
}

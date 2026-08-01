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

#include "BuildQView.h"
#include "ProjectManager.h"
#include "Build.h"


BuildQView::BuildQView() {

	layout = new QVBoxLayout;
	formLayout = new QFormLayout;
	name = new QLineEdit("Application");
	buildDir = new FileQWidget(&ResourceManager::getInstance()->buildDir(), false, true, QDir::homePath().toStdString(), "*.*");
	binaryCreator = new FileQWidget(&ResourceManager::getInstance()->binaryCreator(), true, false, QDir::homePath().toStdString(), "*.exe");

	major = new QSpinBox;
    minor = new QSpinBox;
	patch = new QSpinBox;

	build = new QPushButton("Build");

	formLayout->setMargin(0);

	major->setMinimum(0);
	minor->setMinimum(0);
	patch->setMinimum(0);

	formLayout->addRow("Name: ", name);
	formLayout->addRow("Build Directory: ", buildDir);
	formLayout->addRow("Binary Creator: ", binaryCreator);
	formLayout->addRow("Major: ", major);
	formLayout->addRow("Minor: ", minor);
	formLayout->addRow("Patch: ", patch);
	formLayout->addRow(build);

	layout->addLayout(formLayout);

	setLayout(layout);

	QObject::connect(build, &QPushButton::clicked,  this, &BuildQView::startBuild);

	QObject::connect(name, &QLineEdit::textChanged, this, &BuildQView::updateName);
	QObject::connect(major, QOverload<int>::of(&QSpinBox::valueChanged), this, &BuildQView::updateMajor);
	QObject::connect(minor, QOverload<int>::of(&QSpinBox::valueChanged), this, &BuildQView::updateMinor);
	QObject::connect(patch, QOverload<int>::of(&QSpinBox::valueChanged), this, &BuildQView::updatePatch);
	QObject::connect(ProjectManager::getInstance(), &ProjectManager::projectChanged, this, &BuildQView::updateBuild);
}

BuildQView::~BuildQView() {}

void BuildQView::updateName() {
	ResourceManager::getInstance()->setName(name->text().toStdString());
}

void BuildQView::updateBuildDir() {}

void BuildQView::updateBinaryCreator() {}

void BuildQView::updateMajor(int) {
	ResourceManager::getInstance()->setMajor(major->value());
}

void BuildQView::updateMinor(int) {
	ResourceManager::getInstance()->setMinor(minor->value());
}

void BuildQView::updatePatch(int) {
	ResourceManager::getInstance()->setPatch(patch->value());
}

void BuildQView::updateBuild() {
	buildDir->setValue(ResourceManager::getInstance()->buildDir().c_str());
	binaryCreator->setValue(ResourceManager::getInstance()->binaryCreator().c_str());
	name->setText(ResourceManager::getInstance()->name().c_str());
	major->setValue(ResourceManager::getInstance()->major());
	minor->setValue(ResourceManager::getInstance()->minor());
	patch->setValue(ResourceManager::getInstance()->patch());
}

void BuildQView::startBuild() {
	build::build_executable();
}
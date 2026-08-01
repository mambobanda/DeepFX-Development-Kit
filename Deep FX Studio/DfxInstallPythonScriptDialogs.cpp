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


#include "DfxInstallPythonScriptDialogs.h"


void DfxInstallPythonScriptDialogs::start(std::string script) {
	TypeSelectorDialog().run(script);
}

InstallNodeDialog::InstallNodeDialog(){

	buttonBox = new QDialogButtonBox(this);
	ok = new QPushButton("Install");
	cancel = new QPushButton("Cancel");

	buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);
	buttonBox->addButton(cancel, QDialogButtonBox::RejectRole);

	flayout = new QFormLayout();

	package_name = new QLineEdit();
	name = new QLineEdit();
	version = new QLineEdit();
	description = new QLineEdit();
	menu = new QLineEdit();
	autoload = new QComboBox();
	err = new QLabel("");

	autoload->insertItem(0, "Yes");
	autoload->insertItem(1, "No");

	flayout->addRow(QString("&Package Name*"), package_name);
	flayout->addRow(QString("&Name*"), name);
	flayout->addRow(QString("&Version*"), version);
	flayout->addRow(QString("&Description*"), description);
	flayout->addRow(QString("&Menu Path*"), menu);
	flayout->addRow(QString("&Autoload"), autoload);

	flayout->addWidget(buttonBox);
	flayout->addWidget(err);

	setWindowTitle("Install Python Node");
	setMinimumWidth(300);

	setLayout(flayout);

	QObject::connect(ok, &QPushButton::clicked, this, &InstallNodeDialog::validateForm);
	QObject::connect(cancel, &QPushButton::clicked, this, &QDialog::reject);
}

void InstallNodeDialog::validateForm() {

	if (package_name->text().length() < 3 ||
		name->text().length() < 3 ||
		version->text().length() < 3 ||
		description->text().length() < 3 ||
		menu->text().length() < 3) {

		err->setText("All fields required (3 characters minimum)");

		return;
	}

	DeepStudioRegistry::Entry entry;
	if (DeepStudioRegistry::findEntry(registry::_$PYTHON_NODE_DIR, entry)) {
		filesystem::path dir_path(entry.String);

		std::string pname = package_name->text().replace(" ", "_").toStdString();

		if (filesystem::exists(dir_path.string() + "/" + pname + ".pynode")) {
			err->setText("Package already exists. Change name.");
			return;
		}
	}


	accept();
}

void InstallNodeDialog::run(std::string script) {

	int rc = exec();

	if (rc) {

		DeepStudioRegistry::Entry entry;

		std::string pname = package_name->text().replace(" ", "_").toStdString();

		DfxPythonNodeLoader::PyNodeDesc info;
		info.name = name->text().toStdString();
		info.version = version->text().toStdString();
		info.description = description->text().toStdString();
		info.menu = menu->text().toStdString();
		info.autoload = (autoload->currentIndex() == 0) ? true : false;
		info.script = script;

		if (DeepStudioRegistry::findEntry(registry::_$PYTHON_NODE_DIR, entry)) {
			filesystem::path dir_path(entry.String);

			if (filesystem::exists(dir_path)) {
				DfxPythonNodeLoader loader;
				loader.savePythonNodeInfo(&info, pname, entry.String);
			}
			else
			{
				DfxGlobal::console::println("Failed to install Python Node: directory doesn't exist.\n");
			}
		}
		else {
			DfxGlobal::console::println("Failed to install Python Node: Unknown registry entry.\n");
		}
	}
}


TypeSelectorDialog::TypeSelectorDialog() {

	buttonBox = new QDialogButtonBox(this);
	ok = new QPushButton("Next");
	cancel = new QPushButton("Cancel");

	buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);
	buttonBox->addButton(cancel, QDialogButtonBox::RejectRole);

	flayout = new QFormLayout();

	select = new QComboBox();

	select->insertItem(0, "Install Python Node");
	select->insertItem(1, "Install Python Tool");

	flayout->addRow(QString("&Installation Type"), select);

	flayout->addWidget(buttonBox);

	setWindowTitle("Install Script");
	setMinimumWidth(200);

	setLayout(flayout);

	QObject::connect(ok, &QPushButton::clicked, this, &QDialog::accept);
	QObject::connect(cancel, &QPushButton::clicked, this, &QDialog::reject);
}

void TypeSelectorDialog::run(std::string script) {

	int rc = exec();

	if (rc) {
		if (select->currentIndex() == 0) {
			InstallNodeDialog().run(script);
		}
		else {

		}

	}
}
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

#include <experimental\filesystem>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFormLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinbox>
#include <QLabel>
#include <DfxGlobal.h>
#include "DeepStudioRegistry.h"
#include "DeepStudioRegistryPaths.h"
#include "DfxPythonNodeLoader.h"

namespace filesystem = std::experimental::filesystem;

class InstallNodeDialog : public QDialog {
	Q_OBJECT
private slots:
	void validateForm();
private:

	QDialogButtonBox *buttonBox;
	QPushButton  *ok;
	QPushButton  *cancel;
	QFormLayout *flayout;
	QLineEdit *package_name;
	QLineEdit *name;
	QLineEdit *version;
	QLineEdit *description;
	QLineEdit *menu;
	QComboBox *autoload;
	QLabel *err;
public:

	InstallNodeDialog();

	void run(std::string script);
};

class TypeSelectorDialog : public QDialog {
private:

	QDialogButtonBox *buttonBox;
	QPushButton  *ok;
	QPushButton  *cancel;
	QFormLayout *flayout;
	QComboBox *select;
public:

	TypeSelectorDialog();

	void run(std::string script);
};

class DfxInstallPythonScriptDialogs{
public:

	void start(std::string script);
};
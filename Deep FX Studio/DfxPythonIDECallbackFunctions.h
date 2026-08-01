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

#include <QDialog>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QPushButton>
#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinbox>
#include <QTextStream>
#include <string>
#include <DfxGlobal.h>

bool save_script(std::string, std::string &, std::string path="", bool use_path=false);

bool save_script_as(std::string, std::string &);

void install_script(std::string);

bool load_script(std::string&, std::string&);
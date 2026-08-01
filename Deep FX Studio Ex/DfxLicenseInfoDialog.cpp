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

#include "DfxLicenseInfoDialog.h"

int DfxLicenseInfoDialog::trialModeInfo()
{
	QDialog dialog(nullptr, Qt::WindowCloseButtonHint);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
	QPushButton  *ok = new QPushButton("Activate");

	buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);

	QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);

	QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
	QTextBrowser *browser = new QTextBrowser();

	QLabel *banner = new QLabel("");
	QPixmap img(":/misc/about_banner");
	banner->setPixmap(img);

	std::stringstream ss;
	ss << "<h1> Trial Mode" << "</h4>" << std::endl;
	ss << "<h4> Deep FX Studio will run in trial mode, to unlock all features please purchase the full version of Deep FX Studio. </h4>" << std::endl;
	ss << "<h3> Limitations:  </h3>" << std::endl;
	ss << "<ul> " << std::endl;
	ss << "<li> <h4> Internet activation on startup. </h4></li>" << std::endl;
	ss << "<li> <h4> Saved data unusable after 3 days. </h4></li>" << std::endl;
	ss << "<li> <h4> Saved data cannot be used in commercial versions. </h4></li>" << std::endl;
	ss << "<li> <h4> Maximum render resolution 1280 x 720. </h4></li>" << std::endl;
	ss << "<li> <h4> Renders watermarked. </h4></li>" << std::endl;
	ss << "</ul>" << std::endl;
	ss << "<h4> For More Infomation:  https://www.deepfxworld.com/" << "</h4>" << std::endl;

	browser->setHtml(ss.str().c_str());

	mainLayout->setMargin(0);
	mainLayout->addWidget(banner);
	mainLayout->addWidget(browser);
	mainLayout->addWidget(buttonBox);

	dialog.setWindowTitle("Deep FX Studio Trial");
	dialog.setMinimumWidth(400);
	dialog.setMinimumHeight(400);

	dialog.setLayout(mainLayout);

	return dialog.exec();
}

int DfxLicenseInfoDialog::activationErrorInfo(std::string msg_a, std::string msg_b) {
	QDialog dialog(nullptr, Qt::WindowCloseButtonHint);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
	QPushButton  *ok = new QPushButton("Ok");

	buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);

	QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);

	QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
	QTextBrowser *browser = new QTextBrowser();

	QLabel *banner = new QLabel("");
	QPixmap img(":/misc/about_banner");
	banner->setPixmap(img);

	std::stringstream ss;
	ss << "<h1> Trial Activation Failed" << "</h4>" << std::endl;
	ss << "<h4> Deep FX Studio failed to activate trial mode</h4>" << std::endl;
	ss << "<h4>" << msg_a << "</h4>" << std::endl;
	ss << "<h4>" << msg_b << "</h4>" << std::endl;

	browser->setHtml(ss.str().c_str());

	mainLayout->setMargin(0);
	mainLayout->addWidget(banner);
	mainLayout->addWidget(browser);
	mainLayout->addWidget(buttonBox);

	dialog.setWindowTitle("Deep FX Studio Trial");
	dialog.setMinimumWidth(400);

	dialog.setLayout(mainLayout);

	return dialog.exec();
}

int DfxLicenseInfoDialog::trialCommercialNotCompatible() {
	QDialog dialog(nullptr, Qt::WindowCloseButtonHint);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
	QPushButton  *ok = new QPushButton("Ok");

	buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);

	QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);

	QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
	QTextBrowser *browser = new QTextBrowser();

	QLabel *banner = new QLabel("");
	QPixmap img(":/misc/about_banner");
	banner->setPixmap(img);

	std::stringstream ss;
	ss << "<h1> Incompatible Deep FX Studio Project" << "</h4>" << std::endl;
	ss << "<h4> Deep FX Studio failed to load the project. The project you are trying to load was saved in a trial / commercial version.</h4>" << std::endl;
	ss << "<h4> Trial projects can only be used in the trial version. Commercial projects can only be used in the commercial version.</h4>" << std::endl;
	ss << "<h4></h4>" << std::endl;

	browser->setHtml(ss.str().c_str());

	mainLayout->setMargin(0);
	mainLayout->addWidget(banner);
	mainLayout->addWidget(browser);
	mainLayout->addWidget(buttonBox);

	dialog.setWindowTitle("Deep FX Studio");
	dialog.setMinimumWidth(400);

	dialog.setLayout(mainLayout);

	return dialog.exec();
}

int DfxLicenseInfoDialog::trialFileExpired() {
	QDialog dialog(nullptr, Qt::WindowCloseButtonHint);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
	QPushButton  *ok = new QPushButton("Ok");

	buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);

	QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);

	QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
	QTextBrowser *browser = new QTextBrowser();

	QLabel *banner = new QLabel("");
	QPixmap img(":/misc/about_banner");
	banner->setPixmap(img);

	std::stringstream ss;
	ss << "<h1> Expired Deep FX Studio Trial Project" << "</h4>" << std::endl;
	ss << "<h4> The project file has expired.  All files saved in the trial version expire after 3 days from the point of creation (not modified). Please purchase the full version of Deep FX Studio to bypass this limitation.</h4>" << std::endl;
	ss << "<h4></h4>" << std::endl;

	browser->setHtml(ss.str().c_str());

	mainLayout->setMargin(0);
	mainLayout->addWidget(banner);
	mainLayout->addWidget(browser);
	mainLayout->addWidget(buttonBox);

	dialog.setWindowTitle("Deep FX Studio");
	dialog.setMinimumWidth(400);

	dialog.setLayout(mainLayout);

	return dialog.exec();
}

int DfxLicenseInfoDialog::trialResolutionExceeded() {
	QDialog dialog(nullptr, Qt::WindowCloseButtonHint);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
	QPushButton  *ok = new QPushButton("Ok");

	buttonBox->addButton(ok, QDialogButtonBox::AcceptRole);

	QObject::connect(ok, &QPushButton::clicked, &dialog, &QDialog::accept);

	QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
	QTextBrowser *browser = new QTextBrowser();

	QLabel *banner = new QLabel("");
	QPixmap img(":/misc/about_banner");
	banner->setPixmap(img);

	std::stringstream ss;
	ss << "<h1> Deep FX Studio Trial" << "</h4>" << std::endl;
	ss << "<h4> Cannot render, You have exceeded the 1280 x 720 camera resolution limit. Please purchase the full version of Deep FX Studio to bypass this limitation.</h4>" << std::endl;
	ss << "<h4></h4>" << std::endl;

	browser->setHtml(ss.str().c_str());

	mainLayout->setMargin(0);
	mainLayout->addWidget(banner);
	mainLayout->addWidget(browser);
	mainLayout->addWidget(buttonBox);

	dialog.setWindowTitle("Deep FX Studio");
	dialog.setMinimumWidth(400);

	dialog.setLayout(mainLayout);

	return dialog.exec();
}
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

#include "DfxQLayoutWidget.h"
#include "DfxQComboBoxControl.h"

DfxQLayoutWidget::DfxQLayoutWidget(QWidget *parent):QWidget(parent) {

	widgetLayout = new QHBoxLayout;
	layout       = new QComboBox;
	menu		 = new QMenu;
	options		 = new QPushButton("");
	save		 = new QAction("Save");
	remove		 = new QAction("Remove");
	setAsDefault = new QAction("Set As Default");

	_ignoreLayoutIndexChange = false;

	DfxQComboBoxControl::styleComboBox(layout);
	styleButton(options);

	options->setMenu(menu);
	
	menu->addAction(save);
	menu->addAction(remove);
	menu->addAction(setAsDefault);
	
	widgetLayout->setContentsMargins(0, 3, 10, 0);
	widgetLayout->setSpacing(1);

	widgetLayout->addWidget(layout);
	widgetLayout->addWidget(options);

	int counter = 0;
	for (auto lt : DfxViewLayoutManager::_layouts) {
		layout->addItem(lt.first.c_str(), counter);
		counter++;
	}
	
	QObject::connect(layout, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DfxQLayoutWidget::changeLayoutAction);
	QObject::connect(save, &QAction::triggered, this, &DfxQLayoutWidget::addAction);
	QObject::connect(remove, &QAction::triggered, this, &DfxQLayoutWidget::removeAction);
	QObject::connect(setAsDefault, &QAction::triggered, this, &DfxQLayoutWidget::setAsDefaultAction);

	setLayout(widgetLayout);
}

DfxQLayoutWidget::~DfxQLayoutWidget() { }

void DfxQLayoutWidget::styleButton(QPushButton *qp) {
	QString ButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border: 0px solid;"
		"border-radius: 3px;"
		"padding-left: 2px;"
		"height: 13px;"
		"width: 13px;"
		"}";

	QString keyMenuButtonSty =
		"QPushButton { "
		"color: white;"
		"background-color: rgb(70,70,70);"
		"border-top-right-radius: 4px;"
		"border-bottom-right-radius: 4px;"
		"width: 11px;"
		"height: 16px;"
		"}";

	QString hoverSty =
		"QPushButton:hover { "
		"background-color: rgb(100,100,100);"
		"}";

	QString hoverPressdSty =
		"QPushButton:hover:pressed { "
		"background-color: rgb(255, 153, 83);"
		"}";

	qp->setStyleSheet(ButtonSty + hoverSty + hoverPressdSty);
	options->setStyleSheet(keyMenuButtonSty + hoverSty + hoverPressdSty);

}

void DfxQLayoutWidget::addAction() {
	_ignoreLayoutIndexChange = true;
	QString appdata = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];

	bool ok;
	QString text = QInputDialog::getText(this, tr("Save Layout"), tr("Layout Name:"), QLineEdit::Normal, "New Layout", &ok, 
											Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

	if (ok && !text.isEmpty()) {
		QString path = QString(appdata + "/layout_config/") + text + ".lt";
		QFile file(path);

		if (file.exists()) {
			QMessageBox msgBox;
			msgBox.setText("Layout with that name already exists.");
			msgBox.exec();

			return;
		}

		DfxViewLayoutManager::DfxViewLayout lt;
		lt.root = new DfxViewLayoutManager::DfxViewLayoutItem;
		lt.mapCurrentLayout(DfxQViewManager::getInstance()->root(), lt.root);

		DfxViewLayoutManager::saveLayout(text.toStdString(), &lt, path.toStdString());

		layout->clear();

		DfxViewLayoutManager::findLayoutConfigs();

		int counter = 0;
		for (auto lt : DfxViewLayoutManager::_layouts) {
			layout->addItem(lt.first.c_str(), counter);
			counter++;
		}
	}

	_ignoreLayoutIndexChange = false;
}

void DfxQLayoutWidget::removeAction() {
	_ignoreLayoutIndexChange = true;

	QMessageBox msgBox;
	msgBox.setText("Removing Last Selected Layout.");
	msgBox.setInformativeText("Do you want to continue?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	int ret = msgBox.exec();

	if (ret == QMessageBox::Yes) {
		int index = layout->currentIndex();

		if (DfxViewLayoutManager::_layouts.size() > 1) {
			if (index >= 0) {

				auto rc = DfxViewLayoutManager::removeLayout(index);

				if (!rc) {
					QMessageBox msgBox;
					msgBox.setText("Failed to remove layout.");
					msgBox.exec();
				}
				else {
					layout->clear();
					DfxViewLayoutManager::findLayoutConfigs();

					int counter = 0;
					for (auto lt : DfxViewLayoutManager::_layouts) {
						layout->addItem(lt.first.c_str(), counter);
						counter++;
					}
				}
			}
		}
	}

	_ignoreLayoutIndexChange = false;
}

void DfxQLayoutWidget::setAsDefaultAction() {

	auto output = DfxViewLayoutManager::currentLayoutToString();

	if (!output.empty()) {
		QSettings settings;
		settings.setValue("~/layout/default_layout", output.c_str());
	}
	
}

void DfxQLayoutWidget::changeLayoutAction(int index) {
	if (!_ignoreLayoutIndexChange) {
		if (index >= 0) {
			DfxViewLayoutManager::switchLayout(*DfxViewLayoutManager::_layouts[index].second);
		}
	}
}
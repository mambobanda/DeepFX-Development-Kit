#pragma once

#include <QtWidgets/QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QTreeView>
#include <QTabWidget>
#include "BuildQView.h"
#include "ResourceQView.h"
#include "ConfigQView.h"
#include "PackageQView.h"
#include "InstallScriptQView.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
private:
	QTabWidget *_tabs;

	ResourceQView *_resourceView;
	ConfigQView *_configView;
	PackageQView *_packageView;
	InstallScriptQView *_installScriptQView;
	BuildQView *_buildView;

	QMenuBar *menuBar;
	QMenu *file;
	QAction *save;
	QAction *load;
	QAction *clear;

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	virtual ~MainWindow();

public slots:
	void saveSlot();
	void loadSlot();
	void clearSlot();
};

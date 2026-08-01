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

#include <QApplication>
#include "DfxQMainMenu.h"
#include "DfxLuxCoreRenderSession.h"
#include "DfxUtil.h"
#include "ApplicationCommands.h"
#include "DfxResource.h"
#include "DfxResourceManager.h"
#include "DfxSpaceReset.h"
#include "DfxSceneCleaner.h"
#include "WidgetInitializer.h"

namespace command {

	/// Open shortcut editor
	void EchoSpace::execute() {
		DfxGlobal::console::println(util::echoSpace());
	}

	void EchoSpace::execute(DfxCmdArguments args) {	
		appendToFile(args[1], util::echoSpace());	
	}

	void *EchoSpace::create() {
		return new EchoSpace;
	}

	/// Exit Application
	void ExitApplication::execute() {
		QApplication::exit();
	}

	void ExitApplication::execute(DfxCmdArguments args) {
		execute();
	}

	void *ExitApplication::create() {
		return new ExitApplication;
	}

	/// Open shortcut editor
	void OpenShorcutsEditor::execute() {
		DfxQHotkeyEditor().exec();
	}

	void OpenShorcutsEditor::execute(DfxCmdArguments) {
		execute();
	}

	void *OpenShorcutsEditor::create() {
		return new OpenShorcutsEditor;
	}

	/// Scene view context menu
	void SceneViewContextMenu::execute() {
		DfxGlobal::console::println("Context Menu");
	}

	void SceneViewContextMenu::execute(DfxCmdArguments) {
		execute();
	}

	void *SceneViewContextMenu::create() {
		return new SceneViewContextMenu;
	}

	/// Update scene
	void UpdateScene::execute() {
		DfxUpdater::getInstance()->sceneUpdated();
	}

	void UpdateScene::execute(DfxCmdArguments) {
		execute();
	}

	void *UpdateScene::create() {
		return new UpdateScene;
	}

	/// Update scene & Keys
	void UpdateSceneAndKeys::execute() {
		DfxUpdater::getInstance()->sceneUpdated(DfxMainEventLoop::VIEW_UPDATE, true);
	}

	void UpdateSceneAndKeys::execute(DfxCmdArguments) {
		execute();
	}

	void *UpdateSceneAndKeys::create() {
		return new UpdateSceneAndKeys;
	}

	/// Clear scene
	void ClearScene::execute() {
		DfxQSceneManager::getInstance()->removeAll();
		util::clearAllSceneHandlers();
		
		DfxUpdater::getInstance()->sceneUpdated();
	}

	void ClearScene::execute(DfxCmdArguments) {
		execute();
	}

	void *ClearScene::create() {
		return new ClearScene;
	}

	/// Clear scene 02
	void ClearScene02::execute() {
		DfxSceneCleaner::getInstance()->cleanGraph();
		DfxSceneCleaner::getInstance()->clean();
		DfxSceneCleaner::getInstance()->reset();
		widget::initialize();
		
		DfxSpace::getInstance()->removeAllBindings();

		DfxResourceManager::getInstance()->resetId();

		DfxProject::getInstance()->setProjectPath("");
	}

	void ClearScene02::execute(DfxCmdArguments) {
		execute();
	}

	void *ClearScene02::create() {
		return new ClearScene02;
	}

	/// Open Object
	void OpenObj::execute() {

		QSettings settings;
		QString initialDir = settings.value("~/io/loadObject", QDir::homePath()).toString();

		QString fileName
			= QFileDialog::getOpenFileName(nullptr, QString("Load Object"), initialDir, QString("Files (*.obj)"));

		if (fileName.length() > 0) {

			DfxObjectHandler obj_handler;
			filesystem::path p_path(fileName.toStdString());

			std::string _name = p_path.stem().generic_string();

			auto mshResource = new DfxResource(resource::mode::RESOURCE, resource::type::GEOMETRY, fileName.toStdString());
			bool rc = obj_handler.create(mshResource->newPointer(), fileName.toStdString(), _name);

			if (rc) {
				
				DfxObjectLoader objLoader;

				auto mesh = objLoader.loadObject(fileName.toStdString(), obj_handler.dataObject()->absolutePath(),[](int current, int total) {
					DfxGlobal::console::println("Loading object #" + std::to_string(current + 1) + " of " + std::to_string(total));
				});

				if (mesh != nullptr) {
					ds::MeshOperator mop(mesh);
					mop.calcSmoothNormals("smooth_normals");
					mop.calcFaceNormals("face_normals");

					mshResource->setResource(mesh);
					mshResource->setFormat(resource::format::DS_MESH);
					mshResource->setReleaseCallback(DfxObjectHandler::releaseDeepSimplexResource);
					mshResource->setName(_name);

					DfxResourceManager::getInstance()->geo()->addChild(mshResource);

					// auto attributes = obj_handler.createAttributes();
					auto gnode = obj_handler.createGeoNode();

					if (gnode != nullptr)
						DfxQSceneManager::getInstance()->moveNode(obj_handler.geoNode(), deepsg::SgRootNode::getInstance());
					else
						DfxGlobal::console::println("Failed To Open Object: Object Handler failed to create node.");

					DfxUpdater::getInstance()->sceneUpdated();

					if (fileName.length() > 0) {
						settings.setValue("~/io/loadObject", fileName);
					}

				}
				else {
					obj_handler.destroy();
					DfxGlobal::console::println("Failed To Open Object: Failed to read OBJ file.");
				}

			}
			else {
				DfxGlobal::console::println("Failed To Open Object: Object Handler failed");
			}
		}
	}

	void OpenObj::execute(DfxCmdArguments) {
		execute();
	}

	void *OpenObj::create() {
		return new OpenObj;
	}

	/// Save Project
	void SaveProject::execute() {

		QString fileName = "";
		QSettings settings;

		QString initialDir = settings.value("~/io/saveProject", QDir::homePath()).toString();

		if (!DfxProject::getInstance()->isProjectOpen())
			fileName = QFileDialog::getSaveFileName(nullptr, QString("New Project"), initialDir, QString("Files (*.dfxs)"));
		else
			fileName = DfxProject::getInstance()->projectPath().c_str();

		if (fileName.length() <= 0) {
			DfxGlobal::console::println("Failed to save project, empty project path.");
			return;
		}
		else {
			if (fileName.length() > 0) {
				settings.setValue("~/io/saveProject", fileName);
			}
		}
		
		if (!DfxProject::getInstance()->save(fileName.toStdString())) {
			DfxGlobal::console::println("Failed to save project.");
		}
		else {
			if (!DfxProject::getInstance()->isProjectOpen()) {
				DfxProject::getInstance()->setProjectPath(fileName.toStdString());
			}
		}
		
	}

	void SaveProject::execute(DfxCmdArguments) {
		execute();
	}

	void *SaveProject::create() {
		return new SaveProject;
	}

	/// Save Project As
	void SaveProjectAs::execute() {

		QString fileName = "";
		QSettings settings;

		QString initialDir = settings.value("~/io/saveProject", QDir::homePath()).toString();
	
		fileName = QFileDialog::getSaveFileName(nullptr, QString("New Project"), initialDir, QString("Files (*.dfxs)"));

		if (fileName.length() <= 0) {
			DfxGlobal::console::println("Failed to save project, empty project path.");
			return;
		}
		else {
			if (fileName.length() > 0) {
				settings.setValue("~/io/saveProject", fileName);
			}
		}

		if (!DfxProject::getInstance()->save(fileName.toStdString())) {
			DfxGlobal::console::println("Failed to save project.");
		}
		else {
			DfxProject::getInstance()->setProjectPath(fileName.toStdString());	
		}

	}

	void SaveProjectAs::execute(DfxCmdArguments) {
		execute();
	}

	void *SaveProjectAs::create() {
		return new SaveProjectAs;
	}

	/// Load Project
	void LoadProject::execute() {
		execute(DfxCmdArguments());
	}

	void LoadProject::execute(DfxCmdArguments args) {

		if (args.size() == 0) {

			QSettings settings;
			QString initialDir = settings.value("~/io/loadProject", QDir::homePath()).toString();

			QString fileName = "";
			
			fileName = QFileDialog::getOpenFileName(nullptr, QString("Load Project"), initialDir, QString("Files (*.dfxs)"));

			if (fileName.length() <= 0) {
				DfxGlobal::console::println("Failed to load project, empty project path.");
				return;
			}
			else {

				DfxGlobal::command::exec("clearScene");
				if (!DfxProject::getInstance()->load(fileName.toStdString())) {
					DfxGlobal::console::println("Failed to load project.");
				}
				else {
					DfxProject::getInstance()->setProjectPath(fileName.toStdString());
				}

				settings.setValue("~/io/loadProject", fileName);

				DfxQMainMenu::getInstance()->updateRecentOpenedSceneList(fileName.toStdString());

				DfxUpdater::getInstance()->sceneUpdated();
			}
			
		}
		else {
		
			auto fileName = QString(args[1].c_str());

			if (fileName.length() <= 0) {
				DfxGlobal::console::println("Failed to load project, empty project path.");
				return;
			}
			else {

				DfxGlobal::command::exec("clearScene");
				if (!DfxProject::getInstance()->load(fileName.toStdString())) {
					DfxGlobal::console::println("Failed to load project.");
				}
				else {
					DfxProject::getInstance()->setProjectPath(fileName.toStdString());
				}

				DfxUpdater::getInstance()->sceneUpdated();
			}		
		}
	
	}

	void *LoadProject::create() {
		return new LoadProject;
	}


	/// Save layout 
	void SaveLayout::execute() {
		std::string path("c:/test/layout_test.lt");
		
		DfxViewLayoutManager::DfxViewLayout layout;
		layout.root = new DfxViewLayoutManager::DfxViewLayoutItem;
		layout.mapCurrentLayout(DfxQViewManager::getInstance()->root(), layout.root);

		DfxViewLayoutManager::saveLayout("Test Layout", &layout, path);
	}

	void SaveLayout::execute(DfxCmdArguments) {
		execute();
	}

	void *SaveLayout::create() {
		return new SaveLayout;
	}

	/// Load Layout
	void LoadLayout::execute() {
		std::string path("c:/test/layout_test01.lt");

		auto p = DfxViewLayoutManager::loadLayout(path);

		if (p.second != nullptr) {
			
			DfxViewLayoutManager::switchLayout(*p.second);
		}
		else {
			printf("Error, failed to load layout.\n\n");
		}
	}

	void LoadLayout::execute(DfxCmdArguments) {
		execute();
	}

	void *LoadLayout::create() {
		return new LoadLayout;
	}

	/// Lux Test Render
	void LuxTestRender::execute() {

		auto any = DfxSpace::getInstance()->findAbsolute("~/CAMS");

		auto session = DfxRenderManager::getInstance()->newRenderSession(DfxRenderTarget::LUXCORE);

		if (any != nullptr) {
			if (any->type() == fl::type::DATA_OBJECT) {
				for (auto p : any->data<fl::DataObject>().properties()) {
					auto path = p->absolutePath();
					session->setCameraPath(path);
					break;
				}
			}
		}	
		
		session->init();
		session->testMaterials();
		//session->loadScene(deepsg::SgRootNode::getInstance());
		//session->prepareSession();
		//session->render();
		/// Saveoutput

	}

	void LuxTestRender::execute(DfxCmdArguments) {
		execute();
	}

	void *LuxTestRender::create() {
		return new LuxTestRender;
	}

	// Reset
	void Reset::execute() {
		DfxSpaceReset::reset();
	}

	void Reset::execute(DfxCmdArguments) {
		execute();
	}

	void *Reset::create() {
		return new Reset;
	}

	// Undo
	void Undo::execute() {
		DfxHistoryManager::getInstance()->undo();
	}

	void Undo::execute(DfxCmdArguments) {
		execute();
	}

	void *Undo::create() {
		return new Undo;
	}

	// Redo
	void Redo::execute() {
		DfxHistoryManager::getInstance()->redo();
	}

	void Redo::execute(DfxCmdArguments) {
		execute();
	}

	void *Redo::create() {
		return new Redo;
	}
}
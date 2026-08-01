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

#include <filesystem>
#include <QApplication>
#include "Build.h"

namespace build {

	void writeToFile(std::string path, std::string content) {
		std::ofstream file;
		file.open(path, std::ofstream::out);
		file << content;
		file.close();
	}

	std::string moveFiles(std::string dir, Resource *rsc, std::string configDir, std::string metaDir, std::string versionStr) {
		if (rsc->type() == Resource::ROOT) {

			// Copy Main Executable
			filesystem::path fromExe(rsc->main_executable);
			std::string toExeDir = dir + "/" + rsc->executable_name + ".exe";
			filesystem::path toExe(toExeDir);
			filesystem::copy_file(fromExe, toExe, filesystem::copy_options::overwrite_existing);

			filesystem::path fromEula(rsc->eula);
			std::string toEulaDir = metaDir + "/" + "license.txt";
			filesystem::path toEula(toEulaDir);
			filesystem::copy_file(fromEula, toEula, filesystem::copy_options::overwrite_existing);

			QString config = ResourceManager::getInstance()->configXML().c_str();
			QString package = ResourceManager::getInstance()->packageXML().c_str();
			QString installScript = ResourceManager::getInstance()->installScriptQS().c_str();

			QString installationDir = QString("@ApplicationsDirX64@/") + rsc->installation_dir.c_str() + " " + versionStr.c_str();
			config = config.replace("##version##", versionStr.c_str());
			config = config.replace("##installation_dir##", installationDir);

			QDate cdate = QDate::currentDate();
			package = package.replace("##version##", versionStr.c_str());
			package = package.replace("##date##", cdate.toString("yyyy-MM-dd"));
			//package = package.replace("##license##", rsc->eula.c_str());

			writeToFile(configDir + "/config.xml", config.toStdString());
			writeToFile(metaDir + "/package.xml", package.toStdString());
			writeToFile(metaDir + "/installscript.qs", installScript.toStdString());

			for (auto chd : rsc->children()) {
				moveFiles(dir, chd);
			}
		}
		else if (rsc->type() == Resource::DIR) {
			std::string folderDir = dir + "/" + rsc->name();
			filesystem::path _folderDir(folderDir);

			filesystem::create_directory(_folderDir);

			for (auto chd : rsc->children()) {
				moveFiles(folderDir, chd);
			}
		}
		else if (rsc->type() == Resource::FILE) {
			filesystem::path from(rsc->resourcePath);

			std::string toDir = dir + "/" + from.filename().string();
			
			filesystem::path to(toDir);

			filesystem::copy_file(from, to, filesystem::copy_options::overwrite_existing);
		}

		return std::string("");
	}

	std::string build_repo() {

		std::string name = ResourceManager::getInstance()->name();
		std::string _major = std::to_string(ResourceManager::getInstance()->major());
		std::string _minor = std::to_string(ResourceManager::getInstance()->minor());
		std::string _patch = std::to_string(ResourceManager::getInstance()->patch());
		std::string verStr = _major + "." + _minor + "." + _patch;
		std::string dirname = name + " " + verStr;
		std::string dirPath = ResourceManager::getInstance()->buildDir() + "/" + dirname;
		std::string configDirPath = ResourceManager::getInstance()->buildDir() + "/" + dirname + "/config";
		std::string packagesDirPath = ResourceManager::getInstance()->buildDir() + "/" + dirname + "/packages";
		std::string componentDirPath = ResourceManager::getInstance()->buildDir() + "/" + dirname + "/packages/" + ResourceManager::getInstance()->rootApplication()->component_name;
		std::string dataDirPath = ResourceManager::getInstance()->buildDir() + "/" + dirname + "/packages/" + ResourceManager::getInstance()->rootApplication()->component_name + "/data";
		std::string metaDirPath = ResourceManager::getInstance()->buildDir() + "/" + dirname + "/packages/" + ResourceManager::getInstance()->rootApplication()->component_name + "/meta";

		filesystem::path repoDir(dirPath);
		filesystem::path repoConfigDir(configDirPath);
		filesystem::path repoPackagesDir(packagesDirPath);
		filesystem::path repoComponentDir(componentDirPath);
		filesystem::path repoDataDir(dataDirPath);
		filesystem::path repoMetaDir(metaDirPath);

		filesystem::remove_all(dirPath);

		filesystem::create_directory(repoDir);
		filesystem::create_directory(repoConfigDir);
		filesystem::create_directory(repoPackagesDir);
		filesystem::create_directory(repoComponentDir);
		filesystem::create_directory(repoDataDir);
		filesystem::create_directory(repoMetaDir);

		QProgressDialog progress("Building Installer", "Stop", 0, 0, nullptr);
		progress.setWindowModality(Qt::WindowModality::ApplicationModal);
		progress.show();

		moveFiles(dataDirPath, ResourceManager::getInstance()->rootApplication(), configDirPath, metaDirPath, verStr);

		std::stringstream ss;
		ss << ResourceManager::getInstance()->binaryCreator() << " ";
		ss << "-c \"" + configDirPath + "/config.xml\" ";
		ss << "-p \"" << packagesDirPath + "\" ";
		ss << "-v ";
		ss << "\""<< ResourceManager::getInstance()->buildDir() + "/" + name+ " " + verStr + ".exe\"";

		QProcess buildProcess;
		buildProcess.start(ss.str().c_str());

		while (buildProcess.state() != QProcess::ProcessState::NotRunning) {
			QApplication::processEvents();
		}

		buildProcess.waitForFinished(-1);

		printf("%s\n", buildProcess.readAllStandardOutput().toStdString().c_str());

		progress.close();

		return std::string("");
	}

	void build_executable() {
		build_repo();
	}
}
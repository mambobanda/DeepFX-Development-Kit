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

#include "AnimationCommands.h"
#include "DfxGraphEditorManager.h"

namespace command {
	
	void CreateKey::execute() {
		if (DfxMode::getInstance()->mode() == DfxMode::Mode::OBJECT_EDIT) {

			int currentFrame = DfxSpace::getInstance()->findAbsolute("~/TIME/current_frame")->data<int>();

			auto createLocKey = [currentFrame](std::string path) {

				std::string xPath = path + "/xform/x_pos";
				std::string yPath = path + "/xform/y_pos";
				std::string zPath = path + "/xform/z_pos";

				auto xVal = DfxSpace::getInstance()->findAbsolute(xPath)->data<float>();
				auto yVal = DfxSpace::getInstance()->findAbsolute(yPath)->data<float>();
				auto zVal = DfxSpace::getInstance()->findAbsolute(zPath)->data<float>();

				if (!DfxPropertyConduit::getInstance()->hasKeyframeOverride(xPath)) {
					DfxPropertyConduit::getInstance()->createKeyframeFloatOverride(xPath, xVal);
				}

				if (!DfxPropertyConduit::getInstance()->hasKeyframeOverride(yPath)) {
					DfxPropertyConduit::getInstance()->createKeyframeFloatOverride(yPath, yVal);
				}

				if (!DfxPropertyConduit::getInstance()->hasKeyframeOverride(zPath)) {
					DfxPropertyConduit::getInstance()->createKeyframeFloatOverride(zPath, zVal);
				}

				DfxPropertyConduit::getInstance()->updateKeyframeFloatOverride(xPath, currentFrame, xVal);
				DfxPropertyConduit::getInstance()->updateKeyframeFloatOverride(yPath, currentFrame, yVal);
				DfxPropertyConduit::getInstance()->updateKeyframeFloatOverride(zPath, currentFrame, zVal);
				
			};

			auto createRotKey = [currentFrame](std::string path) {

				std::string xPath = path + "/xform/x_rot";
				std::string yPath = path + "/xform/y_rot";
				std::string zPath = path + "/xform/z_rot";

				auto xVal = DfxSpace::getInstance()->findAbsolute(xPath)->data<float>();
				auto yVal = DfxSpace::getInstance()->findAbsolute(yPath)->data<float>();
				auto zVal = DfxSpace::getInstance()->findAbsolute(zPath)->data<float>();

				if (!DfxPropertyConduit::getInstance()->hasKeyframeOverride(xPath)) {
					DfxPropertyConduit::getInstance()->createKeyframeFloatOverride(xPath, xVal);
				}

				if (!DfxPropertyConduit::getInstance()->hasKeyframeOverride(yPath)) {
					DfxPropertyConduit::getInstance()->createKeyframeFloatOverride(yPath, yVal);
				}

				if (!DfxPropertyConduit::getInstance()->hasKeyframeOverride(zPath)) {
					DfxPropertyConduit::getInstance()->createKeyframeFloatOverride(zPath, zVal);
				}

				DfxPropertyConduit::getInstance()->updateKeyframeFloatOverride(xPath, currentFrame, xVal);
				DfxPropertyConduit::getInstance()->updateKeyframeFloatOverride(yPath, currentFrame, yVal);
				DfxPropertyConduit::getInstance()->updateKeyframeFloatOverride(zPath, currentFrame, zVal);
			};

			auto createSclKey = [currentFrame](std::string path) {

				std::string xPath = path + "/xform/x_scl";
				std::string yPath = path + "/xform/y_scl";
				std::string zPath = path + "/xform/z_scl";

				auto xVal = DfxSpace::getInstance()->findAbsolute(xPath)->data<float>();
				auto yVal = DfxSpace::getInstance()->findAbsolute(yPath)->data<float>();
				auto zVal = DfxSpace::getInstance()->findAbsolute(zPath)->data<float>();

				if (!DfxPropertyConduit::getInstance()->hasKeyframeOverride(xPath)) {
					DfxPropertyConduit::getInstance()->createKeyframeFloatOverride(xPath, xVal);
				}

				if (!DfxPropertyConduit::getInstance()->hasKeyframeOverride(yPath)) {
					DfxPropertyConduit::getInstance()->createKeyframeFloatOverride(yPath, yVal);
				}

				if (!DfxPropertyConduit::getInstance()->hasKeyframeOverride(zPath)) {
					DfxPropertyConduit::getInstance()->createKeyframeFloatOverride(zPath, zVal);
				}

				DfxPropertyConduit::getInstance()->updateKeyframeFloatOverride(xPath, currentFrame, xVal);
				DfxPropertyConduit::getInstance()->updateKeyframeFloatOverride(yPath, currentFrame, yVal);
				DfxPropertyConduit::getInstance()->updateKeyframeFloatOverride(zPath, currentFrame, zVal);
			};

			if (!DfxSceneSelectionModel::getInstance()->selected().empty()) {
				auto space = DfxSpace::getInstance();
				int keyable = space->findAbsolute("~/KEY_OPTIONS/keyable")->data<int>();

				if (keyable == DfxKeyableTypes::LOC_ROT_SCL) {
					for (auto node : DfxSceneSelectionModel::getInstance()->selected()) {
						if (node->xFormProperties() != nullptr) {
							createLocKey(node->referencePath());
							createRotKey(node->referencePath());
							createSclKey(node->referencePath());
						}
					}

					DfxPropertyConduit::getInstance()->announceNewKeyframeCreated();
				}
				else if (keyable == DfxKeyableTypes::LOC) {
					for (auto node : DfxSceneSelectionModel::getInstance()->selected()) {
						if (node->xFormProperties() != nullptr) {
							createLocKey(node->referencePath());
						}
					}

					DfxPropertyConduit::getInstance()->announceNewKeyframeCreated();
				}
				else if (keyable == DfxKeyableTypes::ROT) {
					for (auto node : DfxSceneSelectionModel::getInstance()->selected()) {
						if (node->xFormProperties() != nullptr) {
							createRotKey(node->referencePath());
						}
					}

					DfxPropertyConduit::getInstance()->announceNewKeyframeCreated();
				}
				else if (keyable == DfxKeyableTypes::SCL) {
					for (auto node : DfxSceneSelectionModel::getInstance()->selected()) {
						if (node->xFormProperties() != nullptr) {
							createSclKey(node->referencePath());
						}
					}

					DfxPropertyConduit::getInstance()->announceNewKeyframeCreated();
				}

				DfxGraphEditorManager::getInstance()->notifyChannelUpdated();
			}
			else {
				DfxGlobal::console::println("Failed to create key: No object selected.");
			}
		}
		else {
			DfxGlobal::console::println("Failed to create key: Must be in object mode.");
		}
	}

	void CreateKey::execute(DfxCmdArguments) {
		execute();
	}

	void *CreateKey::create() {
		return new CreateKey;
	}

	void GoToFrame::execute() {
		DfxGlobal::console::println("goToFrame Failed: Specify Frame");
	}

	void GoToFrame::execute(DfxCmdArguments args) {
		if (!args.empty()) {

			QString arg(args[1].c_str());
			int frame = arg.toInt();

			auto currentFrameAny = DfxSpace::getInstance()->findAbsolute("~/TIME/current_frame");
			auto start = DfxSpace::getInstance()->findAbsolute("~/TIME/frame_start")->data<int>();
			auto end = DfxSpace::getInstance()->findAbsolute("~/TIME/frame_end")->data<int>();

			if (frame < start) {
				DfxGlobal::console::println("goToFrame Failed: Out Of Range <" + std::to_string(start) + ", " + std::to_string(end) + ">");
				return;
			}

			if (frame > end) {
				DfxGlobal::console::println("goToFrame Failed: Out Of Range <" + std::to_string(start) + ", " + std::to_string(end) + ">");
				return;
			}

			currentFrameAny->setData<int>(frame);
			currentFrameAny->notifyObservers();
		}
	}

	void *GoToFrame::create() {
		return new GoToFrame;
	}
}
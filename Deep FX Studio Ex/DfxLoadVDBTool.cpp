/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2021 Mailo Today Solutions.
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

#include <SgGridNode.h>
#include <openvdb/openvdb.h>
#include <regex>
#include "NaturalSort.h"
#include "DfxGridTypes.h"
#include "DfxGridHandler.h"
#include "DfxLoadVDBTool.h"
#include "DfxLoadVDBHandler.h"
#include "DfxOpenVDBGridAdapter.h"
#include "DfxVDBGridChannel.h"

std::string DfxLoadVDBTool::_uniqueName = "UID_VDB_READER_TOOL";

DfxLoadVDBTool::DfxLoadVDBTool() {
	openvdb::initialize();
	_sequencePathsCached = false;
	_dispatcher = nullptr;
}

DfxLoadVDBTool::~DfxLoadVDBTool() {

}

std::string DfxLoadVDBTool::uniqueName() {
	return _uniqueName;
}

std::string DfxLoadVDBTool::getUniqueName() {
	return _uniqueName;
}

bool DfxLoadVDBTool::isReady() {
	return true;
}

void DfxLoadVDBTool::onSetReferencePath(std::string path) {
	if (_dispatcher != nullptr) {
		delete _dispatcher;
		_dispatcher = nullptr;
	}

	auto any = DfxSpace::getInstance()->findAbsolute(path);

	if (any != nullptr) {
		_dispatcher = new DfxToolFlDispatcher(this);
		_dispatcher->attachBranch(any);
	}

	DfxToolNode::onSetReferencePath(path);
}

void DfxLoadVDBTool::create(SgNode *parent) {
}

void DfxLoadVDBTool::destroy() {
}

bool DfxLoadVDBTool::evaluateOnUpdate() {
	return false;
}

bool DfxLoadVDBTool::evaluateOnNewFrame() {
	return true;
}

bool DfxLoadVDBTool::evaluateOnValueChanged() {
	_sequencePathsCached = false;
	return true;
}

bool DfxLoadVDBTool::pathCompareFunction(std::string a, std::string b) {
	std::regex r("[^0-9]*([0-9]+).*");
	std::smatch sm_a;
	std::smatch sm_b;

	if (std::regex_search(a, sm_a, r) && std::regex_search(b, sm_b, r)) {
		try {
			return std::stoi(sm_a[1]) < std::stoi(sm_b[1]);
		}
		catch (std::exception const & e) {
			DfxGlobal::console::println("VDB Loader Failed to cache sequence path: " + std::string(e.what()));
			return a < b;
		}
	}
	else {
		return a < b;
	}
}

void DfxLoadVDBTool::cacheSequencePaths(std::string pth) {
	_sequencePaths.clear();
	
	filesystem::path dpath(pth);

	for (auto const& dir_entry : filesystem::directory_iterator(dpath.parent_path()))
		_sequencePaths.push_back(dir_entry.path().string());

	SI::natural::sort(_sequencePaths);

}

void DfxLoadVDBTool::evaluate(SgNode *parent) {
	DfxLoadVDBHandler toolHandler;
	DfxGridHandler parentHandler;

	if (parent->getType() == deepsg::SgNodeType::GRID) {

		if (toolHandler.fetch(referencePath()) && parentHandler.fetch(parent->referencePath())) {

			auto vadapter = ((DfxGridNode*)parent)->getGridAdapter();
			auto dao = toolHandler.dataObject()->rawData<fl::DataObject>();

			auto path = dao->property("vdb_path")->data<std::string>();
			auto loadSequence = dao->property("load_sequence")->data<bool>();


			if (vadapter->gridImplementation() == DfxGridTypes::OPEN_VDB) {
				auto vdbAdapter = static_cast<DfxOpenVDBGridAdapter*>(vadapter);
				vdbAdapter->clearAll();

				if (!path.empty()) {
					
					if (loadSequence) {
						if (!_sequencePathsCached) {
							cacheSequencePaths(path);
							// for (auto str : _sequencePaths) printf("path: %s\n", str.c_str());
							_sequencePathsCached = true;
						}

						auto currentFame = DfxTimelineManager::getInstance()->currentFrame();

						if (currentFame < _sequencePaths.size() && currentFame > 0) {
							path = _sequencePaths[currentFame];
						}
					}

					openvdb::io::File file(path);

					if (file.open()) {
						for (openvdb::io::File::NameIterator nameIter = file.beginName(); nameIter != file.endName(); ++nameIter) {
							auto gridPtr = file.readGrid(nameIter.gridName());
							if (gridPtr->type() == openvdb::FloatGrid::gridType()) {
								
								openvdb::CoordBBox bbox;
								gridPtr->baseTree().evalLeafBoundingBox(bbox);
								if ((bbox.extents().x() * bbox.extents().y() * bbox.extents().z()) > 0) {
									auto channel = new DfxVDBGridChannel;
									channel->setVDBGrid(gridPtr);
									channel->initAccessor();
									vdbAdapter->addChannel(channel);
								}
							}
						}

						file.close();

						if (vdbAdapter->channelCount() > 0) {
							auto cache = parentHandler.volumeCache();
							DfxOpenGLCacheQueue::getInstance()->queueCache(cache);
						}
					}
					else {
						DfxGlobal::console::println(std::string("Failed to load OpenVDb File: ") + path);
					}
				}
			}
			/*auto offset = dao->property("offset")->data<unsigned int>();
			auto frame = dao->property("frame")->data<unsigned int>();
			auto fixed_frame = dao->property("fixed_frame")->data<bool>();*/
	

			parent->emitSignal(deepsg::SgNode::Signal::UPDATED);
		}
	}
}

std::vector<std::pair<QWidget*, QString>> DfxLoadVDBTool::propertyForms() {
	std::pair<QWidget*, QString> grid(form(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> {grid};
}

QWidget *DfxLoadVDBTool::form() {

	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto propertiesControlPanel = new DfxControlPanel();

	DfxQControlArray *propertiesControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// Type Controls
	auto load_sequence = new DfxQCheckBoxControl(path + "load_sequence", "", false);
	auto vdb_path = new DfxQFilePathControl(path + "vdb_path", true, false, "Open VDB File", "c:/", "(*.vdb)", "", false);
	
	propertiesControlArray->addControl("Load Sequence", load_sequence);
	propertiesControlArray->addControl("File", vdb_path);

	propertiesControlPanel->setTitle("Properties");
	propertiesControlPanel->setWidget(propertiesControlArray);

	// Main layout
	main_layout->addWidget(propertiesControlPanel);

	widget->setLayout(main_layout);

	return widget;
}
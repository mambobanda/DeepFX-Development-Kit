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

#include <amp.h>
#include <amp_math.h>
#include <openvdb/openvdb.h>
#include "DfxGridHandler.h"
#include "DfxTimelineManager.h"
#include "DfxUtil.h"
#include "NaturalSort.h"
#include "DfxEntropyCachePropNode.h"
#include "DfxEntropyCachePropHandler.h"
#include "DfxOpenVDBGridAdapter.h"
#include "DfxVDBGridChannel.h"

std::string DfxEntropyCachePropNode::_uniqueName = "UID_ENTROPY_CACHE_PROP_NODE";

DfxEntropyCachePropNode::DfxEntropyCachePropNode(deepsg::SgStageNode *stage) : DfxSimulationStagePropBase(stage, entropy::type::CACHE) {
	currentFrameObserver = new CurrentFrameObserver(this);
	DfxTimelineManager::getInstance()->addCurrentFrameObserver(currentFrameObserver);

	while (true) {
		random_num = util::rand<int>(1000, 1000000);

		QString appdata = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];

		QDir dir(appdata + "/tmp/entropy_cache_" + QString(std::to_string(random_num).c_str()));

		if (!dir.exists()) break;	
	}

	clearTempDir();

}

DfxEntropyCachePropNode::~DfxEntropyCachePropNode() {
	QString appdata = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];

	QString path(appdata + "/tmp/entropy_cache_" + QString(std::to_string(random_num).c_str()));
	QDir dir(path);

	if (dir.exists()) {
		filesystem::remove_all(filesystem::path(path.toStdString()));
	}
}

std::string DfxEntropyCachePropNode::uniqueName() {
	return _uniqueName;
}

std::string DfxEntropyCachePropNode::getUniqueName() {
	return _uniqueName;
}

void DfxEntropyCachePropNode::clearTempDir() {
	QString appdata = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];

	QString path(appdata + "/tmp/entropy_cache_" + QString(std::to_string(random_num).c_str()));
	QDir dir(path);

	if (!dir.exists()) {
		filesystem::create_directories(filesystem::path(path.toStdString()));
	}
	else {
		filesystem::remove_all(filesystem::path(path.toStdString()));
		filesystem::create_directories(path.toStdString());
	}
}

void DfxEntropyCachePropNode::clear() {
	clearTempDir();
}

void DfxEntropyCachePropNode::updateVisualization() {
	DfxEntropyCachePropHandler handler;
	DfxGridHandler gridHandler;

	if (handler.fetch(referencePath())) {
		auto dao = handler.dataObject()->rawData<fl::DataObject>();

		auto grid_path = dao->property("grid")->data<std::string>();

		if (gridHandler.fetch(grid_path)) {
			auto dao = gridHandler.dataObject()->rawData<fl::DataObject>();

			if (dao->hasProperty("sg_grid_node")) {

				auto node = gridHandler.gridNode();
				auto gridAdapter = node->getGridAdapter();

				std::vector<std::string> _sequencePaths;

				QString appdata = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];

				QString path(appdata + "/tmp/entropy_cache_" + QString(std::to_string(random_num).c_str()));
				QDir dir(path);

				auto FRAME = DfxTimelineManager::getInstance()->currentFrame() - DfxTimelineManager::getInstance()->firstFrame();

				if (dir.exists()) {
					filesystem::path dpath(path.toStdString());

					for (auto const& dir_entry : filesystem::directory_iterator(dpath))
						_sequencePaths.push_back(dir_entry.path().string());

					if (!_sequencePaths.empty()) {
						SI::natural::sort(_sequencePaths);

						if (FRAME >= 0 && FRAME < _sequencePaths.size()) {
							openvdb::io::File file(_sequencePaths[FRAME]);

							if (file.open()) {

								if (gridAdapter != nullptr) {

									if (gridAdapter->gridImplementation() == DfxGridTypes::OPEN_VDB) {

										auto vdbAdapter = static_cast<DfxOpenVDBGridAdapter*>(gridAdapter);
										vdbAdapter->clear();

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

										if (vdbAdapter->channelCount() > 0) {
											auto cache = gridHandler.volumeCache();
											DfxOpenGLCacheQueue::getInstance()->queueCache(cache);
										}
									}
								}

								file.close();
							}
						}
					}
				}
			}
		}
	}
}

void DfxEntropyCachePropNode::cache(boom::Entropy *entropy, float voxel_size, glm::ivec3 dims) {
	namespace xamp = concurrency;

	if (DfxTimelineManager::getInstance()->isSimulating()) {
		DfxEntropyCachePropHandler handler;
		DfxGridHandler gridHandler;

		if (handler.fetch(referencePath())) {
			auto dao = handler.dataObject()->rawData<fl::DataObject>();

			auto save_to_disk = dao->property("save_to_disk")->data<bool>();
			auto filename = dao->property("filename")->data<std::string>();
			auto directory = dao->property("directory")->data<std::string>();
			auto grid_path = dao->property("grid")->data<std::string>();

			deepsg::SgGridAdapterInterface *gridAdapter = nullptr;

			if (gridHandler.fetch(grid_path)) {
				auto dao = gridHandler.dataObject()->rawData<fl::DataObject>();

				if (dao->hasProperty("sg_grid_node")) {
					auto node = gridHandler.gridNode();
					gridAdapter = node->getGridAdapter();
				}
			}

			auto FRAME = DfxTimelineManager::getInstance()->currentFrame();

			openvdb::math::Transform::Ptr linearTransform = openvdb::math::Transform::createLinearTransform(voxel_size);

			openvdb::FloatGrid::Ptr density_grid = openvdb::FloatGrid::create(0.0);
			density_grid->setTransform(linearTransform);
			density_grid->setName("density");

			openvdb::FloatGrid::Ptr temperature_grid = openvdb::FloatGrid::create(0.0);
			temperature_grid->setTransform(linearTransform);
			temperature_grid->setName("temperature");

			auto density_accessor = density_grid->getAccessor();
			auto temperature_accessor = temperature_grid->getAccessor();

			std::vector<float> density;
			std::vector<float> temerature;
			std::vector<int> cellType;

			density.resize(dims.x * dims.y * dims.z, 0.f);
			temerature.resize(dims.x * dims.y * dims.z, 0.f);
			cellType.resize(dims.x * dims.y * dims.z, 0.f);

			xamp::copy(entropy->getDens(), density.begin());
			xamp::copy(entropy->getTemperature(), temerature.begin());
			xamp::copy(entropy->getCellType(), cellType.begin());

			for (int i = 0; i < entropy->getGridSizeX(); i++) {
				for (int j = 0; j < entropy->getGridSizeY(); j++) {
					for (int k = 0; k < entropy->getGridSizeZ(); k++) {

						float d_value = density[boom::Entropy::index3d(i, j, k, entropy->getGridSizeX(), entropy->getGridSizeY())];
						if (d_value > 0 && cellType[boom::Entropy::index3d(i, j, k, entropy->getGridSizeX(), entropy->getGridSizeY())] != boom::constants::BOUNDARY) {
							density_accessor.setValue(openvdb::Coord(i, j, k), d_value);
						}

						float t_value = temerature[boom::Entropy::index3d(i, j, k, entropy->getGridSizeX(), entropy->getGridSizeY())];
						if (t_value > 0 && cellType[boom::Entropy::index3d(i, j, k, entropy->getGridSizeX(), entropy->getGridSizeY())] != boom::constants::BOUNDARY) {
							temperature_accessor.setValue(openvdb::Coord(i, j, k), t_value);
						}
					}
				}
			}

			auto frame = DfxTimelineManager::getInstance()->currentFrame();

			openvdb::GridPtrVec grids;
			grids.push_back(density_grid);
			grids.push_back(temperature_grid);

			// Save To Temp Folder			
			{
				QString appdata = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)[0];

				auto path = (appdata + "/tmp/entropy_cache_" + QString(std::to_string(random_num).c_str())).toStdString();
				path = path + "/cache_" + std::to_string(FRAME) + ".vdb";

				openvdb::io::File file(path);
				file.write(grids);
				file.close();
			}

			if (gridAdapter != nullptr) {
				if (gridAdapter->gridImplementation() == DfxGridTypes::OPEN_VDB) {
					auto vdbAdapter = static_cast<DfxOpenVDBGridAdapter*>(gridAdapter);
					vdbAdapter->clear();

					auto dens_channel = new DfxVDBGridChannel;
					dens_channel->setVDBGrid(grids[0]);
					dens_channel->initAccessor();

					auto temp_channel = new DfxVDBGridChannel;
					temp_channel->setVDBGrid(grids[1]);
					temp_channel->initAccessor();

					vdbAdapter->addChannel(dens_channel);
					vdbAdapter->addChannel(temp_channel);
				}

				if (gridAdapter->channelCount() > 0) {
					auto cache = gridHandler.volumeCache();
					DfxOpenGLCacheQueue::getInstance()->queueCache(cache);
				}
			}

			if (save_to_disk) {
				
				filesystem::path dirPath(directory);

				if (filesystem::exists(dirPath) && filesystem::is_directory(dirPath)) {
					std::string xname = filename.empty() ? "default" : filename;
					std::string full_path = directory + "/" + xname + "_" + std::to_string(frame) + ".vdb";
					openvdb::io::File file(full_path);
					file.write(grids);
					file.close();
				}
			}
			

		}
	}
}

std::vector<std::pair<QWidget*, QString>> DfxEntropyCachePropNode::propertyForms() {
	std::pair<QWidget*, QString> properties(properties(), "Cache Properties");

	return std::vector<std::pair<QWidget*, QString>> { properties };
}

QWidget *DfxEntropyCachePropNode::properties() {
	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto vizControlPanel = new DfxControlPanel();
	auto outputControlPanel = new DfxControlPanel();

	DfxQControlArray *vizControlArray = new DfxQControlArray();
	DfxQControlArray *outputControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// Type Controls
	auto grid = new DfxQFluentPathControl(path + "grid", "", false);

	auto save_to_disk = new DfxQCheckBoxControl(path + "save_to_disk", "", false);
	auto filename = new DfxQLineEditControl(path + "filename", "", false);
	auto dir = new DfxQFilePathControl(path + "directory", true, true, "Output Directory", "c:/", "", "", false);

	// Viz
	vizControlArray->addControl("Volume", grid);

	vizControlPanel->setTitle("Visualization");
	vizControlPanel->setWidget(vizControlArray);

	// Output
	outputControlArray->addControl(save_to_disk, "Save To Disk");
	outputControlArray->addSpacer(1, 2);
	outputControlArray->addControl("Filename", filename);
	outputControlArray->addSpacer(1, 2);
	outputControlArray->addControl("Directory", dir);

	outputControlPanel->setTitle("Output");
	outputControlPanel->setWidget(outputControlArray);

	// Main layout
	main_layout->addWidget(vizControlPanel);
	main_layout->addWidget(outputControlPanel);

	widget->setLayout(main_layout);

	return widget;
}

DfxEntropyCachePropNode::CurrentFrameObserver::CurrentFrameObserver(DfxEntropyCachePropNode *cn) {
	_cacheNode = cn;
}

void DfxEntropyCachePropNode::CurrentFrameObserver::update(fl::Any *any) {
	if (any != nullptr) {
		auto  val = any->data<int>();

		if (!DfxTimelineManager::getInstance()->isSimulating()) {
			_cacheNode->updateVisualization();
		}

	}
}

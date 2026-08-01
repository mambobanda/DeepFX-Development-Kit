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
#include <openvdb/util/Util.h>
#include <openvdb/tools/MeshToVolume.h>
#include <regex>
#include "DfxQPushButtonStyles.h"
#include "DfxUpdater.h"
#include "DfxGridTypes.h"
#include "DfxGridHandler.h"
#include "DfxMeshToGridTool.h"
#include "DfxMeshToGridHandler.h"
#include "DfxOpenVDBGridAdapter.h"
#include "DfxVDBGridChannel.h"

std::string DfxMeshToGridTool::_uniqueName = "UID_MESH_TO_GRID_TOOL";

typedef struct {
	std::vector<openvdb::Vec3s> points;
	std::vector<openvdb::Vec4I> polys;

	unsigned int polygonCount();
}VDB_TempMesh;

unsigned int VDB_TempMesh::polygonCount() {
	return polys.size();
}

DfxMeshToGridTool::DfxMeshToGridTool() {
	openvdb::initialize();
	_dispatcher = nullptr;
}

DfxMeshToGridTool::~DfxMeshToGridTool() {

}

std::string DfxMeshToGridTool::uniqueName() {
	return _uniqueName;
}

std::string DfxMeshToGridTool::getUniqueName() {
	return _uniqueName;
}

bool DfxMeshToGridTool::isReady() {
	return true;
}

void DfxMeshToGridTool::onSetReferencePath(std::string path) {
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

void DfxMeshToGridTool::create(SgNode *parent) {
}

void DfxMeshToGridTool::destroy() {
}

bool DfxMeshToGridTool::evaluateOnUpdate() {
	return false;
}

bool DfxMeshToGridTool::evaluateOnNewFrame() {
	return true;
}

bool DfxMeshToGridTool::evaluateOnValueChanged() {
	return true;
}

void DfxMeshToGridTool::convertMesh() {
	if (deepsg::SgNode::parent() != nullptr) {
		_generate = true;
		deepsg::SgNode::parent()->evaluateTools();
		DfxUpdater::getInstance()->sceneUpdated();
	}
}

void DfxMeshToGridTool::evaluate(SgNode *parent) {
	DfxMeshToGridHandler toolHandler;
	DfxGridHandler parentHandler;

	if (parent->getType() == deepsg::SgNodeType::GRID) {

		if (toolHandler.fetch(referencePath()) && parentHandler.fetch(parent->referencePath())) {

			auto vadapter = ((DfxGridNode*)parent)->getGridAdapter();
			auto dao = toolHandler.dataObject()->rawData<fl::DataObject>();

			deepsg::SgNode *_node = nullptr;
			deepsg::SgGeoAdapterInterface *_interface = nullptr;

			auto grid_name = dao->property("grid_name")->data<std::string>();
			auto mesh_path = dao->property("mesh_path")->data<std::string>();
			auto voxel_size = dao->property("voxel_size")->data<float>();
			auto exterior_band = dao->property("exterior_band")->data<float>();
			auto interior_band = dao->property("interior_band")->data<float>();
			auto apply_transform = dao->property("apply_transform")->data<bool>();
			auto append = dao->property("append")->data<bool>();
			auto autoSample = dao->property("auto_sample_mesh")->data<bool>();

			if (autoSample || _generate) {
				_generate = false;

				if (vadapter->gridImplementation() == DfxGridTypes::OPEN_VDB &&
					!mesh_path.empty()) {
					auto vdbAdapter = static_cast<DfxOpenVDBGridAdapter*>(vadapter);
					if (!append) vdbAdapter->clearAll();

					auto any = DfxSpace::getInstance()->findAbsolute(mesh_path);

					if (any != nullptr) {

						if (any->type() == fl::type::DATA_OBJECT) {
							auto dao = any->rawData<fl::DataObject>();

							if (dao->hasProperty("sg_geo_node")) {
								_node = dao->property("sg_geo_node")->rawData<deepsg::SgGeoNode>();
								if (_node != nullptr) {
									_interface = ((deepsg::SgGeoNode*)_node)->getGeometryAdapter();
								}
							}
						}
					}

					if (_interface != nullptr) {

						if (_interface->getVertexCount() >= 3 && _interface->getPolygonCount() > 0) {

							VDB_TempMesh tm;
							openvdb::math::Transform::Ptr vdb_xform = openvdb::math::Transform::createLinearTransform(voxel_size);
							glm::mat4 xform(1.f);

							if (apply_transform && _node != nullptr) xform = deepsg::SgRootNode::getInstance()->getTransformation4x4(_node);

							auto tricount = int(_interface->getTriangleIndexListSize() / 3);
							std::unique_ptr<unsigned int[]> triIndices(_interface->getTriangleIndexList());

							for (int i = 0; i < _interface->getVertexCount(); i++) {
								glm::vec3 transPos = glm::vec3(xform * glm::vec4(_interface->getVertex(i), 1.0f));
								openvdb::Vec3s pos = vdb_xform->worldToIndex(openvdb::Vec3s(transPos.x, transPos.y, transPos.z));
								tm.points.push_back(pos);
							}

							for (int i = 0; i < tricount; i++) {
								auto offset = i * 3;
								tm.polys.push_back(openvdb::Vec4I(
									triIndices.get()[offset],
									triIndices.get()[offset + 1],
									triIndices.get()[offset + 2],
									openvdb::util::INVALID_IDX));
							}

							openvdb::tools::QuadAndTriangleDataAdapter<openvdb::Vec3s, openvdb::Vec4I> tmsh(tm.points, tm.polys);
							openvdb::GridBase::Ptr grid = openvdb::tools::meshToVolume<openvdb::FloatGrid>(tmsh, *vdb_xform, exterior_band, interior_band);
							grid->setName(grid_name.empty() ? "surface" : grid_name);

							auto channel = new DfxVDBGridChannel;
							channel->setVDBGrid(grid);
							channel->initAccessor();
							vdbAdapter->addChannel(channel);

							if (vdbAdapter->channelCount() > 0) {
								auto cache = parentHandler.volumeCache();
								DfxOpenGLCacheQueue::getInstance()->queueCache(cache);
							}
						}

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

std::vector<std::pair<QWidget*, QString>> DfxMeshToGridTool::propertyForms() {
	std::pair<QWidget*, QString> grid(form(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> {grid};
}

QWidget *DfxMeshToGridTool::form() {

	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto propertiesControlPanel = new DfxControlPanel();

	DfxQControlArray *propertiesControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	QPushButton *convertButton = new QPushButton("Convert");
	convertButton->setStyleSheet(convertButton->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());

	// Type Controls
	auto grid_name = new DfxQLineEditControl(path + "grid_name", "", false);
	auto mesh_path = new DfxQFluentPathControl(path + "mesh_path", "", false);
	auto voxel_size = new DfxQDecimalSpinnerControl(path + "voxel_size", "", false);
	auto exterior_band = new DfxQDecimalSpinnerControl(path + "exterior_band", "", false);
	auto interior_band = new DfxQDecimalSpinnerControl(path + "interior_band", "", false);
	auto auto_sample_mesh = new DfxQCheckBoxControl(path + "auto_sample_mesh", "", false);
	auto apply_transform = new DfxQCheckBoxControl(path + "apply_transform", "", false);
	auto append = new DfxQCheckBoxControl(path + "append", "", false);

	propertiesControlArray->addControl(convertButton);
	propertiesControlArray->addSpacer(1, 3);
	propertiesControlArray->addControl("Grid Name", grid_name);
	propertiesControlArray->addSpacer(1,3);
	propertiesControlArray->addControl("Mesh Path", mesh_path);
	propertiesControlArray->addSpacer(1, 3);
	propertiesControlArray->addControl("Voxel Size", voxel_size);
	propertiesControlArray->addSpacer(1, 3);
	propertiesControlArray->addControl("Outer Band", exterior_band);
	propertiesControlArray->addSpacer(1, 3);
	propertiesControlArray->addControl("Inner Band", interior_band);
	propertiesControlArray->addSpacer(1, 3);
	propertiesControlArray->addControl("Auto Convert Mesh", auto_sample_mesh);
	propertiesControlArray->addSpacer(1, 3);
	propertiesControlArray->addControl("Apply Transform", apply_transform);
	propertiesControlArray->addSpacer(1, 3);
	propertiesControlArray->addControl("Append", append);

	propertiesControlPanel->setTitle("Properties");
	propertiesControlPanel->setWidget(propertiesControlArray);

	// Main layout
	main_layout->addWidget(propertiesControlPanel);

	widget->setLayout(main_layout);

	QObject::connect(convertButton, &QPushButton::clicked, this, &DfxMeshToGridTool::convertMesh);

	return widget;
}
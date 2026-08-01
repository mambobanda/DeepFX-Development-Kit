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

#include <openvdb/tools/GridTransformer.h>
#include "DfxEntropyDomainPropNode.h"

std::string DfxEntropyDomainPropNode::_uniqueName = "UID_ENTROPY_DOMAIN_PROP_NODE";

DfxEntropyDomainPropNode::DfxEntropyDomainPropNode(deepsg::SgStageNode *stage) : DfxSimulationStagePropBase(stage, entropy::type::SIM_DOMAIN) {
	setPropState(DfxEntropyDomainPropNode::PropState::SPAWN);
}

DfxEntropyDomainPropNode::~DfxEntropyDomainPropNode() {

}

std::string DfxEntropyDomainPropNode::uniqueName() {
	return _uniqueName;
}

std::string DfxEntropyDomainPropNode::getUniqueName() {
	return _uniqueName;
}

void DfxEntropyDomainPropNode::clear() {

}

deepsg::SgShape *DfxEntropyDomainPropNode::shape() {

	if (_shape != nullptr) {
		delete _shape;
		_shape = nullptr;
	}

	_shape = new deepsg::SgShape;
	_shape->setScale(1.f);

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {

		if (any->type() == fl::type::DATA_OBJECT) {
			auto dao = any->rawData<fl::DataObject>();

			return drawDomain(dao);		
		}

	}

	return nullptr;
}

size_t DfxEntropyDomainPropNode::propState() {
	return _propState;
}

void DfxEntropyDomainPropNode::setPropState(size_t ps) {
	_propState = ps;
}

void DfxEntropyDomainPropNode::spawn() {
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {
		auto dao = any->rawData<fl::DataObject>();

		auto voxel_size = dao->property("voxel_size")->data<float>();

		if (voxel_size > 0.0001f) {
			if (_sourceGrid != nullptr) _sourceGrid.reset();
			if (_obstacleGrid != nullptr) _obstacleGrid.reset();

			_sourceGrid = openvdb::FloatGrid::create();
			_obstacleGrid = openvdb::FloatGrid::create();

			auto xform = position();
			openvdb::math::Transform::Ptr linearTransform = openvdb::math::Transform::createLinearTransform(voxel_size);
			linearTransform->preTranslate(openvdb::Vec3d(xform.x, xform.y, xform.z));

			_sourceGrid->setTransform(linearTransform);
			_obstacleGrid->setTransform(linearTransform);
		}
	}
}

void DfxEntropyDomainPropNode::reset() {
	if (_sourceGrid != nullptr) _sourceGrid.reset();
	if (_obstacleGrid != nullptr) _obstacleGrid.reset();
}

void DfxEntropyDomainPropNode::addSource(SourceInfo info, deepsg::SgGridAdapterInterface *adapter, boom::Entropy *entropy, glm::mat4 xform) {
	
	boom::emitters::FixedEmitterSettings emitterSettings;

	emitterSettings.density = info.inflow_density;
	emitterSettings.temperature = info.inflow_temperature;
	emitterSettings.useVelocity = 0;

	auto voxel = voxelSize();
	auto dims = dimensions();
	auto xbounds = bounds();
	auto d_pos = position();

	if (adapter->channelCount() > 0) {
		auto channel = adapter->channel(adapter->channelName(0));

		if (channel->valueType() == deepsg::SgGridChannel::FLOAT) {
			
			std::vector<float> coords;
			std::vector<int> emitter_map;
			emitter_map.resize(dims.x * dims.y * dims.z, -1);

			channel->allCoordinatesFloat(1.f, coords);

			openvdb::math::Transform::Ptr linearTransform = openvdb::math::Transform::createLinearTransform(voxel);
			linearTransform->preTranslate(openvdb::Vec3d(d_pos.x, d_pos.y, d_pos.z));

			for (int i = 0; i < coords.size() / 3; i++) {
				auto offset = i * 3;

				auto pos = glm::vec3(xform * glm::vec4(glm::vec3(coords[offset], coords[offset + 1], coords[offset + 2]), 1.0f));
				if (xbounds.intersectsPoint(pos.x, pos.y, pos.z)) {
					auto index3i = linearTransform->worldToIndexCellCentered(openvdb::Vec3d(pos.x, pos.y, pos.z));
					auto inx = boom::Entropy::index3d(index3i.x(), index3i.y(), index3i.z(), dims.x, dims.y);

					emitter_map[inx] = 0;
 				}
			}

			entropy->addFixedEmitter(emitter_map, emitterSettings);
		}
	}
}

void DfxEntropyDomainPropNode::addObstacle(deepsg::SgGridAdapterInterface *adapter, boom::Entropy *entropy, glm::mat4 xform) {

}

DfxBounds DfxEntropyDomainPropNode::bounds() {
	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {
		auto dao = any->rawData<fl::DataObject>();

		auto voxel = dao->property("voxel_size")->data<float>();
		auto width = dao->property("width")->data<int>();
		auto height = dao->property("height")->data<int>();
		auto depth = dao->property("depth")->data<int>();

		glm::vec3 min(0.f, 0.f, 0.f);
		glm::vec3 max(width * voxel, height * voxel, depth * voxel);

		auto xform = glm::translate(glm::mat4(1.f), position());

		min = glm::vec3(xform * glm::vec4(min, 1.f));
		max = glm::vec3(xform * glm::vec4(max, 1.f));

		return DfxBounds(min, max);
	}

	return DfxBounds();
}

deepsg::SgShape *DfxEntropyDomainPropNode::drawDomain(fl::DataObject *dao) {

	auto voxel_size = dao->property("voxel_size")->data<float>();
	auto width  = dao->property("width")->data<int>();
	auto height = dao->property("height")->data<int>();
	auto depth  = dao->property("depth")->data<int>();

	auto wmax = voxel_size * width;
	auto hmax = voxel_size * height;
	auto dmax = voxel_size * depth;

	deepsg::SgShapeBuffer shBuffer;
	shBuffer.setColor(153, 255, 192);

	// Domain
	shBuffer.drawLine(glm::vec3(.0f, 0.f, .0f),   glm::vec3(.0f, hmax, 0.f));
	shBuffer.drawLine(glm::vec3(.0f, 0.f, dmax),  glm::vec3(.0f, hmax, dmax));
	shBuffer.drawLine(glm::vec3(wmax, 0.f, .0f),  glm::vec3(wmax, hmax, 0.f));
	shBuffer.drawLine(glm::vec3(wmax, 0.f, dmax), glm::vec3(wmax, hmax, dmax));

	shBuffer.drawLine(glm::vec3(.0f, 0.f, .0f),   glm::vec3(wmax, .0f, 0.f));
	shBuffer.drawLine(glm::vec3(.0f, hmax, .0f),  glm::vec3(wmax, hmax, 0.f));
	shBuffer.drawLine(glm::vec3(.0f, 0.f, dmax),  glm::vec3(wmax, .0f, dmax));
	shBuffer.drawLine(glm::vec3(.0f, hmax, dmax), glm::vec3(wmax, hmax, dmax));

	shBuffer.drawLine(glm::vec3(.0f, 0.f, .0f),   glm::vec3(0.f, .0f, dmax));
	shBuffer.drawLine(glm::vec3(wmax, 0.f, .0f),  glm::vec3(wmax, .0f, dmax));
	shBuffer.drawLine(glm::vec3(.0f, hmax, .0f),  glm::vec3(0.f, hmax, dmax));
	shBuffer.drawLine(glm::vec3(wmax, hmax, .0f), glm::vec3(wmax, hmax, dmax));

	// Voxel
	shBuffer.drawLine(glm::vec3(.0f, 0.f, .0f), glm::vec3(.0f, voxel_size, 0.f));
	shBuffer.drawLine(glm::vec3(.0f, 0.f, voxel_size), glm::vec3(.0f, voxel_size, voxel_size));
	shBuffer.drawLine(glm::vec3(voxel_size, 0.f, .0f), glm::vec3(voxel_size, voxel_size, 0.f));
	shBuffer.drawLine(glm::vec3(voxel_size, 0.f, voxel_size), glm::vec3(voxel_size, voxel_size, voxel_size));

	shBuffer.drawLine(glm::vec3(.0f, 0.f, .0f), glm::vec3(voxel_size, .0f, 0.f));
	shBuffer.drawLine(glm::vec3(.0f, voxel_size, .0f), glm::vec3(voxel_size, voxel_size, 0.f));
	shBuffer.drawLine(glm::vec3(.0f, 0.f, voxel_size), glm::vec3(voxel_size, .0f, voxel_size));
	shBuffer.drawLine(glm::vec3(.0f, voxel_size, voxel_size), glm::vec3(voxel_size, voxel_size, voxel_size));

	shBuffer.drawLine(glm::vec3(.0f, 0.f, .0f), glm::vec3(0.f, .0f, voxel_size));
	shBuffer.drawLine(glm::vec3(voxel_size, 0.f, .0f), glm::vec3(voxel_size, .0f, voxel_size));
	shBuffer.drawLine(glm::vec3(.0f, voxel_size, .0f), glm::vec3(0.f, voxel_size, voxel_size));
	shBuffer.drawLine(glm::vec3(voxel_size, voxel_size, .0f), glm::vec3(voxel_size, voxel_size, voxel_size));

	_shape->shapes().push_back(shBuffer);

	return _shape;
}

glm::ivec3 DfxEntropyDomainPropNode::dimensions() {

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {
		auto dao = any->rawData<fl::DataObject>();

		auto width = dao->property("width")->data<int>();
		auto height = dao->property("height")->data<int>();
		auto depth = dao->property("depth")->data<int>();

		return glm::ivec3(width, height, depth);
	}

	return glm::ivec3(0, 0, 0);
}

float DfxEntropyDomainPropNode::voxelSize() {

	auto any = DfxSpace::getInstance()->findAbsolute(referencePath());

	if (any != nullptr) {
		auto dao = any->rawData<fl::DataObject>();

		auto voxel_size = dao->property("voxel_size")->data<float>();

		return voxel_size;
	}

	return 0.f;
}

std::vector<std::pair<QWidget*, QString>> DfxEntropyDomainPropNode::propertyForms() {
	std::pair<QWidget*, QString> properties(properties(), "Domain Properties");

	return std::vector<std::pair<QWidget*, QString>> { properties };
}

QWidget *DfxEntropyDomainPropNode::properties() {
	QWidget *widget = new QWidget;

	QVBoxLayout *main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	auto propertiesControlPanel = new DfxControlPanel();

	DfxQControlArray *propertiesControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	auto voxel_size = new DfxQDecimalSpinnerControl(path + "voxel_size", "", false);
	
	auto width = new DfxQIntegerSpinnerControl(path + "width", "", false);
	auto height = new DfxQIntegerSpinnerControl(path + "height", "", false);
	auto depth = new DfxQIntegerSpinnerControl(path + "depth", "", false);

	voxel_size->setMinValue(0.0001f);

	width->setMinValue(10);
	height->setMinValue(10);
	depth->setMinValue(10);

	propertiesControlArray->addControl("Voxel Size", voxel_size);
	propertiesControlArray->addSpacer(0, 6);
	propertiesControlArray->addControl("Width", width);
	propertiesControlArray->addSpacer(0, 2);
	propertiesControlArray->addControl("Height", height);
	propertiesControlArray->addSpacer(0, 2);
	propertiesControlArray->addControl("Depth", depth);

	propertiesControlPanel->setTitle("Settings");
	propertiesControlPanel->setWidget(propertiesControlArray);

	main_layout->addWidget(propertiesControlPanel);

	widget->setLayout(main_layout);

	return widget;
}

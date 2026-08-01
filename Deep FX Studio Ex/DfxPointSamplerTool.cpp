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

#include <SgPointCloudNode.h>
#include "DfxPointSampling.h"
#include "DfxUpdater.h"
#include "DfxQPushButtonStyles.h"
#include "DfxPointSamplerTool.h"
#include "DfxPointSamplerHandler.h"

std::string DfxPointSamplerTool::_uniqueName = "UID_SAMPLING_POINTS_TOOL";

DfxPointSamplerTool::DfxPointSamplerTool() {

	_dispatcher = nullptr;

	typeQControlenums.push_back(std::pair<std::string, unsigned int>("Surface", Type::SURFACE));
	typeQControlenums.push_back(std::pair<std::string, unsigned int>("Volume", Type::VOLUME));

	surfaceSamplingQControlenums.push_back(std::pair<std::string, unsigned int>("Regular", SurfaceSamplingType::REGULAR));
	surfaceSamplingQControlenums.push_back(std::pair<std::string, unsigned int>("Poisson", SurfaceSamplingType::POISSON));
	surfaceSamplingQControlenums.push_back(std::pair<std::string, unsigned int>("Triangle", SurfaceSamplingType::TRIANGLE));

	volumeSamplingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Voxel", VolumeSamplingType::VOXEL));
	volumeSamplingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Uniform Random", VolumeSamplingType::UNIFORM_RANDOM));
	volumeSamplingTypeQControlenums.push_back(std::pair<std::string, unsigned int>("Sphere", VolumeSamplingType::SPHERE));

	_generate = false;
}

DfxPointSamplerTool::~DfxPointSamplerTool() {

}

std::string DfxPointSamplerTool::uniqueName() {
	return _uniqueName;
}

std::string DfxPointSamplerTool::getUniqueName() {
	return _uniqueName;
}

bool DfxPointSamplerTool::isReady() {
	return true;
}

void DfxPointSamplerTool::onSetReferencePath(std::string path) {
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

void DfxPointSamplerTool::create(SgNode *parent) {
}

void DfxPointSamplerTool::destroy() {
}

bool DfxPointSamplerTool::evaluateOnUpdate() {
	return false;
}

bool DfxPointSamplerTool::evaluateOnNewFrame() {
	return true;
}

bool DfxPointSamplerTool::evaluateOnValueChanged() {
	return true;
}

void DfxPointSamplerTool::sampleMesh() {
	if (deepsg::SgNode::parent() != nullptr) {
		_generate = true;
		deepsg::SgNode::parent()->evaluateTools();
		DfxUpdater::getInstance()->sceneUpdated();
	}
}

void DfxPointSamplerTool::evaluate(SgNode *parent) {
	DfxPointSamplerHandler handler;

	if (parent->getType() == deepsg::SgNodeType::POINT_CLOUD) {

		auto pcloud = ((deepsg::SgPointCloudNode*)parent)->geo();

		if (handler.fetch(referencePath())) {

			auto dao = handler.dataObject()->rawData<fl::DataObject>();

			deepsg::SgNode *_node = nullptr;
			deepsg::SgGeoAdapterInterface *_interface = nullptr;

			auto gen_type = dao->property("type")->data<size_t>();
			auto surf_samp_type = dao->property("surface_sampler")->data<size_t>();
			auto vol_samp_type = dao->property("volume_sampler")->data<size_t>();
			auto objectPath = dao->property("object")->data<std::string>();
			auto autoSample = dao->property("auto_sample_mesh")->data<bool>();

			if (autoSample || _generate) {
				_generate = false;

				auto any = DfxSpace::getInstance()->findAbsolute(objectPath);

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

				pcloud->clear();

				if (gen_type == DfxPointSamplerTool::Type::SURFACE) {

					if (surf_samp_type == DfxPointSamplerTool::SurfaceSamplingType::TRIANGLE) {
						DfxPointSampling::TriangleConfig config;

						auto use_resolution = dao->property("triangle_use_resolution")->data<bool>();
						auto apply_transform = dao->property("triangle_apply_transform")->data<bool>();

						config.radius = dao->property("triangle_radius")->data<float>();
						config.xform = glm::mat4(1.f);

						if (use_resolution) config.radius = (DfxPointSampling::particleRadiusFromResolution(config.radius) * 1.5f) * 0.5f;

						if (apply_transform && _node != nullptr) config.xform = deepsg::SgRootNode::getInstance()->getTransformation4x4(_node);

						if (_interface != nullptr) {
							auto samples = DfxPointSampling::triangleSampling(_interface, config);

							if (!samples.empty()) {
								pcloud->resize(samples.size());

								for (int i = 0; i < samples.size(); i++) {
									pcloud->at<glm::vec3>("@position")[i] = samples[i];
								}
							}
						}
						else {
							DfxGlobal::console::println("Point Sampling Failed: No Geometry");
						}
					}
					else if (surf_samp_type == DfxPointSamplerTool::SurfaceSamplingType::REGULAR) {
						DfxPointSampling::RegularConfig config;

						auto use_resolution = dao->property("regular_use_resolution")->data<bool>();
						auto use_packed_resolution = dao->property("regular_packed_resolution")->data<bool>();
						auto apply_transform = dao->property("regular_apply_transform")->data<bool>();

						config.radius = dao->property("regular_radius")->data<float>();
						config.xform = glm::mat4(1.f);

						if (use_resolution) {
							if(!use_packed_resolution) config.radius = (DfxPointSampling::particleRadiusFromResolution(config.radius) * 2.f);
							else config.radius = (DfxPointSampling::particleRadiusFromResolution(config.radius) * 1.5f);
						}

						if (_interface != nullptr) {
							auto samples = DfxPointSampling::regularSampling(_interface, config);

							if (!samples.empty()) {
								pcloud->resize(samples.size());

								for (int i = 0; i < samples.size(); i++) {
									pcloud->at<glm::vec3>("@position")[i] = samples[i];
								}
							}
						}
						else {
							DfxGlobal::console::println("Point Sampling Failed: No Geometry");
						}
					}
					else if (surf_samp_type == DfxPointSamplerTool::SurfaceSamplingType::POISSON) {
						DfxPointSampling::PoissonConfig config;

						auto use_resolution = dao->property("poisson_use_resolution")->data<bool>();
						auto use_packed_resolution = dao->property("poisson_packed_resolution")->data<bool>();
						auto apply_transform = dao->property("poisson_apply_transform")->data<bool>();

						config.radius = dao->property("poisson_radius")->data<float>();
						config.xform = glm::mat4(1.f);

						if (apply_transform && _node != nullptr) config.xform = deepsg::SgRootNode::getInstance()->getTransformation4x4(_node);

						if (use_resolution) {
							if (!use_packed_resolution) config.radius = (DfxPointSampling::particleRadiusFromResolution(config.radius) * 2.f);
							else config.radius = (DfxPointSampling::particleRadiusFromResolution(config.radius) * 1.3f);
						}

						if (_interface != nullptr) {
							auto samples = DfxPointSampling::poissonSampling(_interface, config);

							if (!samples.empty()) {
								pcloud->resize(samples.size());

								for (int i = 0; i < samples.size(); i++) {
									pcloud->at<glm::vec3>("@position")[i] = samples[i];
								}
							}
						}
						else {
							DfxGlobal::console::println("Point Sampling Failed: No Geometry");
						}
					}

				}
				else if (gen_type == DfxPointSamplerTool::Type::VOLUME) {

					if (vol_samp_type == DfxPointSamplerTool::VolumeSamplingType::VOXEL) {
						DfxPointSampling::VoxelConfig config;

						auto use_resolution = dao->property("voxel_use_resolution")->data<bool>();
						auto apply_transform = dao->property("voxel_apply_transform")->data<bool>();

						config.radius = dao->property("voxel_radius")->data<float>();
						config.xform = glm::mat4(1.f);

						if (apply_transform && _node != nullptr) config.xform = deepsg::SgRootNode::getInstance()->getTransformation4x4(_node);

						if (use_resolution) config.radius = (DfxPointSampling::particleRadiusFromResolution(config.radius));

						if (_interface != nullptr) {
							auto samples = DfxPointSampling::voxelSampling(_interface, config);

							if (!samples.empty()) {
								pcloud->resize(samples.size());

								for (int i = 0; i < samples.size(); i++) {
									pcloud->at<glm::vec3>("@position")[i] = samples[i];
								}
							}
						}
						else {
							DfxGlobal::console::println("Point Sampling Failed: No Geometry");
						}
					}
					else if (vol_samp_type == DfxPointSamplerTool::VolumeSamplingType::UNIFORM_RANDOM) {
						DfxPointSampling::UniformRandomConfig config;

						config.rand_seed = dao->property("uniform_random_seed")->data<size_t>();
						config.cell_count = dao->property("uniform_random_count")->data<size_t>();
						auto apply_transform = dao->property("uniform_apply_transform")->data<bool>();

						config.xform = glm::mat4(1.f);

						if (apply_transform && _node != nullptr) config.xform = deepsg::SgRootNode::getInstance()->getTransformation4x4(_node);

						if (_interface != nullptr) {
							auto samples = DfxPointSampling::uniformRandomSampling(_interface, config);

							if (!samples.empty()) {
								pcloud->resize(samples.size());

								for (int i = 0; i < samples.size(); i++) {
									pcloud->at<glm::vec3>("@position")[i] = samples[i];
								}
							}
						}
						else {
							DfxGlobal::console::println("Point Sampling Failed: No Geometry");
						}

					}
					else if (vol_samp_type == DfxPointSamplerTool::VolumeSamplingType::SPHERE) {
						DfxPointSampling::SphereConfig config;

						config.center = glm::vec3(0.f);
						config.count = dao->property("sphere_count")->data<size_t>();
						config.rand_seed = dao->property("sphere_random_seed")->data<size_t>();
						config.radius = dao->property("sphere_radius")->data<size_t>();
						
						auto apply_transform = dao->property("sphere_apply_transform")->data<bool>();

						// if (apply_transform && _node != nullptr) config.xform = deepsg::SgRootNode::getInstance()->getTransformation4x4(_node);

						if (_interface != nullptr) {
							auto samples = DfxPointSampling::sphereSampling(config);

							if (!samples.empty()) {
								pcloud->resize(samples.size());

								for (int i = 0; i < samples.size(); i++) {
									pcloud->at<glm::vec3>("@position")[i] = samples[i];
								}
							}
						}
						else {
							DfxGlobal::console::println("Point Sampling Failed: No Geometry");
						}
					}
				}
			}

			parent->emitSignal(deepsg::SgNode::Signal::UPDATED);
		}
	}
}

std::vector<std::pair<QWidget*, QString>> DfxPointSamplerTool::propertyForms() {
	std::pair<QWidget*, QString> grid(form(), "Properties");

	return std::vector<std::pair<QWidget*, QString>> { grid };
}

QWidget *DfxPointSamplerTool::form() {

	QWidget *widget = new QWidget;

	QWidget *surfaceWidget = new QWidget;
	QWidget *volumeWidget  = new QWidget;

	DfxQStackControl *typeStack    = new DfxQStackControl;
	DfxQStackControl *surfaceStack = new DfxQStackControl;
	DfxQStackControl *volumeStack  = new DfxQStackControl;

	QVBoxLayout *main_layout = new QVBoxLayout;

	QVBoxLayout *surface_type_layout = new QVBoxLayout;

	QVBoxLayout *volume_type_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 15, 0, 0);
	main_layout->setAlignment(Qt::AlignTop);

	surface_type_layout->setContentsMargins(0, 5, 0, 0);
	surface_type_layout->setAlignment(Qt::AlignTop);

	volume_type_layout->setContentsMargins(0, 5, 0, 0);
	volume_type_layout->setAlignment(Qt::AlignTop);

	auto typeControlPanel = new DfxControlPanel();

	auto surfaceTypePanel = new DfxControlPanel();

	auto volumeTypePanel = new DfxControlPanel();

	DfxQControlArray *typeControlArray = new DfxQControlArray();

	DfxQControlArray *surfaceTypeControlArray = new DfxQControlArray();
	DfxQControlArray *regularSamplingTypeControlArray = new DfxQControlArray();
	DfxQControlArray *poissonSamplingTypeControlArray = new DfxQControlArray();
	DfxQControlArray *triangleSamplingTypeControlArray = new DfxQControlArray();

	DfxQControlArray *volumeTypeControlArray = new DfxQControlArray();
	DfxQControlArray *voxelSamplingControlArray = new DfxQControlArray();
	DfxQControlArray *uniformSamplingControlArray = new DfxQControlArray();
	DfxQControlArray *sphereSamplingControlArray = new DfxQControlArray();

	std::string path = referencePath() + "/";

	// Type Controls
	QPushButton *sampleButton = new QPushButton("Sample");

	auto type = new DfxQComboBoxControl(path + "type", "", typeQControlenums, false);
	auto auto_sample = new DfxQCheckBoxControl(path + "auto_sample_mesh", "", false);
	auto object = new DfxQFluentPathControl(path + "object", "", false);

	///
	auto sufaceTypeSampler = new DfxQComboBoxControl(path + "surface_sampler", "", surfaceSamplingQControlenums, false);

	auto regular_radius = new DfxQDecimalSpinnerControl(path + "regular_radius", "", false);
	auto regular_use_resolution = new DfxQCheckBoxControl(path + "regular_use_resolution", "", false);
	auto regular_packed_resolution = new DfxQCheckBoxControl(path + "regular_packed_resolution", "", false);
	auto regular_apply_tranform = new DfxQCheckBoxControl(path + "regular_apply_transform", "", false);

	auto poisson_radius = new DfxQDecimalSpinnerControl(path + "poisson_radius", "", false);
	auto poisson_use_resolution = new DfxQCheckBoxControl(path + "poisson_use_resolution", "", false);
	auto poisson_packed_resolution = new DfxQCheckBoxControl(path + "poisson_packed_resolution", "", false);
	auto poisson_apply_tranform = new DfxQCheckBoxControl(path + "poisson_apply_transform", "", false);

	auto triangle_radius = new DfxQDecimalSpinnerControl(path + "triangle_radius", "", false);
	auto triangle_use_resolution = new DfxQCheckBoxControl(path + "triangle_use_resolution", "", false);
	auto triangle_apply_tranform = new DfxQCheckBoxControl(path + "triangle_apply_transform", "", false);

	///
	auto volumeTypeSampler = new DfxQComboBoxControl(path + "volume_sampler", "", volumeSamplingTypeQControlenums, false);

	auto voxel_radius = new DfxQDecimalSpinnerControl(path + "voxel_radius", "", false);
	auto voxel_use_resolution = new DfxQCheckBoxControl(path + "voxel_use_resolution", "", false);
	auto voxel_apply_tranform = new DfxQCheckBoxControl(path + "voxel_apply_transform", "", false);

	auto uniform_random_seed = new DfxQIntegerSpinnerControl(path + "uniform_random_seed", "", false);
	auto uniform_random_count = new DfxQIntegerSpinnerControl(path + "uniform_random_count", "", false);
	auto uniform_random_apply_tranform = new DfxQCheckBoxControl(path + "uniform_random_apply_transform", "", false);
	
	auto sphere_random_seed = new DfxQIntegerSpinnerControl(path + "sphere_random_seed", "", false);
	auto sphere_count = new DfxQIntegerSpinnerControl(path + "sphere_count", "", false);
	auto sphere_radius = new DfxQDecimalSpinnerControl(path + "sphere_radius", "", false);
	auto sphere_apply_tranform = new DfxQCheckBoxControl(path + "sphere_apply_transform", "", false);

	sampleButton->setStyleSheet(sampleButton->styleSheet() + style::leftButtonSty() + style::rightButtonSty() + style::hoverSty() + style::hoverPressdSty());

	// Main
	typeControlArray->addControl(sampleButton);
	typeControlArray->addSpacer(0, 2);
	typeControlArray->addControl("Type", type);
	typeControlArray->addSpacer(0, 2);
	typeControlArray->addControl("Object", object);
	typeControlArray->addSpacer(0, 2);
	typeControlArray->addControl("Auto Sample", auto_sample);

	typeControlPanel->setTitle("Type");
	typeControlPanel->setWidget(typeControlArray);

	// Type Stack
	typeStack->addPage(surfaceWidget);
	typeStack->addPage(volumeWidget);

	// Surface
	surfaceTypeControlArray->addControl("Sampler", sufaceTypeSampler);

	surfaceTypePanel->setTitle("Surface Sampling");
	surfaceTypePanel->setWidget(surfaceTypeControlArray);

	regularSamplingTypeControlArray->addControl("Radius/Resolution", regular_radius);
	regularSamplingTypeControlArray->addSpacer(0, 2);
	regularSamplingTypeControlArray->addControl("Use Resolution", regular_use_resolution);
	regularSamplingTypeControlArray->addSpacer(0, 2);
	regularSamplingTypeControlArray->addControl("Packed Resolution", regular_packed_resolution);
	regularSamplingTypeControlArray->addSpacer(0, 2);
	regularSamplingTypeControlArray->addControl("Apply Transform", regular_apply_tranform);

	poissonSamplingTypeControlArray->addControl("Radius/Resolution", poisson_radius);
	poissonSamplingTypeControlArray->addSpacer(0, 2);
	poissonSamplingTypeControlArray->addControl("Use Resolution", poisson_use_resolution);
	poissonSamplingTypeControlArray->addSpacer(0, 2);
	poissonSamplingTypeControlArray->addControl("Packed Resolution", poisson_packed_resolution);
	poissonSamplingTypeControlArray->addSpacer(0, 2);
	poissonSamplingTypeControlArray->addControl("Apply Transform", poisson_apply_tranform);

	triangleSamplingTypeControlArray->addControl("Radius/Resolution", triangle_radius);
	triangleSamplingTypeControlArray->addSpacer(0, 2);
	triangleSamplingTypeControlArray->addControl("Use Resolution", triangle_use_resolution);
	triangleSamplingTypeControlArray->addSpacer(0, 2);
	triangleSamplingTypeControlArray->addControl("Apply Transform", triangle_apply_tranform);

	surfaceStack->addPage(regularSamplingTypeControlArray);
	surfaceStack->addPage(poissonSamplingTypeControlArray);
	surfaceStack->addPage(triangleSamplingTypeControlArray);

	surface_type_layout->addWidget(surfaceTypePanel);
	surface_type_layout->addWidget(surfaceStack);

	surfaceWidget->setLayout(surface_type_layout);

	// Volume
	volumeTypeControlArray->addControl("Sampler", volumeTypeSampler);

	volumeTypePanel->setTitle("Volume Sampling");
	volumeTypePanel->setWidget(volumeTypeControlArray);

	voxelSamplingControlArray->addControl("Radius/Resolution", voxel_radius);
	voxelSamplingControlArray->addSpacer(0, 2);
	voxelSamplingControlArray->addControl("Use Resolution", voxel_use_resolution);
	voxelSamplingControlArray->addSpacer(0, 2);
	voxelSamplingControlArray->addControl("Apply Transform", voxel_apply_tranform);

	uniformSamplingControlArray->addControl("Random Seed", uniform_random_seed);
	uniformSamplingControlArray->addSpacer(0, 2);
	uniformSamplingControlArray->addControl("Count", uniform_random_count);
	uniformSamplingControlArray->addSpacer(0, 2);
	uniformSamplingControlArray->addControl("Apply Transform", uniform_random_apply_tranform);

	sphereSamplingControlArray->addControl("Random Seed", sphere_random_seed);
	sphereSamplingControlArray->addSpacer(0, 2);
	sphereSamplingControlArray->addControl("Count", sphere_count);
	sphereSamplingControlArray->addSpacer(0, 2);
	sphereSamplingControlArray->addControl("Radius", sphere_radius);
	sphereSamplingControlArray->addSpacer(0, 2);
	sphereSamplingControlArray->addControl("Apply Transform", sphere_apply_tranform);

	volumeStack->addPage(voxelSamplingControlArray);
	volumeStack->addPage(uniformSamplingControlArray);
	volumeStack->addPage(sphereSamplingControlArray);

	volume_type_layout->addWidget(volumeTypePanel);
	volume_type_layout->addWidget(volumeStack);

	volumeWidget->setLayout(volume_type_layout);

	// Main layout
	typeStack->changeCurrentPage(type->comboBox()->currentIndex());
	surfaceStack->changeCurrentPage(sufaceTypeSampler->comboBox()->currentIndex());
	volumeStack->changeCurrentPage(volumeTypeSampler->comboBox()->currentIndex());

	main_layout->addWidget(typeControlPanel);
	main_layout->addWidget(typeStack);

	widget->setLayout(main_layout);

	// Signal  & Slots
	QObject::connect(type->comboBox(), QOverload<int>::of(&QComboBox::activated), typeStack, &DfxQStackControl::changeCurrentPage);
	QObject::connect(sufaceTypeSampler->comboBox(), QOverload<int>::of(&QComboBox::activated), surfaceStack, &DfxQStackControl::changeCurrentPage);
	QObject::connect(volumeTypeSampler->comboBox(), QOverload<int>::of(&QComboBox::activated), volumeStack, &DfxQStackControl::changeCurrentPage);

	QObject::connect(sampleButton, &QPushButton::clicked, this, &DfxPointSamplerTool::sampleMesh);

	return widget;
}

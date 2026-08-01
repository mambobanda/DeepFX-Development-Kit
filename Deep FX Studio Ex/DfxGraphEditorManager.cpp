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

#include "DfxGraphEditorManager.h"
#include "DfxSceneSelectionModel.h"
#include "DfxPropertyConduit.h"
#include "DfxSpace.h"

DfxGraphEditorManager *DfxGraphEditorManager::_instance = nullptr;

DfxGraphEditorManager::DfxGraphEditorManager() {

	_instance = nullptr;
	_model = new DfxGraphEditorModel;
	_rootChannel = new DfxGraphEditorChannel;
	_baseChannel = new DfxGraphEditorChannel;

	_rootChannel->setType(DfxGraphEditorChannel::NONE);
	_rootChannel->setPath("_root_");
	
	_baseChannel->setType(DfxGraphEditorChannel::NONE);
	_baseChannel->setPath("_base_");
	_baseChannel->setName("Channels");

	_rootChannel->addChannel(_baseChannel);


	QObject::connect(DfxSceneSelectionModel::getInstance(), &DfxSceneSelectionModel::selectionChanged, this, &DfxGraphEditorManager::selectionChanged);

}

DfxGraphEditorManager::~DfxGraphEditorManager() {

}

DfxGraphEditorManager *DfxGraphEditorManager::getInstance() {
	if (_instance == nullptr) {
		_instance = new DfxGraphEditorManager;
	}

	return _instance;
}

DfxGraphEditorModel *DfxGraphEditorManager::model() {
	return _model;
}

DfxGraphEditorChannel *DfxGraphEditorManager::rootChannel() {
	return _rootChannel;
}

void DfxGraphEditorManager::addChannel(std::string path) {
	auto any = DfxSpace::getInstance()->findAbsolute(path);
	addChannel(any, nullptr);
}

void DfxGraphEditorManager::addChannel(fl::Any *any, DfxGraphEditorChannel *parent) {

	if (any != nullptr) {
		auto path = any->absolutePath();
		DfxGraphEditorChannel *channel = new DfxGraphEditorChannel;

		if (any->type() == fl::type::DATA_OBJECT) {
			auto dao = any->rawData<fl::DataObject>();

			channel->setName(any->label());
			channel->setPath(path);
			channel->setType(DfxGraphEditorChannel::DATA_OBJECT);

			for (auto prop : dao->propertiesOriginalOrder()) {
				addChannel(prop, channel);
			}

			if (channel->channelCount() == 0) {
				delete channel;
				channel = nullptr;
			}
		}
		else {
			if (DfxPropertyConduit::getInstance()->hasKeyframeOverride(path)) {
				channel->setName(any->label());
				channel->setPath(path);
				channel->setType(DfxGraphEditorChannel::ANY);
				channel->setColor(colors.next());
			}
			else {
				delete channel;
				channel = nullptr;
			}
		}

		if (channel != nullptr) {
			if (parent == nullptr) _baseChannel->addChannel(channel);
			else parent->addChannel(channel);
		}
	}

}

void DfxGraphEditorManager::removeChannel(std::string path) {

}

void DfxGraphEditorManager::selectionChanged() {
	
	_model->beginReset();
	_baseChannel->clear();
	colors.reset();
	if (!DfxSceneSelectionModel::getInstance()->selected().empty()) {

		for (auto node: DfxSceneSelectionModel::getInstance()->selected()) {
			addChannel(node->referencePath());
		}
	}

	_model->endReset();
	emit modelUpdated();
}

void DfxGraphEditorManager::notifyChannelUpdated() {
	emit channelUpdated();
}

void DfxGraphEditorManager::notifyNewChannelCreated() {
	emit newChannelCreated();
}
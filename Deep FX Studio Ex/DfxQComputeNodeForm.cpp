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

#include "DfxQComputeNodeForm.h"

DfxQComputeNodeForm::~DfxQComputeNodeForm() {

}

bool DfxQComputeNodeForm::create(DfxQComputeBaseNode *node) {

	if (node->type() != DfxQNodeEnums::COMPUTE_NODE) return false;

	auto space = DfxSpace::getInstance();
	auto any = space->findAbsolute(((DfxQComputeNode*)node)->dataObjectPath());

	if (any == nullptr) {
		DfxGlobal::console::println("Compute node properties don't exist.");
		return false;
	}

	if (any->type() != fl::type::DATA_OBJECT) {
		DfxGlobal::console::println("Compute Node Data missing.");
		return false;
	}

	if (any->rawData<fl::DataObject>()->mode() != fl::DataObject::NODE &&
		any->rawData<fl::DataObject>()->mode() != fl::DataObject::COMPOUND_NODE ) {
		DfxGlobal::console::println("Compute Node not found.");
		return false;
	}
	
	QVBoxLayout *main_layout = new QVBoxLayout;

	auto properties = new DfxControlPanel();

	DfxQControlArray *propertiesArray = new DfxQControlArray();

	main_layout->setContentsMargins(5, 15, 10, 0);
	main_layout->setAlignment(Qt::AlignTop);

	for (auto p : any->rawData<fl::DataObject>()->propertiesOriginalOrder()) {

		if (p->role() != fl::role::INPUT && p->role() != fl::role::HIDDEN_INPUT) continue;
		if (p->isArray()) continue;

		if (p->type() == fl::type::FLOAT) {

			auto prop = new DfxQDecimalSpinnerControl(p->absolutePath(), "", false);
			propertiesArray->addControl(p->label().c_str(), prop);
		}
		if (p->type() == fl::type::UNSIGNED_INT) {
			if (p->enumerations()) {
				auto prop = new DfxQComboBoxControl(p->absolutePath(), "", p->enumArray(), false);
				propertiesArray->addControl(p->label().c_str(), prop);
			}
		}
		else if (p->type() == fl::type::STD_STRING) {
			if (p->miscFlag() == -100) {
				auto prop = new DfxQFluentPathControl(p->absolutePath(), "", false);
				propertiesArray->addControl(p->label().c_str(), prop);
			}
			else if (p->miscFlag() == -200) {
				auto path = any->absolutePath() + "/";
				auto prop = new DfxQColorControl(path + p->colorReferences()[0], path + p->colorReferences()[1], path + p->colorReferences()[2], "", false);
				propertiesArray->addControl(p->label().c_str(), prop);
			}
			else if (p->miscFlag() == -300) {
				auto prop = new DfxQEditableComboBoxControl(p->absolutePath(), "",  DfxFluentAttributeDictionary::getInstance()->attributeList(), false);
				propertiesArray->addControl(p->label().c_str(), prop);
			}
			else {
				auto prop = new DfxQLineEditControl(p->absolutePath(), "", false);
				propertiesArray->addControl(p->label().c_str(), prop);
			}
		}
		else if (p->type() == fl::type::BOOL) {

			auto prop = new DfxQCheckBoxControl(p->absolutePath(), "", false);;
			propertiesArray->addControl(p->label().c_str(), prop);
		}
		else if (p->type() == fl::type::ATTRIBUTE_PATH) {

			auto prop = new DfxQAttributePathControl(p->absolutePath(), "", false);
			propertiesArray->addControl(p->label().c_str(), prop);
		}
		else if (p->type() == fl::type::FLOAT_3) {

			auto prop = new DfxQVectorControl(p->absolutePath(), "", false);
			propertiesArray->addControl(p->label().c_str(), prop);
		}
		
	}

	properties->setTitle(any->label().c_str());
	properties->setWidget(propertiesArray);

	main_layout->addWidget(properties);

	setLayout(main_layout);

	return true;
}

void DfxQComputeNodeForm::destroy() {

}
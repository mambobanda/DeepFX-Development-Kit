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
	* @file FlDataObject.cpp
	*
	* @author Mambo Banda
	*
	*/

#include "FlDataObject.h"

namespace fl {
	size_t DataObject::UNIQUE_ID_GEN = 0;

	const std::string DataObject::CONSTRUCTOR = "__CONSTRUCT__";
	const std::string DataObject::DESTRUCTOR = "__DESTRUCT__";
	const std::string DataObject::MAIN = "__MAIN__";
	const std::string DataObject::STATIC_MAIN = "__STATIC_MAIN__";

	DataObject::DataObject() {
		/// Generate  Unique ID
		_uid = UNIQUE_ID_GEN++;
		_mode = DATA;
		_defaultStaticMethod = nullptr;
		_defaultStaticDestructor = nullptr;
		_userdata = nullptr;
		_protected = false;
		_errorFlag = false;
		_hasModel  = false;
		_dynamicInputs = false;
		_functional = false;
		_space = nullptr;
		_iterationConstruct = IterationType::NONE;
		_errorString = "";
		_modelReference = "";
	}

	DataObject::~DataObject() {
		removeAndDeleteAll();
		if (_defaultStaticDestructor != nullptr) _defaultStaticDestructor(this);
	}

	size_t DataObject::uid() {
		return _uid;
	}

	Any *DataObject::value() {
		return &_value;
	}

	DataObject &DataObject::fromAny(Any *any) {
		return any->data<DataObject>();
	}

	bool DataObject::hasProperty(std::string name) {
		auto itr = _propertyMap.find(name);

		if (itr != _propertyMap.end()) return true;
		else return false;
	}

	Any *DataObject::property(std::string name) {

		auto itr = _propertyMap.find(name);

		if (itr != _propertyMap.end()) return itr->second;
		else return nullptr;
	}

	std::vector<Any*> DataObject::properties() {

		std::vector<Any*> values = util::unordered_map_values<std::string, Any*>(_propertyMap);
		return values;
	}

	std::vector<Any*> DataObject::propertiesOriginalOrder() {
		std::vector<Any*> ps;

		for (auto k : _propertyKeys) {
			auto itr = _propertyMap.find(k);

			if (itr != _propertyMap.end()) ps.push_back(itr->second);
		}

		return ps;
	}

	std::vector<std::string> &DataObject::propertyKeys() {
		return _propertyKeys;
	}

	void DataObject::setSpace(SpaceInterface *sp) {
		_space = sp;
	}

	void DataObject::addProperty(Any *any) {

		auto name = any->name();

		std::vector<std::string> keys = util::unordered_map_keys<std::string, Any*>(_propertyMap);
		name = util::makeStringUnique(name, keys);

		any->setName(name);

		_propertyMap[name] = any;
		_propertyKeys.push_back(name);
	
	}

	void DataObject::removeProperty(Any *any) {

		auto str = any->name();
		auto itr = _propertyMap.find(str);

		if (itr != _propertyMap.end()) {
			_propertyMap.erase(itr);
			any->setParent(nullptr);
		}

		auto _kpt = std::find(_propertyKeys.begin(), _propertyKeys.end(), str);
		if (_kpt != _propertyKeys.end()) _propertyKeys.erase(_kpt);
	}

	void  DataObject::removeAndDeleteAll() {

		if (_space != nullptr) {
			std::vector<Any*> _properties;
			for (auto &k : _propertyMap) {
				_properties.push_back(k.second);
			}

			for (auto p : _properties) {
				removeProperty(p);
				_space->remove(p);
			}
		}

		_propertyMap.clear();
		_propertyKeys.clear();
	}

	std::string DataObject::updatePropertyName(std::string oldname, std::string newname) {
		auto itr = _propertyMap.find(oldname);

		if (itr != _propertyMap.end()) {
			std::vector<std::string> keys = util::unordered_map_keys<std::string, Any*>(_propertyMap);
			newname = util::makeStringUnique(newname, keys);

			auto temp = itr->second;

			_propertyMap.erase(itr);

			newname = util::makeStringUnique(newname, keys);

			_propertyMap[newname] = temp;

			for (size_t inx = 0; inx < _propertyKeys.size(); inx++) {
				if (_propertyKeys[inx] == oldname) _propertyKeys[inx] = newname;
			}
		}

		return newname;
	}

	void DataObject::addMethod(std::string name, DataObjectMethod func) {
		_functionMap[name] = func;
	}

	void DataObject::removeMethod(std::string name) {
		auto itr = _functionMap.find(name);

		if (itr != _functionMap.end()) {
			_functionMap.erase(itr);
		}
	}

	void DataObject::addStaticMethod(std::string name, DataObjectStaticMethod func) {
		if (name == STATIC_MAIN) _defaultStaticMethod = func;
		else _staticFunctionMap[name] = func;
	}

	void DataObject::setStaticDestructor(DataObjectDestructor func) {
		_defaultStaticDestructor = func;
	}

	void DataObject::removeStaticMethod(std::string name) {
		auto itr = _staticFunctionMap.find(name);

		if (itr != _staticFunctionMap.end()) {
			_staticFunctionMap.erase(itr);
		}
	}

	bool DataObject::callMethod(std::string method_name, SpaceInterface *space, InterpreterScope *scope) {
		auto itr = _functionMap.find(method_name);

		if (itr != _functionMap.end()) {
			itr->second(space, this, scope);

			return true;
		}

		return false;
	}

	void *DataObject::userdata() {
		return _userdata;
	}

	void DataObject::setUserdata(void *usd) {
		_userdata = usd;
	}

	bool DataObject::callStaticMethod(std::string method_name, SpaceInterface *space, DataObject *dao, std::vector<Any*> input, std::vector<Any*> &output) {
		if (method_name == STATIC_MAIN) {
			_defaultStaticMethod(space, dao, input, output);
			return true;
		}
		else {
			auto itr = _staticFunctionMap.find(method_name);

			if (itr != _staticFunctionMap.end()) {
				itr->second(space, dao, input, output);
				return true;
			}
		}
		return false;
	}

	std::vector<std::pair<std::string, std::string>> &DataObject::inputReferences() {
		return _inputReferences;
	}

	std::vector<std::pair<std::string, std::string>> &DataObject::outputReferences() {
		return _outputReferences;
	}

	void DataObject::addInputReference(std::string path, std::string label) {
		std::pair<std::string, std::string> p(path, label);

		bool hasReference = false;

		for (auto p : _inputReferences) {
			if (p.first == path) hasReference = true;
		}

		if (!hasReference) _inputReferences.push_back(p);
	}

	void DataObject::removeInputReference(std::string path) {
		auto itr = _inputReferences.begin();

		while (itr != _inputReferences.end()) {

			if (itr->first == path) {
				_inputReferences.erase(itr);
				break;
			}
			itr++;
		}
	}

	void DataObject::addOutputReference(std::string path, std::string label) {
		std::pair<std::string, std::string> p(path, label);
		bool hasReference = false;

		for (auto p : _outputReferences) {
			if (p.first == path) hasReference = true;
		}

		if (!hasReference) _outputReferences.push_back(p);
	}

	void DataObject::removeOutputReference(std::string path) {
		auto itr = _outputReferences.begin();

		while (itr != _outputReferences.end()) {

			if (itr->first == path) {
				_outputReferences.erase(itr);
				break;
			}
			itr++;
		}
	}

	size_t DataObject::mode() {
		return _mode;
	}

	void DataObject::setMode(size_t md) {
		_mode = md;
	}

	bool DataObject::hasErrorFlag() {
		return _errorFlag;
	}

	void DataObject::setErrorFlag(bool errFlag) {
		_errorFlag = errFlag;
	}

	std::string DataObject::errorString() {
		return _errorString;
	}

	void DataObject::setErrorString(std::string errString) {
		_errorString = errString;
	}

	bool DataObject::isProtected() {
		return _protected;
	}

	void DataObject::setProtected(bool bl) {
		_protected = bl;
	}

	bool DataObject::hasModel() {
		return _hasModel;
	}

	void DataObject::setHasModel(bool val) {
		_hasModel = val;
	}

	std::string DataObject::modelReference() {
		return _modelReference;
	}

	void DataObject::setModelReference(std::string ref) {
		_modelReference = ref;
	}

	bool DataObject::allowDynamicInputs() {
		return _dynamicInputs;
	}

	void DataObject::setAllowDynamicInputs(bool _dc) {
		_dynamicInputs = _dc;
	}

	bool DataObject::isFunctional() {
		return _functional;
	}

	void DataObject::setIsFunctional(bool func) {
		_functional = func;
	}

	bool DataObject::isDataMode() {
		return _mode == DATA;
	}

	bool DataObject::isNodeMode() {
		return _mode == NODE;
	}

	bool DataObject::isCompoundNodeMode() {
		return _mode == COMPOUND_NODE;
	}

	size_t DataObject::iterationType() {
		return _iterationConstruct;
	}

	void DataObject::setIterationType(size_t val) {
		_iterationConstruct = val;
	}

	DataObjectStaticMethod DataObject::defuaultStaticMethod() {
		return _defaultStaticMethod;
	}

	// DataObject builder
	DataObject::Builder::Builder() {
		name  = "none";
		label = "none";
		mode  = DataObject::DATA;
		destroy = true;
		_protected = false;
		_array     = false;
		_hasModel  = false;
		_functional = false;
		_allowDynamicInputs = false;
		_modelReference = "";
	}

	DataObject::Builder::~Builder() {
		/// If build() was not called, delete properties to prevent memory leak.
		if (destroy) {
			for (auto any : _properties) {
				delete any;
			}
		}
	}

	DataObject::Builder &DataObject::Builder::setIsArray(bool arr) {
		_array = arr;
		return *this;
	}

	DataObject::Builder &DataObject::Builder::setName(std::string name) {
		this->name = name;

		/// Return instance of builder
		return *this;
	}

	DataObject::Builder &DataObject::Builder::setLabel(std::string label) {
		this->label = label;

		/// Return instance of builder
		return *this;
	}

	DataObject::Builder &DataObject::Builder::setMode(size_t t) {
		mode = t;

		return *this;
	}

	/// Set Protection Mode
	DataObject::Builder &DataObject::Builder::setProtected(bool bl){
		_protected = bl;

		/// Return instance of builder
		return *this;
	}

	/// Set Has Model
	DataObject::Builder &DataObject::Builder::setHasModel(bool bl) {
		_hasModel = bl;

		return *this;
	}

	/// Set Model Reference
	DataObject::Builder &DataObject::Builder::setModelReference(std::string ref) {
		_modelReference = ref;

		return *this;
	}

	DataObject::Builder &DataObject::Builder::setAllowDynamicInput(bool val) {
		_allowDynamicInputs  = val;

		return *this;
	}

	DataObject::Builder &DataObject::Builder::setIsFunctional(bool val) {
		_functional = val;

		return *this;
	}


	DataObject::Builder &DataObject::Builder::addFunction(std::string name, DataObjectMethod func) {

		std::pair<std::string, DataObjectMethod> p(name, func);
		_functions.push_back(p);

		/// Return instance of builder
		return *this;
	}

	DataObject::Builder &DataObject::Builder::addStaticFunction(std::string name, DataObjectStaticMethod func) {

		std::pair<std::string, DataObjectStaticMethod> p(name, func);
		_staticFunctions.push_back(p);

		/// Return instance of builder
		return *this;
	}

	Any *DataObject::Builder::build(SpaceInterface *space) {

		/// Create Instance and set Fl::Type
		auto doObj = space->any(name, label);
		doObj->setIsArray(_array);
		doObj->newData<DataObject>(type::DATA_OBJECT);
		doObj->rawData<DataObject>()->setMode(mode);
		doObj->rawData<DataObject>()->setProtected(_protected);
		doObj->rawData<DataObject>()->setHasModel(_hasModel);
		doObj->rawData<DataObject>()->setModelReference(_modelReference);
		doObj->rawData<DataObject>()->setAllowDynamicInputs(_allowDynamicInputs);
		doObj->rawData<DataObject>()->setIsFunctional(_functional);
		doObj->rawData<DataObject>()->setSpace(space);

		/// Add properties
		for (auto any : _properties) {
			space->move(any, doObj);
		}

		/// Add functions
		for (auto p : _functions) {
			doObj->data<DataObject>().addMethod(p.first, p.second);
		}

		/// Add static functions
		for (auto p : _staticFunctions) {
			doObj->data<DataObject>().addStaticMethod(p.first, p.second);
		}

		/// Impotant!! Don't destroy properties when done 
		destroy = false;

		/// Return built Fl::DataObject
		return doObj;
	}
}
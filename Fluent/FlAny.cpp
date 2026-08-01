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
	* @file FlAny.cpp
	*
	* @author Mambo Banda
	*
	*/

#include "FlDataObject.h"
#include "FlAny.h"
#include "FlObserver.h"
#include "FlDispatchManager.h"

namespace fl {

	size_t Any::ADDRESS_GEN = 0x3e8;

	Any::Any() {
		_space = nullptr;
		_ptr   = nullptr;
		_parent = nullptr;
		_explicitDelete = true;
		_allowAnyType = false;
		_array = false;
		_dynamic = false;
		_enumerations = false;
		_label = "none";
		_name = "property";
		_cardinalality = 1;
		_miscFlag = 0;
	}

	Any::Any(SpaceInterface *_sp) {
		/// Assign unique id to this instance
		_address = ADDRESS_GEN++;

		/// Assign default values to members
		_ptr = nullptr;
		_parent = nullptr;
		_ownership = 0;
		_explicitDelete = true;
		_enumerations = false;
		_dynamic = false;
		_label = "none";
		_name = "property";
		_space = _sp;
		_stampCheck = -1;
		_cardinalality = 1;
		_miscFlag = 0;
		stampValue();
	}

	Any::~Any() {
		for (auto ob : _observers) {
			ob->setParent(nullptr);
			if (!ob->deleteLater()) 
				delete ob;
		}
		_observers.clear();

		if (_explicitDelete) delete _ptr;
	}

	SpaceInterface *Any::space() {
		return _space;
	}

	size_t Any::address() {
		return _address;
	}

	Any *Any::parent() {
		return _parent;
	}

	void Any::forgetParent() {
		_parent = nullptr;
	}

	void Any::setParent(Any *pt) {
		_parent = pt;
	}

	uint32_t Any::type() {
		return _type;
	}

	void Any::setType(uint32_t ty) {
		_type = ty;
	}

	uint32_t Any::role() {
		return _role;
	}

	void Any::setRole(uint32_t rl) {
		_role = rl;
	}

	void Any::setVoidData(void *val) {
		_ptr = val;
	}

	void Any::incrementOwnership() {
		_ownership++;
	}

	void Any::decrementOwnership() {
		_ownership--;
	}

	uint32_t Any::ownership() {
		return _ownership;
	}

	bool Any::explicitDelete() {
		return _explicitDelete;
	}

	void Any::setExplicitDelete(bool val) {
		_explicitDelete = val;
	}

	std::string Any::name() {
		return _name;
	}

	void Any::setName(std::string nm) {
		std::string name = nm;

		if (name != _name) {

			if (_parent != nullptr) {
				if (_parent->type() == fl::type::DATA_OBJECT) {
					name = _parent->rawData<DataObject>()->updatePropertyName(_name, name);
				}
			}

			_name = name;
		}
	}

	std::string Any::label() {
		return _label;
	}

	bool Any::isInsync() {
		return (_stamp == _stampCheck);
	}

	void Any::sync() {
		_stampCheck = _stamp;
	}

	void Any::unsync() {
		_stampCheck = -1;
	}

	void Any::setLabel(std::string lb) {
		_label = lb;
	}

	int Any::stamp() {
		return _stamp;
	}

	/// Is Array
	bool Any::isArray() {
		return _array;
	}

	/// Set Is Array
	void Any::setIsArray(bool arr) {
		_array = arr;
	}

	void Any::stampValue() {
		bool done = false;

		/// Assign a random number to stamp ( 0 <), and make sure
		/// it is different from the last one.
		while (!done) {
			done = true;

			auto v = _rand();
			if (v == _stamp) done = false;
			else _stamp = v;
		}
	}

	/// Return Observers
	std::vector<Observer*> Any::observers() {
		return _observers;
	}

	/// Add Observer
	void Any::addObserver(Observer* obs) {
		obs->setParent(this);
		_observers.push_back(obs);
	}

	/// Remove Observer
	void Any::removeObserver(Observer* obs) {
		auto itr = _observers.begin();

		while (itr != _observers.end()) {
			if ((*itr)->id() == obs->id()) {
				itr = _observers.erase(itr);
				break;
			}
			itr++;
		}
	}

	/// Notify Observers
	void Any::notifyObservers(Observer *source) {

		for (auto ob : _observers) {

			if (source != nullptr) {
				if (source->id() == ob->id()) continue;
			}

			ob->update();
		}
	}

	/// Notify Observers
	void Any::notifyObservers(std::vector<Observer*> source) {
		for (auto ob : _observers) {
			bool skip = false;

			for (size_t inx = 0; inx < source.size(); inx++) {
				if (source[inx] != nullptr) {
					if (source[inx]->id() == ob->id()) skip = true;
				}
			}

			if (!skip) {
				ob->update();
			}
		}
	}

	std::string Any::absolutePath() {
		if (_space != nullptr) {
			return _space->absolutePath(this);
		}
		else {
			return "";
		}
	}

	int Any::cardinalality() {
		return _cardinalality;
	}

	void Any::setCardinalality(int card) {
		_cardinalality = card;
	}

	void Any::addDispatcher(size_t id) {
		_dispatchers.push_back(id);
	}

	void Any::removeDispatcher(size_t id) {
		auto itr = _dispatchers.begin();

		while (itr != _dispatchers.end()) {

			if ((*itr) == id) {
				itr = _dispatchers.erase(itr);
			}

			if(itr != _dispatchers.end())itr++;
		}
	}

	void Any::invokeDispatchers() {
		std::vector<size_t> loose;

		for (auto d : _dispatchers) {
			if (DispatchManager::getInstance()->hasDispatcher(d)) {
				DispatchManager::getInstance()->invokeDispatcher(d, this);
			}
			else {
				loose.push_back(d);
			}
		}

		for (auto ls : loose) removeDispatcher(ls);
	}

	void *Any::voidData() {
		return _ptr;
	}

	bool Any::isDynamic() {
		return _dynamic;
	}

	void Any::setIsDynamic(bool dyn) {
		_dynamic = dyn;
	}

	void Any::nullify() {
		_ptr = nullptr;
	}

	bool Any::isNull() {
		return (_ptr == nullptr);
	}

	bool Any::allowAnyType() {
		return _allowAnyType;
	}

	void Any::setAllowAnyType(bool val) {
		_allowAnyType = val;
	}

	int Any::miscFlag() {
		return _miscFlag;
	}

	void Any::setMiscFlag(int mf) {
		_miscFlag = mf;
	}

	bool Any::enumerations() {
		return _enumerations;
	}

	void Any::setEnumerations(bool bl) {
		_enumerations = bl;
	}

	void Any::setEnumArray(std::vector<std::pair<std::string, unsigned int>> &arr) {
		_enumArray = arr;
	}

	void Any::setStringEnumArray(std::vector<std::string> &arr) {
		_stringEnumArray = arr;
	}

	std::vector<std::string> &Any::stringEnumArray() {
		return _stringEnumArray;
	}

	std::vector<std::pair<std::string, unsigned int>> &Any::enumArray() {
		return _enumArray;
	}

	std::vector <std::string> &Any::colorReferences() {
		return _colorRef;
	}

	void Any::addColorReferences(std::string red, std::string green, std::string blue) {
		_colorRef.clear();
		_colorRef.push_back(red);
		_colorRef.push_back(green);
		_colorRef.push_back(blue);
	}
}
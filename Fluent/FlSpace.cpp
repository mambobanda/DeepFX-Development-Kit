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

#include "FlSpace.h"
#include "FlSpaceObserver.h"


namespace fl {

	std::mutex Space::_spaceDataLock;

	Space::Space() {
		initSpace();
	}

	Space::~Space() {
		delete _root;
	}

	Any *Space::root() {
		return _root;
	}

	Any *Space::any(std::string name, std::string label) {

		auto any = new Any(this);
		any->setName(name);
		any->setLabel(label);
		any->setType(type::NONE);
		any->setRole(role::NONE);

		_root->rawData<DataObject>()->addProperty(any);
		any->setParent(_root);

		_pathCache[any->absolutePath()] = any;

		return any;
	}

	void Space::remove(Any *any, SpaceObserver *source) {

		SpaceEventInfo infoPre;
		SpaceEventInfo infoPost;

		auto apath = any->absolutePath();
		for (auto rmCallback : _removeCallback) {
			rmCallback(any);
		}

		infoPre.type = fl::SpaceEventType::PRE_REMOVE_EVENT;
		infoPre.remove_source = any;
		
		infoPost.type = fl::SpaceEventType::POST_REMOVE_EVENT;
		infoPost.remove_source = any;

		auto pt = any->parent();

		notifyObservers(infoPre, source);

		_pathCache.erase(any->absolutePath());

		if (any->type() == type::DATA_OBJECT) {

			auto props = any->rawData<DataObject>()->properties();

			for (auto p : props) remove(p);
		}

		if (pt != nullptr) {
			if (pt->type() == type::DATA_OBJECT) {
				pt->rawData<DataObject>()->removeProperty(any);
			}
		}
		
		notifyObservers(infoPost, source);

		if (any->type() == type::DATA_OBJECT) {
			auto raw = any->rawData<DataObject>();
			delete raw;
			any->nullify();
		}
		else {
			any->deleteData();
		}

		delete any;	
	}

	int Space::addRemoveCallback(SpaceRemoveAnyCallBack cb) {
		_removeCallback.push_back(cb);
		return (int)_removeCallback.size() - 1;
	}

	void Space::removeRemoveCallback(size_t inx) {
		_removeCallback.erase(_removeCallback.begin() + inx);
	}

	Any *Space::findRelative(Any *any, std::string path)  {

		if (any->parent() == nullptr) return nullptr;

		util::StringSplitter splitter(path);
		std::string first = splitter.first();
		std::string next = splitter.next();
		bool splittable = splitter.isSplittable();
		auto count = splitter.count();
		bool matchLabel = false;

		if (first.empty()) return nullptr;

		if (first.size() > 2) {
			if (first[0] == '{') {
				if (first[first.size() - 1] == '}') {
					first.erase(first.begin());
					first.erase(first.begin() + (first.size() - 1));
					matchLabel = true;
				}
			}
		}

		if (first == "~") {
			if (!_root->rawData<DataObject>()->properties().empty()) {
				return findRelative(_root->rawData<DataObject>()->properties()[0], next);
			}
		}
		else if (first == "..") {
			return findRelative(any->parent(), next);
		}
		else {
			if (!splittable) {
				for (auto a : any->parent()->rawData<DataObject>()->properties()) {

					if (!matchLabel) {
						if (a->name() == first)
							return a;
					}
					else{
						if (a->label() == first)
							return a;
					}
				}
			}
			else {
			
				auto tempDao = any->parent()->rawData<DataObject>();
				for (auto a : any->parent()->rawData<DataObject>()->properties()) {
					bool process = false;

					if (!matchLabel) {
						if (a->name() == first) process = true;
					}
					else {
						if (a->label() == first) process = true;
					}

					if (process) {
						if (a->type() == type::DATA_OBJECT) {
							if (!a->rawData<DataObject>()->properties().empty()) {
								return findRelative(a->rawData<DataObject>()->properties()[0], next);
							}
						}
					}
				}
			}
		}

		return nullptr;
	}

	Any *Space::findAbsolute(std::string path) {

		auto itr = _pathCache.find(path);

		if (itr != _pathCache.end()) {
			return itr->second;
		}

		util::StringSplitter splitter(path);
		std::string first = splitter.first();
		std::string next  = splitter.next();

		if (first != "~") return nullptr;
		if (_root->rawData<DataObject>()->properties().empty()) return nullptr;

		auto outAny = findRelative(_root->rawData<DataObject>()->properties()[0], next);

		return outAny;
	}

	void Space::move(Any *any, Any *dest, SpaceObserver *source) {

		std::vector<std::pair<std::string, std::string>> oldNewPaths;
		std::unordered_map<size_t, OldNewPathStruct> map;

		collectOldPaths(any, map);
		if (dest->type() == type::DATA_OBJECT) {
			SpaceEventInfo infoPre;
			SpaceEventInfo infoPost;

			infoPre.type = fl::SpaceEventType::PRE_MOVE_EVENT;
			infoPre.move_source = any;
			infoPre.move_destination = dest;

			infoPost.type = fl::SpaceEventType::POST_MOVE_EVENT;
			infoPost.remove_source = any;
			infoPost.move_source = any;
			infoPost.move_destination = dest;

			notifyObservers(infoPre, source);

			_pathCache.erase(any->absolutePath());
			any->parent()->rawData<DataObject>()->removeProperty(any);
			dest->rawData<DataObject>()->addProperty(any);
			any->setParent(dest);
			_pathCache[any->absolutePath()] = any;

			collectNewPaths(any, map);
			updateBindings(map);

			notifyObservers(infoPost, source);
		}

	}

	DataObjectProperty Space::dataObjectProperty(DataObject *dataObject, std::string name) {

		auto prop = dataObject->property(name);

		if (prop != nullptr) {

			std::string input_path = absolutePath(prop);
			if (hasBinding(input_path)) {
				std::vector<Any*> links;

				for (auto lk : findLinks(input_path)) {
					auto out = findAbsolute(lk.output_path);

					if (out != nullptr) links.push_back(out);
				}

				return DataObjectProperty{ links, true };
			}
			else {
				return DataObjectProperty{ std::vector<Any*> {prop}, false };
			}
		}

		return DataObjectProperty{ std::vector<Any*> {}, false };
	}

	std::string Space::absolutePath(Any *any) {
		if (any->parent() == nullptr) return any->name();
		else return absolutePath(any->parent()) + "/" + any->name();
	}

	void Space::addBinding(std::string output, std::string output_node, std::string input, std::string input_node, int stamp) {

		auto itr = _bindingMap.find(input);

		if (itr != _bindingMap.end()) {

			if (!itr->second.hasLink(output)) {
				itr->second.addLink(output, output_node, input, input_node);
			}
		}
		else {
			Binding binding;
			binding.addLink(output, output_node, input, input_node);

			_bindingMap[input] = binding;
		}

	}

	void Space::removeBinding(std::string output, std::string input) {

		auto itr = _bindingMap.find(input);

		if (itr != _bindingMap.end()) {

			if (itr->second.hasLink(output)) {
				itr->second.removeLink(output);
			}

			if (itr->second.links().empty()) {
				_bindingMap.erase(itr);
			}
		}

	}

	void Space::removeAllBindings() {
		_bindingMap.clear();
	}

	std::unordered_map<std::string, Binding> &Space::bindingMap() {
		return _bindingMap;
	}

	bool Space::hasBinding(std::string input) {
		auto itr = _bindingMap.find(input);

		if (itr != _bindingMap.end()) return true;
		else return false;
	}

	bool Space::hasBindings(Any *any) {
		if (any->type() == fl::type::DATA_OBJECT) {

			for (auto a : any->rawData<DataObject>()->properties()) {
				if (hasBinding(absolutePath(a))) {
					return true;
				}
			}
		}

		return false;
	}

	bool Space::linkExits(std::string output, std::string input) {
		auto itr = _bindingMap.find(input);

		if (itr != _bindingMap.end()) {

			if (itr->second.hasLink(output)) {
				return true;
			}
		}

		return false;
	}

	Link Space::findLink(std::string output, std::string input) {
		auto itr = _bindingMap.find(input);

		if (itr != _bindingMap.end()) {

			if (itr->second.hasLink(output)) {

				auto out = findAbsolute(output);

				if (out != nullptr) {
					return itr->second.link(output);
				}
			}
		}

		return Link();
	}

	std::vector<Link> Space::findLinks(std::string input) {
		auto itr = _bindingMap.find(input);

		if (itr != _bindingMap.end()) {
			return itr->second.links();
		}

		return std::vector<Link>();
	}

	std::vector<Link> Space::findAssociatedLinks(std::string output) {
		std::vector<Link> outputs;

		for (auto p : _bindingMap) {
			for (auto lnk : p.second.links()) {
				if (lnk.output_path == output) {
					outputs.push_back(lnk);
				}
			}
		}

		return outputs;
	}

	void Space::syncBindings(std::string output, std::string input) {
		auto itr = _bindingMap.find(input);

		if (itr != _bindingMap.end()) {

			if (itr->second.hasLink(output)) {

				auto out = findAbsolute(output);

				if (out != nullptr) {
					itr->second.updateStamp(output, out->stamp());
				}
			}

		}
	}

	bool Space::isBindingSynced(std::string output, std::string input) {
		auto itr = _bindingMap.find(input);

		if (itr != _bindingMap.end()) {

			if (itr->second.hasLink(output)) {

				auto out = findAbsolute(output);

				if (out != nullptr) {
					return out->stamp() == itr->second.linkStamp(output);
				}
			}

		}

		return false;
	}

	bool Space::hasSyncedBindings(Any *any) {

		if (hasBindings(any)) {
			if (any->type() == fl::type::DATA_OBJECT) {

				for (auto a : any->rawData<DataObject>()->properties()) {
					if (hasBinding(absolutePath(a))) {

						for (auto lk : findLinks(absolutePath(a))) {
							if (!isBindingSynced(lk.output_path, lk.input_path)) return false;
						}
					}
				}
			}
		}
		else {
			return false;
		}

		return true;
	}

	bool Space::hasSyncedProperties(Any *any) {

		if (any->type() == fl::type::DATA_OBJECT) {

			for (auto p : any->rawData<DataObject>()->properties()) {

				if (!p->isInsync())
					return false;
			}
		}
		else {
			return false;
		}

		return true;
	}

	bool Space::isBindingOutputParentSynced(std::string output) {
		auto *any = findAbsolute(output);

		if (any != nullptr) {
			if (any->parent() != nullptr) {

				if (any->parent()->type() == fl::type::DATA_OBJECT) {
					return isDataObjectSynced(any->parent());
				}
			}
		}

		return false;
	}

	bool Space::hasSyncedOuputParents(Any *any) {

		if (any->type() == fl::type::DATA_OBJECT) {

			for (auto p : any->rawData<DataObject>()->properties()) {

				for (auto lks : findLinks(absolutePath(p))) {
					if (!isBindingOutputParentSynced(lks.output_path)) {
						return false;
					}
				}
			}
		}
		else {
			return false;
		}

		return true;
	}

	bool Space::isDataObjectSynced(Any *any) {

		if (any->type() == fl::type::DATA_OBJECT) {
			bool isSynced = true;

			for (auto prop : any->rawData<DataObject>()->properties()) {

				if (hasBinding(absolutePath(prop))) {

					for (auto lks : findLinks(absolutePath(prop))) {
						if (!isBindingSynced(lks.output_path, lks.input_path))
							isSynced = false;
						break;
					}
				}
				else {
					if (!prop->isInsync()) {
						isSynced = false;
						break;
					}
				}
			}

			return isSynced;
		}

		return false;
	}

	void Space::syncDataObject(DataObject *dataObject) {
		for (auto p : dataObject->properties()) {

			auto path = absolutePath(p);
			if (hasBinding(path)) {

				for (auto link : findLinks(path)) {
					syncBindings(link.output_path, link.input_path);
				}
			}
			else if (!p->isInsync()) {
				p->sync();
			}
		}
	}

	std::string Space::parentPath(std::string path) {
		std::size_t found = path.find_last_of("/");
		return path.substr(0, found);
	}

	void Space::explore(Any *any, std::vector<std::string> methods, SpaceMessage msg, bool enforceBinding) {
		std::queue<Any*> que;

		if (any->type() == fl::type::DATA_OBJECT) {
			que.push(any);

			while (!que.empty()) {
				auto anyFront = que.front();
				que.pop();

				auto dObject = anyFront->rawData<DataObject>();

				/*std::cout << "Name: " << anyFront->name() << std::endl;
				std::cout << "Has Bindings: " << (hasBindings(anyFront) ? "Yes" : "No") << std::endl;
				std::cout << "Bindings Synced: " << (hasSyncedBindings(anyFront) ? "Yes" : "No") << std::endl;
				std::cout << "Synced Parents: " << (hasSyncedOuputParents(anyFront) ? "Yes" : "No") << "\n\n";*/

				bool hasSyncedBindings = true;

				if (hasBindings(anyFront)) {
					if (!hasSyncedOuputParents(anyFront)) {
						que.push(anyFront);
						hasSyncedBindings = false;
					}
				}

				//if (hasSyncedBindings) {
				//	for (auto meth : methods) {
				//		//auto rc = dObject->callMethod(meth, this);

				//		if (msg != nullptr) {
				//			if (!rc) msg("Failed to run method.");
				//		}
				//	}
				//}

				for (auto p : dObject->properties()) {

					if (p->type() == fl::type::DATA_OBJECT) {
						que.push(p);
					}
				}

			}
		}
	}

	void Space::addObserver(SpaceObserver *obsv) {
		obsv->setParent(this);
		_spaceObservers.push_back(obsv);
	}

	void Space::removeObserver(SpaceObserver *obsv) {
		auto itr = _spaceObservers.begin();

		while (itr != _spaceObservers.end()) {
			if ((*itr)->id() == obsv->id()) {
				itr = _spaceObservers.erase(itr);
				break;
			}
			itr++;
		}
	}

	void Space::notifyObservers(SpaceEventInfo info, SpaceObserver *source) {
		for (auto ob : _spaceObservers) {

			if (source != nullptr) {
				if (source->id() == ob->id()) continue;
			}

			ob->update(info);
		}
	}

	void Space::collectOldPaths(Any* any, std::unordered_map<size_t, OldNewPathStruct> &map) {
		if (any->type() == type::DATA_OBJECT) {
			for (auto p : any->rawData<DataObject>()->properties()) {
				collectOldPaths(p, map);
			}
		}
		else {
			OldNewPathStruct st;
			st.id = any->address();
			st.old_path = any->absolutePath();

			map[any->address()] = st;
		}
	}

	void Space::collectNewPaths(Any* any, std::unordered_map<size_t, OldNewPathStruct> &map) {
		if (any->type() == type::DATA_OBJECT) {
			for (auto p : any->rawData<DataObject>()->properties()) {
				collectNewPaths(p, map);
			}
		}
		else {
			auto itr = map.find(any->address());

			if (itr != map.end()) {
				itr->second.new_path = any->absolutePath();
			}
		}
	}

	void Space::updateBindings(std::unordered_map<size_t, OldNewPathStruct> &map) {
		
		for (auto &bind : _bindingMap) {
			for (auto &mlink: bind.second.mapOflinks()) {
				for (auto strut : map) {

					if (strut.second.old_path == mlink.second.input_path) {
						mlink.second.input_path = strut.second.new_path;
					}

					if (strut.second.old_path == mlink.second.output_path) {
						mlink.second.output_path = strut.second.new_path;
					}
				}
			}
		}

		for (auto strut : map) {
			auto itr = _bindingMap.find(strut.second.old_path);

			if (itr != _bindingMap.end()) {
				auto bind = itr->second;

				_bindingMap.erase(itr);
				_bindingMap[strut.second.new_path] = bind;
			}
		}
	}

	void Space::initEssentials() {

		auto fluent = fl::DataObject::Builder()
			.setName("FLUENT")
			.setLabel("Fluent Essential Data")
			.build(this);

		auto flow = fl::DataObject::Builder()
			.setName("flow")
			.setLabel("Flow Essential Data")
			.build(this);

		auto decision = fl::DataObject::Builder()
			.setName("decisions")
			.setLabel("Decision Data Objects")
			.build(this);

		move(flow, fluent);
		move(decision, flow);
	}

	void  Space::initSpace() {
		_root = new Any(this);
		_root->newData<DataObject>(type::DATA_OBJECT);
		_root->setName("~");
		_root->setLabel("root");
		_root->setParent(nullptr);
		_root->setRole(role::NONE);

		initEssentials();
	}

	void  Space::reset() {
		delete _root;
		_root = nullptr;

		initSpace();
	}
}
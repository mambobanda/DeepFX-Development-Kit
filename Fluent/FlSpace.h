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

#pragma once

#include <mutex>
#include "FlCommon.h"
#include "FlBinding.h"
#include "FlDataObject.h"
#include "FlDataObjectProperty.h"
#include "FlSpaceInterface.h"
#include "FlSpaceEventType.h"

namespace fl {

	class Space : public SpaceInterface {
	private:
		static std::mutex _spaceDataLock;

	private:
		Any *_root;
		std::unordered_map<std::string, Any*> _pathCache;
	private:
		std::unordered_map<std::string, Binding> _bindingMap;
		std::vector<SpaceObserver*> _spaceObservers;
		std::vector<SpaceRemoveAnyCallBack> _removeCallback;
	private:
		class OldNewPathStruct {
		public:
			size_t id;
			std::string old_path;
			std::string new_path;
		};

		void collectOldPaths(Any* any, std::unordered_map<size_t, OldNewPathStruct> &map);
		void collectNewPaths(Any* any, std::unordered_map<size_t, OldNewPathStruct> &map);
		void updateBindings(std::unordered_map<size_t, OldNewPathStruct> &map);
	public:

		Space();

		~Space();

		Any *root();

		Any *any(std::string name, std::string label) override;

		Any *findRelative(Any *any, std::string path) override;

		Any *findAbsolute(std::string path);

		static std::string parentPath(std::string path);

		void move(Any *any, Any *dest, SpaceObserver *source = nullptr) override;

		void remove(Any *any, SpaceObserver *source = nullptr) override;

		int addRemoveCallback(SpaceRemoveAnyCallBack cb);

		void removeRemoveCallback(size_t inx);

		DataObjectProperty dataObjectProperty(DataObject *dataObject, std::string name) override;

		std::string absolutePath(Any *any) override;

		void addBinding(std::string output, std::string output_node, std::string input, std::string input_node, int stamp = -1) override;

		void removeBinding(std::string output, std::string input) override;

		std::unordered_map<std::string, Binding> &bindingMap();

		virtual void removeAllBindings() override;

		bool hasBinding(std::string input) override;

		bool hasBindings(Any *any) override;

		void updateBindings(std::string oldPath, std::string newPath);

		bool linkExits(std::string output, std::string input) override;

		Link findLink(std::string output, std::string input) override;

		std::vector<Link> findLinks(std::string input) override;

		std::vector<Link> findAssociatedLinks(std::string output) override;

		void syncBindings(std::string output, std::string input) override;

		bool isBindingSynced(std::string output, std::string input) override;

		bool hasSyncedBindings(Any *any) override;

		bool hasSyncedProperties(Any *any) override;

		bool isBindingOutputParentSynced(std::string output) override;

		bool hasSyncedOuputParents(Any *any) override;

		bool isDataObjectSynced(Any *any) override;

		void syncDataObject(DataObject *dataObject) override;

		void explore(Any *any, std::vector<std::string> methods, SpaceMessage msg=nullptr, bool enforceBinding = true) override;
		
		void addObserver(SpaceObserver *obsv);

		void removeObserver(SpaceObserver *obsv);

		void notifyObservers(SpaceEventInfo info, SpaceObserver *source = nullptr);

		void initSpace();

		void initEssentials();

		void reset();
	};
}

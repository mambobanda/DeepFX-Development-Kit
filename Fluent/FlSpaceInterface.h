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
	* @file FlSpaceInterface.h
	*
	* @brief An interface for Fl::Space, should always be used instead of using
	*		 Fl::Space directly.
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include "FlCommon.h"

namespace fl {
	using SpaceMessage = void(const char*);///< Generic message function pointer for explore()
	class SpaceInterface {
	public:
		virtual ~SpaceInterface();

		virtual Any *root() = 0;

		virtual Any *any(std::string name, std::string label) = 0;

		virtual Any *findRelative(Any *any, std::string path) = 0;
		virtual Any *findAbsolute(std::string path) = 0;
		virtual std::string absolutePath(Any *any) = 0;

		virtual void move(Any *any, Any *dest, SpaceObserver *source = nullptr) = 0;

		virtual void remove(Any *any, SpaceObserver *source = nullptr) = 0;

		virtual DataObjectProperty dataObjectProperty(DataObject *dataObject, std::string name) = 0;
		virtual void syncDataObject(DataObject *dataObject) = 0;

		virtual void addBinding(std::string output, std::string output_node, std::string input, std::string input_node, int stamp = -1) = 0;
		virtual void removeBinding(std::string output, std::string input) = 0;
		virtual void removeAllBindings() = 0;
		virtual bool hasBinding(std::string input) = 0;
		virtual bool hasBindings(Any *any) = 0;
		virtual bool linkExits(std::string output, std::string input) = 0;
		virtual Link findLink(std::string output, std::string input) = 0;
		virtual std::vector<Link> findLinks(std::string input) = 0;
		virtual std::vector<Link> findAssociatedLinks(std::string output) = 0;

		virtual void syncBindings(std::string output, std::string input) = 0;
		virtual bool isBindingSynced(std::string output, std::string input) = 0;
		virtual bool hasSyncedBindings(Any *any) = 0;

		virtual bool isBindingOutputParentSynced(std::string output) = 0;
		virtual bool hasSyncedOuputParents(Any *any) = 0;
		virtual bool isDataObjectSynced(Any *any) = 0;

		virtual bool hasSyncedProperties(Any *any) = 0;

		virtual void explore(Any *any, std::vector<std::string> methods, SpaceMessage=nullptr, bool enforceBinding = true) = 0;

	};
}
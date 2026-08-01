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
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <unordered_map>
#include <string>
#include "FlCommon.h"

namespace fl {

	class DispatchManager {
	private:
		static DispatchManager *_instance;
		std::unordered_map<size_t, Dispatcher*> _dispatchMap;
		
		DispatchManager();
	public:
		
		virtual ~DispatchManager();

		static DispatchManager *getInstance();

		std::vector<Dispatcher*> dispatcherList();

		bool hasDispatcher(size_t uid);

		void addDispatcher(Dispatcher *);
		void removeDispatcher(size_t uid);
		void invokeDispatcher(size_t uid, Any *any);
	};
}
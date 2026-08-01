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
#include "FlModel.h"

namespace fl {

	class ModelDB {
	private:
		static ModelDB *_instance;
		std::unordered_map < std::string, fl::Model*> _map;

		ModelDB();
	public:
		
		virtual ~ModelDB();

		static ModelDB *getInstance();

		void addModel(std::string str, fl::Model*);
		void removeModel(std::string str);
		Any *fetchModel(std::string str, fl::Space *space);

	};
};
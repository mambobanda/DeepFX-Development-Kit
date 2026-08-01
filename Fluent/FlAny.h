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
   * @file FlAny.h
   *
   * @brief Fl::Any provides a generic container for any data type.
   *		This is the most basic element in the Fluent module.
   *        It is uniquely defined by its name. Fl::Any should not
   *		be initialized directly, instead a Fl::Space should be used
   *		because all Fl::Any instances should exist inside a space in order for the
   *		module to work properly.
   *      
   * @author Mambo Banda
   *
   */

#pragma once

#include "FlCommon.h"
#include "FlSpaceInterface.h"
#include "FlUtil.h"

namespace fl {

	class Any {
	private:
		static size_t ADDRESS_GEN; ///< Global number used to generate normal ids

	private:
		SpaceInterface *_space; ///< Reference to a space
		Any *_parent; ///< Parent of instance

		size_t _address; ///< Unique ID
		uint32_t _type;  ///< The type the data type represents (used for manual type checking)
		uint32_t _role;  ///< Role the type should play under a Fl::DataObject

		std::string _name; ///< Unique name (Unique under parent, not in global space)
		std::string _label; ///< Very shory description

		bool _allowAnyType;

		int _stamp; ///< Stamp used to syncronise data, useful for data binding
		int _stampCheck; ///< Should be equal to _stamp if data is synced 

		int _miscFlag;

		uint32_t _ownership; ///< Ownership counter
		bool _explicitDelete; ///< Delete _ptr when destructor called; 
		bool _dynamic;

		util::RandInt _rand; ///< Random generator for stamps

		std::vector<Observer*> _observers;
		std::vector<size_t> _dispatchers;

		std::vector <std::string> _colorRef;

		bool _array;

		int _cardinalality;

		bool _enumerations;
		std::vector<std::pair<std::string, unsigned int>> _enumArray;
		std::vector<std::string> _stringEnumArray;

	private:
		void *_ptr; ///< Generic data heap storage

	public:
		
		/// Default Constructor, use any independent of space
		Any();

		/// Contructor, requires a space to live in.
		Any(SpaceInterface *_sp);

		/// Destructor
		virtual ~Any();

		/// Return space pointer.
		SpaceInterface *space();

		size_t address();

		/// Return parent pointer.
		Any *parent();

		/// Set parent to nullptr
		void forgetParent();

		/// Set parent
		void setParent(Any *pt);

		/// Return logical type of container
		uint32_t type();

		/// Set logical type of container
		void setType(uint32_t ty);

		/// Get logical role of container
		uint32_t role();

		/// Set logical role of container
		void setRole(uint32_t rl);

		/// Set _ptr directly (Should only be used when needed)
		void setVoidData(void *val);

		/// Increase ownership count by 1
		void incrementOwnership();

		/// Decrease ownership count by 1
		void decrementOwnership();

		/// Return ownership value
		uint32_t ownership();

		/// Check if _ptr should be deleted when constructor is called
		bool explicitDelete();

		/// Set if _ptr should be deleted when constructor is called
		void setExplicitDelete(bool val);

		/// Get name
		std::string name();

		/// Set name
		void setName(std::string nm);

		/// Check if data is in sync
		bool isInsync();

		/// Sync data (stamp == stampCheck)
		void sync();

		/// Unsync data (stamp != stampCheck)
		void unsync();

		/// Set label
		void setLabel(std::string lb);

		/// Get label
		std::string label();

		/// Return Observers
		std::vector<Observer*> observers();

		/// Add Observer
		void addObserver(Observer*);

		/// Remove Observer
		void removeObserver(Observer*);

		/// Notify Observers
		void notifyObservers(Observer *source = nullptr);

		/// Notify Observers
		void notifyObservers(std::vector<Observer*> source);

		/// Absolute Path
		std::string absolutePath();

		/// Is Array
		bool isArray();

		/// Set Is Array
		void setIsArray(bool);

		/// How many binding allowed, must be enforced manually 
		/// with a check, binding function ignores this
		int cardinalality();

		/// Set Binding limit
		void setCardinalality(int );

		void addDispatcher(size_t);

		void removeDispatcher(size_t);

		void invokeDispatchers();

		void *voidData();

		bool isDynamic();

		void setIsDynamic(bool);

		void nullify();

		bool isNull();

		bool allowAnyType();

		void setAllowAnyType(bool val);

		int miscFlag();

		void setMiscFlag(int);

		bool enumerations();

		void setEnumerations(bool bl);

		std::vector<std::string> &stringEnumArray();

		void setStringEnumArray(std::vector<std::string> &arr);

		std::vector<std::pair<std::string, unsigned int>> &enumArray();

		void setEnumArray(std::vector<std::pair<std::string, unsigned int>> &arr);

		std::vector <std::string> &colorReferences();
		void addColorReferences(std::string red, std::string green, std::string blue);

		/**
		* @brief Initialize storage on heap of type <T>.
		* 
		* @param type The logical type the storage will hold 
		*/
		template<typename T>
		inline void newData(uint32_t type) {
			delete _ptr;
			T *t_val = new T;
			_ptr = t_val;
			_type = type;

			invokeDispatchers();
		}

		/**
		* @brief Initialize storage on heap of type <T>.
		*
		* @param type The logical type the storage will hold
		* @param data pointer to data
		*/
		template<typename T>
		inline void newData(uint32_t type, T* data) {
			delete _ptr;
			_ptr = data;
			_type = type;

			invokeDispatchers();
		}

		/**
		* @brief Store a value in _ptr of type <T>.
		*
		* @param val The value to copy to storage
		*/
		template<typename T>
		inline void setData(T val) {
			T *temp = (T*)_ptr;
			*temp = val;
			stampValue();
			invokeDispatchers();
		}

		/// Return a reference to the storage (_ptr) cast to type <T>
		template<typename T>
		inline T &data() {
			T *t_val = (T*)_ptr;
			return *t_val;
		}

		/// Return stamp
		int stamp();

		/// Return stamp value
		void stampValue();

		/// Return _ptr cast to type <T>
		template<typename T>
		inline T *rawData() {
			T *t_val = (T*)_ptr;
			return t_val;
		}

		/**
		* @brief Store a value in _ptr of type <T>.
		*
		*/
		inline void deleteData(bool isArray = false) {

			if (_ptr != nullptr) {
				if (!isArray)
					delete _ptr;
				else
					delete[] _ptr;

				_ptr = nullptr;
			}
		}
	};
}
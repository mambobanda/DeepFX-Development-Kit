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
	* @file FlDataObject.h
	*
	* @brief A Fl::DataObject is a functional generic container that behaves like a dynamic
	*		 class with basic functionality. It allows Spaces to form complex hierarchies.
	*		 It also allows dynamic encapsulation, by grouping related properties and methods.
	*		 Fl::DataObject::Builder is a utility builder class to initialize DataObjects, and group
	*		 properties and functions in a concise and readable manner,
	*
	* @author Mambo Banda
	*
	*/

#pragma once

#include <map>
#include <mutex>
#include "FlCommon.h"
#include "FlSpaceInterface.h"
#include "FlIterationConstructType.h"
#include "FlSpaceInterface.h"
#include "FlAny.h"

namespace fl {

	using DataObjectMethod = std::function<void(SpaceInterface *, DataObject*, InterpreterScope*)>; ///< Function pointer alias used for methods in DataObjects
	using DataObjectStaticMethod = void(*)(SpaceInterface *, DataObject*, std::vector<Any*>&, std::vector<Any*>&); ///< Function pointer alias used for static methods in DataObjects
	using DataObjectDestructor = std::function<void(DataObject*)>; ///< Function pointer alias used for methods in DataObjects

	class DataObject {
	private:
		static size_t UNIQUE_ID_GEN; ///< unique ID generator for DataObject instances.

	private:
		SpaceInterface *_space;
	private:
		size_t _uid; ///<  Unique ID
		size_t _mode;
		size_t _iterationConstruct;
		bool _dynamicInputs;
		bool _protected;
		bool _functional;
	private:
		Any _value;
	private:
		bool _errorFlag;
		std::string _errorString;
	private:
		bool _hasModel;
		std::string _modelReference;
	private:
		void *_userdata;
	private:

		std::vector<std::string> _propertyKeys;
		std::unordered_map<std::string, Any*> _propertyMap; ///< A map to properties held in Fl::DataObject
		std::unordered_map<std::string, DataObjectMethod> _functionMap; ///< A map of methods held in Fl::DataObject
		std::unordered_map<std::string, DataObjectStaticMethod> _staticFunctionMap; ///< A map of methods held in Fl::DataObject

		DataObjectStaticMethod _defaultStaticMethod;
		DataObjectDestructor _defaultStaticDestructor;
	private:
		std::vector<std::pair<std::string, std::string>> _inputReferences;
		std::vector<std::pair<std::string, std::string>> _outputReferences;
	public:
		static const std::string CONSTRUCTOR;
		static const std::string DESTRUCTOR;
		static const std::string MAIN;
		static const std::string STATIC_MAIN;
		static const std::string STATIC_DESTRUCTOR;

	public:
		enum { DATA=0, NODE=1, COMPOUND_NODE=2 };

		/// Forward Declaration of Builder
		class Builder;

		/// Default Constructor
		DataObject();
		
		/// Destructor
		virtual ~DataObject();

		/// Return Unique ID
		size_t uid();

		/// Return DataObject Value
		Any *value();

		/// unique Name Resolution
		std::string uniqueName(std::string name);

		/**
		* @brief Utility function - Return a Fl::DataObject
		*
		* @param any 
		*
		* @retval Fl::DataObject reference if Fl::Any holds Fl::DataObject
		*		  otherwise unknown outcome.
		*/
		static DataObject &fromAny(Any *any);

		/**
		* @brief Check if _propertyMap has a key / property
		*
		* @param name The name of the property
		*
		* @retval true  If property is found.
		*
		* @retval false If property is not found.
		*/
		bool hasProperty(std::string name);

		/**
		* @brief Get property 
		*
		* @param name The name of the property
		*
		* @retval Fl::Any* If property exists.
		*
		* @retval nullptr is property doesn't exist.
		*/
		Any* property(std::string name);

		/// Return an unrodered list or all properties held in Fl::DataObject
		std::vector<Any*> properties();

		/// Return a list of all properties held in Fl::DataObject in the order inserted
		std::vector<Any*> propertiesOriginalOrder();

		/// Return a list of all properties held in Fl::DataObject in the order inserted
		std::vector<std::string> &propertyKeys();

		/// Set Space
		void setSpace(SpaceInterface *_space);

		/// Add Property to Fl::DataObject
		void addProperty(Any *any);

		/// Remove Property from Fl::DataObject
		void removeProperty(Any *any);

		void removeAndDeleteAll();

		/// Add method to Fl::DataObject (method_name, function_ptr)
		void addMethod(std::string name, DataObjectMethod func);

		/// Remove method
		void removeMethod(std::string name);

		/// Add static method to Fl::DataObject (method_name, function_ptr)
		void addStaticMethod(std::string name, DataObjectStaticMethod func);

		DataObjectStaticMethod defuaultStaticMethod();

		/// Set static destructor to Fl::DataObject (method_name, function_ptr)
		void setStaticDestructor(DataObjectDestructor func);

		/// Remove static method
		void removeStaticMethod(std::string name);

		/// Update property name
		std::string updatePropertyName(std::string oldname, std::string newname);

		/// Call Method with reference to relevant space (Fl::Space instance Fl::DataObject exist in).
		bool callMethod(std::string method_name, SpaceInterface *space, InterpreterScope *scope);

		/// Call Method with reference to relevant space (Fl::Space instance Fl::DataObject exist in).
		bool callStaticMethod(std::string method_name, SpaceInterface *space, DataObject *dao, std::vector<Any*> input, std::vector<Any*> &output);

		std::vector<std::pair<std::string, std::string>> &inputReferences();
		std::vector<std::pair<std::string, std::string>> &outputReferences();

		void addInputReference(std::string path, std::string label);
		void removeInputReference(std::string path);

		void addOutputReference(std::string path, std::string label);
		void removeOutputReference(std::string path);

		bool hasErrorFlag();
		void setErrorFlag(bool);

		std::string errorString();
		void setErrorString(std::string);

		size_t mode();
		void setMode(size_t md);

		bool isProtected();
		void setProtected(bool);

		bool hasModel();
		void setHasModel(bool);
		
		std::string modelReference();
		void setModelReference(std::string);

		bool allowDynamicInputs();
		void setAllowDynamicInputs(bool );

		bool isFunctional();
		void setIsFunctional(bool);

		size_t iterationType();
		void setIterationType(size_t);

		bool isDataMode();
		bool isNodeMode();
		bool isCompoundNodeMode();

		void *userdata();
		void setUserdata(void *);
	};

	class DataObject::Builder {
	private:
		std::string name; ///< Name of Fl::DataObject to be created
		std::string label; ///< Label of Fl::DataObject to be created
		size_t mode; ///< What mode Fl::DataObject will operate in
		bool _protected;
		bool _array;
		bool _hasModel;
		bool _allowDynamicInputs;
		bool _functional;
		std::string _modelReference;
		std::vector<Any*> _properties; ///< Properties to be included in Fl::DataObject
		std::vector<std::pair<std::string, DataObjectMethod>> _functions; ///< Functions to be included in Fl::DataObject 
		std::vector<std::pair<std::string, DataObjectStaticMethod>> _staticFunctions; ///< Functions to be included in Fl::DataObject 

		bool destroy; ///< Destroy any loose pointers if Fl::DataObject is not created for what ever reason.
	public:

		/// Default Constructor
		Builder();
		
		/// Destructor
		~Builder();

		/// Set name of Fl::DataObject
		Builder &setName(std::string name);
		
		/// Set label of Fl::DataObject
		Builder &setLabel(std::string label);

		/// Set Mode of Fl::DataObject
		Builder &setMode(size_t t);

		/// Set Protection Mode
		Builder &setProtected(bool);

		/// Set Array Mode
		Builder &setIsArray(bool);

		/// Set Has Model
		Builder &setHasModel(bool);

		/// Set Model Reference
		Builder &setModelReference(std::string);

		/// Set Model Reference
		Builder &setAllowDynamicInput(bool);

		/// Set is Functional
		Builder &setIsFunctional(bool);

		/**
		* @brief Create and add Fl::Any to a Fl::DataObject
		*
		* @param space The space the Fl::Any should be created in
		*
		* @param type The logical type of the container (e.g Fl::Type)
		*
		* @param role The logical role of the container
		*
		* @param name Fl::Any name
		*
		* @param label Fl::Any label
		*
		* @param defaultValue Default value to store in container
		*
		* @retval Fl::Any* If property exists.
		*
		* @retval nullptr is property doesn't exist.
		*/
		template<typename T>
		Builder &addProperty(SpaceInterface *space, uint32_t type, uint32_t role, std::string name, std::string label, T defaultValue) {

			/// New Fl::Any instance in space
			auto any = space->any(name, label);
			
			/// Set values passed as arguments
			any->newData<T>(type);
			any->data<T>() = defaultValue;
			any->setRole(role);

			/// Add any to property list
			_properties.push_back(any);

			return *this;
		}

		/// Add function to method list
		Builder &addFunction(std::string name, DataObjectMethod func);

		/// Add static function to method list
		Builder &addStaticFunction(std::string name, DataObjectStaticMethod func);

		/// Final step build Fl::DataObject
		Any *build(SpaceInterface *space);
	};
}
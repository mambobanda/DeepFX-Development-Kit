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

#include "DfxEntityBasicPropertyTypes.h"


namespace DfxEntityPropertyTypes {

	UIDPropertyType::UIDPropertyType() {
		type_id = TRAIT_UID_TYPE;
	}

	void *UIDPropertyType::create() {
		return new UID();
	}

	AnyPropertyType::AnyPropertyType() {
		type_id = iANY_TYPE;
	}

	void *AnyPropertyType::create() {
		return new DfxAny();
	}

	glm::vec3 float3ToVec3(Float3 flt) {
		return glm::vec3(flt.x, flt.y, flt.z);
	}

	IntPropertyType::IntPropertyType() {
		type_id = iINT_TYPE;
	}

	void *IntPropertyType::create() {
		return new int(0);
	}

	IntArrayPropertyType::IntArrayPropertyType() {
		type_id = iINT_ARRAY_TYPE;
	}

	void *IntArrayPropertyType::create() {
		return new IntArray();
	}

	UnsignedIntPropertyType::UnsignedIntPropertyType() {
		type_id = iUNSIGNED_INT_TYPE;
	}

	void *UnsignedIntPropertyType::create() {
		return new unsigned int(0);
	}

	UnsignedIntArrayPropertyType::UnsignedIntArrayPropertyType() {
		type_id = iUNSIGNED_INT_ARRAY_TYPE;
	}

	void *UnsignedIntArrayPropertyType::create() {
		return new UnsignedIntArray();
	}

	Size_TPropertyType::Size_TPropertyType() {
		type_id = iSIZE_T_TYPE;
	}

	void *Size_TPropertyType::create(){
		return new size_t(0);
	}

	Size_TArrayPropertyType::Size_TArrayPropertyType() {
		type_id = iSIZE_T_ARRAY_TYPE;
	}

	void *Size_TArrayPropertyType::create() {
		return new SizeTArray();
	}

	FloatPropertyType::FloatPropertyType() {
		type_id = iFLOAT_TYPE;
	}

	void *FloatPropertyType::create() {
		return new float(0.0f);
	}

	FloatArrayPropertyType::FloatArrayPropertyType() {
		type_id = iFLOAT_ARRAY_TYPE;
	}

	void *FloatArrayPropertyType::create() {
		return new FloatArray();
	}

	Float3PropertyType::Float3PropertyType() {
		type_id = iFLOAT3_TYPE;
	}

	void *Float3PropertyType::create() {
		return new Float3;
	}

	Float3ArrayPropertyType::Float3ArrayPropertyType() {
		type_id = iFLOAT3_ARRAY_TYPE;
	}

	void *Float3ArrayPropertyType::create() {
		return new Float3Array();
	}

	DoublePropertyType::DoublePropertyType() {
		type_id = iDOUBLE_TYPE;
	}

	void *DoublePropertyType::create() {
		return new double(0.0);
	}

	DoubleArrayPropertyType::DoubleArrayPropertyType() {
		type_id = iDOUBLE_ARRAY_TYPE;
	}

	void *DoubleArrayPropertyType::create() {
		return new DoubleArray();
	}

	BoolPropertyType::BoolPropertyType() {
		type_id = iBOOL_TYPE;
	}

	void *BoolPropertyType::create() {
		return new bool(false);
	}

	BoolArrayPropertyType::BoolArrayPropertyType() {
		type_id = iBOOL_ARRAY_TYPE;
	}

	void *BoolArrayPropertyType::create() {
		return new BoolArray();
	}

	CharPropertyType::CharPropertyType() {
		type_id = iCHAR_TYPE;
	}

	void *CharPropertyType::create() {
		return new char(' ');
	}

	CharArrayPropertyType::CharArrayPropertyType() {
		type_id = iCHAR_ARRAY_TYPE;
	}

	void *CharArrayPropertyType::create() {
		return new CharArray();
	}

	StringPropertyType::StringPropertyType() {
		type_id = iSTRING_TYPE;
	}

	void *StringPropertyType::create() {
		return new std::string();
	}

	FilePathPropertyType::FilePathPropertyType() {
		type_id = iFILE_PATH_TYPE;
	}

	void *DirPathPropertyType::create() {
		return new std::string();
	}

	DirPathPropertyType::DirPathPropertyType() {
		type_id = iDIR_PATH_TYPE;
	}

	void *FilePathPropertyType::create() {
		return new std::string();
	}

	StringArrayPropertyType::StringArrayPropertyType() {
		type_id = iSTRING_ARRAY_TYPE;
	}

	void *StringArrayPropertyType::create() {
		return new StringArray();
	}

	Dfx3DMeshPropertyType::Dfx3DMeshPropertyType() {
		type_id = i3DMESH_TYPE;
	}

	void *Dfx3DMeshPropertyType::create() {
		return new Dfx3dMeshData();
	}

	DfxEnumPropertyType::DfxEnumPropertyType() {
		type_id = iENUM_TYPE;
	}

	void *DfxEnumPropertyType::create() {
		return new DfxEnum();
	}

	DfxMatrix3x3PropertyType::DfxMatrix3x3PropertyType() {
		type_id = iMATRIX3x3_TYPE;
	}

	void *DfxMatrix3x3PropertyType::create() {
		return new Matrix3x3();
	}

	DfxMatrix4x4PropertyType::DfxMatrix4x4PropertyType() {
		type_id = iMATRIX4x4_TYPE;
	}

	void *DfxMatrix4x4PropertyType::create() {
		return new Matrix4x4();
	}

	DfxMatrix4x4ArrayPropertyType::DfxMatrix4x4ArrayPropertyType() {
		type_id = iMATRIX4x4_ARRAY_TYPE;
	}

	void *DfxMatrix4x4ArrayPropertyType::create() {
		return new Matrix4x4Array();
	}

	ProxyObjectPropertyType::ProxyObjectPropertyType() {
		type_id = iPROXY_OBJECT_TYPE;
	}

	void *ProxyObjectPropertyType::create() {
		return new ProxyObject();
	}

	ProxyObjectArrayPropertyType::ProxyObjectArrayPropertyType() {
		type_id = iPROXY_OBJECT_ARRAY_TYPE;
	}

	void *ProxyObjectArrayPropertyType::create() {
		return new ProxyObjectArray();
	}
}
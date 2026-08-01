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

#include <string>
#include "DfxArray.h"
#include "DfxTraitUID.h"
#include "DfxEnum.h"
#include "DfxAny.h"
#include "Dfx3dMeshData.h"
#include "DfxObjectDisplacementMap.h"
#include "DfxEntityPropertyType.h"


namespace DfxEntityPropertyTypes {
	
	enum _DfxEntityPropertyTypes {
		   iINT_TYPE=11, iUNSIGNED_INT_TYPE=12, iSIZE_T_TYPE = 13, iFLOAT_TYPE = 14, iDOUBLE_TYPE = 15, iBOOL_TYPE = 16,
		   iCHAR_TYPE=17, iSTRING_TYPE=18, iFLOAT3_TYPE=19, i3DMESH_TYPE=20, iINT_ARRAY_TYPE=21, iFLOAT_ARRAY_TYPE = 22,
		   iSIZE_T_ARRAY_TYPE=23, iCHAR_ARRAY_TYPE=24, iSTRING_ARRAY_TYPE=25, iBOOL_ARRAY_TYPE=26, iFLOAT3_ARRAY_TYPE=27,
		   iUNSIGNED_INT_ARRAY_TYPE=28, iDOUBLE_ARRAY_TYPE=29, iENUM_TYPE=30, iMATRIX3x3_TYPE=31, iMATRIX4x4_TYPE =32,
		   iPROXY_OBJECT_TYPE=33, iPROXY_OBJECT_ARRAY_TYPE = 34, iMATRIX4x4_ARRAY_TYPE = 35, iFILE_PATH_TYPE = 36,
		   iDIR_PATH_TYPE = 37, TRAIT_UID_TYPE = 38, iANY_TYPE = 39,
		 };

	typedef struct st_matrix3x3 {
		float m[9];

		st_matrix3x3() {
			for (int i = 0; i < 9; i++) m[i] = 0.f;
		}

	}Matrix3x3;

	typedef struct st_matrix4x4 {
		float m[16];

		st_matrix4x4() {
			for (int i = 0; i < 9; i++) m[i] = 0.f;
		}

		void copy(st_matrix4x4 &mat) {
			m[0] = mat.m[0];
			m[1] = mat.m[1];
			m[2] = mat.m[2];
			m[3] = mat.m[3];

			m[4] = mat.m[4];
			m[5] = mat.m[5];
			m[6] = mat.m[6];
			m[7] = mat.m[7];

			m[8] = mat.m[8];
			m[9] = mat.m[9];
			m[10] = mat.m[10];
			m[11] = mat.m[11];

			m[12] = mat.m[12];
			m[13] = mat.m[13];
			m[14] = mat.m[14];
			m[15] = mat.m[15];
		}

		void copy(float *m) {
			m[0] = m[0];
			m[1] = m[1];
			m[2] = m[2];
			m[3] = m[3];

			m[4] = m[4];
			m[5] = m[5];
			m[6] = m[6];
			m[7] = m[7];

			m[8] = m[8];
			m[9] = m[9];
			m[10] = m[10];
			m[11] = m[11];

			m[12] = m[12];
			m[13] = m[13];
			m[14] = m[14];
			m[15] = m[15];
		}

	}Matrix4x4;

	typedef struct st_proxy_object {
		
		Float3 position;
		Float3 rotation;
		Float3 scale;

		Dfx3dMeshData meshData;

		DfxObjectDisplacementMap *displacementMap;

		st_proxy_object() {
			displacementMap = new DfxObjectDisplacementMap();
		}

		~st_proxy_object() {

			meshData.clearIndexBuffer();
			meshData.clearVertexBuffer();
			displacementMap->clear();

			delete displacementMap;
		}

	}ProxyObject;
	
	using UID = DfxTraitUID<size_t>;

	using IntArray = DfxArray<int>;
	using UnsignedIntArray = DfxArray<unsigned int>;
	using FloatArray  = DfxArray<float>;
	using DoubleArray = DfxArray<double>;
	using SizeTArray  = DfxArray<size_t>;
	using CharArray   = DfxArray<char>;
	using StringArray = DfxArray<std::string>;
	using BoolArray   = DfxArray<bool> ;
	using Float3Array = DfxArray<Float3>;
	using Matrix4x4Array = DfxArray<Matrix4x4>;
	using ProxyObjectArray = DfxArray<ProxyObject>;

	glm::vec3 float3ToVec3(Float3 flt);

	class UIDPropertyType : public DfxEntityPropertyType {
	public:
		UIDPropertyType();
		virtual void *create();
	};

	class AnyPropertyType : public DfxEntityPropertyType {
	public:
		AnyPropertyType();
		virtual void *create();
	};

	class IntPropertyType : public DfxEntityPropertyType {
	public:
		IntPropertyType();
		virtual void *create();
	};

	class IntArrayPropertyType : public DfxEntityPropertyType {
	public:
		IntArrayPropertyType();
		virtual void *create();
	};

	class UnsignedIntPropertyType : public DfxEntityPropertyType {
	public:
		UnsignedIntPropertyType();
		virtual void *create();
	};

	class UnsignedIntArrayPropertyType : public DfxEntityPropertyType {
	public:
		UnsignedIntArrayPropertyType();
		virtual void *create();
	};

	class Size_TPropertyType : public DfxEntityPropertyType {
	public:
		Size_TPropertyType();
		virtual void *create();
	};

	class Size_TArrayPropertyType : public DfxEntityPropertyType {
	public:
		Size_TArrayPropertyType();
		virtual void *create();
	};

	class FloatPropertyType : public DfxEntityPropertyType {
	public:
		FloatPropertyType();
		virtual void *create();
	};

	class FloatArrayPropertyType : public DfxEntityPropertyType {
	public:
		FloatArrayPropertyType();
		virtual void *create();
	};

	class Float3PropertyType : public DfxEntityPropertyType {
	public:
		Float3PropertyType();
		virtual void *create();
	};

	class Float3ArrayPropertyType : public DfxEntityPropertyType {
	public:
		Float3ArrayPropertyType();
		virtual void *create();
	};

	class DoublePropertyType : public DfxEntityPropertyType {
	public:
		DoublePropertyType();
		virtual void *create();
	};

	class DoubleArrayPropertyType : public DfxEntityPropertyType {
	public:
		DoubleArrayPropertyType();
		virtual void *create();
	};

	class BoolPropertyType : public DfxEntityPropertyType {
	public:
		BoolPropertyType();
		virtual void *create();
	};
	
	class BoolArrayPropertyType : public DfxEntityPropertyType {
	public:
		BoolArrayPropertyType();
		virtual void *create();
	};

	class CharPropertyType : public DfxEntityPropertyType {
	public:
		CharPropertyType();
		virtual void *create();
	};

	class CharArrayPropertyType : public DfxEntityPropertyType {
	public:
		CharArrayPropertyType();
		virtual void *create();
	};

	class StringPropertyType : public DfxEntityPropertyType {
	public:
		StringPropertyType();
		virtual void *create();
	};

	class StringArrayPropertyType : public DfxEntityPropertyType {
	public:
		StringArrayPropertyType();
		virtual void *create();
	};

	class FilePathPropertyType : public DfxEntityPropertyType {
	public:
		FilePathPropertyType();
		virtual void *create();
	};

	class DirPathPropertyType : public DfxEntityPropertyType {
	public:
		DirPathPropertyType();
		virtual void *create();
	};

	class Dfx3DMeshPropertyType : public DfxEntityPropertyType {
	public:
		Dfx3DMeshPropertyType();
		virtual void *create();
	};

	class DfxEnumPropertyType : public DfxEntityPropertyType {
	public:
		DfxEnumPropertyType();
		virtual void *create();
	};

	class DfxMatrix3x3PropertyType : public DfxEntityPropertyType {
	public:
		DfxMatrix3x3PropertyType();
		virtual void *create();
	};

	class DfxMatrix4x4PropertyType : public DfxEntityPropertyType {
	public:
		DfxMatrix4x4PropertyType();
		virtual void *create();
	};

	class DfxMatrix4x4ArrayPropertyType : public DfxEntityPropertyType {
	public:
		DfxMatrix4x4ArrayPropertyType();
		virtual void *create();
	};

	class ProxyObjectPropertyType : public DfxEntityPropertyType {
	public:
		ProxyObjectPropertyType();
		virtual void *create();
	};

	class ProxyObjectArrayPropertyType : public DfxEntityPropertyType {
	public:
		ProxyObjectArrayPropertyType();
		virtual void *create();
	};
}
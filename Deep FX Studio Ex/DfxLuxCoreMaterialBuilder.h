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

#include <luxcore/luxcore.h>
#include <DfxGlobal.h>
#include <DfxRenderTarget.h>
#include <unordered_map>
#include <glm/vec3.hpp>
#include <FlDirectedAcyclicGraph.h>
#include "DfxSpace.h"
#include "DfxLuxMaterialChangeDispatch.h"


class DfxLuxCoreMaterialBuilder {
private:
	class Entry {
	private:
		unsigned int _type;
		std::string _signature;
	public: 
		enum { NONE=0, FLOAT=1, FLOAT2 = 2, FLOAT3 = 3, INT = 4, INT2 = 5, INT3 = 6, REFERENCE = 7, STRING = 8, BOOL = 9 };

	public:

		std::string xref;
		std::string xstring;
		bool xbool;
		float xfloat;
		glm::vec3 xfloat3;
		glm::vec2 xfloat2;
		int xint;
		glm::ivec3 xint3;

	public:

		Entry(std::string sig, unsigned int type);
		luxrays::Property toProp();

		unsigned int type();

		std::string signature();
	};
private:
	size_t ID_GEN;
	luxcore::Scene *_scene;

	std::unordered_map<std::string, std::string> _matUniqueNameMap;
	std::unordered_map<std::string, std::string> _rootMaterialMap;
	std::unordered_map<std::string, std::string> _displacementMaterialMap;
	std::unordered_map<size_t, luxrays::Properties> _nodePropertyMap;
	fl::DirectedAcyclicGraph<size_t, fl::DataObject> _dag;
private:

	std::string entryToString(Entry);

	void registerMaterial(fl::Any *any);
	void setMaterialProperties(fl::Any *any);
	Entry resolveSlot(std::string signature, fl::DataObject  *obj, std::string name, std::string defaultStrVal = "");
	Entry resolveSlot(std::string signature, fl::DataObject *obj, std::string proxy, std::string a, std::string b, std::string c);

	void buildDAG(std::vector<fl::Any*> dao, fl::DirectedAcyclicGraph<size_t, fl::DataObject> *graph);
	void deleteDAG(fl::DirectedAcyclicGraph<size_t, fl::DataObject> *graph);

private:
	void setLuxNull(fl::Any *any);
	void setLuxMatte(fl::Any *any);
	void setLuxMatteTranslucent(fl::Any *any);
	void setLuxEmission(fl::Any *any);
	void setLuxDisney(fl::Any *any);
	void setLuxMix(fl::Any *any);
	void setLuxGlass(fl::Any *any);
	void setLuxArchGlass(fl::Any *any);
	void setLuxRoughGlass(fl::Any *any);
	void setLuxCarPaint(fl::Any *any);
	void setLuxCloth(fl::Any *any);
	void setLuxGlossy(fl::Any *any);
	void setLuxGlossyTranslucent(fl::Any *any);
	void setLuxMetal(fl::Any *any);
	void setLuxMirror(fl::Any *any);
	void setLuxVelvet(fl::Any *any);
	void setLuxRoughMatte(fl::Any *any);

	void setLuxImagemap(fl::Any *any);
	void setLuxNormalmap(fl::Any *any);
	void setLuxFBM(fl::Any *any);
	void setLux2dCheckerBoard(fl::Any *any);
	void setLuxBlend(fl::Any *any);
	void setLuxCloud(fl::Any *any);
	void setLuxDistortedNoise(fl::Any *any);
	void setLuxMagic(fl::Any *any);
	void setLuxMarble(fl::Any *any);
	void setLuxMusgrave(fl::Any *any);
	void setLuxNoise(fl::Any *any);
	void setLuxStucci(fl::Any *any);
	void setLuxWood(fl::Any *any);
	void setLuxVoronoi(fl::Any *any);
	void setLuxFresnel01(fl::Any *any);

	void setLuxFresnelColor(fl::Any *any);
	void setLuxFresnelSopra(fl::Any *any);

	void setLuxMixTex(fl::Any *any);
	void setLuxClamp(fl::Any *any);
	void setLuxHsv(fl::Any *any);


public:

	DfxLuxCoreMaterialBuilder();
	virtual ~DfxLuxCoreMaterialBuilder();

	void build(luxcore::Scene *);
	std::string material(std::string path);
	std::string displacement(std::string path);

	static DfxLuxMaterialChangeDispatch *createMatrialDispatcher();
};
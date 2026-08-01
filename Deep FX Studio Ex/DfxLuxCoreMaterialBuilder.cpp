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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxLuxCoreMaterialBuilder.h"


DfxLuxCoreMaterialBuilder::Entry::Entry(std::string sig, unsigned int type) {
	_signature = sig;
	_type = type;
}

luxrays::Property DfxLuxCoreMaterialBuilder::Entry::toProp() {

	if (_type == REFERENCE) {
		return luxrays::Property(_signature, xref);
	}
	else if (_type == STRING) {
		return luxrays::Property(_signature, xstring);
	}
	else if (_type == BOOL) {
		return luxrays::Property(_signature, xbool);
	}
	else if (_type == FLOAT) {
		return luxrays::Property(_signature, xfloat);
	}
	else if (_type == FLOAT2) {
		luxrays::PropertyValues vals;
		vals.push_back(xfloat2.x);
		vals.push_back(xfloat2.y);
		return luxrays::Property(_signature, vals);
	}
	else if (_type == FLOAT3) {
		luxrays::PropertyValues vals;
		vals.push_back(xfloat3.x);
		vals.push_back(xfloat3.y);
		vals.push_back(xfloat3.z);
		return luxrays::Property(_signature, vals);
	}
	else if (_type == INT) {
		return luxrays::Property(_signature, xint);
	}
	else if (_type == INT3) {
		luxrays::PropertyValues vals;
		vals.push_back(xint3.x);
		vals.push_back(xint3.y);
		vals.push_back(xint3.z);
		return luxrays::Property(_signature, vals);
	}
	else {
		return luxrays::Property();
	}
}

unsigned int DfxLuxCoreMaterialBuilder::Entry::type() {
	return _type;
}

std::string DfxLuxCoreMaterialBuilder::Entry::signature() {
	return _signature;
}

DfxLuxCoreMaterialBuilder::DfxLuxCoreMaterialBuilder() {
	ID_GEN = 0xffff;
}

DfxLuxCoreMaterialBuilder::~DfxLuxCoreMaterialBuilder() {
	deleteDAG(&_dag);
}

void DfxLuxCoreMaterialBuilder::buildDAG(std::vector<fl::Any*> dao, fl::DirectedAcyclicGraph<size_t, fl::DataObject> *graph) {

	std::unordered_map<size_t, fl::DirectedAcyclicGraph<size_t, fl::DataObject>::Vertex*> vertices;

	auto _space = DfxSpace::getInstance();

	for (auto any : dao) {
		
		auto node = any->rawData<fl::DataObject>();

		auto vertex = new fl::DirectedAcyclicGraph<size_t, fl::DataObject>::Vertex();
		vertex->id = node->uid();
		vertex->description = any->absolutePath();
		vertex->userData = node;

		vertices[vertex->id] = vertex;
		graph->addVertex(vertex);
		
	}

	for (auto vert : graph->allVertices()) {
		for (auto p : vert->userData->properties()) {
			auto lnks = _space->findLinks(p->absolutePath());

			for (auto link : lnks) {
				auto any = _space->findAbsolute(link.output_path);

				if (any->parent() != nullptr) {
					if (any->parent()->type() == fl::type::DATA_OBJECT) {
						auto itr = vertices.find(any->parent()->rawData<fl::DataObject>()->uid());

						if (itr != vertices.end()) {
							graph->addEdge(itr->second, vert);
						}
					}
				}
			}
		}
	}
}

void DfxLuxCoreMaterialBuilder::deleteDAG(fl::DirectedAcyclicGraph<size_t, fl::DataObject> *graph) {
	for (auto v : graph->allVertices()) delete v;
	//delete graph;
}

void DfxLuxCoreMaterialBuilder::registerMaterial(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	if (obj->hasProperty("render_target")) {
		if (obj->property("render_target")->data<unsigned int>() == DfxRenderTarget::LUXCORE) {
			auto uid = std::to_string(obj->uid()) + "_Material";
			_matUniqueNameMap[any->absolutePath()] = uid;
		}
	}
	
}

DfxLuxCoreMaterialBuilder::Entry DfxLuxCoreMaterialBuilder::resolveSlot(std::string signature, fl::DataObject *obj, std::string name, std::string defaultStrVal ) {
	auto space = DfxSpace::getInstance();

	if (obj->hasProperty(name)) {
		auto any = obj->property(name);
		fl::Any *input = any;

		if (space->hasBinding(any->absolutePath())) {
			
			auto lnks = space->findLinks(any->absolutePath());

			auto other = space->findAbsolute(lnks[0].output_path);

			if (other != nullptr) {
				auto itr = _matUniqueNameMap.find(other->parent()->absolutePath());
				if (itr != _matUniqueNameMap.end()) {
					Entry ent(signature, Entry::REFERENCE);
					ent.xref = itr->second;

					return ent;
				}
				else {
					input = other;
				}
			}
		}

		if(input != nullptr) {
			if (input->type() == fl::type::FLOAT) {
				Entry ent(signature, Entry::FLOAT);
				ent.xfloat = input->data<float>();

				return ent;
			}
			if (input->type() == fl::type::FLOAT_2) {
				Entry ent(signature, Entry::FLOAT2);
				ent.xfloat2 = input->data<glm::vec2>();

				return ent;
			}
			else if (input->type() == fl::type::INT) {
				Entry ent(signature, Entry::INT);
				ent.xint = input->data<int>();

				return ent;
			}
			else if (input->type() == fl::type::STD_STRING) {
				std::string val = defaultStrVal;

				if(defaultStrVal.empty()) val = input->data<std::string>();

				Entry ent(signature, Entry::STRING);
				ent.xstring = val;

				return ent;
			}
		}
	}

	return Entry("", Entry::NONE);
}

DfxLuxCoreMaterialBuilder::Entry DfxLuxCoreMaterialBuilder::resolveSlot(std::string signature, fl::DataObject *obj, std::string proxy, std::string a, std::string b, std::string c) {
	auto space = DfxSpace::getInstance();

	if (obj->hasProperty(proxy)) {
		auto any = obj->property(proxy);
		fl::Any *input = nullptr;

		if (space->hasBinding(any->absolutePath())) {

			auto lnks = space->findLinks(any->absolutePath());

			auto other = space->findAbsolute(lnks[0].output_path);

			if (other != nullptr) {
				auto itr = _matUniqueNameMap.find(other->parent()->absolutePath());
				if (itr != _matUniqueNameMap.end()) {
					Entry ent(signature, Entry::REFERENCE);
					ent.xref = itr->second;

					return ent;
				}
				else {
					input = other;
				}
			}
		}

		if (input != nullptr) {
			if (input->type() == fl::type::FLOAT_3) {
				Entry ent(signature, Entry::FLOAT3);
				ent.xfloat3 = input->data<glm::vec3>();

				return ent;
			}
			else if (input->type() == fl::type::INT_3) {
				Entry ent(signature, Entry::INT3);
				ent.xint3 = input->data<glm::vec3>();

				return ent;
			}
		}
		else {
			auto _a = obj->property(a);
			auto _b = obj->property(b);
			auto _c = obj->property(c);

			if (_a->type() == fl::type::FLOAT || _b->type() == fl::type::FLOAT || _c->type() == fl::type::FLOAT) {
				Entry ent(signature, Entry::FLOAT3);
				ent.xfloat3 = glm::vec3(_a->data<float>(), _b->data<float>(), _c->data<float>());

				return ent;
			}
			else if (_a->type() == fl::type::INT || _b->type() == fl::type::INT || _c->type() == fl::type::INT) {
				Entry ent(signature, Entry::INT3);
				ent.xint3 = glm::ivec3(_a->data<int>(), _b->data<int>(), _c->data<int>());

				return ent;
			}
		}
	}

	return Entry("", Entry::NONE);
}

void DfxLuxCoreMaterialBuilder::setMaterialProperties(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();
	if (obj->hasProperty("uid")) {

		if (obj->property("uid")->data<std::string>() == "LUX_NULL_MAT") {
			setLuxNull(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUX_MATTE"){
			setLuxMatte(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_MATTE_TRANSLUCENT_MAT") {
			setLuxMatteTranslucent(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUX_EMISSION") {
			setLuxEmission(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUX_DISNEY") {
			setLuxDisney(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUX_MIX_MATERIAL") {
			setLuxMix(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUX_GLASS_MATERIAL") {
			setLuxGlass(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_ARCH_GLASS_MAT") {
			setLuxArchGlass(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_ROUGH_GLASS_MAT") {
			setLuxRoughGlass(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_CAR_PAINT_MAT") {
			setLuxCarPaint(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_CLOTH_MAT") {
			setLuxCloth(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_GLOSSY_MAT") {
			setLuxGlossy(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_GLOSSY_TRANSLUCENT_MAT") {
			setLuxGlossyTranslucent(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_METAL_MAT") {
			setLuxMetal(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_MIRROR_MAT") {
			setLuxMirror(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_VELVET_MAT") {
			setLuxVelvet(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_ROUGH_MATTE_MAT") {
			setLuxRoughMatte(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUX_IMAGEMAP") {
			setLuxImagemap(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUX_NORMALMAP_TEX") {
			setLuxNormalmap(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUX_2D_CHECKERBOARD") {
			setLux2dCheckerBoard(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUX_FBM") {
			setLuxFBM(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_FRESNEL_TEX") {
			setLuxFresnelColor(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_SOPRA_TEX") {
			setLuxFresnelSopra(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_BLEND_TEX") {
			setLuxBlend(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_CLOUD_TEX") {
			setLuxCloud(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_DISTORTED_NOISE_TEX") {
			setLuxDistortedNoise(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_MAGIC_TEX") {
			setLuxMagic(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_MARBLE_TEX") {
			setLuxMarble(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_MUSGRAVE_TEX") {
			setLuxMusgrave(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_NOISE_TEX") {
			setLuxNoise(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_STUCCI_TEX") {
			setLuxStucci(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_WOOD_TEX") {
			setLuxWood(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_VORONOI_TEX") {
			setLuxVoronoi(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_FRESNEL01_TEX") {
			setLuxFresnel01(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_MIX_TEX") {
			setLuxMixTex(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_CLAMP_TEX") {
			setLuxClamp(any);
		}
		else if (obj->property("uid")->data<std::string>() == "LUXCORE_HSV_TEX") {
			setLuxHsv(any);
		}
		
		
	}
}

DfxLuxMaterialChangeDispatch *DfxLuxCoreMaterialBuilder::createMatrialDispatcher() {

	auto space = DfxSpace::getInstance();

	auto any = space->findAbsolute("~/MATS");

	auto dispatcher = new DfxLuxMaterialChangeDispatch;

	if (any != nullptr) {

		for (auto material : any->data<fl::DataObject>().properties()) {
			DfxGlobal::console::println(material->label());

			if (material->type() == fl::type::DATA_OBJECT) {
				auto matObj = material->rawData<fl::DataObject>();

				if (matObj->hasProperty("render_target")) {
					if (matObj->property("render_target")->data<unsigned int>() == DfxRenderTarget::LUXCORE) {

						if (matObj->hasProperty("material_output")) {
							if (space->hasBinding(matObj->property("material_output")->absolutePath())) {

								for (auto node : matObj->properties()) {
									if (node->type() == fl::type::DATA_OBJECT) {
										dispatcher->attachBranch(node, {"xpos", "ypos"});
									}
								}

							}
						}
					}
				}
			}
		}
	}

	return dispatcher;
}


void DfxLuxCoreMaterialBuilder::build(luxcore::Scene *scn) {
	_scene = scn;
	auto space = DfxSpace::getInstance();

	auto any = space->findAbsolute("~/MATS");

	if (any != nullptr) {

		for (auto material : any->data<fl::DataObject>().properties()) {
			DfxGlobal::console::println(material->label());

			if (material->type() == fl::type::DATA_OBJECT) {
				auto matObj = material->rawData<fl::DataObject>();

				if (matObj->hasProperty("render_target")) {
					if (matObj->property("render_target")->data<unsigned int>() == DfxRenderTarget::LUXCORE) {

						if (matObj->hasProperty("material_output")) {
							if (space->hasBinding(matObj->property("material_output")->absolutePath())) {
								
								for (auto node : matObj->properties()) {
									if (node->type() == fl::type::DATA_OBJECT) registerMaterial(node);
								}

								std::vector<fl::Any*> dagNodes;
								for (auto node : matObj->properties()) if (node->type() == fl::type::DATA_OBJECT) dagNodes.push_back(node);
								buildDAG(dagNodes, &_dag);

								for (auto node : matObj->properties()) {
									if (node->type() == fl::type::DATA_OBJECT) setMaterialProperties(node);
								}

								auto vList = _dag.topologicalSort();
								for (auto v : vList) {
									auto props = _nodePropertyMap[v->id];
									_scene->Parse(props);
								}

								auto lnks = space->findLinks(matObj->property("material_output")->absolutePath());
								if (!lnks.empty()) {
									auto slt = space->findAbsolute(lnks[0].output_path);
									if (slt != nullptr) _rootMaterialMap[material->absolutePath()] = _matUniqueNameMap[slt->parent()->absolutePath()];
									else _rootMaterialMap[material->absolutePath()] = "";
								}
								else {
									_rootMaterialMap[material->absolutePath()] = "";
								}

								lnks = space->findLinks(matObj->property("displacement_output")->absolutePath());
								if (!lnks.empty()) {
									auto slt = space->findAbsolute(lnks[0].output_path);
									if (slt != nullptr) _displacementMaterialMap[material->absolutePath()] = _matUniqueNameMap[slt->parent()->absolutePath()];
									else _displacementMaterialMap[material->absolutePath()] = "";
								}
								else {
									_displacementMaterialMap[material->absolutePath()] = "";
								}

								DfxGlobal::console::println(std::string("Root Material: ") + _rootMaterialMap[material->absolutePath()]);
							}
						}					
					}
				}
			}
		}
	}
}

std::string DfxLuxCoreMaterialBuilder::material(std::string path) {
	return _rootMaterialMap[path];
}

std::string DfxLuxCoreMaterialBuilder::displacement(std::string path) {
	return _displacementMaterialMap[path];
}

std::string DfxLuxCoreMaterialBuilder::entryToString(DfxLuxCoreMaterialBuilder::Entry ent) {
	std::stringstream ss;
	if (ent.type() == Entry::REFERENCE) {
		ss << "( " << ent.signature() << ")(" << ent.xref << ")\n";
		return ss.str();
	}
	else if (ent.type() == Entry::STRING) {
		ss << "( " << ent.signature() << ")(" << ent.xstring << ")\n";
		return ss.str();
	}
	else if (ent.type() == Entry::FLOAT) {
		ss << "( " << ent.signature() << ")(" << std::to_string(ent.xfloat) << ")\n";
		return ss.str();
	}
	else if (ent.type() == Entry::FLOAT2) {
		ss << "( " << ent.signature() << ")(" << std::to_string(ent.xfloat2.x) << ", " << std::to_string(ent.xfloat2.y) << ")\n";
		return ss.str();
	}
	else if (ent.type() == Entry::FLOAT3) {
		ss << "( " << ent.signature() << ")(" << std::to_string(ent.xfloat3.x) << ", " << std::to_string(ent.xfloat3.y) << ", " << std::to_string(ent.xfloat3.y) << ")\n";
		return ss.str();
	}
	else if (ent.type() == Entry::INT) {
		ss << "( " << ent.signature() << ")(" << std::to_string(ent.xint) << ")\n";
		return ss.str();
	}
	else if (ent.type() == Entry::INT3) {
		ss << "( " << ent.signature() << ")(" << std::to_string(ent.xint3.x) << ", " << std::to_string(ent.xint3.y) << ", " << std::to_string(ent.xint3.y) << ")\n";
		return ss.str();
	}
	else {
		ss << "NONE\n";
		return ss.str();
	}
}

/// Method to set properties of each unique node
void DfxLuxCoreMaterialBuilder::setLuxNull(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "null";

	entries.push_back(type);

	DfxGlobal::console::println("Null:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
	//_scene->Parse(properties);
}

void DfxLuxCoreMaterialBuilder::setLuxMatte(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	auto shadow_catcher = obj->property("shadow_catcher")->data<bool>();

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "matte";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".kd", obj, "diffuse_color", "diffuse_r", "diffuse_g", "diffuse_b"));
	entries.push_back(resolveSlot(prefix + id + ".transparency.front", obj, "opacity"));
	entries.push_back(resolveSlot(prefix + id + ".transparency.back", obj, "opacity"));

	if (shadow_catcher) {
		Entry enable_scatcher(prefix + id + ".shadowcatcher.enable", Entry::INT);
		enable_scatcher.xint = 1;

		Entry enable_scatcher_inf_light(prefix + id + ".shadowcatcher.onlyinfinitelights", Entry::INT);
		enable_scatcher_inf_light.xint = 0;

		entries.push_back(enable_scatcher);
		entries.push_back(enable_scatcher_inf_light);
	}

	auto bump = resolveSlot(prefix + id + ".bumptex", obj, "bumpmap");
	if(!bump.xref.empty())entries.push_back(bump);

	DfxGlobal::console::println("Matte:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
	//_scene->Parse(properties);
}

/// Method to set properties of each unique node
void DfxLuxCoreMaterialBuilder::setLuxMatteTranslucent(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "mattetranslucent";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".kr", obj, "reflected_color", "reflected_r", "reflected_g", "reflected_b"));
	entries.push_back(resolveSlot(prefix + id + ".kt", obj, "transmission_color", "transmission_r", "transmission_g", "transmission_b"));
	entries.push_back(resolveSlot(prefix + id + ".transparency.front", obj, "opacity"));
	entries.push_back(resolveSlot(prefix + id + ".transparency.back", obj, "opacity"));

	auto bump = resolveSlot(prefix + id + ".bumptex", obj, "bumpmap");
	if (!bump.xref.empty())entries.push_back(bump);

	DfxGlobal::console::println("Matte Translucent:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
	//_scene->Parse(properties);
}

void DfxLuxCoreMaterialBuilder::setLuxEmission(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	float gain = obj->property("gain")->data<float>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "matte";

	Entry emGain(prefix + id + ".emission.gain", Entry::FLOAT3);
	emGain.xfloat3 = glm::vec3(gain, gain, gain);

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".emission", obj, "diffuse_color", "diffuse_r", "diffuse_g", "diffuse_b"));
	entries.push_back(resolveSlot(prefix + id + ".emission.power", obj, "power"));
	entries.push_back(resolveSlot(prefix + id + ".emission.efficency", obj, "efficency"));
	entries.push_back(resolveSlot(prefix + id + ".emission.importance", obj, "importance"));
	entries.push_back(resolveSlot(prefix + id + ".emission.theta", obj, "spread"));
	entries.push_back(emGain);

	DfxGlobal::console::println("Emission:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
	//_scene->Parse(properties);
}

void DfxLuxCoreMaterialBuilder::setLuxDisney(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "disney";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".basecolor",	obj, "base_color", "base_r", "base_g", "base_b"));
	entries.push_back(resolveSlot(prefix + id + ".subsurface",	obj, "subsurface"));
	entries.push_back(resolveSlot(prefix + id + ".roughness",	obj, "roughness"));
	entries.push_back(resolveSlot(prefix + id + ".metallic",	obj, "metallic"));
	entries.push_back(resolveSlot(prefix + id + ".specular",	obj, "specular"));
	entries.push_back(resolveSlot(prefix + id + ".speculartint", obj, "specular_tint"));
	entries.push_back(resolveSlot(prefix + id + ".clearcoat",	obj, "clear_coat"));
	entries.push_back(resolveSlot(prefix + id + ".clearcoatgloss", obj, "clear_coat_gloss"));
	entries.push_back(resolveSlot(prefix + id + ".anisotropic", obj, "anisotropic"));
	entries.push_back(resolveSlot(prefix + id + ".sheen",		obj, "sheen"));
	entries.push_back(resolveSlot(prefix + id + ".sheentint", obj, "sheen_tint"));
	entries.push_back(resolveSlot(prefix + id + ".transparency.front", obj, "opacity"));
	entries.push_back(resolveSlot(prefix + id + ".transparency.back",	obj, "opacity"));

	auto bump = resolveSlot(prefix + id + ".bumptex", obj, "bumpmap");
	if (!bump.xref.empty())entries.push_back(bump);
	
	DfxGlobal::console::println("Disney:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
	//_scene->Parse(properties);
}

void DfxLuxCoreMaterialBuilder::setLuxMix(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "mix";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".material1", obj, "mat_00", "default"));
	entries.push_back(resolveSlot(prefix + id + ".material2", obj, "mat_01", "default"));
	entries.push_back(resolveSlot(prefix + id + ".amount", obj, "amount"));

	DfxGlobal::console::println("Mix:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
	//_scene->Parse(properties);
}

void  DfxLuxCoreMaterialBuilder::setLuxGlass(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "glass";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".kt", obj, "transmission_color", "transmission_r", "transmission_g", "transmission_b"));
	entries.push_back(resolveSlot(prefix + id + ".kr", obj, "reflection_color", "reflection_r", "reflection_g", "reflection_b"));
	entries.push_back(resolveSlot(prefix + id + ".transparency.shadow", obj, "shadow_color", "shadow_r", "shadow_g", "shadow_b"));
	entries.push_back(resolveSlot(prefix + id + ".interiorior", obj, "ior"));
	entries.push_back(resolveSlot(prefix + id + ".cauchyc", obj, "dispersion"));

	auto bump = resolveSlot(prefix + id + ".bumptex", obj, "bumpmap");
	if (!bump.xref.empty())entries.push_back(bump);

	DfxGlobal::console::println("Glass:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxArchGlass(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "archglass";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".kt", obj, "transmission_color", "transmission_r", "transmission_g", "transmission_b"));
	entries.push_back(resolveSlot(prefix + id + ".kr", obj, "reflection_color", "reflection_r", "reflection_g", "reflection_b"));
	entries.push_back(resolveSlot(prefix + id + ".transparency.shadow", obj, "shadow_color", "shadow_r", "shadow_g", "shadow_b"));
	entries.push_back(resolveSlot(prefix + id + ".interiorior", obj, "ior"));

	auto bump = resolveSlot(prefix + id + ".bumptex", obj, "bumpmap");
	if (!bump.xref.empty())entries.push_back(bump);

	DfxGlobal::console::println("Arch Glass:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxRoughGlass(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "roughglass";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".kt", obj, "transmission_color", "transmission_r", "transmission_g", "transmission_b"));
	entries.push_back(resolveSlot(prefix + id + ".kr", obj, "reflection_color", "reflection_r", "reflection_g", "reflection_b"));
	entries.push_back(resolveSlot(prefix + id + ".transparency.shadow", obj, "shadow_color", "shadow_r", "shadow_g", "shadow_b"));
	entries.push_back(resolveSlot(prefix + id + ".uroughness", obj, "roughness_u"));
	entries.push_back(resolveSlot(prefix + id + ".vroughness", obj, "roughness_v"));
	entries.push_back(resolveSlot(prefix + id + ".interiorior", obj, "ior"));

	auto bump = resolveSlot(prefix + id + ".bumptex", obj, "bumpmap");
	if (!bump.xref.empty())entries.push_back(bump);

	DfxGlobal::console::println("Rough Glass:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxCarPaint(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "carpaint";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".kd", obj, "diffuse_color", "diffuse_r", "diffuse_g", "diffuse_b"));
	entries.push_back(resolveSlot(prefix + id + ".ka", obj, "absorption_color", "absorption_r", "absorption_g", "absorption_b"));
	entries.push_back(resolveSlot(prefix + id + ".d", obj, "absorption_depth"));
	entries.push_back(resolveSlot(prefix + id + ".ks1", obj, "specular_color00", "specular00_r", "specular00_g", "specular00_b"));
	entries.push_back(resolveSlot(prefix + id + ".ks2", obj, "specular_color01", "specular01_r", "specular01_g", "specular01_b"));
	entries.push_back(resolveSlot(prefix + id + ".ks3", obj, "specular_color02", "specular02_r", "specular02_g", "specular02_b"));
	entries.push_back(resolveSlot(prefix + id + ".r1", obj, "fresnel_effect00"));
	entries.push_back(resolveSlot(prefix + id + ".r2", obj, "fresnel_effect01"));
	entries.push_back(resolveSlot(prefix + id + ".r3", obj, "fresnel_effect02"));
	entries.push_back(resolveSlot(prefix + id + ".m1", obj, "roughness00"));
	entries.push_back(resolveSlot(prefix + id + ".m2", obj, "roughness01"));
	entries.push_back(resolveSlot(prefix + id + ".m3", obj, "roughness02"));

	auto bump = resolveSlot(prefix + id + ".bumptex", obj, "bumpmap");
	if (!bump.xref.empty())entries.push_back(bump);

	DfxGlobal::console::println("Car Paint:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxCloth(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "cloth";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".warp_kd", obj, "warp_diffuse_reflection", "warp_diffuse_r", "warp_diffuse_g", "warp_diffuse_b"));
	entries.push_back(resolveSlot(prefix + id + ".warp_ks", obj, "warp_specular_reflection", "warp_specular_r", "warp_specular_g", "warp_specular_b"));
	entries.push_back(resolveSlot(prefix + id + ".weft_kd", obj, "weft_diffuse_reflection", "weft_diffuse_r", "weft_diffuse_g", "weft_diffuse_b"));
	entries.push_back(resolveSlot(prefix + id + ".weft_ks", obj, "weft_specular_reflection", "weft_specular_r", "weft_specular_g", "weft_specular_b"));
	entries.push_back(resolveSlot(prefix + id + ".repeat_u", obj, "thread_count_u"));
	entries.push_back(resolveSlot(prefix + id + ".repeat_v", obj, "thread_count_v"));

	auto bump = resolveSlot(prefix + id + ".bumptex", obj, "bumpmap");
	if (!bump.xref.empty())entries.push_back(bump);

	DfxGlobal::console::println("Cloth:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxGlossy(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "glossy2";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".kd", obj, "diffuse_color", "diffuse_r", "diffuse_g", "diffuse_b"));
	entries.push_back(resolveSlot(prefix + id + ".ks", obj, "specular_color", "specular_r", "specular_g", "specular_b"));
	entries.push_back(resolveSlot(prefix + id + ".ka", obj, "absorption_color", "absorption_r", "absorption_g", "absorption_b"));
	entries.push_back(resolveSlot(prefix + id + ".uroughness", obj, "roughness_u"));
	entries.push_back(resolveSlot(prefix + id + ".vroughness", obj, "roughness_v"));
	entries.push_back(resolveSlot(prefix + id + ".d", obj, "absorption_depth"));
	entries.push_back(resolveSlot(prefix + id + ".index", obj, "ior"));

	auto bump = resolveSlot(prefix + id + ".bumptex", obj, "bumpmap");
	if (!bump.xref.empty())entries.push_back(bump);

	DfxGlobal::console::println("Glossy:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxGlossyTranslucent(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "glossytranslucent";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".kd", obj, "diffuse_color", "diffuse_r", "diffuse_g", "diffuse_b"));
	entries.push_back(resolveSlot(prefix + id + ".kt", obj, "transmission_color", "transmission_r", "transmission_g", "transmission_b"));
	entries.push_back(resolveSlot(prefix + id + ".ks", obj, "specular_color", "specular_r", "specular_g", "specular_b"));
	entries.push_back(resolveSlot(prefix + id + ".ka", obj, "absorption_color", "absorption_r", "absorption_g", "absorption_b"));
	entries.push_back(resolveSlot(prefix + id + ".uroughness", obj, "roughness_u"));
	entries.push_back(resolveSlot(prefix + id + ".vroughness", obj, "roughness_v"));
	entries.push_back(resolveSlot(prefix + id + ".d", obj, "absorption_depth"));
	entries.push_back(resolveSlot(prefix + id + ".index", obj, "ior"));

	auto bump = resolveSlot(prefix + id + ".bumptex", obj, "bumpmap");
	if (!bump.xref.empty())entries.push_back(bump);

	DfxGlobal::console::println("Glossy Translucent:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxMetal(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "metal2";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".fresnel", obj, "fresnel_color", "fresnel_r", "fresnel_g", "fresnel_b"));
	entries.push_back(resolveSlot(prefix + id + ".uroughness", obj, "roughness_u"));
	entries.push_back(resolveSlot(prefix + id + ".vroughness", obj, "roughness_v"));

	auto bump = resolveSlot(prefix + id + ".bumptex", obj, "bumpmap");
	if (!bump.xref.empty())entries.push_back(bump);

	DfxGlobal::console::println("Metal:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxMirror(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "mirror";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".kr", obj, "reflection_color", "reflection_r", "reflection_g", "reflection_b"));

	auto bump = resolveSlot(prefix + id + ".bumptex", obj, "bumpmap");
	if (!bump.xref.empty())entries.push_back(bump);

	DfxGlobal::console::println("Mirror:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxVelvet(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "velvet";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".kd", obj, "diffuse_color", "diffuse_r", "diffuse_g", "diffuse_b"));
	entries.push_back(resolveSlot(prefix + id + ".p1", obj, "p00"));
	entries.push_back(resolveSlot(prefix + id + ".p2", obj, "p01"));
	entries.push_back(resolveSlot(prefix + id + ".p3", obj, "p02"));
	entries.push_back(resolveSlot(prefix + id + ".thickness", obj, "thickness"));

	auto bump = resolveSlot(prefix + id + ".bumptex", obj, "bumpmap");
	if (!bump.xref.empty())entries.push_back(bump);

	DfxGlobal::console::println("Velvet:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxRoughMatte(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.materials.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "roughmatte";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".kd", obj, "diffuse_color", "diffuse_r", "diffuse_g", "diffuse_b"));
	entries.push_back(resolveSlot(prefix + id + ".sigma", obj, "sigma"));

	auto bump = resolveSlot(prefix + id + ".bumptex", obj, "bumpmap");
	if (!bump.xref.empty())entries.push_back(bump);

	DfxGlobal::console::println("Rough Matte:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

// Textures
void DfxLuxCoreMaterialBuilder::setLuxFBM(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "fbm";

	Entry mapping(prefix + id + ".mapping.type", Entry::STRING);
	mapping.xstring = "globalmapping3d";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".octaves", obj, "octaves"));
	entries.push_back(resolveSlot(prefix + id + ".roughness", obj, "roughness"));
	entries.push_back(mapping);

	DfxGlobal::console::println("FBM:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;

}

void DfxLuxCoreMaterialBuilder::setLux2dCheckerBoard(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "checkerboard2d";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".texture1", obj, "octaves", "default"));
	entries.push_back(resolveSlot(prefix + id + ".texture2", obj, "roughness", "default"));

	DfxGlobal::console::println("2D Checkerboard:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxImagemap(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::string wrapStr = "";
	std::string channelStr = "";

	auto wrapId = obj->property("wrap")->data<unsigned int>();
	auto channelId = obj->property("channel")->data<unsigned int>();

	auto u_rot = obj->property("uv_rotation")->data<float>();

	auto u_transx = obj->property("uv_translation_x")->data<float>();
	auto u_transy = obj->property("uv_translation_y")->data<float>();
	
	auto u_scalex = obj->property("uv_scale_x")->data<float>();
	auto u_scaley = obj->property("uv_scale_y")->data<float>();

	if (wrapId == 0) wrapStr = "repeat";
	else if (wrapId == 1) wrapStr = "black";
	else if (wrapId == 2) wrapStr = "white";
	else if (wrapId == 3) wrapStr = "clamp";

	if (channelId == 0) channelStr = "default";
	else if (channelId == 1) channelStr = "red";
	else if (channelId == 2) channelStr = "green";
	else if (channelId == 3) channelStr = "blue";
	else if (channelId == 4) channelStr = "alpha";
	else if (channelId == 5) channelStr = "mean";
	else if (channelId == 6) channelStr = "colored_mean";
	else if (channelId == 7) channelStr = "rgb";

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "imagemap";

	Entry wrap(prefix + id + ".wrap", Entry::STRING);
	wrap.xstring = wrapStr;

	Entry channel(prefix + id + ".channel", Entry::STRING);
	channel.xstring = channelStr;

	Entry storage(prefix + id + ".storage", Entry::STRING);
	storage.xstring = "byte";

	Entry mappingTyp(prefix + id + ".mapping.type", Entry::STRING);
	mappingTyp.xstring = "uvmapping2d";

	Entry mappingScale(prefix + id + ".mapping.uvscale", Entry::FLOAT2);
	mappingScale.xfloat2 = glm::vec2(u_scalex, u_scaley);

	Entry mappingDelta(prefix + id + ".uvdelta", Entry::FLOAT2);
	mappingDelta.xfloat2 = glm::vec2(u_transx, u_transy);

	auto filePath = resolveSlot(prefix + id + ".file", obj, "file");

	if (!filePath.xstring.empty()) {
		entries.push_back(type);
		entries.push_back(filePath);
		entries.push_back(resolveSlot(prefix + id + ".gamma", obj, "gamma"));
		entries.push_back(resolveSlot(prefix + id + ".gain", obj, "gain"));
		entries.push_back(storage);
		entries.push_back(mappingTyp);
		entries.push_back(resolveSlot(prefix + id + ".mapping.rotation", obj, "uv_rotation"));
		entries.push_back(wrap);
		entries.push_back(channel);
		entries.push_back(mappingScale);
		entries.push_back(mappingDelta);

		DfxGlobal::console::println("Imagemap: ");
		for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

		luxrays::Properties properties;
		for (auto &e : entries) properties << e.toProp();

		_nodePropertyMap[obj->uid()] = properties;
	}
}

void DfxLuxCoreMaterialBuilder::setLuxNormalmap(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "normalmap";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".texture", obj, "texture", "texture_x", "texture_y", "texture_z"));
	entries.push_back(resolveSlot(prefix + id + ".scale", obj, "scale"));

	DfxGlobal::console::println("Normal Map:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxFresnelColor(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "fresnelcolor";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".kr", obj, "reflection_color", "reflection_r", "reflection_g", "reflection_b"));

	DfxGlobal::console::println("Fresnel Color:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxFresnelSopra(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "fresnelsopra";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".file", obj, "file"));

	DfxGlobal::console::println("Fresnel Sopra:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxBlend(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "blender_blend";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".progressiontype", obj, "progression"));
	entries.push_back(resolveSlot(prefix + id + ".direction", obj, "direction"));
	entries.push_back(resolveSlot(prefix + id + ".bright", obj, "bright"));
	entries.push_back(resolveSlot(prefix + id + ".contrast", obj, "contrast"));

	DfxGlobal::console::println("Blend:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxCloud(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "blender_clouds";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".noisetype", obj, "noise_type"));
	entries.push_back(resolveSlot(prefix + id + ".noisebasis", obj, "noise_basis"));
	entries.push_back(resolveSlot(prefix + id + ".noisesize", obj, "noise_size"));
	entries.push_back(resolveSlot(prefix + id + ".noisedepth", obj, "noise_depth"));
	entries.push_back(resolveSlot(prefix + id + ".bright", obj, "bright"));
	entries.push_back(resolveSlot(prefix + id + ".contrast", obj, "contrast"));

	DfxGlobal::console::println("Clouds:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxDistortedNoise(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "blender_distortednoise";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".noise_distortion", obj, "noise_distortion"));
	entries.push_back(resolveSlot(prefix + id + ".noisebasis", obj, "noise_basis"));
	entries.push_back(resolveSlot(prefix + id + ".noisesize", obj, "noise_size"));
	entries.push_back(resolveSlot(prefix + id + ".distortion", obj, "distortion"));
	entries.push_back(resolveSlot(prefix + id + ".bright", obj, "bright"));
	entries.push_back(resolveSlot(prefix + id + ".contrast", obj, "contrast"));

	DfxGlobal::console::println("Distorted Noise:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxMagic(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "blender_magic";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".noisedepth", obj, "noise_depth"));
	entries.push_back(resolveSlot(prefix + id + ".turbulence", obj, "turbulence"));
	entries.push_back(resolveSlot(prefix + id + ".bright", obj, "bright"));
	entries.push_back(resolveSlot(prefix + id + ".contrast", obj, "contrast"));

	DfxGlobal::console::println("Magic:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxMarble(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "blender_marble";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".marbletype", obj, "marble_type"));
	entries.push_back(resolveSlot(prefix + id + ".noisetype", obj, "noise_type"));
	entries.push_back(resolveSlot(prefix + id + ".noisebasis", obj, "noise_basis"));
	entries.push_back(resolveSlot(prefix + id + ".noisebasis2", obj, "noise_basis2"));
	entries.push_back(resolveSlot(prefix + id + ".turbulence", obj, "turbulence"));
	entries.push_back(resolveSlot(prefix + id + ".noisedepth", obj, "noise_depth"));
	entries.push_back(resolveSlot(prefix + id + ".noisesize", obj, "noise_size"));
	entries.push_back(resolveSlot(prefix + id + ".bright", obj, "bright"));
	entries.push_back(resolveSlot(prefix + id + ".contrast", obj, "contrast"));

	DfxGlobal::console::println("Marble:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxMusgrave(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "blender_musgrave";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".musgravetype", obj, "musgrave_type"));
	entries.push_back(resolveSlot(prefix + id + ".noisetype", obj, "noise_type"));
	entries.push_back(resolveSlot(prefix + id + ".noisebasis", obj, "noise_basis"));
	entries.push_back(resolveSlot(prefix + id + ".dimension", obj, "dimension"));
	entries.push_back(resolveSlot(prefix + id + ".intensity", obj, "intensity"));
	entries.push_back(resolveSlot(prefix + id + ".lacunarity", obj, "lacunarity"));
	entries.push_back(resolveSlot(prefix + id + ".offset", obj, "offset"));
	entries.push_back(resolveSlot(prefix + id + ".gain", obj, "gain"));
	entries.push_back(resolveSlot(prefix + id + ".octaves", obj, "octaves"));
	entries.push_back(resolveSlot(prefix + id + ".noisesize", obj, "noise_size"));
	entries.push_back(resolveSlot(prefix + id + ".bright", obj, "bright"));
	entries.push_back(resolveSlot(prefix + id + ".contrast", obj, "contrast"));

	DfxGlobal::console::println("Musgrave:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxNoise(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "blender_noise";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".noisedepth", obj, "noise_depth"));
	entries.push_back(resolveSlot(prefix + id + ".bright", obj, "bright"));
	entries.push_back(resolveSlot(prefix + id + ".contrast", obj, "contrast"));

	DfxGlobal::console::println("Noise:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxStucci(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "blender_stucci";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".stuccitype", obj, "stucci_type"));
	entries.push_back(resolveSlot(prefix + id + ".noisebasis", obj, "noise_basis"));
	entries.push_back(resolveSlot(prefix + id + ".noisetype", obj, "noise_type"));
	entries.push_back(resolveSlot(prefix + id + ".noisesize", obj, "noise_size"));
	entries.push_back(resolveSlot(prefix + id + ".turbulence", obj, "turbulence"));
	entries.push_back(resolveSlot(prefix + id + ".bright", obj, "bright"));
	entries.push_back(resolveSlot(prefix + id + ".contrast", obj, "contrast"));

	DfxGlobal::console::println("Stucci:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxWood(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "blender_wood";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".woodtype", obj, "wood_type"));
	entries.push_back(resolveSlot(prefix + id + ".noisebasis", obj, "noise_basis"));
	entries.push_back(resolveSlot(prefix + id + ".noisebasis2", obj, "noise_basis2"));
	entries.push_back(resolveSlot(prefix + id + ".noisetype", obj, "noise_type"));
	entries.push_back(resolveSlot(prefix + id + ".noisesize", obj, "noise_size"));
	entries.push_back(resolveSlot(prefix + id + ".turbulence", obj, "turbulence"));
	entries.push_back(resolveSlot(prefix + id + ".bright", obj, "bright"));
	entries.push_back(resolveSlot(prefix + id + ".contrast", obj, "contrast"));

	DfxGlobal::console::println("Wood:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxVoronoi(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "blender_voronoi";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".distmetric", obj, "distmetric"));
	entries.push_back(resolveSlot(prefix + id + ".intensity", obj, "intensity"));
	entries.push_back(resolveSlot(prefix + id + ".w1", obj, "w00"));
	entries.push_back(resolveSlot(prefix + id + ".w2", obj, "w01"));
	entries.push_back(resolveSlot(prefix + id + ".w3", obj, "w02"));
	entries.push_back(resolveSlot(prefix + id + ".w4", obj, "w03"));
	entries.push_back(resolveSlot(prefix + id + ".noisesize", obj, "noise_size"));
	entries.push_back(resolveSlot(prefix + id + ".bright", obj, "bright"));
	entries.push_back(resolveSlot(prefix + id + ".contrast", obj, "contrast"));

	DfxGlobal::console::println("Voronoi:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxFresnel01(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "fresnelapproxk";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".texture", obj, "color", "color_r", "color_g", "color_b"));
	
	DfxGlobal::console::println("Fresnel 01:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

// Texture Util
void DfxLuxCoreMaterialBuilder::setLuxMixTex(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "mix";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".amount", obj, "amount"));
	entries.push_back(resolveSlot(prefix + id + ".texture1", obj, "texture1"));
	entries.push_back(resolveSlot(prefix + id + ".texture2", obj, "texture2"));

	DfxGlobal::console::println("Mix Texture:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxClamp(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "clamp";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".texture", obj, "texture"));
	entries.push_back(resolveSlot(prefix + id + ".min", obj, "min"));
	entries.push_back(resolveSlot(prefix + id + ".max", obj, "max"));

	DfxGlobal::console::println("Clamp:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}

void DfxLuxCoreMaterialBuilder::setLuxHsv(fl::Any *any) {
	auto obj = any->rawData<fl::DataObject>();

	std::vector<Entry> entries;

	auto id = _matUniqueNameMap.find(any->absolutePath())->second;
	std::string prefix("scene.textures.");

	// Material type
	Entry type(prefix + id + ".type", Entry::STRING);
	type.xstring = "hsv";

	entries.push_back(type);
	entries.push_back(resolveSlot(prefix + id + ".texture", obj, "texture"));
	entries.push_back(resolveSlot(prefix + id + ".hue", obj, "hue"));
	entries.push_back(resolveSlot(prefix + id + ".saturation", obj, "saturation"));
	entries.push_back(resolveSlot(prefix + id + ".value", obj, "value"));

	DfxGlobal::console::println("Hsv:");
	for (auto &e : entries) DfxGlobal::console::println("   -" + entryToString(e));

	luxrays::Properties properties;
	for (auto &e : entries) properties << e.toProp();

	_nodePropertyMap[obj->uid()] = properties;
}
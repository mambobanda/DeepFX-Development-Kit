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

#include "FluentComputeAttributesInitializer.h"

namespace compute {
	void initAttributeNames() {
		auto dict = DfxFluentAttributeDictionary::getInstance();

		dict->addAttribute("@mass");
		dict->addAttribute("@friction");
		dict->addAttribute("@restitution");
		dict->addAttribute("@l_dampening");
		dict->addAttribute("@a_dampening");
		dict->addAttribute("@force");
		dict->addAttribute("@angular_force");
		dict->addAttribute("@uid");
		dict->addAttribute("@sph_class");
		dict->addAttribute("@alive");
		dict->addAttribute("@update");
		dict->addAttribute("@use_viscosity");
		dict->addAttribute("@viscosity");
		dict->addAttribute("@use_vorticity");
		dict->addAttribute("@vorticity_viscosity");
		dict->addAttribute("@vorticity");
		dict->addAttribute("@vorticity_viscosity");
		dict->addAttribute("@vorticity_inertia");
		dict->addAttribute("@use_surface_tension");
		dict->addAttribute("@surface_tension");
		dict->addAttribute("@set_name");
		dict->addAttribute("@scene_path");
		dict->addAttribute("@alias");
		dict->addAttribute("@xform");
		dict->addAttribute("@position");
		dict->addAttribute("@velocity");
		//dict->addAttribute("@");
		//dict->addAttribute("@");

		DfxFluentAttributeDictionary::getInstance()->sortList();
	}
}
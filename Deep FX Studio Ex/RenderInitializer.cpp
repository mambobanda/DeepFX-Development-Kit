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


#include "RenderInitializer.h"

namespace render {

	void intitialize() {

		// Register Render Instances
		DfxRenderManager::getInstance()->addRenderSessionCallback(DfxRenderTarget::LUXCORE, []() -> DfxRenderSessionInterface* {
			return new DfxLuxCoreRenderSession;
		});

		/// Master Space
		auto space = DfxSpace::getInstance();

		/// Find Render Branch
		auto temp_branch = space->findAbsolute("~/RENDER_GLOBAL");

		/// Proceed only if it exists
		if (temp_branch != nullptr) {

			/// Render Targets
			auto render_targets = fl::DataObject::Builder()
				.setName("render_targets")
				.setLabel("Render Targets")
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "luxcore", "LuxCore", DfxRenderTarget::LUXCORE)
				.build(space);

			/// General Properties
			auto general_properties = fl::DataObject::Builder()
				.setName("general_properties")
				.setLabel("General Render Properties")
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "frame_start", "Frame Start", 0)
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "frame_end", "Frame End", 250)
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "frame_step", "Frame Step", 1)
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "fps", "Frames Per Second", 24)
				.addProperty(space, fl::type::UNSIGNED_INT, fl::role::NONE, "film_resolution", "Resolution Percentage", 100)
				.build(space);

			space->move(render_targets, temp_branch);
			space->move(general_properties, temp_branch);
		
		}
		else {
			DfxGlobal::console::println("Failed to Init Render Settings.");
		}

	}
}
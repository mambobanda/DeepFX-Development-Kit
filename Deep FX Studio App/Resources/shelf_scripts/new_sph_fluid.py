from xglobal import println
from xglobal import messageBox
import xgraph as xg


"""
Create a new sph fluid network
- This script creates a new sph fluid network for a selected object
It makes certain assumptions about the existing dynamics network and
tries to incorprate the new network into an existing one, if not
it will ask the user if they want to create a new one.
"""

def new_sph_fluid(selection):

	if xg.hasMeshData(selection):
		all_ = xg.findAllNodes()
		sphSolverID = 0
		sphSolverExists = False
		linkToExistingSolver = False

		voxelizerID  = xg.createNode("2F2X252X_DFX_MESH_VOXELIZER")
		poinXFormID  = xg.createNode("2F2X252X_DFX_POINTX_FORM")
		sph_fluidID  = xg.createNode("2F2X252X_DFX_SPH_LIQUID")
		solver = 0
		cache  = 0
		group  = 0
		frameIinfo = 0

		# If selected object is standard object item connect the basics
		if xg.getNodeUniqueName(selection) == "2F2X252X_DFX_OBJECT_ITEM":
			out_sel_pos = xg.getPropertyID(selection, "out_position")
			out_sel_rot = xg.getPropertyID(selection, "out_rotation")
			out_sel_scl = xg.getPropertyID(selection, "out_scale")

			in_vox_scl = xg.getPropertyID(voxelizerID, "scale")

			in_xform_pos = xg.getPropertyID(poinXFormID, "position")
			in_xform_rot = xg.getPropertyID(poinXFormID, "rotation")
			

			# Connect outputs to inputs 
			xg.connectProperties(selection, out_sel_pos, poinXFormID, in_xform_pos)
			xg.connectProperties(selection, out_sel_rot, poinXFormID, in_xform_rot)
			xg.connectProperties(selection, out_sel_scl, voxelizerID, in_vox_scl)

		if xg.hasProperty(selection, "@ref"):
			out_ref = xg.getPropertyID(selection, "@ref")
			in_ref  = xg.getPropertyID(voxelizerID, "in_ref")

			xg.connectProperties(selection, out_ref, voxelizerID, in_ref)

		# Connect Point to SPH body
		in_sph_points   = xg.getPropertyID(sph_fluidID, "points")
		in_xform_points = xg.getPropertyID(poinXFormID, "in_points")
		out_xform_points = xg.getPropertyID(poinXFormID, "points")
		out_vox_points  = xg.getPropertyID(voxelizerID, "points")

		xg.connectProperties(voxelizerID, out_vox_points, poinXFormID, in_xform_points)
		xg.connectProperties(poinXFormID, out_xform_points, sph_fluidID, in_sph_points)

		xg.uniformLayout()
		xg.viewAllNodes()

		# Check for existing solver
		for uid in all_:
			if xg.getNodeUniqueName(uid) == "2F2X252X_DFX_DIVERGENCE_FREE_SPH_SOLVER":
				sphSolverExists = True
				sphSolverID = uid
				break

		# Let the user decide what path to take
		if sphSolverExists:
			rt = messageBox("Found Existing SPH Solver", "Link sph fluid to exiting solver?", 400, "Yes_No")
			if rt:
				linkToExistingSolver = True
		else:
			rt = messageBox("Create New SPH Solver", "Link sph fluid to new solver?", 400, "Yes_No")
			if rt:
				solver = xg.createNode("2F2X252X_DFX_DIVERGENCE_FREE_SPH_SOLVER")
				sph_map = xg.createViewNode("2F2X252X_DFX_SPH_MAP_CHANNEL")
				cache  = xg.createViewNode("2F2X252X_DFX_PARTICLE_CACHE")
				group  = xg.createNode("2F2X252X_DFX_GROUP_NODE")
				frameinfo = xg.createNode("2F2X252X_DFX_TIME_INFO")

				solv_out_ref = xg.getPropertyID(solver, "@ref")
				solv_in_ref  = xg.getPropertyID(solver, "in_ref")
				solv_in_trigger  = xg.getPropertyID(solver, "trigger")
				map_in_ref   = xg.getPropertyID(sph_map, "in_ref")
				map_out_pos  = xg.getPropertyID(sph_map, "positions")
				map_out_vel  = xg.getPropertyID(sph_map, "velocities")
				cach_in_index = xg.getPropertyID(cache, "index")
				cach_in_pos  = xg.getPropertyID(cache, "position")
				cach_in_vel  = xg.getPropertyID(cache, "velocity")
				frame_out_current = xg.getPropertyID(frameIinfo, "current_frame")

				xg.connectProperties(frameinfo, frame_out_current, solver, solv_in_trigger)
				xg.connectProperties(frameinfo, frame_out_current, cache, cach_in_index)

				xg.connectProperties(group, 0, solver, solv_in_ref)

				xg.connectProperties(solver, solv_out_ref, sph_map, map_in_ref)

				xg.connectProperties(sph_map, map_out_pos, cache, cach_in_pos)
				xg.connectProperties(sph_map, map_out_vel, cache, cach_in_vel)

				sphSolverID = solver
				linkToExistingSolver = True

		# Handle solver stuff here, create or link to existing one
		if linkToExistingSolver:
			solv_in_ref = xg.getPropertyID(sphSolverID, "in_ref")

			if xg.propertyHasParent(sphSolverID, solv_in_ref):
				in_ref_parent = xg.getPropertyParent(sphSolverID, solv_in_ref)

				if xg.getNodeUniqueName(in_ref_parent) == "2F2X252X_DFX_GROUP_NODE":
					fluid_our_ref = xg.getPropertyID(sph_fluidID, "@ref")
					xg.connectToGroup(sph_fluidID, fluid_our_ref, in_ref_parent)
			else:
				group = xg.createNode("2F2X252X_DFX_GROUP_NODE")
				fluid_our_ref = xg.getPropertyID(sph_fluidID, "@ref")
				xg.connectProperties(group, 0, sphSolverID, solv_in_ref)
				xg.connectToGroup(sph_fluidID, fluid_our_ref, group)

		
		# Finally layout nodes in a readable manner and show whole graph for clarity
		xg.uniformLayout()
		xg.viewAllNodes()
	else:
		messageBox("No Mesh Data Found.", "Please select an object with mesh data.", 400, "")


# Check viewport selection
hasSelection = xg.viewHasSelection()
hasGraphSelection = xg.graphHasSelection()
selectionList = []

# Add items selected in viewport to list
if hasSelection:
	selection = xg.getViewSelection()
	selectionList.append(selection)

# Add items selected in Graph editor to list
if hasGraphSelection:
	selections = xg.getGraphSelection()
	for selection in selections:
		selectionList.append(selection)

# Attempt to create rigid objects from list
if len(selectionList) > 0:
	# Remove Duplicates
	selectionList = list(dict.fromkeys(selectionList))
	for selection in selectionList:
		new_sph_fluid(selection)
else:
	messageBox("Nothing Selected.", "Please select an object first to turn it into a sph fluid.", 400, "")
	
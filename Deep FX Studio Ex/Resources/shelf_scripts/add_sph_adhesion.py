from xglobal import println
from xglobal import messageBox
from xglobal import getFloatValue
import xgraph as xg


"""
Create a new sph adhesion network
- This script creates a new sph emitter network.
It makes certain assumptions about the existing dynamics network and
tries to incorprate the new network into an existing one, if not
it will ask the user if they want to create a new one.
"""

def add_sph_adhesion():

	all_ = xg.findAllNodes()
	sphSolverID = 0
	sphSolverExists = False
	linkToExistingSolver = False

	force = getFloatValue("SPH Adhesion", "Fluid Adhesion", 0.05)
	
	forceID  = xg.createViewNode("2F2X252X_DFX_SPH_ADHESION")
	solver = 0
	cache  = 0
	group  = 0
	frameIinfo = 0

	xg.setFloat(forceID, "in_strength", force);

	# Check for existing solver
	for uid in all_:
		if xg.getNodeUniqueName(uid) == "2F2X252X_DFX_DIVERGENCE_FREE_SPH_SOLVER":
			sphSolverExists = True
			sphSolverID = uid
			break

	# Let the user decide what path to take
	if sphSolverExists:
		rt = messageBox("Found Existing SPH Solver", "Link sph force to exiting solver?", 400, "Yes_No")
		if rt:
			linkToExistingSolver = True
	else:
		rt = messageBox("Create New SPH Solver", "Link sph force to new solver?", 400, "Yes_No")
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
				force_our_ref = xg.getPropertyID(forceID, "@ref")
				xg.connectToGroup(forceID, force_our_ref, in_ref_parent)
		else:
			group = xg.createNode("2F2X252X_DFX_GROUP_NODE")
			force_our_ref = xg.getPropertyID(sph_fluidID, "@ref")
			xg.connectProperties(group, 0, sphSolverID, solv_in_ref)
			xg.connectToGroup(forceID, force_our_ref, group)

	
	# Finally layout nodes in a readable manner and show whole graph for clarity
	xg.uniformLayout()
	xg.viewAllNodes()

# Run Script
add_sph_adhesion()
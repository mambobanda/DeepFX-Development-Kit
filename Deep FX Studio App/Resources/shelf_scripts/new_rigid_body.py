from xglobal import println
from xglobal import messageBox
import xgraph as xg

"""
Create a new rigid body network
- This script creates a new rigid body network for a selected object
It makes certain assumptions about the existing dynamics network and
tries to incorprate the new network into an existing one, if not
it will ask the user if they want to create a new one.
"""


def create_rigid_body(selection):

	# Only objects with meshdata or proxy mesh data will be turned
	# into dynamics objects.
	if xg.hasMeshData(selection) or xg.hasProxyMeshData(selection):

		all_ = xg.findAllNodes()
		bulletSolverID = 0
		bulletSolverExists = False
		linkToExistingSolver = False


		# Create and setup rigid network
		rigid_bodyID = xg.createNode("2F2X252X_DFX_BULLET_RIGID_OBJECT")
		solver = 0
		cache = 0
		group = 0
		frameIinfo = 0

		# If selected object is standard object item connect the basics
		if xg.getNodeUniqueName(selection) == "2F2X252X_DFX_OBJECT_ITEM":
			out_sel_pos = xg.getPropertyID(selection, "out_position")
			out_sel_rot = xg.getPropertyID(selection, "out_rotation")
			out_sel_scl = xg.getPropertyID(selection, "out_scale")

			in_sel_pos = xg.getPropertyID(rigid_bodyID, "position")
			in_sel_rot = xg.getPropertyID(rigid_bodyID, "rotation")
			in_sel_scl = xg.getPropertyID(rigid_bodyID, "scale")

			# Connect outputs to inputs 
			xg.connectProperties(selection, out_sel_pos, rigid_bodyID, in_sel_pos)
			xg.connectProperties(selection, out_sel_rot, rigid_bodyID, in_sel_rot)
			xg.connectProperties(selection, out_sel_scl, rigid_bodyID, in_sel_scl)


		# Connect references if selection has them (@ref)
		if xg.hasProperty(selection, "@ref"):
			out_ref = xg.getPropertyID(selection, "@ref")
			in_ref = xg.getPropertyID(rigid_bodyID, "in_ref")

			xg.connectProperties(selection, out_ref, rigid_bodyID, in_ref)

		xg.uniformLayout()
		xg.viewAllNodes()
		
		# Search for exising dynamics solvers
		for uid in all_:
			if xg.getNodeUniqueName(uid) == "2F2X252X_DFX_BULLET_SOLVER":
				bulletSolverExists = True
				bulletSolverID = uid
				break

		# Let the user decide what path to take
		if bulletSolverExists:
			rt = messageBox("Found Existing Rigid Solver", "Link rigid object to exiting solver?", 400, "Yes_No")
			if rt:
				linkToExistingSolver = True
		else:
			rt = messageBox("Create New Rigid Solver", "Link rigid object to new solver?", 400, "Yes_No")
			if rt:
				solver = xg.createNode("2F2X252X_DFX_BULLET_SOLVER")
				cache  = xg.createViewNode("2F2X252X_DFX_BULLET_POINT_CACHE")
				group  = xg.createNode("2F2X252X_DFX_GROUP_NODE")
				frameinfo = xg.createNode("2F2X252X_DFX_TIME_INFO")

				solv_out_ref = xg.getPropertyID(solver, "@ref")
				solv_in_ref  = xg.getPropertyID(solver, "in_ref")
				solv_in_trigger  = xg.getPropertyID(solver, "trigger")
				cach_in_ref  = xg.getPropertyID(cache, "in_ref")
				cach_in_index = xg.getPropertyID(cache, "in_index")
				frame_out_current = xg.getPropertyID(frameIinfo, "current_frame")

				xg.connectProperties(frameinfo, frame_out_current, solver, solv_in_trigger)
				xg.connectProperties(frameinfo, frame_out_current, cache, cach_in_index)

				xg.connectProperties(group, 0, solver, solv_in_ref)

				xg.connectProperties(solver, solv_out_ref, cache, cach_in_ref)

				bulletSolverID = solver
				linkToExistingSolver = True
	
		# Handle solver stuff here, create or link to existing one
		if linkToExistingSolver:
			solv_in_ref = xg.getPropertyID(bulletSolverID, "in_ref")

			if xg.propertyHasParent(bulletSolverID, solv_in_ref):
				in_ref_parent = xg.getPropertyParent(bulletSolverID, solv_in_ref)

				if xg.getNodeUniqueName(in_ref_parent) == "2F2X252X_DFX_GROUP_NODE":
					rigid_our_ref = xg.getPropertyID(rigid_bodyID, "@ref")
					xg.connectToGroup(rigid_bodyID, rigid_our_ref, in_ref_parent)
			else:
				group = xg.createNode("2F2X252X_DFX_GROUP_NODE")
				rigid_our_ref = xg.getPropertyID(rigid_bodyID, "@ref")
				xg.connectProperties(group, 0, bulletSolverID, solv_in_ref)
				xg.connectToGroup(rigid_bodyID, rigid_our_ref, group)

		
		# Finally layout nodes in a readable manner and show whole graph for clarity
		xg.uniformLayout()
		xg.viewAllNodes()

	else:
		messageBox("No Mesh Data Found.", "Please select an object with mesh / proxy mesh data.", 400, "")


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
		create_rigid_body(selection)
else:
	messageBox("Nothing Selected.", "Please select an object first to turn it into a rigid object.", 400, "")
	
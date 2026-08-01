from xglobal import println
from xglobal import messageBox
import xgraph as xg

"""
Create a new voronoi fracture network
- This script creates a voronoi fracture network for a selected object.
"""

def voronoi_fracture(selection):
	# Only objects with meshdata or proxy mesh data will be fractured.
	if xg.hasMeshData(selection) or xg.hasProxyMeshData(selection):
		voronoiID = xg.createNode("2F2X252X_DFX_VORONOI_FRACTURE_NODE")
		exViewID  = xg.createViewNode("2F2X252X_DFX_EXPLODE_VIEW_NODE")

		if xg.getNodeUniqueName(selection) == "2F2X252X_DFX_OBJECT_ITEM":
			out_sel_pos = xg.getPropertyID(selection, "out_position")
			out_sel_rot = xg.getPropertyID(selection, "out_rotation")
			out_sel_scl = xg.getPropertyID(selection, "out_scale")

			in_sel_pos = xg.getPropertyID(voronoiID, "in_position")
			in_sel_rot = xg.getPropertyID(voronoiID, "in_rotation")
			in_sel_scl = xg.getPropertyID(voronoiID, "in_scale")

			# Connect outputs to inputs 
			xg.connectProperties(selection, out_sel_pos, voronoiID, in_sel_pos)
			xg.connectProperties(selection, out_sel_rot, voronoiID, in_sel_rot)
			xg.connectProperties(selection, out_sel_scl, voronoiID, in_sel_scl)

		# Connect references if selection has them (@ref)
		if xg.hasProperty(selection, "@ref"):
			sel_out_ref = xg.getPropertyID(selection, "@ref")
			vor_in_ref  = xg.getPropertyID(voronoiID, "in_ref")
			vor_out_ref = xg.getPropertyID(voronoiID, "@ref")
			view_in_ref = xg.getPropertyID(exViewID, "in_ref")

			xg.connectProperties(selection, sel_out_ref, voronoiID, vor_in_ref)
			xg.connectProperties(voronoiID, vor_out_ref, exViewID, view_in_ref)

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
		voronoi_fracture(selection)
else:
	messageBox("Nothing Selected.", "Please select an object first to apply voronoi fracturing.", 400, "")
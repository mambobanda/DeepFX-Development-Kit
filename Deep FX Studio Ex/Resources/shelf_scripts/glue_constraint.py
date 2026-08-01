from xglobal import println
from xglobal import messageBox
from xglobal import getFloatValue
import xgraph as xg



def add_glue_constraint(selection):
	if xg.getNodeUniqueName(selection) == "2F2X252X_DFX_BULLET_RIGID_OBJECT":

		all_ = xg.findAllNodes()
		bulletSolverID = 0
		bulletSolverExists = False

		constraint_id = xg.createNode("2F2X252X_DFX_BULLET_GLUE_CONSTRAINT")

		out_ref = xg.getPropertyID(selection, "@ref")
		con_out_ref = xg.getPropertyID(constraint_id, "@ref")
		con_in_ref  = xg.getPropertyID(constraint_id, "in_ref")

		xg.connectProperties(selection, out_ref, constraint_id, con_in_ref)

		for uid in all_:
			if xg.getNodeUniqueName(uid) == "2F2X252X_DFX_BULLET_SOLVER":
				bulletSolverExists = True
				bulletSolverID = uid
				break

		if bulletSolverExists:
			solv_in_ref = xg.getPropertyID(bulletSolverID, "in_ref")

			if xg.propertyHasParent(bulletSolverID, solv_in_ref):
				in_ref_parent = xg.getPropertyParent(bulletSolverID, solv_in_ref)

				if xg.getNodeUniqueName(in_ref_parent) == "2F2X252X_DFX_GROUP_NODE":
					xg.connectToGroup(constraint_id, con_out_ref, in_ref_parent)
	
		xg.uniformLayout()
		xg.viewAllNodes()
	else:
		messageBox("Bullet rigid object not found.", "Please select one or more bullet rigid objects.", 400, "")

# Check viewport selection
hasSelection = xg.viewHasSelection()
hasGraphSelection = xg.graphHasSelection()
selectionList = []

# Add items selected in viewport to list
if hasSelection:
	selection = xg.getViewSelection()
	if xg.getNodeUniqueName(selection) == "2F2X252X_DFX_BULLET_RIGID_OBJECT":
		selectionList.append(selection)
	else:
		if xg.hasChildren(selection):
			children = xg.getAllChildren(selection)

			for child in children:
				if xg.getNodeUniqueName(child) == "2F2X252X_DFX_BULLET_RIGID_OBJECT":
					selectionList.append(child)

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
		add_glue_constraint(selection)
else:
	messageBox("Nothing Selected.", "Please select a rigid object first", 400, "")
from xglobal import println
from xglobal import messageBox
import xgraph as xg

"""
Turn a bullet rigid body into a static object
- This script turns a list of bullet selected items into static objects.
"""

def make_static(selection):

	if xg.getNodeUniqueName(selection) == "2F2X252X_DFX_BULLET_RIGID_OBJECT":
		xg.setFloat(selection, "mass", 0)

		println("Bullet Rigid Object to Static Done...")
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
		make_static(selection)
else:
	messageBox("Nothing Selected.", "Please select an object first to turn it into a static rigid object.", 400, "")
	
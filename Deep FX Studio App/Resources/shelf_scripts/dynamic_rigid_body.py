from xglobal import println
from xglobal import messageBox
from xglobal import getFloatValue
import xgraph as xg

"""
Turn a bullet rigid body into a dynamic object
- This script turns a list of bullet selected items into dynamic objects.
"""

def make_dynamic(selection):

	if xg.getNodeUniqueName(selection) == "2F2X252X_DFX_BULLET_RIGID_OBJECT":
		value = getFloatValue("Rigid Body Mass", "mass", 10.0)

		if(value <= 0):
			value = 1.0

		xg.setFloat(selection, "mass", value)

		println("Bullet Rigid Object to Dynamic Done...")
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
		make_dynamic(selection)
else:
	messageBox("Nothing Selected.", "Please select an object first to turn it into a dynamic rigid object.", 400, "")
	
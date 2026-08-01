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


#include "DfxQHierarchicalLayoutAlpha.h"

DfxQHierarchicalLayoutAlpha::DfxQHierarchicalLayoutAlpha() {
	iterations = 1;
	ylayoutType = YLayout::UNIFORM_LAYOUT;
	ySpacing = 100.f;
	layerOffset = 100.f;
}

DfxQHierarchicalLayoutAlpha::~DfxQHierarchicalLayoutAlpha() {

}

void DfxQHierarchicalLayoutAlpha::initExtras() {

	for (auto const& it : gnodes) {
		auto extraPtr = new GNodeExtra;

		extraPtr->dummy = false;
		extraPtr->gnode = it.second;
		extraPtr->layer = -1;

		std::pair<size_t, GNodeExtra*> p(extraPtr->gnode.baseNode->getEntity()->getInstanceID(), extraPtr);
		extras.insert(p);
	}
}

void DfxQHierarchicalLayoutAlpha::setLayerOffset(float offset) {
	layerOffset = offset;
}

float DfxQHierarchicalLayoutAlpha::getLayerOffset() {
	return layerOffset;
}

void DfxQHierarchicalLayoutAlpha::setReductionCrossingIterations(size_t val) {
	iterations = val;
}

size_t DfxQHierarchicalLayoutAlpha::getReductionCrossingIterations() {
	return iterations;
}

void DfxQHierarchicalLayoutAlpha::setYLayoutType(YLayout l) {
	ylayoutType = l;
}

DfxQHierarchicalLayoutAlpha::YLayout DfxQHierarchicalLayoutAlpha::getYLayoutType() {
	return ylayoutType;
}

void DfxQHierarchicalLayoutAlpha::setYLayout(float val) {
	ySpacing = val;
}

float DfxQHierarchicalLayoutAlpha::getYLayoutSpacing() {
	return ySpacing;
}

void DfxQHierarchicalLayoutAlpha::assignLayers() {
	
	std::queue<GNodeExtra*> queue;;

	for (auto const& it : extras) {

		if (!it.second->gnode.baseNode->getEntity()->hasParent()) {
			it.second->layer = 0;
			queue.push(it.second);
		}
	}

	while (!queue.empty())
	{
		GNodeExtra *tmp_ent = queue.front();
		queue.pop();

		size_t current_layer = extras.find(tmp_ent->gnode.baseNode->getEntity()->getInstanceID())->second->layer;

		for (auto ent : tmp_ent->gnode.baseNode->getEntity()->getAllChildren()) {

			for (auto const& it : extras) {

				if (it.second->gnode.baseNode->getEntity()->getInstanceID() == ent->getInstanceID()) {

					if (!ent->isHidden()) {
						it.second->layer = (current_layer + 1);
						tmp_ent->children.push_back(it.second);
						queue.push(it.second);
					}

				}
			}
		}
	}
}

void DfxQHierarchicalLayoutAlpha::insertDummyNodes() {

	std::vector<GNodeExtra*> tempExtras;
	size_t maxID = -1;

	for (auto const& it : extras) {
		maxID = std::max(maxID, it.second->gnode.baseNode->getEntity()->getInstanceID());
		tempExtras.push_back(it.second);
	}

	maxID++;
	extras.clear();

	for (auto _parent = tempExtras.begin(); _parent != tempExtras.end(); _parent++) {

		GNodeExtra *parent = (*_parent);
		auto child = parent->children.begin();
		std::unordered_map<size_t, bool> mapCheck;

		while (child != parent->children.end()) {
			
			size_t uid = 0;

			if ((*child)->dummy) {
				size_t uid = (*child)->dummyID;
			}
			else {
				size_t uid = (*child)->gnode.baseNode->getEntity()->getInstanceID();
			}
			auto check = mapCheck.find(uid);

			if ((*child)->layer > (parent->layer + 1) && check == mapCheck.end()) {

				auto extraPtr = new GNodeExtra;

				extraPtr->dummy = true;
				extraPtr->dummyID = maxID++;
				extraPtr->layer = (parent->layer + 1);

				std::pair<size_t, bool> p(uid, true);
				mapCheck.insert(p);

				extraPtr->children.push_back((*child));
				parent->children.insert(child, extraPtr);
				_parent = tempExtras.insert(_parent, extraPtr);

				child = parent->children.begin();
			}
			else {

				++child;
			}
		}

		for (auto child = parent->children.begin(); child != parent->children.end();) {

			size_t uid = 0;

			if ((*child)->dummy) {
				size_t uid = (*child)->dummyID;
			}
			else {
				size_t uid = (*child)->gnode.baseNode->getEntity()->getInstanceID();
			}
			auto check = mapCheck.find(uid);

			if (check != mapCheck.end()) {
				child = parent->children.erase(child);
			}
			else {
				++child;
			}
		}
	}

	for (auto n : tempExtras) {
		if (n->dummy) {
			std::pair<size_t, GNodeExtra*> p(n->dummyID, n);
			extras.insert(p);
		}
		else {
			std::pair<size_t, GNodeExtra*> p(n->gnode.baseNode->getEntity()->getInstanceID(), n);
			extras.insert(p);
		}
	}
}

void DfxQHierarchicalLayoutAlpha::setXCoordinate() {

	size_t counter = 0;
	size_t maxLayers = 0;
	bool layerFound = true;

	/*for (auto const& it : extras) {
		maxLayers = std::max(maxLayers, it.second->layer);
	}

	for (size_t inx = 0; inx < (maxLayers + 1); inx++) {
		_Layer *lptr = new _Layer;
		lptr->index = counter;
		lptr->max_width = 0;

		for (auto const& it : extras) {

			if (it.second->layer == inx) {
			}
		}
	}*/
	while (layerFound) {
		layerFound = false;
		_Layer *lptr = nullptr;

		for (auto const& it : extras) {

			if (it.second->layer == counter) {
				layerFound = true;

				if (lptr == nullptr) {
					lptr = new _Layer;
					lptr->index = counter;
					lptr->max_width = 0;
					layers.push_back(lptr);
				}

				lptr->nodes.push_back(it.second);
				
			}

		}

		counter++;
	}

	for (auto l : layers) {

		for (auto gn : l->nodes) {
			if (!gn->dummy) {
				float w = gn->gnode.baseNode->childrenBoundingRect().width();
				l->max_width = std::max(l->max_width, w);
			}
		}
	}

	size_t base = 0;
	for (auto layer : layers) {
		float max = layer->max_width;
		
		for (auto gn : layer->nodes) {
			if (!gn->dummy) {
				float w = gn->gnode.baseNode->childrenBoundingRect().width();
				gn->gnode.new_posx = base + (max - w) / 2.f;
			}
		}
			
		base = base + max + layerOffset;
		//base += 100.f;
	}

	/*for (auto layer : layers) {
		delete layer;
	}*/
}

void DfxQHierarchicalLayoutAlpha::crossingReduction() {

	if (layers.size() > 0) {

		//Order first layer according to index in array
		for (size_t inx = 0; inx < layers[0]->nodes.size(); inx++) {
			layers[0]->nodes[inx]->order = (float)inx;
		}

		for (size_t it = 0; it < iterations; it++) {
			// Down Sweep
			for (size_t inx = 1; inx < layers.size(); inx++) {

				for (auto extra: layers[inx]->nodes) {

					size_t indexSum = 0;
					size_t count = 0;

					for (auto extra_other : layers[inx - 1]->nodes) {

						for (auto extra_other_child : extra_other->children) {

							if (extra_other_child->dummy && extra->dummy) {

								if (extra_other_child->dummyID == extra->dummyID) {

									indexSum += extra_other->order;
									count++;
								}
							}
							else if (!extra_other_child->dummy && !extra->dummy) {

								if (extra_other_child->gnode.baseNode->getEntity()->getInstanceID() ==
									extra->gnode.baseNode->getEntity()->getInstanceID()) {

									indexSum += extra_other->order;
									count++;
								}
							}
						}
					}

					if (count == 0) count = 1;

					extra->order = indexSum / count;
				}

				std::sort(layers[inx]->nodes.begin(), layers[inx]->nodes.end(),
					[](GNodeExtra *a, GNodeExtra *b) -> bool {
					return (a->order < b->order);
				});
			}

			// Up Sweep
			for (int inx = (layers.size() - 2); inx >= 0; inx--) {

				for (auto extra : layers[inx]->nodes) {

					size_t indexSum = 0;
					size_t count = 0;

					for (auto extra_other : layers[inx + 1]->nodes) {


						for (auto extra_other_child : extra_other->children) {

							if (extra_other_child->dummy && extra->dummy) {

								if(extra_other_child->dummyID == extra->dummyID) {

									indexSum += extra_other->order;
									count++;
								}
							}
							else if(!extra_other_child->dummy && !extra->dummy){

								if (extra_other_child->gnode.baseNode->getEntity()->getInstanceID() ==
									extra->gnode.baseNode->getEntity()->getInstanceID()) {

									indexSum += extra_other->order;
									count++;
								}
							}
						}
					}

					if (count == 0) count = 1;

					extra->order = indexSum / count;
				}

				std::sort(layers[inx]->nodes.begin(), layers[inx]->nodes.end(),
					[](GNodeExtra *a, GNodeExtra *b) -> bool {
					return (a->order < b->order);
				});
			}
		}
	}
}

void DfxQHierarchicalLayoutAlpha::uniformYLayout() {

	for (auto l : layers) {

		float ypos = ySpacing;

		for (auto m : l->nodes) {

			if (!m->dummy) {

				m->gnode.new_posy = ypos;
				ypos += ySpacing + m->gnode.baseNode->childrenBoundingRect().height();
			}
		}
	}
}

void DfxQHierarchicalLayoutAlpha::dagYLayout() {

}

void DfxQHierarchicalLayoutAlpha::rubberbandYLayout() {

}

void DfxQHierarchicalLayoutAlpha::assignYCoordinates() {

	if (ylayoutType == YLayout::UNIFORM_LAYOUT) {
		uniformYLayout();
	}
	else if (ylayoutType == YLayout::RUBBERBAND_LAYOUT) {
		rubberbandYLayout();
	}
	else if (ylayoutType == YLayout::DAG_LAYOUT) {
		dagYLayout();
	}

}

void DfxQHierarchicalLayoutAlpha::resync() {

	gnodes.clear();

	for (auto const& it : extras) {

		if (!it.second->dummy) {
			std::pair<size_t, GNode> p(it.second->gnode.baseNode->getEntity()->getInstanceID(), it.second->gnode);
			gnodes.insert(p);
		}
	}
}

void DfxQHierarchicalLayoutAlpha::solveLayout() {

	initExtras();
	assignLayers();
	insertDummyNodes();
	setXCoordinate();
	crossingReduction();
	assignYCoordinates();
	resync();

}
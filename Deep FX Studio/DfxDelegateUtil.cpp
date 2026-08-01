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

#include "DfxDelegateUtil.h"

namespace DfxDelegate {

	std::vector<DfxEntity*> findRoots(DfxEntity *delegator) {
		std::vector<DfxEntity*> roots;

		auto all = delegator->getAllDelegatees();

		for (auto a : all) {

			if (a->hasParent()) {
				bool hasParent = false;

				for (auto b : all) {
					if (b->hasChild(a)) {
						hasParent = true;
						break;
					}
				}

				if (!hasParent) {
					roots.push_back(a);
				}
			}
			else {
				roots.push_back(a);
			}
		}

		return roots;
	}

	std::vector<DfxEntity*> findTips(DfxEntity *delegator) {
		std::vector<DfxEntity*> tips;

		auto all = delegator->getAllDelegatees();

		for (auto a : all) {
			bool hasChild = false;

			for (auto b : all) {
				if (a->hasChild(b)) {
					hasChild = true;
					break;
				}
			}

			if (!hasChild) {
				tips.push_back(a);
			}
		}

		return tips;
	}

	bool isTip(DfxEntity *ent, std::vector<DfxEntity*> tips) {

		for (auto tip : tips) {
			if (ent->getInstanceID() == tip->getInstanceID()) {
				return true;
			}
		}

		return false;
	}

	void executeDelegator(DfxEntity *ent, bool executeRootChildren) {

		std::vector<DfxEntity*> rootChildren;
		auto roots = findRoots(ent);
		
		for (auto r : roots) {
			for (auto c : r->getAllChildren()) {
				rootChildren.push_back(c);
			}
		}

		if (roots.size() > 0) {

			auto tips = findTips(ent);
			std::queue<DfxEntity*> queue;

			if (executeRootChildren) {
				for (auto c : rootChildren) {
					c->makeDirty();
					queue.push(c);
				};
			}
			else {
				for (auto r : roots) {
					r->makeDirty();
					queue.push(r);
				};
			}

			while (!queue.empty()) {

				DfxEntity *tmp_ent = queue.front();
				queue.pop();

				if (tmp_ent->parentsAreClean()) {

					if (!tmp_ent->getDataInstance()->isClean()) {
						tmp_ent->compute();
					}

					if (!isTip(tmp_ent, tips)) {
						if (tmp_ent->hasChildren()) {

							for (auto ent : tmp_ent->getAllChildren()) {

								if (!ent->isHidden()) {

									queue.push(ent);
								}
							}
						}
					}
				}
			}
		}
	}
}
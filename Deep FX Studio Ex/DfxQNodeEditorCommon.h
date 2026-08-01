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
 /**
	* @file ...h
	*
	* @author Mambo Banda
	*
	*/

#pragma once

class DfxQMaterialBaseNode;
class DfxQMaterialNode;
class DfxQMaterialNodeSlot;
class DfxQMaterialNodeProperty;
class DfxQMaterialPropertyLink;

class DfxQListenerNode; 
class DfxQListenerItem; 
class DfxQListenerItemDeleteButton; 
class DfxQListenerItemEditButton;
class DfxQListenerItemOrderButton;
class DfxQListenerMenuButton;

class DfxQFlowBaseNode;
class DfxQFlowCollection;
class DfxQFlowView;
class DfxQFlowStartNode;
class DfxQFlowProcessNode;
class DfxQFlowDecisionNode;
class DfxQFlowBreakNode;
class DfxQFlowConnector;
class DfxQFlowLink;

class DfxQComputeBaseNode;
class DfxQComputeNode;
class DfxQComputeCompoundNode;
class DfxQComputeCompoundInNode;
class DfxQComputeCompoundOutNode;
class DfxQComputeNodeSlot;
class DfxQComputeNodeProperty;
class DfxQComputePropertyLink;

class DfxFluentNodeEditor;

namespace QFluentViewType {
	enum { LISTENER = 0, FLOW = 1, NODE = 2, COMPOUND = 3 };
};
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
	* @file ....cpp
	*
	* @author Mambo Banda
	*
	*/

#include "DfxQComputeSlotColors.h"



QColor DfxQComputeSlotColors::slotColor(unsigned int type) {
/*
	QColor(0, 0, 128);
	QColor(170, 136, 0);
	QColor(136, 170, 0);
	QColor(103, 108, 83);
	QColor(145, 111, 111);
	QColor(138, 111, 145);
	QColor(153, 85, 255);
	QColor(255, 0, 102);
	QColor(229, 255, 128);
	QColor(26, 26, 26);
*/
	switch (type)
	{
	case fl::type::FLOAT:
		return QColor(128, 0, 0);
		break;
	case fl::type::FLOAT_3:
		return QColor(255, 255, 0);
		break;
	case fl::type::SIZE_T:
		return QColor(0, 255, 0);
		break;
	case fl::type::BOOL:
		return QColor(0, 0, 255);
		break;
	case fl::type::STD_STRING:
		return QColor(255, 0, 255);
		break;
	case fl::type::SHAPE:
		return QColor(0, 128, 128);
		break;
	case fl::type::ATTRIBUTE_PATH:
		return QColor(255, 102, 0);
		break;
	case fl::type::ANY:
		return QColor(255, 255, 255);
		break;
	default:
		break;
	}

	return QColor();
}
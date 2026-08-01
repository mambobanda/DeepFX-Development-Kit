/*
* The Mailo Today Solutions Software License
*
* Copyright 2018 Mailo Today Solutions.
*
* No Permission is granted, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software.
*/

#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(DEEPFXSTUDIOOPENVDBPLUGIN_LIB)
#  define DEEPFXSTUDIOOPENVDBPLUGIN_EXPORT Q_DECL_EXPORT
# else
#  define DEEPFXSTUDIOOPENVDBPLUGIN_EXPORT Q_DECL_IMPORT
# endif
#else
# define DEEPFXSTUDIOOPENVDBPLUGIN_EXPORT
#endif

/*
 * The Mailo Today Solutions Software License
 *
 * Copyright 2017 Mailo Today Solutions.
 *
 * No Permission is granted, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software.
 */

#pragma once

#include <CL\cl.h>


class KernelInfo{
public:
	size_t KERNEL_WORK_GROUP_SIZE;
	size_t KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE;
	cl_ulong KERNEL_LOCAL_MEM_SIZE;
	cl_ulong KERNEL_PRIVATE_MEM_SIZE;
	size_t KERNEL_COMPILE_WORK_GROUP_SIZE[3];
};
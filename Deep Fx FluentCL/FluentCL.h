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

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#include <CL\cl.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include "DeviceProfile.h"
#include "ComputeBuffer.h"
#include "ComputeKernel.h"
#include "ComputeBuffer.h"
#include "ContextPolicy.h"
#include "Constants.h"
#include "PlatformInfo.h"
#include "PlatformState.h"
#include "Context.h"
#include "KernelInfo.h"


class FluentCL{
private:
	static int platformState;
	Context context;
	std::string compilationOutput;
	vector<ComputeKernel> kernelMap; 

	int selectedDeviceType;
	int selectedDeviceIndex;

	int lastErrorCode;

	void addPlatformDevice(cl_device_id &, std::vector<DeviceInfo> &, string);
	int  buildProgram(const char ** , int , const size_t *, double );
	std::string buildCompilationLog(cl_program);

	void waitAndReleaseDevice(cl_event event);
#ifdef _WIN32
	wchar_t *_charToLPCWSTR(const char* arr);
	bool _fileExists(const char* path);
#endif

public:
	static bool infoInitialised;
	static std::vector<PlatformInfo> clPlatforms;
public:
	FluentCL();
	void initInfo();
	int  getPlatformState();
	std::string compilePlatformInfo();
	Context     &getContext();
	void clearContext();
	int  createContext(ContextPolicy );
	void setContext(Context context);
	vector<Context> createAllContexts(bool enableProfile=false);

	int loadKernelsFromSource(std::string path, double clversion);
	int loadKernelsFromSource(vector<std::string> paths, double clversion);
	int loadKernelsFromSourcePtr(const char** source, int size, double clversion);
	int loadKernelsFromSourceRaw(const char** source, double clversion);

	ComputeBuffer createReadWriteBuffer(void* data,  size_t size);
	ComputeBuffer createReadWriteBuffer(size_t size);
	ComputeBuffer createReadOnlyBuffer(void* data,  size_t size);
	ComputeBuffer createWriteOnlyBuffer(size_t size);
	ComputeBuffer createReadWriteLocalBuffer(void* data,  size_t size);
	ComputeBuffer createReadOnlyLocalBuffer(void* data,  size_t size);

	int executeKernel(string name);
	int executeKernelParallel1D(string name, int workitems, int localsize, bool wait=false);
	int executeKernelParallel2D(string name, size_t *workitems, size_t *localsize, bool wait=false);

	int executeKernelAndProfile(string name, cl_ulong &timing);
	int executeKernelParallel1DAndProfile(string name, int workitems, int localsize, cl_ulong &timing, bool wait=false);
	int executeKernelParallel2DAndProfile(string name, size_t *workitems, size_t *localsize, cl_ulong &timing, bool wait=false);

	int setKernelArgument(string name, cl_int inx, ComputeBuffer cb);
	int setKernelArgument(string name, cl_int inx, size_t size, void* data);

	int readComputeBuffer(ComputeBuffer cb, size_t size, void *data);
	int mapReadComputeBuffer(ComputeBuffer cb, size_t size, void *data);
	int writeToComputeBuffer(int, int, ComputeBuffer);

	int getLastErrorCode();

	void setSelectedDevice(int type, int index);

	vector<DeviceProfile> getDeviceProfiles(vector<Context> contexts);

	std::string getCompilationOutput();

	KernelInfo getKernelInfo(string name);
	
	int	getNumberOfPlatforms();

	std::pair<int, int> getMinMaxGroupSize(vector<string> kernels);

	int calculateOptimalGlobalWorksize(std::pair<int, int> minMax, int buffersize);

	int calculateOptimalLocalWorksize(std::pair<int, int> minMax, int buffersize);
};
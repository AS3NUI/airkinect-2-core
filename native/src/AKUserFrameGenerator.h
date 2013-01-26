/*
 * Copyright 2012 AS3NUI
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#ifndef KinectExtension_AKUserFrameGenerator_h
#define KinectExtension_AKUserFrameGenerator_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include <stdlib.h>
#include "AKUtilityFunctions.h"
#include "Data/AKUserFrame.h"

class AKUserFrameGenerator
{
public:

	AKUserFrameGenerator();
	virtual ~AKUserFrameGenerator();

	virtual void setRGBTargetMirrored(bool rgbTargetMirrored);
	virtual void setRGBTargetSize(int width, int height);
	virtual void setRGBSourceSize(int width, int height);

	virtual void setDepthTargetMirrored(bool depthTargetMirrored);
	virtual void setDepthTargetSize(int width, int height);
	virtual void setDepthSourceSize(int width, int height);

	virtual void setSkeletonTrackingEnabled(bool skeletonTrackingEnabled);
	virtual void setSkeletonMirrored(bool skeletonMirrored);

	virtual int getMaxUsers();
	virtual void generateUserFrame();
	virtual FREObject getFREObject();

protected:
	
	const char* _asUserFrameClass;
	const char* _asUserClass;
	const char* _asJointClass;
	const char* _asBoneClass;
	const char* _framework;

	bool _rgbTargetMirrored;
	bool _depthTargetMirrored;

	int _rgbSourceWidth;
	int _rgbSourceHeight;

	int _rgbTargetWidth;
	int _rgbTargetHeight;

	int _depthTargetWidth;
	int _depthTargetHeight;

	int _depthSourceWidth;
	int _depthSourceHeight;

	bool _skeletonTrackingEnabled;
	bool _skeletonMirrored;

	int _numJoints;
	int _numBones;
	int _maxSkeletons;
	
	const char** _jointNames;
	const char** _boneNames;

	AKUserFrame* _userFrame;
	virtual void allocateUserFrame();
	virtual void deallocateUserFrame();

	virtual FREObject freGetSkeletonJointNameIndices();
	virtual FREObject freGetSkeletonJointNames();
	virtual FREObject freGetSkeletonBoneNameIndices();
	virtual FREObject freGetSkeletonBoneNames();
};

#endif
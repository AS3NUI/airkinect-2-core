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
#ifndef KinectExtension_AKSkeletonBone_h
#define KinectExtension_AKSkeletonBone_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include "AKBasicStructs.h"

#ifndef _AKSkeletonBone_
#define _AKSkeletonBone_
typedef struct _AKSkeletonBone
{
	AKOrientation orientation;

	int boneNameIndex;
	int startJointNameIndex;
	int endJointNameIndex;
	int parentBoneNameIndex;

	_AKSkeletonBone()
	{
		boneNameIndex = -1;
		startJointNameIndex = -1;
		endJointNameIndex = -1;
		parentBoneNameIndex = -1;
	};

} AKSkeletonBone;
#endif _AKSkeletonBone_

#endif
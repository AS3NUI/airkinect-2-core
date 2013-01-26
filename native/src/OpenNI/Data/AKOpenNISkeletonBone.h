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
#ifndef KinectExtension_AKOpenNISkeletonBone_h
#define KinectExtension_AKOpenNISkeletonBone_h

#include "../../ExtensionConfig.h"

#ifdef AIRKINECT_TARGET_OPENNI

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include "../../Data/AKSkeletonBone.h"
#include "../../Data/AKBasicStructs.h"

#ifndef _AKOpenNISkeletonBone_
#define _AKOpenNISkeletonBone_

typedef enum AkOpenniBoneIndex
{
	AK_OPENNI_BONE_INDEX_NECK = 0,
	AK_OPENNI_BONE_INDEX_LEFT_UPPER_ARM = 1,
	AK_OPENNI_BONE_INDEX_LEFT_LOWER_ARM = 2,
	AK_OPENNI_BONE_INDEX_RIGHT_UPPER_ARM = 3,
	AK_OPENNI_BONE_INDEX_RIGHT_LOWER_ARM = 4,

	AK_OPENNI_BONE_INDEX_SPINE = 5,
	AK_OPENNI_BONE_INDEX_LEFT_UPPER_LEG = 6,
	AK_OPENNI_BONE_INDEX_LEFT_LOWER_LEG = 7,
	AK_OPENNI_BONE_INDEX_RIGHT_UPPER_LEG = 8,
	AK_OPENNI_BONE_INDEX_RIGHT_LOWER_LEG = 9
};

typedef struct _AKOpenNISkeletonBone
{
	AKSkeletonBone* skeletonBone;

} AKOpenNISkeletonBone;
#endif _AKOpenNISkeletonBone_

#endif

#endif
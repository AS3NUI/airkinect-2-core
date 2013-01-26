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
#ifndef KinectExtension_AKMSSDKSkeletonBone_h
#define KinectExtension_AKMSSDKSkeletonBone_h

#include "../../ExtensionConfig.h"

#ifdef AIRKINECT_TARGET_MSSDK

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include "../../Data/AKSkeletonBone.h"
#include "../../Data/AKBasicStructs.h"

#ifndef _AKMSSDKSkeletonBone_
#define _AKMSSDKSkeletonBone_

typedef enum AkMssdkRegularBoneIndex
{
	AK_MSSDK_REGULAR_BONE_INDEX_NECK = 0,

	AK_MSSDK_REGULAR_BONE_INDEX_LEFT_COLLAR = 1,
	AK_MSSDK_REGULAR_BONE_INDEX_LEFT_UPPER_ARM = 2,
	AK_MSSDK_REGULAR_BONE_INDEX_LEFT_LOWER_ARM = 3,
	AK_MSSDK_REGULAR_BONE_INDEX_LEFT_HAND = 4,

	AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_COLLAR = 5,
	AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_UPPER_ARM = 6,
	AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_LOWER_ARM = 7,
	AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_HAND = 8,

	AK_MSSDK_REGULAR_BONE_INDEX_SPINE = 9,
	AK_MSSDK_REGULAR_BONE_INDEX_LOWER_SPINE = 10,

	AK_MSSDK_REGULAR_BONE_INDEX_LEFT_HIP = 11,
	AK_MSSDK_REGULAR_BONE_INDEX_LEFT_UPPER_LEG = 12,
	AK_MSSDK_REGULAR_BONE_INDEX_LEFT_LOWER_LEG = 13,
	AK_MSSDK_REGULAR_BONE_INDEX_LEFT_FOOT = 14,

	AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_HIP = 15,
	AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_UPPER_LEG = 16,
	AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_LOWER_LEG = 17,
	AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_FOOT = 18
};

typedef enum AkMssdkSeatedBoneIndex
{
	AK_MSSDK_SEATED_BONE_INDEX_NECK = 0,

	AK_MSSDK_SEATED_BONE_INDEX_LEFT_COLLAR = 1,
	AK_MSSDK_SEATED_BONE_INDEX_LEFT_UPPER_ARM = 2,
	AK_MSSDK_SEATED_BONE_INDEX_LEFT_LOWER_ARM = 3,
	AK_MSSDK_SEATED_BONE_INDEX_LEFT_HAND = 4,

	AK_MSSDK_SEATED_BONE_INDEX_RIGHT_COLLAR = 5,
	AK_MSSDK_SEATED_BONE_INDEX_RIGHT_UPPER_ARM = 6,
	AK_MSSDK_SEATED_BONE_INDEX_RIGHT_LOWER_ARM = 7,
	AK_MSSDK_SEATED_BONE_INDEX_RIGHT_HAND = 8
};

typedef struct _AKMSSDKSkeletonBone
{
	AKSkeletonBone* skeletonBone;

} AKMSSDKSkeletonBone;
#endif _AKMSSDKSkeletonBone_

#endif

#endif
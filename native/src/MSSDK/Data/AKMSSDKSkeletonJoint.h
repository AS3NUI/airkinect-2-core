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
#ifndef KinectExtension_AKMSSDKSkeletonJoint_h
#define KinectExtension_AKMSSDKSkeletonJoint_h

#include "../../ExtensionConfig.h"

#ifdef AIRKINECT_TARGET_MSSDK

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include "../../Data/AKSkeletonJoint.h"
#include "../../Data/AKBasicStructs.h"

#ifndef _AKMSSDKSkeletonJoint_
#define _AKMSSDKSkeletonJoint_

typedef enum AkMssdkRegularJointIndex
{
	AK_MSSDK_REGULAR_JOINT_INDEX_WAIST = 0,
	AK_MSSDK_REGULAR_JOINT_INDEX_TORSO = 1,
	AK_MSSDK_REGULAR_JOINT_INDEX_NECK = 2,
	AK_MSSDK_REGULAR_JOINT_INDEX_HEAD = 3,

	AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_SHOULDER = 4,
	AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_ELBOW = 5,
	AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_WRIST = 6,
	AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_HAND = 7,

	AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_SHOULDER = 8,
	AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_ELBOW = 9,
	AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_WRIST = 10,
	AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_HAND = 11,

	AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_HIP = 12,
	AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_KNEE = 13,
	AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_ANKLE = 14,
	AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_FOOT = 15,

	AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_HIP = 16,
	AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_KNEE = 17,
	AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_ANKLE = 18,
	AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_FOOT = 19
};

typedef enum AkMssdkSeatedJointIndex
{
	AK_MSSDK_SEATED_JOINT_INDEX_NECK = 0,
	AK_MSSDK_SEATED_JOINT_INDEX_HEAD = 1,

	AK_MSSDK_SEATED_JOINT_INDEX_LEFT_SHOULDER = 2,
	AK_MSSDK_SEATED_JOINT_INDEX_LEFT_ELBOW = 3,
	AK_MSSDK_SEATED_JOINT_INDEX_LEFT_WRIST = 4,
	AK_MSSDK_SEATED_JOINT_INDEX_LEFT_HAND = 5,

	AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_SHOULDER = 6,
	AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_ELBOW = 7,
	AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_WRIST = 8,
	AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_HAND = 9
};

typedef struct _AKMSSDKSkeletonJoint
{
	AKSkeletonJoint* skeletonJoint;

	AKQuaternion hierarchicalRotationQuaternion;
	AKMatrix3D hierarchicalRotationMatrix;

	AKQuaternion absoluteRotationQuaternion;
	AKMatrix3D absoluteRotationMatrix;

} AKMSSDKSkeletonJoint;
#endif _AKMSSDKSkeletonJoint_

#endif

#endif
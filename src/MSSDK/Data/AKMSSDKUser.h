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
#ifndef KinectExtension_AKMSSDKUser_h
#define KinectExtension_AKMSSDKUser_h

#include "../../ExtensionConfig.h"

#ifdef AIRKINECT_TARGET_MSSDK

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include "../../Data/AKUser.h"
#include "AKMSSDKSkeletonJoint.h"
#include "AKMSSDKSkeletonBone.h"

#ifndef _AKMSSDKUser_
#define _AKMSSDKUser_
typedef struct _AKMSSDKUser
{
	AKUser* user;
	AKMSSDKSkeletonJoint* mssdkSkeletonJoints;
	AKMSSDKSkeletonBone* mssdkSkeletonBones;

} AKMSSDKUser;
#endif _AKMSSDKUser_

#endif

#endif
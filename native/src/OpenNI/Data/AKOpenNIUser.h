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
#ifndef KinectExtension_AKOpenNIUser_h
#define KinectExtension_AKOpenNIUser_h

#include "../../ExtensionConfig.h"

#ifdef AIRKINECT_TARGET_OPENNI

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include "../../Data/AKUser.h"
#include "AKOpenNISkeletonJoint.h"
#include "AKOpenNISkeletonBone.h"

#ifndef _AKOpenNIUser_
#define _AKOpenNIUser_
typedef struct _AKOpenNIUser
{
	AKUser* user;
	AKOpenNISkeletonJoint* openNISkeletonJoints;
	AKOpenNISkeletonBone* openNISkeletonBones;

} AKOpenNIUser;
#endif _AKOpenNIUser_

#endif

#endif
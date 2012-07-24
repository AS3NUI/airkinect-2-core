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
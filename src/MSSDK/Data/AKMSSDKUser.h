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
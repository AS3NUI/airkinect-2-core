#ifndef KinectExtension_AKUser_h
#define KinectExtension_AKUser_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include "AKBasicStructs.h"
#include "AKSkeletonJoint.h"

#ifndef _AKUser_
#define _AKUser_
typedef struct _AKUser
{
    int trackingID;
	int userID;
    bool isTracking;

	AKPosition position;
    
    bool hasSkeleton;
    AKSkeletonJoint* skeletonJoints;
} AKUser;
#endif _AKUser_

#endif
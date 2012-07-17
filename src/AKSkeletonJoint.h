#ifndef KinectExtension_AKSkeletonJoint_h
#define KinectExtension_AKSkeletonJoint_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include "AKBasicStructs.h"

#ifndef _AKSkeletonJoint_
#define _AKSkeletonJoint_
typedef struct _AKSkeletonJoint
{
	double positionConfidence;
	AKPosition position;
} AKSkeletonJoint;
#endif _AKSkeletonJoint_

#endif
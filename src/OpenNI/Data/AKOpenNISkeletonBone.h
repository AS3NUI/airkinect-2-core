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
typedef struct _AKOpenNISkeletonBone
{
	AKSkeletonBone* skeletonBone;

	FREObject asFREObject()
	{
		FREObject freBone = skeletonBone->asFREObject();
		return freBone;
	};

} AKOpenNISkeletonBone;
#endif _AKOpenNISkeletonBone_

#endif

#endif
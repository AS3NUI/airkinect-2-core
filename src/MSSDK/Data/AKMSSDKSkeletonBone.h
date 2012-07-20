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
typedef struct _AKMSSDKSkeletonBone
{
	AKSkeletonBone* skeletonBone;

	FREObject asFREObject()
	{
		FREObject freBone = skeletonBone->asFREObject();
		return freBone;
	};

} AKMSSDKSkeletonBone;
#endif _AKMSSDKSkeletonBone_

#endif

#endif
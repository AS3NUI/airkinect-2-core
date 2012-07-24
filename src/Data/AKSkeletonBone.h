#ifndef KinectExtension_AKSkeletonBone_h
#define KinectExtension_AKSkeletonBone_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include "AKBasicStructs.h"

#ifndef _AKSkeletonBone_
#define _AKSkeletonBone_
typedef struct _AKSkeletonBone
{
	AKOrientation orientation;

	int boneNameIndex;
	int startJointNameIndex;
	int endJointNameIndex;
	int parentBoneNameIndex;

	_AKSkeletonBone()
	{
		boneNameIndex = -1;
		startJointNameIndex = -1;
		endJointNameIndex = -1;
		parentBoneNameIndex = -1;
	};

} AKSkeletonBone;
#endif _AKSkeletonBone_

#endif
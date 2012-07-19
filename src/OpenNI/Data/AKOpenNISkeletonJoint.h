#ifndef KinectExtension_AKOpenNISkeletonJoint_h
#define KinectExtension_AKOpenNISkeletonJoint_h

#include "../../ExtensionConfig.h"

#ifdef AIRKINECT_TARGET_OPENNI

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include "../../Data/AKSkeletonJoint.h"
#include "../../Data/AKBasicStructs.h"

#ifndef _AKOpenNISkeletonJoint_
#define _AKOpenNISkeletonJoint_
typedef struct _AKOpenNISkeletonJoint
{
	AKSkeletonJoint* skeletonJoint;

	double orientationConfidence;
	AKMatrix3D orientation;

	FREObject asFREObject()
	{
		FREObject freJoint = skeletonJoint->asFREObject();

		FREObject freOrientationConfidence; FRENewObjectFromDouble(orientationConfidence, &freOrientationConfidence);
		FRESetObjectProperty(freJoint, (const uint8_t*) "nativeOrientationConfidence", freOrientationConfidence, NULL);

		FRESetObjectProperty(freJoint, (const uint8_t*) "nativeOrientation", orientation.asFREObject(), NULL);

		return freJoint;
	};

} AKOpenNISkeletonJoint;
#endif _AKOpenNISkeletonJoint_

#endif

#endif
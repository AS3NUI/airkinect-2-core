#ifndef KinectExtension_AKMSSDKSkeletonJoint_h
#define KinectExtension_AKMSSDKSkeletonJoint_h

#include "../../ExtensionConfig.h"

#ifdef AIRKINECT_TARGET_MSSDK

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include "../../Data/AKSkeletonJoint.h"
#include "../../Data/AKBasicStructs.h"

#ifndef _AKMSSDKSkeletonJoint_
#define _AKMSSDKSkeletonJoint_
typedef struct _AKMSSDKSkeletonJoint
{
	AKSkeletonJoint* skeletonJoint;

	AKQuaternion hierarchicalRotationQuaternion;
	AKMatrix3D hierarchicalRotationMatrix;

	AKQuaternion absoluteRotationQuaternion;
	AKMatrix3D absoluteRotationMatrix;

	FREObject asFREObject()
	{
		FREObject freJoint = skeletonJoint->asFREObject();

		FRESetObjectProperty(freJoint, (const uint8_t*) "nativeHierarchicalRotationQuaternion", hierarchicalRotationQuaternion.asFREObject(), NULL);
		FRESetObjectProperty(freJoint, (const uint8_t*) "nativeHierarchicalRotationMatrix", hierarchicalRotationMatrix.asFREObject(), NULL);
		FRESetObjectProperty(freJoint, (const uint8_t*) "nativeAbsoluteRotationQuaternion", absoluteRotationQuaternion.asFREObject(), NULL);
		FRESetObjectProperty(freJoint, (const uint8_t*) "nativeAbsoluteRotationMatrix", absoluteRotationMatrix.asFREObject(), NULL);

		return freJoint;
	};

} AKMSSDKSkeletonJoint;
#endif _AKMSSDKSkeletonJoint_

#endif

#endif
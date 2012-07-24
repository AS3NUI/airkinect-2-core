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

typedef enum AkOpenniJointIndex
{
	AK_OPENNI_JOINT_INDEX_HEAD = 0,
	AK_OPENNI_JOINT_INDEX_NECK = 1,
	AK_OPENNI_JOINT_INDEX_TORSO = 2,

	AK_OPENNI_JOINT_INDEX_LEFT_SHOULDER = 3,
	AK_OPENNI_JOINT_INDEX_LEFT_ELBOW = 4,
	AK_OPENNI_JOINT_INDEX_LEFT_HAND = 5,

	AK_OPENNI_JOINT_INDEX_RIGHT_SHOULDER = 6,
	AK_OPENNI_JOINT_INDEX_RIGHT_ELBOW = 7,
	AK_OPENNI_JOINT_INDEX_RIGHT_HAND = 8,

	AK_OPENNI_JOINT_INDEX_LEFT_HIP = 9,
	AK_OPENNI_JOINT_INDEX_LEFT_KNEE = 10,
	AK_OPENNI_JOINT_INDEX_LEFT_FOOT = 11,

	AK_OPENNI_JOINT_INDEX_RIGHT_HIP = 12,
	AK_OPENNI_JOINT_INDEX_RIGHT_KNEE = 13,
	AK_OPENNI_JOINT_INDEX_RIGHT_FOOT = 14
};

typedef struct _AKOpenNISkeletonJoint
{
	AKSkeletonJoint* skeletonJoint;

	double orientationConfidence;
	AKMatrix3D orientation;

} AKOpenNISkeletonJoint;
#endif _AKOpenNISkeletonJoint_

#endif

#endif
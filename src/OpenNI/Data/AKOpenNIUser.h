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

#ifndef _AKOpenNIUser_
#define _AKOpenNIUser_
typedef struct _AKOpenNIUser
{
	AKUser* user;
	AKOpenNISkeletonJoint* openNISkeletonJoints;

	FREObject asFREObject()
	{
		FREObject freUser = user->asFREObjectWithoutJoints();

		FREObject freJoints;
		FREGetObjectProperty(freUser, (const uint8_t*) "skeletonJoints", &freJoints, NULL);
		for(int i = 0; i < this->user->numJoints; i++)
		{
			FREObject freJoint = this->openNISkeletonJoints[i].asFREObject();
			FRESetArrayElementAt(freJoints, i, freJoint);
		}
		FRESetObjectProperty(freUser, (const uint8_t*) "skeletonJoints", freJoints, NULL);

		return freUser;
	};

} AKOpenNIUser;
#endif _AKOpenNIUser_

#endif

#endif
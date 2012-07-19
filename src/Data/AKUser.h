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

	char** jointNames;
	int numJoints;

	const char* asUserClass;
	const char* framework;

	FREObject asFREObject()
	{
		FREObject freUser, freUserType, freUserID, freTrackingID, freHasSkeleton, freJoints;

		FRENewObject( (const uint8_t*) asUserClass, 0, NULL, &freUser, NULL);

		FRENewObjectFromUTF8(strlen(this->framework), (const uint8_t*) this->framework, &freUserType);
		FRENewObjectFromUint32(this->userID, &freUserID);
		FRENewObjectFromUint32(this->trackingID, &freTrackingID);
		FRENewObjectFromBool((this->hasSkeleton) ? 1 : 0, &freHasSkeleton);

		FRESetObjectProperty(freUser, (const uint8_t*) "framework", freUserType, NULL);
		FRESetObjectProperty(freUser, (const uint8_t*) "userID", freUserID, NULL);
		FRESetObjectProperty(freUser, (const uint8_t*) "trackingID", freTrackingID, NULL);
		FRESetObjectProperty(freUser, (const uint8_t*) "position", this->position.asFREObject(), NULL);
		FRESetObjectProperty(freUser, (const uint8_t*) "hasSkeleton", freHasSkeleton, NULL);

		//create the joints vector
		FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint>", 0, NULL, &freJoints, NULL);
            
		for(int i = 0; i < numJoints; i++)
		{
			FREObject freJoint = this->skeletonJoints[i].asFREObject();
			FRESetArrayElementAt(freJoints, i, freJoint);
		}
            
		FRESetObjectProperty(freUser, (const uint8_t*) "skeletonJoints", freJoints, NULL);
		FRESetObjectProperty(freUser, (const uint8_t*) "skeletonJointNameIndices", freGetSkeletonJointNameIndices(), NULL);
		FRESetObjectProperty(freUser, (const uint8_t*) "skeletonJointNames", freGetSkeletonJointNames(), NULL);

		return freUser;
	};
	
	FREObject freGetSkeletonJointNameIndices()
	{
		FREObject skeletonJointNameIndices, jointIndex;
		FRENewObject( (const uint8_t*) "flash.utils.Dictionary", 0, NULL, &skeletonJointNameIndices, NULL);
		for(int i = 0; i < numJoints; i++)
		{
			FRENewObjectFromUint32(i, &jointIndex);
			FRESetObjectProperty(skeletonJointNameIndices, (const uint8_t*) jointNames[i], jointIndex, NULL);
		}
		return skeletonJointNameIndices;
	};

	FREObject freGetSkeletonJointNames()
	{ 
		FREObject skeletonJointNames, skeletonJointName;
		FRENewObject( (const uint8_t*) "Vector.<String>", 0, NULL, &skeletonJointNames, NULL);
		for(int i = 0; i < numJoints; i++)
		{
		FRENewObjectFromUTF8(strlen(jointNames[i]), (const uint8_t*) jointNames[i], &skeletonJointName);
		FRESetArrayElementAt(skeletonJointNames, i, skeletonJointName);
		}
		return skeletonJointNames;
	};

} AKUser;
#endif _AKUser_

#endif
#ifndef KinectExtension_AKUser_h
#define KinectExtension_AKUser_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include <string.h>
#include "AKBasicStructs.h"
#include "AKSkeletonJoint.h"
#include "AKSkeletonBone.h"

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
	AKSkeletonBone* skeletonBones;

	char** jointNames;
	int numJoints;

	char** boneNames;
	int numBones;

	const char* asUserClass;
	const char* framework;

	FREObject asFREObject()
	{
		FREObject freUser = asFREObjectWithoutJoints();
        
		FREObject freJoints;
		FREGetObjectProperty(freUser, (const uint8_t*) "skeletonJoints", &freJoints, NULL);
		for(int i = 0; i < numJoints; i++)
		{
			FREObject freJoint = this->skeletonJoints[i].asFREObject();
			FRESetArrayElementAt(freJoints, i, freJoint);
		}
		FRESetObjectProperty(freUser, (const uint8_t*) "skeletonJoints", freJoints, NULL);

		FREObject freBones;
		FREGetObjectProperty(freUser, (const uint8_t*) "skeletonBones", &freBones, NULL);
		for(int i = 0; i < numBones; i++)
		{
			FREObject freBone = this->skeletonBones[i].asFREObject();
			FRESetArrayElementAt(freBones, i, freBone);
		}
		FRESetObjectProperty(freUser, (const uint8_t*) "skeletonBones", freBones, NULL);

		return freUser;
	};
	
	FREObject asFREObjectWithoutJoints()
	{
		FREObject freUser, freUserType, freUserID, freTrackingID, freHasSkeleton, freJoints, freBones;

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

		FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint>", 0, NULL, &freJoints, NULL);            
		FRESetObjectProperty(freUser, (const uint8_t*) "skeletonJoints", freJoints, NULL);

		FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.SkeletonBone>", 0, NULL, &freBones, NULL);            
		FRESetObjectProperty(freUser, (const uint8_t*) "skeletonBones", freBones, NULL);

		FRESetObjectProperty(freUser, (const uint8_t*) "skeletonJointNameIndices", freGetSkeletonJointNameIndices(), NULL);
		FRESetObjectProperty(freUser, (const uint8_t*) "skeletonJointNames", freGetSkeletonJointNames(), NULL);

		FRESetObjectProperty(freUser, (const uint8_t*) "skeletonBoneNameIndices", freGetSkeletonBoneNameIndices(), NULL);
		FRESetObjectProperty(freUser, (const uint8_t*) "skeletonBoneNames", freGetSkeletonBoneNames(), NULL);

		return freUser;
	}

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

	FREObject freGetSkeletonBoneNameIndices()
	{
		FREObject skeletonBoneNameIndices, boneIndex;
		FRENewObject( (const uint8_t*) "flash.utils.Dictionary", 0, NULL, &skeletonBoneNameIndices, NULL);
		for(int i = 0; i < numBones; i++)
		{
			FRENewObjectFromUint32(i, &boneIndex);
			FRESetObjectProperty(skeletonBoneNameIndices, (const uint8_t*) boneNames[i], boneIndex, NULL);
		}
		return skeletonBoneNameIndices;
	};

	FREObject freGetSkeletonBoneNames()
	{ 
		FREObject skeletonBoneNames, skeletonBoneName;
		FRENewObject( (const uint8_t*) "Vector.<String>", 0, NULL, &skeletonBoneNames, NULL);
		for(int i = 0; i < numBones; i++)
		{
		FRENewObjectFromUTF8(strlen(boneNames[i]), (const uint8_t*) boneNames[i], &skeletonBoneName);
		FRESetArrayElementAt(skeletonBoneNames, i, skeletonBoneName);
		}
		return skeletonBoneNames;
	};

} AKUser;
#endif _AKUser_

#endif
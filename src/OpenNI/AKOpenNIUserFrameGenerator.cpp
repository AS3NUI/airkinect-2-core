#include "AKOpenNIUserFrameGenerator.h"

#ifdef AIRKINECT_TARGET_OPENNI

#include <math.h>

AKOpenNIUserFrameGenerator::AKOpenNIUserFrameGenerator()
{
	_asJointClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNISkeletonJoint";
	_asBoneClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNISkeletonBone";
	_asUserClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNIUser";
	_asUserFrameClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNIUserFrame";
	_framework = "openni";

	_maxSkeletons = 15;
	_numJoints = 15;
	_numBones = 10;

	allocateJointNames();
	allocateBoneNames();
	allocateUserFrame();
}

AKOpenNIUserFrameGenerator::~AKOpenNIUserFrameGenerator()
{
	deallocateJointNames();
	deallocateBoneNames();
	deallocateUserFrame();
}

void AKOpenNIUserFrameGenerator::allocateJointNames()
{
	_jointNames = new char*[_numJoints];
	_jointNames[AK_OPENNI_JOINT_INDEX_HEAD] = "head";
	_jointNames[AK_OPENNI_JOINT_INDEX_NECK] = "neck";
	_jointNames[AK_OPENNI_JOINT_INDEX_TORSO] = "torso";
    
	_jointNames[AK_OPENNI_JOINT_INDEX_LEFT_SHOULDER] = "left_shoulder";
	_jointNames[AK_OPENNI_JOINT_INDEX_LEFT_ELBOW] = "left_elbow";
	_jointNames[AK_OPENNI_JOINT_INDEX_LEFT_HAND] = "left_hand";
    
	_jointNames[AK_OPENNI_JOINT_INDEX_RIGHT_SHOULDER] = "right_shoulder";
	_jointNames[AK_OPENNI_JOINT_INDEX_RIGHT_ELBOW] = "right_elbow";
	_jointNames[AK_OPENNI_JOINT_INDEX_RIGHT_HAND] = "right_hand";
    
	_jointNames[AK_OPENNI_JOINT_INDEX_LEFT_HIP] = "left_hip";
	_jointNames[AK_OPENNI_JOINT_INDEX_LEFT_KNEE] = "left_knee";
	_jointNames[AK_OPENNI_JOINT_INDEX_LEFT_FOOT] = "left_foot";
    
	_jointNames[AK_OPENNI_JOINT_INDEX_RIGHT_HIP] = "right_hip";
	_jointNames[AK_OPENNI_JOINT_INDEX_RIGHT_KNEE] = "right_knee";
	_jointNames[AK_OPENNI_JOINT_INDEX_RIGHT_FOOT] = "right_foot";
}

void AKOpenNIUserFrameGenerator::deallocateJointNames()
{
	if(_jointNames != 0)
	{
		delete [] _jointNames;
		_jointNames = 0;
	}
}

void AKOpenNIUserFrameGenerator::allocateBoneNames()
{
	_boneNames = new char*[_numBones];
	_boneNames[AK_OPENNI_BONE_INDEX_NECK] = "neck";
	_boneNames[AK_OPENNI_BONE_INDEX_LEFT_UPPER_ARM] = "left_upper_arm";
	_boneNames[AK_OPENNI_BONE_INDEX_LEFT_LOWER_ARM] = "left_lower_arm";
	_boneNames[AK_OPENNI_BONE_INDEX_RIGHT_UPPER_ARM] = "right_upper_arm";
	_boneNames[AK_OPENNI_BONE_INDEX_RIGHT_LOWER_ARM] = "right_lower_arm";

	_boneNames[AK_OPENNI_BONE_INDEX_SPINE] = "spine";
	_boneNames[AK_OPENNI_BONE_INDEX_LEFT_UPPER_LEG] = "left_upper_leg";
	_boneNames[AK_OPENNI_BONE_INDEX_LEFT_LOWER_LEG] = "left_lower_leg";
	_boneNames[AK_OPENNI_BONE_INDEX_RIGHT_UPPER_LEG] = "right_upper_leg";
	_boneNames[AK_OPENNI_BONE_INDEX_RIGHT_LOWER_LEG] = "right_lower_leg";
}

void AKOpenNIUserFrameGenerator::deallocateBoneNames()
{
	if(_boneNames != 0)
	{
		delete [] _boneNames;
		_boneNames = 0;
	}
}

void AKOpenNIUserFrameGenerator::allocateUserFrame()
{
	AKUserFrameGenerator::allocateUserFrame();

	_openNIUserFrame = new AKOpenNIUserFrame();
	_openNIUserFrame->userFrame = _userFrame;
	_openNIUserFrame->openNIUsers = new AKOpenNIUser[_maxSkeletons];
	for(int i = 0; i < _maxSkeletons; i++)
	{
		_openNIUserFrame->openNIUsers[i].user = &_userFrame->users[i];
		
		_openNIUserFrame->openNIUsers[i].openNISkeletonJoints = new AKOpenNISkeletonJoint[_numJoints];
		for(int j = 0; j < _numJoints; j++)
		{
			_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[j].skeletonJoint = &_userFrame->users[i].skeletonJoints[j];
		}

		_openNIUserFrame->openNIUsers[i].openNISkeletonBones = new AKOpenNISkeletonBone[_numBones];
		for(int j = 0; j < _numBones; j++)
		{
			_openNIUserFrame->openNIUsers[i].openNISkeletonBones[j].skeletonBone = &_userFrame->users[i].skeletonBones[j];
		}
	}
}

void AKOpenNIUserFrameGenerator::deallocateUserFrame()
{
	AKUserFrameGenerator::deallocateUserFrame();
	if(_openNIUserFrame != 0)
	{
		if(_openNIUserFrame->openNIUsers != 0)
		{
			for(int i = 0; i < _maxSkeletons; i++)
			{
				delete [] _openNIUserFrame->openNIUsers[i].openNISkeletonJoints;
				_openNIUserFrame->openNIUsers[i].openNISkeletonJoints = 0;

				delete [] _openNIUserFrame->openNIUsers[i].openNISkeletonBones;
				_openNIUserFrame->openNIUsers[i].openNISkeletonBones = 0;
			}
			delete [] _openNIUserFrame->openNIUsers;
			_openNIUserFrame->openNIUsers = 0;
		}
		delete _openNIUserFrame;
		_openNIUserFrame = 0;
	}
}

void AKOpenNIUserFrameGenerator::setDepthGenerator(xn::DepthGenerator* depthGenerator)
{
	_depthGenerator = depthGenerator;
}
void AKOpenNIUserFrameGenerator::setUserGenerator(xn::UserGenerator* userGenerator)
{
	_userGenerator = userGenerator;
}

void AKOpenNIUserFrameGenerator::generateUserFrame()
{
	XnUserID *aUsers = new XnUserID[_maxSkeletons];
	XnUInt16 nUsers = _maxSkeletons;
	XnUInt16 trackedUsers = _userGenerator->GetNumberOfUsers();
	XnPoint3D position;
	XnStatus rc;
    
	_userGenerator->GetUsers(aUsers, nUsers);
    
	_userFrame->frameNumber = _userGenerator->GetFrameID();
	_userFrame->timeStamp = (int) (_userGenerator->GetTimestamp() / 1000);
    
	for (int i = 0; i < _maxSkeletons; ++i)
	{
		if(i < trackedUsers)
		{
			rc = _userGenerator->GetCoM(aUsers[i], position);

			_userFrame->users[i].isTracking = true;
			_userFrame->users[i].userID = aUsers[i];
			_userFrame->users[i].trackingID = aUsers[i];

			_userFrame->users[i].hasSkeleton = (_skeletonTrackingEnabled && _userGenerator->GetSkeletonCap().IsTracking(aUsers[i]));

			calculatePosition(_userFrame->users[i].position, position);
            
			if (_userFrame->users[i].hasSkeleton)
			{
				addJointElements(_openNIUserFrame->openNIUsers[i], aUsers[i]);
				addBoneElements(_openNIUserFrame->openNIUsers[i], aUsers[i]);
			}
		}
		else
		{
			_userFrame->users[i].isTracking = false;
		}
	}
}

void AKOpenNIUserFrameGenerator::addJointElements(AKOpenNIUser &openNIUser, XnUserID userID)
{
	//in openni, left/right are in device space, rather than user space
	//that's why we switch LEFT/RIGHT here, to match MS SDK (user space)
	setJointProperties(openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_HEAD], userID, XN_SKEL_HEAD);
	setJointProperties(openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_NECK], userID, XN_SKEL_NECK);
	setJointProperties(openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_TORSO], userID, XN_SKEL_TORSO);
                
	setJointProperties(openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_LEFT_SHOULDER], userID, XN_SKEL_RIGHT_SHOULDER);
	setJointProperties(openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_LEFT_ELBOW], userID, XN_SKEL_RIGHT_ELBOW);
	setJointProperties(openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_LEFT_HAND], userID, XN_SKEL_RIGHT_HAND);
                
	setJointProperties(openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_RIGHT_SHOULDER], userID, XN_SKEL_LEFT_SHOULDER);
	setJointProperties(openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_RIGHT_ELBOW], userID, XN_SKEL_LEFT_ELBOW);
	setJointProperties(openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_RIGHT_HAND], userID, XN_SKEL_LEFT_HAND);
                
	setJointProperties(openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_LEFT_HIP], userID, XN_SKEL_RIGHT_HIP);
	setJointProperties(openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_LEFT_KNEE], userID, XN_SKEL_RIGHT_KNEE);
	setJointProperties(openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_LEFT_FOOT], userID, XN_SKEL_RIGHT_FOOT);
                
	setJointProperties(openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_RIGHT_HIP], userID, XN_SKEL_LEFT_HIP);
	setJointProperties(openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_RIGHT_KNEE], userID, XN_SKEL_LEFT_KNEE);
	setJointProperties(openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_RIGHT_FOOT], userID, XN_SKEL_LEFT_FOOT);
}

void AKOpenNIUserFrameGenerator::setJointProperties(AKOpenNISkeletonJoint& openNISkeletonJoint, XnUserID user, XnSkeletonJoint eJoint)
{

    XnSkeletonJointPosition jointPosition;
    _userGenerator->GetSkeletonCap().GetSkeletonJointPosition(user, eJoint, jointPosition);

	calculatePosition(openNISkeletonJoint.skeletonJoint->position, jointPosition.position);
	openNISkeletonJoint.skeletonJoint->positionConfidence = jointPosition.fConfidence;
    
    XnSkeletonJointOrientation orientation;
    _userGenerator->GetSkeletonCap().GetSkeletonJointOrientation(user, eJoint, orientation);

	openNISkeletonJoint.orientationConfidence = orientation.fConfidence;
	if(openNISkeletonJoint.orientationConfidence == 0.0)
	{
		openNISkeletonJoint.orientation.M11 = 1.0;
		openNISkeletonJoint.orientation.M12 = 0.0;
		openNISkeletonJoint.orientation.M13 = 0.0;
        
		openNISkeletonJoint.orientation.M21 = 0.0;
		openNISkeletonJoint.orientation.M22 = 1.0;
		openNISkeletonJoint.orientation.M23 = 0.0;
        
		openNISkeletonJoint.orientation.M31 = 0.0;
		openNISkeletonJoint.orientation.M32 = 0.0;
		openNISkeletonJoint.orientation.M33 = 1.0;
	}
	else
	{
		openNISkeletonJoint.orientation.M11 = orientation.orientation.elements[0];
		openNISkeletonJoint.orientation.M12 = orientation.orientation.elements[3];
		openNISkeletonJoint.orientation.M13 = orientation.orientation.elements[6];

		openNISkeletonJoint.orientation.M21 = orientation.orientation.elements[1];
		openNISkeletonJoint.orientation.M22 = orientation.orientation.elements[4];
		openNISkeletonJoint.orientation.M23 = orientation.orientation.elements[7];
		
		openNISkeletonJoint.orientation.M31 = orientation.orientation.elements[2];
		openNISkeletonJoint.orientation.M32 = orientation.orientation.elements[5];
		openNISkeletonJoint.orientation.M33 = orientation.orientation.elements[8];
	}

	openNISkeletonJoint.orientation.M14 = 0.0;
	openNISkeletonJoint.orientation.M24 = 0.0;
	openNISkeletonJoint.orientation.M34 = 0.0;

	openNISkeletonJoint.orientation.M41 = 0.0;
	openNISkeletonJoint.orientation.M42 = 0.0;
	openNISkeletonJoint.orientation.M43 = 0.0;
	openNISkeletonJoint.orientation.M44 = 1.0;
}

void AKOpenNIUserFrameGenerator::addBoneElements(AKOpenNIUser &openNIUser, XnUserID userID)
{
	setBoneProperties(openNIUser.openNISkeletonBones[AK_OPENNI_BONE_INDEX_NECK], openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_NECK], -135.0, AK_OPENNI_JOINT_INDEX_NECK, AK_OPENNI_JOINT_INDEX_HEAD, AK_OPENNI_BONE_INDEX_SPINE);
	setBoneProperties(openNIUser.openNISkeletonBones[AK_OPENNI_BONE_INDEX_LEFT_UPPER_ARM], openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_LEFT_SHOULDER], 80.0, AK_OPENNI_JOINT_INDEX_LEFT_SHOULDER, AK_OPENNI_JOINT_INDEX_LEFT_ELBOW, AK_OPENNI_BONE_INDEX_SPINE);
	setBoneProperties(openNIUser.openNISkeletonBones[AK_OPENNI_BONE_INDEX_LEFT_LOWER_ARM], openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_LEFT_ELBOW], -90.0, AK_OPENNI_JOINT_INDEX_LEFT_ELBOW, AK_OPENNI_JOINT_INDEX_LEFT_HAND, AK_OPENNI_BONE_INDEX_LEFT_UPPER_ARM);
	setBoneProperties(openNIUser.openNISkeletonBones[AK_OPENNI_BONE_INDEX_RIGHT_UPPER_ARM], openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_RIGHT_SHOULDER], -80.0, AK_OPENNI_JOINT_INDEX_RIGHT_SHOULDER, AK_OPENNI_JOINT_INDEX_RIGHT_ELBOW, AK_OPENNI_BONE_INDEX_SPINE);
	setBoneProperties(openNIUser.openNISkeletonBones[AK_OPENNI_BONE_INDEX_RIGHT_LOWER_ARM], openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_RIGHT_ELBOW], 90.0, AK_OPENNI_JOINT_INDEX_RIGHT_ELBOW, AK_OPENNI_JOINT_INDEX_RIGHT_HAND, AK_OPENNI_BONE_INDEX_LEFT_LOWER_ARM);

	setBoneProperties(openNIUser.openNISkeletonBones[AK_OPENNI_BONE_INDEX_SPINE], openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_TORSO], -135.0, AK_OPENNI_JOINT_INDEX_TORSO, AK_OPENNI_JOINT_INDEX_NECK, -1);
	setBoneProperties(openNIUser.openNISkeletonBones[AK_OPENNI_BONE_INDEX_LEFT_UPPER_LEG], openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_LEFT_HIP], 0.0, AK_OPENNI_JOINT_INDEX_LEFT_HIP, AK_OPENNI_JOINT_INDEX_LEFT_KNEE, -1);
	setBoneProperties(openNIUser.openNISkeletonBones[AK_OPENNI_BONE_INDEX_LEFT_LOWER_LEG], openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_LEFT_KNEE], 0.0, AK_OPENNI_JOINT_INDEX_LEFT_KNEE, AK_OPENNI_JOINT_INDEX_LEFT_FOOT, AK_OPENNI_BONE_INDEX_LEFT_UPPER_LEG);
	setBoneProperties(openNIUser.openNISkeletonBones[AK_OPENNI_BONE_INDEX_RIGHT_UPPER_LEG], openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_RIGHT_HIP], 0.0, AK_OPENNI_JOINT_INDEX_RIGHT_HIP, AK_OPENNI_JOINT_INDEX_RIGHT_KNEE, -1);
	setBoneProperties(openNIUser.openNISkeletonBones[AK_OPENNI_BONE_INDEX_RIGHT_LOWER_LEG], openNIUser.openNISkeletonJoints[AK_OPENNI_JOINT_INDEX_RIGHT_KNEE], 0.0, AK_OPENNI_JOINT_INDEX_RIGHT_KNEE, AK_OPENNI_JOINT_INDEX_RIGHT_FOOT, AK_OPENNI_BONE_INDEX_RIGHT_UPPER_LEG);
}

void AKOpenNIUserFrameGenerator::setBoneProperties(AKOpenNISkeletonBone &openNISkeletonBone, AKOpenNISkeletonJoint &jointWhichContainsOrientation, double zDegreesCorrection, int startJointNameIndex, int endJointNameIndex, int parentBoneNameIndex)
{
	AKMatrix3D zMatrix;
	zMatrix.createRotationZ(zDegreesCorrection);

	openNISkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix = zMatrix * jointWhichContainsOrientation.orientation;

	openNISkeletonBone.skeletonBone->startJointNameIndex = startJointNameIndex;
	openNISkeletonBone.skeletonBone->endJointNameIndex = endJointNameIndex;
	openNISkeletonBone.skeletonBone->parentBoneNameIndex = parentBoneNameIndex;
}

void AKOpenNIUserFrameGenerator::calculatePosition(AKPosition &akPosition, XnPoint3D xnPosition)
{
	akPosition.world.create((!_skeletonMirrored) ? xnPosition.X : xnPosition.X * -1, xnPosition.Y, xnPosition.Z);

	akPosition.worldRelative.x = -1 * ((_depthSourceWidth - akPosition.world.x) / (_depthSourceWidth * 2) - .5);
	akPosition.worldRelative.y = -1 * (((_depthSourceHeight - akPosition.world.y) / (_depthSourceHeight * 2)) - .5);
	akPosition.worldRelative.z = (akPosition.world.z * 7.8125) / MAX_DEPTH;
            
	//depth & rgb space are the same, as we aligned the depth & image streams
	XnPoint3D pt[1];
	pt[0] = xnPosition;
	_depthGenerator->ConvertRealWorldToProjective(1, pt, pt);
            
	akPosition.rgbRelative.x = akPosition.depthRelative.x = pt[0].X / _depthSourceWidth;
	akPosition.rgbRelative.y = akPosition.depthRelative.y = pt[0].Y / _depthSourceHeight;
            
	if(_rgbTargetMirrored) akPosition.rgbRelative.x = 1 - akPosition.rgbRelative.x;
	if(_depthTargetMirrored) akPosition.depthRelative.x = 1 - akPosition.depthRelative.x;
            
	akPosition.rgb.x = (int) (akPosition.rgbRelative.x * _rgbTargetWidth);
	akPosition.rgb.y = (int) (akPosition.rgbRelative.y * _rgbTargetHeight);
	akPosition.depth.x = (int) (akPosition.depthRelative.x * _depthTargetWidth);
	akPosition.depth.y = (int) (akPosition.depthRelative.y * _depthTargetHeight);
}

FREObject AKOpenNIUserFrameGenerator::getFREObject()
{
	FREObject freUserFrame, freUsers;

	FRENewObject( (const uint8_t*) _asUserFrameClass, 0, NULL, &freUserFrame, NULL);

	FRESetObjectProperty(freUserFrame, (const uint8_t*) "frameNumber", createFREObjectForUnsignedInt(_userFrame->frameNumber), NULL);
	FRESetObjectProperty(freUserFrame, (const uint8_t*) "timeStamp", createFREObjectForUnsignedInt(_userFrame->timeStamp), NULL);
    
	FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.User>", 0, NULL, &freUsers, NULL);

	int trackedSkeletons = 0;
	for(int i = 0; i < _userFrame->maxSkeletons; i++)
	{
		AKOpenNIUser* user = &_openNIUserFrame->openNIUsers[i];
		if(user->user->isTracking)
		{
			FREObject freUser, freJoints, freBones;

			FRENewObject( (const uint8_t*) _asUserClass, 0, NULL, &freUser, NULL);

			FRESetObjectProperty(freUser, (const uint8_t*) "framework", createFREObjectForUTF8(_framework), NULL);
			FRESetObjectProperty(freUser, (const uint8_t*) "userID", createFREObjectForUnsignedInt(user->user->userID), NULL);
			FRESetObjectProperty(freUser, (const uint8_t*) "trackingID", createFREObjectForUnsignedInt(user->user->trackingID), NULL);
			FRESetObjectProperty(freUser, (const uint8_t*) "position", user->user->position.asFREObject(), NULL);
			FRESetObjectProperty(freUser, (const uint8_t*) "hasSkeleton", createFREObjectForBool(user->user->hasSkeleton), NULL);

			FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint>", 0, NULL, &freJoints, NULL);

			for(int j = 0; j < user->user->numJoints; j++)
			{
				AKOpenNISkeletonJoint* skeletonJoint = &user->openNISkeletonJoints[j];

				FREObject freJoint;
				FRENewObject( (const uint8_t*) _asJointClass, 0, NULL, &freJoint, NULL);

				FRESetObjectProperty(freJoint, (const uint8_t*) "name", createFREObjectForUTF8(_jointNames[skeletonJoint->skeletonJoint->jointNameIndex]), NULL);
				FRESetObjectProperty(freJoint, (const uint8_t*) "position", skeletonJoint->skeletonJoint->position.asFREObject(), NULL);
				FRESetObjectProperty(freJoint, (const uint8_t*) "positionConfidence", createFREObjectForDouble(skeletonJoint->skeletonJoint->positionConfidence), NULL);
				
				FRESetObjectProperty(freJoint, (const uint8_t*) "nativeOrientationConfidence", createFREObjectForDouble(skeletonJoint->orientationConfidence), NULL);
				FRESetObjectProperty(freJoint, (const uint8_t*) "nativeOrientation", skeletonJoint->orientation.asFREObject(), NULL);
				
				FRESetArrayElementAt(freJoints, j, freJoint);
			}

			FRESetObjectProperty(freUser, (const uint8_t*) "skeletonJoints", freJoints, NULL);

			FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.SkeletonBone>", 0, NULL, &freBones, NULL);

			for(int j = 0; j < user->user->numBones; j++)
			{
				AKOpenNISkeletonBone* skeletonBone = &user->openNISkeletonBones[j];
				FREObject freBone;
				FRENewObject( (const uint8_t*) _asBoneClass, 0, NULL, &freBone, NULL);

				FRESetObjectProperty(freBone, (const uint8_t*) "name", createFREObjectForUTF8(_boneNames[skeletonBone->skeletonBone->boneNameIndex]), NULL);
				FRESetObjectProperty(freBone, (const uint8_t*) "orientation", skeletonBone->skeletonBone->orientation.asFREObject(), NULL);
				
				if(skeletonBone->skeletonBone->startJointNameIndex > -1) 
					FRESetObjectProperty(freBone, (const uint8_t*) "startJointName", createFREObjectForUTF8(_jointNames[skeletonBone->skeletonBone->startJointNameIndex]), NULL);
				
				if(skeletonBone->skeletonBone->endJointNameIndex > -1) 
					FRESetObjectProperty(freBone, (const uint8_t*) "endJointName", createFREObjectForUTF8(_jointNames[skeletonBone->skeletonBone->endJointNameIndex]), NULL);
				
				if(skeletonBone->skeletonBone->parentBoneNameIndex > -1) 
					FRESetObjectProperty(freBone, (const uint8_t*) "parentBoneName", createFREObjectForUTF8(_boneNames[skeletonBone->skeletonBone->parentBoneNameIndex]), NULL);

				FRESetArrayElementAt(freBones, j, freBone);
			}

			FRESetObjectProperty(freUser, (const uint8_t*) "skeletonBones", freBones, NULL);

			FRESetObjectProperty(freUser, (const uint8_t*) "skeletonJointNameIndices", freGetSkeletonJointNameIndices(), NULL);
			FRESetObjectProperty(freUser, (const uint8_t*) "skeletonJointNames", freGetSkeletonJointNames(), NULL);

			FRESetObjectProperty(freUser, (const uint8_t*) "skeletonBoneNameIndices", freGetSkeletonBoneNameIndices(), NULL);
			FRESetObjectProperty(freUser, (const uint8_t*) "skeletonBoneNames", freGetSkeletonBoneNames(), NULL);

			FRESetArrayElementAt(freUsers, trackedSkeletons, freUser);
			trackedSkeletons++;
		}
	}

	FRESetObjectProperty(freUserFrame, (const uint8_t*) "users", freUsers, NULL);

	return freUserFrame;
}

#endif
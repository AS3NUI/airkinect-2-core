#include "AKOpenNIUserFrameGenerator.h"
#ifdef AIRKINECT_TARGET_OPENNI

AKOpenNIUserFrameGenerator::AKOpenNIUserFrameGenerator()
{
	_asJointClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNISkeletonJoint";
	_asUserClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNIUser";
	_asUserFrameClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNIUserFrame";

	_maxSkeletons = 15;
	_numJoints = 15;

	allocateJointNames();
	allocateUserFrame();
}

AKOpenNIUserFrameGenerator::~AKOpenNIUserFrameGenerator()
{
	deallocateJointNames();
	deallocateUserFrame();
}

void AKOpenNIUserFrameGenerator::allocateJointNames()
{
	_jointNames = new char*[_numJoints];
	_jointNames[0] = "head";
	_jointNames[1] = "neck";
	_jointNames[2] = "torso";
    
	_jointNames[3] = "left_shoulder";
	_jointNames[4] = "left_elbow";
	_jointNames[5] = "left_hand";
    
	_jointNames[6] = "right_shoulder";
	_jointNames[7] = "right_elbow";
	_jointNames[8] = "right_hand";
    
	_jointNames[9] = "left_hip";
	_jointNames[10] = "left_knee";
	_jointNames[11] = "left_foot";
    
	_jointNames[12] = "right_hip";
	_jointNames[13] = "right_knee";
	_jointNames[14] = "right_foot";
}

void AKOpenNIUserFrameGenerator::deallocateJointNames()
{
	if(_jointNames != 0)
	{
		delete [] _jointNames;
		_jointNames = 0;
	}
}

void AKOpenNIUserFrameGenerator::allocateUserFrame()
{
	_userFrame.users = new AKUser[_maxSkeletons];
	for(int i = 0; i < _maxSkeletons; i++)
	{
		_userFrame.users[i].skeletonJoints = new AKSkeletonJoint[_numJoints];
	}
}

void AKOpenNIUserFrameGenerator::deallocateUserFrame()
{
	if(_userFrame.users != 0)
	{
		for(int i = 0; i < _maxSkeletons; i++)
		{
			delete [] _userFrame.users[i].skeletonJoints;
			_userFrame.users[i].skeletonJoints = 0;
		}
		delete [] _userFrame.users;
		_userFrame.users = 0;
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
	memset(&_userFrame.users[0], 0, sizeof(_userFrame.users));
    
	XnUserID *aUsers = new XnUserID[_maxSkeletons];
	XnUInt16 nUsers = _maxSkeletons;
	XnUInt16 trackedUsers = _userGenerator->GetNumberOfUsers();
	XnPoint3D position;
	XnStatus rc;
    
	_userGenerator->GetUsers(aUsers, nUsers);
    
	_userFrame.frameNumber = _userGenerator->GetFrameID();
	_userFrame.timeStamp = (int) (_userGenerator->GetTimestamp() / 1000);
    
	for (int i = 0; i < _maxSkeletons; ++i)
	{
		if(i < trackedUsers)
		{
			rc = _userGenerator->GetCoM(aUsers[i], position);
			_userFrame.users[i].isTracking = true;
			_userFrame.users[i].userID = aUsers[i];
			_userFrame.users[i].trackingID = aUsers[i];

			_userFrame.users[i].hasSkeleton = (_skeletonTrackingEnabled && _userGenerator->GetSkeletonCap().IsTracking(aUsers[i]));

			double worldX = position.X;
			if(!_skeletonMirrored) worldX *= -1;

			_userFrame.users[i].position.world.create(worldX, position.Y, position.Z);

			_userFrame.users[i].position.worldRelative.x = (_depthSourceWidth - worldX) / (_depthSourceWidth * 2) - .5;
			_userFrame.users[i].position.worldRelative.y = -1 * (((_depthSourceHeight - position.Y) / (_depthSourceHeight * 2)) - .5);
			_userFrame.users[i].position.worldRelative.z = (position.Z * 7.8125) / MAX_DEPTH;
            
			//depth & rgb space are the same, as we aligned the depth & image streams
			XnPoint3D pt[1];
			pt[0] = position;
			_depthGenerator->ConvertRealWorldToProjective(1, pt, pt);
            
			_userFrame.users[i].position.rgbRelative.x = _userFrame.users[i].position.depthRelative.x = pt[0].X / _depthSourceWidth;
			_userFrame.users[i].position.rgbRelative.y = _userFrame.users[i].position.depthRelative.y = pt[0].Y / _depthSourceHeight;
            
			if(_rgbTargetMirrored) _userFrame.users[i].position.rgbRelative.x = 1 - _userFrame.users[i].position.rgbRelative.x;
			if(_depthTargetMirrored) _userFrame.users[i].position.depthRelative.x = 1 - _userFrame.users[i].position.depthRelative.x;
            
			_userFrame.users[i].position.rgb.x = (int) (_userFrame.users[i].position.rgbRelative.x * _rgbTargetWidth);
			_userFrame.users[i].position.rgb.y = (int) (_userFrame.users[i].position.rgbRelative.y * _rgbTargetHeight);
			_userFrame.users[i].position.depth.x = (int) (_userFrame.users[i].position.depthRelative.x * _depthTargetWidth);
			_userFrame.users[i].position.depth.y = (int) (_userFrame.users[i].position.depthRelative.y * _depthTargetHeight);
            
			if (_userFrame.users[i].hasSkeleton)
			{
				//in openni, left/right are in device space, rather than user space
				//that's why we switch LEFT/RIGHT here, to match MS SDK (user space)
				addJointElement(_userFrame.users[i], aUsers[i], XN_SKEL_HEAD, 0);
				addJointElement(_userFrame.users[i], aUsers[i], XN_SKEL_NECK, 1);
				addJointElement(_userFrame.users[i], aUsers[i], XN_SKEL_TORSO, 2);
                
				addJointElement(_userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_SHOULDER, 3);
				addJointElement(_userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_ELBOW, 4);
				addJointElement(_userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_HAND, 5);
                
				addJointElement(_userFrame.users[i], aUsers[i], XN_SKEL_LEFT_SHOULDER, 6);
				addJointElement(_userFrame.users[i], aUsers[i], XN_SKEL_LEFT_ELBOW, 7);
				addJointElement(_userFrame.users[i], aUsers[i], XN_SKEL_LEFT_HAND, 8);
                
				addJointElement(_userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_HIP, 9);
				addJointElement(_userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_KNEE, 10);
				addJointElement(_userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_FOOT, 11);
                
				addJointElement(_userFrame.users[i], aUsers[i], XN_SKEL_LEFT_HIP, 12);
				addJointElement(_userFrame.users[i], aUsers[i], XN_SKEL_LEFT_KNEE, 13);
				addJointElement(_userFrame.users[i], aUsers[i], XN_SKEL_LEFT_FOOT, 14);
			}
		}
		else
		{
			_userFrame.users[i].isTracking = false;
		}
	}
}

void AKOpenNIUserFrameGenerator::addJointElement(AKUser &kUser, XnUserID user, XnSkeletonJoint eJoint, unsigned int targetIndex)
{
    float jointPositionX, jointPositionY, jointPositionZ, jointW, jointPositionConfidence;
    
    XnSkeletonJointPosition jointPosition;
    _userGenerator->GetSkeletonCap().GetSkeletonJointPosition(user, eJoint, jointPosition);
    
    jointPositionX = jointPosition.position.X;
	if(!_skeletonMirrored) jointPositionX *= -1;

    jointPositionY = jointPosition.position.Y;
    jointPositionZ = jointPosition.position.Z;
    jointW = 0;
    jointPositionConfidence = jointPosition.fConfidence;
    
	/*
    XnSkeletonJointOrientation orientation;
    _userGenerator->GetSkeletonCap().GetSkeletonJointOrientation(user, eJoint, orientation);
    
    kUser.joints[targetOrientationIndex].orientationConfidence = orientation.fConfidence;
    
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M11 = orientation.orientation.elements[0];
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M12 = orientation.orientation.elements[3];
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M13 = orientation.orientation.elements[6];
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M14 = 0.0;
        
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M21 = orientation.orientation.elements[1];
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M22 = orientation.orientation.elements[4];
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M23 = orientation.orientation.elements[7];
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M24 = 0.0;
        
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M31 = orientation.orientation.elements[2];
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M32 = orientation.orientation.elements[5];
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M33 = orientation.orientation.elements[8];
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M34 = 0.0;
        
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M41 = 0.0;
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M42 = 0.0;
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M43 = 0.0;
    kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M44 = 1.0;
	*/
    
    kUser.skeletonJoints[targetIndex].positionConfidence = jointPositionConfidence;

    kUser.skeletonJoints[targetIndex].position.world.x = jointPositionX;
    kUser.skeletonJoints[targetIndex].position.world.y = jointPositionY;
    kUser.skeletonJoints[targetIndex].position.world.z = jointPositionZ;
    
	kUser.skeletonJoints[targetIndex].position.worldRelative.x = (_depthSourceWidth - jointPositionX) / (_depthSourceWidth * 2) - .5;
	kUser.skeletonJoints[targetIndex].position.worldRelative.y = -1 * (((_depthSourceHeight - jointPositionY) / (_depthSourceHeight * 2)) - .5);
    kUser.skeletonJoints[targetIndex].position.worldRelative.z = (jointPositionZ * 7.8125) / MAX_DEPTH;
    
    //relative position / depth & rgb are aligned
    XnPoint3D pt[1];
    pt[0] = jointPosition.position;
    _depthGenerator->ConvertRealWorldToProjective(1, pt, pt);
    
	kUser.skeletonJoints[targetIndex].position.rgbRelative.x = kUser.skeletonJoints[targetIndex].position.depthRelative.x = pt[0].X / _depthSourceWidth;
	kUser.skeletonJoints[targetIndex].position.rgbRelative.y = kUser.skeletonJoints[targetIndex].position.depthRelative.y = pt[0].Y / _depthSourceHeight;
    
    //take mirrored rgb / depth images into account
	if(_rgbTargetMirrored) kUser.skeletonJoints[targetIndex].position.rgbRelative.x = 1 - kUser.skeletonJoints[targetIndex].position.rgbRelative.x;
	if(_depthTargetMirrored) kUser.skeletonJoints[targetIndex].position.depthRelative.x = 1 - kUser.skeletonJoints[targetIndex].position.depthRelative.x;
    
    kUser.skeletonJoints[targetIndex].position.rgb.x = (int) (kUser.skeletonJoints[targetIndex].position.rgbRelative.x * _rgbTargetWidth);
    kUser.skeletonJoints[targetIndex].position.rgb.y = (int) (kUser.skeletonJoints[targetIndex].position.rgbRelative.y * _rgbTargetHeight);
	kUser.skeletonJoints[targetIndex].position.depth.x = (int) (kUser.skeletonJoints[targetIndex].position.depthRelative.x * _depthTargetWidth);
	kUser.skeletonJoints[targetIndex].position.depth.y = (int) (kUser.skeletonJoints[targetIndex].position.depthRelative.y * _depthTargetHeight);
}

FREObject AKOpenNIUserFrameGenerator::getFREObject()
{
	short int trackedSkeletons = 0;
    
    FREObject freUserFrame, frameNumber, timestamp, users, user, userType, userID, trackingID, hasSkeleton, joints, joint, jointName;
    FREObject positionConfidence;

	FRENewObject( (const uint8_t*) _asUserFrameClass, 0, NULL, &freUserFrame, NULL);

	FRENewObjectFromUint32(_userFrame.frameNumber, &frameNumber);
    FRENewObjectFromUint32(_userFrame.timeStamp, &timestamp);

	FRESetObjectProperty(freUserFrame, (const uint8_t*) "frameNumber", frameNumber, NULL);
	FRESetObjectProperty(freUserFrame, (const uint8_t*) "timeStamp", timestamp, NULL);
    
    FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.User>", 0, NULL, &users, NULL);
    
    for(int i = 0; i < _maxSkeletons; i++)
    {
        if(_userFrame.users[i].isTracking)
        {
			//create the user object
			FRENewObject( (const uint8_t*) _asUserClass, 0, NULL, &user, NULL);

			char* framework = "openni";

			FRENewObjectFromUTF8(strlen(framework), (const uint8_t*) framework, &userType);
			FRENewObjectFromUint32(_userFrame.users[i].userID, &userID);
			FRENewObjectFromUint32(_userFrame.users[i].trackingID, &trackingID);
			FRENewObjectFromBool((_userFrame.users[i].hasSkeleton) ? 1 : 0, &hasSkeleton);

			FRESetObjectProperty(user, (const uint8_t*) "framework", userType, NULL);
			FRESetObjectProperty(user, (const uint8_t*) "userID", userID, NULL);
			FRESetObjectProperty(user, (const uint8_t*) "trackingID", trackingID, NULL);
			FRESetObjectProperty(user, (const uint8_t*) "position", _userFrame.users[i].position.asFREObject(), NULL);
			FRESetObjectProperty(user, (const uint8_t*) "hasSkeleton", hasSkeleton, NULL);

			//create the joints vector
            FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint>", 0, NULL, &joints, NULL);
            
            for(int j = 0; j < _numJoints; j++)
            {

				FRENewObject( (const uint8_t*) _asJointClass, 0, NULL, &joint, NULL);

                FRENewObjectFromUTF8(strlen(_jointNames[j]), (const uint8_t*) _jointNames[j], &jointName);
				FRENewObjectFromDouble(_userFrame.users[i].skeletonJoints[j].positionConfidence, &positionConfidence);

				FRESetObjectProperty(joint, (const uint8_t*) "name", jointName, NULL);
				FRESetObjectProperty(joint, (const uint8_t*) "position", _userFrame.users[i].skeletonJoints[j].position.asFREObject(), NULL);
				FRESetObjectProperty(joint, (const uint8_t*) "positionConfidence", positionConfidence, NULL);
                
				FRESetArrayElementAt(joints, j, joint);
            }
            
			FRESetObjectProperty(user, (const uint8_t*) "skeletonJoints", joints, NULL);
			FRESetObjectProperty(user, (const uint8_t*) "skeletonJointNameIndices", freGetSkeletonJointNameIndices(), NULL);
			FRESetObjectProperty(user, (const uint8_t*) "skeletonJointNames", freGetSkeletonJointNames(), NULL);
            
            FRESetArrayElementAt(users, trackedSkeletons, user);
            trackedSkeletons++;
        }
    }

	FRESetObjectProperty(freUserFrame, (const uint8_t*) "users", users, NULL);
	return freUserFrame;
}

FREObject AKOpenNIUserFrameGenerator::freGetSkeletonJointNameIndices()
{
	FREObject skeletonJointNameIndices, jointIndex;
    FRENewObject( (const uint8_t*) "flash.utils.Dictionary", 0, NULL, &skeletonJointNameIndices, NULL);
	for(int i = 0; i < _numJoints; i++)
    {
        FRENewObjectFromUint32(i, &jointIndex);
        FRESetObjectProperty(skeletonJointNameIndices, (const uint8_t*) _jointNames[i], jointIndex, NULL);
    }
    return skeletonJointNameIndices;
}
FREObject AKOpenNIUserFrameGenerator::freGetSkeletonJointNames()
{ 
	FREObject skeletonJointNames, skeletonJointName;
	FRENewObject( (const uint8_t*) "Vector.<String>", 0, NULL, &skeletonJointNames, NULL);
	for(int i = 0; i < _numJoints; i++)
	{
	FRENewObjectFromUTF8(strlen(_jointNames[i]), (const uint8_t*) _jointNames[i], &skeletonJointName);
	FRESetArrayElementAt(skeletonJointNames, i, skeletonJointName);
	}
	return skeletonJointNames;
}

#endif
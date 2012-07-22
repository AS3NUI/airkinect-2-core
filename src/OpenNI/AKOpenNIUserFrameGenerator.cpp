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

void AKOpenNIUserFrameGenerator::allocateBoneNames()
{
	_boneNames = new char*[_numBones];
	_boneNames[0] = "neck";
	_boneNames[1] = "left_upper_arm";
	_boneNames[2] = "left_lower_arm";
	_boneNames[3] = "right_upper_arm";
	_boneNames[4] = "right_lower_arm";

	_boneNames[5] = "spine";
	_boneNames[6] = "left_upper_leg";
	_boneNames[7] = "left_lower_leg";
	_boneNames[8] = "right_upper_leg";
	_boneNames[9] = "right_lower_leg";
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

	setJointProperties(openNIUser.openNISkeletonJoints[0], userID, XN_SKEL_HEAD);
	setJointProperties(openNIUser.openNISkeletonJoints[1], userID, XN_SKEL_NECK);
	setJointProperties(openNIUser.openNISkeletonJoints[2], userID, XN_SKEL_TORSO);
                
	setJointProperties(openNIUser.openNISkeletonJoints[3], userID, XN_SKEL_RIGHT_SHOULDER);
	setJointProperties(openNIUser.openNISkeletonJoints[4], userID, XN_SKEL_RIGHT_ELBOW);
	setJointProperties(openNIUser.openNISkeletonJoints[5], userID, XN_SKEL_RIGHT_HAND);
                
	setJointProperties(openNIUser.openNISkeletonJoints[6], userID, XN_SKEL_LEFT_SHOULDER);
	setJointProperties(openNIUser.openNISkeletonJoints[7], userID, XN_SKEL_LEFT_ELBOW);
	setJointProperties(openNIUser.openNISkeletonJoints[8], userID, XN_SKEL_LEFT_HAND);
                
	setJointProperties(openNIUser.openNISkeletonJoints[9], userID, XN_SKEL_RIGHT_HIP);
	setJointProperties(openNIUser.openNISkeletonJoints[10], userID, XN_SKEL_RIGHT_KNEE);
	setJointProperties(openNIUser.openNISkeletonJoints[11], userID, XN_SKEL_RIGHT_FOOT);
                
	setJointProperties(openNIUser.openNISkeletonJoints[12], userID, XN_SKEL_LEFT_HIP);
	setJointProperties(openNIUser.openNISkeletonJoints[13], userID, XN_SKEL_LEFT_KNEE);
	setJointProperties(openNIUser.openNISkeletonJoints[14], userID, XN_SKEL_LEFT_FOOT);
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
	/*
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
	*/
	setBoneProperties(openNIUser.openNISkeletonBones[0], openNIUser.openNISkeletonJoints[1], 0.0, 0.0, -135.0); //neck bone
	setBoneProperties(openNIUser.openNISkeletonBones[1], openNIUser.openNISkeletonJoints[3], 0.0, 0.0, -90.0); //left upper arm
	setBoneProperties(openNIUser.openNISkeletonBones[2], openNIUser.openNISkeletonJoints[4], 0.0, 0.0, -90.0); //left lower arm
	setBoneProperties(openNIUser.openNISkeletonBones[3], openNIUser.openNISkeletonJoints[6], 0.0, 0.0, 90.0); //right upper arm
	setBoneProperties(openNIUser.openNISkeletonBones[4], openNIUser.openNISkeletonJoints[7], 0.0, 0.0, 90.0); //right lower arm

	setBoneProperties(openNIUser.openNISkeletonBones[5], openNIUser.openNISkeletonJoints[2], 0.0, 0.0, -135.0); //spine
	setBoneProperties(openNIUser.openNISkeletonBones[6], openNIUser.openNISkeletonJoints[9], 0.0, 0.0, 0.0); //left_upper_leg
	setBoneProperties(openNIUser.openNISkeletonBones[7], openNIUser.openNISkeletonJoints[10], 0.0, 0.0, 0.0); //left_lower_leg
	setBoneProperties(openNIUser.openNISkeletonBones[8], openNIUser.openNISkeletonJoints[12], 0.0, 0.0, 0.0); //right_upper_leg
	setBoneProperties(openNIUser.openNISkeletonBones[9], openNIUser.openNISkeletonJoints[13], 0.0, 0.0, 0.0); //right_lower_leg
}

void AKOpenNIUserFrameGenerator::setBoneProperties(AKOpenNISkeletonBone &openNISkeletonBone, AKOpenNISkeletonJoint &jointWhichContainsOrientation, double xDegreesToAppend, double yDegreesToAppend, double zDegreesToAppend)
{
	//create a bind pose matrix with the degreestoappend values & append the native orientation to that matrix
	double cosX = cos(xDegreesToAppend);
	double sinX = sin(xDegreesToAppend);
	AKMatrix3D xMatrix;
	xMatrix.create(
		1.0, 0.0, 0.0, 0.0,
		0.0, cosX, -sinX, 0.0,
		0.0, sinX, cosX, 0.0,
		0.0, 0.0, 0.0, 1.0);

	double cosY = cos(yDegreesToAppend);
	double sinY = sin(yDegreesToAppend);
	AKMatrix3D yMatrix;
	yMatrix.create(
		cosY, 0.0, sinY, 0.0,
		0.0, 1.0, 0.0, 0.0,
		-sinY, 0.0, cosY, 0.0,
		0.0, 0.0, 0.0, 1.0);

	double cosZ = cos(zDegreesToAppend);
	double sinZ = sin(zDegreesToAppend);
	AKMatrix3D zMatrix;
	zMatrix.create(
		cosZ, -sinZ, 0.0, 0.0,
		sinZ, cosZ, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);

	openNISkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix = xMatrix * yMatrix * zMatrix * jointWhichContainsOrientation.orientation;
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
	return _openNIUserFrame->asFREObject();
}

#endif
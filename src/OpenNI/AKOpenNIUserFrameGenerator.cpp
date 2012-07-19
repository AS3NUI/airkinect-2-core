#include "AKOpenNIUserFrameGenerator.h"

#ifdef AIRKINECT_TARGET_OPENNI

AKOpenNIUserFrameGenerator::AKOpenNIUserFrameGenerator()
{
	_asJointClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNISkeletonJoint";
	_asUserClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNIUser";
	_asUserFrameClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNIUserFrame";
	_framework = "openni";

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
				//in openni, left/right are in device space, rather than user space
				//that's why we switch LEFT/RIGHT here, to match MS SDK (user space)

				setJointProperties(_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[0], aUsers[i], XN_SKEL_HEAD);
				setJointProperties(_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[1], aUsers[i], XN_SKEL_NECK);
				setJointProperties(_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[2], aUsers[i], XN_SKEL_TORSO);
                
				setJointProperties(_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[3], aUsers[i], XN_SKEL_RIGHT_SHOULDER);
				setJointProperties(_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[4], aUsers[i], XN_SKEL_RIGHT_ELBOW);
				setJointProperties(_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[5], aUsers[i], XN_SKEL_RIGHT_HAND);
                
				setJointProperties(_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[6], aUsers[i], XN_SKEL_LEFT_SHOULDER);
				setJointProperties(_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[7], aUsers[i], XN_SKEL_LEFT_ELBOW);
				setJointProperties(_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[8], aUsers[i], XN_SKEL_LEFT_HAND);
                
				setJointProperties(_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[9], aUsers[i], XN_SKEL_RIGHT_HIP);
				setJointProperties(_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[10], aUsers[i], XN_SKEL_RIGHT_KNEE);
				setJointProperties(_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[11], aUsers[i], XN_SKEL_RIGHT_FOOT);
                
				setJointProperties(_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[12], aUsers[i], XN_SKEL_LEFT_HIP);
				setJointProperties(_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[13], aUsers[i], XN_SKEL_LEFT_KNEE);
				setJointProperties(_openNIUserFrame->openNIUsers[i].openNISkeletonJoints[14], aUsers[i], XN_SKEL_LEFT_FOOT);
			}
		}
		else
		{
			_userFrame->users[i].isTracking = false;
		}
	}
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

void AKOpenNIUserFrameGenerator::calculatePosition(AKPosition &akPosition, XnPoint3D xnPosition)
{
	double worldX = xnPosition.X;
	if(!_skeletonMirrored) worldX *= -1;

	akPosition.world.create(worldX, xnPosition.Y, xnPosition.Z);

	akPosition.worldRelative.x = (_depthSourceWidth - worldX) / (_depthSourceWidth * 2) - .5;
	akPosition.worldRelative.y = -1 * (((_depthSourceHeight - xnPosition.Y) / (_depthSourceHeight * 2)) - .5);
	akPosition.worldRelative.z = (xnPosition.Z * 7.8125) / MAX_DEPTH;
            
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
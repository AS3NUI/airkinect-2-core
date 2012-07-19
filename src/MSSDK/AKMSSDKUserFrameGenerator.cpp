#include "AKMSSDKUserFrameGenerator.h"
#ifdef AIRKINECT_TARGET_MSSDK

AKMSSDKUserFrameGenerator::AKMSSDKUserFrameGenerator()
{	
	_asJointClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSSkeletonJoint";
	_asUserClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSUser";
	_asUserFrameClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSUserFrame";
	_framework = "mssdk";

	_maxSkeletons = NUI_SKELETON_COUNT;

	_seatedSkeletonEnabled = false;

	allocateJointNames();
	allocateUserFrame();
}

AKMSSDKUserFrameGenerator::~AKMSSDKUserFrameGenerator()
{
	deallocateJointNames();
	deallocateUserFrame();
}

void AKMSSDKUserFrameGenerator::allocateJointNames()
{
	if(_seatedSkeletonEnabled)
		allocateJointNamesForSeatedSkeletonTracking();
	else
		allocateJointNamesForRegularSkeletonTracking();
}

void AKMSSDKUserFrameGenerator::deallocateJointNames()
{
	if(_jointNames != 0)
	{
		delete [] _jointNames;
		_jointNames = 0;
	}
}

void AKMSSDKUserFrameGenerator::allocateJointNamesForSeatedSkeletonTracking()
{
	_numJoints = 10;
	_jointNames = new char*[_numJoints];
	_jointNames[0] = "neck";
	_jointNames[1] = "head";
    
	_jointNames[2] = "left_shoulder";
	_jointNames[3] = "left_elbow";
	_jointNames[4] = "left_wrist";
	_jointNames[5] = "left_hand";
    
	_jointNames[6] = "right_shoulder";
	_jointNames[7] = "right_elbow";
	_jointNames[8] = "right_wrist";
	_jointNames[9] = "right_hand";
}

void AKMSSDKUserFrameGenerator::allocateJointNamesForRegularSkeletonTracking()
{
	_numJoints = 20;
	_jointNames = new char*[_numJoints];
	_jointNames[0] = "waist";
	_jointNames[1] = "torso";
	_jointNames[2] = "neck";
	_jointNames[3] = "head";
    
	_jointNames[4] = "left_shoulder";
	_jointNames[5] = "left_elbow";
	_jointNames[6] = "left_wrist";
	_jointNames[7] = "left_hand";
    
	_jointNames[8] = "right_shoulder";
	_jointNames[9] = "right_elbow";
	_jointNames[10] = "right_wrist";
	_jointNames[11] = "right_hand";
    
	_jointNames[12] = "left_hip";
	_jointNames[13] = "left_knee";
	_jointNames[14] = "left_ankle";
	_jointNames[15] = "left_foot";
    
	_jointNames[16] = "right_hip";
	_jointNames[17] = "right_knee";
	_jointNames[18] = "right_ankle";
	_jointNames[19] = "right_foot";
}

void AKMSSDKUserFrameGenerator::allocateUserFrame()
{
	AKUserFrameGenerator::allocateUserFrame();

	_mssdkUserFrame = new AKMSSDKUserFrame();
	_mssdkUserFrame->userFrame = _userFrame;
	_mssdkUserFrame->mssdkUsers = new AKMSSDKUser[_maxSkeletons];
	for(int i = 0; i < _maxSkeletons; i++)
	{
		_mssdkUserFrame->mssdkUsers[i].user = &_userFrame->users[i];
		_mssdkUserFrame->mssdkUsers[i].mssdkSkeletonJoints = new AKMSSDKSkeletonJoint[_numJoints];
		for(int j = 0; j < _numJoints; j++)
		{
			_mssdkUserFrame->mssdkUsers[i].mssdkSkeletonJoints[j].skeletonJoint = &_userFrame->users[i].skeletonJoints[j];
		}
	}
}

void AKMSSDKUserFrameGenerator::deallocateUserFrame()
{
	AKUserFrameGenerator::deallocateUserFrame();
	if(_mssdkUserFrame != 0)
	{
		if(_mssdkUserFrame->mssdkUsers != 0)
		{
			for(int i = 0; i < _maxSkeletons; i++)
			{
				delete [] _mssdkUserFrame->mssdkUsers[i].mssdkSkeletonJoints;
				_mssdkUserFrame->mssdkUsers[i].mssdkSkeletonJoints = 0;
			}
			delete [] _mssdkUserFrame->mssdkUsers;
			_mssdkUserFrame->mssdkUsers = 0;
		}
		delete _mssdkUserFrame;
		_mssdkUserFrame = 0;
	}
}

void AKMSSDKUserFrameGenerator::setRGBTargetSize(int width, int height)
{
	AKUserFrameGenerator::setRGBTargetSize(width, height);
	_targetRGBResolution = getNuiImageResolutionForGivenWidthAndHeight(width, height);
}

void AKMSSDKUserFrameGenerator::setDepthTargetSize(int width, int height)
{
	AKUserFrameGenerator::setDepthTargetSize(width, height);
	_targetDepthResolution = getNuiImageResolutionForGivenWidthAndHeight(width, height);
}

void AKMSSDKUserFrameGenerator::setSeatedSkeletonEnabled(bool seatedSkeletonEnabled)
{
	deallocateJointNames();
	deallocateUserFrame();
	_seatedSkeletonEnabled = seatedSkeletonEnabled;
	allocateJointNames();
	allocateUserFrame();
}

void AKMSSDKUserFrameGenerator::setNuiSensor(INuiSensor* nuiSensor)
{
	_nuiSensor = nuiSensor;
}

void AKMSSDKUserFrameGenerator::setTransformSmoothingParameters(NUI_TRANSFORM_SMOOTH_PARAMETERS transformSmoothingParameters)
{
	_transformSmoothingParameters = transformSmoothingParameters;
}

void AKMSSDKUserFrameGenerator::generateUserFrame()
{
	NUI_SKELETON_FRAME skeletonFrame = {0};
	HRESULT hr = _nuiSensor->NuiSkeletonGetNextFrame( 0, &skeletonFrame );
	if(FAILED(hr))
	{
		return;
	}

	_nuiSensor->NuiTransformSmooth(&skeletonFrame, &_transformSmoothingParameters);

	NUI_SKELETON_DATA skeletonData;
	_userFrame->frameNumber = skeletonFrame.dwFrameNumber;
	_userFrame->timeStamp = (int)(skeletonFrame.liTimeStamp.QuadPart /1000);

	for (int i = 0; i < NUI_SKELETON_COUNT; ++i){
		skeletonData = skeletonFrame.SkeletonData[i];

		if(skeletonData.eTrackingState == NUI_SKELETON_TRACKED || skeletonData.eTrackingState == NUI_SKELETON_POSITION_ONLY) {
			_userFrame->users[i].isTracking = true;
			//dwTrackingID is some wierd ass number by microsoft, to match properly to depth player index use i+1
			_userFrame->users[i].trackingID = skeletonData.dwTrackingID;
			_userFrame->users[i].userID = i+1;
			_userFrame->users[i].hasSkeleton = skeletonData.eTrackingState == NUI_SKELETON_TRACKED;
			
			//Transform for User
			calculatePosition(_userFrame->users[i].position, skeletonData.Position);

			//joint orientation (kinect sdk 1.5)
			NUI_SKELETON_BONE_ORIENTATION *boneOrientations = new NUI_SKELETON_BONE_ORIENTATION[NUI_SKELETON_POSITION_COUNT];
			hr = NuiSkeletonCalculateBoneOrientations(&skeletonData, boneOrientations);

			//Joint Position Calculations
			if (_userFrame->users[i].hasSkeleton){
				addJointElements(_mssdkUserFrame->mssdkUsers[i], skeletonData, boneOrientations);
			}

			//cleanup
			delete [] boneOrientations;
		}else{
			_userFrame->users[i].isTracking = false;
		}
	}
}

void AKMSSDKUserFrameGenerator::calculatePosition(AKPosition &kTransform, Vector4 skeletonTransform){
	long colorX, colorY, depthX, depthY;
	USHORT depthValue;

	Vector4 mSkeletonPosition = skeletonTransform;
	skeletonTransform.x *= -1;

	kTransform.world.create(_skeletonMirrored ? mSkeletonPosition.x * 1000 : skeletonTransform.x * 1000, mSkeletonPosition.y * 1000, mSkeletonPosition.z * 1000);
	kTransform.worldRelative.create(_skeletonMirrored ? mSkeletonPosition.x : skeletonTransform.x, mSkeletonPosition.y, mSkeletonPosition.z);

	//User to Pixel Positions on Images
	if(_depthTargetMirrored){
		NuiTransformSkeletonToDepthImage(mSkeletonPosition, &depthX, &depthY, &depthValue,_targetDepthResolution);
	}else{
		NuiTransformSkeletonToDepthImage(skeletonTransform, &depthX, &depthY, &depthValue,_targetDepthResolution);
	}
	
	kTransform.depth.create(depthX, depthY);
	kTransform.depthRelative.create(((float)depthX)/((float)_depthTargetWidth), ((float)depthY)/((float)_depthTargetHeight));

	if(_rgbTargetMirrored){
		NuiTransformSkeletonToDepthImage(mSkeletonPosition, &depthX, &depthY, &depthValue,_targetDepthResolution);
	}else{
		NuiTransformSkeletonToDepthImage(skeletonTransform, &depthX, &depthY, &depthValue,_targetDepthResolution);
	}
	
	_nuiSensor->NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(_targetRGBResolution, _targetDepthResolution, 0, depthX,depthY, depthValue,&colorX,&colorY);
	
	kTransform.rgb.create(colorX, colorY);
	kTransform.rgbRelative.create(((float)colorX)/((float)_rgbTargetWidth), ((float)colorY)/((float)_rgbTargetHeight));
}

void AKMSSDKUserFrameGenerator::addJointElements(AKMSSDKUser &mssdkUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations)
{
	if(_seatedSkeletonEnabled)
		addJointElementsForSeatedSkeletonTracking(mssdkUser, skeletonData, boneOrientations);
	else
		addJointElementsForRegularSkeletonTracking(mssdkUser, skeletonData, boneOrientations);
}

void AKMSSDKUserFrameGenerator::addJointElementsForSeatedSkeletonTracking(AKMSSDKUser &mssdkUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations)
{
	setJointProperties(mssdkUser.mssdkSkeletonJoints[0], skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_CENTER);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[1], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HEAD);
                
    setJointProperties(mssdkUser.mssdkSkeletonJoints[2], skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[3], skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[4], skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[5], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_LEFT);
                
    setJointProperties(mssdkUser.mssdkSkeletonJoints[6], skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[7], skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[8], skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[9], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_RIGHT);
}

void AKMSSDKUserFrameGenerator::addJointElementsForRegularSkeletonTracking(AKMSSDKUser &mssdkUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations)
{
	setJointProperties(mssdkUser.mssdkSkeletonJoints[0], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HIP_CENTER);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[1], skeletonData, boneOrientations, NUI_SKELETON_POSITION_SPINE);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[2], skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_CENTER);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[3], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HEAD);
                
    setJointProperties(mssdkUser.mssdkSkeletonJoints[4], skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[5], skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[6], skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[7], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_LEFT);
                
    setJointProperties(mssdkUser.mssdkSkeletonJoints[8], skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[9], skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[10], skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[11], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_RIGHT);
                
    setJointProperties(mssdkUser.mssdkSkeletonJoints[12], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HIP_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[13], skeletonData, boneOrientations, NUI_SKELETON_POSITION_KNEE_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[14], skeletonData, boneOrientations, NUI_SKELETON_POSITION_ANKLE_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[15], skeletonData, boneOrientations, NUI_SKELETON_POSITION_FOOT_LEFT);
                
    setJointProperties(mssdkUser.mssdkSkeletonJoints[16], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HIP_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[17], skeletonData, boneOrientations, NUI_SKELETON_POSITION_KNEE_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[18], skeletonData, boneOrientations, NUI_SKELETON_POSITION_ANKLE_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[19], skeletonData, boneOrientations, NUI_SKELETON_POSITION_FOOT_RIGHT);
}

void AKMSSDKUserFrameGenerator::setJointProperties(AKMSSDKSkeletonJoint &mssdkSkeletonJoint, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations, NUI_SKELETON_POSITION_INDEX eJoint)
{
	Vector4 jointPosition = skeletonData.SkeletonPositions[eJoint];

	calculatePosition(mssdkSkeletonJoint.skeletonJoint->position, jointPosition);

	mssdkSkeletonJoint.skeletonJoint->positionConfidence = 0;

	if(skeletonData.eSkeletonPositionTrackingState[eJoint] == NUI_SKELETON_POSITION_INFERRED)
	{
		mssdkSkeletonJoint.skeletonJoint->positionConfidence = 0.5;
	}
	else if(skeletonData.eSkeletonPositionTrackingState[eJoint] == NUI_SKELETON_POSITION_TRACKED)
	{
		mssdkSkeletonJoint.skeletonJoint->positionConfidence = 1;
	}
	
	mssdkSkeletonJoint.absoluteRotationMatrix.M11 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M11;
	mssdkSkeletonJoint.absoluteRotationMatrix.M12 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M12;
	mssdkSkeletonJoint.absoluteRotationMatrix.M13 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M13;
	mssdkSkeletonJoint.absoluteRotationMatrix.M14 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M14;

	mssdkSkeletonJoint.absoluteRotationMatrix.M21 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M21;
	mssdkSkeletonJoint.absoluteRotationMatrix.M22 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M22;
	mssdkSkeletonJoint.absoluteRotationMatrix.M23 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M23;
	mssdkSkeletonJoint.absoluteRotationMatrix.M24 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M24;

	mssdkSkeletonJoint.absoluteRotationMatrix.M31 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M31;
	mssdkSkeletonJoint.absoluteRotationMatrix.M32 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M32;
	mssdkSkeletonJoint.absoluteRotationMatrix.M33 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M33;
	mssdkSkeletonJoint.absoluteRotationMatrix.M34 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M34;

	mssdkSkeletonJoint.absoluteRotationMatrix.M41 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M41;
	mssdkSkeletonJoint.absoluteRotationMatrix.M42 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M42;
	mssdkSkeletonJoint.absoluteRotationMatrix.M43 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M43;
	mssdkSkeletonJoint.absoluteRotationMatrix.M44 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M44;

	mssdkSkeletonJoint.absoluteRotationQuaternion.x = boneOrientations[eJoint].absoluteRotation.rotationQuaternion.x;
	mssdkSkeletonJoint.absoluteRotationQuaternion.y = boneOrientations[eJoint].absoluteRotation.rotationQuaternion.y;
	mssdkSkeletonJoint.absoluteRotationQuaternion.z = boneOrientations[eJoint].absoluteRotation.rotationQuaternion.z;
	mssdkSkeletonJoint.absoluteRotationQuaternion.w = boneOrientations[eJoint].absoluteRotation.rotationQuaternion.w;

	mssdkSkeletonJoint.hierarchicalRotationMatrix.M11 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M11;
	mssdkSkeletonJoint.hierarchicalRotationMatrix.M12 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M12;
	mssdkSkeletonJoint.hierarchicalRotationMatrix.M13 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M13;
	mssdkSkeletonJoint.hierarchicalRotationMatrix.M14 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M14;

	mssdkSkeletonJoint.hierarchicalRotationMatrix.M21 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M21;
	mssdkSkeletonJoint.hierarchicalRotationMatrix.M22 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M22;
	mssdkSkeletonJoint.hierarchicalRotationMatrix.M23 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M23;
	mssdkSkeletonJoint.hierarchicalRotationMatrix.M24 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M24;

	mssdkSkeletonJoint.hierarchicalRotationMatrix.M31 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M31;
	mssdkSkeletonJoint.hierarchicalRotationMatrix.M32 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M32;
	mssdkSkeletonJoint.hierarchicalRotationMatrix.M33 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M33;
	mssdkSkeletonJoint.hierarchicalRotationMatrix.M34 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M34;

	mssdkSkeletonJoint.hierarchicalRotationMatrix.M41 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M41;
	mssdkSkeletonJoint.hierarchicalRotationMatrix.M42 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M42;
	mssdkSkeletonJoint.hierarchicalRotationMatrix.M43 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M43;
	mssdkSkeletonJoint.hierarchicalRotationMatrix.M44 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M44;

	mssdkSkeletonJoint.hierarchicalRotationQuaternion.x = boneOrientations[eJoint].hierarchicalRotation.rotationQuaternion.x;
	mssdkSkeletonJoint.hierarchicalRotationQuaternion.y = boneOrientations[eJoint].hierarchicalRotation.rotationQuaternion.y;
	mssdkSkeletonJoint.hierarchicalRotationQuaternion.z = boneOrientations[eJoint].hierarchicalRotation.rotationQuaternion.z;
	mssdkSkeletonJoint.hierarchicalRotationQuaternion.w = boneOrientations[eJoint].hierarchicalRotation.rotationQuaternion.w;
}

FREObject AKMSSDKUserFrameGenerator::getFREObject()
{
	return _mssdkUserFrame->asFREObject();
}

#endif
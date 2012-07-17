#include "AKMSSDKUserFrameGenerator.h"
#ifdef AIRKINECT_TARGET_MSSDK

AKMSSDKUserFrameGenerator::AKMSSDKUserFrameGenerator()
{
	_asJointClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSSkeletonJoint";
	_asUserClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSUser";
	_asUserFrameClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSUserFrame";
	
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
	_userFrame.users = new AKUser[_maxSkeletons];
	for(int i = 0; i < _maxSkeletons; i++)
	{
		_userFrame.users[i].skeletonJoints = new AKSkeletonJoint[_numJoints];
	}
}

void AKMSSDKUserFrameGenerator::deallocateUserFrame()
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

    memset(&_userFrame.users[0], 0, sizeof(_userFrame.users));
	NUI_SKELETON_FRAME skeletonFrame = {0};
	HRESULT hr = _nuiSensor->NuiSkeletonGetNextFrame( 0, &skeletonFrame );
	if(FAILED(hr))
	{
		return;
	}

	_nuiSensor->NuiTransformSmooth(&skeletonFrame, &_transformSmoothingParameters);

	NUI_SKELETON_DATA skeletonData;
	_userFrame.frameNumber = skeletonFrame.dwFrameNumber;
	_userFrame.timeStamp = (int)(skeletonFrame.liTimeStamp.QuadPart /1000);

	for (int i = 0; i < NUI_SKELETON_COUNT; ++i){
		skeletonData = skeletonFrame.SkeletonData[i];

		if(skeletonData.eTrackingState == NUI_SKELETON_TRACKED || skeletonData.eTrackingState == NUI_SKELETON_POSITION_ONLY) {
			_userFrame.users[i].isTracking = true;
			//dwTrackingID is some wierd ass number by microsoft, to match properly to depth player index use i+1
			_userFrame.users[i].trackingID = skeletonData.dwTrackingID;
			_userFrame.users[i].userID = i+1;
			_userFrame.users[i].hasSkeleton = skeletonData.eTrackingState == NUI_SKELETON_TRACKED;
			
			//Transform for User
			calculateKinectTransform(_userFrame.users[i].position, skeletonData.Position);

			//joint orientation (kinect sdk 1.5)
			NUI_SKELETON_BONE_ORIENTATION *boneOrientations = new NUI_SKELETON_BONE_ORIENTATION[NUI_SKELETON_POSITION_COUNT];
			hr = NuiSkeletonCalculateBoneOrientations(&skeletonData, boneOrientations);

			//Joint Position Calculations
			if (_userFrame.users[i].hasSkeleton){
				addJointElements(_userFrame.users[i], skeletonData, boneOrientations);
			}

			//cleanup
			delete [] boneOrientations;
		}else{
			_userFrame.users[i].isTracking = false;
		}
	}
}

void AKMSSDKUserFrameGenerator::calculateKinectTransform(AKPosition &kTransform, Vector4 skeletonTransform){
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

void AKMSSDKUserFrameGenerator::addJointElements(AKUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations)
{
	if(_seatedSkeletonEnabled)
		addJointElementsForSeatedSkeletonTracking(kUser, skeletonData, boneOrientations);
	else
		addJointElementsForRegularSkeletonTracking(kUser, skeletonData, boneOrientations);
}

void AKMSSDKUserFrameGenerator::addJointElementsForSeatedSkeletonTracking(AKUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations)
{
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_CENTER, 0);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HEAD, 1);
                
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_LEFT, 2);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_LEFT, 3);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_LEFT, 4);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_LEFT, 5);
                
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_RIGHT, 6);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_RIGHT, 7);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_RIGHT, 8);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_RIGHT, 9);
}

void AKMSSDKUserFrameGenerator::addJointElementsForRegularSkeletonTracking(AKUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations)
{
	addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HIP_CENTER, 0);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_SPINE, 1);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_CENTER, 2);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HEAD, 3);
                
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_LEFT, 4);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_LEFT, 5);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_LEFT, 6);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_LEFT, 7);
                
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_RIGHT, 8);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_RIGHT, 9);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_RIGHT, 10);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_RIGHT, 11);
                
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HIP_LEFT, 12);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_KNEE_LEFT, 13);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_ANKLE_LEFT, 14);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_FOOT_LEFT, 15);
                
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HIP_RIGHT, 16);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_KNEE_RIGHT, 17);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_ANKLE_RIGHT, 18);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_FOOT_RIGHT, 19);
}

void AKMSSDKUserFrameGenerator::addJointElement(AKUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations, NUI_SKELETON_POSITION_INDEX eJoint, uint32_t targetIndex)
{
	Vector4 jointPosition = skeletonData.SkeletonPositions[eJoint];

	//Transform for User
	calculateKinectTransform(kUser.skeletonJoints[targetIndex].position, jointPosition);

	kUser.skeletonJoints[targetIndex].positionConfidence = 0;

	if(skeletonData.eSkeletonPositionTrackingState[eJoint] == NUI_SKELETON_POSITION_INFERRED)
	{
		kUser.skeletonJoints[targetIndex].positionConfidence = 0.5;
	}
	else if(skeletonData.eSkeletonPositionTrackingState[eJoint] == NUI_SKELETON_POSITION_TRACKED)
	{
		kUser.skeletonJoints[targetIndex].positionConfidence = 1;
	}
	/*
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M11 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M11;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M12 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M12;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M13 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M13;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M14 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M14;

	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M21 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M21;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M22 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M22;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M23 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M23;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M24 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M24;

	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M31 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M31;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M32 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M32;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M33 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M33;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M34 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M34;

	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M41 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M41;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M42 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M42;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M43 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M43;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M44 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M44;

	kUser.joints[targetIndex].absoluteOrientation.rotationQuaternion.x = boneOrientations[eJoint].absoluteRotation.rotationQuaternion.x;
	kUser.joints[targetIndex].absoluteOrientation.rotationQuaternion.y = boneOrientations[eJoint].absoluteRotation.rotationQuaternion.y;
	kUser.joints[targetIndex].absoluteOrientation.rotationQuaternion.z = boneOrientations[eJoint].absoluteRotation.rotationQuaternion.z;
	kUser.joints[targetIndex].absoluteOrientation.rotationQuaternion.w = boneOrientations[eJoint].absoluteRotation.rotationQuaternion.w;

	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M11 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M11;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M12 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M12;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M13 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M13;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M14 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M14;

	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M21 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M21;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M22 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M22;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M23 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M23;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M24 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M24;

	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M31 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M31;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M32 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M32;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M33 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M33;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M34 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M34;

	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M41 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M41;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M42 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M42;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M43 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M43;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M44 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M44;

	kUser.joints[targetIndex].hierarchicalOrientation.rotationQuaternion.x = boneOrientations[eJoint].hierarchicalRotation.rotationQuaternion.x;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationQuaternion.y = boneOrientations[eJoint].hierarchicalRotation.rotationQuaternion.y;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationQuaternion.z = boneOrientations[eJoint].hierarchicalRotation.rotationQuaternion.z;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationQuaternion.w = boneOrientations[eJoint].hierarchicalRotation.rotationQuaternion.w;

    kUser.joints[targetIndex].positionConfidence = 1;
	*/
}

FREObject AKMSSDKUserFrameGenerator::getFREObject()
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

			char* framework = "mssdk";

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

FREObject AKMSSDKUserFrameGenerator::freGetSkeletonJointNameIndices()
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
FREObject AKMSSDKUserFrameGenerator::freGetSkeletonJointNames()
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
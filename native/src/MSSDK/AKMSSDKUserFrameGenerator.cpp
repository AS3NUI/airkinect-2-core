/*
 * Copyright 2012 AS3NUI
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#include "AKMSSDKUserFrameGenerator.h"
#ifdef AIRKINECT_TARGET_MSSDK

AKMSSDKUserFrameGenerator::AKMSSDKUserFrameGenerator()
{
	_asJointClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSSkeletonJoint";
	_asBoneClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSSkeletonBone";
	_asUserClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSUser";
	_asUserFrameClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSUserFrame";
	_framework = "mssdk";

	_maxSkeletons = NUI_SKELETON_COUNT;

	_seatedSkeletonEnabled = false;

	allocateJointNames();
	allocateBoneNames();
	allocateUserFrame();

	_nuiSensor = 0;
	_nuiInteractionStream = 0;
}

AKMSSDKUserFrameGenerator::~AKMSSDKUserFrameGenerator()
{
	deallocateJointNames();
	deallocateBoneNames();
	deallocateUserFrame();

	_nuiSensor = 0;
	_nuiInteractionStream = 0;
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

void AKMSSDKUserFrameGenerator::allocateBoneNames()
{
	if(_seatedSkeletonEnabled)
		allocateBoneNamesForSeatedSkeletonTracking();
	else
		allocateBoneNamesForRegularSkeletonTracking();
}

void AKMSSDKUserFrameGenerator::deallocateBoneNames()
{
	if(_boneNames != 0)
	{
		delete [] _boneNames;
		_boneNames = 0;
	}
}

void AKMSSDKUserFrameGenerator::allocateJointNamesForSeatedSkeletonTracking()
{
	_numJoints = 10;
	_jointNames = new const char*[_numJoints];
	_jointNames[AK_MSSDK_SEATED_JOINT_INDEX_NECK] = "neck";
	_jointNames[AK_MSSDK_SEATED_JOINT_INDEX_HEAD] = "head";
    
	_jointNames[AK_MSSDK_SEATED_JOINT_INDEX_LEFT_SHOULDER] = "left_shoulder";
	_jointNames[AK_MSSDK_SEATED_JOINT_INDEX_LEFT_ELBOW] = "left_elbow";
	_jointNames[AK_MSSDK_SEATED_JOINT_INDEX_LEFT_WRIST] = "left_wrist";
	_jointNames[AK_MSSDK_SEATED_JOINT_INDEX_LEFT_HAND] = "left_hand";
    
	_jointNames[AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_SHOULDER] = "right_shoulder";
	_jointNames[AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_ELBOW] = "right_elbow";
	_jointNames[AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_WRIST] = "right_wrist";
	_jointNames[AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_HAND] = "right_hand";
}

void AKMSSDKUserFrameGenerator::allocateJointNamesForRegularSkeletonTracking()
{
	_numJoints = 20;
	_jointNames = new const char*[_numJoints];
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_WAIST] = "waist";
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_TORSO] = "torso";
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_NECK] = "neck";
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_HEAD] = "head";
    
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_SHOULDER] = "left_shoulder";
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_ELBOW] = "left_elbow";
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_WRIST] = "left_wrist";
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_HAND] = "left_hand";
    
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_SHOULDER] = "right_shoulder";
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_ELBOW] = "right_elbow";
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_WRIST] = "right_wrist";
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_HAND] = "right_hand";
    
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_HIP] = "left_hip";
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_KNEE] = "left_knee";
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_ANKLE] = "left_ankle";
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_FOOT] = "left_foot";
    
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_HIP] = "right_hip";
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_KNEE] = "right_knee";
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_ANKLE] = "right_ankle";
	_jointNames[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_FOOT] = "right_foot";
}

void AKMSSDKUserFrameGenerator::allocateBoneNamesForSeatedSkeletonTracking()
{
	_numBones = 9;
	_boneNames = new const char*[_numBones];
	_boneNames[AK_MSSDK_SEATED_BONE_INDEX_NECK] = "neck";

	_boneNames[AK_MSSDK_SEATED_BONE_INDEX_LEFT_COLLAR] = "left_collar";
	_boneNames[AK_MSSDK_SEATED_BONE_INDEX_LEFT_UPPER_ARM] = "left_upper_arm";
	_boneNames[AK_MSSDK_SEATED_BONE_INDEX_LEFT_LOWER_ARM] = "left_lower_arm";
	_boneNames[AK_MSSDK_SEATED_BONE_INDEX_LEFT_HAND] = "left_hand";

	_boneNames[AK_MSSDK_SEATED_BONE_INDEX_RIGHT_COLLAR] = "right_collar";
	_boneNames[AK_MSSDK_SEATED_BONE_INDEX_RIGHT_UPPER_ARM] = "right_upper_arm";
	_boneNames[AK_MSSDK_SEATED_BONE_INDEX_RIGHT_LOWER_ARM] = "right_lower_arm";
	_boneNames[AK_MSSDK_SEATED_BONE_INDEX_RIGHT_HAND] = "right_hand";
}

void AKMSSDKUserFrameGenerator::allocateBoneNamesForRegularSkeletonTracking()
{
	_numBones = 19;
	_boneNames = new const char*[_numBones];
	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_NECK] = "neck";

	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_COLLAR] = "left_collar";
	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_UPPER_ARM] = "left_upper_arm";
	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_LOWER_ARM] = "left_lower_arm";
	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_HAND] = "left_hand";

	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_COLLAR] = "right_collar";
	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_UPPER_ARM] = "right_upper_arm";
	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_LOWER_ARM] = "right_lower_arm";
	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_HAND] = "right_hand";

	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_SPINE] = "spine";
	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_LOWER_SPINE] = "lower_spine";

	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_HIP] = "left_hip";
	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_UPPER_LEG] = "left_upper_leg";
	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_LOWER_LEG] = "left_lower_leg";
	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_FOOT] = "left_foot";

	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_HIP] = "right_hip";
	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_UPPER_LEG] = "right_upper_leg";
	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_LOWER_LEG] = "right_lower_leg";
	_boneNames[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_FOOT] = "right_foot";
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
		_mssdkUserFrame->mssdkUsers[i].mssdkSkeletonBones = new AKMSSDKSkeletonBone[_numBones];
		for(int j = 0; j < _numBones; j++)
		{
			_mssdkUserFrame->mssdkUsers[i].mssdkSkeletonBones[j].skeletonBone = &_userFrame->users[i].skeletonBones[j];
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

				delete [] _mssdkUserFrame->mssdkUsers[i].mssdkSkeletonBones;
				_mssdkUserFrame->mssdkUsers[i].mssdkSkeletonBones = 0;
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
	deallocateBoneNames();
	deallocateUserFrame();
	_seatedSkeletonEnabled = seatedSkeletonEnabled;
	allocateJointNames();
	allocateBoneNames();
	allocateUserFrame();
}

void AKMSSDKUserFrameGenerator::setNuiSensor(INuiSensor* nuiSensor)
{
	_nuiSensor = nuiSensor;
}

void AKMSSDKUserFrameGenerator::setNuiInteractionStream(INuiInteractionStream* nuiInteractionStream)
{
	_nuiInteractionStream = nuiInteractionStream;
}

void AKMSSDKUserFrameGenerator::setTransformSmoothingParameters(NUI_TRANSFORM_SMOOTH_PARAMETERS transformSmoothingParameters)
{
	_transformSmoothingParameters = transformSmoothingParameters;
}

void AKMSSDKUserFrameGenerator::generateUserFrame()
{
	NUI_SKELETON_FRAME skeletonFrame = {0};
	HRESULT hr = _nuiSensor->NuiSkeletonGetNextFrame( 0, &skeletonFrame );
	if(FAILED(hr)){
		return;
	}

	_nuiSensor->NuiTransformSmooth(&skeletonFrame, &_transformSmoothingParameters);

	if(_nuiInteractionStream) {
		Vector4 v = {0};
		_nuiSensor->NuiAccelerometerGetCurrentReading(&v);
		hr = _nuiInteractionStream->ProcessSkeleton(NUI_SKELETON_COUNT, skeletonFrame.SkeletonData, &v, skeletonFrame.liTimeStamp);
	}

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
				addBoneElements(_mssdkUserFrame->mssdkUsers[i], skeletonData, boneOrientations);
			}

			//cleanup
			delete [] boneOrientations;
		}else{
			_userFrame->users[i].isTracking = false;
		}
	}
}

void AKMSSDKUserFrameGenerator::addInteractionInfo(NUI_INTERACTION_FRAME* interactionFrame){
	for (int i = 0; i < NUI_SKELETON_COUNT; ++i){
		NUI_USER_INFO userInfo = interactionFrame->UserInfos[i];
		if(userInfo.SkeletonTrackingId != 0){
			//get this user in the userframe
			AKMSSDKUser* msUser = 0;
			for (int u = 0; u < NUI_SKELETON_COUNT; u++) {
				if(_mssdkUserFrame->mssdkUsers[u].user->trackingID == userInfo.SkeletonTrackingId){
					msUser = &_mssdkUserFrame->mssdkUsers[u];
					break;
				}
			}
			if(msUser != 0){
				for(int j = 0; j < NUI_USER_HANDPOINTER_COUNT; j++){
					NUI_HANDPOINTER_INFO handPointerInfo = userInfo.HandPointerInfos[j];
					if(handPointerInfo.HandType == NUI_HAND_TYPE_LEFT) {
						msUser->leftHandInfo = handPointerInfo;
					}else if(handPointerInfo.HandType == NUI_HAND_TYPE_RIGHT) {
						msUser->rightHandInfo = handPointerInfo;
					}
				}
			}
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
	setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_SEATED_JOINT_INDEX_NECK], skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_CENTER);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_SEATED_JOINT_INDEX_HEAD], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HEAD);
                
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_SEATED_JOINT_INDEX_LEFT_SHOULDER], skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_SEATED_JOINT_INDEX_LEFT_ELBOW], skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_SEATED_JOINT_INDEX_LEFT_WRIST], skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_SEATED_JOINT_INDEX_LEFT_HAND], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_LEFT);
                
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_SHOULDER], skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_ELBOW], skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_WRIST], skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_HAND], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_RIGHT);
}

void AKMSSDKUserFrameGenerator::addJointElementsForRegularSkeletonTracking(AKMSSDKUser &mssdkUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations)
{
	setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_WAIST], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HIP_CENTER);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_TORSO], skeletonData, boneOrientations, NUI_SKELETON_POSITION_SPINE);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_NECK], skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_CENTER);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_HEAD], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HEAD);
                
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_SHOULDER], skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_ELBOW], skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_WRIST], skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_HAND], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_LEFT);
                
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_SHOULDER], skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_ELBOW], skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_WRIST], skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_HAND], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_RIGHT);
                
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_HIP], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HIP_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_KNEE], skeletonData, boneOrientations, NUI_SKELETON_POSITION_KNEE_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_ANKLE], skeletonData, boneOrientations, NUI_SKELETON_POSITION_ANKLE_LEFT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_FOOT], skeletonData, boneOrientations, NUI_SKELETON_POSITION_FOOT_LEFT);
                
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_HIP], skeletonData, boneOrientations, NUI_SKELETON_POSITION_HIP_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_KNEE], skeletonData, boneOrientations, NUI_SKELETON_POSITION_KNEE_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_ANKLE], skeletonData, boneOrientations, NUI_SKELETON_POSITION_ANKLE_RIGHT);
    setJointProperties(mssdkUser.mssdkSkeletonJoints[AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_FOOT], skeletonData, boneOrientations, NUI_SKELETON_POSITION_FOOT_RIGHT);
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

void AKMSSDKUserFrameGenerator::addBoneElements(AKMSSDKUser &mssdkUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations)
{
	if(_seatedSkeletonEnabled)
		addBoneElementsForSeatedSkeletonTracking(mssdkUser, skeletonData, boneOrientations);
	else
		addBoneElementsForRegularSkeletonTracking(mssdkUser, skeletonData, boneOrientations);
}

void AKMSSDKUserFrameGenerator::addBoneElementsForSeatedSkeletonTracking(AKMSSDKUser &mssdkUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations)
{
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_SEATED_BONE_INDEX_NECK], boneOrientations, NUI_SKELETON_POSITION_HEAD, AK_MSSDK_SEATED_JOINT_INDEX_NECK, AK_MSSDK_SEATED_JOINT_INDEX_HEAD, -1);
	
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_SEATED_BONE_INDEX_LEFT_COLLAR], boneOrientations, NUI_SKELETON_POSITION_SHOULDER_LEFT, AK_MSSDK_SEATED_JOINT_INDEX_NECK, AK_MSSDK_SEATED_JOINT_INDEX_LEFT_SHOULDER, -1);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_SEATED_BONE_INDEX_LEFT_UPPER_ARM], boneOrientations, NUI_SKELETON_POSITION_ELBOW_LEFT, AK_MSSDK_SEATED_JOINT_INDEX_LEFT_SHOULDER, AK_MSSDK_SEATED_JOINT_INDEX_LEFT_ELBOW, AK_MSSDK_SEATED_BONE_INDEX_LEFT_COLLAR);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_SEATED_BONE_INDEX_LEFT_LOWER_ARM], boneOrientations, NUI_SKELETON_POSITION_WRIST_LEFT, AK_MSSDK_SEATED_JOINT_INDEX_LEFT_ELBOW, AK_MSSDK_SEATED_JOINT_INDEX_LEFT_WRIST, AK_MSSDK_SEATED_BONE_INDEX_LEFT_UPPER_ARM);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_SEATED_BONE_INDEX_LEFT_HAND], boneOrientations, NUI_SKELETON_POSITION_HAND_LEFT, AK_MSSDK_SEATED_JOINT_INDEX_LEFT_WRIST, AK_MSSDK_SEATED_JOINT_INDEX_LEFT_HAND, AK_MSSDK_SEATED_BONE_INDEX_LEFT_LOWER_ARM);
	
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_SEATED_BONE_INDEX_RIGHT_COLLAR], boneOrientations, NUI_SKELETON_POSITION_SHOULDER_RIGHT, AK_MSSDK_SEATED_JOINT_INDEX_NECK, AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_SHOULDER, -1);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_SEATED_BONE_INDEX_RIGHT_UPPER_ARM], boneOrientations, NUI_SKELETON_POSITION_ELBOW_RIGHT, AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_SHOULDER, AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_ELBOW, AK_MSSDK_SEATED_BONE_INDEX_RIGHT_COLLAR);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_SEATED_BONE_INDEX_RIGHT_LOWER_ARM], boneOrientations, NUI_SKELETON_POSITION_WRIST_RIGHT, AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_ELBOW, AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_WRIST, AK_MSSDK_SEATED_BONE_INDEX_RIGHT_UPPER_ARM);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_SEATED_BONE_INDEX_RIGHT_HAND], boneOrientations, NUI_SKELETON_POSITION_HAND_RIGHT, AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_WRIST, AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_HAND, AK_MSSDK_SEATED_BONE_INDEX_RIGHT_LOWER_ARM);
}

void AKMSSDKUserFrameGenerator::addBoneElementsForRegularSkeletonTracking(AKMSSDKUser &mssdkUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations)
{
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_NECK], boneOrientations, NUI_SKELETON_POSITION_HEAD, AK_MSSDK_REGULAR_JOINT_INDEX_NECK, AK_MSSDK_REGULAR_JOINT_INDEX_HEAD, AK_MSSDK_REGULAR_BONE_INDEX_SPINE);
	
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_COLLAR], boneOrientations, NUI_SKELETON_POSITION_SHOULDER_LEFT, AK_MSSDK_REGULAR_JOINT_INDEX_NECK, AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_SHOULDER, AK_MSSDK_REGULAR_BONE_INDEX_SPINE);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_UPPER_ARM], boneOrientations, NUI_SKELETON_POSITION_ELBOW_LEFT, AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_SHOULDER, AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_ELBOW, AK_MSSDK_REGULAR_BONE_INDEX_LEFT_COLLAR);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_LOWER_ARM], boneOrientations, NUI_SKELETON_POSITION_WRIST_LEFT, AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_ELBOW, AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_WRIST, AK_MSSDK_REGULAR_BONE_INDEX_LEFT_UPPER_ARM);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_HAND], boneOrientations, NUI_SKELETON_POSITION_HAND_LEFT, AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_WRIST, AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_HAND, AK_MSSDK_REGULAR_BONE_INDEX_LEFT_LOWER_ARM);
	
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_COLLAR], boneOrientations, NUI_SKELETON_POSITION_SHOULDER_RIGHT, AK_MSSDK_REGULAR_JOINT_INDEX_NECK, AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_SHOULDER, AK_MSSDK_REGULAR_BONE_INDEX_SPINE);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_UPPER_ARM], boneOrientations, NUI_SKELETON_POSITION_ELBOW_RIGHT, AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_SHOULDER, AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_ELBOW, AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_COLLAR);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_LOWER_ARM], boneOrientations, NUI_SKELETON_POSITION_WRIST_RIGHT, AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_ELBOW, AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_WRIST, AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_UPPER_ARM);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_HAND], boneOrientations, NUI_SKELETON_POSITION_HAND_RIGHT, AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_WRIST, AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_HAND, AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_LOWER_ARM);

	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_SPINE], boneOrientations, NUI_SKELETON_POSITION_SHOULDER_CENTER, AK_MSSDK_REGULAR_JOINT_INDEX_TORSO, AK_MSSDK_REGULAR_JOINT_INDEX_NECK, AK_MSSDK_REGULAR_BONE_INDEX_LOWER_SPINE);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_LOWER_SPINE], boneOrientations, NUI_SKELETON_POSITION_SPINE, AK_MSSDK_REGULAR_JOINT_INDEX_WAIST, AK_MSSDK_REGULAR_JOINT_INDEX_TORSO, -1);

	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_HIP], boneOrientations, NUI_SKELETON_POSITION_HIP_LEFT, AK_MSSDK_REGULAR_JOINT_INDEX_WAIST, AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_HIP, -1);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_UPPER_LEG], boneOrientations, NUI_SKELETON_POSITION_KNEE_LEFT, AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_HIP, AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_KNEE, AK_MSSDK_REGULAR_BONE_INDEX_LEFT_HIP);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_LOWER_LEG], boneOrientations, NUI_SKELETON_POSITION_ANKLE_LEFT, AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_KNEE, AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_ANKLE, AK_MSSDK_REGULAR_BONE_INDEX_LEFT_UPPER_LEG);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_LEFT_FOOT], boneOrientations, NUI_SKELETON_POSITION_FOOT_LEFT, AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_ANKLE, AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_FOOT, AK_MSSDK_REGULAR_BONE_INDEX_LEFT_LOWER_LEG);

	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_HIP], boneOrientations, NUI_SKELETON_POSITION_HIP_RIGHT, AK_MSSDK_REGULAR_JOINT_INDEX_WAIST, AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_HIP, -1);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_UPPER_LEG], boneOrientations, NUI_SKELETON_POSITION_KNEE_RIGHT, AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_HIP, AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_KNEE, AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_HIP);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_LOWER_LEG], boneOrientations, NUI_SKELETON_POSITION_ANKLE_RIGHT, AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_KNEE, AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_ANKLE, AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_UPPER_LEG);
	setBoneProperties(mssdkUser.mssdkSkeletonBones[AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_FOOT], boneOrientations, NUI_SKELETON_POSITION_FOOT_RIGHT, AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_ANKLE, AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_FOOT, AK_MSSDK_REGULAR_BONE_INDEX_RIGHT_LOWER_LEG);
}


void AKMSSDKUserFrameGenerator::setBoneProperties(AKMSSDKSkeletonBone &mssdkSkeletonBone, NUI_SKELETON_BONE_ORIENTATION *boneOrientations, NUI_SKELETON_POSITION_INDEX indexOfJointWithRotation, int startJointNameIndex, int endJointNameIndex, int parentBoneNameIndex)
{
	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M11 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M11;
	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M12 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M12;
	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M13 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M13;
	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M14 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M14;

	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M21 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M21;
	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M22 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M22;
	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M23 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M23;
	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M24 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M24;

	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M31 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M31;
	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M32 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M32;
	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M33 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M33;
	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M34 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M34;

	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M41 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M41;
	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M42 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M42;
	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M43 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M43;
	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M44 = boneOrientations[indexOfJointWithRotation].absoluteRotation.rotationMatrix.M44;

	//invert Y
	mssdkSkeletonBone.skeletonBone->orientation.absoluteOrientationMatrix.M22 *= -1.0;

	mssdkSkeletonBone.skeletonBone->startJointNameIndex = startJointNameIndex;
	mssdkSkeletonBone.skeletonBone->endJointNameIndex = endJointNameIndex;
	mssdkSkeletonBone.skeletonBone->parentBoneNameIndex = parentBoneNameIndex;
}

FREObject AKMSSDKUserFrameGenerator::getFREObject()
{
	FREObject freUserFrame, freUsers;

	FRENewObject( (const uint8_t*) _asUserFrameClass, 0, NULL, &freUserFrame, NULL);

	FRESetObjectProperty(freUserFrame, (const uint8_t*) "frameNumber", createFREObjectForUnsignedInt(_userFrame->frameNumber), NULL);
	FRESetObjectProperty(freUserFrame, (const uint8_t*) "timeStamp", createFREObjectForUnsignedInt(_userFrame->timeStamp), NULL);
    
	FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.User>", 0, NULL, &freUsers, NULL);

	int trackedSkeletons = 0;
	for(int i = 0; i < _userFrame->maxSkeletons; i++)
	{
		AKMSSDKUser* user = &_mssdkUserFrame->mssdkUsers[i];
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
				AKMSSDKSkeletonJoint* skeletonJoint = &user->mssdkSkeletonJoints[j];

				FREObject freJoint;

				bool isLeftHand = false;
				bool isRightHand = false;
				if(_seatedSkeletonEnabled){
					isLeftHand = (skeletonJoint->skeletonJoint->jointNameIndex == AK_MSSDK_SEATED_JOINT_INDEX_LEFT_HAND);
					isRightHand = (skeletonJoint->skeletonJoint->jointNameIndex == AK_MSSDK_SEATED_JOINT_INDEX_RIGHT_HAND);
				} else{
					isLeftHand = (skeletonJoint->skeletonJoint->jointNameIndex == AK_MSSDK_REGULAR_JOINT_INDEX_LEFT_HAND);
					isRightHand = (skeletonJoint->skeletonJoint->jointNameIndex == AK_MSSDK_REGULAR_JOINT_INDEX_RIGHT_HAND);
				}

				if(isLeftHand){
					FRENewObject( (const uint8_t*) "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSHand", 0, NULL, &freJoint, NULL);
				}else if(isRightHand){
					FRENewObject( (const uint8_t*) "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSHand", 0, NULL, &freJoint, NULL);
				}else{
					FRENewObject( (const uint8_t*) _asJointClass, 0, NULL, &freJoint, NULL);
				}

				FRESetObjectProperty(freJoint, (const uint8_t*) "name", createFREObjectForUTF8(_jointNames[skeletonJoint->skeletonJoint->jointNameIndex]), NULL);
				FRESetObjectProperty(freJoint, (const uint8_t*) "position", skeletonJoint->skeletonJoint->position.asFREObject(), NULL);
				FRESetObjectProperty(freJoint, (const uint8_t*) "positionConfidence", createFREObjectForDouble(skeletonJoint->skeletonJoint->positionConfidence), NULL);
				
				FRESetObjectProperty(freJoint, (const uint8_t*) "nativeHierarchicalRotationQuaternion", skeletonJoint->hierarchicalRotationQuaternion.asFREObject(), NULL);
				FRESetObjectProperty(freJoint, (const uint8_t*) "nativeHierarchicalRotationMatrix", skeletonJoint->hierarchicalRotationMatrix.asFREObject(), NULL);
				FRESetObjectProperty(freJoint, (const uint8_t*) "nativeAbsoluteRotationQuaternion", skeletonJoint->absoluteRotationQuaternion.asFREObject(), NULL);
				FRESetObjectProperty(freJoint, (const uint8_t*) "nativeAbsoluteRotationMatrix", skeletonJoint->absoluteRotationMatrix.asFREObject(), NULL);
				
				if(isLeftHand) {
					FRESetObjectProperty(freJoint, (const uint8_t*) "event", createFREObjectForUnsignedInt(user->leftHandInfo.HandEventType), NULL);
					FRESetObjectProperty(freJoint, (const uint8_t*) "state", createFREObjectForUnsignedInt(user->leftHandInfo.State), NULL);
				} else if(isRightHand) {
					FRESetObjectProperty(freJoint, (const uint8_t*) "event", createFREObjectForUnsignedInt(user->rightHandInfo.HandEventType), NULL);
					FRESetObjectProperty(freJoint, (const uint8_t*) "state", createFREObjectForUnsignedInt(user->rightHandInfo.State), NULL);
				}

				FRESetArrayElementAt(freJoints, j, freJoint);
			}

			FRESetObjectProperty(freUser, (const uint8_t*) "skeletonJoints", freJoints, NULL);

			FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.SkeletonBone>", 0, NULL, &freBones, NULL);

			for(int j = 0; j < user->user->numBones; j++)
			{
				AKMSSDKSkeletonBone* skeletonBone = &user->mssdkSkeletonBones[j];
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
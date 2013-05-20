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
#ifndef KinectExtension_AKMSSDKUserFrameGenerator_h
#define KinectExtension_AKMSSDKUserFrameGenerator_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_MSSDK

#include "../AKUserFrameGenerator.h"
#include "../AKUtilityFunctions.h"
#include "Data/AKMSSDKUserFrame.h"

class AKMSSDKUserFrameGenerator : public AKUserFrameGenerator
{
public:

	AKMSSDKUserFrameGenerator();
	~AKMSSDKUserFrameGenerator();

	void setRGBTargetSize(int width, int height);
	void setDepthTargetSize(int width, int height);

	void setSeatedSkeletonEnabled(bool seatedSkeletonEnabled);
	void setNuiSensor(INuiSensor* nuiSensor);
	void setNuiInteractionStream(INuiInteractionStream* nuiInteractionStream);
	void setTransformSmoothingParameters(NUI_TRANSFORM_SMOOTH_PARAMETERS transformSmoothingParameters);

	void generateUserFrame();
	void addInteractionInfo(NUI_INTERACTION_FRAME* interactionFrame);
	FREObject getFREObject();

protected:

	void allocateUserFrame();
	void deallocateUserFrame();

	void allocateJointNames();
	void deallocateJointNames();

	void allocateBoneNames();
	void deallocateBoneNames();

private:

	AKMSSDKUserFrame* _mssdkUserFrame;

	bool _seatedSkeletonEnabled;
	INuiSensor* _nuiSensor;
	INuiInteractionStream* _nuiInteractionStream;

	NUI_IMAGE_RESOLUTION _targetDepthResolution;
	NUI_IMAGE_RESOLUTION _targetRGBResolution;

	NUI_TRANSFORM_SMOOTH_PARAMETERS _transformSmoothingParameters;

	void allocateJointNamesForSeatedSkeletonTracking();
	void allocateJointNamesForRegularSkeletonTracking();

	void allocateBoneNamesForSeatedSkeletonTracking();
	void allocateBoneNamesForRegularSkeletonTracking();

	void addJointElements(AKMSSDKUser &mssdkUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations);
	void addJointElementsForSeatedSkeletonTracking(AKMSSDKUser &mssdkUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations);
	void addJointElementsForRegularSkeletonTracking(AKMSSDKUser &mssdkUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations);
	
	void addBoneElements(AKMSSDKUser &mssdkUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations);
	void addBoneElementsForSeatedSkeletonTracking(AKMSSDKUser &mssdkUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations);
	void addBoneElementsForRegularSkeletonTracking(AKMSSDKUser &mssdkUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations);
	
	void setJointProperties(AKMSSDKSkeletonJoint &mssdkSkeletonJoint, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations, NUI_SKELETON_POSITION_INDEX eJoint);
	void calculatePosition(AKPosition &kTransform, Vector4 skeletonTransform);

	void setBoneProperties(AKMSSDKSkeletonBone &mssdkSkeletonBone, NUI_SKELETON_BONE_ORIENTATION *boneOrientations, NUI_SKELETON_POSITION_INDEX indexOfJointWithRotation, int startJointNameIndex, int endJointNameIndex, int parentBoneNameIndex);

};

#endif
#endif
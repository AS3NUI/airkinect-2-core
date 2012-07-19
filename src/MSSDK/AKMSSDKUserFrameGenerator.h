#ifndef KinectExtension_AKMSSDKUserFrameGenerator_h
#define KinectExtension_AKMSSDKUserFrameGenerator_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_MSSDK

#include "../AKUserFrameGenerator.h"
#include "../AKUtilityFunctions.h"

class AKMSSDKUserFrameGenerator : public AKUserFrameGenerator
{
public:

	AKMSSDKUserFrameGenerator();
	~AKMSSDKUserFrameGenerator();

	void setRGBTargetSize(int width, int height);
	void setDepthTargetSize(int width, int height);

	void setSeatedSkeletonEnabled(bool seatedSkeletonEnabled);
	void setNuiSensor(INuiSensor* nuiSensor);
	void setTransformSmoothingParameters(NUI_TRANSFORM_SMOOTH_PARAMETERS transformSmoothingParameters);

	void generateUserFrame();
	FREObject getFREObject();

protected:

	void allocateUserFrame();
	void deallocateUserFrame();

	void allocateJointNames();
	void deallocateJointNames();

private:

	bool _seatedSkeletonEnabled;
	INuiSensor* _nuiSensor;

	NUI_IMAGE_RESOLUTION _targetDepthResolution;
	NUI_IMAGE_RESOLUTION _targetRGBResolution;

	NUI_TRANSFORM_SMOOTH_PARAMETERS _transformSmoothingParameters;

	void allocateJointNamesForSeatedSkeletonTracking();
	void allocateJointNamesForRegularSkeletonTracking();

	void addJointElements(AKUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations);
	void addJointElementsForSeatedSkeletonTracking(AKUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations);
	void addJointElementsForRegularSkeletonTracking(AKUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations);
	void addJointElement(AKUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations, NUI_SKELETON_POSITION_INDEX eJoint, uint32_t targetIndex);
	void calculatePosition(AKPosition &kTransform, Vector4 skeletonTransform);

};

#endif
#endif
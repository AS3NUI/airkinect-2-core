#ifndef KinectExtension_AKOpenNIUserFrameGenerator_h
#define KinectExtension_AKOpenNIUserFrameGenerator_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_OPENNI

#include "../AKUserFrameGenerator.h"
#include <XnCppWrapper.h>

class AKOpenNIUserFrameGenerator : public AKUserFrameGenerator
{
	static const int        MAX_DEPTH = 10000;
public:

	AKOpenNIUserFrameGenerator();
	~AKOpenNIUserFrameGenerator();

	void setDepthGenerator(xn::DepthGenerator* depthGenerator);
	void setUserGenerator(xn::UserGenerator* userGenerator);

	void generateUserFrame();
	FREObject getFREObject();

private:
	
	const char* _asJointClass;
	const char* _asUserClass;
	const char* _asUserFrameClass;

	xn::DepthGenerator* _depthGenerator;
	xn::UserGenerator* _userGenerator;

	void allocateUserFrame();
	void deallocateUserFrame();

	void allocateJointNames();
	void deallocateJointNames();

	FREObject freGetSkeletonJointNameIndices();
	FREObject freGetSkeletonJointNames();

	void addJointElement(AKUser &kUser, XnUserID user, XnSkeletonJoint eJoint, unsigned int targetIndex);

};

#endif
#endif
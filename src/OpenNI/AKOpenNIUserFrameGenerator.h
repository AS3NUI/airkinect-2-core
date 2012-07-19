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

protected:

	void allocateUserFrame();
	void deallocateUserFrame();

	void allocateJointNames();
	void deallocateJointNames();

private:

	xn::DepthGenerator* _depthGenerator;
	xn::UserGenerator* _userGenerator;

	void addJointElement(AKUser &kUser, XnUserID user, XnSkeletonJoint eJoint, unsigned int targetIndex);
	void calculatePosition(AKPosition &akPosition, XnPoint3D xnPosition);

};

#endif
#endif
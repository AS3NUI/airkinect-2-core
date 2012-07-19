#ifndef KinectExtension_AKOpenNIUserFrameGenerator_h
#define KinectExtension_AKOpenNIUserFrameGenerator_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_OPENNI

#include "../AKUserFrameGenerator.h"
#include "Data/AKOpenNIUserFrame.h"

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

	AKOpenNIUserFrame* _openNIUserFrame;

	xn::DepthGenerator* _depthGenerator;
	xn::UserGenerator* _userGenerator;

	void setJointProperties(AKOpenNISkeletonJoint &openNISkeletonJoint, XnUserID user, XnSkeletonJoint eJoint);
	void calculatePosition(AKPosition &akPosition, XnPoint3D xnPosition);

};

#endif
#endif
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

	void allocateBoneNames();
	void deallocateBoneNames();

private:

	AKOpenNIUserFrame* _openNIUserFrame;

	xn::DepthGenerator* _depthGenerator;
	xn::UserGenerator* _userGenerator;

	void addJointElements(AKOpenNIUser &openNIUser, XnUserID userID);
	void addBoneElements(AKOpenNIUser &openNIUser, XnUserID userID);

	void setJointProperties(AKOpenNISkeletonJoint &openNISkeletonJoint, XnUserID user, XnSkeletonJoint eJoint);
	void calculatePosition(AKPosition &akPosition, XnPoint3D xnPosition);

	void setBoneProperties(AKOpenNISkeletonBone &openNISkeletonBone, AKOpenNISkeletonJoint &jointWhichContainsOrientation, double zDegreesCorrection, int startJointNameIndex, int endJointNameIndex, int parentBoneNameIndex);

};

#endif
#endif
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
#ifndef KinectExtension_OpenNIDevice_h
#define KinectExtension_OpenNIDevice_h

#include "ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_OPENNI

#ifdef AIRKINECT_OS_WINDOWS
#else
    #include <Adobe AIR/Adobe AIR.h>
    #include <boost/thread.hpp>
    #include <boost/thread/mutex.hpp>
#endif

#include <XnCppWrapper.h>
#include "KinectDevice.h"
#include "KinectCapabilities.h"
#include "PointCloudRegion.h"
#include "AKOpenNIRGBParser.h"
#include "AKOpenNIDepthParser.h"

class OpenNIDevice : KinectDevice
{
    static const int        MAX_DEPTH = 10000;
public:
    
    OpenNIDevice(int nr, xn::Context context);
    
	//Starts and Stop the Kinect Device
    void                    start();
    void                    stop();
    
	//Dispose the Device from memory
    void                    dispose();
    
    FREObject               freSetInfraredMode(FREObject argv[]);
    FREObject               freSetInfraredEnabled(FREObject argv[]);
    FREObject               freGetInfraredFrame(FREObject argv[]);
    
protected:
    
 	void                    setRGBMode(int rgbWidth, int rgbHeight, int asRGBWidth, int asRGBHeight, bool asRGBMirrored);
	void                    setUserColor(int userID, int color, bool useIntensity);
	void					setDefaults();
	void					cleanupByteArrays();
	void					setNumJointsAndJointNames();

	void					createPointCloudGenerator();
	void					createUserMasksGenerator();
	void					createUserFrameGenerator();
    
private:
    xn::Context             context;
    
    void                    run();
    
    static void             *deviceThread(void *ptr);
    
    int                     returnVal;
    
    XnCallbackHandle        userHandle;
    XnCallbackHandle        userExitHandle;
    XnCallbackHandle        userReentryHandle;
    XnCallbackHandle        calibrationStartHandle;
    XnCallbackHandle        calibrationCompleteHandle;
    XnCallbackHandle        poseDetectedHandle;
    XnCallbackHandle        outOfPoseHandle;
    
    int                     frameRate;
    
    bool                    userCallbacksRegistered;
    
	void                    addJointElement(AKUser &kUser, XnUserID user, XnSkeletonJoint eJoint, uint32_t targetIndex);
    
    float                   **userIndexColors;//[MAX_SKELETONS][4];
    
    int                     asInfraredWidth;
    int                     asInfraredHeight;
    int                     asInfraredPixelCount;
    bool                    asInfraredMirrored;
    bool                    asInfraredEnabled;
    
    int                     infraredWidth;
    int                     infraredHeight;
    int                     infraredPixelCount;
    int                     infraredScale;
    
    uint32_t                *asInfraredByteArray;
    
    xn::ImageGenerator      imageGenerator;
    xn::DepthGenerator      depthGenerator;
    xn::UserGenerator       userGenerator;
    xn::IRGenerator         infraredGenerator;

	bool					imageGeneratorCreated;
	bool					depthGeneratorCreated;
	bool					userGeneratorCreated;
	bool					infraredGeneratorCreated;
    
    xn::ImageMetaData       imageMetaData;
    xn::DepthMetaData       depthMetaData;
    xn::SceneMetaData       sceneMetaData;
    xn::IRMetaData          infraredMetaData;
    
	void					readRGBFrame();
	void					readDepthFrame();

	AKOpenNIRGBParser*		rgbParser;
    
    const XnIRPixel         *infraredFrameBuffer;
	AKOpenNIDepthParser*	depthParser;

	void					dispatchRGBIfNeeded();
	void					dispatchDepthIfNeeded();
	void					dispatchInfraredIfNeeded();
	void					dispatchPointCloudIfNeeded();
	void					dispatchUserMaskIfNeeded();
	void					dispatchUserFrameIfNeeded();
};

#endif
#endif

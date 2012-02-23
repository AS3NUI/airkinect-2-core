//
//  OpenNIDevice.h
//  KinectExtension
//
//  Created by Wouter Verweirder on 24/01/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef KinectExtension_OpenNIDevice_h
#define KinectExtension_OpenNIDevice_h

#include <Adobe AIR/Adobe AIR.h>
#include <XnCppWrapper.h>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "IKinectDevice.h"
#include "KinectSkeleton.h"
#include "KinectCapabilities.h"
#include "PointCloudRegion.h"

class OpenNIDevice : IKinectDevice
{
    static const int        MAX_DEPTH = 10000;
public:
    static KinectCapabilities	getCapabilities(){
		KinectCapabilities kinectCapabilities;
		
		kinectCapabilities.hasCameraElevationSupport				= false;
		kinectCapabilities.hasDepthCameraSupport					= true;
		kinectCapabilities.hasDepthUserSupport						= true;
		kinectCapabilities.hasInfraredSupport						= true;
		kinectCapabilities.hasJointOrientationConfidenceSupport		= true;
		kinectCapabilities.hasJointOrientationSupport				= true;
		kinectCapabilities.hasMultipleSensorSupport					= false;
		kinectCapabilities.hasPointCloudRegionSupport				= true;
		kinectCapabilities.hasPointCloudSupport						= true;
		kinectCapabilities.hasPositionConfidenceSupport				= true;
		kinectCapabilities.hasRGBCameraSupport						= true;
		kinectCapabilities.hasSkeletonSupport						= true;
		kinectCapabilities.hasUserMaskSupport						= true;
        
		kinectCapabilities.maxSensors								= 1;
		kinectCapabilities.framework								= "openni";
		return kinectCapabilities;
	};
    
    OpenNIDevice(int nr, xn::Context context);
    
    //Getter/Setters for FREContext
    FREContext			getFreContext();
	void				setFreContext(FREContext pFreContext);
    
	//Starts and Sotp the Kinect Device
    void				start();
    void				stop();
    
	//Dispose the Device form memory
    void				dispose();
    
    FREObject           freSetUserMode(FREObject argv[]);
    FREObject           freSetUserColor(FREObject argv[]);
    FREObject           freSetUserEnabled(FREObject argv[]);
    FREObject           freSetSkeletonMode(FREObject argv[]);
    FREObject           freSetSkeletonEnabled(FREObject argv[]);
    FREObject           freGetUserFrame(FREObject argv[]);
    FREObject           freGetSkeletonJointNameIndices(FREObject argv[]);
    FREObject           freGetSkeletonJointNames(FREObject argv[]);
    FREObject           freSetUserMaskMode(FREObject argv[]);
    FREObject           freSetUserMaskEnabled(FREObject argv[]);
    FREObject           freGetUserMaskFrame(FREObject argv[]);
    FREObject           freSetDepthMode(FREObject argv[]);
    FREObject           freSetDepthEnabled(FREObject argv[]);
    FREObject           freGetDepthFrame(FREObject argv[]);
    FREObject           freSetDepthShowUserColors(FREObject argv[]);
    FREObject           freSetRGBMode(FREObject argv[]);
    FREObject           freSetRGBEnabled(FREObject argv[]);
    FREObject           freGetRGBFrame(FREObject argv[]);
    FREObject           freSetInfraredMode(FREObject argv[]);
    FREObject           freSetInfraredEnabled(FREObject argv[]);
    FREObject           freGetInfraredFrame(FREObject argv[]);
    FREObject           freSetPointCloudMode(FREObject argv[]);
    FREObject           freSetPointCloudEnabled(FREObject argv[]);
    FREObject           freGetPointCloudFrame(FREObject argv[]);
    FREObject           freSetPointCloudRegions(FREObject argv[]);
    
private:
    int                     nr;
    xn::Context             context;
    FREContext              freContext;
    
    boost::mutex            userMutex;
    boost::mutex            depthMutex;
    boost::mutex            rgbMutex;
    boost::mutex            userMaskMutex;
    boost::mutex            infraredMutex;
    boost::mutex            pointCloudMutex;
    
    void                    lockUserMutex();
    void                    unlockUserMutex();
    
    void                    lockDepthMutex();
    void                    unlockDepthMutex();
    
    void                    lockRGBMutex();
    void                    unlockRGBMutex();
    
    void                    lockUserMaskMutex();
    void                    unlockUserMaskMutex();
    
    void                    lockInfraredMutex();
    void                    unlockInfraredMutex();
    
    void                    lockPointCloudMutex();
    void                    unlockPointCloudMutex();
    
    void                    setUserColor(int userID, int color, bool useIntensity);
    int                     getAsPointCloudByteArrayLength();
    
    kinectUserFrame         userFrame;
    uint32_t                *depthByteArray;
    uint32_t                *RGBByteArray;
    uint32_t                **userMaskByteArray;
    uint32_t                *infraredByteArray;
    short                   *pointCloudByteArray;
    PointCloudRegion        *pointCloudRegions;
    unsigned int            numRegions;
    
    
    void                    setDefaults();
    
    void                    run();
    
    static void             *deviceThread(void *ptr);
    boost::thread           mThread;
    
    int                     returnVal;
    bool                    running;
    bool                    started;
    
    bool                    asUserMirrored;
    bool                    asUserEnabled;
    bool                    asSkeletonMirrored;
    bool                    asSkeletonEnabled;
    
    XnCallbackHandle        userHandle;
    XnCallbackHandle        userExitHandle;
    XnCallbackHandle        userReentryHandle;
    XnCallbackHandle        calibrationStartHandle;
    XnCallbackHandle        calibrationCompleteHandle;
    XnCallbackHandle        poseDetectedHandle;
    XnCallbackHandle        outOfPoseHandle;
    
    bool                    userCallbacksRegistered;
    
    void                    userHandler();
    void                    addJointElement(kinectUser &kUser, XnUserID user, XnSkeletonJoint eJoint, uint32_t targetIndex);
    
    int                     asDepthWidth;
    int                     asDepthHeight;
    int                     asDepthPixelCount;
    bool                    asDepthMirrored;
    bool                    asDepthEnabled;
    bool                    asDepthShowUserColors;
    
    int                     depthWidth;
    int                     depthHeight;
    int                     depthPixelCount;
    int                     depthScale;
    
    float                   depthHistogram[MAX_DEPTH];
    
    float                   userIndexColors[MAX_SKELETONS][4];
    
    int                     asRGBWidth;
    int                     asRGBHeight;
    int                     asRGBPixelCount;
    bool                    asRGBMirrored;
    bool                    asRGBEnabled;
    
    int                     rgbWidth;
    int                     rgbHeight;
    int                     rgbPixelCount;
    int                     rgbScale;
    
    int                     asUserMaskWidth;
    int                     asUserMaskHeight;
    int                     asUserMaskPixelCount;
    bool                    asUserMaskMirrored;
    bool                    asUserMaskEnabled;
    
    int                     userMaskWidth;
    int                     userMaskHeight;
    int                     userMaskPixelCount;
    int                     userMaskScale;
    
    int                     asInfraredWidth;
    int                     asInfraredHeight;
    int                     asInfraredPixelCount;
    bool                    asInfraredMirrored;
    bool                    asInfraredEnabled;
    
    int                     infraredWidth;
    int                     infraredHeight;
    int                     infraredPixelCount;
    int                     infraredScale;
    
    int                     asPointCloudWidth;
    int                     asPointCloudHeight;
    int                     asPointCloudPixelCount;
    bool                    asPointCloudMirrored;
    bool                    asPointCloudEnabled;
    int                     asPointCloudDensity;
    bool                    asPointCloudIncludeRGB;
    
    int                     pointCloudWidth;
    int                     pointCloudHeight;
    int                     pointCloudPixelCount;
    int                     pointCloudScale;
    
    xn::ImageGenerator      imageGenerator;
    xn::DepthGenerator      depthGenerator;
    xn::UserGenerator       userGenerator;
    xn::IRGenerator         infraredGenerator;
    
    xn::ImageMetaData       imageMetaData;
    xn::DepthMetaData       depthMetaData;
    xn::SceneMetaData       sceneMetaData;
    xn::IRMetaData          infraredMetaData;
    
    const XnRGB24Pixel      *RGBFrameBuffer;
    void                    rgbFrameHandler();
    
    const XnDepthPixel      *depthFrameBuffer;
    const XnLabel           *sceneFrameBuffer;
    
    const XnIRPixel         *infraredFrameBuffer;
    void                    infraredHandler();
    
    void                    calculateHistogram();
    void                    depthFrameHandler();
    void                    depthFrameWithUserColorsHandler();
    
    void                    userMaskHandler();
    void                    pointCloudHandler();
    void                    pointCloudWithRGBHandler();
};

#endif

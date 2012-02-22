//
//  OpenNIDevice.h
//  KinectExtension
//
//  Created by Wouter Verweirder on 24/01/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef KinectExtension_OpenNIDevice_h
#define KinectExtension_OpenNIDevice_h

#include <iostream>
#include <Adobe AIR/Adobe AIR.h>
#include <XnCppWrapper.h>

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
    
	//Sets the color of the player for Depth + Player index image
	void				setUserColor(int userID, int color, bool useIntensity);
    
	//User and Skeleton Feature Setters
    void				setUserMode(bool mirrored);
    void				setUserEnabled(bool enabled);
    void				setSkeletonMode(bool mirrored);
    void				setSkeletonEnabled(bool enabled);
	
	//Should return the current User Frame Data
	kinectUserFrame		getUserFrameBuffer();
    
	//Depth Image Feature Setters
    void				setDepthMode(unsigned int width, unsigned int height, bool mirrored);
    void				setDepthEnabled(bool enabled);
    void				setDepthShowUserColors(bool show);
    
	//RGB Image Feature Setters
    void				setRGBMode(unsigned int width, unsigned int height, bool mirrored);
    void				setRGBEnabled(bool enabled);
    
	//IR Camera Feature Setters
	void				setInfraredMode(unsigned int width, unsigned int height, bool mirrored);
	void				setInfraredEnabled(bool enabled);
    
	//User Mask Image Feature Setters
    void				setUserMaskMode(unsigned int width, unsigned int height, bool mirrored);
    void				setUserMaskEnabled(bool enabled);
    
	//Point Cloud Feature Setters
    void				setPointCloudMode(unsigned int width, unsigned int height, bool mirrored, unsigned int density, bool includeRGB);
    void				setPointCloudEnabled(bool enabled);
    void				setPointCloudRegions(PointCloudRegion *pointCloudRegions, unsigned int numRegions);
	
	//Depth Image Accessors
	int					getAsDepthWidth();
    int					getAsDepthHeight();
	//Returns the current Depth Frame as a byte array
	uint32_t*			getAsDepthByteArray();
    
	//RGB Image Accessors
    int					getAsRGBWidth();
    int					getAsRGBHeight();
	//Returns the current RGB Frame as a byte array
    uint32_t*			getAsRGBByteArray();
    
	//User Mask Image Accessors
    int					getAsUserMaskWidth();
    int					getAsUserMaskHeight();
	//Returns the current Player Mask Frame as a byte array
    uint32_t*			getAsUserMaskByteArray(int userID);
    
	//IR Image Accessors
	int					getAsInfraredWidth();
	int					getAsInfraredHeight();
	uint32_t*           getAsInfraredByteArray();
    
	//Point Cloud Accessors
    int					getAsPointCloudWidth();
    int					getAsPointCloudHeight();
    bool				getASPointCloudMirror();
    int					getASPointCloudDensity();
    bool				getASPointCloudIncludeRGB();
    int					getAsPointCloudByteArrayLength();
	//Returns the current Point cloud data as byte array (x,y,z) format
    short*             getAsPointCloudByteArray();
    
	//Region Accessors
	//Returns the current poinbt cloud regions
    PointCloudRegion*	getPointCloudRegions();
    unsigned int		getNumRegions();
        
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
    
private:
    int                     nr;
    xn::Context             context;
    FREContext              freContext;
    
    pthread_mutex_t         userMutex;
    pthread_mutex_t         depthMutex;
    pthread_mutex_t         rgbMutex;
    pthread_mutex_t         userMaskMutex;
    pthread_mutex_t         infraredMutex;
    pthread_mutex_t         pointCloudMutex;
    
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
    pthread_attr_t          attr;
    pthread_t               posixThreadID;
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

//
//  KinectDevice.h
//  KinectExtension
//
//  Created by Wouter Verweirder on 24/01/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef KinectExtension_KinectDevice_h
#define KinectExtension_KinectDevice_h

#include <iostream>
#include <Adobe AIR/Adobe AIR.h>
#include <XnCppWrapper.h>
#include "KinectSkeleton.h"

class KinectDevice
{
    static const int        MAX_DEPTH = 10000;
public:
    KinectDevice(int nr, xn::Context context);
    
    FREContext              freContext;
    
    void                    start();
    void                    stop();
    
    void                    dispose();
    
    void                    setUserMode(bool mirrored);
    void                    setUserEnabled(bool enabled);
    void                    setSkeletonMode(bool mirrored);
    void                    setSkeletonEnabled(bool enabled);
    
    void                    setDepthMode(unsigned int width, unsigned int height, bool mirrored);
    void                    setDepthEnabled(bool enabled);
    void                    setDepthShowUserColors(bool show);
    
    void                    setRGBMode(unsigned int width, unsigned int height, bool mirrored);
    void                    setRGBEnabled(bool enabled);
    
    void                    setUserMaskMode(unsigned int width, unsigned int height, bool mirrored);
    void                    setUserMaskEnabled(bool enabled);
    
    void                    setInfraredMode(unsigned int width, unsigned int height, bool mirrored);
    void                    setInfraredEnabled(bool enabled);
    
    void                    setPointCloudMode(unsigned int width, unsigned int height, bool mirrored);
    void                    setPointCloudEnabled(bool enabled);
    
    kinectUserFrame         userFrame;
    pthread_mutex_t         userMutex;
    
    int                     getAsDepthWidth();
    int                     getAsDepthHeight();
    uint32_t                *depthByteArray;
    pthread_mutex_t         depthMutex;
    
    int                     getAsRGBWidth();
    int                     getAsRGBHeight();
    uint32_t                *RGBByteArray;
    pthread_mutex_t         rgbMutex;
    
    int                     getAsUserMaskWidth();
    int                     getAsUserMaskHeight();
    uint32_t                *userMaskByteArray;
    pthread_mutex_t         userMaskMutex;
    
    int                     getAsInfraredWidth();
    int                     getAsInfraredHeight();
    uint32_t                *infraredByteArray;
    pthread_mutex_t         infraredMutex;
    
    int                     getAsPointCloudWidth();
    int                     getAsPointCloudHeight();
    ushort                  *pointCloudByteArray;
    pthread_mutex_t         pointCloudMutex;
    
private:
    int                     nr;
    xn::Context             context;
    
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
};

#endif

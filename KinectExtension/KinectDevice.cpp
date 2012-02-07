//
//  KinectDevice.cpp
//  KinectExtension
//
//  Created by Wouter Verweirder on 24/01/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <math.h>
#include "KinectDevice.h"

//colors for depth image
const XnFloat Colors[][3] =
{
	{0,1,1},
	{0,0,1},
	{0,1,0},
	{1,1,0},
	{1,0,0},
	{1,.5,0},
	{.5,1,0},
	{0,.5,1},
	{.5,0,1},
	{1,1,.5},
	{1,1,1}
};
const XnUInt32 nColors = 10;

XnBool needPose = FALSE;
XnChar strPose[20] = "";

void XN_CALLBACK_TYPE newUserCallback(xn::UserGenerator& rGenerator, XnUserID nID, void* pCookie);
void XN_CALLBACK_TYPE lostUserCallback(xn::UserGenerator& rGenerator, XnUserID nID, void* pCookie);
void XN_CALLBACK_TYPE userExitCallback(xn::UserGenerator& rGenerator, XnUserID nID, void* pCookie);
void XN_CALLBACK_TYPE userReentryCallback(xn::UserGenerator& rGenerator, XnUserID nID, void* pCookie);
void XN_CALLBACK_TYPE poseDetectedCallback(xn::PoseDetectionCapability& rCapability, const XnChar* strPose, XnUserID nID, void* pCookie);
void XN_CALLBACK_TYPE outOfPoseCallback(xn::PoseDetectionCapability& rCapability, const XnChar* strPose, XnUserID nID, void* pCookie);
void XN_CALLBACK_TYPE calibrationStartCallback(xn::SkeletonCapability& rCapability, XnUserID nID, void* pCookie);
void XN_CALLBACK_TYPE calibrationCompleteCallback(xn::SkeletonCapability& rCapability, XnUserID nID, XnCalibrationStatus calibrationError, void* pCookie);

KinectDevice::KinectDevice(int nr, xn::Context context)
{
    printf("KinectDevice::KinectDevice(%i)\n", nr);
    this->nr = nr;
    this->freContext = freContext;
    this->context = context;
    
    //initialize the mutexes
    pthread_mutex_init(&userMutex, NULL);
    pthread_mutex_init(&depthMutex, NULL);
    pthread_mutex_init(&rgbMutex, NULL);
    pthread_mutex_init(&userMaskMutex, NULL);
    pthread_mutex_init(&infraredMutex, NULL);
    pthread_mutex_init(&pointCloudMutex, NULL);
    
    //set default values
    setDefaults();
}

void KinectDevice::setDefaults()
{
    //set some default values
    running = false;
    started = false;
    userCallbacksRegistered = false;
    
    asUserMirrored = false;
    asUserEnabled = false;
    asSkeletonMirrored = false;
    asSkeletonEnabled = false;
    
    asDepthWidth = 320;
    asDepthHeight = 240;
    asDepthPixelCount = asDepthWidth * asDepthHeight;
    asDepthMirrored = false;
    asDepthEnabled = false;
    asDepthShowUserColors = false;
    
    depthWidth = 640;
    depthHeight = 480;
    depthPixelCount = depthWidth * depthHeight;
    depthScale = depthWidth / asDepthWidth;
    
    depthGenerator = NULL;
    
    depthByteArray = 0;
    
    asRGBWidth = 320;
    asRGBHeight = 240;
    asRGBPixelCount = asRGBWidth * asRGBHeight;
    asRGBMirrored = false;
    asRGBEnabled = false;
    
    rgbWidth = 640;
    rgbHeight = 480;
    rgbPixelCount = rgbWidth * rgbHeight;
    rgbScale = rgbWidth / asRGBWidth;
    
    imageGenerator = NULL;
    
    RGBByteArray = 0;
    
    userGenerator = NULL;
    
    asUserMaskWidth = 320;
    asUserMaskHeight = 240;
    asUserMaskPixelCount = asUserMaskWidth * asUserMaskHeight;
    asUserMaskMirrored = false;
    asUserMaskEnabled = false;
    
    userMaskWidth = 640;
    userMaskHeight = 480;
    userMaskPixelCount = userMaskWidth * userMaskHeight;
    userMaskScale = userMaskWidth / asUserMaskWidth;
    
    userMaskByteArray = 0;
    
    asInfraredWidth = 320;
    asInfraredHeight = 240;
    asInfraredPixelCount = asInfraredWidth * asInfraredHeight;
    asInfraredMirrored = false;
    asInfraredEnabled = false;
    
    infraredWidth = 640;
    infraredHeight = 480;
    infraredPixelCount = infraredWidth * infraredHeight;
    infraredScale = infraredWidth / asInfraredWidth;
    
    infraredGenerator = NULL;
    
    infraredByteArray = 0;
    
    asPointCloudWidth = 320;
    asPointCloudHeight = 240;
    asPointCloudPixelCount = asPointCloudWidth * asPointCloudHeight;
    asPointCloudMirrored = false;
    asPointCloudEnabled = false;
    asPointCloudDensity = 1;
    asPointCloudIncludeRGB = false;
    
    pointCloudWidth = 640;
    pointCloudHeight = 480;
    pointCloudPixelCount = pointCloudWidth * pointCloudHeight;
    pointCloudScale = pointCloudWidth / asPointCloudWidth;
    
    pointCloudByteArray = 0;
    pointCloudRegions = 0;
}

int KinectDevice::getAsDepthWidth()
{
    return asDepthWidth;
}

int KinectDevice::getAsDepthHeight()
{
    return asDepthHeight;
}

int KinectDevice::getAsRGBWidth()
{
    return asRGBWidth;
}

int KinectDevice::getAsRGBHeight()
{
    return asRGBHeight;
}

int KinectDevice::getAsUserMaskWidth()
{
    return asUserMaskWidth;
}

int KinectDevice::getAsUserMaskHeight()
{
    return asUserMaskHeight;
}

int KinectDevice::getAsInfraredWidth()
{
    return asInfraredWidth;
}

int KinectDevice::getAsInfraredHeight()
{
    return asInfraredHeight;
}

int KinectDevice::getAsPointCloudWidth()
{
    return asPointCloudWidth;
}

int KinectDevice::getAsPointCloudHeight()
{
    return asPointCloudHeight;
}

int KinectDevice::getAsPointCloudByteArrayLength()
{
    if(asPointCloudIncludeRGB)
    {
        return (asPointCloudWidth * asPointCloudHeight * sizeof(ushort) * 6) / asPointCloudDensity;
    }
    else
    {
        return (asPointCloudWidth * asPointCloudHeight * sizeof(ushort) * 3) / asPointCloudDensity;
    }
}

void KinectDevice::start()
{
    printf("KinectDevice::start()\n");
    if(!running)
    {
        returnVal = pthread_attr_init(&attr);
        returnVal = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        
        running = true;
        
        int threadError = pthread_create(&posixThreadID, &attr, deviceThread, (void *) this);
        printf("thread create code: %i\n", threadError);
    }
}

void KinectDevice::stop()
{
    printf("KinectDevice::stop()\n");
    if(running)
    {
        running = false;
        int threadError = pthread_join(posixThreadID, NULL);
        printf("thread join code: %d\n", threadError);
    }
    if(depthGenerator.IsValid())
    {
        if(depthGenerator.IsGenerating())
        {
            depthGenerator.StopGenerating();
        }
        depthGenerator.Release();
        depthGenerator = NULL;
    }
    if(imageGenerator.IsValid())
    {
        if(imageGenerator.IsGenerating())
        {
            imageGenerator.StopGenerating();
        }
        imageGenerator.Release();
        imageGenerator = NULL;
    }
    if(userGenerator.IsValid())
    {
        if(userGenerator.IsGenerating())
        {
            //stop generating
            userGenerator.StopGenerating();
        }
        if(userCallbacksRegistered)
        {
            //unregister skeleton detection callbacks
            userGenerator.UnregisterUserCallbacks(userHandle);
            userGenerator.UnregisterFromUserExit(userExitHandle);
            userGenerator.UnregisterFromUserReEnter(userReentryHandle);
            userGenerator.GetSkeletonCap().UnregisterFromCalibrationStart(calibrationStartHandle);
            userGenerator.GetSkeletonCap().UnregisterFromCalibrationComplete(calibrationCompleteHandle);
            userGenerator.GetPoseDetectionCap().UnregisterFromPoseDetected(poseDetectedHandle);
            userGenerator.GetPoseDetectionCap().UnregisterFromOutOfPose(outOfPoseHandle);
        }
        userGenerator.Release();
        userGenerator = NULL;
    }
    if(infraredGenerator.IsValid())
    {
        if(infraredGenerator.IsGenerating())
        {
            infraredGenerator.StopGenerating();
        }
        infraredGenerator.Release();
        infraredGenerator = NULL;
    }
    //cleanup bytearrays
    if(depthByteArray != 0) delete [] depthByteArray;
    if(RGBByteArray != 0) delete [] RGBByteArray;
    if(infraredByteArray != 0) delete [] infraredByteArray;
    if(pointCloudByteArray != 0) delete [] pointCloudByteArray;
    if(userMaskByteArray != 0)
    {
        for(int i = 0; i < MAX_SKELETONS; i++)
        {
            delete [] userMaskByteArray[i];
        }
        delete [] userMaskByteArray;
    }
    if(pointCloudRegions != 0)
    {
        delete [] pointCloudRegions;
    }
    //reset defaults
    setDefaults();
    if(started)
    {
        started = false;
        //send stopped event
        const uint8_t* statusCode = (const uint8_t*) "status";
        const uint8_t* level = (const uint8_t*) "stopped";
        FREDispatchStatusEventAsync(freContext, statusCode, level);
    }
}

void * KinectDevice::deviceThread(void *self)
{
    KinectDevice *adapter = (KinectDevice *) self;
    adapter->run();
    return NULL;
}

void KinectDevice::run()
{
    printf("KinectDevice::run(), %s\n", (running) ? "true" : "false");
    if(running)
    {  
        XnStatus rc;
        
        XnMapOutputMode depthMode;
        depthMode.nXRes = depthWidth;
        depthMode.nYRes = depthHeight;
        depthMode.nFPS = 30;
        
        rc = depthGenerator.Create(context);
        if(rc != XN_STATUS_OK)
        {
            printf("KinectDevice create depthGenerator failed: %s\n", xnGetStatusString(rc));
            stop();
            return;
        }
        depthGenerator.SetMapOutputMode(depthMode);
        depthGenerator.GetMirrorCap().SetMirror(false);
        
        //check if running is still true, as OpenNi takes a while to initialize
        if(!running)
        {
            stop();
            return;
        }  
        
        XnMapOutputMode rgbMode;
        rgbMode.nXRes = rgbWidth;
        rgbMode.nYRes = rgbHeight;
        rgbMode.nFPS = 30;
        
        rc = imageGenerator.Create(context);
        if(rc != XN_STATUS_OK)
        {
            printf("KinectDevice create imageGenerator failed: %s\n", xnGetStatusString(rc));
            stop();
            return;
        }
        imageGenerator.SetMapOutputMode(rgbMode);
        imageGenerator.GetMirrorCap().SetMirror(false);
        
        //check if running is still true, as OpenNi takes a while to initialize
        if(!running)
        {
            stop();
            return;
        }
        
        XnMapOutputMode infraredMode;
        infraredMode.nXRes = infraredWidth;
        infraredMode.nYRes = infraredHeight;
        infraredMode.nFPS = 30;
        
        rc = infraredGenerator.Create(context);
        if(rc != XN_STATUS_OK)
        {
            printf("KinectDevice create infraredGenerator failed: %s\n", xnGetStatusString(rc));
            stop();
            return;
        }
        infraredGenerator.SetMapOutputMode(infraredMode);
        infraredGenerator.GetMirrorCap().SetMirror(false);
        
        //check if running is still true, as OpenNi takes a while to initialize
        if(!running)
        {
            stop();
            return;
        }
        
        //map depth to rgb stream
        depthGenerator.GetAlternativeViewPointCap().SetViewPoint(imageGenerator);
        
        //initialize the user generator
        rc = userGenerator.Create(context);
        if(rc != XN_STATUS_OK)
        {
            printf("KinectDevice create userGenerator failed: %s\n", xnGetStatusString(rc));
            stop();
            return;
        }
        
        //check if running is still true, as OpenNi takes a while to initialize
        if(!running)
        {
            stop();
            return;
        }
        
        //register skeleton detection callbacks
        userGenerator.RegisterUserCallbacks(newUserCallback, lostUserCallback, (xn::UserGenerator *)&userGenerator, userHandle);
        userGenerator.RegisterToUserExit(userExitCallback, (xn::UserGenerator *)&userGenerator, userExitHandle);
        userGenerator.RegisterToUserReEnter(userReentryCallback, (xn::UserGenerator *)&userGenerator, userReentryHandle);
        userGenerator.GetSkeletonCap().RegisterToCalibrationStart(calibrationStartCallback, (xn::UserGenerator *)&userGenerator, calibrationStartHandle);
        userGenerator.GetSkeletonCap().RegisterToCalibrationComplete(calibrationCompleteCallback, (xn::UserGenerator *)&userGenerator, calibrationCompleteHandle);
        userGenerator.GetPoseDetectionCap().RegisterToPoseDetected(poseDetectedCallback, (xn::UserGenerator *)&userGenerator, poseDetectedHandle);
        userGenerator.GetPoseDetectionCap().RegisterToOutOfPose(outOfPoseCallback, (xn::UserGenerator *)&userGenerator, outOfPoseHandle);
        
        userCallbacksRegistered = true;
        
        if (!userGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
        {
            printf("Supplied user generator doesn't support skeleton\n");
        }
        
        if (userGenerator.GetSkeletonCap().NeedPoseForCalibration())
        {
            needPose = true;
            if (!userGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
            {
                printf("Pose required, but not supported\n");
            }
            else
            {
                userGenerator.GetSkeletonCap().GetCalibrationPose(strPose);
            }
        }
        else
        {
            needPose = false;
        }
        
        userGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
        userGenerator.GetSkeletonCap().SetSmoothing(0.5f);
        
        //check if running is still true, as OpenNi takes a while to initialize
        if(!running)
        {
            stop();
            return;
        }
        
        if(asDepthEnabled || asPointCloudEnabled)
        {
            rc = depthGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                printf("KinectDevice start depthGenerator failed: %s\n", xnGetStatusString(rc));
                stop();
                return;
            }
        }
        if(asRGBEnabled || asUserMaskEnabled || (asPointCloudEnabled && asPointCloudIncludeRGB))
        {
            rc = imageGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                printf("KinectDevice start imageGenerator failed: %s\n", xnGetStatusString(rc));
                stop();
                return;
            }
        }
        if((asDepthEnabled && asDepthShowUserColors) || asUserMaskEnabled || asUserEnabled || asSkeletonEnabled)
        {
            rc = userGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                printf("KinectDevice start userGenerator failed: %s\n", xnGetStatusString(rc));
                stop();
                return;
            }
        }
        //you cant have both infrared and rgb
        if(asInfraredEnabled && !imageGenerator.IsGenerating())
        {
            rc = infraredGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                printf("KinectDevice start infraredGenerator failed: %s\n", xnGetStatusString(rc));
                stop();
                return;
            }
        }
        
        //check if running is still true, as OpenNi takes a while to initialize
        if(!running)
        {
            stop();
            return;
        }
        
        //send started event
        started = true;
        const uint8_t* statusCode = (const uint8_t*) "status";
        const uint8_t* level = (const uint8_t*) "started";
        FREDispatchStatusEventAsync(freContext, statusCode, level);
        while(running)
        {
            XnStatus rc = XN_STATUS_OK;
            
            if(imageGenerator.IsGenerating())
            {
                //read a new RGB frame
                rc = imageGenerator.WaitAndUpdateData();
                if (rc != XN_STATUS_OK)
                {
                    printf("RGB read failed: %s\n", xnGetStatusString(rc));
                    break;
                }
                
                //get the rgb metadata
                imageGenerator.GetMetaData(imageMetaData);
            }
            
            if(depthGenerator.IsGenerating())
            {
                //read a new Depth frame
                rc = depthGenerator.WaitAndUpdateData();
                if (rc != XN_STATUS_OK)
                {
                    printf("Depth read failed: %s\n", xnGetStatusString(rc));
                    break;
                }
                
                //get the depth metadata
                depthGenerator.GetMetaData(depthMetaData);
                //calculate the histogram
                calculateHistogram();
            }
            
            if(infraredGenerator.IsGenerating())
            {
                //read a new Infrared frame
                rc = infraredGenerator.WaitAndUpdateData();
                if (rc != XN_STATUS_OK)
                {
                    printf("IR read failed: %s\n", xnGetStatusString(rc));
                    break;
                }
                
                //get the depth metadata
                infraredGenerator.GetMetaData(infraredMetaData);
            }
            
            if(userGenerator.IsGenerating())
            {
                //read a new User frame
                userGenerator.WaitAndUpdateData();
                //get the user pixels
                userGenerator.GetUserPixels(0, sceneMetaData);
            }
            
            //rgb image
            if(asRGBEnabled)
            {
                pthread_mutex_lock(&rgbMutex);
                rgbFrameHandler();
                pthread_mutex_unlock(&rgbMutex);
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "RGBFrame", (const uint8_t*) "");
            }
            
            //depth image
            if(asDepthEnabled)
            {
                pthread_mutex_lock(&depthMutex);
                if(asDepthShowUserColors)
                {
                    depthFrameWithUserColorsHandler();
                }
                else
                {
                    depthFrameHandler();
                }
                pthread_mutex_unlock(&depthMutex);
                //dispatch depth frame event
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "depthFrame", (const uint8_t*) "");
            }
            
            //infrared image - not available when rgb is enabled
            if(asInfraredEnabled && !imageGenerator.IsGenerating())
            {
                pthread_mutex_lock(&infraredMutex);
                infraredHandler();
                pthread_mutex_unlock(&infraredMutex);
                //dispatch infrared frame event
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "infraredFrame", (const uint8_t*) "");
            }
            
            //point cloud
            if(asPointCloudEnabled)
            {
                pthread_mutex_lock(&pointCloudMutex);
                if(asPointCloudIncludeRGB)
                {
                    pointCloudWithRGBHandler();
                }
                else
                {
                    pointCloudHandler();
                }
                pthread_mutex_unlock(&pointCloudMutex);
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "pointCloudFrame", (const uint8_t*) "");
            }
            
            //user information
            if(asUserEnabled || asSkeletonEnabled || asUserMaskEnabled)
            {
                pthread_mutex_lock(&userMutex);
                userHandler();
                pthread_mutex_unlock(&userMutex);
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "userFrame", (const uint8_t*) "");
            }
            
            //user mask image
            if(asUserMaskEnabled)
            {
                pthread_mutex_lock(&userMaskMutex);
                userMaskHandler();
                pthread_mutex_unlock(&userMaskMutex);
                //dispatch user mask frame event
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "userMaskFrame", (const uint8_t*) "");
            }
        }
    }
}

void KinectDevice::rgbFrameHandler()
{
    RGBFrameBuffer = imageMetaData.RGB24Data();
    
    if(RGBByteArray == 0) RGBByteArray = new uint32_t[asRGBPixelCount];
    
    uint32_t *rgbRun = RGBByteArray;
    int direction = asRGBMirrored ? -1 : 1;
    int directionFactor = asRGBMirrored ? 1 : 0;
    
    for(uint32_t y = 0; y < asRGBHeight; y++)
    {
        const XnRGB24Pixel *pRGBBuffer = RGBFrameBuffer + ((y + directionFactor) * (rgbWidth * rgbScale)) - directionFactor;
        
        for(uint32_t x = 0; x < asRGBWidth; x++)
        {
            *rgbRun = 0xff << 24 | ((*pRGBBuffer).nBlue + ((*pRGBBuffer).nGreen << 8) + ((*pRGBBuffer).nRed << 16));
            rgbRun++;
            pRGBBuffer += (rgbScale * direction);
        }
    }
}

void KinectDevice::depthFrameHandler()
{
    depthFrameBuffer = depthMetaData.Data();
    
    if(depthByteArray == 0) depthByteArray = new uint32_t[asDepthPixelCount];
    
    uint32_t *depthRun = depthByteArray;
    int direction = asDepthMirrored ? -1 : 1;
    int directionFactor = asDepthMirrored ? 1 : 0;
    
    unsigned int red, green, blue;
    float value;
    
    for(uint32_t y = 0; y < asDepthHeight; y++)
    {
        const XnDepthPixel *pDepthBuffer = depthFrameBuffer + ((y + directionFactor) * (depthWidth * depthScale)) - directionFactor;
        
        for(uint32_t x = 0; x < asDepthWidth; x++)
        {
            //get histogram pixel
            value = 0;
            
            if (*pDepthBuffer != 0)
            {
                value = depthHistogram[*pDepthBuffer];
            }
            
            red = ((int) (value * 1)) << 16;
            green = ((int) (value * 1)) << 8;
            blue = ((int) (value * 1));
            
            *depthRun = 0xff << 24 | (red + green + blue);
            
            pDepthBuffer += (depthScale * direction);
            depthRun++;
        }
    }
}

void KinectDevice::depthFrameWithUserColorsHandler()
{
    depthFrameBuffer = depthMetaData.Data();
    sceneFrameBuffer = sceneMetaData.Data();
    
    if(depthByteArray == 0) depthByteArray = new uint32_t[asDepthPixelCount];
    
    uint32_t *depthRun = depthByteArray;
    int direction = asDepthMirrored ? -1 : 1;
    int directionFactor = asDepthMirrored ? 1 : 0;
    
    unsigned int red, green, blue;
    float value;
    
    for(uint32_t y = 0; y < asDepthHeight; y++)
    {
        const XnDepthPixel *pDepthBuffer = depthFrameBuffer + ((y + directionFactor) * (depthWidth * depthScale)) - directionFactor;
        
        const XnLabel *pSceneBuffer = sceneFrameBuffer + ((y + directionFactor) * (depthWidth * depthScale)) - directionFactor;
        
        for(uint32_t x = 0; x < asDepthWidth; x++)
        {
            //get histogram pixel
            value = 0;
            
            if (*pDepthBuffer != 0)
            {
                value = depthHistogram[*pDepthBuffer];
            }
            
            XnLabel label = *pSceneBuffer;
            
            XnUInt32 nColorID = label % nColors;
            if(label == 0)
            {
                nColorID = nColors;
            }
            
            red = ((int) (value * Colors[nColorID][0])) << 16;
            green = ((int) (value * Colors[nColorID][1])) << 8;
            blue = ((int) (value * Colors[nColorID][2]));
            
            *depthRun = 0xff << 24 | (red + green + blue);
            
            pDepthBuffer += (depthScale * direction);
            pSceneBuffer += (depthScale * direction);
            depthRun++;
        }
    }
}

void KinectDevice::userMaskHandler()
{
    //we need depth, rgb & scene info
    RGBFrameBuffer = imageMetaData.RGB24Data();
    sceneFrameBuffer = sceneMetaData.Data();
    
    if(userMaskByteArray == 0)
    {
        userMaskByteArray = new uint32_t*[MAX_SKELETONS];
        for(int i = 0; i < MAX_SKELETONS; i++)
        {
            userMaskByteArray[i] = new uint32_t[asUserMaskPixelCount];
        }
    }
    
    int direction = asUserMaskMirrored ? -1 : 1;
    int directionFactor = asUserMaskMirrored ? 1 : 0;
    
    int pixelNr = 0;
    for(uint32_t y = 0; y < asUserMaskHeight; y++)
    {
        const XnRGB24Pixel *pRGBBuffer = RGBFrameBuffer + ((y + directionFactor) * (rgbWidth * userMaskScale)) - directionFactor;
        const XnLabel *pSceneBuffer = sceneFrameBuffer + ((y + directionFactor) * (depthWidth * userMaskScale)) - directionFactor;
        for(uint32_t x = 0; x < asUserMaskWidth; x++)
        {
            XnLabel label = *pSceneBuffer;
            
            for(int i = 0; i < MAX_SKELETONS; i++)
            {
                userMaskByteArray[i][pixelNr] = 0;
            }
            if(label > 0)
            {
                userMaskByteArray[label - 1][pixelNr] = 0xff << 24 | ((*pRGBBuffer).nBlue + ((*pRGBBuffer).nGreen << 8) + ((*pRGBBuffer).nRed << 16));
            }
            
            pRGBBuffer += (userMaskScale * direction);
            pSceneBuffer += (userMaskScale * direction);
            pixelNr++;
        }
    }
    
}

void KinectDevice::infraredHandler()
{
    infraredFrameBuffer = infraredMetaData.Data();
    
    if(infraredByteArray == 0) infraredByteArray = new uint32_t[asInfraredPixelCount];
    
    uint32_t *depthRun = infraredByteArray;
    int direction = asInfraredMirrored ? -1 : 1;
    int directionFactor = asInfraredMirrored ? 1 : 0;
    
    unsigned int red, green, blue;
    float value;
    
    for(uint32_t y = 0; y < asInfraredHeight; y++)
    {
        const XnIRPixel *pInfraredBuffer = infraredFrameBuffer + ((y + directionFactor) * (infraredWidth * infraredScale)) - directionFactor;
        
        for(uint32_t x = 0; x < asInfraredWidth; x++)
        {
            value = *pInfraredBuffer;
            
            red = ((int) (value * 1)) << 16;
            green = ((int) (value * 1)) << 8;
            blue = ((int) (value * 1));
            
            *depthRun = 0xff << 24 | (red + green + blue);
            
            pInfraredBuffer += (infraredScale * direction);
            depthRun++;
        }
    }
}

void KinectDevice::pointCloudHandler()
{
    depthFrameBuffer = depthMetaData.Data();
    
    if(pointCloudByteArray == 0) pointCloudByteArray = new ushort[(pointCloudPixelCount * 3) / asPointCloudDensity];
    
    ushort *pointCloudRun = pointCloudByteArray;
    int direction = asPointCloudMirrored ? -1 : 1;
    int directionFactor = asPointCloudMirrored ? 1 : 0;
    
    if(pointCloudRegions != 0)
    {
        for(int i = 0; i < numRegions; i++)
        {
            pointCloudRegions[i].numPoints = 0;
        }
    }
    
    for(uint32_t y = 0; y < asPointCloudHeight; y+=asPointCloudDensity)
    {
        const XnDepthPixel *pDepthBuffer = depthFrameBuffer + ((y + directionFactor) * (pointCloudWidth * pointCloudScale)) - directionFactor;
        
        for(uint32_t x = 0; x < asPointCloudWidth; x+=asPointCloudDensity)
        {
            //write to point cloud
            *pointCloudRun = x;
            pointCloudRun++;
            *pointCloudRun = y;
            pointCloudRun++;
            *pointCloudRun = *pDepthBuffer;
            pointCloudRun++;
            
            //check regions
            for(int i = 0; i < numRegions; i++)
            {
                if(
                   x >= pointCloudRegions[i].x && x <= pointCloudRegions[i].maxX &&
                   y >= pointCloudRegions[i].y && y <= pointCloudRegions[i].maxY &&
                   *pDepthBuffer >= pointCloudRegions[i].z && *pDepthBuffer <= pointCloudRegions[i].maxZ
                )
                {
                    pointCloudRegions[i].numPoints++;
                }
            }
            
            pDepthBuffer += (pointCloudScale * direction * asPointCloudDensity);
        }
    }
}

void KinectDevice::pointCloudWithRGBHandler()
{
    RGBFrameBuffer = imageMetaData.RGB24Data();
    depthFrameBuffer = depthMetaData.Data();
    
    if(pointCloudByteArray == 0) pointCloudByteArray = new ushort[(pointCloudPixelCount * 6) / asPointCloudDensity];
    
    ushort *pointCloudRun = pointCloudByteArray;
    int direction = asPointCloudMirrored ? -1 : 1;
    int directionFactor = asPointCloudMirrored ? 1 : 0;
    
    if(pointCloudRegions != 0)
    {
        for(int i = 0; i < numRegions; i++)
        {
            pointCloudRegions[i].numPoints = 0;
        }
    }
    
    for(uint32_t y = 0; y < asPointCloudHeight; y+=asPointCloudDensity)
    {
        const XnRGB24Pixel *pRGBBuffer = RGBFrameBuffer + ((y + directionFactor) * (rgbWidth * pointCloudScale)) - directionFactor;
        const XnDepthPixel *pDepthBuffer = depthFrameBuffer + ((y + directionFactor) * (pointCloudWidth * pointCloudScale)) - directionFactor;
        
        for(uint32_t x = 0; x < asPointCloudWidth; x+=asPointCloudDensity)
        {
            //write to point cloud
            *pointCloudRun = x;
            pointCloudRun++;
            *pointCloudRun = y;
            pointCloudRun++;
            *pointCloudRun = *pDepthBuffer;
            pointCloudRun++;
            *pointCloudRun = (*pRGBBuffer).nRed;
            pointCloudRun++;
            *pointCloudRun = (*pRGBBuffer).nGreen;
            pointCloudRun++;
            *pointCloudRun = (*pRGBBuffer).nBlue;
            pointCloudRun++;
            
            //check regions
            for(int i = 0; i < numRegions; i++)
            {
                if(
                   x >= pointCloudRegions[i].x && x <= pointCloudRegions[i].maxX &&
                   y >= pointCloudRegions[i].y && y <= pointCloudRegions[i].maxY &&
                   *pDepthBuffer >= pointCloudRegions[i].z && *pDepthBuffer <= pointCloudRegions[i].maxZ
                   )
                {
                    pointCloudRegions[i].numPoints++;
                }
            }
            
            pRGBBuffer += (pointCloudScale * direction * asPointCloudDensity);
            pDepthBuffer += (pointCloudScale * direction * asPointCloudDensity);
        }
    }
}

void KinectDevice::userHandler()
{
    //clear the current users
    memset(&userFrame.users[0], 0, sizeof(userFrame.users));
    
    XnUserID aUsers[MAX_SKELETONS];
    XnUInt16 nUsers = MAX_SKELETONS;
    XnUInt16 trackedUsers = userGenerator.GetNumberOfUsers();
    XnPoint3D position;
    XnStatus rc;
    
    userGenerator.GetUsers(aUsers, nUsers);
    
    userFrame.frameNumber = userGenerator.GetFrameID();
    userFrame.timeStamp = (int) (userGenerator.GetTimestamp() / 1000);
    
    for (int i = 0; i < MAX_SKELETONS; ++i)
    {
        if(i < trackedUsers)
        {
            rc = userGenerator.GetCoM(aUsers[i], position);
            userFrame.users[i].isTracking = true;
            userFrame.users[i].trackingID = aUsers[i];
            userFrame.users[i].hasSkeleton = (asSkeletonEnabled && userGenerator.GetSkeletonCap().IsTracking(aUsers[i]));
            
            userFrame.users[i].worldX = position.X;
            userFrame.users[i].worldY = position.Y;
            userFrame.users[i].worldZ = position.Z;
            
            userFrame.users[i].worldRelativeX = (depthWidth - position.X) / (depthWidth * 2) - .5;
            //if(asSkeletonMirrored) skeleton.joints[targetIndex].x = .5 - skeleton.joints[targetIndex].x;
            userFrame.users[i].worldRelativeY = -1 * (((depthHeight - position.Y) / (depthHeight * 2)) - .5);
            userFrame.users[i].worldRelativeZ = (position.Z * 7.8125) / MAX_DEPTH;
            
            
            //depth & rgb space are the same, as we aligned the depth & image streams
            XnPoint3D pt[1];
            pt[0] = position;
            depthGenerator.ConvertRealWorldToProjective(1, pt, pt);
            
            userFrame.users[i].rgbRelativeX = userFrame.users[i].depthRelativeX = pt[0].X / depthWidth;
            userFrame.users[i].rgbRelativeY = userFrame.users[i].depthRelativeY = pt[0].Y / depthHeight;
            
            //take mirrored rgb / depth images into account
            if(asRGBMirrored) userFrame.users[i].rgbRelativeX = 1 - userFrame.users[i].rgbRelativeX;
            if(asDepthMirrored) userFrame.users[i].depthRelativeX = 1 - userFrame.users[i].depthRelativeX;
            
            userFrame.users[i].rgbX = userFrame.users[i].rgbRelativeX * asRGBWidth;
            userFrame.users[i].rgbY = userFrame.users[i].rgbRelativeY * asRGBHeight;
            userFrame.users[i].depthX = userFrame.users[i].depthRelativeX * asDepthWidth;
            userFrame.users[i].depthY = userFrame.users[i].depthRelativeY * asDepthHeight;
            
            if (userFrame.users[i].hasSkeleton)
            {
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_HEAD, 0);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_NECK, 1);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_TORSO, 2);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_WAIST, 3);
                
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_COLLAR, 4);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_SHOULDER, 5);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_ELBOW, 6);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_WRIST, 7);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_HAND, 8);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_FINGERTIP, 9);
                
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_COLLAR, 10);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_SHOULDER, 11);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_ELBOW, 12);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_WRIST, 13);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_HAND, 14);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_FINGERTIP, 15);
                
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_HIP, 16);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_KNEE, 17);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_ANKLE, 18);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_FOOT, 19);
                
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_HIP, 20);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_KNEE, 21);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_ANKLE, 22);
                addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_FOOT, 23);
            }
        }
        else
        {
            userFrame.users[i].isTracking = false;
        }
    }
    
}

void KinectDevice::addJointElement(kinectUser &kUser, XnUserID user, XnSkeletonJoint eJoint, uint32_t targetIndex)
{
    float jointPositionX, jointPositionY, jointPositionZ, jointW, jointPositionConfidence;
    
    XnSkeletonJointPosition jointPosition;
    userGenerator.GetSkeletonCap().GetSkeletonJointPosition(user, eJoint, jointPosition);
    
    XnSkeletonJointOrientation orientation;
    userGenerator.GetSkeletonCap().GetSkeletonJointOrientation(user, eJoint, orientation);
    
    jointPositionX = jointPosition.position.X;
    jointPositionY = jointPosition.position.Y;
    jointPositionZ = jointPosition.position.Z;
    jointW = 0;
    jointPositionConfidence = jointPosition.fConfidence;
    
    kUser.joints[targetIndex].orientationConfidence = orientation.fConfidence;
    
    kUser.joints[targetIndex].orientationX = atan2f(orientation.orientation.elements[7], orientation.orientation.elements[8]);
    kUser.joints[targetIndex].orientationY = -asinf(orientation.orientation.elements[6]);
    kUser.joints[targetIndex].orientationZ = atan2f(orientation.orientation.elements[3], orientation.orientation.elements[0]);
    
    kUser.joints[targetIndex].positionConfidence = jointPositionConfidence;

    kUser.joints[targetIndex].worldX = jointPositionX;
    //if(asSkeletonMirrored) skeleton.joints[targetIndex].x = .5 - skeleton.joints[targetIndex].x;
    kUser.joints[targetIndex].worldY = jointPositionY;
    kUser.joints[targetIndex].worldZ = jointPositionZ;
    
    kUser.joints[targetIndex].worldRelativeX = (depthWidth - jointPositionX) / (depthWidth * 2) - .5;
    //if(asSkeletonMirrored) skeleton.joints[targetIndex].x = .5 - skeleton.joints[targetIndex].x;
    kUser.joints[targetIndex].worldRelativeY = -1 * (((depthHeight - jointPositionY) / (depthHeight * 2)) - .5);
    kUser.joints[targetIndex].worldRelativeZ = (jointPositionZ * 7.8125) / MAX_DEPTH;
    
    
    //depth & rgb space are the same, as we aligned the depth & image streams
    XnPoint3D pt[1];
    pt[0] = jointPosition.position;
    depthGenerator.ConvertRealWorldToProjective(1, pt, pt);
    
    kUser.joints[targetIndex].rgbRelativeX = kUser.joints[targetIndex].depthRelativeX = pt[0].X / depthWidth;
    kUser.joints[targetIndex].rgbRelativeY = kUser.joints[targetIndex].depthRelativeY = pt[0].Y / depthHeight;
    
    //take mirrored rgb / depth images into account
    if(asRGBMirrored) kUser.joints[targetIndex].rgbRelativeX = 1 - kUser.joints[targetIndex].rgbRelativeX;
    if(asDepthMirrored) kUser.joints[targetIndex].depthRelativeX = 1 - kUser.joints[targetIndex].depthRelativeX;
    
    kUser.joints[targetIndex].rgbX = kUser.joints[targetIndex].rgbRelativeX * asRGBWidth;
    kUser.joints[targetIndex].rgbY = kUser.joints[targetIndex].rgbRelativeY * asRGBHeight;
    kUser.joints[targetIndex].depthX = kUser.joints[targetIndex].depthRelativeX * asDepthWidth;
    kUser.joints[targetIndex].depthY = kUser.joints[targetIndex].depthRelativeY * asDepthHeight;
}

void KinectDevice::calculateHistogram()
{
    depthFrameBuffer = depthMetaData.Data();
    
    xnOSMemSet(depthHistogram, 0, MAX_DEPTH*sizeof(float));
    
    unsigned int nNumberOfPoints = 0;
    for (XnUInt y = 0; y < depthHeight; ++y)
    {
        for (XnUInt x = 0; x < depthWidth; ++x, ++depthFrameBuffer)
        {
            
            if (*depthFrameBuffer != 0)
            {
                depthHistogram[*depthFrameBuffer]++;
                nNumberOfPoints++;
                
            }
        }
    }
    for (int nIndex=1; nIndex<MAX_DEPTH; nIndex++)
    {
        depthHistogram[nIndex] += depthHistogram[nIndex-1];
    }
    if (nNumberOfPoints)
    {
        for (int nIndex=1; nIndex<MAX_DEPTH; nIndex++)
        {
            depthHistogram[nIndex] = (unsigned int)(256 * (1.0f - (depthHistogram[nIndex] / nNumberOfPoints)));
        }
    }
}

void KinectDevice::setUserMode(bool mirrored)
{
    //printf("KinectDevice::setUserMode(%s)\n", (mirrored) ? "true" : "false");
    asUserMirrored = mirrored;
}

void KinectDevice::setUserEnabled(bool enabled)
{
    //printf("KinectDevice::setSkeletonEnabled(%s)\n", (enabled) ? "true" : "false");
    asUserEnabled = enabled;
}

void KinectDevice::setSkeletonMode(bool mirrored)
{
    //printf("KinectDevice::setSkeletonMode(%s)\n", (mirrored) ? "true" : "false");
    asSkeletonMirrored = mirrored;
}

void KinectDevice::setSkeletonEnabled(bool enabled)
{
    //printf("KinectDevice::setSkeletonEnabled(%s)\n", (enabled) ? "true" : "false");
    asSkeletonEnabled = enabled;
}

void KinectDevice::setDepthMode(unsigned int width, unsigned int height, bool mirrored)
{
    //printf("KinectDevice::setDepthMode(%i, %i, %s)\n", width, height, (mirrored) ? "true" : "false");
    asDepthWidth = width;
    asDepthHeight = height;
    asDepthPixelCount = asDepthWidth * asDepthHeight;
    asDepthMirrored = mirrored;
    depthScale = depthWidth / asDepthWidth;
}

void KinectDevice::setDepthEnabled(bool enabled)
{
    //printf("KinectDevice::setDepthEnabled(%s)\n", (enabled) ? "true" : "false");
    asDepthEnabled = enabled;
}

void KinectDevice::setDepthShowUserColors(bool show)
{
    //printf("KinectDevice::setDepthShowUserColors(%s)\n", (show) ? "true" : "false");
    asDepthShowUserColors = show;
}

void KinectDevice::setRGBMode(unsigned int width, unsigned int height, bool mirrored)
{
    //printf("KinectDevice::setRGBMode(%i, %i, %s)\n", width, height, (mirrored) ? "true" : "false");
    asRGBWidth = width;
    asRGBHeight = height;
    asRGBPixelCount = asRGBWidth * asRGBHeight;
    asRGBMirrored = mirrored;
    rgbScale = rgbWidth / asRGBWidth;
}

void KinectDevice::setRGBEnabled(bool enabled)
{
    //printf("KinectDevice::setRGBEnabled(%s)\n", (enabled) ? "true" : "false");
    asRGBEnabled = enabled;
}

void KinectDevice::setUserMaskMode(unsigned int width, unsigned int height, bool mirrored)
{
    //printf("KinectDevice::setUserMaskMode(%i, %i, %s)\n", width, height, (mirrored) ? "true" : "false");
    asUserMaskWidth = width;
    asUserMaskHeight = height;
    asUserMaskPixelCount = asUserMaskWidth * asUserMaskHeight;
    asUserMaskMirrored = mirrored;
    userMaskScale = userMaskWidth / asUserMaskWidth;
}

void KinectDevice::setUserMaskEnabled(bool enabled)
{
    //printf("KinectDevice::setUserMaskEnabled(%s)\n", (enabled) ? "true" : "false");
    asUserMaskEnabled = enabled;
}

void KinectDevice::setInfraredMode(unsigned int width, unsigned int height, bool mirrored)
{
    //printf("KinectDevice::setInfraredMode(%i, %i, %s)\n", width, height, (mirrored) ? "true" : "false");
    asInfraredWidth = width;
    asInfraredHeight = height;
    asInfraredPixelCount = asInfraredWidth * asInfraredHeight;
    asInfraredMirrored = mirrored;
    infraredScale = infraredWidth / asInfraredWidth;
}

void KinectDevice::setInfraredEnabled(bool enabled)
{
    //printf("KinectDevice::setInfraredEnabled(%s)\n", (enabled) ? "true" : "false");
    asInfraredEnabled = enabled;
}

void KinectDevice::setPointCloudMode(unsigned int width, unsigned int height, bool mirrored, unsigned int density, bool includeRGB)
{
    //printf("KinectDevice::setPointCloudMode(%i, %i, %s)\n", width, height, (mirrored) ? "true" : "false");
    asPointCloudWidth = width;
    asPointCloudHeight = height;
    asPointCloudDensity = density;
    asPointCloudIncludeRGB = includeRGB;
    asPointCloudPixelCount = (asPointCloudWidth * asPointCloudHeight) / asPointCloudDensity;
    asPointCloudMirrored = mirrored;
    pointCloudScale = pointCloudWidth / asPointCloudWidth;
}

void KinectDevice::setPointCloudEnabled(bool enabled)
{
    //printf("KinectDevice::setPointCloudEnabled(%s)\n", (enabled) ? "true" : "false");
    asPointCloudEnabled = enabled;
}

void KinectDevice::setPointCloudRegions(PointCloudRegion *pointCloudRegions, unsigned int numRegions)
{
    this->pointCloudRegions = pointCloudRegions;
    this->numRegions = numRegions;
}

void KinectDevice::dispose()
{
    printf("KinectDevice::dispose()\n");
    //make sure threads are stopped
    stop();
    //the context of this instance will be destroyed, cleanup everything of this instance
    freContext = NULL;
}

//user tracking callbacks
void XN_CALLBACK_TYPE newUserCallback(xn::UserGenerator& rGenerator, XnUserID nID, void* pCookie)
{
	XnStatus status;
	xn::UserGenerator *userGenerator = static_cast<xn::UserGenerator*>(pCookie);
	
    if(needPose)
    {
        status = userGenerator->GetPoseDetectionCap().StartPoseDetection(strPose, nID);
        printf("New user %i, waiting for calibration pose... status: %s\n", nID, xnGetStatusString(status));
    }
    else
    {
        userGenerator->GetSkeletonCap().RequestCalibration(nID, TRUE);
    }
	
}

void XN_CALLBACK_TYPE lostUserCallback(xn::UserGenerator& rGenerator, XnUserID nID, void* pCookie)
{
	XnStatus status;
	xn::UserGenerator *userGenerator = static_cast<xn::UserGenerator*>(pCookie);
	status = userGenerator->GetSkeletonCap().StopTracking(nID);
	status = userGenerator->GetSkeletonCap().Reset(nID);
	printf("Stopped tracking of user %i... status: %s\n", nID, xnGetStatusString(status));
}

void XN_CALLBACK_TYPE userExitCallback(xn::UserGenerator& rGenerator, XnUserID nID, void* pCookie)
{
	XnStatus status;
	xn::UserGenerator *userGenerator = static_cast<xn::UserGenerator*>(pCookie);
	status = userGenerator->GetSkeletonCap().Reset(nID);
    printf("User %i exited - resetting skeleton data... status: %s\n", nID, xnGetStatusString(status));
}

void XN_CALLBACK_TYPE userReentryCallback(xn::UserGenerator& rGenerator, XnUserID nID, void* pCookie)
{
	XnStatus status;
	XnChar calibrationPose[20];
	xn::UserGenerator *userGenerator = static_cast<xn::UserGenerator*>(pCookie);
	
	status = userGenerator->GetSkeletonCap().GetCalibrationPose(calibrationPose);
	status = userGenerator->GetPoseDetectionCap().StartPoseDetection(calibrationPose, nID);
    
    printf("User %i reentered, waiting for calibration pose... status: %s\n", nID, xnGetStatusString(status));
}

void XN_CALLBACK_TYPE poseDetectedCallback(xn::PoseDetectionCapability& rCapability, const XnChar* strPose, XnUserID nID, void* pCookie)
{
	XnStatus status;
	xn::UserGenerator *userGenerator = static_cast<xn::UserGenerator*>(pCookie);
	
	status = userGenerator->GetPoseDetectionCap().StopPoseDetection(nID);
	status = userGenerator->GetSkeletonCap().RequestCalibration(nID, true);
    
    printf("Pose detected for user %i, starting calibration... status: %s\n", nID, xnGetStatusString(status));
}

void XN_CALLBACK_TYPE outOfPoseCallback(xn::PoseDetectionCapability& rCapability, const XnChar* strPose, XnUserID nID, void* pCookie)
{
	XnStatus status;
	XnChar calibrationPose[20];
	xn::UserGenerator* userGenerator = static_cast<xn::UserGenerator*>(pCookie);
	status = userGenerator->GetSkeletonCap().GetCalibrationPose(calibrationPose);
	status = userGenerator->GetPoseDetectionCap().StartPoseDetection(calibrationPose, nID);
	printf("Out of pose, waiting for calibration pose of user %i... status: %s\n", nID, xnGetStatusString(status));
}

void XN_CALLBACK_TYPE calibrationStartCallback(xn::SkeletonCapability& rCapability, XnUserID nID, void* pCookie)
{
	printf("Starting calibration of user %i\n", nID);
}

void XN_CALLBACK_TYPE calibrationCompleteCallback(xn::SkeletonCapability& rCapability, XnUserID nID, XnCalibrationStatus calibrationError, void* pCookie)
{
	XnStatus status;
	xn::UserGenerator* userGenerator = static_cast<xn::UserGenerator*>(pCookie);
	
	if (calibrationError == XN_STATUS_OK)
	{
		status = userGenerator->GetSkeletonCap().StartTracking(nID);
		printf("Calibration success, beginning to track user %i now... status: %s\n", nID, xnGetStatusString(status));	}
	else
	{
		XnChar calibrationPose[20];
		status = userGenerator->GetSkeletonCap().GetCalibrationPose(calibrationPose);
		status = userGenerator->GetPoseDetectionCap().StartPoseDetection(calibrationPose, nID);
		printf("Calibration failure for user %i, waiting for calibration pose... status: %s\n", nID, xnGetStatusString(status));
	}
}
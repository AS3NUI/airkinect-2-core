//
//  OpenNIDevice.cpp
//  KinectExtension
//
//  Created by Wouter Verweirder on 24/01/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <math.h>
#include "OpenNIDevice.h"

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

OpenNIDevice::OpenNIDevice(int nr, xn::Context context)
{
    printf("OpenNIDevice::OpenNIDevice(%i)\n", nr);
    this->nr = nr;
    this->freContext = freContext;
    this->context = context;
    
    //initialize the mutexes
    /*
    pthread_mutex_init(&userMutex, NULL);
    pthread_mutex_init(&depthMutex, NULL);
    pthread_mutex_init(&rgbMutex, NULL);
    pthread_mutex_init(&userMaskMutex, NULL);
    pthread_mutex_init(&infraredMutex, NULL);
    pthread_mutex_init(&pointCloudMutex, NULL);
     */
    
    //set default values
    setDefaults();
}

void OpenNIDevice::setFreContext(FREContext pFreContext)
{
	freContext = pFreContext;
}

FREContext OpenNIDevice::getFreContext()
{
	return freContext;
}

void OpenNIDevice::setDefaults()
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
    numRegions = 0;
    
    //player index coloring
    setUserColor(1, 0xff0000, 1);
	setUserColor(2, 0x00ff00, 1);
	setUserColor(3, 0x0000ff, 1);
	setUserColor(4, 0xffff00, 1);
	setUserColor(5, 0xff00ff, 1);
	setUserColor(6, 0x8080ff, 1);
    
    setUserColor(7, 0xff0000, 1);
	setUserColor(8, 0x00ff00, 1);
	setUserColor(9, 0x0000ff, 1);
	setUserColor(10, 0xffff00, 1);
	setUserColor(11, 0xff00ff, 1);
	setUserColor(12, 0x8080ff, 1);
    
    setUserColor(13, 0xff0000, 1);
	setUserColor(14, 0x00ff00, 1);
	setUserColor(15, 0x0000ff, 1);
}

void OpenNIDevice::setUserColor(int userID, int color, bool useIntensity)
{
	if(userID > MAX_SKELETONS) return;
	
    userIndexColors[userID - 1][0] = (0xFF & (color >> 16)) / 255.0f;
    userIndexColors[userID - 1][1] = (0xFF & (color >> 8)) / 255.0f;
    userIndexColors[userID - 1][2] = (0xFF & (color)) / 255.0f;
    userIndexColors[userID - 1][3] = useIntensity ? 1 : 0;
}

//////////////// START FRE FUNCTIONS

FREObject OpenNIDevice::freSetUserMode(FREObject argv[])
{
    unsigned int mirrored; FREGetObjectAsBool(argv[1], &mirrored);
    asUserMirrored = (mirrored != 0);
    return NULL;
}

FREObject OpenNIDevice::freSetUserColor(FREObject argv[])
{
    unsigned int userID; FREGetObjectAsUint32(argv[1], &userID);
    unsigned int color; FREGetObjectAsUint32(argv[2], &color);
    unsigned int useIntensity; FREGetObjectAsBool(argv[3], &useIntensity);
    setUserColor(userID, color, (useIntensity != 0));
    return NULL;
}

FREObject OpenNIDevice::freSetUserEnabled(FREObject argv[])
{
    unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asUserEnabled = (enabled != 0);
    return NULL;
}

FREObject OpenNIDevice::freSetSkeletonMode(FREObject argv[])
{
    unsigned int mirrored; FREGetObjectAsBool(argv[1], &mirrored);
    asSkeletonMirrored = (mirrored != 0);
    return NULL;
}

FREObject OpenNIDevice::freSetSkeletonEnabled(FREObject argv[])
{
    unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asSkeletonEnabled = (enabled != 0);
    return NULL;
}

FREObject OpenNIDevice::freGetUserFrame(FREObject argv[])
{
    lockUserMutex();
    
    short int trackedSkeletons = 0;
    
    FREObject freUserFrame, frameNumber, timestamp, users, user, userType, userID, trackingID, hasSkeleton, joints, joint, jointName;
    FREObject position, positionRelative, positionConfidence, orientation, orientationConfidence, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition;
    FREObject orientationX, orientationY, orientationZ;
    FREObject positionX, positionY, positionZ, positionRelativeX, positionRelativeY, positionRelativeZ;
    FREObject rgbPositionX, rgbPositionY, rgbRelativePositionX, rgbRelativePositionY, depthPositionX, depthPositionY, depthRelativePositionX, depthRelativePositionY;
    
    FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.User>", 0, NULL, &users, NULL);
    
    for(int i = 0; i < MAX_SKELETONS; i++)
    {
        if(userFrame.users[i].isTracking)
        {   
            //create the joints vector
            FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint>", 0, NULL, &joints, NULL);
            
            for(int j = 0; j < NUM_JOINTS; j++)
            {
                //name
                FRENewObjectFromUTF8(strlen(JOINT_NAMES[j]), (const uint8_t*) JOINT_NAMES[j], &jointName);
                //position
                FRENewObjectFromDouble(userFrame.users[i].joints[j].worldX, &positionX);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].worldY, &positionY);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].worldZ, &positionZ);
                FREObject positionParams[] = {positionX, positionY, positionZ};
                FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, positionParams, &position, NULL);
                //position relative
                FRENewObjectFromDouble(userFrame.users[i].joints[j].worldRelativeX, &positionRelativeX);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].worldRelativeY, &positionRelativeY);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].worldRelativeZ, &positionRelativeZ);
                FREObject positionRelativeParams[] = {positionRelativeX, positionRelativeY, positionRelativeZ};
                FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, positionRelativeParams, &positionRelative, NULL);
                //position confidence
                FRENewObjectFromDouble(userFrame.users[i].joints[j].positionConfidence, &positionConfidence);
                
                FRENewObjectFromDouble(userFrame.users[i].joints[j].orientationX, &orientationX);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].orientationY, &orientationY);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].orientationZ, &orientationZ);
                
                //orientation
                FREObject orientationParams[] = {orientationX, orientationY, orientationZ};
                FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, orientationParams, &orientation, NULL);
                
                //orientation confidence
                FRENewObjectFromDouble(userFrame.users[i].joints[j].orientationConfidence, &orientationConfidence);
                //rgb position
                FRENewObjectFromInt32(userFrame.users[i].joints[j].rgbX, &rgbPositionX);
                FRENewObjectFromInt32(userFrame.users[i].joints[j].rgbY, &rgbPositionY);
                FREObject rgbPositionParams[] = {rgbPositionX, rgbPositionY};
                FRENewObject( (const uint8_t*) "flash.geom.Point", 2, rgbPositionParams, &rgbPosition, NULL);
                //rgb relative position
                FRENewObjectFromDouble(userFrame.users[i].joints[j].rgbRelativeX, &rgbRelativePositionX);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].rgbRelativeY, &rgbRelativePositionY);
                FREObject rgbRelativePositionParams[] = {rgbRelativePositionX, rgbRelativePositionY};
                FRENewObject( (const uint8_t*) "flash.geom.Point", 2, rgbRelativePositionParams, &rgbRelativePosition, NULL);
                //depth position
                FRENewObjectFromInt32(userFrame.users[i].joints[j].depthX, &depthPositionX);
                FRENewObjectFromInt32(userFrame.users[i].joints[j].depthY, &depthPositionY);
                FREObject depthPositionParams[] = {depthPositionX, depthPositionY};
                FRENewObject( (const uint8_t*) "flash.geom.Point", 2, depthPositionParams, &depthPosition, NULL);
                //depth relative position
                FRENewObjectFromDouble(userFrame.users[i].joints[j].depthRelativeX, &depthRelativePositionX);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].depthRelativeY, &depthRelativePositionY);
                FREObject depthRelativePositionParams[] = {depthRelativePositionX, depthRelativePositionY};
                FRENewObject( (const uint8_t*) "flash.geom.Point", 2, depthRelativePositionParams, &depthRelativePosition, NULL);
                //create the joint
                FREObject jointParams[] = {jointName, position, positionRelative, positionConfidence, orientation, orientationConfidence, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition};
                FRENewObject( (const uint8_t*) "com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint", 10, jointParams, &joint, NULL);
                FRESetArrayElementAt(joints, j, joint);
            }
            
            //user position
            FRENewObjectFromDouble(userFrame.users[i].worldX, &positionX);
            FRENewObjectFromDouble(userFrame.users[i].worldY, &positionY);
            FRENewObjectFromDouble(userFrame.users[i].worldZ, &positionZ);
            FREObject positionParams[] = {positionX, positionY, positionZ};
            FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, positionParams, &position, NULL);
            
            //user position relative
            FRENewObjectFromDouble(userFrame.users[i].worldRelativeX, &positionRelativeX);
            FRENewObjectFromDouble(userFrame.users[i].worldRelativeY, &positionRelativeY);
            FRENewObjectFromDouble(userFrame.users[i].worldRelativeZ, &positionRelativeZ);
            FREObject positionRelativeParams[] = {positionRelativeX, positionRelativeY, positionRelativeZ};
            FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, positionRelativeParams, &positionRelative, NULL);
            
            //user rgb position
            FRENewObjectFromInt32(userFrame.users[i].rgbX, &rgbPositionX);
            FRENewObjectFromInt32(userFrame.users[i].rgbY, &rgbPositionY);
            FREObject rgbPositionParams[] = {rgbPositionX, rgbPositionY};
            FRENewObject( (const uint8_t*) "flash.geom.Point", 2, rgbPositionParams, &rgbPosition, NULL);
            //user rgb relative position
            FRENewObjectFromDouble(userFrame.users[i].rgbRelativeX, &rgbRelativePositionX);
            FRENewObjectFromDouble(userFrame.users[i].rgbRelativeY, &rgbRelativePositionY);
            FREObject rgbRelativePositionParams[] = {rgbRelativePositionX, rgbRelativePositionY};
            FRENewObject( (const uint8_t*) "flash.geom.Point", 2, rgbRelativePositionParams, &rgbRelativePosition, NULL);
            //user depth position
            FRENewObjectFromInt32(userFrame.users[i].depthX, &depthPositionX);
            FRENewObjectFromInt32(userFrame.users[i].depthY, &depthPositionY);
            FREObject depthPositionParams[] = {depthPositionX, depthPositionY};
            FRENewObject( (const uint8_t*) "flash.geom.Point", 2, depthPositionParams, &depthPosition, NULL);
            //user depth relative position
            FRENewObjectFromDouble(userFrame.users[i].depthRelativeX, &depthRelativePositionX);
            FRENewObjectFromDouble(userFrame.users[i].depthRelativeY, &depthRelativePositionY);
            FREObject depthRelativePositionParams[] = {depthRelativePositionX, depthRelativePositionY};
            FRENewObject( (const uint8_t*) "flash.geom.Point", 2, depthRelativePositionParams, &depthRelativePosition, NULL);
            
            FRENewObjectFromUTF8(6, (const uint8_t*) "openni", &userType);
            FRENewObjectFromUint32(userFrame.users[i].userID, &userID);
            FRENewObjectFromUint32(userFrame.users[i].trackingID, &trackingID);
            FRENewObjectFromBool((userFrame.users[i].hasSkeleton) ? 1 : 0, &hasSkeleton);
            FREObject skeletonParams[] = {userType, userID, trackingID, position, positionRelative, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition, hasSkeleton, joints};
            
            FRENewObject( (const uint8_t*) "com.as3nui.nativeExtensions.air.kinect.data.User", 11, skeletonParams, &user, NULL);
            
            FRESetArrayElementAt(users, trackedSkeletons, user);
            trackedSkeletons++;
        }
    }
    
    FRENewObjectFromUint32(userFrame.frameNumber, &frameNumber);
    FRENewObjectFromUint32(userFrame.timeStamp, &timestamp);
    
    FREObject skeletonFrameParams[] = {frameNumber, timestamp, users};
    FRENewObject( (const uint8_t*) "com.as3nui.nativeExtensions.air.kinect.data.UserFrame", 3, skeletonFrameParams, &freUserFrame, NULL);
    
    unlockUserMutex();
    
    return freUserFrame;
}

FREObject OpenNIDevice::freGetSkeletonJointNameIndices(FREObject argv[])
{
    return NULL;
}

FREObject OpenNIDevice::freGetSkeletonJointNames(FREObject argv[])
{
    return NULL;
}

FREObject OpenNIDevice::freSetUserMaskMode(FREObject argv[])
{
    unsigned int width; FREGetObjectAsUint32(argv[1], &width);
    unsigned int height; FREGetObjectAsUint32(argv[2], &height);
    unsigned int mirrored; FREGetObjectAsBool(argv[3], &mirrored);
    
    lockUserMaskMutex();
    
    asUserMaskWidth = width;
    asUserMaskHeight = height;
    asUserMaskPixelCount = asUserMaskWidth * asUserMaskHeight;
    asUserMaskMirrored = (mirrored != 0);
    userMaskScale = userMaskWidth / asUserMaskWidth;
    
    //reset bytearray
    if(userMaskByteArray != 0)
    {
        for(int i = 0; i < MAX_SKELETONS; i++)
        {
            delete [] userMaskByteArray[i];
        }
        delete [] userMaskByteArray;
    }
    userMaskByteArray = new uint32_t*[MAX_SKELETONS];
    for(int i = 0; i < MAX_SKELETONS; i++)
    {
        userMaskByteArray[i] = new uint32_t[asUserMaskPixelCount];
    }
    
    unlockUserMaskMutex();
    return NULL;
}

FREObject OpenNIDevice::freSetUserMaskEnabled(FREObject argv[])
{
    unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asUserMaskEnabled = (enabled != 0);
    return NULL;
}

FREObject OpenNIDevice::freGetUserMaskFrame(FREObject argv[])
{
    unsigned int trackingID; FREGetObjectAsUint32(argv[1], &trackingID);
    
    if(trackingID > 0) trackingID--;
    
    const unsigned int numUserMaskBytes = asUserMaskPixelCount * 4;
    
    FREObject objectByteArray = argv[2];
    FREByteArray byteArray;			
    FREObject length;
    FRENewObjectFromUint32(numUserMaskBytes, &length);
    FRESetObjectProperty(objectByteArray, (const uint8_t*) "length", length, NULL);
    FREAcquireByteArray(objectByteArray, &byteArray);
    lockUserMaskMutex();
    memcpy(byteArray.bytes, userMaskByteArray[trackingID], numUserMaskBytes);
    unlockUserMaskMutex();
    FREReleaseByteArray(objectByteArray);
    
    return NULL;
}

FREObject OpenNIDevice::freSetDepthMode(FREObject argv[])
{
    unsigned int width; FREGetObjectAsUint32(argv[1], &width);
    unsigned int height; FREGetObjectAsUint32(argv[2], &height);
    unsigned int mirrored; FREGetObjectAsBool(argv[3], &mirrored);
    
    lockDepthMutex();
    
    asDepthWidth = width;
    asDepthHeight = height;
    asDepthPixelCount = asDepthWidth * asDepthHeight;
    asDepthMirrored = (mirrored != 0);
    depthScale = depthWidth / asDepthWidth;
    
    //reset bytearray
    if(depthByteArray != 0) delete [] depthByteArray;
    depthByteArray = new uint32_t[asDepthPixelCount];
    
    unlockDepthMutex();
    return NULL;
}

FREObject OpenNIDevice::freSetDepthEnabled(FREObject argv[])
{
    unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asDepthEnabled = (enabled != 0);
    return NULL;
}

FREObject OpenNIDevice::freGetDepthFrame(FREObject argv[])
{
    const unsigned int numDepthBytes = depthPixelCount * 4;
    
    FREObject objectByteArray = argv[1];
    FREByteArray byteArray;			
    FREObject length;
    FRENewObjectFromUint32(numDepthBytes, &length);
    FRESetObjectProperty(objectByteArray, (const uint8_t*) "length", length, NULL);
    FREAcquireByteArray(objectByteArray, &byteArray);
    lockDepthMutex();
    memcpy(byteArray.bytes, depthByteArray, numDepthBytes);
    unlockDepthMutex();
    FREReleaseByteArray(objectByteArray);
    
    return NULL;
}

FREObject OpenNIDevice::freSetDepthShowUserColors(FREObject argv[])
{
    unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asDepthShowUserColors = (enabled != 0);
    return NULL;
}

FREObject OpenNIDevice::freSetRGBMode(FREObject argv[])
{
    unsigned int width; FREGetObjectAsUint32(argv[1], &width);
    unsigned int height; FREGetObjectAsUint32(argv[2], &height);
    unsigned int mirrored; FREGetObjectAsBool(argv[3], &mirrored);
    
    lockRGBMutex();
    
    asRGBWidth = width;
    asRGBHeight = height;
    asRGBPixelCount = asRGBWidth * asRGBHeight;
    asRGBMirrored = (mirrored != 0);
    rgbScale = rgbWidth / asRGBWidth;
    
    //reset bytearray
    if(RGBByteArray != 0) delete [] RGBByteArray;
    RGBByteArray = new uint32_t[asRGBPixelCount];
    
    unlockRGBMutex();
    return NULL;
}

FREObject OpenNIDevice::freSetRGBEnabled(FREObject argv[])
{
    unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asRGBEnabled = (enabled != 0);
    return NULL;
}

FREObject OpenNIDevice::freGetRGBFrame(FREObject argv[])
{
    const unsigned int numRGBBytes = asRGBPixelCount * 4;
    
    FREObject objectByteArray = argv[1];
    FREByteArray byteArray;			
    FREObject length;
    FRENewObjectFromUint32(numRGBBytes, &length);
    FRESetObjectProperty(objectByteArray, (const uint8_t*) "length", length, NULL);
    FREAcquireByteArray(objectByteArray, &byteArray);
    lockRGBMutex();
    memcpy(byteArray.bytes, RGBByteArray, numRGBBytes);
    unlockRGBMutex();
    FREReleaseByteArray(objectByteArray);
    
    return NULL;
}

FREObject OpenNIDevice::freSetInfraredMode(FREObject argv[])
{
    unsigned int width; FREGetObjectAsUint32(argv[1], &width);
    unsigned int height; FREGetObjectAsUint32(argv[2], &height);
    unsigned int mirrored; FREGetObjectAsBool(argv[3], &mirrored);
    
    lockInfraredMutex();
    
    asInfraredWidth = width;
    asInfraredHeight = height;
    asInfraredPixelCount = asInfraredWidth * asInfraredHeight;
    asInfraredMirrored = (mirrored != 0);
    infraredScale = infraredWidth / asInfraredWidth;
    
    if(infraredByteArray != 0) delete [] infraredByteArray;
    infraredByteArray = new uint32_t[asInfraredPixelCount];
    
    unlockInfraredMutex();
    return NULL;
}

FREObject OpenNIDevice::freSetInfraredEnabled(FREObject argv[])
{
    unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asInfraredEnabled = (enabled != 0);
    return NULL;
}

FREObject OpenNIDevice::freGetInfraredFrame(FREObject argv[])
{
    const unsigned int numInfraredBytes = asInfraredPixelCount * 4;
    
    FREObject objectByteArray = argv[1];
    FREByteArray byteArray;			
    FREObject length;
    FRENewObjectFromUint32(numInfraredBytes, &length);
    FRESetObjectProperty(objectByteArray, (const uint8_t*) "length", length, NULL);
    FREAcquireByteArray(objectByteArray, &byteArray);
    lockInfraredMutex();
    memcpy(byteArray.bytes, infraredByteArray, numInfraredBytes);
    unlockInfraredMutex();
    FREReleaseByteArray(objectByteArray);
    
    return NULL;
}

FREObject OpenNIDevice::freSetPointCloudMode(FREObject argv[])
{
    lockPointCloudMutex();
    
    unsigned int width; FREGetObjectAsUint32(argv[1], &width);
    unsigned int height; FREGetObjectAsUint32(argv[2], &height);
    unsigned int mirrored; FREGetObjectAsBool(argv[3], &mirrored);
    unsigned int density; FREGetObjectAsUint32(argv[4], &density);
    unsigned int includeRGB; FREGetObjectAsBool(argv[5], &includeRGB);
    
    asPointCloudWidth = width;
    asPointCloudHeight = height;
    asPointCloudDensity = density;
    asPointCloudIncludeRGB = includeRGB;
    asPointCloudPixelCount = (asPointCloudWidth * asPointCloudHeight) / asPointCloudDensity;
    asPointCloudMirrored = mirrored;
    pointCloudScale = pointCloudWidth / asPointCloudWidth;
    
    if(pointCloudByteArray != 0) delete [] pointCloudByteArray;
    if(asPointCloudIncludeRGB)
    {
        pointCloudByteArray = new short[asPointCloudPixelCount * 6];
    }
    else
    {
        pointCloudByteArray = new short[asPointCloudPixelCount * 3];
    }
    
    unlockPointCloudMutex();
    
    //kinectDeviceManager.getDevice(nr, ctx)->setPointCloudMode(width, height, (mirrored != 0), density, includeRGB);
    return NULL;
}

FREObject OpenNIDevice::freSetPointCloudEnabled(FREObject argv[])
{
    unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asPointCloudEnabled = (enabled != 0);
    return NULL;
}

FREObject OpenNIDevice::freGetPointCloudFrame(FREObject argv[])
{
    FREObject objectPointsByteArray = argv[1];
    
    const unsigned int numPointBytes = getAsPointCloudByteArrayLength();
    
    lockPointCloudMutex();
    
    FREByteArray pointsByteArray;			
    FREObject pointsLength;
    FRENewObjectFromUint32(numPointBytes, &pointsLength);
    FRESetObjectProperty(objectPointsByteArray, (const uint8_t*) "length", pointsLength, NULL);
    FREAcquireByteArray(objectPointsByteArray, &pointsByteArray);
    memcpy(pointsByteArray.bytes, pointCloudByteArray, numPointBytes);
    FREReleaseByteArray(objectPointsByteArray);
    
    //set the region information?
    FREObject asPointCloudRegions = argv[2];
    if(asPointCloudRegions != NULL && &pointCloudRegions != 0)
    {
        //loop through these actionscript regions and get the native info back
        FREObject asPointCloudRegion, asRegionId;
        FREObject asNumPoints;
        unsigned int regionId;
        
        uint32_t freNumRegions;
        FREGetArrayLength(asPointCloudRegions, &freNumRegions);
        
        for(int i = 0; i < freNumRegions; i++)
        {
            FREGetArrayElementAt(asPointCloudRegions, i, &asPointCloudRegion);
            FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "regionId", &asRegionId, NULL);
            FREGetObjectAsUint32(asRegionId, &regionId);
            //get the region with this id from the device memory
            for(int j = 0; j < numRegions; j++)
            {
                PointCloudRegion *nativeRegion = &pointCloudRegions[j];
                if(nativeRegion->regionId == regionId)
                {
                    //update the actionscript properties
                    FRENewObjectFromUint32(nativeRegion->numPoints, &asNumPoints);
                    FRESetObjectProperty(asPointCloudRegion, (const uint8_t *) "numPoints", asNumPoints, NULL);
                    break;
                }
            }
        }
    }
    
    unlockPointCloudMutex();
    
    return NULL;
}

FREObject OpenNIDevice::freSetPointCloudRegions(FREObject argv[])
{
    FREObject asPointCloudRegions = argv[1];
    FREObject asPointCloudRegion, asRegionId, asX, asY, asZ, asWidth, asHeight, asDepth;
    unsigned int regionId;
    double x, y, z, width, height, depth;
    
    uint32_t freNumRegions;
    FREGetArrayLength(asPointCloudRegions, &freNumRegions);
    
    PointCloudRegion *nativeRegions = new PointCloudRegion[freNumRegions];
    
    for(int i = 0; i < freNumRegions; i++)
    {
        FREGetArrayElementAt(asPointCloudRegions, i, &asPointCloudRegion);
        FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "regionId", &asRegionId, NULL);
        FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "x", &asX, NULL);
        FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "y", &asY, NULL);
        FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "z", &asZ, NULL);
        FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "width", &asWidth, NULL);
        FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "height", &asHeight, NULL);
        FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "depth", &asDepth, NULL);
        FREGetObjectAsUint32(asRegionId, &regionId);
        FREGetObjectAsDouble(asX, &x);
        FREGetObjectAsDouble(asY, &y);
        FREGetObjectAsDouble(asZ, &z);
        FREGetObjectAsDouble(asWidth, &width);
        FREGetObjectAsDouble(asHeight, &height);
        FREGetObjectAsDouble(asDepth, &depth);
        PointCloudRegion *nativeRegion = new PointCloudRegion();
        nativeRegion->setProperties(regionId, x, y, z, width, height, depth);
        nativeRegions[i] = *nativeRegion;
    }
    
    this->pointCloudRegions = nativeRegions;
    this->numRegions = freNumRegions;
    
    return NULL;
}

// END FRE FUNCTIONS

int OpenNIDevice::getAsPointCloudByteArrayLength()
{
    if(asPointCloudIncludeRGB)
    {
        return (asPointCloudWidth * asPointCloudHeight * sizeof(short) * 6) / asPointCloudDensity;
    }
    else
    {
        return (asPointCloudWidth * asPointCloudHeight * sizeof(short) * 3) / asPointCloudDensity;
    }
}

void OpenNIDevice::lockUserMutex()
{
    userMutex.lock();
    //pthread_mutex_lock(&userMutex);
}

void OpenNIDevice::unlockUserMutex()
{
    userMutex.unlock();
    //pthread_mutex_unlock(&userMutex);
}

void OpenNIDevice::lockDepthMutex()
{
    depthMutex.lock();
    //pthread_mutex_lock(&depthMutex);
}

void OpenNIDevice::unlockDepthMutex()
{
    depthMutex.unlock();
    //pthread_mutex_unlock(&depthMutex);
}

void OpenNIDevice::lockRGBMutex()
{
    rgbMutex.lock();
    //pthread_mutex_lock(&rgbMutex);
}

void OpenNIDevice::unlockRGBMutex()
{
    rgbMutex.unlock();
    //pthread_mutex_unlock(&rgbMutex);
}

void OpenNIDevice::lockUserMaskMutex()
{
    userMaskMutex.lock();
    //pthread_mutex_lock(&userMaskMutex);
}

void OpenNIDevice::unlockUserMaskMutex()
{
    userMaskMutex.unlock();
    //pthread_mutex_unlock(&userMaskMutex);
}

void OpenNIDevice::lockInfraredMutex()
{
    infraredMutex.lock();
    //pthread_mutex_lock(&infraredMutex);
}

void OpenNIDevice::unlockInfraredMutex()
{
    infraredMutex.unlock();
    //pthread_mutex_unlock(&infraredMutex);
}

void OpenNIDevice::lockPointCloudMutex()
{
    pointCloudMutex.lock();
    //pthread_mutex_lock(&pointCloudMutex);
}

void OpenNIDevice::unlockPointCloudMutex()
{
    pointCloudMutex.unlock();
    //pthread_mutex_unlock(&pointCloudMutex);
}

void OpenNIDevice::start()
{
    printf("OpenNIDevice::start()\n");
    if(!running)
    {
        //returnVal = pthread_attr_init(&attr);
        //returnVal = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        
        running = true;
        
        //int threadError = pthread_create(&posixThreadID, &attr, deviceThread, (void *) this);
        //printf("thread create code: %i\n", threadError);
        mThread = boost::thread(&OpenNIDevice::deviceThread, this);
    }
}

void OpenNIDevice::stop()
{
    printf("OpenNIDevice::stop()\n");
    if(running)
    {
        running = false;
        //int threadError = pthread_join(posixThreadID, NULL);
        //printf("thread join code: %d\n", threadError);
        mThread.join();
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
        FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "stopped");
    }
}

void * OpenNIDevice::deviceThread(void *self)
{
    OpenNIDevice *adapter = (OpenNIDevice *) self;
    adapter->run();
    return NULL;
}

void OpenNIDevice::run()
{
    printf("OpenNIDevice::run(), %s\n", (running) ? "true" : "false");
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
            printf("OpenNIDevice create depthGenerator failed: %s\n", xnGetStatusString(rc));
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
            printf("OpenNIDevice create imageGenerator failed: %s\n", xnGetStatusString(rc));
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
            printf("OpenNIDevice create infraredGenerator failed: %s\n", xnGetStatusString(rc));
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
            printf("OpenNIDevice create userGenerator failed: %s\n", xnGetStatusString(rc));
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
                printf("OpenNIDevice start depthGenerator failed: %s\n", xnGetStatusString(rc));
                stop();
                return;
            }
        }
        if(asRGBEnabled || asUserMaskEnabled || asPointCloudEnabled)
        {
            rc = imageGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                printf("OpenNIDevice start imageGenerator failed: %s\n", xnGetStatusString(rc));
                stop();
                return;
            }
        }
        if((asDepthEnabled && asDepthShowUserColors) || asUserMaskEnabled || asUserEnabled || asSkeletonEnabled)
        {
            rc = userGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                printf("OpenNIDevice start userGenerator failed: %s\n", xnGetStatusString(rc));
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
                printf("OpenNIDevice start infraredGenerator failed: %s\n", xnGetStatusString(rc));
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
        FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "started");
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
                lockRGBMutex();
                rgbFrameHandler();
                unlockRGBMutex();
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "RGBFrame");
            }
            
            //depth image
            if(asDepthEnabled)
            {
                lockDepthMutex();
                if(asDepthShowUserColors)
                {
                    depthFrameWithUserColorsHandler();
                }
                else
                {
                    depthFrameHandler();
                }
                unlockDepthMutex();
                //dispatch depth frame event
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "depthFrame");
            }
            
            //infrared image - not available when rgb is enabled
            if(asInfraredEnabled && !imageGenerator.IsGenerating())
            {
                lockInfraredMutex();
                infraredHandler();
                unlockInfraredMutex();
                //dispatch infrared frame event
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "infraredFrame");
            }
            
            //point cloud
            if(asPointCloudEnabled)
            {
                lockPointCloudMutex();
                if(asPointCloudIncludeRGB)
                {
                    pointCloudWithRGBHandler();
                }
                else
                {
                    pointCloudHandler();
                }
                unlockPointCloudMutex();
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "pointCloudFrame");
            }
            
            //user information
            if(asUserEnabled || asSkeletonEnabled || asUserMaskEnabled)
            {
                lockUserMutex();
                userHandler();
                unlockUserMutex();
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "userFrame");
            }
            
            //user mask image
            if(asUserMaskEnabled)
            {
                lockUserMaskMutex();
                userMaskHandler();
                unlockUserMaskMutex();
                //dispatch user mask frame event
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "userMaskFrame");
            }
        }
    }
}

void OpenNIDevice::rgbFrameHandler()
{
    RGBFrameBuffer = imageMetaData.RGB24Data();
    
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

void OpenNIDevice::depthFrameHandler()
{
    depthFrameBuffer = depthMetaData.Data();
    
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

void OpenNIDevice::depthFrameWithUserColorsHandler()
{
    depthFrameBuffer = depthMetaData.Data();
    sceneFrameBuffer = sceneMetaData.Data();
    
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
            
            short label = *pSceneBuffer;
            
            if(label == 0)
            {
                red = ((int) value) << 16;
                green = ((int) value) << 8;
                blue = ((int) value);
            }
            else
            {
                if(userIndexColors[label][3] == 1)
                {
                    red = ((int) (value * userIndexColors[label][0])) << 16;
                    green = ((int) (value * userIndexColors[label][1])) << 8;
                    blue = ((int) (value * userIndexColors[label][2]));
                }
                else
                {
                    red = ((int) (0xFF && userIndexColors[label][0])) << 16;
                    green = ((int) (0xFF && userIndexColors[label][1])) << 8;
                    blue = ((int) (0xFF && userIndexColors[label][2]));
                }
            }
            
            *depthRun = 0xff << 24 | (red + green + blue);
            
            pDepthBuffer += (depthScale * direction);
            pSceneBuffer += (depthScale * direction);
            depthRun++;
        }
    }
}

void OpenNIDevice::userMaskHandler()
{
    //we need depth, rgb & scene info
    RGBFrameBuffer = imageMetaData.RGB24Data();
    sceneFrameBuffer = sceneMetaData.Data();
    
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

void OpenNIDevice::infraredHandler()
{
    infraredFrameBuffer = infraredMetaData.Data();
    
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

void OpenNIDevice::pointCloudHandler()
{
    depthFrameBuffer = depthMetaData.Data();
    
    short *pointCloudRun = pointCloudByteArray;
    int direction = asPointCloudMirrored ? -1 : 1;
    int directionFactor = asPointCloudMirrored ? 1 : 0;
    
    if(pointCloudRegions != 0)
    {
        for(int i = 0; i < numRegions; i++)
        {
            pointCloudRegions[i].numPoints = 0;
        }
    }
    else
    {
        numRegions = 0;
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

void OpenNIDevice::pointCloudWithRGBHandler()
{
    RGBFrameBuffer = imageMetaData.RGB24Data();
    depthFrameBuffer = depthMetaData.Data();
    
    short *pointCloudRun = pointCloudByteArray;
    int direction = asPointCloudMirrored ? -1 : 1;
    int directionFactor = asPointCloudMirrored ? 1 : 0;
    
    if(pointCloudRegions != 0)
    {
        for(int i = 0; i < numRegions; i++)
        {
            pointCloudRegions[i].numPoints = 0;
        }
    }
    else
    {
        numRegions = 0;
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

void OpenNIDevice::userHandler()
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
            userFrame.users[i].userID = aUsers[i];
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
            
            userFrame.users[i].rgbX = (int) (userFrame.users[i].rgbRelativeX * asRGBWidth);
            userFrame.users[i].rgbY = (int) (userFrame.users[i].rgbRelativeY * asRGBHeight);
            userFrame.users[i].depthX = (int) (userFrame.users[i].depthRelativeX * asDepthWidth);
            userFrame.users[i].depthY = (int) (userFrame.users[i].depthRelativeY * asDepthHeight);
            
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

void OpenNIDevice::addJointElement(kinectUser &kUser, XnUserID user, XnSkeletonJoint eJoint, uint32_t targetIndex)
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
    
    kUser.joints[targetIndex].rgbX = (int) (kUser.joints[targetIndex].rgbRelativeX * asRGBWidth);
    kUser.joints[targetIndex].rgbY = (int) (kUser.joints[targetIndex].rgbRelativeY * asRGBHeight);
    kUser.joints[targetIndex].depthX = (int) (kUser.joints[targetIndex].depthRelativeX * asDepthWidth);
    kUser.joints[targetIndex].depthY = (int) (kUser.joints[targetIndex].depthRelativeY * asDepthHeight);
}

void OpenNIDevice::calculateHistogram()
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

void OpenNIDevice::dispose()
{
    printf("OpenNIDevice::dispose()\n");
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
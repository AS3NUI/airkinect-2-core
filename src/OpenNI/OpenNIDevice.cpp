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
#include "OpenNIDevice.h"
#ifdef AIRKINECT_TARGET_OPENNI

#ifdef AIRKINECT_OS_OSX
    #include <iostream>
    #include <math.h>
#endif

#include "AKOpenNIPointCloudGenerator.h"
#include "AKOpenNIUserMasksGenerator.h"
#include "AKOpenNIUserFrameGenerator.h"

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
    
    //initialize the capabilities of this device
    capabilities.hasCameraElevationSupport				= false;
    capabilities.hasDepthCameraSupport					= true;
    capabilities.hasDepthUserSupport					= true;
    capabilities.hasInfraredSupport						= true;
    capabilities.hasJointOrientationConfidenceSupport	= false;
    capabilities.hasJointOrientationSupport				= false;
    capabilities.hasMultipleSensorSupport				= false;
    capabilities.hasPointCloudRegionSupport				= true;
    capabilities.hasPointCloudSupport					= true;
    capabilities.hasPositionConfidenceSupport			= true;
    capabilities.hasRGBCameraSupport					= true;
    capabilities.hasSkeletonSupport						= true;
    capabilities.hasUserMaskSupport						= true;
    capabilities.hasNearModeSupport                     = false;
	capabilities.hasSeatedSkeletonSupport				= false;
	capabilities.hasChooseSkeletonsSupport				= false;
    
    capabilities.maxSensors								= 1;
    capabilities.framework								= "openni";
    
    //some sensors don't have an RGB camera (asus xtion pro)
    XnStatus rc;
    xn::NodeInfoList imageNodes;
    rc = context.EnumerateProductionTrees(XN_NODE_TYPE_IMAGE, NULL, imageNodes, NULL );
    if(rc != XN_STATUS_OK)
    {
        capabilities.hasRGBCameraSupport = false;
        capabilities.hasUserMaskSupport = false;
    }
    
    //set default values
    setDefaults();
}

void OpenNIDevice::createPointCloudGenerator()
{
	pointCloudGenerator = new AKOpenNIPointCloudGenerator();
}

void OpenNIDevice::createUserMasksGenerator()
{
	userMasksGenerator = new AKOpenNIUserMasksGenerator();
}

void OpenNIDevice::createUserFrameGenerator()
{
	userFrameGenerator = new AKOpenNIUserFrameGenerator();
}

void OpenNIDevice::setDefaults()
{
    KinectDevice::setDefaults();
    
    frameRate = 30;
    
    userCallbacksRegistered = false;
    
    depthGenerator = NULL;    
    imageGenerator = NULL;
    userGenerator = NULL;
	infraredGenerator = NULL;

	depthGeneratorCreated = false;
	imageGeneratorCreated = false;
	userGeneratorCreated = false;
	infraredGeneratorCreated = false;

	//openni mirroring is inverse from MS SDK
	rgbImageBytesGenerator->setSourceMirrored(true);
	depthImageBytesGenerator->setSourceMirrored(true);

	pointCloudGenerator->setSourceRGBMirrored(true);
	pointCloudGenerator->setSourceDepthMirrored(true);

	userMasksGenerator->setSourceRGBMirrored(true);
	userMasksGenerator->setSourceDepthMirrored(true);
	userMasksGenerator->setSourceSceneMirrored(true);

	rgbParser = new AKOpenNIRGBParser();
	depthParser = new AKOpenNIDepthParser();
    infraredParser = new AKOpenNIInfraredParser();
    
    //player index coloring
	userIndexColors = new float*[userFrameGenerator->getMaxUsers()];
	for(int i = 0; i < userFrameGenerator->getMaxUsers(); i++)
	{
		userIndexColors[i] = new float[4];
	}

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

void OpenNIDevice::setRGBMode(int rgbWidth, int rgbHeight, int asRGBWidth, int asRGBHeight, bool asRGBMirrored)
{
	//support higher resolution
	if(asRGBWidth == 1280 && asRGBHeight == 960)
	{
		rgbWidth = 1280;
        //openni works with 1024 height instead of 960
		rgbHeight = 1024;
        //asRGBHeight = 1024;
        frameRate = 15;
	}
	KinectDevice::setRGBMode(rgbWidth, rgbHeight, asRGBWidth, asRGBHeight, asRGBMirrored);
}

void OpenNIDevice::setUserColor(int userID, int color, bool useIntensity)
{
	if(userID > userFrameGenerator->getMaxUsers()) return;
	
    userIndexColors[userID - 1][0] = (0xFF & (color >> 16)) / 255.0f;
    userIndexColors[userID - 1][1] = (0xFF & (color >> 8)) / 255.0f;
    userIndexColors[userID - 1][2] = (0xFF & (color)) / 255.0f;
    userIndexColors[userID - 1][3] = useIntensity ? 1.0f : 0.0f;
}

void OpenNIDevice::start()
{
    printf("OpenNIDevice::start()\n");
    if(!running)
    {
        running = true;
        mThread = boost::thread(&OpenNIDevice::deviceThread, this);
    }
}

void OpenNIDevice::stop()
{
    printf("OpenNIDevice::stop()\n");
    if(running)
    {
        running = false;
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
	cleanupByteArrays();

    //reset defaults
    setDefaults();
    if(started)
    {
        started = false;
        //send stopped event
        dispatchStatusMessage((const uint8_t*) "stopped");
    }
}

void OpenNIDevice::cleanupByteArrays()
{
	KinectDevice::cleanupByteArrays();

	if(rgbParser != 0)
		delete rgbParser;
	rgbParser = 0;

	if(depthParser != 0)
		delete depthParser;
	depthParser = 0;
    
    if(infraredParser != 0)
		delete infraredParser;
	infraredParser = 0;
}

void * OpenNIDevice::deviceThread(void *self)
{
    OpenNIDevice *adapter = (OpenNIDevice *) self;
    adapter->run();
    return NULL;
}

void OpenNIDevice::run()
{
    if(running)
    {  
        dispatchInfoMessage((const uint8_t*) "Starting Device");
        XnStatus rc;
        
        bool needsDepthGenerator = (asDepthEnabled || asPointCloudEnabled);
        bool needsImageGenerator = (asRGBEnabled || asUserMaskEnabled || asPointCloudEnabled);
        bool needsUserGenerator = ((asDepthEnabled && asDepthShowUserColors) || asUserMaskEnabled || asUserEnabled || asSkeletonEnabled);
        bool needsInfraredGenerator = asInfraredEnabled;
        
        if(needsDepthGenerator)
        {
            XnMapOutputMode depthMode;
            depthMode.nXRes = depthImageBytesGenerator->getSourceWidth();
            depthMode.nYRes = depthImageBytesGenerator->getSourceHeight();
            depthMode.nFPS = frameRate;
            
            dispatchInfoMessage((const uint8_t*) "Creating Depth Generator");
            rc = depthGenerator.Create(context);
            if(rc != XN_STATUS_OK)
            {
                dispatchErrorMessage((const uint8_t*) "Create DepthGenerator Failed");
            }
            else
            {
                depthGeneratorCreated = true;
                depthGenerator.SetMapOutputMode(depthMode);
                depthGenerator.GetMirrorCap().SetMirror(false);
            }
        }
        
        //check if running is still true, as OpenNi takes a while to initialize
        if(!running)
        {
            stop();
            return;
        }
        
        if(needsImageGenerator)
        {
            XnMapOutputMode rgbMode;
            rgbMode.nXRes = rgbImageBytesGenerator->getSourceWidth();
            rgbMode.nYRes = rgbImageBytesGenerator->getSourceHeight();
            rgbMode.nFPS = frameRate;
            
            dispatchInfoMessage((const uint8_t*) "Creating Image Generator");
            rc = imageGenerator.Create(context);
            if(rc != XN_STATUS_OK)
            {
                dispatchErrorMessage((const uint8_t*) "Create ImageGenerator Failed");
            }
            else
            {
                imageGeneratorCreated = true;
                imageGenerator.SetMapOutputMode(rgbMode);
                imageGenerator.GetMirrorCap().SetMirror(false);
            }
        }
        
        //check if running is still true, as OpenNi takes a while to initialize
        if(!running)
        {
            stop();
            return;
        }
        
        if(needsInfraredGenerator)
        {
            XnMapOutputMode infraredMode;
            infraredMode.nXRes = infraredImageBytesGenerator->getSourceWidth();
            infraredMode.nYRes = infraredImageBytesGenerator->getSourceHeight();
            infraredMode.nFPS = 30;
            
            dispatchInfoMessage((const uint8_t*) "Creating Infrared Generator");
            rc = infraredGenerator.Create(context);
            if(rc != XN_STATUS_OK)
            {
                dispatchErrorMessage((const uint8_t*) "Create Infrared Generator Failed");
            }
            else
            {
                infraredGeneratorCreated = true;
                infraredGenerator.SetMapOutputMode(infraredMode);
                infraredGenerator.GetMirrorCap().SetMirror(false);
            }
        }
        
        //check if running is still true, as OpenNi takes a while to initialize
        if(!running)
        {
            stop();
            return;
        }
        
        //align depth to rgb stream
        if(depthGeneratorCreated && imageGeneratorCreated && depthGenerator.IsCapabilitySupported(XN_CAPABILITY_ALTERNATIVE_VIEW_POINT))
        {
            depthGenerator.GetAlternativeViewPointCap().SetViewPoint(imageGenerator);
        }
        
        if(needsUserGenerator)
        {
            //initialize the user generator
            dispatchInfoMessage((const uint8_t*) "Creating User Generator");
            rc = userGenerator.Create(context);
            if(rc != XN_STATUS_OK)
            {
                dispatchErrorMessage((const uint8_t*) "Create Infrared Generator Failed");
            }
            else
            {
                userGeneratorCreated = true;
            }
        }
        
        //check if running is still true, as OpenNi takes a while to initialize
        if(!running)
        {
            stop();
            return;
        }
        
        if(userGeneratorCreated)
        {
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
                dispatchErrorMessage((const uint8_t*) "Supplied user generator doesn't support skeleton");
            }
            
            if (userGenerator.GetSkeletonCap().NeedPoseForCalibration())
            {
                needPose = true;
                if (!userGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
                {
                    dispatchErrorMessage((const uint8_t*) "Pose required, but not supported");
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
        }
        
        //check if running is still true, as OpenNi takes a while to initialize
        if(!running)
        {
            stop();
            return;
        }
        
        if(depthGeneratorCreated)
        {
            dispatchInfoMessage((const uint8_t*) "Starting Depth Generator");
            rc = depthGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                dispatchErrorMessage((const uint8_t*) "Starting Depth Generator Failed");
            }
        }
        if(imageGeneratorCreated)
        {
            dispatchInfoMessage((const uint8_t*) "Starting Image Generator");
            rc = imageGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                dispatchErrorMessage((const uint8_t*) "Starting Image Generator Failed");
            }
        }
        if(userGeneratorCreated)
        {
            dispatchInfoMessage((const uint8_t*) "Starting User Generator");
            rc = userGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                dispatchErrorMessage((const uint8_t*) "Starting User Generator Failed");
            }
        }
        //you cant have both infrared and rgb
        if(infraredGeneratorCreated && !(imageGeneratorCreated && imageGenerator.IsGenerating()))
        {
            dispatchInfoMessage((const uint8_t*) "Starting Infrared Generator");
            rc = infraredGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                dispatchInfoMessage((const uint8_t*) "Starting Infrared Generator Failed");
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
        dispatchStatusMessage((const uint8_t*) "started");
        while(running)
        {
            XnStatus rc = XN_STATUS_OK;
            
            if(imageGeneratorCreated && imageGenerator.IsGenerating())
            {
                rc = imageGenerator.WaitAndUpdateData();
                if (rc != XN_STATUS_OK)
                {
                    dispatchErrorMessage((const uint8_t*) "RGB Read Failed");
                    break;
                }
				imageGenerator.GetMetaData(imageMetaData);
            }
            
            if(depthGeneratorCreated && depthGenerator.IsGenerating())
            {
                rc = depthGenerator.WaitAndUpdateData();
                if (rc != XN_STATUS_OK)
                {
                    dispatchErrorMessage((const uint8_t*) "Depth Read Failed");
                    break;
                }
                depthGenerator.GetMetaData(depthMetaData);
            }
            
            if(infraredGeneratorCreated && infraredGenerator.IsGenerating())
            {
                rc = infraredGenerator.WaitAndUpdateData();
                if (rc != XN_STATUS_OK)
                {
                    dispatchErrorMessage((const uint8_t*) "IR Read Failed");
                    break;
                }
                infraredGenerator.GetMetaData(infraredMetaData);
            }
            
            if(userGeneratorCreated && userGenerator.IsGenerating())
            {
                userGenerator.WaitAndUpdateData();
                userGenerator.GetUserPixels(0, sceneMetaData);
            }

			if(imageGeneratorCreated && imageGenerator.IsGenerating())
			{
				lockRGBMutex();
                readRGBFrame();
                unlockRGBMutex();
			}

			if(depthGeneratorCreated && depthGenerator.IsGenerating())
            {
				lockDepthMutex();
				readDepthFrame();
				unlockDepthMutex();
			}
            
            if(infraredGeneratorCreated && infraredGenerator.IsGenerating())
            {
                lockInfraredMutex();
                readInfraredFrame();
                unlockInfraredMutex();
            }

			dispatchRGBIfNeeded();
			dispatchDepthIfNeeded();
			dispatchInfraredIfNeeded();
			dispatchPointCloudIfNeeded();
			dispatchUserFrameIfNeeded();
			dispatchUserMaskIfNeeded();
        }
    }
}

void OpenNIDevice::readRGBFrame()
{
	rgbParser->setImageSize(rgbImageBytesGenerator->getSourceWidth(), rgbImageBytesGenerator->getSourceHeight());
	rgbParser->setImageMetaData(&imageMetaData);
	rgbParser->parseData();
}

void OpenNIDevice::readDepthFrame()
{
	depthParser->setImageSize(depthImageBytesGenerator->getSourceWidth(), depthImageBytesGenerator->getSourceHeight());
	depthParser->setDepthMetaData(&depthMetaData);
	depthParser->setSceneMetaData(&sceneMetaData);
	depthParser->setShowUserColors(asDepthShowUserColors);
	depthParser->setUserIndexColors(userIndexColors);
	depthParser->parseData();
}

void OpenNIDevice::readInfraredFrame()
{
    infraredParser->setImageSize(infraredImageBytesGenerator->getSourceWidth(), infraredImageBytesGenerator->getSourceHeight());
    infraredParser->setInfraredMetaData(&infraredMetaData);
    infraredParser->parseData();
}

void OpenNIDevice::dispatchRGBIfNeeded()
{
	if(asRGBEnabled && imageGeneratorCreated)
    {
        lockRGBMutex();
        rgbImageBytesGenerator->setSourceBytes(rgbParser->getImageByteArray());
		rgbImageBytesGenerator->generateTargetBytes();
        unlockRGBMutex();
        dispatchStatusMessage((const uint8_t*) "RGBFrame");
    }
}

void OpenNIDevice::dispatchDepthIfNeeded()
{
	if(asDepthEnabled && depthGeneratorCreated)
    {
        lockDepthMutex();
		depthImageBytesGenerator->setSourceBytes(depthParser->getImageByteArray());
		depthImageBytesGenerator->generateTargetBytes();
        unlockDepthMutex();
        dispatchStatusMessage((const uint8_t*) "depthFrame");
    }
}

void OpenNIDevice::dispatchInfraredIfNeeded()
{
	if(asInfraredEnabled && infraredGeneratorCreated && !(imageGeneratorCreated && imageGenerator.IsGenerating()))
    {
        lockInfraredMutex();
        infraredImageBytesGenerator->setSourceBytes(infraredParser->getImageByteArray());
        infraredImageBytesGenerator->generateTargetBytes();
        unlockInfraredMutex();
        dispatchStatusMessage((const uint8_t*) "infraredFrame");
    }
}

void OpenNIDevice::dispatchPointCloudIfNeeded()
{
	if(asPointCloudEnabled)
	{
		lockPointCloudMutex();

		pointCloudGenerator->setNumRegions(numRegions);
		pointCloudGenerator->setPointCloudRegions(pointCloudRegions);
		pointCloudGenerator->setSourceDepthBytes(depthParser->getDepthByteArray());
		pointCloudGenerator->setSourceDepthSize(depthParser->getWidth(), depthParser->getHeight());
		pointCloudGenerator->setSourceRGBBytes(rgbParser->getImageByteArray());
		pointCloudGenerator->setSourceRGBSize(rgbParser->getWidth(), rgbParser->getHeight());

		pointCloudGenerator->generateTargetBytes();

		unlockPointCloudMutex();
		dispatchStatusMessage((const uint8_t*) "pointCloudFrame");
	}
}

void OpenNIDevice::dispatchUserFrameIfNeeded()
{
	if((asUserEnabled || asSkeletonEnabled || asUserMaskEnabled) && userGeneratorCreated)
	{
		lockUserMutex();

		((AKOpenNIUserFrameGenerator*) userFrameGenerator)->setDepthGenerator(&depthGenerator);
		((AKOpenNIUserFrameGenerator*) userFrameGenerator)->setUserGenerator(&userGenerator);
		userFrameGenerator->setSkeletonMirrored(asSkeletonMirrored);
        userFrameGenerator->setSkeletonTrackingEnabled(asSkeletonEnabled);
		userFrameGenerator->setDepthTargetMirrored(depthImageBytesGenerator->getTargetMirrored());
		userFrameGenerator->setDepthTargetSize(depthImageBytesGenerator->getTargetWidth(), depthImageBytesGenerator->getTargetHeight());
		userFrameGenerator->setDepthSourceSize(depthImageBytesGenerator->getSourceWidth(), depthImageBytesGenerator->getSourceHeight());
		userFrameGenerator->setRGBTargetMirrored(rgbImageBytesGenerator->getTargetMirrored());
		userFrameGenerator->setRGBTargetSize(rgbImageBytesGenerator->getTargetWidth(), rgbImageBytesGenerator->getTargetHeight());
		userFrameGenerator->setRGBSourceSize(rgbImageBytesGenerator->getSourceWidth(), rgbImageBytesGenerator->getSourceHeight());
		userFrameGenerator->generateUserFrame();

		unlockUserMutex();
		dispatchStatusMessage((const uint8_t*) "userFrame");
	}
}

void OpenNIDevice::dispatchUserMaskIfNeeded()
{
	if(asUserMaskEnabled)
	{
		lockUserMaskMutex();
		
		((AKOpenNIUserMasksGenerator*) userMasksGenerator)->setSceneMetaData(&sceneMetaData);
		userMasksGenerator->setSourceDepthBytes(depthParser->getDepthByteArray());
		userMasksGenerator->setSourceRGBBytes(rgbParser->getImageByteArray());
		userMasksGenerator->generateTargetBytes();

		unlockUserMaskMutex();
		dispatchStatusMessage((const uint8_t*) "userMaskFrame");
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
#endif
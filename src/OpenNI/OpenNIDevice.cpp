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

OpenNIDevice::OpenNIDevice(int nr, xn::Context context)
{
    printf("OpenNIDevice::OpenNIDevice(%i)\n", nr);
    this->nr = nr;
    this->freContext = freContext;
    this->context = context;
    
    //initialize the capabilities of this device
#ifdef AIRKINECT_OS_OSX
    capabilities.hasCameraElevationSupport				= true;
#else
    capabilities.hasCameraElevationSupport				= false;
#endif

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
    capabilities.hasHandTrackingSupport                 = false;
    
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

	//openni mirroring is inverse from MS SDK
	rgbImageBytesGenerator->setSourceMirrored(true);
	depthImageBytesGenerator->setSourceMirrored(true);

	pointCloudGenerator->setSourceRGBMirrored(true);
	pointCloudGenerator->setSourceDepthMirrored(true);

	userMasksGenerator->setSourceRGBMirrored(true);
	userMasksGenerator->setSourceDepthMirrored(true);
	userMasksGenerator->setSourceSceneMirrored(true);
    
    rgbGenerator = new AKOpenNIRGBGenerator(this);
	depthGenerator = new AKOpenNIDepthGenerator(this);
    irGenerator = new AKOpenNIIRGenerator(this);
    userGenerator = new AKOpenNIUserGenerator(this);
    
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
    
#ifdef AIRKINECT_OS_OSX
    hardware = new AKOpenNIHardware(this);
#endif
}

FREObject OpenNIDevice::freSetSkeletonMode(FREObject argv[])
{
    KinectDevice::freSetSkeletonMode(argv);
    if(userGenerator != 0 && userGenerator->userGeneratorCreated())
    {
        userGenerator->setSmoothing(asSkeletonSmoothing);
    }
    return NULL;
}

FREObject OpenNIDevice::freCameraElevationGetAngle(FREObject argv[])
{
	dispatchInfoMessage((const uint8_t*) "[OpenNIDevice] Camera Elevation Get Angle Called");
	FREObject asCameraAngle;
	long degrees = 0;
	if(running)
	{
#ifdef AIRKINECT_OS_OSX
        degrees = hardware->getTiltAngle();
#endif
	}
	FRENewObjectFromInt32 ((int) degrees, &asCameraAngle );
	return asCameraAngle;
}

FREObject OpenNIDevice::freCameraElevationSetAngle(FREObject argv[])
{
	dispatchInfoMessage((const uint8_t*) "[OpenNIDevice] Camera Elevation Set Angle Called");
	if(running)
	{
		int degrees; FREGetObjectAsInt32(argv[1], &degrees);
#ifdef AIRKINECT_OS_OSX
        hardware->setTiltAngle(degrees);
#endif
	}
	return NULL;
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
    
    delete depthGenerator;
    delete rgbGenerator;
    delete irGenerator;
    delete userGenerator;
    
#ifdef AIRKINECT_OS_OSX
    delete hardware;
#endif

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
        
#ifdef AIRKINECT_OS_OSX
        hardware->setup();
#endif
        
        bool needsDepthGenerator = (asDepthEnabled || asPointCloudEnabled);
        bool needsImageGenerator = (asRGBEnabled || asUserMaskEnabled || asPointCloudEnabled);
        bool needsUserGenerator = ((asDepthEnabled && asDepthShowUserColors) || asUserMaskEnabled || asUserEnabled || asSkeletonEnabled);
        bool needsInfraredGenerator = asInfraredEnabled;
        
        if(needsDepthGenerator)
        {
            depthGenerator->setup(&context, depthImageBytesGenerator->getSourceWidth(), depthImageBytesGenerator->getSourceHeight(), frameRate);
            if(!running) return stop();
        }
        
        if(needsImageGenerator)
        {
            rgbGenerator->setup(&context, rgbImageBytesGenerator->getSourceWidth(), rgbImageBytesGenerator->getSourceHeight(), frameRate);
            if(!running) return stop();
        }
        
        if(needsInfraredGenerator)
        {
            irGenerator->setup(&context, infraredImageBytesGenerator->getSourceWidth(), infraredImageBytesGenerator->getSourceHeight(), frameRate);
            if(!running) return stop();
        }
        
        //align depth to rgb stream
        if(depthGenerator->depthGeneratorCreated() && rgbGenerator->imageGeneratorCreated() && depthGenerator->getXnDepthGenerator()->IsCapabilitySupported(XN_CAPABILITY_ALTERNATIVE_VIEW_POINT))
        {
            depthGenerator->getXnDepthGenerator()->GetAlternativeViewPointCap().SetViewPoint(*(rgbGenerator->getXnImageGenerator()));
            if(!running) return stop();
        }
        
        if(needsUserGenerator)
        {
            userGenerator->setup(&context);
            userGenerator->setSmoothing(asSkeletonSmoothing);
            if(!running) return stop();
        }
        
        depthGenerator->start();
        if(!running) return stop();
        
        rgbGenerator->start();
        if(!running) return stop();
        
        //you cant have both infrared and rgb
        if(irGenerator->irGeneratorCreated() && !(rgbGenerator->imageGeneratorCreated() && rgbGenerator->getXnImageGenerator()->IsGenerating()))
        {
            irGenerator->start();
            if(!running) return stop();
        }
        
        userGenerator->start();
        if(!running) return stop();
        
        //send started event
        started = true;
        dispatchStatusMessage((const uint8_t*) "started");
        while(running)
        {
            if(!rgbGenerator->update()) break;
            if(!depthGenerator->update()) break;
            if(!irGenerator->update()) break;
            if(!userGenerator->update()) break;
            
#ifdef AIRKINECT_OS_OSX
            hardware->update();
#endif
            
            lockRGBMutex();
            rgbGenerator->parse();
            unlockRGBMutex();
            
            lockDepthMutex();
            depthGenerator->updateUserIndexColors(asDepthShowUserColors, userIndexColors, userGenerator->getXnSceneMetaData());
            depthGenerator->parse();
            unlockDepthMutex();
            
            lockInfraredMutex();
            irGenerator->parse();
            unlockInfraredMutex();

			dispatchRGBIfNeeded();
			dispatchDepthIfNeeded();
			dispatchInfraredIfNeeded();
			dispatchPointCloudIfNeeded();
			dispatchUserFrameIfNeeded();
			dispatchUserMaskIfNeeded();
        }
    }
}

void OpenNIDevice::dispatchRGBIfNeeded()
{
	if(asRGBEnabled && rgbGenerator->isGenerating())
    {
        lockRGBMutex();
        rgbImageBytesGenerator->setSourceBytes(rgbGenerator->getImageByteArray());
		rgbImageBytesGenerator->generateTargetBytes();
        unlockRGBMutex();
        dispatchStatusMessage((const uint8_t*) "RGBFrame");
    }
}

void OpenNIDevice::dispatchDepthIfNeeded()
{
	if(asDepthEnabled && depthGenerator->isGenerating())
    {
        lockDepthMutex();
		depthImageBytesGenerator->setSourceBytes(depthGenerator->getImageByteArray());
		depthImageBytesGenerator->generateTargetBytes();
        unlockDepthMutex();
        dispatchStatusMessage((const uint8_t*) "depthFrame");
    }
}

void OpenNIDevice::dispatchInfraredIfNeeded()
{
	if(asInfraredEnabled && irGenerator->isGenerating() && !rgbGenerator->isGenerating())
    {
        lockInfraredMutex();
        infraredImageBytesGenerator->setSourceBytes(irGenerator->getImageByteArray());
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
		pointCloudGenerator->setSourceDepthBytes(depthGenerator->getDepthByteArray());
		pointCloudGenerator->setSourceDepthSize(depthGenerator->getSourceWidth(), depthGenerator->getSourceHeight());
		pointCloudGenerator->setSourceRGBBytes(rgbGenerator->getImageByteArray());
		pointCloudGenerator->setSourceRGBSize(rgbGenerator->getSourceWidth(), rgbGenerator->getSourceHeight());

		pointCloudGenerator->generateTargetBytes();

		unlockPointCloudMutex();
		dispatchStatusMessage((const uint8_t*) "pointCloudFrame");
	}
}

void OpenNIDevice::dispatchUserFrameIfNeeded()
{
	if((asUserEnabled || asSkeletonEnabled || asUserMaskEnabled) && userGenerator->isGenerating())
	{
		lockUserMutex();

		((AKOpenNIUserFrameGenerator*) userFrameGenerator)->setDepthGenerator(depthGenerator->getXnDepthGenerator());
		((AKOpenNIUserFrameGenerator*) userFrameGenerator)->setUserGenerator(userGenerator->getXnUserGenerator());
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
		
		((AKOpenNIUserMasksGenerator*) userMasksGenerator)->setSceneMetaData(userGenerator->getXnSceneMetaData());
		userMasksGenerator->setSourceDepthBytes(depthGenerator->getDepthByteArray());
		userMasksGenerator->setSourceRGBBytes(rgbGenerator->getImageByteArray());
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
#endif
#include "OpenNIDevice.h"
#ifdef AIRKINECT_TARGET_OPENNI

#ifdef AIRKINECT_OS_OSX
    #include <iostream>
    #include <math.h>
#endif

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
    capabilities.hasJointOrientationConfidenceSupport	= true;
    capabilities.hasJointOrientationSupport				= true;
    capabilities.hasMultipleSensorSupport				= false;
    capabilities.hasPointCloudRegionSupport				= true;
    capabilities.hasPointCloudSupport					= true;
    capabilities.hasPositionConfidenceSupport			= true;
    capabilities.hasRGBCameraSupport					= true;
    capabilities.hasSkeletonSupport						= true;
    capabilities.hasUserMaskSupport						= true;
    capabilities.hasNearModeSupport                     = false;
    
    capabilities.maxSensors								= 1;
    capabilities.framework								= "openni";
    
    //set class names of actionscript equivalents
	asJointClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNISkeletonJoint";
	asUserClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNIUser";
	asUserFrameClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNIUserFrame";
	numJoints = 24;
	maxSkeletons = 15;
	jointNames = new char*[numJoints];
	jointNames[0] = "head";
	jointNames[1] = "neck";
	jointNames[2] = "torso";
	jointNames[3] = "waist";
    
	jointNames[4] = "left_collar";
	jointNames[5] = "left_shoulder";
	jointNames[6] = "left_elbow";
	jointNames[7] = "left_wrist";
	jointNames[8] = "left_hand";
	jointNames[9] = "left_fingertip";
    
	jointNames[10] = "right_collar";
	jointNames[11] = "right_shoulder";
	jointNames[12] = "right_elbow";
	jointNames[13] = "right_wrist";
	jointNames[14] = "right_hand";
	jointNames[15] = "right_fingertip";
    
	jointNames[16] = "left_hip";
	jointNames[17] = "left_knee";
	jointNames[18] = "left_ankle";
	jointNames[19] = "left_foot";
    
	jointNames[20] = "right_hip";
	jointNames[21] = "right_knee";
	jointNames[22] = "right_ankle";
	jointNames[23] = "right_foot";
    
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

void OpenNIDevice::setDefaults()
{
    KinectDevice::setDefaults();
    //set some default values
    
    userCallbacksRegistered = false;
    
    depthGenerator = NULL;    
    imageGenerator = NULL;
    userGenerator = NULL;
    
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
    
    asInfraredByteArray = 0;
    
    //player index coloring
	userIndexColors = new float*[maxSkeletons];
	for(int i = 0; i < maxSkeletons; i++)
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

void OpenNIDevice::setUserColor(int userID, int color, bool useIntensity)
{
	if(userID > maxSkeletons) return;
	
    userIndexColors[userID - 1][0] = (0xFF & (color >> 16)) / 255.0f;
    userIndexColors[userID - 1][1] = (0xFF & (color >> 8)) / 255.0f;
    userIndexColors[userID - 1][2] = (0xFF & (color)) / 255.0f;
    userIndexColors[userID - 1][3] = useIntensity ? 1.0f : 0.0f;
}

//START FRE FUNCTIONS

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
    
    if(asInfraredByteArray != 0) delete [] asInfraredByteArray;
    asInfraredByteArray = new uint32_t[asInfraredPixelCount];
    
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
    memcpy(byteArray.bytes, asInfraredByteArray, numInfraredBytes);
    unlockInfraredMutex();
    FREReleaseByteArray(objectByteArray);
    
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
    if(asDepthByteArray != 0) delete [] asDepthByteArray;
    if(asRGBByteArray != 0) delete [] asRGBByteArray;
    if(asInfraredByteArray != 0) delete [] asInfraredByteArray;
    if(asPointCloudByteArray != 0) delete [] asPointCloudByteArray;
    if(asUserMaskByteArray != 0)
    {
        for(int i = 0; i < maxSkeletons; i++)
        {
            delete [] asUserMaskByteArray[i];
        }
        delete [] asUserMaskByteArray;
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
        FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Starting Device");
        XnStatus rc;
        
        XnMapOutputMode depthMode;
        depthMode.nXRes = depthWidth;
        depthMode.nYRes = depthHeight;
        depthMode.nFPS = 30;
        
        bool depthGeneratorCreated = false;
        FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Creating Depth Generator");
        rc = depthGenerator.Create(context);
        if(rc != XN_STATUS_OK)
        {
            FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Create DepthGenerator Failed");
            //printf("OpenNIDevice create depthGenerator failed: %s\n", xnGetStatusString(rc));
        }
        else
        {
            depthGeneratorCreated = true;
            depthGenerator.SetMapOutputMode(depthMode);
            depthGenerator.GetMirrorCap().SetMirror(false);
        }
        
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
        
        bool imageGeneratorCreated = false;
        FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Creating Image Generator");
        rc = imageGenerator.Create(context);
        if(rc != XN_STATUS_OK)
        {
            FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Create ImageGenerator Failed");
            //printf("OpenNIDevice create imageGenerator failed: %s\n", xnGetStatusString(rc));
        }
        else
        {
            imageGeneratorCreated = true;
            imageGenerator.SetMapOutputMode(rgbMode);
            imageGenerator.GetMirrorCap().SetMirror(false);
        }
        
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
        
        bool infraredGeneratorCreated = false;
        FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Creating Infrared Generator");
        rc = infraredGenerator.Create(context);
        if(rc != XN_STATUS_OK)
        {
            FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Create Infrared Generator Failed");
            //printf("OpenNIDevice create infraredGenerator failed: %s\n", xnGetStatusString(rc));
        }
        else
        {
            infraredGeneratorCreated = true;
            infraredGenerator.SetMapOutputMode(infraredMode);
            infraredGenerator.GetMirrorCap().SetMirror(false);
        }
        
        //check if running is still true, as OpenNi takes a while to initialize
        if(!running)
        {
            stop();
            return;
        }
        
        //map depth to rgb stream
        if(depthGeneratorCreated && imageGeneratorCreated && depthGenerator.IsCapabilitySupported(XN_CAPABILITY_ALTERNATIVE_VIEW_POINT))
        {
            depthGenerator.GetAlternativeViewPointCap().SetViewPoint(imageGenerator);
        }
        
        //initialize the user generator
        bool userGeneratorCreated = false;
        FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Creating User Generator");
        rc = userGenerator.Create(context);
        if(rc != XN_STATUS_OK)
        {
            FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Create Infrared Generator Failed");
            //printf("OpenNIDevice create userGenerator failed: %s\n", xnGetStatusString(rc));
        }
        else
        {
            userGeneratorCreated = true;
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
        
        if(depthGeneratorCreated && (asDepthEnabled || asPointCloudEnabled))
        {
            FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Starting Depth Generator");
            rc = depthGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Starting Depth Generator Failed");
                //printf("OpenNIDevice start depthGenerator failed: %s\n", xnGetStatusString(rc));
            }
        }
        if(imageGeneratorCreated && (asRGBEnabled || asUserMaskEnabled || asPointCloudEnabled))
        {
            FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Starting Image Generator");
            rc = imageGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Starting Image Generator Failed");
                //printf("OpenNIDevice start imageGenerator failed: %s\n", xnGetStatusString(rc));
            }
        }
        if(userGeneratorCreated && ((asDepthEnabled && asDepthShowUserColors) || asUserMaskEnabled || asUserEnabled || asSkeletonEnabled))
        {
            FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Starting User Generator");
            rc = userGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Starting User Generator Failed");
                //printf("OpenNIDevice start userGenerator failed: %s\n", xnGetStatusString(rc));
            }
        }
        //you cant have both infrared and rgb
        if(infraredGeneratorCreated && (asInfraredEnabled && !(imageGeneratorCreated && imageGenerator.IsGenerating())))
        {
            FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Starting Infrared Generator");
            rc = infraredGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Starting Infrared Generator Failed");
                //printf("OpenNIDevice start infraredGenerator failed: %s\n", xnGetStatusString(rc));
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
            
            if(imageGeneratorCreated && imageGenerator.IsGenerating())
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
            
            if(depthGeneratorCreated && depthGenerator.IsGenerating())
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
            
            if(infraredGeneratorCreated && infraredGenerator.IsGenerating())
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
            
            if(userGeneratorCreated && userGenerator.IsGenerating())
            {
                //read a new User frame
                userGenerator.WaitAndUpdateData();
                //get the user pixels
                userGenerator.GetUserPixels(0, sceneMetaData);
            }
            
            //rgb image
            if(asRGBEnabled && imageGeneratorCreated)
            {
                lockRGBMutex();
                rgbFrameHandler();
                unlockRGBMutex();
                FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "RGBFrame");
            }
            
            //depth image
            if(asDepthEnabled && depthGeneratorCreated)
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
            if(asInfraredEnabled && infraredGeneratorCreated && !(imageGeneratorCreated && imageGenerator.IsGenerating()))
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
            if((asUserEnabled || asSkeletonEnabled || asUserMaskEnabled) && userGeneratorCreated)
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
    
    uint32_t *rgbRun = asRGBByteArray;
    int direction = asRGBMirrored ? -1 : 1;
    int directionFactor = asRGBMirrored ? 1 : 0;
    
    for(int y = 0; y < asRGBHeight; y++)
    {
        const XnRGB24Pixel *pRGBBuffer = RGBFrameBuffer + ((y + directionFactor) * (rgbWidth * rgbScale)) - directionFactor;
        
        for(int x = 0; x < asRGBWidth; x++)
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
    
    uint32_t *depthRun = asDepthByteArray;
    int direction = asDepthMirrored ? -1 : 1;
    int directionFactor = asDepthMirrored ? 1 : 0;
    
    unsigned int red, green, blue;
    float value;
    
    for(int y = 0; y < asDepthHeight; y++)
    {
        const XnDepthPixel *pDepthBuffer = depthFrameBuffer + ((y + directionFactor) * (depthWidth * depthScale)) - directionFactor;
        
        for(int x = 0; x < asDepthWidth; x++)
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
    
    uint32_t *depthRun = asDepthByteArray;
    int direction = asDepthMirrored ? -1 : 1;
    int directionFactor = asDepthMirrored ? 1 : 0;
    
    unsigned int red, green, blue;
    float value;
    
    for(int y = 0; y < asDepthHeight; y++)
    {
        const XnDepthPixel *pDepthBuffer = depthFrameBuffer + ((y + directionFactor) * (depthWidth * depthScale)) - directionFactor;
        
        const XnLabel *pSceneBuffer = sceneFrameBuffer + ((y + directionFactor) * (depthWidth * depthScale)) - directionFactor;
        
        for(int x = 0; x < asDepthWidth; x++)
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
    for(int y = 0; y < asUserMaskHeight; y++)
    {
        const XnRGB24Pixel *pRGBBuffer = RGBFrameBuffer + ((y + directionFactor) * (rgbWidth * userMaskScale)) - directionFactor;
        const XnLabel *pSceneBuffer = sceneFrameBuffer + ((y + directionFactor) * (depthWidth * userMaskScale)) - directionFactor;
        for(int x = 0; x < asUserMaskWidth; x++)
        {
            XnLabel label = *pSceneBuffer;
            
            for(int i = 0; i < maxSkeletons; i++)
            {
                asUserMaskByteArray[i][pixelNr] = 0;
            }
            if(label > 0)
            {
                asUserMaskByteArray[label - 1][pixelNr] = 0xff << 24 | ((*pRGBBuffer).nBlue + ((*pRGBBuffer).nGreen << 8) + ((*pRGBBuffer).nRed << 16));
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
    
    uint32_t *depthRun = asInfraredByteArray;
    int direction = asInfraredMirrored ? -1 : 1;
    int directionFactor = asInfraredMirrored ? 1 : 0;
    
    unsigned int red, green, blue;
    float value;
    
    for(int y = 0; y < asInfraredHeight; y++)
    {
        const XnIRPixel *pInfraredBuffer = infraredFrameBuffer + ((y + directionFactor) * (infraredWidth * infraredScale)) - directionFactor;
        
        for(int x = 0; x < asInfraredWidth; x++)
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
    
    short *pointCloudRun = asPointCloudByteArray;
    int direction = asPointCloudMirrored ? -1 : 1;
    int directionFactor = asPointCloudMirrored ? 1 : 0;
    
    if(pointCloudRegions != 0)
    {
        for(unsigned int i = 0; i < numRegions; i++)
        {
            pointCloudRegions[i].numPoints = 0;
        }
    }
    else
    {
        numRegions = 0;
    }
    
    for(int y = 0; y < asPointCloudHeight; y+=asPointCloudDensity)
    {
        const XnDepthPixel *pDepthBuffer = depthFrameBuffer + ((y + directionFactor) * (pointCloudWidth * pointCloudScale)) - directionFactor;
        
        for(int x = 0; x < asPointCloudWidth; x+=asPointCloudDensity)
        {
            //write to point cloud
            *pointCloudRun = x;
            pointCloudRun++;
            *pointCloudRun = y;
            pointCloudRun++;
            *pointCloudRun = *pDepthBuffer;
            pointCloudRun++;
            
            //check regions
            for(unsigned int i = 0; i < numRegions; i++)
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
    
    short *pointCloudRun = asPointCloudByteArray;
    int direction = asPointCloudMirrored ? -1 : 1;
    int directionFactor = asPointCloudMirrored ? 1 : 0;
    
    if(pointCloudRegions != 0)
    {
        for(unsigned int i = 0; i < numRegions; i++)
        {
            pointCloudRegions[i].numPoints = 0;
        }
    }
    else
    {
        numRegions = 0;
    }
    
    for(int y = 0; y < asPointCloudHeight; y+=asPointCloudDensity)
    {
        const XnRGB24Pixel *pRGBBuffer = RGBFrameBuffer + ((y + directionFactor) * (rgbWidth * pointCloudScale)) - directionFactor;
        const XnDepthPixel *pDepthBuffer = depthFrameBuffer + ((y + directionFactor) * (pointCloudWidth * pointCloudScale)) - directionFactor;
        
        for(int x = 0; x < asPointCloudWidth; x+=asPointCloudDensity)
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
            for(unsigned int i = 0; i < numRegions; i++)
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
    
	XnUserID *aUsers = new XnUserID[maxSkeletons];
    //XnUserID aUsers[maxSkeletons];
    XnUInt16 nUsers = maxSkeletons;
    XnUInt16 trackedUsers = userGenerator.GetNumberOfUsers();
    XnPoint3D position;
    XnStatus rc;
    
    userGenerator.GetUsers(aUsers, nUsers);
    
    userFrame.frameNumber = userGenerator.GetFrameID();
    userFrame.timeStamp = (int) (userGenerator.GetTimestamp() / 1000);
    
    for (int i = 0; i < maxSkeletons; ++i)
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
    
    //kUser.joints[targetIndex].orientationX = atan2f(orientation.orientation.elements[7], orientation.orientation.elements[8]);
    //kUser.joints[targetIndex].orientationY = -asinf(orientation.orientation.elements[6]);
    //kUser.joints[targetIndex].orientationZ = atan2f(orientation.orientation.elements[3], orientation.orientation.elements[0]);
    
    kUser.joints[targetIndex].positionConfidence = jointPositionConfidence;

    kUser.joints[targetIndex].worldX = jointPositionX;
    //if(asSkeletonMirrored) skeleton.joints[targetIndex].x = .5 - skeleton.joints[targetIndex].x;
    kUser.joints[targetIndex].worldY = jointPositionY;
    kUser.joints[targetIndex].worldZ = jointPositionZ;
    
    kUser.joints[targetIndex].worldRelativeX = (depthWidth - jointPositionX) / (depthWidth * 2) - .5;
    //if(asSkeletonMirrored) skeleton.joints[targetIndex].x = .5 - skeleton.joints[targetIndex].x;
    kUser.joints[targetIndex].worldRelativeY = -1 * (((depthHeight - jointPositionY) / (depthHeight * 2)) - .5);
    kUser.joints[targetIndex].worldRelativeZ = (jointPositionZ * 7.8125) / MAX_DEPTH;
    
    
    //relative position / depth & rgb are aligned
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
    for (int y = 0; y < depthHeight; ++y)
    {
        for (int x = 0; x < depthWidth; ++x, ++depthFrameBuffer)
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
            depthHistogram[nIndex] = (unsigned int)(256.0 * (1.0f - (depthHistogram[nIndex] / nNumberOfPoints)));
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
#endif
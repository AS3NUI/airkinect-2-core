#include "OpenNIDevice.h"
#ifdef AIRKINECT_TARGET_OPENNI

#ifdef AIRKINECT_OS_OSX
    #include <iostream>
    #include <math.h>
#endif

#include "AKOpenNIPointCloudGenerator.h"
#include "AKOpenNIUserMasksGenerator.h"

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
    
    //set class names of actionscript equivalents
	asJointClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNISkeletonJoint";
	asUserClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNIUser";
	asUserFrameClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNIUserFrame";

	maxSkeletons = 15;
    
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

void OpenNIDevice::setNumJointsAndJointNames()
{
	numJoints = 15;
	jointNames = new char*[numJoints];
	jointNames[0] = "head";
	jointNames[1] = "neck";
	jointNames[2] = "torso";
    
	jointNames[3] = "left_shoulder";
	jointNames[4] = "left_elbow";
	jointNames[5] = "left_hand";
    
	jointNames[6] = "right_shoulder";
	jointNames[7] = "right_elbow";
	jointNames[8] = "right_hand";
    
	jointNames[9] = "left_hip";
	jointNames[10] = "left_knee";
	jointNames[11] = "left_foot";
    
	jointNames[12] = "right_hip";
	jointNames[13] = "right_knee";
	jointNames[14] = "right_foot";
}

void OpenNIDevice::createPointCloudGenerator()
{
	pointCloudGenerator = new AKOpenNIPointCloudGenerator();
}

void OpenNIDevice::createUserMasksGenerator()
{
	userMasksGenerator = new AKOpenNIUserMasksGenerator();
}

void OpenNIDevice::setDefaults()
{
    KinectDevice::setDefaults();
    
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
    
    asInfraredWidth = 320;
    asInfraredHeight = 240;
    asInfraredPixelCount = asInfraredWidth * asInfraredHeight;
    asInfraredMirrored = false;
    asInfraredEnabled = false;
    
    infraredWidth = 640;
    infraredHeight = 480;
    infraredPixelCount = infraredWidth * infraredHeight;
    infraredScale = infraredWidth / asInfraredWidth;
    
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

    if(asInfraredByteArray != 0) 
		delete [] asInfraredByteArray;
	asInfraredByteArray = 0;
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
        
        XnMapOutputMode depthMode;
		depthMode.nXRes = depthImageBytesGenerator->getSourceWidth();
		depthMode.nYRes = depthImageBytesGenerator->getSourceHeight();
        depthMode.nFPS = 30;
        
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
        
        //check if running is still true, as OpenNi takes a while to initialize
        if(!running)
        {
            stop();
            return;
        }  
        
        XnMapOutputMode rgbMode;
        rgbMode.nXRes = rgbImageBytesGenerator->getSourceWidth();
        rgbMode.nYRes = rgbImageBytesGenerator->getSourceHeight();
        rgbMode.nFPS = 30;
        
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
        
        //check if running is still true, as OpenNi takes a while to initialize
        if(!running)
        {
            stop();
            return;
        }
        
        if(depthGeneratorCreated && (asDepthEnabled || asPointCloudEnabled))
        {
            dispatchInfoMessage((const uint8_t*) "Starting Depth Generator");
            rc = depthGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                dispatchErrorMessage((const uint8_t*) "Starting Depth Generator Failed");
            }
        }
        if(imageGeneratorCreated && (asRGBEnabled || asUserMaskEnabled || asPointCloudEnabled))
        {
            dispatchInfoMessage((const uint8_t*) "Starting Image Generator");
            rc = imageGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                dispatchErrorMessage((const uint8_t*) "Starting Image Generator Failed");
            }
        }
        if(userGeneratorCreated && ((asDepthEnabled && asDepthShowUserColors) || asUserMaskEnabled || asUserEnabled || asSkeletonEnabled))
        {
            dispatchInfoMessage((const uint8_t*) "Starting User Generator");
            rc = userGenerator.StartGenerating();
            if(rc != XN_STATUS_OK)
            {
                dispatchErrorMessage((const uint8_t*) "Starting User Generator Failed");
            }
        }
        //you cant have both infrared and rgb
        if(infraredGeneratorCreated && (asInfraredEnabled && !(imageGeneratorCreated && imageGenerator.IsGenerating())))
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
		
		memset(&userFrame.users[0], 0, sizeof(userFrame.users));
    
		XnUserID *aUsers = new XnUserID[maxSkeletons];
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
            
				userFrame.users[i].worldRelativeX = (depthImageBytesGenerator->getSourceWidth() - position.X) / (depthImageBytesGenerator->getSourceWidth() * 2) - .5;
				userFrame.users[i].worldRelativeY = -1 * (((depthImageBytesGenerator->getSourceHeight() - position.Y) / (depthImageBytesGenerator->getSourceHeight() * 2)) - .5);
				userFrame.users[i].worldRelativeZ = (position.Z * 7.8125) / MAX_DEPTH;
            
				//depth & rgb space are the same, as we aligned the depth & image streams
				XnPoint3D pt[1];
				pt[0] = position;
				depthGenerator.ConvertRealWorldToProjective(1, pt, pt);
            
				userFrame.users[i].rgbRelativeX = userFrame.users[i].depthRelativeX = pt[0].X / depthImageBytesGenerator->getSourceWidth();
				userFrame.users[i].rgbRelativeY = userFrame.users[i].depthRelativeY = pt[0].Y / depthImageBytesGenerator->getSourceHeight();
            
				if(rgbImageBytesGenerator->getTargetMirrored()) userFrame.users[i].rgbRelativeX = 1 - userFrame.users[i].rgbRelativeX;
				if(depthImageBytesGenerator->getTargetMirrored()) userFrame.users[i].depthRelativeX = 1 - userFrame.users[i].depthRelativeX;
            
				userFrame.users[i].rgbX = (int) (userFrame.users[i].rgbRelativeX * rgbImageBytesGenerator->getTargetWidth());
				userFrame.users[i].rgbY = (int) (userFrame.users[i].rgbRelativeY * rgbImageBytesGenerator->getTargetHeight());
				userFrame.users[i].depthX = (int) (userFrame.users[i].depthRelativeX * depthImageBytesGenerator->getTargetWidth());
				userFrame.users[i].depthY = (int) (userFrame.users[i].depthRelativeY * depthImageBytesGenerator->getTargetHeight());
            
				if (userFrame.users[i].hasSkeleton)
				{
					//in openni, left/right are in device space, rather than user space
					//that's why we switch LEFT/RIGHT here, to match MS SDK (user space)
					addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_HEAD, 0, 0);
					addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_NECK, 1, 1);
					addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_TORSO, 2, 2);
                
					addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_SHOULDER, 3, 3);
					addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_ELBOW, 4, 4);
					addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_HAND, 5, 5);
                
					addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_SHOULDER, 6, 6);
					addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_ELBOW, 7, 7);
					addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_HAND, 8, 8);
                
					addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_HIP, 9, 9);
					addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_KNEE, 10, 10);
					addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_RIGHT_FOOT, 11, 11);
                
					addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_HIP, 12, 12);
					addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_KNEE, 13, 13);
					addJointElement(userFrame.users[i], aUsers[i], XN_SKEL_LEFT_FOOT, 14, 14);
                
					//set joint orientation matrices for joints with no orientation confidence
					for(int j = 0; j < numJoints; j++)
					{
						if(userFrame.users[i].joints[j].orientationConfidence == 0.0)
						{
							//set it to the identity matrix
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M11 = 1.0;
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M12 = 0.0;
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M13 = 0.0;
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M14 = 0.0;
                        
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M21 = 0.0;
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M22 = 1.0;
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M23 = 0.0;
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M24 = 0.0;
                        
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M31 = 0.0;
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M32 = 0.0;
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M33 = 1.0;
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M34 = 0.0;
                        
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M41 = 0.0;
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M42 = 0.0;
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M43 = 0.0;
							userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix.M44 = 1.0;
						}
					}
				}
			}
			else
			{
				userFrame.users[i].isTracking = false;
			}
		}


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

void OpenNIDevice::addJointElement(kinectUser &kUser, XnUserID user, XnSkeletonJoint eJoint, uint32_t targetIndex, int targetOrientationIndex)
{
    float jointPositionX, jointPositionY, jointPositionZ, jointW, jointPositionConfidence;
    
    XnSkeletonJointPosition jointPosition;
    userGenerator.GetSkeletonCap().GetSkeletonJointPosition(user, eJoint, jointPosition);
    
    jointPositionX = jointPosition.position.X;
    jointPositionY = jointPosition.position.Y;
    jointPositionZ = jointPosition.position.Z;
    jointW = 0;
    jointPositionConfidence = jointPosition.fConfidence;
    
    //diable joint orientation on OpenNI for now
    if(false && targetOrientationIndex > -1)
    {
        XnSkeletonJointOrientation orientation;
        userGenerator.GetSkeletonCap().GetSkeletonJointOrientation(user, eJoint, orientation);
        
        kUser.joints[targetOrientationIndex].orientationConfidence = orientation.fConfidence;
        
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M11 = orientation.orientation.elements[0];
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M12 = orientation.orientation.elements[3];
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M13 = orientation.orientation.elements[6];
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M14 = 0.0;
        
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M21 = orientation.orientation.elements[1];
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M22 = orientation.orientation.elements[4];
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M23 = orientation.orientation.elements[7];
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M24 = 0.0;
        
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M31 = orientation.orientation.elements[2];
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M32 = orientation.orientation.elements[5];
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M33 = orientation.orientation.elements[8];
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M34 = 0.0;
        
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M41 = 0.0;
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M42 = 0.0;
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M43 = 0.0;
        kUser.joints[targetOrientationIndex].absoluteOrientation.rotationMatrix.M44 = 1.0;
    }
    else
    {
        kUser.joints[targetOrientationIndex].orientationConfidence = 0.0;
    }
    
    kUser.joints[targetIndex].positionConfidence = jointPositionConfidence;

    kUser.joints[targetIndex].worldX = jointPositionX;
    kUser.joints[targetIndex].worldY = jointPositionY;
    kUser.joints[targetIndex].worldZ = jointPositionZ;
    
	kUser.joints[targetIndex].worldRelativeX = (depthImageBytesGenerator->getSourceWidth() - jointPositionX) / (depthImageBytesGenerator->getSourceWidth() * 2) - .5;
	kUser.joints[targetIndex].worldRelativeY = -1 * (((depthImageBytesGenerator->getSourceHeight() - jointPositionY) / (depthImageBytesGenerator->getSourceHeight() * 2)) - .5);
    kUser.joints[targetIndex].worldRelativeZ = (jointPositionZ * 7.8125) / MAX_DEPTH;
    
    //relative position / depth & rgb are aligned
    XnPoint3D pt[1];
    pt[0] = jointPosition.position;
    depthGenerator.ConvertRealWorldToProjective(1, pt, pt);
    
	kUser.joints[targetIndex].rgbRelativeX = kUser.joints[targetIndex].depthRelativeX = pt[0].X / depthImageBytesGenerator->getSourceWidth();
	kUser.joints[targetIndex].rgbRelativeY = kUser.joints[targetIndex].depthRelativeY = pt[0].Y / depthImageBytesGenerator->getSourceHeight();
    
    //take mirrored rgb / depth images into account
    if(rgbImageBytesGenerator->getTargetMirrored()) kUser.joints[targetIndex].rgbRelativeX = 1 - kUser.joints[targetIndex].rgbRelativeX;
	if(depthImageBytesGenerator->getTargetMirrored()) kUser.joints[targetIndex].depthRelativeX = 1 - kUser.joints[targetIndex].depthRelativeX;
    
    kUser.joints[targetIndex].rgbX = (int) (kUser.joints[targetIndex].rgbRelativeX * rgbImageBytesGenerator->getTargetWidth());
    kUser.joints[targetIndex].rgbY = (int) (kUser.joints[targetIndex].rgbRelativeY * rgbImageBytesGenerator->getTargetHeight());
	kUser.joints[targetIndex].depthX = (int) (kUser.joints[targetIndex].depthRelativeX * depthImageBytesGenerator->getTargetWidth());
	kUser.joints[targetIndex].depthY = (int) (kUser.joints[targetIndex].depthRelativeY * depthImageBytesGenerator->getTargetHeight());
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
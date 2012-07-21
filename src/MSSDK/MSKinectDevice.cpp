#include "MSKinectDevice.h"
#ifdef AIRKINECT_TARGET_MSSDK

#include "AKMSSDKPointCloudGenerator.h"
#include "AKMSSDKUserMasksGenerator.h"
#include "AKMSSDKUserFrameGenerator.h"

const float PI = acos(-1.0f);

MSKinectDevice::MSKinectDevice(int nr)
{
    this->nr = nr;
    this->freContext = freContext;

	//initialize the capabilities of this device
	capabilities.hasCameraElevationSupport				= true;
	capabilities.hasDepthCameraSupport					= true;
	capabilities.hasDepthUserSupport					= true;
	capabilities.hasInfraredSupport						= false;
	capabilities.hasJointOrientationConfidenceSupport	= false;
	capabilities.hasJointOrientationSupport				= true;
	capabilities.hasMultipleSensorSupport				= true;
	capabilities.hasPointCloudRegionSupport				= true;
	capabilities.hasPointCloudSupport					= true;
	capabilities.hasPositionConfidenceSupport			= false;
	capabilities.hasRGBCameraSupport					= true;
	capabilities.hasSkeletonSupport						= true;
	capabilities.hasAdvancedJointSupport				= true;
	capabilities.hasUserMaskSupport						= true;
	capabilities.hasNearModeSupport						= true;
	capabilities.hasSeatedSkeletonSupport				= true;
	capabilities.hasChooseSkeletonsSupport				= true;

	capabilities.maxSensors								= 4;
	capabilities.framework								= "mssdk";
	
    setDefaults();
	addKinectDeviceStatusListener();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                      Device Status FUNCTIONS        
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
void MSKinectDevice::addKinectDeviceStatusListener() {
	dispatchInfoMessage((const uint8_t*) "Added Device Status");
	MSKinectDevice *pThis = NULL;
	pThis = reinterpret_cast<MSKinectDevice*>(this);
	NuiSetDeviceStatusCallback(&statusProcThunk, pThis );
}

void CALLBACK MSKinectDevice::statusProcThunk( HRESULT hrStatus, const OLECHAR* instanceName, const OLECHAR* uniqueDeviceName, void * pUserData)
{
	reinterpret_cast<MSKinectDevice *>(pUserData)->statusProc( hrStatus, instanceName, uniqueDeviceName);	
}

void CALLBACK MSKinectDevice::statusProc( HRESULT hrStatus, const OLECHAR* instanceName, const OLECHAR* uniqueDeviceName )
{
	const uint8_t* level = (const uint8_t*) "unknown";
	if (SUCCEEDED(hrStatus))
	{
		level = (const uint8_t*) "reconnected";
	}
	else
	{
		this->dispose();
		level = (const uint8_t*) "disconnected";
	}
	dispatchStatusMessage(level);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                      Default Creation Functions        
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

void MSKinectDevice::createPointCloudGenerator()
{
	pointCloudGenerator = new AKMSSDKPointCloudGenerator();
}

void MSKinectDevice::createUserMasksGenerator()
{
	userMasksGenerator = new AKMSSDKUserMasksGenerator();
}

void MSKinectDevice::createUserFrameGenerator()
{
	userFrameGenerator = new AKMSSDKUserFrameGenerator();
}

void MSKinectDevice::setDefaults()
{
	//set the defaults from the base class
	KinectDevice::setDefaults();

	rgbImageBytesGenerator->setSourceMirrored(false);
	depthImageBytesGenerator->setSourceMirrored(false);

	pointCloudGenerator->setSourceRGBMirrored(false);
	pointCloudGenerator->setSourceDepthMirrored(false);

	userMasksGenerator->setSourceRGBMirrored(false);
	userMasksGenerator->setSourceDepthMirrored(false);

	//set specific defaults for MS SDK
    imageFrameTimeout = 0;
    
	depthResolution = getNuiImageResolutionForGivenWidthAndHeight(depthImageBytesGenerator->getSourceWidth(), depthImageBytesGenerator->getSourceHeight());
	depthPlayerIndexEnabled = false;

	depthParser = new AKMSSDKDepthParser();
	rgbParser = new AKMSSDKRGBParser();

	//Player Index Coloring
	userIndexColors = 0;
	setUserColor(1, 0xff0000, 1);
	setUserColor(2, 0x00ff00, 1);
	setUserColor(3, 0x0000ff, 1);
	setUserColor(4, 0xffff00, 1);
	setUserColor(5, 0xff00ff, 1);
	setUserColor(6, 0x8080ff, 1);

	//----------------
	// Transform Smoothing for Skeleton
	//----------------
	transformSmoothingParameters.fCorrection=0.5f;
	transformSmoothingParameters.fSmoothing=0.5f;
	transformSmoothingParameters.fPrediction =0.5f;
	transformSmoothingParameters.fJitterRadius =0.05f;
	transformSmoothingParameters.fMaxDeviationRadius=0.04f;
}

//-------------------
// Start FRE Function
//-------------------

FREObject MSKinectDevice::freSetDepthMode(FREObject argv[])
{
	KinectDevice::freSetDepthMode(argv);
	lockDepthMutex();
	depthResolution = getNuiImageResolutionForGivenWidthAndHeight(depthImageBytesGenerator->getSourceWidth(), depthImageBytesGenerator->getSourceHeight());
	unlockDepthMutex();
	return NULL;
}

FREObject MSKinectDevice::freSetSkeletonMode(FREObject argv[]) 
{
	bool previousSeatedSkeletonEnabled = asSeatedSkeletonEnabled;
	bool previousChooseSkeletonsEnabled = asChooseSkeletonsEnabled;
	KinectDevice::freSetSkeletonMode(argv);
	if(running && (previousSeatedSkeletonEnabled != asSeatedSkeletonEnabled || previousChooseSkeletonsEnabled != asChooseSkeletonsEnabled)) 
	{
		lockUserMutex();
		((AKMSSDKUserFrameGenerator*) userFrameGenerator)->setSeatedSkeletonEnabled(asSeatedSkeletonEnabled);
		HRESULT hr = setSkeletonTrackingFlags();
		if (FAILED(hr))
			dispatchErrorMessage((const uint8_t*) "Failed to Initalize Skeleton Tracking");
		unlockUserMutex();
	}
	return NULL;
}

FREObject MSKinectDevice::freChooseSkeletons(FREObject argv[])
{
	KinectDevice::freChooseSkeletons(argv);
	if(started && asChooseSkeletonsEnabled) {
		nuiSensor->NuiSkeletonSetTrackedSkeletons((DWORD *) chosenSkeletonIds);
	}
	return NULL;
}

FREObject MSKinectDevice::freSetNearModeEnabled(FREObject argv[])
{
	bool previousValue = asNearModeEnabled;
	KinectDevice::freSetNearModeEnabled(argv);

	if(started && previousValue != asNearModeEnabled)
	{
		lockDepthMutex();
		if(asNearModeEnabled)
		{
			nuiSensor->NuiImageStreamSetImageFrameFlags(depthFrameHandle, NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE);
		}
		else
		{
			nuiSensor->NuiImageStreamSetImageFrameFlags(depthFrameHandle, 0);
		}
		unlockDepthMutex();
	}

    return NULL;
}

FREObject MSKinectDevice::freCameraElevationGetAngle(FREObject argv[])
{
	dispatchInfoMessage((const uint8_t*) "[MSKinectDevice] Camera Elevation Get Angle Called");
	FREObject asCameraAngle;
	long degrees = 0;
	if(running)
	{
		HRESULT hr = nuiSensor->NuiCameraElevationGetAngle(&degrees);
		if ( FAILED(hr) ){
			dispatchErrorMessage((const uint8_t*) "Failed to get sensor angle");
		}
	}
	FRENewObjectFromInt32 ((int) degrees, &asCameraAngle );
	return asCameraAngle;
}

FREObject MSKinectDevice::freCameraElevationSetAngle(FREObject argv[])
{
	dispatchInfoMessage((const uint8_t*) "[MSKinectDevice] Camera Elevation Set Angle Called");
	if(running)
	{
		int degrees; FREGetObjectAsInt32(argv[1], &degrees);
		HRESULT hr = nuiSensor->NuiCameraElevationSetAngle((long) degrees);
		if ( FAILED(hr) ){
			dispatchErrorMessage((const uint8_t*) "Failed to set sensor angle");
		}
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                      MANAGEMENT FUNCTIONS        
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
void MSKinectDevice::start()
{
	if(!running)
    {
        running = true;
        mThread = boost::thread(&MSKinectDevice::deviceThread, this);
    }
}

void MSKinectDevice::dispatchError(HRESULT hr){
	if(hr == E_NUI_DEVICE_NOT_CONNECTED){
		dispatchErrorMessage((const uint8_t*) "Device Not Connected");
	}else if(hr == E_NUI_DEVICE_NOT_READY){
		dispatchErrorMessage((const uint8_t*) "Device Not Ready");
	}else if(hr == E_NUI_ALREADY_INITIALIZED){
		dispatchErrorMessage((const uint8_t*) "Device Already Initalized");
	}else if(hr == E_NUI_NO_MORE_ITEMS){
		dispatchErrorMessage((const uint8_t*) "No More Items");
	}else if(hr == E_NUI_FRAME_NO_DATA){
		dispatchErrorMessage((const uint8_t*) "No Frame Data");
	}else if(hr == E_NUI_STREAM_NOT_ENABLED){
		dispatchErrorMessage((const uint8_t*) "Stream Not Enabled");
	}else if(hr == E_NUI_IMAGE_STREAM_IN_USE){
		dispatchErrorMessage((const uint8_t*) "Image Stream In-Use");
	}else if(hr == E_NUI_FRAME_LIMIT_EXCEEDED){
		dispatchErrorMessage((const uint8_t*) "Frame Limit Exceeded");
	}else if(hr == E_NUI_FEATURE_NOT_INITIALIZED){
		dispatchErrorMessage((const uint8_t*) "Feature Not Initialized");
	}else if(hr == E_NUI_DATABASE_NOT_FOUND){
		dispatchErrorMessage((const uint8_t*) "Database Not Found");
	}else if(hr == E_NUI_DATABASE_VERSION_MISMATCH){
		dispatchErrorMessage((const uint8_t*) "Database Version MisMatch");
	}else if(hr == E_NUI_NOTCONNECTED){
		dispatchErrorMessage((const uint8_t*) "NUI Not Connected");
	}else if(hr == E_NUI_NOTREADY){
		dispatchErrorMessage((const uint8_t*) "NUI Not Ready");
	}else if(hr == E_NUI_SKELETAL_ENGINE_BUSY){
		dispatchErrorMessage((const uint8_t*) "NUI Skeletal Engine Busy");
	}else if(hr == E_NUI_NOTPOWERED){
		dispatchErrorMessage((const uint8_t*) "NUI Not Powered");
	}else if(hr == E_NUI_BADIINDEX){
		dispatchErrorMessage((const uint8_t*) "Bad Index");
	}else{
		char errorMessage[100];
		sprintf_s(errorMessage, "0x%x", hr);
		dispatchErrorMessage((const uint8_t*) errorMessage);
		trace((const uint8_t*) errorMessage);
	}
}

void MSKinectDevice::stop()
{
	dispatchInfoMessage((const uint8_t*) "Stopping Kinect");
	if(running)
    {
        running = false;
        mThread.join();
    }

	if(nuiSensor) 
		nuiSensor->NuiShutdown( );
	
	if( userEvent && ( userEvent != INVALID_HANDLE_VALUE ) ) 
	{
		CloseHandle(userEvent );
		userEvent = 0;
		nuiSensor->NuiSkeletonTrackingDisable();
	}

	if( depthFrameEvent && ( depthFrameEvent != INVALID_HANDLE_VALUE ) ) 
	{
		CloseHandle( depthFrameEvent );
		depthFrameEvent = 0;
	}

	if( rgbFrameEvent && ( rgbFrameEvent != INVALID_HANDLE_VALUE ) ) 
	{
		CloseHandle( rgbFrameEvent );
		rgbFrameEvent = 0;
	}

    //cleanup bytearrays
	cleanupByteArrays();

	if (nuiSensor)
    {
        nuiSensor->Release();
        nuiSensor = 0;
    }
    
	bool hadStarted = started;
	//reset defaults
	setDefaults();
    if(hadStarted) 
	{
		dispatchStatusMessage((const uint8_t*) "stopped");
    }
	trace((const uint8_t*) "MSKinectDevice::stop finished");
}

void MSKinectDevice::cleanupByteArrays()
{
	KinectDevice::cleanupByteArrays();

	if(depthParser != 0)
		delete depthParser;
	depthParser = 0;

	if(rgbParser != 0)
		delete rgbParser;
	rgbParser = 0;

	if(userIndexColors != 0)
		delete [] userIndexColors;
	userIndexColors = 0;
}

void MSKinectDevice::dispose()
{
	dispatchInfoMessage((const uint8_t*) "Disposing Kinect");
    stop();
    freContext = 0;

	trace((const uint8_t*) "MSKinectDevice::dispose");
}

void * MSKinectDevice::deviceThread(void *self)
{
    MSKinectDevice* adapter = (MSKinectDevice *) self;
    adapter->run();
    return NULL;
}

void MSKinectDevice::run()
{
	trace((const uint8_t*) "MSKinectDevice::run");
    if(running)
    { 
		dispatchInfoMessage((const uint8_t*) "Starting Device");
	
		HRESULT hr = NuiCreateSensorByIndex(this->nr, &nuiSensor);
		//Failed dispatch some message
		if ( FAILED(hr) ){
			dispatchErrorMessage((const uint8_t*) "Failed to create Sensor");
			running = false;
			stop();
			return;
		}

		//try initializing the sensor
		//it might not be available for initialization right away, so try a couple of times
		bool initialized = false;
		for(int i = 0; i < 5; i++)
		{
			depthPlayerIndexEnabled = asDepthShowUserColors || asUserMaskEnabled;
			DWORD dwFlags = 0;
			if(asSkeletonEnabled || asUserMaskEnabled) 
			{
				dwFlags |= NUI_INITIALIZE_FLAG_USES_SKELETON;
			}
			if(asRGBEnabled || asPointCloudEnabled || asUserMaskEnabled)
			{
				dwFlags |= NUI_INITIALIZE_FLAG_USES_COLOR;
			}
			if(asDepthEnabled || asPointCloudEnabled || asUserMaskEnabled){
				if(depthPlayerIndexEnabled)
				{
					dwFlags |= NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX;
				}
				else
				{
					dwFlags |= NUI_INITIALIZE_FLAG_USES_DEPTH;
				}
			}

			char flagsMessage[100];
			sprintf_s(flagsMessage, "dwFlags: %d", dwFlags);
			trace((const uint8_t*) flagsMessage);
			hr = nuiSensor->NuiInitialize(dwFlags);
			if (FAILED(hr))
			{
				dispatchErrorMessage((const uint8_t*) "Failed to initialize Sensor");
				dispatchError(hr);

				nuiSensor->NuiShutdown();
				nuiSensor->Release();
				NuiCreateSensorByIndex(this->nr, &nuiSensor);

				boost::this_thread::sleep(boost::posix_time::seconds(1));
			}
			else
			{
				initialized = true;
				break;
			}
		}

		if(!initialized)
		{
			trace((const uint8_t*) "Sensor initialization failed, aborting");
			running = false;
			stop();
			return;
		}
		
		userEvent	= CreateEvent( NULL, TRUE, FALSE, NULL );
		if(asSkeletonEnabled || asUserMaskEnabled){
			dispatchInfoMessage((const uint8_t*) "Starting Skeleton Tracking");

			hr = setSkeletonTrackingFlags();
			if ( FAILED(hr) ){
				dispatchErrorMessage((const uint8_t*) "Failed to Initalize Skeleton Tracking");
				running = false;
				stop();
				return;
			}
		}

		rgbFrameEvent	= CreateEvent( NULL, TRUE, FALSE, NULL );
		if(asRGBEnabled || asPointCloudEnabled || asUserMaskEnabled) {
			dispatchInfoMessage((const uint8_t*) "Starting RGB Camera");

			hr = nuiSensor->NuiImageStreamOpen( NUI_IMAGE_TYPE_COLOR, rgbResolution, 0, 2, rgbFrameEvent, &rgbFrameHandle );
			if ( FAILED(hr) ){
				dispatchErrorMessage((const uint8_t*) "Failed to Initalize RGB Camera");
				running = false;
				stop();
				return;
			}
		}

		depthFrameEvent	= CreateEvent( NULL, TRUE, FALSE, NULL );
	
		if(asDepthEnabled || asPointCloudEnabled || asUserMaskEnabled) {
			dispatchInfoMessage((const uint8_t*) "Starting Depth Camera");
			if(depthPlayerIndexEnabled){
				hr = nuiSensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, depthResolution, 0, 2, depthFrameEvent, &depthFrameHandle );
			}else{
				hr = nuiSensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, depthResolution, 0, 2, depthFrameEvent, &depthFrameHandle );
			}

			//near mode support?
			if(asNearModeEnabled) {
				nuiSensor->NuiImageStreamSetImageFrameFlags(depthFrameHandle, NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE);
			}
		
			//Failed dispatch some message
			if ( FAILED(hr) ){
				dispatchErrorMessage((const uint8_t*) "Failed to Initalize Depth Camera");
				running = false;
				stop();
				return;
			}
		}

		if(asSkeletonEnabled || asRGBEnabled || asDepthEnabled || asPointCloudEnabled || asUserMaskEnabled){
			dispatchInfoMessage((const uint8_t*) "Starting Kinect Thread");

			const int numEvents = 3;
			HANDLE hEvents[numEvents] = { depthFrameEvent, rgbFrameEvent, userEvent };
			int    nEventIdx;

			started = true;
			dispatchStatusMessage((const uint8_t*) "started");

			while(running) {
				nEventIdx = WaitForMultipleObjects(numEvents, hEvents, FALSE, 100);

				switch(nEventIdx) {
					case WAIT_TIMEOUT:
						continue;
					case WAIT_OBJECT_0:
						readDepthFrame();
						dispatchDepthIfNeeded();
						dispatchPointCloudIfNeeded();
						dispatchUserMaskIfNeeded();
						break;
					case WAIT_OBJECT_0 +1:
						readRGBFrame();
						dispatchRGBIfNeeded();
						break;
					case WAIT_OBJECT_0 +2:
						dispatchUserFrameIfNeeded();
						break;
				}
			}

		} else {
			stop();
		}
	}
	running = false;
}

HRESULT MSKinectDevice::setSkeletonTrackingFlags()
{
	DWORD skeletonFlags = 0;
	if(asSeatedSkeletonEnabled) skeletonFlags |= NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT;
	if(asSkeletonEnabled) skeletonFlags |= NUI_SKELETON_TRACKING_FLAG_ENABLE_IN_NEAR_RANGE;
	if(asChooseSkeletonsEnabled) skeletonFlags |= NUI_SKELETON_TRACKING_FLAG_TITLE_SETS_TRACKED_SKELETONS;
	return nuiSensor->NuiSkeletonTrackingEnable( userEvent, skeletonFlags );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                      EVENT PROCESSING         
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MSKinectDevice::readRGBFrame()
{
	rgbParser->setImageSize(rgbImageBytesGenerator->getSourceWidth(), rgbImageBytesGenerator->getSourceHeight());
	rgbParser->setNuiSensor(nuiSensor);
	rgbParser->setImageFrameTimeout(imageFrameTimeout);
	rgbParser->setRGBFrameHandle(rgbFrameHandle);

	rgbParser->parseData();
}

void MSKinectDevice::readDepthFrame()
{
	depthParser->setImageSize(depthImageBytesGenerator->getSourceWidth(), depthImageBytesGenerator->getSourceHeight());
	depthParser->setNuiSensor(nuiSensor);
	depthParser->setDepthFrameTimeout(imageFrameTimeout);
	depthParser->setDepthFrameHandle(depthFrameHandle);
	depthParser->setUserIndexColors(userIndexColors);

	depthParser->parseData();
}

void MSKinectDevice::dispatchRGBIfNeeded()
{
	if(asRGBEnabled)
	{
		lockRGBMutex();
		rgbImageBytesGenerator->setSourceBytes(rgbParser->getImageByteArray());
		rgbImageBytesGenerator->generateTargetBytes();
		unlockRGBMutex();
		dispatchStatusMessage((const uint8_t*) "RGBFrame");
	}
}

void MSKinectDevice::dispatchDepthIfNeeded()
{
	if(asDepthEnabled) 
	{
		lockDepthMutex();
		depthImageBytesGenerator->setSourceBytes(depthParser->getImageByteArray());
		depthImageBytesGenerator->generateTargetBytes();
		unlockDepthMutex();
		dispatchStatusMessage((const uint8_t*) "depthFrame");
	}
}

void MSKinectDevice::dispatchPointCloudIfNeeded()
{
	if(asPointCloudEnabled)
	{
		lockPointCloudMutex();

		((AKMSSDKPointCloudGenerator*)pointCloudGenerator)->setNuiSensor(nuiSensor);

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

void MSKinectDevice::dispatchUserMaskIfNeeded()
{
	if(asUserMaskEnabled)
	{
		lockUserMaskMutex();

		((AKMSSDKUserMasksGenerator*) userMasksGenerator)->setNuiSensor(nuiSensor);
		((AKMSSDKUserMasksGenerator*) userMasksGenerator)->setSourceSceneBytes(depthParser->getSceneByteArray());
		userMasksGenerator->setSourceDepthBytes(depthParser->getDepthByteArray());
		userMasksGenerator->setSourceRGBBytes(rgbParser->getImageByteArray());
		userMasksGenerator->generateTargetBytes();

		unlockUserMaskMutex();
		dispatchStatusMessage((const uint8_t*) "userMaskFrame");
	}
}

void MSKinectDevice::dispatchUserFrameIfNeeded()
{
	if(asUserEnabled || asSkeletonEnabled || asUserMaskEnabled)
	{
		lockUserMutex();

		((AKMSSDKUserFrameGenerator*) userFrameGenerator)->setNuiSensor(nuiSensor);
		((AKMSSDKUserFrameGenerator*) userFrameGenerator)->setTransformSmoothingParameters(transformSmoothingParameters);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                      INTERNAL HELPER FUNCTIONS         
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MSKinectDevice::setUserColor(int userID, int color, bool useIntensity)
{
	if(userID > NUI_SKELETON_COUNT) return;
	if(userIndexColors == 0) userIndexColors = new BYTE[NUI_SKELETON_COUNT * 4];
	
	byte colorIndex = ((userID-1)*4);
	userIndexColors[colorIndex] = (BYTE)(color >> 16);
	userIndexColors[colorIndex+1] = (BYTE) (color >> 8);
	userIndexColors[colorIndex+2] = (BYTE) color;
	userIndexColors[colorIndex+3] = useIntensity ? 1 : 0;
}

void MSKinectDevice::setRGBMode(int rgbWidth, int rgbHeight, int asRGBWidth, int asRGBHeight, bool asRGBMirrored)
{
	//support higher resolution for MS SDK
	if(asRGBWidth == 1280 && asRGBHeight == 960)
	{
		rgbWidth = 1280;
		rgbHeight = 960;
	}
	KinectDevice::setRGBMode(rgbWidth, rgbHeight, asRGBWidth, asRGBHeight, asRGBMirrored);
	rgbResolution = getNuiImageResolutionForGivenWidthAndHeight(rgbWidth, rgbHeight);
}

#endif
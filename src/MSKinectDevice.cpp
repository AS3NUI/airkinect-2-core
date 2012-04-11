#include "MSKinectDevice.h"
#ifdef AIRKINECT_TARGET_MSSDK

MSKinectDevice::MSKinectDevice(int nr)
{
    printf("MSKinectDevice::MSKinectDevice(%i)\n", nr);
    this->nr = nr;
    this->freContext = freContext;

	//initialize the capabilities of this device
	capabilities.hasCameraElevationSupport				= true;
	capabilities.hasDepthCameraSupport					= true;
	capabilities.hasDepthUserSupport					= true;
	capabilities.hasInfraredSupport						= false;
	capabilities.hasJointOrientationConfidenceSupport	= false;
	capabilities.hasJointOrientationSupport				= false;
	capabilities.hasMultipleSensorSupport				= true;
	capabilities.hasPointCloudRegionSupport				= true;
	capabilities.hasPointCloudSupport					= true;
	capabilities.hasPositionConfidenceSupport			= false;
	capabilities.hasRGBCameraSupport					= true;
	capabilities.hasSkeletonSupport						= true;
	capabilities.hasAdvancedJointSupport				= true;
	capabilities.hasUserMaskSupport						= true;
	capabilities.hasNearModeSupport						= true;

	capabilities.maxSensors								= 4;
	capabilities.framework								= "mssdk";
    
    //set class names of actionscript equivalents
	asJointClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSSkeletonJoint";
	asUserClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSUser";
	asUserFrameClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSUserFrame";
	numJoints = 20;
	maxSkeletons = NUI_SKELETON_COUNT;
	jointNames = new char*[numJoints];
	jointNames[0] = "waist";
	jointNames[1] = "torso";
	jointNames[2] = "neck";
	jointNames[3] = "head";
    
	jointNames[4] = "left_shoulder";
	jointNames[5] = "left_elbow";
	jointNames[6] = "left_wrist";
	jointNames[7] = "left_hand";
    
	jointNames[8] = "right_shoulder";
	jointNames[9] = "right_elbow";
	jointNames[10] = "right_wrist";
	jointNames[11] = "right_hand";
    
	jointNames[12] = "left_hip";
	jointNames[13] = "left_knee";
	jointNames[14] = "left_ankle";
	jointNames[15] = "left_foot";
    
	jointNames[16] = "right_hip";
	jointNames[17] = "right_knee";
	jointNames[18] = "right_ankle";
	jointNames[19] = "right_foot";

	//set default values
    setDefaults();
	addKinectDeviceStatusListener();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                      Device Status FUNCTIONS        
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
void MSKinectDevice::addKinectDeviceStatusListener() {
	FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Added Device Status");
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
	const uint8_t* statusCode = (const uint8_t*) "status";
	const uint8_t* level = (const uint8_t*) "unknown";
	
	if (SUCCEEDED(hrStatus)) {
		level = (const uint8_t*) "reconnected";
	}else{
		this->dispose();
		level = (const uint8_t*) "disconnected";
	}
	FREDispatchStatusEventAsync(freContext, statusCode, level);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                      Default Creation Functions        
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
void MSKinectDevice::setDefaults()
{
	//set the defaults from the base class
	KinectDevice::setDefaults();

	//set specific defaults for MS SDK
    imageFrameTimeout = 0;
    
	//----------------
	// Depth Image
	//----------------
	asDepthResolution = getResolutionFrom(asDepthWidth, asDepthHeight);
	depthResolution = getResolutionFrom(depthWidth, depthHeight);
	depthImageByteArray = 0;
	depthByteArray = 0;
	sceneByteArray = 0;
	depthPlayerIndexEnabled = false;
    
	//----------------
	// RGB Image
	//----------------
	asRGBResolution = getResolutionFrom(asRGBWidth, asRGBHeight);
	rgbResolution = getResolutionFrom(rgbWidth, rgbHeight);
    rgbImageByteArray = 0;
    
	//----------------
	// User Masked Image
	//----------------

	asUserMaskResolution = getResolutionFrom(asUserMaskWidth, asUserMaskHeight);
    
	//----------------
	// Point Cloud Data
	//----------------

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

// ----------------------- USER MASK ------------------------

FREObject MSKinectDevice::freSetUserMaskMode(FREObject argv[]) {
	KinectDevice::freSetUserMaskMode(argv);
	asUserMaskResolution = getResolutionFrom(asUserMaskWidth, asUserMaskHeight);
    return NULL;
}

// ----------------------- DEPTH IMAGE ------------------------

FREObject MSKinectDevice::freSetDepthMode(FREObject argv[]) {
	KinectDevice::freSetDepthMode(argv);
	asDepthResolution = getResolutionFrom(asDepthWidth, asDepthHeight);
    return NULL;
}

// ----------------------- RGB IMAGE ------------------------

FREObject MSKinectDevice::freSetRGBMode(FREObject argv[]) {
    KinectDevice::freSetRGBMode(argv);
	asRGBResolution = getResolutionFrom(asRGBWidth, asRGBHeight);
    return NULL;
}

FREObject MSKinectDevice::freCameraElevationGetAngle(FREObject argv[])
{
	FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "[MSKinectDevice] Camera Elevation Get Angle Called");
	FREObject asCameraAngle;
	long degrees;
	HRESULT hr = nuiSensor->NuiCameraElevationGetAngle(&degrees);
	if ( FAILED(hr) ){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Failed to get sensor angle");
	}
	FRENewObjectFromInt32 ((int) degrees, &asCameraAngle );
	return asCameraAngle;
}

FREObject MSKinectDevice::freCameraElevationSetAngle(FREObject argv[])
{
	FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "[MSKinectDevice] Camera Elevation Set Angle Called");
	int degrees; FREGetObjectAsInt32(argv[1], &degrees);
	HRESULT hr = nuiSensor->NuiCameraElevationSetAngle((long) degrees);
	if ( FAILED(hr) ){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Failed to set sensor angle");
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
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Device Not Connected");
	}else if(hr == E_NUI_DEVICE_NOT_READY){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Device Not Ready");
	}else if(hr == E_NUI_ALREADY_INITIALIZED){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Device Already Initalized");
	}else if(hr == E_NUI_NO_MORE_ITEMS){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "No More Items");
	}else if(hr == E_NUI_FRAME_NO_DATA){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "No Frame Data");
	}else if(hr == E_NUI_STREAM_NOT_ENABLED){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Stream Not Enabled");
	}else if(hr == E_NUI_IMAGE_STREAM_IN_USE){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Image Stream In-Use");
	}else if(hr == E_NUI_FRAME_LIMIT_EXCEEDED){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Frame Limit Exceeded");
	}else if(hr == E_NUI_FEATURE_NOT_INITIALIZED){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Feature Not Initialized");
	}else if(hr == E_NUI_DATABASE_NOT_FOUND){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Database Not Found");
	}else if(hr == E_NUI_DATABASE_VERSION_MISMATCH){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Database Version MisMatch");
	}else if(hr == E_NUI_NOTCONNECTED){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "NUI Not Connected");
	}else if(hr == E_NUI_NOTREADY){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "NUI Not Ready");
	}else if(hr == E_NUI_SKELETAL_ENGINE_BUSY){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "NUI Skeltal Engine Busy");
	}else if(hr == E_NUI_NOTPOWERED){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "NUI Not Powered");
	}else if(hr == E_NUI_BADIINDEX){
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Bad Index");
	}else{
		char errorMessage[100];
		sprintf_s(errorMessage, "0x%x", hr);
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) errorMessage);
	}
}

void MSKinectDevice::stop()
{
	FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Stopping Kinect");
	if(running)
    {
        running = false;
        mThread.join();
    }

	if(nuiSensor) nuiSensor->NuiShutdown( );
	
	if( userEvent && ( userEvent != INVALID_HANDLE_VALUE ) ) 
	{
		CloseHandle(userEvent );
		userEvent = NULL;
		nuiSensor->NuiSkeletonTrackingDisable();
	}

	if( depthFrameEvent && ( depthFrameEvent != INVALID_HANDLE_VALUE ) ) 
	{
		CloseHandle( depthFrameEvent );
		depthFrameEvent = NULL;
	}

	if( rgbFrameEvent && ( rgbFrameEvent != INVALID_HANDLE_VALUE ) ) 
	{
		CloseHandle( rgbFrameEvent );
		rgbFrameEvent = NULL;
	}

    //cleanup bytearrays
    if(depthImageByteArray != 0) delete [] depthImageByteArray;
	if(depthByteArray != 0) delete [] depthByteArray;
	if(sceneByteArray != 0) delete [] sceneByteArray;
	if(asDepthByteArray != 0) delete [] asDepthByteArray;

    if(rgbImageByteArray != 0) delete [] rgbImageByteArray;
	if(asRGBByteArray != 0) delete [] asRGBByteArray;
	
    if(asPointCloudByteArray != 0) delete [] asPointCloudByteArray;
	if(userIndexColors != 0) delete [] userIndexColors;

	if(asUserMaskByteArray != 0)
    {
		for(int i = 0; i < NUI_SKELETON_COUNT; i++)
        {
            delete [] asUserMaskByteArray[i];
        }
        delete [] asUserMaskByteArray;
    }
	
    if(pointCloudRegions != 0)
    {
        delete [] pointCloudRegions;
    }

	if (nuiSensor)
    {
        nuiSensor->Release();
        nuiSensor = NULL;
    }

    
	bool hadStarted = started;
	//reset defaults
	setDefaults();
    if(hadStarted) {
        //send stopped event
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "stopped");
    }
	
}

void MSKinectDevice::dispose()
{
	FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Disposing Kinect");
    printf("MSKinectDevice::dispose()\n");
    //make sure threads are stopped
    stop();

    //the context of this instance will be destroyed, cleanup everything of this instance
    freContext = NULL;
}

void * MSKinectDevice::deviceThread(void *self)
{
    MSKinectDevice *adapter = (MSKinectDevice *) self;
    adapter->run();
    return NULL;
}

void MSKinectDevice::run()
{
    if(running)
    { 
		FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Starting Device");
	
		HRESULT hr = NuiCreateSensorByIndex(this->nr, &nuiSensor);
		//Failed dispatch some message
		if ( FAILED(hr) ){
			FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Failed to create Sensor");
			stop();
			return;
		}

		depthPlayerIndexEnabled = asDepthShowUserColors || asUserMaskEnabled;
		DWORD dwFlags;
		if(asSkeletonEnabled || asUserMaskEnabled) dwFlags |= NUI_INITIALIZE_FLAG_USES_SKELETON;
		if(asRGBEnabled || asPointCloudEnabled || asUserMaskEnabled) dwFlags |= NUI_INITIALIZE_FLAG_USES_COLOR;
		if(asDepthEnabled || asPointCloudEnabled || asUserMaskEnabled){
			if(depthPlayerIndexEnabled) {
				dwFlags |= NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX;
			}else{
				dwFlags |= NUI_INITIALIZE_FLAG_USES_DEPTH;
			}
		}

		hr = nuiSensor->NuiInitialize(dwFlags);	
		if (FAILED(hr)) {
			FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Failed to initialize Sensor");
			dispatchError(hr);
			this->dispose();
			stop();
			return;
		}

		userEvent	= CreateEvent( NULL, TRUE, FALSE, NULL );
		if(asSkeletonEnabled || asUserMaskEnabled){
			FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Starting Skeleton Tracking");
			hr = nuiSensor->NuiSkeletonTrackingEnable( userEvent, 0 );
			//Failed dispatch some message
			if ( FAILED(hr) ){
				FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Failed to Initalize Skeleton Tracking");
				stop();
				return;
			}
		}

		rgbFrameEvent	= CreateEvent( NULL, TRUE, FALSE, NULL );
		if(asRGBEnabled || asPointCloudEnabled || asUserMaskEnabled) {
			FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Starting RGB Camera");
			hr = nuiSensor->NuiImageStreamOpen( NUI_IMAGE_TYPE_COLOR, rgbResolution, 0, 2, rgbFrameEvent, &rgbFrameHandle );
			//Failed dispatch some message
			if ( FAILED(hr) ){
				FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Failed to Initalize RGB Camera");
				stop();
				return;
			}
		}

		depthFrameEvent	= CreateEvent( NULL, TRUE, FALSE, NULL );
	
		if(asDepthEnabled || asPointCloudEnabled || asUserMaskEnabled) {
			FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Starting Depth Camera");
			if(depthPlayerIndexEnabled){
				hr = nuiSensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, depthResolution, 0, 2, depthFrameEvent, &depthFrameHandle );
			}else{
				hr = nuiSensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, depthResolution, 0, 2, depthFrameEvent, &depthFrameHandle );
			}

			//near mode support?
			if(asDepthEnableNearMode) {
				nuiSensor->NuiImageStreamSetImageFrameFlags(depthFrameHandle, NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE | NUI_IMAGE_STREAM_FLAG_DISTINCT_OVERFLOW_DEPTH_VALUES);
			}
		
			//Failed dispatch some message
			if ( FAILED(hr) ){
				FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", (const uint8_t*) "Failed to Initalize Depth Camera");
				stop();
				return;
			}
		}

		//Update all the scale information for all the features
		updateConfigScale();

		if(asSkeletonEnabled || asRGBEnabled || asDepthEnabled || asPointCloudEnabled || asUserMaskEnabled){
			FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", (const uint8_t*) "Starting Kinect Thread");

			const int numEvents = 3;
			HANDLE hEvents[numEvents] = { depthFrameEvent, rgbFrameEvent, userEvent };
			int    nEventIdx;

			started = true;
			FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "started");

			while(running) {
				nEventIdx = WaitForMultipleObjects(numEvents, hEvents, FALSE, 100);

				switch(nEventIdx) {
					case WAIT_TIMEOUT:
						continue;
					case WAIT_OBJECT_0:

						readDepthFrame();

						if(asDepthEnabled) {
							lockDepthMutex();
							depthFrameHandler();
							unlockDepthMutex();
							FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "depthFrame");
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

						//user mask
						if(asUserMaskEnabled)
						{
							lockUserMaskMutex();
							userMaskHandler();
							unlockUserMaskMutex();
							FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "userMaskFrame");
						}

						break;
					case WAIT_OBJECT_0 +1:
						readRGBFrame();

						lockRGBMutex();
						rgbFrameHandler();
						unlockRGBMutex();
						FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "RGBFrame");
						break;
					case WAIT_OBJECT_0 +2:
						lockUserMutex();
						userFrameHandler();
						unlockUserMutex();
						FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "userFrame");
						break;
				}
			}

		} else {
			stop();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                      EVENT PROCESSING         
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////
//
//  Process Kinect events	
// 	
/// 

/*
DWORD WINAPI MSKinectDevice::processThread(LPVOID pParam)
{
    MSKinectDevice *pthis = (MSKinectDevice *) pParam;
    return pthis->processThread();
}

DWORD WINAPI MSKinectDevice::processThread() {
	const int numEvents = 4;
    HANDLE hEvents[numEvents] = { nuiProcessStop, depthFrameEvent, rgbFrameEvent, userEvent };
    int    nEventIdx;

	FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "started");
    
	bool continueProcessing = true;
	bool depthReceived = false;
	bool rgbReceived = false;
	bool userReceived = false;
	while(continueProcessing) {
		nEventIdx = WaitForMultipleObjects(numEvents, hEvents, FALSE, 100);

		switch(nEventIdx) {
			case WAIT_TIMEOUT:
                continue;
			case WAIT_OBJECT_0:
				continueProcessing = false;
				continue;
			case WAIT_OBJECT_0 +1:

				readDepthFrame();

				if(asDepthEnabled) {
					lockDepthMutex();
					depthFrameHandler();
					unlockDepthMutex();
					FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "depthFrame");
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

				//user mask
				if(asUserMaskEnabled)
				{
					lockUserMaskMutex();
					userMaskHandler();
					unlockUserMaskMutex();
					FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "userMaskFrame");
				}

				break;
			case WAIT_OBJECT_0 +2:
				readRGBFrame();

				lockRGBMutex();
				rgbFrameHandler();
				unlockRGBMutex();
				FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "RGBFrame");
				break;
			case WAIT_OBJECT_0 +3:
				lockUserMutex();
				userFrameHandler();
				unlockUserMutex();
				FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", (const uint8_t*) "userFrame");
				break;
		}
	}

	return (0);
}
*/

void MSKinectDevice::readRGBFrame()
{
	//Init RGB Byte Array
	if(rgbImageByteArray == 0) rgbImageByteArray = new uint32_t[rgbPixelCount];
	uint32_t *rgbrun = rgbImageByteArray;
	//get image stream
	NUI_IMAGE_FRAME rgbImageFrame;
	HRESULT hr = nuiSensor->NuiImageStreamGetNextFrame(rgbFrameHandle, imageFrameTimeout, &rgbImageFrame );
	if (FAILED(hr)) return;
	//get texture
	INuiFrameTexture * pTexture = rgbImageFrame.pFrameTexture;
	NUI_LOCKED_RECT LockedRect;
	pTexture->LockRect( 0, &LockedRect, NULL, 0 );
	if( LockedRect.Pitch != 0 ) {
		//put the camera feed bgr data into the rgbImageByteArray
		int numRGBBytes = rgbPixelCount *4;
		for( int x = 0 ; x < numRGBBytes ; x+=4 ) {
			* rgbrun = 0xff << 24 | LockedRect.pBits[x+2] << 16 | LockedRect.pBits[x+1] << 8 | LockedRect.pBits[x];
			rgbrun++;
		}
		//release the frame
		nuiSensor->NuiImageStreamReleaseFrame( rgbFrameHandle, &rgbImageFrame );
	}
}

void MSKinectDevice::readDepthFrame()
{
	NUI_IMAGE_FRAME depthImageFrame;
	HRESULT hr = nuiSensor->NuiImageStreamGetNextFrame(depthFrameHandle, imageFrameTimeout, &depthImageFrame );
	if(FAILED(hr)) return;

	INuiFrameTexture * pTexture = depthImageFrame.pFrameTexture;
	NUI_LOCKED_RECT LockedRect;
	pTexture->LockRect( 0, &LockedRect, NULL, 0 );

	if( LockedRect.Pitch != 0 ) {
		//put the depth image camera feed into the depthImageByteArray
		if(depthImageByteArray == 0) depthImageByteArray = new uint32_t[depthPixelCount];
		uint32_t *depthImageRun = depthImageByteArray;

		//put the depth info into the depthByteArray
		if(depthByteArray == 0) depthByteArray = new USHORT[depthPixelCount];
		USHORT *depthRun = depthByteArray;

		//put user/scene info into the sceneByteArray
		if(sceneByteArray == 0) sceneByteArray = new USHORT[depthPixelCount];
		USHORT *sceneRun = sceneByteArray;

		USHORT depth;
		RGBQUAD quad;
		int numDepthBytes = depthPixelCount *2;
		for( int x = 0 ; x < numDepthBytes ; x+=2 ) {
			//depth info
			depth = (USHORT) (LockedRect.pBits[x] | (LockedRect.pBits[x+1] << 8));
			*depthRun = depth;
			depthRun++;
			//scene info
			*sceneRun = (USHORT) LockedRect.pBits[x] & 7;
			sceneRun++;
			//depth image
			quad = ShortToQuad_Depth(depth, depthPlayerIndexEnabled);
			* depthImageRun = 0xff << 24 | quad.rgbReserved << 24 | quad.rgbRed <<16 | quad.rgbGreen << 8| quad.rgbBlue;
			depthImageRun++;
		}
		//release the frame
		nuiSensor->NuiImageStreamReleaseFrame( depthFrameHandle, &depthImageFrame );
	}
}

void MSKinectDevice::rgbFrameHandler()
{
	if(rgbImageByteArray != 0) {
		uint32_t *asRGBrun = asRGBByteArray;

		int direction = asRGBMirrored ? 1 : -1;
		int directionFactor = (direction == -1) ? 1 : 0;
		int scaledWidth = rgbWidth * rgbScale;
    
		for(int y = 0; y < asRGBHeight; y++)
		{
			const uint32_t *pRGBBuffer = rgbImageByteArray + ((y + directionFactor) * scaledWidth) - directionFactor;
        
			for(int x = 0; x < asRGBWidth; x++)
			{
				*asRGBrun = *pRGBBuffer;
				asRGBrun++;
				pRGBBuffer += (rgbScale * direction);
			}
		}
	}
}

void MSKinectDevice::depthFrameHandler()
{
	if(depthImageByteArray != 0 ) {
		uint32_t *asDepthrun = asDepthByteArray;

		int direction = asDepthMirrored ? 1 : -1;
		int directionFactor = (direction == -1) ? 1 : 0;
		int scaledWidth = depthWidth * depthScale;
    
		for(int y = 0; y < asDepthHeight; y++)
		{
			const uint32_t *pDepthBuffer = depthImageByteArray + ((y + directionFactor) * scaledWidth) - directionFactor;
        
			for(int x = 0; x < asDepthWidth; x++)
			{
				*asDepthrun = *pDepthBuffer;
				asDepthrun++;
				pDepthBuffer += (depthScale * direction);
			}
		}
	}
}

void MSKinectDevice::pointCloudWithRGBHandler()
{
	if(depthByteArray != 0 && rgbImageByteArray != 0)
	{
		short *pointCloudRun = asPointCloudByteArray;
		int direction = asPointCloudMirrored ? 1 : -1;
		int directionFactor = (direction == -1) ? 1 : 0;
    
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
    
		uint32_t rgbValue;
		unsigned int rgbIndex;
		long depthX, depthY, rgbX, rgbY;
		USHORT packedDepth, realDepth;
		for(int y = 0; y < asPointCloudHeight; y+=asPointCloudDensity)
		{
			depthY = y * pointCloudScale;

			const USHORT *pDepthBuffer = depthByteArray + ((y + directionFactor) * (depthWidth * pointCloudScale)) - directionFactor;
			
			for(int x = 0; x < asPointCloudWidth; x+=asPointCloudDensity)
			{
				packedDepth = *pDepthBuffer;
				realDepth = NuiDepthPixelToDepth(packedDepth);

				//write to point cloud
				*pointCloudRun = x;
				pointCloudRun++;
				*pointCloudRun = y;
				pointCloudRun++;
				*pointCloudRun = realDepth;
				pointCloudRun++;

				//calculate mirrored x
				depthX = (direction * x * pointCloudScale) + (directionFactor * depthWidth);

				nuiSensor->NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(
					rgbResolution,
					depthResolution,
					0,
					depthX, 
					depthY, 
					packedDepth,
					&rgbX,
					&rgbY
				);
				
				rgbIndex = (rgbX + (rgbY * rgbWidth));

				if(rgbIndex > rgbPixelCount)
				{
					* pointCloudRun = 0;
					pointCloudRun++;
					* pointCloudRun = 0;
					pointCloudRun++;
					* pointCloudRun = 0;
					pointCloudRun++;
				}
				else
				{
					rgbValue = rgbImageByteArray[rgbIndex];
					* pointCloudRun = (USHORT) (0xff & (rgbValue >> 16));
					pointCloudRun++;
					* pointCloudRun = (USHORT) (0xff & (rgbValue >> 8));
					pointCloudRun++;
					* pointCloudRun = (USHORT) (0xff & rgbValue);
					pointCloudRun++;
				}
            
				//check regions
				for(unsigned int i = 0; i < numRegions; i++)
				{
					if(
					   x >= pointCloudRegions[i].x && x <= pointCloudRegions[i].maxX &&
					   y >= pointCloudRegions[i].y && y <= pointCloudRegions[i].maxY &&
					   realDepth >= pointCloudRegions[i].z && realDepth <= pointCloudRegions[i].maxZ
					   )
					{
						pointCloudRegions[i].numPoints++;
					}
				}

				pDepthBuffer += (pointCloudScale * direction * asPointCloudDensity);
			}
		}
	}
}

void MSKinectDevice::pointCloudHandler()
{
	if(depthByteArray != 0 && rgbImageByteArray != 0)
	{
		short *pointCloudRun = asPointCloudByteArray;
		int direction = asPointCloudMirrored ? 1 : -1;
		int directionFactor = (direction == -1) ? 1 : 0;
    
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

		USHORT realDepth;
		for(int y = 0; y < asPointCloudHeight; y+=asPointCloudDensity)
		{
			const USHORT *pDepthBuffer = depthByteArray + ((y + directionFactor) * (depthWidth * pointCloudScale)) - directionFactor;
        
			for(int x = 0; x < asPointCloudWidth; x+=asPointCloudDensity)
			{
				realDepth = NuiDepthPixelToDepth(*pDepthBuffer);
				//write to point cloud
				*pointCloudRun = x;
				pointCloudRun++;
				*pointCloudRun = y;
				pointCloudRun++;
				*pointCloudRun = realDepth;
				pointCloudRun++;
            
				//check regions
				for(unsigned int i = 0; i < numRegions; i++)
				{
					if(
					   x >= pointCloudRegions[i].x && x <= pointCloudRegions[i].maxX &&
					   y >= pointCloudRegions[i].y && y <= pointCloudRegions[i].maxY &&
					   realDepth >= pointCloudRegions[i].z && realDepth <= pointCloudRegions[i].maxZ
					   )
					{
						pointCloudRegions[i].numPoints++;
					}
				}

				pDepthBuffer += (pointCloudScale * direction * asPointCloudDensity);
			}
		}
	}
}

void MSKinectDevice::userMaskHandler()
{
	if(depthByteArray != 0 && rgbImageByteArray != 0)
	{
		int direction = asUserMaskMirrored ? 1 : -1;
		int directionFactor = (direction == -1) ? 1 : 0;
    
		int pixelNr = 0;
		uint32_t rgbValue;
		unsigned int rgbIndex;
		long depthX, depthY, rgbX, rgbY;
		USHORT packedDepth;
		for(int y = 0; y < asUserMaskHeight; y++)
		{
			depthY = y * userMaskScale;

			const USHORT *pDepthBuffer = depthByteArray + ((y + directionFactor) * (depthWidth * userMaskScale)) - directionFactor;
			const uint32_t *pRGBBuffer = rgbImageByteArray + ((y + directionFactor) * (rgbWidth * userMaskScale)) - directionFactor;
			const USHORT *pSceneBuffer = sceneByteArray + ((y + directionFactor) * (depthWidth * userMaskScale)) - directionFactor;
			for(int x = 0; x < asUserMaskWidth; x++)
			{
				for(int i = 0; i < maxSkeletons; i++)
				{
					asUserMaskByteArray[i][pixelNr] = 0;
				}
				USHORT userIndex = *pSceneBuffer;
				if(userIndex > 0)
				{
					//calculate mirrored x
					depthX = (direction * x * userMaskScale) + (directionFactor * depthWidth);

					packedDepth = *pDepthBuffer;
					nuiSensor->NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(
						rgbResolution,
						depthResolution,
						0,
						depthX, 
						depthY, 
						packedDepth,
						&rgbX,
						&rgbY
					);
				
					rgbIndex = (rgbX + (rgbY * rgbWidth));
					rgbValue = 0;

					if(rgbIndex > rgbPixelCount)
					{
						rgbValue = 0;
					}
					else
					{
						rgbValue = rgbImageByteArray[rgbIndex];
					}

					asUserMaskByteArray[userIndex - 1][pixelNr] = 0xff << 24 | rgbValue;
				}
				pRGBBuffer += (userMaskScale * direction);
				pDepthBuffer += (userMaskScale * direction);
				pSceneBuffer += (userMaskScale * direction);
				pixelNr++;
			}
		}
	}
}

void MSKinectDevice::userFrameHandler()
{
	//clear the current users
    memset(&userFrame.users[0], 0, sizeof(userFrame.users));

    //OutputDebugString( "AIRKinect Adapter :: onuserFrame\n" );
	NUI_SKELETON_FRAME skeletonFrame = {0};
	HRESULT hr = nuiSensor->NuiSkeletonGetNextFrame( 0, &skeletonFrame );
	if(FAILED(hr)) return;

	nuiSensor->NuiTransformSmooth(&skeletonFrame, &transformSmoothingParameters);

	NUI_SKELETON_DATA skeletonData;
	userFrame.frameNumber = skeletonFrame.dwFrameNumber;
	userFrame.timeStamp = (int)(skeletonFrame.liTimeStamp.QuadPart /1000);

	for (int i = 0; i < NUI_SKELETON_COUNT; ++i){
		skeletonData = skeletonFrame.SkeletonData[i];
		if(skeletonData.eTrackingState == NUI_SKELETON_TRACKED || skeletonData.eTrackingState == NUI_SKELETON_POSITION_ONLY) {
			userFrame.users[i].isTracking = true;
			//dwTrackingID is some wierd ass number by microsoft, to match properly to depth player index use i+1
			userFrame.users[i].trackingID = skeletonData.dwTrackingID;
			userFrame.users[i].userID = i+1;
			userFrame.users[i].hasSkeleton = skeletonData.eTrackingState == NUI_SKELETON_TRACKED;
			
			//Transform for User
			calculateKinectTransform(userFrame.users[i], skeletonData.Position);

			//Joint Position Calculations
			if (userFrame.users[i].hasSkeleton){
				addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_HIP_CENTER, 0);
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_SPINE, 1);
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_SHOULDER_CENTER, 2);
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_HEAD, 3);
                
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_SHOULDER_LEFT, 4);
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_ELBOW_LEFT, 5);
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_WRIST_LEFT, 6);
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_HAND_LEFT, 7);
                
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_SHOULDER_RIGHT, 8);
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_ELBOW_RIGHT, 9);
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_WRIST_RIGHT, 10);
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_HAND_RIGHT, 11);
                
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_HIP_LEFT, 12);
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_KNEE_LEFT, 13);
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_ANKLE_LEFT, 14);
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_FOOT_LEFT, 15);
                
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_HIP_RIGHT, 16);
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_KNEE_RIGHT, 17);
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_ANKLE_RIGHT, 18);
                addJointElement(userFrame.users[i], skeletonData, NUI_SKELETON_POSITION_FOOT_RIGHT, 19);
			}
		}else{
			userFrame.users[i].isTracking = false;
		}
	}

	previousSkeletonFrame = &skeletonFrame;
}

void MSKinectDevice::calculateKinectTransform(kinectTransform &kTransform, Vector4 skeletonTransform){
	long colorX, colorY, depthX, depthY;
	USHORT depthValue;

	Vector4 mSkeletonPosition = skeletonTransform;
	skeletonTransform.x *= -1;

	kTransform.worldX = asSkeletonMirrored ? mSkeletonPosition.x * 1000 : skeletonTransform.x * 1000;
	kTransform.worldY = mSkeletonPosition.y * 1000;
	kTransform.worldZ = mSkeletonPosition.z * 1000;

	kTransform.worldRelativeX = asSkeletonMirrored ? mSkeletonPosition.x : skeletonTransform.x;
    kTransform.worldRelativeY = mSkeletonPosition.y;
    kTransform.worldRelativeZ = mSkeletonPosition.z;

	//User to Pixel Positions on Images
	if(asDepthMirrored){
		NuiTransformSkeletonToDepthImage(mSkeletonPosition, &depthX, &depthY, &depthValue,asDepthResolution);
	}else{
		NuiTransformSkeletonToDepthImage(skeletonTransform, &depthX, &depthY, &depthValue,asDepthResolution);
	}
	kTransform.depthX = depthX;
	kTransform.depthY = depthY;

	kTransform.depthRelativeX = ((float)depthX)/((float)asDepthWidth);
	kTransform.depthRelativeY = ((float)depthY)/((float)asDepthHeight);

	if(asRGBMirrored){
		NuiTransformSkeletonToDepthImage(mSkeletonPosition, &depthX, &depthY, &depthValue,asDepthResolution);
	}else{
		NuiTransformSkeletonToDepthImage(skeletonTransform, &depthX, &depthY, &depthValue,asDepthResolution);
	}
	
	nuiSensor->NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(asRGBResolution, asDepthResolution, 0, depthX,depthY, depthValue,&colorX,&colorY);
	kTransform.rgbX = (int)colorX;
	kTransform.rgbY = (int)colorY;

	kTransform.rgbRelativeX = ((float)colorX)/((float)asRGBWidth);
	kTransform.rgbRelativeY = ((float)colorY)/((float)asRGBHeight);
}


void MSKinectDevice::addJointElement(kinectUser &kUser, NUI_SKELETON_DATA user, NUI_SKELETON_POSITION_INDEX eJoint, uint32_t targetIndex)
{
    float jointPositionConfidence;
    
	Vector4 jointPosition = user.SkeletonPositions[eJoint];
    
	//Unknown in MSSDK Will need to be calculated
	//Vector4 orientation;
   
	//Unknown in MSSDK Will need to be calculated
    jointPositionConfidence = 1;
    //Unknown in MSSDK Will need to be calculated
    kUser.joints[targetIndex].orientationConfidence = 0;

    //Unknown in MSSDK Will need to be calculated
    kUser.joints[targetIndex].orientationX = 0;
    kUser.joints[targetIndex].orientationY = 0;
    kUser.joints[targetIndex].orientationZ = 0;
    
	//Unknown in MSSDK Will need to be calculated
	//MSKinect SDK Does not have confidence values. 
	//Look into use dwQualityFlag to determine if joint if off any part of the screen.
	//possibly new value for this
    kUser.joints[targetIndex].positionConfidence = 1;

	//Transform for User
	calculateKinectTransform(kUser.joints[targetIndex], jointPosition);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                      TRANSLATION FUNCTIONS
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

NUI_IMAGE_RESOLUTION MSKinectDevice::getResolutionFrom(int width, int height){
	NUI_IMAGE_RESOLUTION rtnRes = NUI_IMAGE_RESOLUTION_320x240;
	if(width == 80 && height == 60) rtnRes = NUI_IMAGE_RESOLUTION_80x60;
	if(width == 640 && height == 480) rtnRes = NUI_IMAGE_RESOLUTION_640x480;
	if(width == 1280 && height == 960) rtnRes = NUI_IMAGE_RESOLUTION_1280x960;
	return rtnRes;
}

POINT MSKinectDevice::getDepthPixelPointFromJointCoordinate(Vector4 jointCoordinates){
	POINT depthPoint;
	float depthX, depthY;
	NuiTransformSkeletonToDepthImage(jointCoordinates, &depthX, &depthY);
	depthPoint.x = (int) depthX;
	depthPoint.y = (int) depthY;
	return depthPoint;
}

RGBQUAD MSKinectDevice::ShortToQuad_Depth( USHORT s, BOOLEAN usePlayer ) {
	RGBQUAD color;
	USHORT realDepth = NuiDepthPixelToDepth(s);
	USHORT playerIndex = NuiDepthPixelToPlayerIndex(s);

	if(playerIndex == 0){
		color.rgbRed = color.rgbGreen = color.rgbBlue = realDepth >> 4;
	}else{
		float depthRatio = (float)(realDepth-DEPTH_MIN) / (float)(DEPTH_RANGE);
		depthRatio = abs(1 - depthRatio);
		USHORT colorIndex = (playerIndex-1) * 4;
		bool useIntensity = (USHORT) userIndexColors[colorIndex+3] != 0;

		color.rgbRed  = useIntensity ? (USHORT)(depthRatio * userIndexColors[colorIndex]) :userIndexColors[colorIndex];
		color.rgbGreen = useIntensity ? (USHORT)(depthRatio * userIndexColors[colorIndex+1]) : userIndexColors[colorIndex+1];
		color.rgbBlue = useIntensity ? (USHORT)(depthRatio * userIndexColors[colorIndex+2]) : userIndexColors[colorIndex+2];
	}

	return color;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                      INTERNAL HELPER FUNCTIONS         
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MSKinectDevice::setUserColor(int userID, int color, bool useIntensity){
	//bitches!
	if(userID > NUI_SKELETON_COUNT) return;
	if(userIndexColors == 0) userIndexColors = new BYTE[NUI_SKELETON_COUNT * 4];
	
	byte colorIndex = ((userID-1)*4);
	userIndexColors[colorIndex] = (BYTE)(color >> 16);
	userIndexColors[colorIndex+1] = (BYTE) (color >> 8);
	userIndexColors[colorIndex+2] = (BYTE) color;
	userIndexColors[colorIndex+3] = useIntensity ? 1 : 0;
}

void MSKinectDevice::updateConfigScale(){
	rgbScale = rgbWidth / asRGBWidth;
	depthScale = depthWidth / asDepthWidth;
	userMaskScale = depthWidth / asUserMaskWidth;
	pointCloudScale = depthWidth / asPointCloudWidth;
}

#endif
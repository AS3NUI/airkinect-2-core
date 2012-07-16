#include "MSKinectDevice.h"
#ifdef AIRKINECT_TARGET_MSSDK

#include "AKMSSDKPointCloudGenerator.h"
#include "AKMSSDKUserMasksGenerator.h"

const float PI = acos(-1.0f);

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
    
    //set class names of actionscript equivalents
	asJointClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSSkeletonJoint";
	asUserClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSUser";
	asUserFrameClass = "com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSUserFrame";
	
	maxSkeletons = NUI_SKELETON_COUNT;
	
    setDefaults();
	addKinectDeviceStatusListener();
}

void MSKinectDevice::setNumJointsAndJointNames()
{
	if(asSeatedSkeletonEnabled)
	{
		setNumJointsAndJointNamesForSeatedSkeletonTracking();
	}
	else
	{
		setNumJointsAndJointNamesForRegularSkeletonTracking();
	}
	
}

void MSKinectDevice::setNumJointsAndJointNamesForSeatedSkeletonTracking()
{
	numJoints = 10;
	jointNames = new char*[numJoints];
	jointNames[0] = "neck";
	jointNames[1] = "head";
    
	jointNames[2] = "left_shoulder";
	jointNames[3] = "left_elbow";
	jointNames[4] = "left_wrist";
	jointNames[5] = "left_hand";
    
	jointNames[6] = "right_shoulder";
	jointNames[7] = "right_elbow";
	jointNames[8] = "right_wrist";
	jointNames[9] = "right_hand";
}

void MSKinectDevice::setNumJointsAndJointNamesForRegularSkeletonTracking()
{
	numJoints = 20;
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
    
	//----------------
	// Depth Image
	//----------------
	asDepthResolution = getResolutionFrom(depthImageBytesGenerator->getTargetWidth(), depthImageBytesGenerator->getTargetHeight());
	depthResolution = getResolutionFrom(depthImageBytesGenerator->getSourceWidth(), depthImageBytesGenerator->getSourceHeight());
	depthPlayerIndexEnabled = false;

	depthParser = new AKMSSDKDepthParser();
	rgbParser = new AKMSSDKRGBParser();
    
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

FREObject MSKinectDevice::freSetSkeletonMode(FREObject argv[]) {

	bool previousSeatedSkeletonEnabled = asSeatedSkeletonEnabled;
	bool previousChooseSkeletonsEnabled = asChooseSkeletonsEnabled;
	KinectDevice::freSetSkeletonMode(argv);
	if(running && (previousSeatedSkeletonEnabled != asSeatedSkeletonEnabled || previousChooseSkeletonsEnabled != asChooseSkeletonsEnabled)) {
		lockUserMutex();
		deallocateUserFrame();
		setNumJointsAndJointNames();
		allocateUserFrame();
		HRESULT hr = setSkeletonTrackingFlags();
		if ( FAILED(hr) ){
			dispatchErrorMessage((const uint8_t*) "Failed to Initalize Skeleton Tracking");
		}
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

FREObject MSKinectDevice::freSetDepthMode(FREObject argv[]) {
	KinectDevice::freSetDepthMode(argv);
	asDepthResolution = getResolutionFrom(depthImageBytesGenerator->getTargetWidth(), depthImageBytesGenerator->getTargetHeight());
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
	long degrees;
	HRESULT hr = nuiSensor->NuiCameraElevationGetAngle(&degrees);
	if ( FAILED(hr) ){
		dispatchErrorMessage((const uint8_t*) "Failed to get sensor angle");
	}
	FRENewObjectFromInt32 ((int) degrees, &asCameraAngle );
	return asCameraAngle;
}

FREObject MSKinectDevice::freCameraElevationSetAngle(FREObject argv[])
{
	dispatchInfoMessage((const uint8_t*) "[MSKinectDevice] Camera Elevation Set Angle Called");
	int degrees; FREGetObjectAsInt32(argv[1], &degrees);
	HRESULT hr = nuiSensor->NuiCameraElevationSetAngle((long) degrees);
	if ( FAILED(hr) ){
		dispatchErrorMessage((const uint8_t*) "Failed to set sensor angle");
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
		dispatchErrorMessage((const uint8_t*) "NUI Skeltal Engine Busy");
	}else if(hr == E_NUI_NOTPOWERED){
		dispatchErrorMessage((const uint8_t*) "NUI Not Powered");
	}else if(hr == E_NUI_BADIINDEX){
		dispatchErrorMessage((const uint8_t*) "Bad Index");
	}else{
		char errorMessage[100];
		sprintf_s(errorMessage, "0x%x", hr);
		dispatchErrorMessage((const uint8_t*) errorMessage);
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
	cleanupByteArrays();

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
		dispatchStatusMessage((const uint8_t*) "stopped");
    }
	
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
		dispatchInfoMessage((const uint8_t*) "Starting Device");
	
		HRESULT hr = NuiCreateSensorByIndex(this->nr, &nuiSensor);
		//Failed dispatch some message
		if ( FAILED(hr) ){
			dispatchErrorMessage((const uint8_t*) "Failed to create Sensor");
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
			dispatchErrorMessage((const uint8_t*) "Failed to initialize Sensor");
			dispatchError(hr);
			this->dispose();
			return;
		}

		userEvent	= CreateEvent( NULL, TRUE, FALSE, NULL );
		if(asSkeletonEnabled || asUserMaskEnabled){
			dispatchInfoMessage((const uint8_t*) "Starting Skeleton Tracking");

			hr = setSkeletonTrackingFlags();
			if ( FAILED(hr) ){
				dispatchErrorMessage((const uint8_t*) "Failed to Initalize Skeleton Tracking");
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
	lockUserMutex();

    memset(&userFrame.users[0], 0, sizeof(userFrame.users));
	NUI_SKELETON_FRAME skeletonFrame = {0};
	HRESULT hr = nuiSensor->NuiSkeletonGetNextFrame( 0, &skeletonFrame );
	if(FAILED(hr))
	{
		unlockUserMutex();
		return;
	}

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

			//joint orientation (kinect sdk 1.5)
			NUI_SKELETON_BONE_ORIENTATION *boneOrientations = new NUI_SKELETON_BONE_ORIENTATION[NUI_SKELETON_POSITION_COUNT];
			hr = NuiSkeletonCalculateBoneOrientations(&skeletonData, boneOrientations);

			//Joint Position Calculations
			if (userFrame.users[i].hasSkeleton){
				addJointElements(userFrame.users[i], skeletonData, boneOrientations);
			}

			//cleanup
			delete [] boneOrientations;
		}else{
			userFrame.users[i].isTracking = false;
		}
	}

	previousSkeletonFrame = &skeletonFrame;

	unlockUserMutex();
	dispatchStatusMessage((const uint8_t*) "userFrame");
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
	if(depthImageBytesGenerator->getTargetMirrored()){
		NuiTransformSkeletonToDepthImage(mSkeletonPosition, &depthX, &depthY, &depthValue,asDepthResolution);
	}else{
		NuiTransformSkeletonToDepthImage(skeletonTransform, &depthX, &depthY, &depthValue,asDepthResolution);
	}
	kTransform.depthX = depthX;
	kTransform.depthY = depthY;

	kTransform.depthRelativeX = ((float)depthX)/((float)depthImageBytesGenerator->getTargetWidth());
	kTransform.depthRelativeY = ((float)depthY)/((float)depthImageBytesGenerator->getTargetHeight());

	if(rgbImageBytesGenerator->getTargetMirrored()){
		NuiTransformSkeletonToDepthImage(mSkeletonPosition, &depthX, &depthY, &depthValue,asDepthResolution);
	}else{
		NuiTransformSkeletonToDepthImage(skeletonTransform, &depthX, &depthY, &depthValue,asDepthResolution);
	}
	
	nuiSensor->NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(asRGBResolution, asDepthResolution, 0, depthX,depthY, depthValue,&colorX,&colorY);
	kTransform.rgbX = (int)colorX;
	kTransform.rgbY = (int)colorY;

	kTransform.rgbRelativeX = ((float)colorX)/((float)rgbImageBytesGenerator->getTargetWidth());
	kTransform.rgbRelativeY = ((float)colorY)/((float)rgbImageBytesGenerator->getTargetHeight());
}

void MSKinectDevice::addJointElements(kinectUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations)
{
	if(asSeatedSkeletonEnabled)
		addJointElementsForSeatedSkeletonTracking(kUser, skeletonData, boneOrientations);
	else
		addJointElementsForRegularSkeletonTracking(kUser, skeletonData, boneOrientations);
}

void MSKinectDevice::addJointElementsForSeatedSkeletonTracking(kinectUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations)
{
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_CENTER, 0);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HEAD, 1);
                
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_LEFT, 2);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_LEFT, 3);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_LEFT, 4);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_LEFT, 5);
                
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_RIGHT, 6);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_RIGHT, 7);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_RIGHT, 8);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_RIGHT, 9);
}

void MSKinectDevice::addJointElementsForRegularSkeletonTracking(kinectUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations)
{
	addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HIP_CENTER, 0);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_SPINE, 1);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_CENTER, 2);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HEAD, 3);
                
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_LEFT, 4);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_LEFT, 5);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_LEFT, 6);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_LEFT, 7);
                
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_SHOULDER_RIGHT, 8);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_ELBOW_RIGHT, 9);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_WRIST_RIGHT, 10);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HAND_RIGHT, 11);
                
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HIP_LEFT, 12);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_KNEE_LEFT, 13);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_ANKLE_LEFT, 14);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_FOOT_LEFT, 15);
                
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_HIP_RIGHT, 16);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_KNEE_RIGHT, 17);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_ANKLE_RIGHT, 18);
    addJointElement(kUser, skeletonData, boneOrientations, NUI_SKELETON_POSITION_FOOT_RIGHT, 19);
}

void MSKinectDevice::addJointElement(kinectUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations, NUI_SKELETON_POSITION_INDEX eJoint, uint32_t targetIndex)
{
	Vector4 jointPosition = skeletonData.SkeletonPositions[eJoint];

	//Transform for User
	calculateKinectTransform(kUser.joints[targetIndex], jointPosition);

	//skeleton.eSkeletonPositionTrackingState[jointTo]

	kUser.joints[targetIndex].orientationConfidence = 0;
	kUser.joints[targetIndex].positionConfidence = 0;

	if(skeletonData.eSkeletonPositionTrackingState[eJoint] == NUI_SKELETON_POSITION_INFERRED)
	{
		kUser.joints[targetIndex].orientationConfidence = 0.5;
		kUser.joints[targetIndex].positionConfidence = 0.5;
	}
	else if(skeletonData.eSkeletonPositionTrackingState[eJoint] == NUI_SKELETON_POSITION_TRACKED)
	{
		kUser.joints[targetIndex].orientationConfidence = 1;
		kUser.joints[targetIndex].positionConfidence = 1;
	}

	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M11 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M11;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M12 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M12;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M13 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M13;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M14 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M14;

	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M21 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M21;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M22 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M22;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M23 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M23;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M24 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M24;

	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M31 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M31;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M32 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M32;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M33 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M33;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M34 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M34;

	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M41 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M41;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M42 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M42;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M43 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M43;
	kUser.joints[targetIndex].absoluteOrientation.rotationMatrix.M44 = boneOrientations[eJoint].absoluteRotation.rotationMatrix.M44;

	kUser.joints[targetIndex].absoluteOrientation.rotationQuaternion.x = boneOrientations[eJoint].absoluteRotation.rotationQuaternion.x;
	kUser.joints[targetIndex].absoluteOrientation.rotationQuaternion.y = boneOrientations[eJoint].absoluteRotation.rotationQuaternion.y;
	kUser.joints[targetIndex].absoluteOrientation.rotationQuaternion.z = boneOrientations[eJoint].absoluteRotation.rotationQuaternion.z;
	kUser.joints[targetIndex].absoluteOrientation.rotationQuaternion.w = boneOrientations[eJoint].absoluteRotation.rotationQuaternion.w;

	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M11 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M11;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M12 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M12;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M13 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M13;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M14 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M14;

	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M21 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M21;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M22 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M22;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M23 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M23;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M24 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M24;

	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M31 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M31;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M32 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M32;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M33 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M33;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M34 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M34;

	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M41 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M41;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M42 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M42;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M43 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M43;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationMatrix.M44 = boneOrientations[eJoint].hierarchicalRotation.rotationMatrix.M44;

	kUser.joints[targetIndex].hierarchicalOrientation.rotationQuaternion.x = boneOrientations[eJoint].hierarchicalRotation.rotationQuaternion.x;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationQuaternion.y = boneOrientations[eJoint].hierarchicalRotation.rotationQuaternion.y;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationQuaternion.z = boneOrientations[eJoint].hierarchicalRotation.rotationQuaternion.z;
	kUser.joints[targetIndex].hierarchicalOrientation.rotationQuaternion.w = boneOrientations[eJoint].hierarchicalRotation.rotationQuaternion.w;

    kUser.joints[targetIndex].positionConfidence = 1;
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

void MSKinectDevice::setRGBMode(int rgbWidth, int rgbHeight, int asRGBWidth, int asRGBHeight, bool asRGBMirrored)
{
	//support higher resolution for MS SDK
	if(asRGBWidth == 1280 && asRGBHeight == 960)
	{
		rgbWidth = 1280;
		rgbHeight = 960;
	}
	KinectDevice::setRGBMode(rgbWidth, rgbHeight, asRGBWidth, asRGBHeight, asRGBMirrored);
	asRGBResolution = getResolutionFrom(asRGBWidth, asRGBHeight);
	rgbResolution = getResolutionFrom(rgbWidth, rgbHeight);
}

#endif
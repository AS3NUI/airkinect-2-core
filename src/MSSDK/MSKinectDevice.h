#ifndef KinectExtension_MSKinectDevice_h
#define KinectExtension_MSKinectDevice_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_TARGET_MSSDK

#include "KinectSkeleton.h"
#include "PointCloudRegion.h"
#include "KinectCapabilities.h"
#include "KinectDevice.h"
#include "AKMSSDKRGBParser.h"
#include "AKMSSDKDepthParser.h"

class MSKinectDevice : public KinectDevice
{
public:

	MSKinectDevice(int nr);
    
    void			start();
    void			stop();
    
    void			dispose();

	//Overridden FRE functions
	FREObject           freSetSkeletonMode(FREObject argv[]);
	FREObject			freChooseSkeletons(FREObject argv[]);
    FREObject           freSetDepthMode(FREObject argv[]);
	FREObject			freSetNearModeEnabled(FREObject argv[]);
	FREObject           freCameraElevationGetAngle(FREObject argv[]);
	FREObject           freCameraElevationSetAngle(FREObject argv[]);

protected:

	void				setUserColor(int userID, int color, bool useIntensity);
	void				setRGBMode(int rgbWidth, int rgbHeight, int asRGBWidth, int asRGBHeight, bool asRGBMirrored);

	void				setDefaults();
	void				cleanupByteArrays();
	void				setNumJointsAndJointNames();
	void				setNumJointsAndJointNamesForSeatedSkeletonTracking();
	void				setNumJointsAndJointNamesForRegularSkeletonTracking();

	void				createPointCloudGenerator();
	void				createUserMasksGenerator();

private:

	NUI_TRANSFORM_SMOOTH_PARAMETERS		transformSmoothingParameters;
	void					addKinectDeviceStatusListener();
	void CALLBACK			statusProc( HRESULT hrStatus, const OLECHAR* instanceName, const OLECHAR* uniqueDeviceName );
	static void CALLBACK	statusProcThunk( HRESULT hrStatus, const OLECHAR* instanceName, const OLECHAR* uniqueDeviceName, void * pUserData);
	void					dispatchError(HRESULT hr);

	void					addJointElements(kinectUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations);
	void					addJointElementsForSeatedSkeletonTracking(kinectUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations);
	void					addJointElementsForRegularSkeletonTracking(kinectUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations);
	void					addJointElement(kinectUser &kUser, NUI_SKELETON_DATA skeletonData, NUI_SKELETON_BONE_ORIENTATION *boneOrientations, NUI_SKELETON_POSITION_INDEX eJoint, uint32_t targetIndex);
	void					calculateKinectTransform(kinectTransform &kTransform, Vector4 skeletonTransform);

	HRESULT					setSkeletonTrackingFlags();

	int						imageFrameTimeout;
	INuiSensor*             nuiSensor;

	static void             *deviceThread(void *ptr);
	void                    run();

    HANDLE					depthFrameEvent;
    HANDLE					rgbFrameEvent;
    HANDLE					userEvent;

	HANDLE					userFrameHandle;
    HANDLE					depthFrameHandle;
    HANDLE					rgbFrameHandle;
	
	NUI_SKELETON_FRAME		*previousSkeletonFrame;

	BYTE					*userIndexColors;
    
	NUI_IMAGE_RESOLUTION	asDepthResolution;
	NUI_IMAGE_RESOLUTION	depthResolution;
	bool					depthPlayerIndexEnabled;
    
	NUI_IMAGE_RESOLUTION	asRGBResolution;
	NUI_IMAGE_RESOLUTION	rgbResolution;

	//Handlers
	void					readRGBFrame();
	AKMSSDKRGBParser*		rgbParser;

	void					readDepthFrame();
	AKMSSDKDepthParser*		depthParser;

	void					dispatchUserFrameIfNeeded();
	void					dispatchRGBIfNeeded();
	void					dispatchDepthIfNeeded();
	void					dispatchPointCloudIfNeeded();
	void					dispatchUserMaskIfNeeded();
    
	NUI_IMAGE_RESOLUTION getResolutionFrom(int width, int height);
	POINT getDepthPixelPointFromJointCoordinate(Vector4 jointCoordinates);
};

#endif
#endif
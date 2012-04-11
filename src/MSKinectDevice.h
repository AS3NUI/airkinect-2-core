#ifndef KinectExtension_MSKinectDevice_h
#define KinectExtension_MSKinectDevice_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_TARGET_MSSDK

#include "KinectSkeleton.h"
#include "PointCloudRegion.h"
#include "KinectCapabilities.h"
#include "KinectDevice.h"

class MSKinectDevice : public KinectDevice
{

	static const int			DEPTH_MIN = 800;
	static const int			DEPTH_MAX = 4000;
	static const int			DEPTH_RANGE = DEPTH_MAX - DEPTH_MIN;
public:

	MSKinectDevice(int nr);
    
    void			start();
    void			stop();
    
    void			dispose();

	//Overridden FRE functions
    FREObject           freSetUserMaskMode(FREObject argv[]);
    FREObject           freSetDepthMode(FREObject argv[]);
    FREObject           freSetRGBMode(FREObject argv[]);
	FREObject           freCameraElevationGetAngle(FREObject argv[]);
	FREObject           freCameraElevationSetAngle(FREObject argv[]);

protected:

	void				setUserColor(int userID, int color, bool useIntensity);

private:

	NUI_TRANSFORM_SMOOTH_PARAMETERS		transformSmoothingParameters;
	void					addKinectDeviceStatusListener();
	void CALLBACK			statusProc( HRESULT hrStatus, const OLECHAR* instanceName, const OLECHAR* uniqueDeviceName );
	static void CALLBACK	statusProcThunk( HRESULT hrStatus, const OLECHAR* instanceName, const OLECHAR* uniqueDeviceName, void * pUserData);
	void					dispatchError(HRESULT hr);

	void					addJointElement(kinectUser &kUser, NUI_SKELETON_DATA user, NUI_SKELETON_POSITION_INDEX eJoint, uint32_t targetIndex);
	void					calculateKinectTransform(kinectTransform &kTransform, Vector4 skeletonTransform);

	int						imageFrameTimeout;
	INuiSensor *            nuiSensor;
    
    void                    setDefaults();

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
	uint32_t                *depthImageByteArray;
	USHORT					*depthByteArray;
	USHORT					*sceneByteArray;
	bool					depthPlayerIndexEnabled;
    
	NUI_IMAGE_RESOLUTION	asRGBResolution;
	NUI_IMAGE_RESOLUTION	rgbResolution;
	uint32_t                *rgbImageByteArray;
    
	NUI_IMAGE_RESOLUTION	asUserMaskResolution;

	//Requested Image to Actual Image Scaling
	void					updateConfigScale();

	//Handlers
	void					readRGBFrame();
	void					readDepthFrame();

	void					userFrameHandler();
	void					rgbFrameHandler();
	void					depthFrameHandler();
	void					pointCloudHandler();
	void					pointCloudWithRGBHandler();
	void					userMaskHandler();
    
	NUI_IMAGE_RESOLUTION getResolutionFrom(int width, int height);
	POINT getDepthPixelPointFromJointCoordinate(Vector4 jointCoordinates);
	RGBQUAD ShortToQuad_Depth( USHORT s, BOOLEAN usePlayer );
};

#endif
#endif
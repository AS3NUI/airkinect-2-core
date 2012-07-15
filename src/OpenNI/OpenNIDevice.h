#ifndef KinectExtension_OpenNIDevice_h
#define KinectExtension_OpenNIDevice_h

#include "ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_OPENNI

#ifdef AIRKINECT_OS_WINDOWS
#else
    #include <Adobe AIR/Adobe AIR.h>
    #include <boost/thread.hpp>
    #include <boost/thread/mutex.hpp>
#endif

#include <XnCppWrapper.h>
#include "KinectDevice.h"
#include "KinectSkeleton.h"
#include "KinectCapabilities.h"
#include "PointCloudRegion.h"
#include "AKOpenNIRGBParser.h"
#include "AKOpenNIDepthParser.h"

class OpenNIDevice : KinectDevice
{
    static const int        MAX_DEPTH = 10000;
public:
    
    OpenNIDevice(int nr, xn::Context context);
    
	//Starts and Stop the Kinect Device
    void                    start();
    void                    stop();
    
	//Dispose the Device from memory
    void                    dispose();
    
    FREObject               freSetInfraredMode(FREObject argv[]);
    FREObject               freSetInfraredEnabled(FREObject argv[]);
    FREObject               freGetInfraredFrame(FREObject argv[]);
    
protected:
    
	void                    setUserColor(int userID, int color, bool useIntensity);
	void					setDefaults();
	void					cleanupByteArrays();
	void					setNumJointsAndJointNames();
    
private:
    xn::Context             context;
    
    int                     getAsPointCloudByteArrayLength();
    
    void                    run();
    
    static void             *deviceThread(void *ptr);
    
    int                     returnVal;
    
    XnCallbackHandle        userHandle;
    XnCallbackHandle        userExitHandle;
    XnCallbackHandle        userReentryHandle;
    XnCallbackHandle        calibrationStartHandle;
    XnCallbackHandle        calibrationCompleteHandle;
    XnCallbackHandle        poseDetectedHandle;
    XnCallbackHandle        outOfPoseHandle;
    
    bool                    userCallbacksRegistered;
    
    void                    userHandler();
    void                    addJointElement(kinectUser &kUser, XnUserID user, XnSkeletonJoint eJoint, uint32_t targetIndex, int targetOrientationIndex);
    
    float                   **userIndexColors;//[MAX_SKELETONS][4];
    
    int                     asInfraredWidth;
    int                     asInfraredHeight;
    int                     asInfraredPixelCount;
    bool                    asInfraredMirrored;
    bool                    asInfraredEnabled;
    
    int                     infraredWidth;
    int                     infraredHeight;
    int                     infraredPixelCount;
    int                     infraredScale;
    
    uint32_t                *asInfraredByteArray;
    
    xn::ImageGenerator      imageGenerator;
    xn::DepthGenerator      depthGenerator;
    xn::UserGenerator       userGenerator;
    xn::IRGenerator         infraredGenerator;
    
    xn::ImageMetaData       imageMetaData;
    xn::DepthMetaData       depthMetaData;
    xn::SceneMetaData       sceneMetaData;
    xn::IRMetaData          infraredMetaData;
    
	void					readRGBFrame();
	void					readDepthFrame();

    void                    rgbFrameHandler();
	AKOpenNIRGBParser*		rgbParser;
    
    const XnIRPixel         *infraredFrameBuffer;
    void                    infraredHandler();
    
    void                    depthFrameHandler();
	AKOpenNIDepthParser*	depthParser;
    
    void                    userMaskHandler();
    void                    pointCloudHandler();
    void                    pointCloudWithRGBHandler();
};

#endif
#endif

#ifndef KinectExtension_OpenNIDevice_h
#define KinectExtension_OpenNIDevice_h

#include "ExtensionConfig.h"

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

class OpenNIDevice : KinectDevice
{
    static const int        MAX_DEPTH = 10000;
public:
    
    OpenNIDevice(int nr, xn::Context context);
    
    //Getter/Setters for FREContext
    FREContext			getFreContext();
	void				setFreContext(FREContext pFreContext);
    
	//Starts and Stop the Kinect Device
    void				start();
    void				stop();
    
	//Dispose the Device form memory
    void				dispose();
    
    FREObject           freSetInfraredMode(FREObject argv[]);
    FREObject           freSetInfraredEnabled(FREObject argv[]);
    FREObject           freGetInfraredFrame(FREObject argv[]);
    
protected:
	void                    lockUserMutex();
    void                    unlockUserMutex();
    
    void                    lockDepthMutex();
    void                    unlockDepthMutex();
    
    void                    lockRGBMutex();
    void                    unlockRGBMutex();
    
    void                    lockUserMaskMutex();
    void                    unlockUserMaskMutex();
    
    void                    lockInfraredMutex();
    void                    unlockInfraredMutex();
    
    void                    lockPointCloudMutex();
    void                    unlockPointCloudMutex();
    
	void                    setUserColor(int userID, int color, bool useIntensity);
    
    void                    setDefaults();
    
private:
    int                     nr;
    xn::Context             context;
    
    boost::mutex            userMutex;
    boost::mutex            depthMutex;
    boost::mutex            rgbMutex;
    boost::mutex            userMaskMutex;
    boost::mutex            infraredMutex;
    boost::mutex            pointCloudMutex;
    
    int                     getAsPointCloudByteArrayLength();
    
    void                    run();
    
    static void             *deviceThread(void *ptr);
    boost::thread           mThread;
    
    int                     returnVal;
    bool                    running;
    
    XnCallbackHandle        userHandle;
    XnCallbackHandle        userExitHandle;
    XnCallbackHandle        userReentryHandle;
    XnCallbackHandle        calibrationStartHandle;
    XnCallbackHandle        calibrationCompleteHandle;
    XnCallbackHandle        poseDetectedHandle;
    XnCallbackHandle        outOfPoseHandle;
    
    bool                    userCallbacksRegistered;
    
    void                    userHandler();
    void                    addJointElement(kinectUser &kUser, XnUserID user, XnSkeletonJoint eJoint, uint32_t targetIndex);
    
    float                   depthHistogram[MAX_DEPTH];
    
    float                   userIndexColors[MAX_SKELETONS][4];
    
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
    
    const XnRGB24Pixel      *RGBFrameBuffer;
    void                    rgbFrameHandler();
    
    const XnDepthPixel      *depthFrameBuffer;
    const XnLabel           *sceneFrameBuffer;
    
    const XnIRPixel         *infraredFrameBuffer;
    void                    infraredHandler();
    
    void                    calculateHistogram();
    void                    depthFrameHandler();
    void                    depthFrameWithUserColorsHandler();
    
    void                    userMaskHandler();
    void                    pointCloudHandler();
    void                    pointCloudWithRGBHandler();
};

#endif

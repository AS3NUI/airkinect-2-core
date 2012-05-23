#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #ifdef KINECTEXTENSION_EXPORTS
        #define KINECTEXTENSION_API __declspec(dllexport)
    #else
        #define KINECTEXTENSION_API __declspec(dllimport)
    #endif
    #include "FlashRuntimeExtensions.h"
#else
    #define KINECTEXTENSION_API __attribute__((visibility("default")))
    #include <Adobe AIR/Adobe AIR.h>
#endif

extern "C"
{
    
    //methods
	FREObject Kinect_applicationStartup(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_getDeviceCount(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_getCapabilities(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_applicationShutdown(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    
    FREObject Kinect_start(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_stop(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);

    FREObject Kinect_setUserMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_setUserColor(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_setUserEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_setSkeletonMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_setSkeletonEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_getSkeletonJointNameIndices(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_getSkeletonJointNames(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_getUserFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_setUserMaskMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_setUserMaskEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_getUserMaskFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    
    FREObject Kinect_setDepthMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_setDepthEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_getDepthFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_setDepthShowUserColors(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject Kinect_setDepthEnableNearMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    
    FREObject Kinect_setRGBMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_setRGBEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_getRGBFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    
    FREObject Kinect_setInfraredMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_setInfraredEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_getInfraredFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    
    FREObject Kinect_setPointCloudMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_setPointCloudEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_getPointCloudFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    FREObject Kinect_setPointCloudRegions(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    
	FREObject Kinect_getCameraElevationAngle(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
	FREObject Kinect_setCameraElevationAngle(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]);
    
    //initializer / finalizer
    void contextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctions, const FRENamedFunction** functions);
    void contextFinalizer(FREContext ctx);
    
    KINECTEXTENSION_API void AIRKinectInitializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer);
	KINECTEXTENSION_API void AIRKinectFinalizer(void* extData);
}
#include <stdlib.h>
#include <stdio.h>

#include "KinectExtension.h"
#include "KinectDeviceManager.h"
#include "PointCloudRegion.h"

// Symbols tagged with EXPORT are externally visible.
// Must use the -fvisibility=hidden gcc option.
#define EXPORT __attribute__((visibility("default")))

extern "C"
{
    
    KinectDeviceManager kinectDeviceManager;
    
    FREObject Kinect_getDeviceCount(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        FREObject retObj;
		FRENewObjectFromInt32(kinectDeviceManager.getNumDevices(), &retObj);
		return retObj;
    }
    
    FREObject Kinect_applicationShutdown(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
		kinectDeviceManager.shutDown();
        return NULL;
    }
    
    FREObject Kinect_start(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
		kinectDeviceManager.getDevice(nr, ctx)->start();
        return NULL;
    }
    
    FREObject Kinect_stop(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
		kinectDeviceManager.getDevice(nr, ctx)->stop();
        return NULL;
    }
    
	FREObject Kinect_getCapabilities(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
		unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freGetCapabilities();
    }
    
    FREObject Kinect_setUserMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
		unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetUserMode(argv);
    }
    
	FREObject Kinect_setUserColor(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetUserColor(argv);
    }
	
    FREObject Kinect_setUserEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetUserEnabled(argv);
    }
    
    FREObject Kinect_setSkeletonMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetSkeletonMode(argv);
    }
    
    FREObject Kinect_setSkeletonEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetSkeletonEnabled(argv);
    }
    
    FREObject Kinect_getSkeletonJointNameIndices(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
		unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
		return kinectDeviceManager.getDevice(nr, ctx)->freGetSkeletonJointNameIndices(argv);
    }
    
    FREObject Kinect_getSkeletonJointNames(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
		unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
		return kinectDeviceManager.getDevice(nr, ctx)->freGetSkeletonJointNames(argv);
    }
    
    FREObject Kinect_getUserFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freGetUserFrame(argv);
    }
    
    FREObject Kinect_setUserMaskMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
		unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetUserMaskMode(argv);
    }
    
    FREObject Kinect_setUserMaskEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetUserMaskEnabled(argv);
    }
    
    FREObject Kinect_getUserMaskFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freGetUserMaskFrame(argv);
    }
    
    FREObject Kinect_setDepthMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetDepthMode(argv);
    }
    
    FREObject Kinect_setDepthEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetDepthEnabled(argv);
    }
    
    FREObject Kinect_getDepthFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freGetDepthFrame(argv);
    }
    
    FREObject Kinect_setDepthShowUserColors(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetDepthShowUserColors(argv);
    }
    
	FREObject Kinect_setDepthEnableNearMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetDepthEnableNearMode(argv);
	}
    
    FREObject Kinect_setRGBMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetRGBMode(argv);
    }
    
    FREObject Kinect_setRGBEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetRGBEnabled(argv);
    }
    
    FREObject Kinect_getRGBFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freGetRGBFrame(argv);
    }
    
    FREObject Kinect_setInfraredMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetInfraredMode(argv);
    }
    
    FREObject Kinect_setInfraredEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetInfraredEnabled(argv);
    }
    
    FREObject Kinect_getInfraredFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freGetInfraredFrame(argv);
    }
    
    FREObject Kinect_setPointCloudMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetPointCloudMode(argv);
    }
    
    FREObject Kinect_setPointCloudEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetPointCloudEnabled(argv);
    }
    
    FREObject Kinect_getPointCloudFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freGetPointCloudFrame(argv);
    }
    
    FREObject Kinect_setPointCloudRegions(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetPointCloudRegions(argv);
    }
    
	FREObject Kinect_getCameraElevationAngle(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freCameraElevationGetAngle(argv);
	}
    
	FREObject Kinect_setCameraElevationAngle(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freCameraElevationSetAngle(argv);
	}
    
	/*-----------------------------------------
     Start Native Extension Accessibility
     ------------------------------------------*/
    
	FRENamedFunction _Static_methods[] = {
		{ (const uint8_t*) "getDeviceCount", 0, Kinect_getDeviceCount},
        { (const uint8_t*) "applicationShutdown", 0, Kinect_applicationShutdown}
    };
    
	FRENamedFunction _Instance_methods[] = {
		{ (const uint8_t*) "start", 0, Kinect_start },
		{ (const uint8_t*) "stop", 0, Kinect_stop },
		{ (const uint8_t*) "getCapabilities", 0, Kinect_getCapabilities },
        { (const uint8_t*) "setUserMode", 0, Kinect_setUserMode },
		{ (const uint8_t*) "setUserColor", 0, Kinect_setUserColor },
        { (const uint8_t*) "setUserEnabled", 0, Kinect_setUserEnabled },
        { (const uint8_t*) "setSkeletonMode", 0, Kinect_setSkeletonMode },
        { (const uint8_t*) "setSkeletonEnabled", 0, Kinect_setSkeletonEnabled },
        { (const uint8_t*) "getUserFrame", 0, Kinect_getUserFrame },
        { (const uint8_t*) "getSkeletonJointNameIndices", 0, Kinect_getSkeletonJointNameIndices },
        { (const uint8_t*) "getSkeletonJointNames", 0, Kinect_getSkeletonJointNames },
        { (const uint8_t*) "setUserMaskMode", 0, Kinect_setUserMaskMode },
        { (const uint8_t*) "setUserMaskEnabled", 0, Kinect_setUserMaskEnabled },
        { (const uint8_t*) "getUserMaskFrame", 0, Kinect_getUserMaskFrame },
        { (const uint8_t*) "setDepthMode", 0, Kinect_setDepthMode },
        { (const uint8_t*) "setDepthEnabled", 0, Kinect_setDepthEnabled },
        { (const uint8_t*) "getDepthFrame", 0, Kinect_getDepthFrame },
        { (const uint8_t*) "setDepthShowUserColors", 0, Kinect_setDepthShowUserColors },
		{ (const uint8_t*) "setDepthEnableNearMode", 0, Kinect_setDepthEnableNearMode },
        { (const uint8_t*) "setRGBMode", 0, Kinect_setRGBMode },
        { (const uint8_t*) "setRGBEnabled", 0, Kinect_setRGBEnabled },
        { (const uint8_t*) "getRGBFrame", 0, Kinect_getRGBFrame },
        { (const uint8_t*) "setInfraredMode", 0, Kinect_setInfraredMode },
        { (const uint8_t*) "setInfraredEnabled", 0, Kinect_setInfraredEnabled },
        { (const uint8_t*) "getInfraredFrame", 0, Kinect_getInfraredFrame },
        { (const uint8_t*) "setPointCloudMode", 0, Kinect_setPointCloudMode },
        { (const uint8_t*) "setPointCloudEnabled", 0, Kinect_setPointCloudEnabled },
        { (const uint8_t*) "getPointCloudFrame", 0, Kinect_getPointCloudFrame },
        { (const uint8_t*) "setPointCloudRegions", 0, Kinect_setPointCloudRegions },
		{ (const uint8_t*) "getCameraElevationAngle", 0, Kinect_getCameraElevationAngle },
		{ (const uint8_t*) "setCameraElevationAngle", 0, Kinect_setCameraElevationAngle }
        
	};
    
    void contextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctions, const FRENamedFunction** functions)
    {
        if ( 0 == strcmp( (const char*) ctxType, "shared" ) )
		{
			*numFunctions = sizeof( _Static_methods ) / sizeof( FRENamedFunction );
			*functions = _Static_methods;
		}
		else
        {
            *numFunctions = sizeof( _Instance_methods ) / sizeof( FRENamedFunction );
            *functions = _Instance_methods;
        }
        
        //make sure the manager is started
        if(!kinectDeviceManager.isStarted())
        {
            kinectDeviceManager.startUp();
        }
	}
    
	void contextFinalizer(FREContext ctx)
    {
		return;
	}
    
    EXPORT
	void initializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer)
    {
		*ctxInitializer = &contextInitializer;
		*ctxFinalizer = &contextFinalizer;
	}
    
    EXPORT
	void finalizer(void* extData)
    {
		return;
	}
}
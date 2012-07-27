/*
 * Copyright 2012 AS3NUI
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#include <stdlib.h>
#include <stdio.h>

#include "KinectExtension.h"
#include "KinectDeviceManager.h"

extern "C"
{
    
    KinectDeviceManager kinectDeviceManager;
    
    FREObject Kinect_getDeviceCount(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        FREObject retObj;
		FRENewObjectFromInt32(kinectDeviceManager.getNumDevices(), &retObj);
		return retObj;
    }

	FREObject Kinect_applicationStartup(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
		unsigned int framework; FREGetObjectAsUint32(argv[0], &framework);
		kinectDeviceManager.startUp(framework);
        return NULL;
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

	FREObject Kinect_chooseSkeletons(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freChooseSkeletons(argv);
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
    
	FREObject Kinect_setNearModeEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
	{
		unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        return kinectDeviceManager.getDevice(nr, ctx)->freSetNearModeEnabled(argv);
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
		{ (const uint8_t*) "applicationStartup", 0, Kinect_applicationStartup},
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
		{ (const uint8_t*) "chooseSkeletons", 0, Kinect_chooseSkeletons },
        { (const uint8_t*) "getUserFrame", 0, Kinect_getUserFrame },
        { (const uint8_t*) "setUserMaskMode", 0, Kinect_setUserMaskMode },
        { (const uint8_t*) "setUserMaskEnabled", 0, Kinect_setUserMaskEnabled },
        { (const uint8_t*) "getUserMaskFrame", 0, Kinect_getUserMaskFrame },
        { (const uint8_t*) "setDepthMode", 0, Kinect_setDepthMode },
        { (const uint8_t*) "setDepthEnabled", 0, Kinect_setDepthEnabled },
        { (const uint8_t*) "getDepthFrame", 0, Kinect_getDepthFrame },
        { (const uint8_t*) "setDepthShowUserColors", 0, Kinect_setDepthShowUserColors },
		{ (const uint8_t*) "setNearModeEnabled", 0, Kinect_setNearModeEnabled },
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
	}
    
	void contextFinalizer(FREContext ctx)
    {
		return;
	}
    
	void AIRKinectInitializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer)
    {
		*ctxInitializer = &contextInitializer;
		*ctxFinalizer = &contextFinalizer;
	}
    
	void AIRKinectFinalizer(void* extData)
    {
		return;
	}
}
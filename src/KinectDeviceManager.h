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
#ifndef KinectExtension_KinectDeviceManager_h
#define KinectExtension_KinectDeviceManager_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#ifdef AIRKINECT_TARGET_OPENNI
#include <XnCppWrapper.h>
#endif

#include <map>
#include "KinectDevice.h"

class KinectDeviceManager
{
public:
    KinectDeviceManager();
    
    int                             getNumDevices();
    
    bool                            isStarted();
    void                            startUp(unsigned int framework);
    void                            shutDown();
    KinectDevice                    *getDevice(int nr, FREContext freContext);
    
private:
#ifdef AIRKINECT_TARGET_OPENNI
    xn::Context                     *openNIContext;
#endif
    
    bool                            started;    
    std::map<int, KinectDevice*>    deviceMap;

	bool							useMSSDK;
	bool							useOpenNI;
};

#endif

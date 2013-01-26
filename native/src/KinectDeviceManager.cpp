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
#include "KinectDeviceManager.h"

#ifdef AIRKINECT_TARGET_MSSDK
    #include "MSSDK/MSKinectDevice.h"
#endif
#ifdef AIRKINECT_TARGET_OPENNI
    #include "OpenNI/OpenNIDevice.h"
#endif

#include <iostream>

KinectDeviceManager::KinectDeviceManager()
{
    printf("KinectDeviceManager::KinectDeviceManager()\n");
}

/**
 * framework lets you specify the desired framework when multiple frameworks are available
 * 0: auto
 * 1: mssdk
 * 2: openni
 */
void KinectDeviceManager::startUp(unsigned int framework)
{
	if(!started)
	{
		started = true;

		//check which framework we will use to create devices
		useMSSDK = false;
		useOpenNI = false;
		#ifdef AIRKINECT_OS_OSX
			useOpenNI = true;
		#else
			//check if we are requiring both OpenNI & MS SDK?
			#ifdef AIRKINECT_TARGET_BOTH
				//check the framework parameter
				switch(framework)
				{
				case 1:
					useMSSDK = true;
					break;
				case 2:
					useOpenNI = true;
					break;
				case 0:
				default:
					//try loading the MS SDK. If this works, we will use the MS SDK for the devices
					HINSTANCE msdll = LoadLibrary(L"Kinect10.dll");
					if(msdll != 0) {
						useMSSDK = true;
					} else {
						useOpenNI = true;
					}
					FreeLibrary(msdll);
					break;
				}
			#else
				#ifdef AIRKINECT_TARGET_MSSDK
					useMSSDK = true;
				#endif
				#ifdef AIRKINECT_TARGET_OPENNI
					useOpenNI = true;
				#endif
			#endif
		#endif

#ifdef AIRKINECT_TARGET_OPENNI
		//MSSDK is started, OpenNI needs to initialize the context
		if(useOpenNI)
		{
			try
			{
				XnStatus rc;
				xn::EnumerationErrors errors;
				openNIContext = new xn::Context();
				rc = openNIContext->Init();
				if (rc == XN_STATUS_NO_NODE_PRESENT)
				{
					XnChar strError[1024];
					errors.ToString(strError, 1024);
					printf("%s\n", strError);
					started = false;
				}
				else if (rc != XN_STATUS_OK)
				{
					printf("Open failed: %s\n", xnGetStatusString(rc));
					started = false;
				}
			}
			catch(char *err)
			{
				printf("OpenNI failed: %s\n", err);
				started = false;
			}
		}
#endif
	}
}

void KinectDeviceManager::shutDown()
{
	if(started)
	{
		started = false;
#ifdef AIRKINECT_TARGET_OPENNI
		if(useOpenNI)
		{
			openNIContext->Release();
		}
#endif
	}
}

bool KinectDeviceManager::isStarted()
{
    return started;
}

int KinectDeviceManager::getNumDevices()
{
    int numDevices = 0;
    if(started)
    {
#ifdef AIRKINECT_TARGET_MSSDK
		if(useMSSDK)
		{
			NuiGetSensorCount(&numDevices);
		}
#endif
#ifdef AIRKINECT_TARGET_OPENNI
		if(useOpenNI)
		{
			XnStatus rc;
			xn::NodeInfoList deviceNodes;
			rc = openNIContext->EnumerateProductionTrees( XN_NODE_TYPE_DEVICE, NULL, deviceNodes, NULL );
        
			numDevices = 0;
        
			//get number of devices -------------------------------------------------------------------------------->
			xn::NodeInfoList::Iterator deviceIter = deviceNodes.Begin();
			for( ; deviceIter != deviceNodes.End(); ++deviceIter )
				++numDevices; 
		}
#endif
    }
    return numDevices;
}

KinectDevice *KinectDeviceManager::getDevice(int nr, FREContext freContext)
{
    KinectDevice* instance = NULL;
    std::map<int, KinectDevice*>::iterator it = deviceMap.find(nr);
    
    if (it != deviceMap.end())
    {
        instance = (KinectDevice*)(it->second);
    }
    else
    {
#ifdef AIRKINECT_TARGET_MSSDK
		if(useMSSDK)
		{
			instance = (KinectDevice *) (new MSKinectDevice(nr));
		}
#endif
#ifdef AIRKINECT_TARGET_OPENNI
		if(useOpenNI)
		{
			instance = (KinectDevice *) (new OpenNIDevice(nr, *openNIContext));  
		}
#endif
        deviceMap[nr] = instance;
    }
    
    instance->setFreContext(freContext);
    
    return instance;
}
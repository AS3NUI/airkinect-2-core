#include "KinectDeviceManager.h"

#ifdef AIRKINECT_TARGET_MSSDK
    #include "MSKinectDevice.h"
#else
    #include "OpenNIDevice.h"
#endif

#include <iostream>

KinectDeviceManager::KinectDeviceManager()
{
    printf("KinectDeviceManager::KinectDeviceManager()\n");
}

void KinectDeviceManager::startUp()
{
#ifdef AIRKINECT_TARGET_MSSDK
    started = true;
#else
    if(!started)
    {
        started = true;
        
        XnStatus rc;
        xn::EnumerationErrors errors;
        rc = context.Init();
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
#endif
}

void KinectDeviceManager::shutDown()
{
#ifdef AIRKINECT_TARGET_MSSDK
    started = false;
#else
    if(started)
    {
        context.Release();
        
        started = false;
    }
#endif
}

bool KinectDeviceManager::isStarted()
{
    return started;
}

int KinectDeviceManager::getNumDevices()
{
    //printf("KinectDeviceManager::getNumDevices()\n");
    int numDevices = 0;
    if(started)
    {
#ifdef AIRKINECT_TARGET_MSSDK
        NuiGetSensorCount(&numDevices);
#else
        XnStatus rc;
        xn::NodeInfoList deviceNodes;
        rc = context.EnumerateProductionTrees( XN_NODE_TYPE_DEVICE, NULL, deviceNodes, NULL );
        
        numDevices = 0;
        
        //get number of devices -------------------------------------------------------------------------------->
        xn::NodeInfoList::Iterator deviceIter = deviceNodes.Begin();
        for( ; deviceIter != deviceNodes.End(); ++deviceIter )
            ++numDevices;  
#endif
    }
    return numDevices;
}

KinectDevice *KinectDeviceManager::getDevice(int nr, FREContext freContext)
{
    //printf("KinectDeviceManager::getDevice()\n");
    KinectDevice* instance = NULL;
    std::map<int, KinectDevice*>::iterator it = deviceMap.find(nr);
    
    if (it != deviceMap.end())
    {
        instance = (KinectDevice*)(it->second);
    }
    else
    {
#ifdef AIRKINECT_TARGET_MSSDK
        instance = (KinectDevice *) (new MSKinectDevice(nr));
#else
        instance = (KinectDevice *) (new OpenNIDevice(nr, context));  
#endif
        deviceMap[nr] = instance;
    }
    
    instance->setFreContext(freContext);
    
    return instance;
}
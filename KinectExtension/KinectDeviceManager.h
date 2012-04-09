//
//  KinectDeviceManager.h
//  KinectExtension
//
//  Created by Wouter Verweirder on 24/01/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef KinectExtension_KinectDeviceManager_h
#define KinectExtension_KinectDeviceManager_h

#include <map>
#include <Adobe AIR/Adobe AIR.h>
#include <XnCppWrapper.h>
#include "KinectDevice.h"

class KinectDeviceManager
{
public:
    KinectDeviceManager();
    
    int                             getNumDevices();
    
    bool                            isStarted();
    void                            startUp();
    void                            shutDown();
    KinectDevice                    *getDevice(int nr, FREContext freContext);
    
private:
    xn::Context                     context;
    
    bool                            started;
    
    int                             numDevices;
    std::map<int, KinectDevice*>    deviceMap;
};

#endif

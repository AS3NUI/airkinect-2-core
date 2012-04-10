#ifndef KinectExtension_KinectDeviceManager_h
#define KinectExtension_KinectDeviceManager_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#ifdef AIRKINECT_TARGET_MSSDK
#else
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
    void                            startUp();
    void                            shutDown();
    KinectDevice                    *getDevice(int nr, FREContext freContext);
    
private:
#ifdef AIRKINECT_TARGET_OPENNI
    xn::Context                     context;
#endif
    
    bool                            started;    
    std::map<int, KinectDevice*>    deviceMap;
};

#endif

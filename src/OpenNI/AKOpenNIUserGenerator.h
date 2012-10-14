//
//  AKOpenNIUserGenerator.h
//  KinectExtension
//
//  Created by Wouter Verweirder on 14/10/12.
//
//

#ifndef __KinectExtension__AKOpenNIUserGenerator__
#define __KinectExtension__AKOpenNIUserGenerator__

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_OPENNI

#include "OpenNIDevice.h"
#include <XnCppWrapper.h>

class OpenNIDevice;

class AKOpenNIUserGenerator
{
public:
    
	AKOpenNIUserGenerator(OpenNIDevice* device);
	~AKOpenNIUserGenerator();
    
    void                    setup(xn::Context* xnContext);
    void                    start();
    void                    stop();
    
    bool                    update( );
    void                    parse();
    
    bool                    userGeneratorCreated();
    xn::UserGenerator*      getXnUserGenerator();
    
    xn::SceneMetaData*      getXnSceneMetaData();
    
    bool                    isGenerating();
    
    void                    setSmoothing(double smoothing);
    
    void					dispatchErrorMessage(const uint8_t* errorMessage);
	void					dispatchInfoMessage(const uint8_t* infoMessage);
	void					dispatchStatusMessage(const uint8_t* statusMessage);
	void					trace(const uint8_t* traceMessage);
    
private:
    
    OpenNIDevice*           _device;
    
    xn::Context*            _xnContext;
    
    xn::UserGenerator       _userGenerator;
    xn::SceneMetaData       _sceneMetaData;
    
    XnCallbackHandle        _userHandle;
    XnCallbackHandle        _userExitHandle;
    XnCallbackHandle        _userReentryHandle;
    XnCallbackHandle        _calibrationStartHandle;
    XnCallbackHandle        _calibrationCompleteHandle;
    XnCallbackHandle        _poseDetectedHandle;
    XnCallbackHandle        _outOfPoseHandle;
    
    XnStatus                _rc;
    
    bool                    _userGeneratorCreated;
    bool                    _userCallbacksRegistered;
    
    double                  _smoothing;
    
};

#endif
#endif

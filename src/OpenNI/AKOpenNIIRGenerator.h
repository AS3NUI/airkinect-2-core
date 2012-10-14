//
//  AKOpenNIIRGenerator.h
//  KinectExtension
//
//  Created by Wouter Verweirder on 14/10/12.
//
//

#ifndef __KinectExtension__AKOpenNIIRGenerator__
#define __KinectExtension__AKOpenNIIRGenerator__
#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_OPENNI

#include "OpenNIDevice.h"
#include <XnCppWrapper.h>

class OpenNIDevice;

class AKOpenNIIRGenerator
{
public:
    
	AKOpenNIIRGenerator(OpenNIDevice* device);
	~AKOpenNIIRGenerator();
    
    void                    setup(xn::Context* xnContext, int sourceWidth, int sourceHeight, int frameRate);
    void                    start();
    void                    stop();
    
    bool                    update();
    void                    parse();
    
    bool                    irGeneratorCreated();
    xn::IRGenerator*        getXnIRGenerator();
    
    unsigned int*           getImageByteArray();
    int                     getSourceWidth();
    int                     getSourceHeight();
    
    bool                    isGenerating();
    
private:
    
    OpenNIDevice*           _device;
    
    xn::Context*            _xnContext;
    
    xn::IRGenerator         _irGenerator;
    xn::IRMetaData          _irMetaData;
    
    XnStatus                _rc;
    
    int                     _sourceWidth;
    int                     _sourceHeight;
    int                     _frameRate;
    
    bool                    _irGeneratorCreated;
    
    unsigned int*           _imageByteArray;
    int                     _numSourcePixels;
    
};

#endif
#endif
//
//  AKOpenNIRGBGenerator.h
//  KinectExtension
//
//  Created by Wouter Verweirder on 13/10/12.
//
//

#ifndef __KinectExtension__AKOpenNIRGBGenerator__
#define __KinectExtension__AKOpenNIRGBGenerator__

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_OPENNI

#include "OpenNIDevice.h"
#include <XnCppWrapper.h>

class OpenNIDevice;

class AKOpenNIRGBGenerator
{
public:
    
	AKOpenNIRGBGenerator(OpenNIDevice* device);
	~AKOpenNIRGBGenerator();
    
    void                    setup(xn::Context* xnContext, int sourceWidth, int sourceHeight, int frameRate);
    void                    start();
    void                    stop();
    
    bool                    update();
    void                    parse();
    
    bool                    imageGeneratorCreated();
    xn::ImageGenerator*     getXnImageGenerator();
    
    unsigned int*           getImageByteArray();
    int                     getSourceWidth();
    int                     getSourceHeight();
    
    bool                    isGenerating();
    
private:
    
    OpenNIDevice*           _device;
    
    xn::Context*            _xnContext;
    
    xn::ImageGenerator      _imageGenerator;
    xn::ImageMetaData       _imageMetaData;
    
    XnStatus                _rc;
    
    int                     _sourceWidth;
    int                     _sourceHeight;
    int                     _frameRate;
    
    bool                    _imageGeneratorCreated;
    
    unsigned int*           _imageByteArray;
    int                     _numSourcePixels;
    
};

#endif
#endif

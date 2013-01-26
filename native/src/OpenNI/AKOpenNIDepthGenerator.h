//
//  AKOpenNIDepthGenerator.h
//  KinectExtension
//
//  Created by Wouter Verweirder on 14/10/12.
//
//

#ifndef __KinectExtension__AKOpenNIDepthGenerator__
#define __KinectExtension__AKOpenNIDepthGenerator__

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_OPENNI

#include "OpenNIDevice.h"
#include <XnCppWrapper.h>

class OpenNIDevice;

class AKOpenNIDepthGenerator
{
    static const int MAX_DEPTH = 10000;
public:
    
	AKOpenNIDepthGenerator(OpenNIDevice* device);
	~AKOpenNIDepthGenerator();
    
    void                    setup(xn::Context* xnContext, int sourceWidth, int sourceHeight, int frameRate);
    void                    start();
    void                    stop();
    
    bool                    update();
    void                    parse();
    
    bool                    depthGeneratorCreated();
    xn::DepthGenerator*     getXnDepthGenerator();
    
    unsigned int*           getImageByteArray();
    unsigned short*         getDepthByteArray();
    int                     getSourceWidth();
    int                     getSourceHeight();
    
    bool                    isGenerating();
    
    void                    updateUserIndexColors(bool showUserColors, float** userIndexColors, xn::SceneMetaData* sceneMetaData);
    
private:
    
    OpenNIDevice*           _device;
    
    xn::Context*            _xnContext;
    
    xn::DepthGenerator      _depthGenerator;
    xn::DepthMetaData       _depthMetaData;
    
    XnStatus                _rc;
    
    int                     _sourceWidth;
    int                     _sourceHeight;
    int                     _frameRate;
    
    bool                    _depthGeneratorCreated;
    
    unsigned int*           _imageByteArray;
    unsigned short*         _depthByteArray;
    int                     _numSourcePixels;
    
    bool                    _showUserColors;
	float**                 _userIndexColors;
    xn::SceneMetaData*      _sceneMetaData;
    
    float                   _depthHistogram[MAX_DEPTH];
    
    void                    calculateHistogram();
    
};

#endif

#endif

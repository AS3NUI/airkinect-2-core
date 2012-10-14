//
//  AKOpenNIRGBGenerator.cpp
//  KinectExtension
//
//  Created by Wouter Verweirder on 13/10/12.
//
//

#include "AKOpenNIRGBGenerator.h"
#ifdef AIRKINECT_TARGET_OPENNI

AKOpenNIRGBGenerator::AKOpenNIRGBGenerator(OpenNIDevice* device)
{
    _device = device;
    
    _imageGenerator = NULL;
    _imageGeneratorCreated = false;
    
    _imageByteArray = 0;
}

AKOpenNIRGBGenerator::~AKOpenNIRGBGenerator()
{
    stop();
}

void AKOpenNIRGBGenerator::setup(xn::Context* xnContext, int sourceWidth, int sourceHeight, int frameRate)
{
    _xnContext = xnContext;
    _sourceWidth = sourceWidth;
    _sourceHeight = sourceHeight;
    _frameRate = frameRate;
    
    _numSourcePixels = _sourceWidth * _sourceHeight;
    _imageByteArray = new unsigned int[_numSourcePixels];
    
    XnMapOutputMode rgbMode;
    rgbMode.nXRes = _sourceWidth;
    rgbMode.nYRes = _sourceHeight;
    rgbMode.nFPS = _frameRate;
    
    _device->dispatchInfoMessage((const uint8_t*) "Creating Image Generator");
    
    _rc = _imageGenerator.Create(*_xnContext);
    if(_rc != XN_STATUS_OK)
    {
        _device->dispatchErrorMessage((const uint8_t*) "Create ImageGenerator Failed");
    }
    else
    {
        _imageGeneratorCreated = true;
        _imageGenerator.SetMapOutputMode(rgbMode);
        _imageGenerator.GetMirrorCap().SetMirror(false);
    }
}

void AKOpenNIRGBGenerator::start()
{
    if(_imageGeneratorCreated)
    {
        _device->dispatchInfoMessage((const uint8_t*) "Starting Image Generator");
        _rc = _imageGenerator.StartGenerating();
        if(_rc != XN_STATUS_OK)
        {
            _device->dispatchErrorMessage((const uint8_t*) "Starting Image Generator Failed");
        }
    }
}

void AKOpenNIRGBGenerator::stop()
{
    if(_imageGeneratorCreated && _imageGenerator.IsValid())
    {
        if(_imageGenerator.IsGenerating())
        {
            _imageGenerator.StopGenerating();
        }
        _imageGenerator.Release();
        _imageGenerator = NULL;
    }
    
    delete [] _imageByteArray;
    _imageByteArray = 0;
}

bool AKOpenNIRGBGenerator::update()
{
    if(isGenerating())
    {
        _rc = _imageGenerator.WaitAndUpdateData();
        if (_rc != XN_STATUS_OK)
        {
            _device->dispatchErrorMessage((const uint8_t*) "RGB Read Failed");
            return false;
        }
        _imageGenerator.GetMetaData(_imageMetaData);
    }
    return true;
}

void AKOpenNIRGBGenerator::parse()
{
    if(isGenerating())
    {
        unsigned int* imageByteArrayRun = _imageByteArray;
    
        const XnRGB24Pixel* rgbBufferRun = _imageMetaData.RGB24Data();
        for(int i = 0; i < _numSourcePixels; i++)
        {
            *imageByteArrayRun = 0xff << 24 | ((*rgbBufferRun).nBlue + ((*rgbBufferRun).nGreen << 8) + ((*rgbBufferRun).nRed << 16));
            imageByteArrayRun++;
            rgbBufferRun++;
        }
    }
}

//GET / SET

bool AKOpenNIRGBGenerator::imageGeneratorCreated()
{
    return _imageGeneratorCreated;
}

bool AKOpenNIRGBGenerator::isGenerating()
{
    return (_imageGeneratorCreated && _imageGenerator.IsGenerating());
}

xn::ImageGenerator* AKOpenNIRGBGenerator::getXnImageGenerator()
{
    return &_imageGenerator;
}

unsigned int* AKOpenNIRGBGenerator::getImageByteArray()
{
    return _imageByteArray;
}

int AKOpenNIRGBGenerator::getSourceWidth()
{
    return _sourceWidth;
}

int AKOpenNIRGBGenerator::getSourceHeight()
{
    return _sourceHeight;
}
#endif
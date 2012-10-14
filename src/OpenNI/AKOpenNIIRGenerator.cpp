//
//  AKOpenNIRGBGenerator.cpp
//  KinectExtension
//
//  Created by Wouter Verweirder on 13/10/12.
//
//

#include "AKOpenNIIRGenerator.h"

AKOpenNIIRGenerator::AKOpenNIIRGenerator(OpenNIDevice* device)
{
    _device = device;
    
    _irGenerator = NULL;
    _irGeneratorCreated = false;
    
    _imageByteArray = 0;
}

AKOpenNIIRGenerator::~AKOpenNIIRGenerator()
{
    stop();
}

void AKOpenNIIRGenerator::setup(xn::Context* xnContext, int sourceWidth, int sourceHeight, int frameRate)
{
    _xnContext = xnContext;
    _sourceWidth = sourceWidth;
    _sourceHeight = sourceHeight;
    _frameRate = frameRate;
    
    _numSourcePixels = _sourceWidth * _sourceHeight;
    _imageByteArray = new unsigned int[_numSourcePixels];
    
    XnMapOutputMode irMode;
    irMode.nXRes = _sourceWidth;
    irMode.nYRes = _sourceHeight;
    irMode.nFPS = _frameRate;
    
    _device->dispatchInfoMessage((const uint8_t*) "Creating IR Generator");
    
    _rc = _irGenerator.Create(*_xnContext);
    if(_rc != XN_STATUS_OK)
    {
        _device->dispatchErrorMessage((const uint8_t*) "Create IR Failed");
    }
    else
    {
        _irGeneratorCreated = true;
        _irGenerator.SetMapOutputMode(irMode);
        _irGenerator.GetMirrorCap().SetMirror(false);
    }
}

void AKOpenNIIRGenerator::start()
{
    if(_irGeneratorCreated)
    {
        _device->dispatchInfoMessage((const uint8_t*) "Starting IR Generator");
        _rc = _irGenerator.StartGenerating();
        if(_rc != XN_STATUS_OK)
        {
            _device->dispatchErrorMessage((const uint8_t*) "Starting IR Generator Failed");
        }
    }
}

void AKOpenNIIRGenerator::stop()
{
    if(_irGeneratorCreated && _irGenerator.IsValid())
    {
        if(_irGenerator.IsGenerating())
        {
            _irGenerator.StopGenerating();
        }
        _irGenerator.Release();
        _irGenerator = NULL;
    }
    
    delete [] _imageByteArray;
    _imageByteArray = 0;
}

bool AKOpenNIIRGenerator::update()
{
    if(isGenerating())
    {
        _rc = _irGenerator.WaitAndUpdateData();
        if (_rc != XN_STATUS_OK)
        {
            _device->dispatchErrorMessage((const uint8_t*) "IR Read Failed");
            return false;
        }
        _irGenerator.GetMetaData(_irMetaData);
    }
    return true;
}

void AKOpenNIIRGenerator::parse()
{
    if(isGenerating())
    {
        unsigned int* imageByteArrayRun = _imageByteArray;
        const XnIRPixel* irBufferRun = _irMetaData.Data();
        for(int i = 0; i < _numSourcePixels; i++)
        {
            unsigned short intensity = *irBufferRun;
        
            *imageByteArrayRun = 0xff << 24 | intensity <<16 | intensity << 8| intensity;
        
            imageByteArrayRun++;
            irBufferRun++;
        }
    }
}

//GET / SET

bool AKOpenNIIRGenerator::irGeneratorCreated()
{
    return _irGeneratorCreated;
}

bool AKOpenNIIRGenerator::isGenerating()
{
    return (_irGeneratorCreated && _irGenerator.IsGenerating());
}

xn::IRGenerator* AKOpenNIIRGenerator::getXnIRGenerator()
{
    return &_irGenerator;
}

unsigned int* AKOpenNIIRGenerator::getImageByteArray()
{
    return _imageByteArray;
}

int AKOpenNIIRGenerator::getSourceWidth()
{
    return _sourceWidth;
}

int AKOpenNIIRGenerator::getSourceHeight()
{
    return _sourceHeight;
}
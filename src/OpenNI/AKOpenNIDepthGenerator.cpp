//
//  AKOpenNIDepthGenerator.cpp
//  KinectExtension
//
//  Created by Wouter Verweirder on 14/10/12.
//
//

#include "AKOpenNIDepthGenerator.h"

AKOpenNIDepthGenerator::AKOpenNIDepthGenerator(OpenNIDevice* device)
{
    _device = device;
    
    _depthGenerator = NULL;
    _depthGeneratorCreated = false;
    
    _imageByteArray = 0;
    _depthByteArray = 0;
}

AKOpenNIDepthGenerator::~AKOpenNIDepthGenerator()
{
    stop();
}

void AKOpenNIDepthGenerator::setup(xn::Context* xnContext, int sourceWidth, int sourceHeight, int frameRate)
{
    _xnContext = xnContext;
    _sourceWidth = sourceWidth;
    _sourceHeight = sourceHeight;
    _frameRate = frameRate;
    
    _numSourcePixels = _sourceWidth * _sourceHeight;
    _imageByteArray = new unsigned int[_numSourcePixels];
    _depthByteArray = new unsigned short[_numSourcePixels];
    
    XnMapOutputMode depthMode;
    depthMode.nXRes = _sourceWidth;
    depthMode.nYRes = _sourceHeight;
    depthMode.nFPS = _frameRate;
    
    _device->dispatchInfoMessage((const uint8_t*) "Creating Depth Generator");
    
    _rc = _depthGenerator.Create(*_xnContext);
    if(_rc != XN_STATUS_OK)
    {
        _device->dispatchErrorMessage((const uint8_t*) "Create Depth Generator Failed");
    }
    else
    {
        _depthGeneratorCreated = true;
        _depthGenerator.SetMapOutputMode(depthMode);
        _depthGenerator.GetMirrorCap().SetMirror(false);
    }
}

void AKOpenNIDepthGenerator::start()
{
    if(_depthGeneratorCreated)
    {
        _device->dispatchInfoMessage((const uint8_t*) "Starting Depth Generator");
        _rc = _depthGenerator.StartGenerating();
        if(_rc != XN_STATUS_OK)
        {
            _device->dispatchErrorMessage((const uint8_t*) "Starting Depth Generator Failed");
        }
    }
}

void AKOpenNIDepthGenerator::stop()
{
    if(_depthGeneratorCreated && _depthGenerator.IsValid())
    {
        if(_depthGenerator.IsGenerating())
        {
            _depthGenerator.StopGenerating();
        }
        _depthGenerator.Release();
        _depthGenerator = NULL;
    }
    
    delete [] _imageByteArray;
    _imageByteArray = 0;
    
    delete [] _depthByteArray;
    _depthByteArray = 0;
}

bool AKOpenNIDepthGenerator::update()
{
    if(isGenerating())
    {
        _rc = _depthGenerator.WaitAndUpdateData();
        if (_rc != XN_STATUS_OK)
        {
            _device->dispatchErrorMessage((const uint8_t*) "Depth Read Failed");
            return false;
        }
        _depthGenerator.GetMetaData(_depthMetaData);
    }
    return true;
}

void AKOpenNIDepthGenerator::parse()
{
    if(isGenerating())
    {
        calculateHistogram();
        
        const XnDepthPixel* depthFrameBuffer = _depthMetaData.Data();
        
        const XnLabel* pSceneBuffer;
        if(_showUserColors)
        {
            pSceneBuffer = _sceneMetaData->Data();
        }
        
        float value;
        int red, green, blue;
        
        unsigned int* depthImageRun = _imageByteArray;
        unsigned short* depthRun = _depthByteArray;
        
        for(int i = 0; i < _numSourcePixels; i++)
        {
            value = 0;
            
            if (*depthFrameBuffer != 0)
            {
                value = _depthHistogram[*depthFrameBuffer];
            }
            
            if(_showUserColors)
            {
                short label = *pSceneBuffer;
                
                if(label == 0)
                {
                    red = ((int) value) << 16;
                    green = ((int) value) << 8;
                    blue = ((int) value);
                }
                else
                {
                    if(_userIndexColors[label][3] == 1)
                    {
                        red = ((int) (value * _userIndexColors[label][0])) << 16;
                        green = ((int) (value * _userIndexColors[label][1])) << 8;
                        blue = ((int) (value * _userIndexColors[label][2]));
                    }
                    else
                    {
                        red = ((int) (0xFF && _userIndexColors[label][0])) << 16;
                        green = ((int) (0xFF && _userIndexColors[label][1])) << 8;
                        blue = ((int) (0xFF && _userIndexColors[label][2]));
                    }
                }
                
                pSceneBuffer++;
            }
            else
            {
                red = ((int) (value * 1)) << 16;
                green = ((int) (value * 1)) << 8;
                blue = ((int) (value * 1));
            }
            
            *depthImageRun = 0xff << 24 | (red + green + blue);
            *depthRun = *depthFrameBuffer;
            
            depthFrameBuffer++;
            depthImageRun++;
            depthRun++;
        }
    }
}

void AKOpenNIDepthGenerator::calculateHistogram()
{
	const XnDepthPixel* depthFrameBuffer = _depthMetaData.Data();
    
    xnOSMemSet(_depthHistogram, 0, MAX_DEPTH*sizeof(float));
    
    unsigned int nNumberOfPoints = 0;
	for(int i = 0; i < _numSourcePixels; i++)
	{
		if (*depthFrameBuffer != 0)
        {
            _depthHistogram[*depthFrameBuffer]++;
            nNumberOfPoints++;
        }
		depthFrameBuffer++;
	}
    for (int nIndex=1; nIndex<MAX_DEPTH; nIndex++)
    {
        _depthHistogram[nIndex] += _depthHistogram[nIndex-1];
    }
    if (nNumberOfPoints)
    {
        for (int nIndex=1; nIndex<MAX_DEPTH; nIndex++)
        {
            _depthHistogram[nIndex] = (float) ((unsigned int)(256.0 * (1.0f - (_depthHistogram[nIndex] / nNumberOfPoints))));
        }
    }
}

//GET / SET

bool AKOpenNIDepthGenerator::depthGeneratorCreated()
{
    return _depthGeneratorCreated;
}

bool AKOpenNIDepthGenerator::isGenerating()
{
    return (_depthGeneratorCreated && _depthGenerator.IsGenerating());
}

xn::DepthGenerator* AKOpenNIDepthGenerator::getXnDepthGenerator()
{
    return &_depthGenerator;
}

unsigned int* AKOpenNIDepthGenerator::getImageByteArray()
{
    return _imageByteArray;
}

unsigned short* AKOpenNIDepthGenerator::getDepthByteArray()
{
    return _depthByteArray;
}

int AKOpenNIDepthGenerator::getSourceWidth()
{
    return _sourceWidth;
}

int AKOpenNIDepthGenerator::getSourceHeight()
{
    return _sourceHeight;
}

void AKOpenNIDepthGenerator::updateUserIndexColors(bool showUserColors, float** userIndexColors, xn::SceneMetaData* sceneMetaData)
{
    _showUserColors = showUserColors;
    _userIndexColors = userIndexColors;
    _sceneMetaData = sceneMetaData;
}
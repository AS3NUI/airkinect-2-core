#include "AKOpenNIDepthParser.h"
#ifdef AIRKINECT_TARGET_OPENNI

#include <math.h>

AKOpenNIDepthParser::AKOpenNIDepthParser()
{
	_width = 0;
	_height = 0;
	_numPixels = 0;
	_imageByteArray = 0;
	_depthByteArray = 0;
}

AKOpenNIDepthParser::~AKOpenNIDepthParser()
{
	if(_imageByteArray != 0)
		delete [] _imageByteArray;
	_imageByteArray = 0;

	if(_depthByteArray != 0)
		delete [] _depthByteArray;
	_depthByteArray = 0;
}

void AKOpenNIDepthParser::setImageSize(int width, int height)
{
	if(_width != width || _height != height)
	{
		_width = width;
		_height = height;
		_numPixels = _width * _height;
		updateByteArrayLengths();
	}
}
int AKOpenNIDepthParser::getWidth()
{
	return _width;
}
int AKOpenNIDepthParser::getHeight()
{
	return _height;
}
int AKOpenNIDepthParser::getNumPixels()
{
	return _numPixels;
}

unsigned int* AKOpenNIDepthParser::getImageByteArray()
{
	return _imageByteArray;
}

unsigned short* AKOpenNIDepthParser::getDepthByteArray()
{
	return _depthByteArray;
}

void AKOpenNIDepthParser::updateByteArrayLengths()
{
	updateImageByteArrayLength();
	updateDepthByteArrayLength();
}

void AKOpenNIDepthParser::updateImageByteArrayLength()
{
	if(_imageByteArray != 0)
		delete [] _imageByteArray;
	_imageByteArray = new unsigned int[_numPixels];
}

void AKOpenNIDepthParser::updateDepthByteArrayLength()
{
	if(_depthByteArray != 0)
		delete [] _depthByteArray;
	_depthByteArray = new unsigned short[_numPixels];
}

void AKOpenNIDepthParser::setDepthMetaData(xn::DepthMetaData* depthMetaData)
{
	_depthMetaData = depthMetaData;
}

void AKOpenNIDepthParser::setSceneMetaData(xn::SceneMetaData* sceneMetaData)
{
	_sceneMetaData = sceneMetaData;
}

void AKOpenNIDepthParser::setShowUserColors(bool showUserColors)
{
	_showUserColors = showUserColors;
}

void AKOpenNIDepthParser::setUserIndexColors(float** userIndexColors)
{
	_userIndexColors = userIndexColors;
}

void AKOpenNIDepthParser::parseData()
{
	calculateHistogram();

	const XnDepthPixel* depthFrameBuffer = _depthMetaData->Data();

	const XnLabel* pSceneBuffer;
	if(_showUserColors)
	{
		pSceneBuffer = _sceneMetaData->Data();
	}

	float value;
	int red, green, blue;

	unsigned int* depthImageRun = _imageByteArray;
	unsigned short* depthRun = _depthByteArray;

	for(int i = 0; i < _numPixels; i++)
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

void AKOpenNIDepthParser::calculateHistogram()
{
	const XnDepthPixel* depthFrameBuffer = _depthMetaData->Data();
    
    xnOSMemSet(_depthHistogram, 0, MAX_DEPTH*sizeof(float));
    
    unsigned int nNumberOfPoints = 0;
	for(int i = 0; i < _numPixels; i++)
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

#endif
/*
 * Copyright 2012 AS3NUI
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#include "AKMSSDKRGBParser.h"
#ifdef AIRKINECT_TARGET_MSSDK

AKMSSDKRGBParser::AKMSSDKRGBParser()
{
	_width = 0;
	_height = 0;
	_numPixels = 0;
	_imageByteArray = 0;

	_nuiSensor = 0;
	_rgbFrameHandle = 0;
	_imageFrameTimeout = 0;
}

AKMSSDKRGBParser::~AKMSSDKRGBParser()
{
	if(_imageByteArray != 0)
		delete [] _imageByteArray;
	_imageByteArray = 0;

	_nuiSensor = 0;
	_rgbFrameHandle = 0;
	_imageFrameTimeout = 0;
}

void AKMSSDKRGBParser::setImageSize(int width, int height)
{
	if(_width != width || _height != height)
	{
		_width = width;
		_height = height;
		_numPixels = _width * _height;
		updateImageByteArrayLength();
	}
}
int AKMSSDKRGBParser::getWidth()
{
	return _width;
}
int AKMSSDKRGBParser::getHeight()
{
	return _height;
}
int AKMSSDKRGBParser::getNumPixels()
{
	return _numPixels;
}

unsigned int* AKMSSDKRGBParser::getImageByteArray()
{
	return _imageByteArray;
}

void AKMSSDKRGBParser::updateImageByteArrayLength()
{
	if(_imageByteArray != 0)
		delete [] _imageByteArray;
	_imageByteArray = new unsigned int[_numPixels];
}

void AKMSSDKRGBParser::setNuiSensor(INuiSensor* nuiSensor)
{
	_nuiSensor = nuiSensor;
}

void AKMSSDKRGBParser::setRGBFrameHandle(HANDLE rgbFrameHandle)
{
	_rgbFrameHandle = rgbFrameHandle;
}

void AKMSSDKRGBParser::setImageFrameTimeout(int imageFrameTimeout)
{
	_imageFrameTimeout = imageFrameTimeout;
}

void AKMSSDKRGBParser::parseData()
{
	unsigned int* imageByteArrayRun = _imageByteArray;

	NUI_IMAGE_FRAME rgbImageFrame;
	HRESULT hr = _nuiSensor->NuiImageStreamGetNextFrame(_rgbFrameHandle, _imageFrameTimeout, &rgbImageFrame );
	if (FAILED(hr)) 
		return;
	INuiFrameTexture * pTexture = rgbImageFrame.pFrameTexture;
	NUI_LOCKED_RECT LockedRect;
	pTexture->LockRect( 0, &LockedRect, NULL, 0 );
	if( LockedRect.Pitch != 0 ) 
	{
		int numRGBBytes = _numPixels * 4;
		for( int x = 0 ; x < numRGBBytes ; x+=4 ) 
		{
			//from BGR to RGB
			*imageByteArrayRun = 0xff << 24 | LockedRect.pBits[x+2] << 16 | LockedRect.pBits[x+1] << 8 | LockedRect.pBits[x];
			imageByteArrayRun++;
		}
		_nuiSensor->NuiImageStreamReleaseFrame(_rgbFrameHandle, &rgbImageFrame );
	}
}
#endif
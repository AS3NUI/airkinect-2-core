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
#include "AKMSSDKInfraredParser.h"
#ifdef AIRKINECT_TARGET_MSSDK

#include <math.h>

AKMSSDKInfraredParser::AKMSSDKInfraredParser()
{
	_width = 0;
	_height = 0;
	_numPixels = 0;
	_imageByteArray = 0;

	_nuiSensor = 0;
}

AKMSSDKInfraredParser::~AKMSSDKInfraredParser()
{
	if(_imageByteArray != 0)
		delete [] _imageByteArray;
	_imageByteArray = 0;

	_nuiSensor = 0;
}

void AKMSSDKInfraredParser::setImageSize(int width, int height)
{
	if(_width != width || _height != height)
	{
		_width = width;
		_height = height;
		_numPixels = _width * _height;
		updateByteArrayLengths();
	}
}
int AKMSSDKInfraredParser::getWidth()
{
	return _width;
}
int AKMSSDKInfraredParser::getHeight()
{
	return _height;
}
int AKMSSDKInfraredParser::getNumPixels()
{
	return _numPixels;
}

unsigned int* AKMSSDKInfraredParser::getImageByteArray()
{
	return _imageByteArray;
}

void AKMSSDKInfraredParser::updateByteArrayLengths()
{
	updateImageByteArrayLength();
}

void AKMSSDKInfraredParser::updateImageByteArrayLength()
{
	if(_imageByteArray != 0)
		delete [] _imageByteArray;
	_imageByteArray = new unsigned int[_numPixels];
}

void AKMSSDKInfraredParser::setNuiSensor(INuiSensor* nuiSensor)
{
	_nuiSensor = nuiSensor;
}

void AKMSSDKInfraredParser::setInfraredFrameHandle(HANDLE infraredFrameHandle)
{
	_infraredFrameHandle = infraredFrameHandle;
}

void AKMSSDKInfraredParser::setInfraredFrameTimeout(int infraredFrameTimeout)
{
	_infraredFrameTimeout = infraredFrameTimeout;
}

void AKMSSDKInfraredParser::parseData()
{
	NUI_IMAGE_FRAME infraredImageFrame;
	HRESULT hr = _nuiSensor->NuiImageStreamGetNextFrame(_infraredFrameHandle, _infraredFrameTimeout, &infraredImageFrame );
	if(FAILED(hr)) return;

	INuiFrameTexture * pTexture = infraredImageFrame.pFrameTexture;
	NUI_LOCKED_RECT LockedRect;
	pTexture->LockRect( 0, &LockedRect, NULL, 0 );

	if( LockedRect.Pitch != 0 ) {
		unsigned int* infraredImageRun = _imageByteArray;

		for( int x = 0 ; x < _numPixels ; x++ ) {
			BYTE intensity = reinterpret_cast<USHORT*>(LockedRect.pBits)[x] >> 8;

			*infraredImageRun = 0xff << 24 | intensity <<16 | intensity << 8| intensity;
			infraredImageRun++;
		}
		//release the frame
		_nuiSensor->NuiImageStreamReleaseFrame(_infraredFrameHandle, &infraredImageFrame);
	}
}

#endif
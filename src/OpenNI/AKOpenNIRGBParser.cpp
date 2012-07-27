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
#include "AKOpenNIRGBParser.h"
#ifdef AIRKINECT_TARGET_OPENNI

AKOpenNIRGBParser::AKOpenNIRGBParser()
{
	_imageByteArray = 0;
}

AKOpenNIRGBParser::~AKOpenNIRGBParser()
{
	if(_imageByteArray != 0)
		delete [] _imageByteArray;
}

void AKOpenNIRGBParser::setImageSize(int width, int height)
{
	if(_width != width || _height != height)
	{
		_width = width;
		_height = height;
		_numPixels = _width * _height;
		updateImageByteArrayLength();
	}
}
int AKOpenNIRGBParser::getWidth()
{
	return _width;
}
int AKOpenNIRGBParser::getHeight()
{
	return _height;
}
int AKOpenNIRGBParser::getNumPixels()
{
	return _numPixels;
}

unsigned int* AKOpenNIRGBParser::getImageByteArray()
{
	return _imageByteArray;
}

void AKOpenNIRGBParser::updateImageByteArrayLength()
{
	if(_imageByteArray != 0)
		delete [] _imageByteArray;
	_imageByteArray = new unsigned int[_numPixels];
}

void AKOpenNIRGBParser::setImageMetaData(xn::ImageMetaData* imageMetaData)
{
	_imageMetaData = imageMetaData;
}

void AKOpenNIRGBParser::parseData()
{
	unsigned int* imageByteArrayRun = _imageByteArray;

	const XnRGB24Pixel* rgbBufferRun = (*_imageMetaData).RGB24Data();
	for(int i = 0; i < _numPixels; i++)
	{
		*imageByteArrayRun = 0xff << 24 | ((*rgbBufferRun).nBlue + ((*rgbBufferRun).nGreen << 8) + ((*rgbBufferRun).nRed << 16));
        imageByteArrayRun++;
        rgbBufferRun++;
	}
}
#endif
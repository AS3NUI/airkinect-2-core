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
#ifndef KinectExtension_AKOpenNIRGBParser_h
#define KinectExtension_AKOpenNIRGBParser_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_OPENNI

#include <XnCppWrapper.h>

class AKOpenNIRGBParser
{
public:

	AKOpenNIRGBParser();
	~AKOpenNIRGBParser();

	void setImageSize(int width, int height);
	int getWidth();
	int getHeight();
	int getNumPixels();

	void setImageMetaData(xn::ImageMetaData* imageMetaData);
	void parseData();

	unsigned int* getImageByteArray();

private:
	xn::ImageMetaData* _imageMetaData;

	int _width;
	int _height;
	int _numPixels;

	unsigned int* _imageByteArray;

	void updateImageByteArrayLength();
};

#endif
#endif
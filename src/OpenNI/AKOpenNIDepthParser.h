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
#ifndef KinectExtension_AKOpenNIDepthParser_h
#define KinectExtension_AKOpenNIDepthParser_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_OPENNI

#include <XnCppWrapper.h>

class AKOpenNIDepthParser
{
	static const int MAX_DEPTH = 10000;
public:

	AKOpenNIDepthParser();
	~AKOpenNIDepthParser();

	void setImageSize(int width, int height);
	int getWidth();
	int getHeight();
	int getNumPixels();

	void setDepthMetaData(xn::DepthMetaData* depthMetaData);
	void setSceneMetaData(xn::SceneMetaData* sceneMetaData);
	void setShowUserColors(bool showUserColors);
	void setUserIndexColors(float** userIndexColors);

	void parseData();

	unsigned int* getImageByteArray();
	unsigned short* getDepthByteArray();

private:

	xn::DepthMetaData* _depthMetaData;
	xn::SceneMetaData* _sceneMetaData;
	float _depthHistogram[MAX_DEPTH];
	bool _showUserColors;
	float** _userIndexColors;

	int _width;
	int _height;
	int _numPixels;

	unsigned int* _imageByteArray;
	unsigned short* _depthByteArray;

	void updateByteArrayLengths();

	void updateImageByteArrayLength();
	void updateDepthByteArrayLength();

	void calculateHistogram();

};

#endif
#endif
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
#ifndef KinectExtension_AKMSSDKDepthParser_h
#define KinectExtension_AKMSSDKDepthParser_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_MSSDK

class AKMSSDKDepthParser
{
	static const int			DEPTH_MIN = 800;
	static const int			DEPTH_MAX = 4000;
	static const int			DEPTH_RANGE = DEPTH_MAX - DEPTH_MIN;
public:

	AKMSSDKDepthParser();
	~AKMSSDKDepthParser();

	void setImageSize(int width, int height);
	int getWidth();
	int getHeight();
	int getNumPixels();

	void setNuiSensor(INuiSensor* nuiSensor);
	void setDepthFrameHandle(HANDLE depthFrameHandle);
	void setDepthFrameTimeout(int depthFrameTimeout);
	void setUserIndexColors(BYTE* userIndexColors);

	void parseData();

	unsigned int* getImageByteArray();
	unsigned short* getDepthByteArray();
	unsigned short* getSceneByteArray();

private:

	INuiSensor* _nuiSensor;
	HANDLE _depthFrameHandle;
	int	_depthFrameTimeout;
	BYTE* _userIndexColors;

	int _width;
	int _height;
	int _numPixels;

	unsigned int* _imageByteArray;
	unsigned short* _depthByteArray;
	unsigned short* _sceneByteArray;

	void updateByteArrayLengths();

	void updateImageByteArrayLength();
	void updateDepthByteArrayLength();
	void updateSceneByteArrayLength();

	RGBQUAD ShortToQuad_Depth(unsigned short s);

};

#endif
#endif
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
#ifndef KinectExtension_AKMSSDKPointCloudGenerator_h
#define KinectExtension_AKMSSDKPointCloudGenerator_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_MSSDK

#include "../AKPointCloudGenerator.h"

class AKMSSDKPointCloudGenerator : public AKPointCloudGenerator
{
public:

	AKMSSDKPointCloudGenerator();
	~AKMSSDKPointCloudGenerator();

	void setSourceDepthSize(int width, int height);
	void setSourceRGBSize(int width, int height);
	void setNuiSensor(INuiSensor* nuiSensor);
	void generateTargetBytes();

private:

	INuiSensor* _nuiSensor;

	NUI_IMAGE_RESOLUTION _sourceDepthResolution;
	NUI_IMAGE_RESOLUTION _sourceRGBResolution;
};

#endif
#endif
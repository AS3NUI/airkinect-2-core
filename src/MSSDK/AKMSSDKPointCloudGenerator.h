#ifndef KinectExtension_AKMSSDKPointCloudGenerator_h
#define KinectExtension_AKMSSDKPointCloudGenerator_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_MSSDK

#include "../AKPointCloudGenerator.h"

class AKMSSDKPointCloudGenerator : public AKPointCloudGenerator
{
public:

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
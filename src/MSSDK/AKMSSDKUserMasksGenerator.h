#ifndef KinectExtension_AKMSSDKUserMasksGenerator_h
#define KinectExtension_AKMSSDKUserMasksGenerator_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_MSSDK

#include "../AKUserMasksGenerator.h"

class AKMSSDKUserMasksGenerator : public AKUserMasksGenerator
{
public:

	void setSourceDepthSize(int width, int height);
	void setSourceRGBSize(int width, int height);
	void setNuiSensor(INuiSensor* nuiSensor);
	void setSourceSceneBytes(unsigned short* sourceSceneBytes);

	void generateTargetBytes();

protected:

	unsigned short* _sourceSceneBytes;

	INuiSensor* _nuiSensor;

	NUI_IMAGE_RESOLUTION _sourceDepthResolution;
	NUI_IMAGE_RESOLUTION _sourceRGBResolution;

};

#endif
#endif
#ifndef KinectExtension_AKOpenNIPointCloudGenerator_h
#define KinectExtension_AKOpenNIPointCloudGenerator_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_OPENNI

#include "../AKPointCloudGenerator.h"

class AKOpenNIPointCloudGenerator : public AKPointCloudGenerator
{
public:

	void generateTargetBytes();

private:

};

#endif
#endif
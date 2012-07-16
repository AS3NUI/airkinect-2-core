#ifndef KinectExtension_AKOpenNIUserMasksGenerator_h
#define KinectExtension_AKOpenNIUserMasksGenerator_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_OPENNI

#include "../AKUserMasksGenerator.h"
#include <XnCppWrapper.h>

class AKOpenNIUserMasksGenerator : public AKUserMasksGenerator
{
public:

	void setSceneMetaData(xn::SceneMetaData* sceneMetaData);

	void generateTargetBytes();

protected:

	xn::SceneMetaData* _sceneMetaData;
	
};

#endif
#endif
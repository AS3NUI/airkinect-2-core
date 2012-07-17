#ifndef KinectExtension_AKUserFrameGenerator_h
#define KinectExtension_AKUserFrameGenerator_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include <stdlib.h>
#include "AKUserFrame.h"

class AKUserFrameGenerator
{
public:

	AKUserFrameGenerator();
	~AKUserFrameGenerator();

	virtual void setRGBTargetMirrored(bool rgbTargetMirrored);
	virtual void setRGBTargetSize(int width, int height);
	virtual void setRGBSourceSize(int width, int height);

	virtual void setDepthTargetMirrored(bool depthTargetMirrored);
	virtual void setDepthTargetSize(int width, int height);
	virtual void setDepthSourceSize(int width, int height);

	virtual void setSkeletonTrackingEnabled(bool skeletonTrackingEnabled);
	virtual void setSkeletonMirrored(bool skeletonMirrored);

	virtual int getMaxUsers();
	virtual void generateUserFrame();
	virtual FREObject getFREObject();

protected:

	bool _rgbTargetMirrored;
	bool _depthTargetMirrored;

	int _rgbSourceWidth;
	int _rgbSourceHeight;

	int _rgbTargetWidth;
	int _rgbTargetHeight;

	int _depthTargetWidth;
	int _depthTargetHeight;

	int _depthSourceWidth;
	int _depthSourceHeight;

	bool _skeletonTrackingEnabled;
	bool _skeletonMirrored;

	int _numJoints;
	int _maxSkeletons;
	char** _jointNames;

	AKUserFrame _userFrame;
};

#endif
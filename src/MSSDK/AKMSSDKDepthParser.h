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
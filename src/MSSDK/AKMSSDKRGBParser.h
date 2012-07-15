#ifndef KinectExtension_AKMSSDKRGBParser_h
#define KinectExtension_AKKMSSDKRGBParser_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_MSSDK

class AKMSSDKRGBParser
{
public:

	AKMSSDKRGBParser();
	~AKMSSDKRGBParser();

	void setImageSize(int width, int height);
	int getWidth();
	int getHeight();
	int getNumPixels();

	void setNuiSensor(INuiSensor* nuiSensor);
	void setRGBFrameHandle(HANDLE rgbFrameHandle);
	void setImageFrameTimeout(int imageFrameTimeout);
	void parseData();

	unsigned int* getImageByteArray();

private:

	INuiSensor* _nuiSensor;
	HANDLE _rgbFrameHandle;
	int	_imageFrameTimeout;

	int _width;
	int _height;
	int _numPixels;

	unsigned int* _imageByteArray;

	void updateImageByteArrayLength();

};

#endif
#endif
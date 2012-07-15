#ifndef KinectExtension_AKOpenNIRGBParser_h
#define KinectExtension_AKOpenNIRGBParser_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_OPENNI

#include <XnCppWrapper.h>

class AKOpenNIRGBParser
{
public:

	AKOpenNIRGBParser();
	~AKOpenNIRGBParser();

	void setImageSize(int width, int height);
	int getWidth();
	int getHeight();
	int getNumPixels();

	void setImageMetaData(xn::ImageMetaData* imageMetaData);
	void parseData();

	unsigned int* getImageByteArray();

private:
	xn::ImageMetaData* _imageMetaData;

	int _width;
	int _height;
	int _numPixels;

	unsigned int* _imageByteArray;

	void updateImageByteArrayLength();
};

#endif
#endif
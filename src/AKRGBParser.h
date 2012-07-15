#ifndef KinectExtension_AKRGBParser_h
#define KinectExtension_AKRGBParser_h

#include "ExtensionConfig.h"

class AKRGBParser
{
public:
	AKRGBParser();
	~AKRGBParser();

	void setImageSize(int width, int height);
	int getWidth();
	int getHeight();
	int getNumPixels();
	
	void parseData();
	unsigned int* getImageByteArray();

protected:

	int _width;
	int _height;
	int _numPixels;

	unsigned int* _imageByteArray;

	void updateImageByteArrayLength();

};

#endif
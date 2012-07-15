#include "AKOpenNIRGBParser.h"
#ifdef AIRKINECT_TARGET_OPENNI

AKOpenNIRGBParser::AKOpenNIRGBParser()
{
	_imageByteArray = 0;
}

AKOpenNIRGBParser::~AKOpenNIRGBParser()
{
	if(_imageByteArray != 0)
		delete [] _imageByteArray;
}

void AKOpenNIRGBParser::setImageSize(int width, int height)
{
	if(_width != width || _height != height)
	{
		_width = width;
		_height = height;
		_numPixels = _width * _height;
		updateImageByteArrayLength();
	}
}
int AKOpenNIRGBParser::getWidth()
{
	return _width;
}
int AKOpenNIRGBParser::getHeight()
{
	return _height;
}
int AKOpenNIRGBParser::getNumPixels()
{
	return _numPixels;
}

unsigned int* AKOpenNIRGBParser::getImageByteArray()
{
	return _imageByteArray;
}

void AKOpenNIRGBParser::updateImageByteArrayLength()
{
	if(_imageByteArray != 0)
		delete [] _imageByteArray;
	_imageByteArray = new unsigned int[_numPixels];
}

void AKOpenNIRGBParser::setImageMetaData(xn::ImageMetaData* imageMetaData)
{
	_imageMetaData = imageMetaData;
}

void AKOpenNIRGBParser::parseData()
{
	unsigned int* imageByteArrayRun = _imageByteArray;

	const XnRGB24Pixel* rgbBufferRun = (*_imageMetaData).RGB24Data();
	for(int i = 0; i < _numPixels; i++)
	{
		*imageByteArrayRun = 0xff << 24 | ((*rgbBufferRun).nBlue + ((*rgbBufferRun).nGreen << 8) + ((*rgbBufferRun).nRed << 16));
        imageByteArrayRun++;
        rgbBufferRun++;
	}
}
#endif
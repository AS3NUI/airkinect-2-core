#include "AKRGBParser.h"

AKRGBParser::AKRGBParser()
{
	_imageByteArray = 0;
}

AKRGBParser::~AKRGBParser()
{
	if(_imageByteArray != 0)
		delete [] _imageByteArray;
}

void AKRGBParser::setImageSize(int width, int height)
{
	if(_width != width || _height != height)
	{
		_width = width;
		_height = height;
		_numPixels = _width * _height;
		updateImageByteArrayLength();
	}
}
int AKRGBParser::getWidth()
{
	return _width;
}
int AKRGBParser::getHeight()
{
	return _height;
}
int AKRGBParser::getNumPixels()
{
	return _numPixels;
}

unsigned int* AKRGBParser::getImageByteArray()
{
	return _imageByteArray;
}

void AKRGBParser::updateImageByteArrayLength()
{
	if(_imageByteArray != 0)
		delete [] _imageByteArray;
	_imageByteArray = new unsigned int[_numPixels];
}

void AKRGBParser::parseData()
{
}
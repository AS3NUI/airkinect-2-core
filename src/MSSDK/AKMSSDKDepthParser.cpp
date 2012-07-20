#include "AKMSSDKDepthParser.h"
#ifdef AIRKINECT_TARGET_MSSDK

#include <math.h>

AKMSSDKDepthParser::AKMSSDKDepthParser()
{
	_width = 0;
	_height = 0;
	_numPixels = 0;
	_imageByteArray = 0;
	_depthByteArray = 0;
	_sceneByteArray = 0;

	_nuiSensor = 0;
	_depthFrameHandle = 0;
	_depthFrameTimeout = 0;
	_userIndexColors = 0;
}

AKMSSDKDepthParser::~AKMSSDKDepthParser()
{
	if(_imageByteArray != 0)
		delete [] _imageByteArray;
	_imageByteArray = 0;

	if(_depthByteArray != 0)
		delete [] _depthByteArray;
	_depthByteArray = 0;

	if(_sceneByteArray != 0)
		delete [] _sceneByteArray;
	_sceneByteArray = 0;

	_nuiSensor = 0;
	_depthFrameHandle = 0;
	_depthFrameTimeout = 0;
	_userIndexColors = 0;
}

void AKMSSDKDepthParser::setImageSize(int width, int height)
{
	if(_width != width || _height != height)
	{
		_width = width;
		_height = height;
		_numPixels = _width * _height;
		updateByteArrayLengths();
	}
}
int AKMSSDKDepthParser::getWidth()
{
	return _width;
}
int AKMSSDKDepthParser::getHeight()
{
	return _height;
}
int AKMSSDKDepthParser::getNumPixels()
{
	return _numPixels;
}

unsigned int* AKMSSDKDepthParser::getImageByteArray()
{
	return _imageByteArray;
}

unsigned short* AKMSSDKDepthParser::getDepthByteArray()
{
	return _depthByteArray;
}

unsigned short* AKMSSDKDepthParser::getSceneByteArray()
{
	return _sceneByteArray;
}

void AKMSSDKDepthParser::updateByteArrayLengths()
{
	updateImageByteArrayLength();
	updateDepthByteArrayLength();
	updateSceneByteArrayLength();
}

void AKMSSDKDepthParser::updateImageByteArrayLength()
{
	if(_imageByteArray != 0)
		delete [] _imageByteArray;
	_imageByteArray = new unsigned int[_numPixels];
}

void AKMSSDKDepthParser::updateDepthByteArrayLength()
{
	if(_depthByteArray != 0)
		delete [] _depthByteArray;
	_depthByteArray = new unsigned short[_numPixels];
}

void AKMSSDKDepthParser::updateSceneByteArrayLength()
{
	if(_sceneByteArray != 0)
		delete [] _sceneByteArray;
	_sceneByteArray = new unsigned short[_numPixels];
}

void AKMSSDKDepthParser::setNuiSensor(INuiSensor* nuiSensor)
{
	_nuiSensor = nuiSensor;
}

void AKMSSDKDepthParser::setDepthFrameHandle(HANDLE depthFrameHandle)
{
	_depthFrameHandle = depthFrameHandle;
}

void AKMSSDKDepthParser::setDepthFrameTimeout(int depthFrameTimeout)
{
	_depthFrameTimeout = depthFrameTimeout;
}

void AKMSSDKDepthParser::setUserIndexColors(BYTE* userIndexColors)
{
	_userIndexColors = userIndexColors;
}

void AKMSSDKDepthParser::parseData()
{
	NUI_IMAGE_FRAME depthImageFrame;
	HRESULT hr = _nuiSensor->NuiImageStreamGetNextFrame(_depthFrameHandle, _depthFrameTimeout, &depthImageFrame );
	if(FAILED(hr)) return;

	INuiFrameTexture * pTexture = depthImageFrame.pFrameTexture;
	NUI_LOCKED_RECT LockedRect;
	pTexture->LockRect( 0, &LockedRect, NULL, 0 );

	if( LockedRect.Pitch != 0 ) {
		unsigned int* depthImageRun = _imageByteArray;
		unsigned short* depthRun = _depthByteArray;
		unsigned short* sceneRun = _sceneByteArray;

		USHORT depth;
		RGBQUAD quad;
		int numDepthBytes = _numPixels *2;
		for( int x = 0 ; x < numDepthBytes ; x+=2 ) {
			//depth info
			depth = (unsigned short) (LockedRect.pBits[x] | (LockedRect.pBits[x+1] << 8));
			*depthRun = depth;
			depthRun++;
			//scene info
			*sceneRun = (unsigned short) LockedRect.pBits[x] & 7;
			sceneRun++;
			//depth image
			quad = ShortToQuad_Depth(depth);
			* depthImageRun = 0xff << 24 | quad.rgbReserved << 24 | quad.rgbRed <<16 | quad.rgbGreen << 8| quad.rgbBlue;
			depthImageRun++;
		}
		//release the frame
		_nuiSensor->NuiImageStreamReleaseFrame(_depthFrameHandle, &depthImageFrame );
	}
}

RGBQUAD AKMSSDKDepthParser::ShortToQuad_Depth(unsigned short s)
{
	RGBQUAD color;
	USHORT realDepth = NuiDepthPixelToDepth(s);
	USHORT playerIndex = NuiDepthPixelToPlayerIndex(s);

	if(playerIndex == 0){
		color.rgbRed = color.rgbGreen = color.rgbBlue = realDepth >> 4;
	}else{
		float depthRatio = (float)(realDepth-DEPTH_MIN) / (float)(DEPTH_RANGE);
		depthRatio = abs(1 - depthRatio);
		USHORT colorIndex = (playerIndex-1) * 4;
		bool useIntensity = (USHORT) _userIndexColors[colorIndex+3] != 0;

		color.rgbRed  = useIntensity ? (USHORT)(depthRatio * _userIndexColors[colorIndex]) : _userIndexColors[colorIndex];
		color.rgbGreen = useIntensity ? (USHORT)(depthRatio * _userIndexColors[colorIndex+1]) : _userIndexColors[colorIndex+1];
		color.rgbBlue = useIntensity ? (USHORT)(depthRatio * _userIndexColors[colorIndex+2]) : _userIndexColors[colorIndex+2];
	}

	return color;
}

#endif
#include "AKMSSDKUserMasksGenerator.h"

#ifdef AIRKINECT_TARGET_MSSDK

void AKMSSDKUserMasksGenerator::setSourceDepthSize(int width, int height)
{
	AKUserMasksGenerator::setSourceDepthSize(width, height);
	_sourceDepthResolution = getResolutionFrom(width, height);
}

void AKMSSDKUserMasksGenerator::setSourceRGBSize(int width, int height)
{
	AKUserMasksGenerator::setSourceRGBSize(width, height);
	_sourceRGBResolution = getResolutionFrom(width, height);
}

void AKMSSDKUserMasksGenerator::setNuiSensor(INuiSensor* nuiSensor)
{
	_nuiSensor = nuiSensor;
}

void AKMSSDKUserMasksGenerator::setSourceSceneBytes(unsigned short* sourceSceneBytes)
{
	_sourceSceneBytes = sourceSceneBytes;
}

void AKMSSDKUserMasksGenerator::generateTargetBytes()
{
	int pixelNr = 0;
	unsigned int rgbValue;
	int rgbIndex;
	long depthX, depthY, rgbX, rgbY;
	USHORT packedDepth;
	for(int y = 0; y < _targetHeight; y++)
	{
		depthY = y * _scale;

		const unsigned short* pDepthBuffer = _sourceDepthBytes + ((y + _directionFactor) * (_sourceDepthWidth * _scale)) - _directionFactor;
		const unsigned int* pRGBBuffer = _sourceRGBBytes + ((y + _directionFactor) * (_sourceRGBWidth * _scale)) - _directionFactor;
		const unsigned short* pSceneBuffer = _sourceSceneBytes + ((y + _directionFactor) * (_sourceSceneWidth * _scale)) - _directionFactor;
		for(int x = 0; x < _targetWidth; x++)
		{
			for(int i = 0; i < _maxUsers; i++)
			{
				_targetBytes[i][pixelNr] = 0;
			}
			unsigned short userIndex = *pSceneBuffer;

			if(userIndex > 0)
			{
				depthX = (_direction * x * _scale) + (_directionFactor * _sourceDepthWidth);

				packedDepth = *pDepthBuffer;
				_nuiSensor->NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(
					_sourceRGBResolution,
					_sourceDepthResolution,
					0,
					depthX, 
					depthY, 
					packedDepth,
					&rgbX,
					&rgbY
				);
				
				rgbIndex = (rgbX + (rgbY * _sourceRGBWidth));
				rgbValue = 0;

				if(rgbIndex > _sourceRGBPixelCount)
				{
					rgbValue = 0;
				}
				else
				{
					rgbValue = _sourceRGBBytes[rgbIndex];
				}

				_targetBytes[userIndex - 1][pixelNr] = 0xff << 24 | rgbValue;
			}
			pRGBBuffer += (_scale * _direction);
			pDepthBuffer += (_scale * _direction);
			pSceneBuffer += (_scale * _direction);
			pixelNr++;
		}
	}
}

NUI_IMAGE_RESOLUTION AKMSSDKUserMasksGenerator::getResolutionFrom(int width, int height)
{
	NUI_IMAGE_RESOLUTION rtnRes = NUI_IMAGE_RESOLUTION_320x240;
	if(width == 80 && height == 60) rtnRes = NUI_IMAGE_RESOLUTION_80x60;
	if(width == 640 && height == 480) rtnRes = NUI_IMAGE_RESOLUTION_640x480;
	if(width == 1280 && height == 960) rtnRes = NUI_IMAGE_RESOLUTION_1280x960;
	return rtnRes;
}

#endif
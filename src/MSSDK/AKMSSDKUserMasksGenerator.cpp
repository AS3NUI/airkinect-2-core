/*
 * Copyright 2012 AS3NUI
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#include "AKMSSDKUserMasksGenerator.h"
#include "../AKUtilityFunctions.h"

#ifdef AIRKINECT_TARGET_MSSDK

AKMSSDKUserMasksGenerator::AKMSSDKUserMasksGenerator()
{
	_sourceSceneBytes = 0;
	_nuiSensor = 0;
}

AKMSSDKUserMasksGenerator::~AKMSSDKUserMasksGenerator()
{
	_sourceSceneBytes = 0;
	_nuiSensor = 0;
}

void AKMSSDKUserMasksGenerator::setSourceDepthSize(int width, int height)
{
	AKUserMasksGenerator::setSourceDepthSize(width, height);
	_sourceDepthResolution = getNuiImageResolutionForGivenWidthAndHeight(width, height);
}

void AKMSSDKUserMasksGenerator::setSourceRGBSize(int width, int height)
{
	AKUserMasksGenerator::setSourceRGBSize(width, height);
	_sourceRGBResolution = getNuiImageResolutionForGivenWidthAndHeight(width, height);
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

#endif
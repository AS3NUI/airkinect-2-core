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
#include "AKPointCloudGenerator.h"

AKPointCloudGenerator::AKPointCloudGenerator()
{
	_sourceDepthWidth = 0;
	_sourceDepthHeight = 0;
	_sourceDepthMirrored = false;
	_sourceDepthPixelCount = 0;

	_sourceRGBWidth = 0;
	_sourceRGBHeight = 0;
	_sourceRGBMirrored = false;
	_sourceRGBPixelCount = 0;

	_targetWidth = 0;
	_targetHeight = 0;
	_targetMirrored = false;
	_targetPixelCount = 0;
	_targetDensity = 1;

	_includeRGB = false;
	_byteArrayLength = 0;
	_targetBytes = 0;
}

AKPointCloudGenerator::~AKPointCloudGenerator()
{
	if(_targetBytes != 0)
		delete [] _targetBytes;
	_targetBytes = 0;
}

void AKPointCloudGenerator::setSourceDepthSize(int width, int height)
{
	_sourceDepthWidth = width;
	_sourceDepthHeight = height;
	_sourceDepthPixelCount = _sourceDepthWidth * _sourceDepthHeight;
	updateScale();
}

void AKPointCloudGenerator::setSourceRGBSize(int width, int height)
{
	_sourceRGBWidth = width;
	_sourceRGBHeight = height;
	_sourceRGBPixelCount = _sourceRGBWidth * _sourceRGBHeight;
}

void AKPointCloudGenerator::setTargetSize(int width, int height)
{
	_targetWidth = width;
	_targetHeight = height;
	_targetPixelCount = _targetWidth * _targetHeight;
	updateScale();
	updateTargetBytesLength();
}

void AKPointCloudGenerator::updateScale()
{
	if(_sourceDepthWidth > 0 && _targetWidth > 0)
	{
		_scale = _sourceDepthWidth / _targetWidth;
		_scaledWidth = _sourceDepthWidth * _scale;
	}
}

void AKPointCloudGenerator::setTargetMirrored(bool value)
{
	_targetMirrored = value;
	updateTargetBytesDirection();
}

void AKPointCloudGenerator::setSourceDepthMirrored(bool value)
{
	_sourceDepthMirrored = value;
	updateTargetBytesDirection();
}

void AKPointCloudGenerator::setSourceRGBMirrored(bool value)
{
	_sourceRGBMirrored = value;
}

void AKPointCloudGenerator::updateTargetBytesDirection()
{
	_direction = _targetMirrored ? 1 : -1;
	_direction *= _sourceDepthMirrored ? -1 : 1;
	_directionFactor = (_direction == -1) ? 1 : 0;
}

void AKPointCloudGenerator::setIncludeRGB(bool value)
{
	_includeRGB = value;
	updateTargetBytesLength();
}

void AKPointCloudGenerator::setTargetDensity(int targetDensity)
{
	if(targetDensity < 1) 
		targetDensity = 1;
	_targetDensity = targetDensity;
	updateTargetBytesLength();
}

int AKPointCloudGenerator::getTargetDensity()
{
	return _targetDensity;
}

void AKPointCloudGenerator::updateTargetBytesLength()
{
	if(_targetBytes != 0)
		delete [] _targetBytes;
	if(_includeRGB)
		_byteArrayLength = _targetPixelCount * 6;
	else
		_byteArrayLength = _targetPixelCount * 3;

	_byteArrayLength /= _targetDensity;

	if(_byteArrayLength > 0)
		_targetBytes = new short[_byteArrayLength];
}

int AKPointCloudGenerator::getByteArrayLength()
{
	return _byteArrayLength;
}

void AKPointCloudGenerator::setPointCloudRegions(PointCloudRegion* pointCloudRegions)
{
	_pointCloudRegions = pointCloudRegions;
}

void AKPointCloudGenerator::setNumRegions(int numRegions)
{
	_numRegions = numRegions;
}

void AKPointCloudGenerator::setSourceDepthBytes(unsigned short* sourceDepthBytes)
{
	_sourceDepthBytes = sourceDepthBytes;
}

void AKPointCloudGenerator::setSourceRGBBytes(unsigned int* sourceRGBBytes)
{
	_sourceRGBBytes = sourceRGBBytes;
}

short* AKPointCloudGenerator::getTargetBytes()
{
	return _targetBytes;
}
void AKPointCloudGenerator::generateTargetBytes()
{
}
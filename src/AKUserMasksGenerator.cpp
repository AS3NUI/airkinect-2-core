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
#include "AKUserMasksGenerator.h"

AKUserMasksGenerator::AKUserMasksGenerator()
{
	_sourceDepthWidth = 0;
	_sourceDepthHeight = 0;
	_sourceDepthMirrored = false;
	_sourceDepthPixelCount = 0;

	_sourceSceneWidth = 0;
	_sourceSceneHeight = 0;
	_sourceScenePixelCount = 0;
	_sourceSceneMirrored = false;

	_sourceRGBWidth = 0;
	_sourceRGBHeight = 0;
	_sourceRGBMirrored = false;
	_sourceRGBPixelCount = 0;

	_targetWidth = 0;
	_targetHeight = 0;
	_targetMirrored = false;
	_targetPixelCount = 0;

	_maxUsers = 1;

	_targetBytes = 0;
}

AKUserMasksGenerator::~AKUserMasksGenerator()
{
	deallocTargetBytes();
}

void AKUserMasksGenerator::setSourceDepthSize(int width, int height)
{
	_sourceDepthWidth = width;
	_sourceDepthHeight = height;
	_sourceDepthPixelCount = _sourceDepthWidth * _sourceDepthHeight;
}

void AKUserMasksGenerator::setSourceSceneSize(int width, int height)
{
	_sourceSceneWidth = width;
	_sourceSceneHeight = height;
	_sourceScenePixelCount = _sourceSceneWidth * _sourceSceneHeight;
	updateScale();
}

void AKUserMasksGenerator::setSourceRGBSize(int width, int height)
{
	_sourceRGBWidth = width;
	_sourceRGBHeight = height;
	_sourceRGBPixelCount = _sourceRGBWidth * _sourceRGBHeight;
}

void AKUserMasksGenerator::setTargetSize(int width, int height)
{
	_targetWidth = width;
	_targetHeight = height;
	_targetPixelCount = _targetWidth * _targetHeight;
	updateScale();
	updateTargetBytesLength();
}

void AKUserMasksGenerator::setMaxUsers(int maxUsers)
{
	if(maxUsers < 1)
		maxUsers = 1;
	deallocTargetBytes();
	_maxUsers = maxUsers;
	updateTargetBytesLength();
}

int AKUserMasksGenerator::getTargetPixelCount()
{
	return _targetPixelCount;
}

void AKUserMasksGenerator::updateScale()
{
	if(_sourceSceneWidth > 0 && _targetWidth > 0)
	{
		_scale = _sourceSceneWidth / _targetWidth;
		_scaledWidth = _sourceSceneWidth * _scale;
	}
}

void AKUserMasksGenerator::setTargetMirrored(bool value)
{
	_targetMirrored = value;
	updateTargetBytesDirection();
}

void AKUserMasksGenerator::setSourceDepthMirrored(bool value)
{
	_sourceDepthMirrored = value;
}

void AKUserMasksGenerator::setSourceSceneMirrored(bool value)
{
	_sourceSceneMirrored = value;
	updateTargetBytesDirection();
}

void AKUserMasksGenerator::setSourceRGBMirrored(bool value)
{
	_sourceRGBMirrored = value;
}

void AKUserMasksGenerator::updateTargetBytesDirection()
{
	_direction = _targetMirrored ? 1 : -1;
	_direction *= _sourceSceneMirrored ? -1 : 1;
	_directionFactor = (_direction == -1) ? 1 : 0;
}

void AKUserMasksGenerator::updateTargetBytesLength()
{
	deallocTargetBytes();

	if(_maxUsers > 0)
	{
		_targetBytes = new unsigned int*[_maxUsers];
		for(int i = 0; i < _maxUsers; i++)
		{
			_targetBytes[i] = new unsigned int[_targetPixelCount];
		}
	}
}

void AKUserMasksGenerator::deallocTargetBytes()
{
	if(_targetBytes != 0)
    {
		for(int i = 0; i < _maxUsers; i++)
        {
            delete [] _targetBytes[i];
        }
        delete [] _targetBytes;
    }
	_targetBytes = 0;
}

void AKUserMasksGenerator::setSourceDepthBytes(unsigned short* sourceDepthBytes)
{
	_sourceDepthBytes = sourceDepthBytes;
}

void AKUserMasksGenerator::setSourceRGBBytes(unsigned int* sourceRGBBytes)
{
	_sourceRGBBytes = sourceRGBBytes;
}

unsigned int** AKUserMasksGenerator::getTargetBytes()
{
	return _targetBytes;
}
void AKUserMasksGenerator::generateTargetBytes()
{
}
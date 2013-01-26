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
#include "AKOpenNIUserMasksGenerator.h"

#ifdef AIRKINECT_TARGET_OPENNI

void AKOpenNIUserMasksGenerator::setSceneMetaData(xn::SceneMetaData* sceneMetaData)
{
	_sceneMetaData = sceneMetaData;
}

void AKOpenNIUserMasksGenerator::generateTargetBytes()
{
	const XnLabel* sceneFrameBuffer = _sceneMetaData->Data();
    
	int pixelNr = 0;
	for(int y = 0; y < _targetHeight; y++)
	{
		const unsigned int* pRGBBuffer = _sourceRGBBytes + ((y + _directionFactor) * (_sourceRGBWidth * _scale)) - _directionFactor;
		const XnLabel* pSceneBuffer = sceneFrameBuffer + ((y + _directionFactor) * (_sourceSceneWidth * _scale)) - _directionFactor;
		for(int x = 0; x < _targetWidth; x++)
		{
			XnLabel label = *pSceneBuffer;
            
			for(int i = 0; i < _maxUsers; i++)
			{
				_targetBytes[i][pixelNr] = 0;
			}
			if(label > 0)
			{
				_targetBytes[label - 1][pixelNr] = 0xff << 24 | *pRGBBuffer;
			}
            
			pRGBBuffer += (_scale * _direction);
			pSceneBuffer += (_scale * _direction);
			pixelNr++;
		}
	}
}

#endif
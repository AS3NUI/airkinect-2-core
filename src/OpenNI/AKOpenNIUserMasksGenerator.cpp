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
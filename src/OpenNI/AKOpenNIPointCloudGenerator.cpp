#include "AKOpenNIPointCloudGenerator.h"
#ifdef AIRKINECT_TARGET_OPENNI

void AKOpenNIPointCloudGenerator::generateTargetBytes()
{
	short* pointCloudRun = _targetBytes;
    
    if(_pointCloudRegions != 0)
	{
		for(int i = 0; i < _numRegions; i++)
		{
			_pointCloudRegions[i].numPoints = 0;
		}
	}
	else
	{
		_numRegions = 0;
	}
    
    for(int y = 0; y < _targetHeight; y+=_targetDensity)
    {
		const unsigned int* pRGBBuffer = _sourceRGBBytes + ((y + _directionFactor) * (_sourceRGBWidth * _scale)) - _directionFactor;
        const unsigned short* pDepthBuffer = _sourceDepthBytes + ((y + _directionFactor) * (_sourceDepthWidth * _scale)) - _directionFactor;

        for(int x = 0; x < _targetWidth; x+=_targetDensity)
        {
            //write to point cloud
            *pointCloudRun = x;
            pointCloudRun++;
            *pointCloudRun = y;
            pointCloudRun++;
            *pointCloudRun = *pDepthBuffer;
            pointCloudRun++;

			if(_includeRGB)
			{
				unsigned int rgbValue = *pRGBBuffer;
				*pointCloudRun = (short) (0xff & (rgbValue >> 16));
				pointCloudRun++;
				*pointCloudRun = (short) (0xff & (rgbValue >> 8));
				pointCloudRun++;
				*pointCloudRun = (short) (0xff & rgbValue);
				pointCloudRun++;
			}
            
            //check regions
            for(int i = 0; i < _numRegions; i++)
            {
                if(
                   x >= _pointCloudRegions[i].x && x <= _pointCloudRegions[i].maxX &&
                   y >= _pointCloudRegions[i].y && y <= _pointCloudRegions[i].maxY &&
                   *pDepthBuffer >= _pointCloudRegions[i].z && *pDepthBuffer <= _pointCloudRegions[i].maxZ
                   )
                {
                    _pointCloudRegions[i].numPoints++;
                }
            }
            
			pRGBBuffer += (_scale * _direction * _targetDensity);
            pDepthBuffer += (_scale * _direction * _targetDensity);
        }
    }
}

#endif
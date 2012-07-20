#ifndef KinectExtension_AKPointCloudGenerator_h
#define KinectExtension_AKPointCloudGenerator_h

#include "ExtensionConfig.h"
#include "PointCloudRegion.h"

class AKPointCloudGenerator
{
public:

	AKPointCloudGenerator();
	virtual ~AKPointCloudGenerator();

	virtual void setSourceDepthSize(int width, int height);
	virtual void setSourceRGBSize(int width, int height);
	virtual void setTargetSize(int width, int height);

	virtual void setSourceDepthMirrored(bool value);
	virtual void setSourceRGBMirrored(bool value);
	virtual void setTargetMirrored(bool value);
	
	virtual void setIncludeRGB(bool value);

	virtual void setTargetDensity(int targetDensity);
	virtual int getTargetDensity();

	virtual void setPointCloudRegions(PointCloudRegion* pointCloudRegions);
	virtual void setNumRegions(int numRegions);

	virtual void setSourceDepthBytes(unsigned short* depthBytes);
	virtual void setSourceRGBBytes(unsigned int* rgbBytes);

	virtual int getByteArrayLength();

	virtual short* getTargetBytes();
	virtual void generateTargetBytes();

protected:

	int _sourceDepthWidth;
	int _sourceDepthHeight;
	int _sourceDepthPixelCount;
	bool _sourceDepthMirrored;

	int _sourceRGBWidth;
	int _sourceRGBHeight;
	int _sourceRGBPixelCount;
	bool _sourceRGBMirrored;

	int _targetPixelCount;
	int _targetWidth;
	int _targetHeight;
	bool _targetMirrored;
	int _scale;

	int _direction;
	int _directionFactor;
	int _scaledWidth;

	bool _includeRGB;
	int _targetDensity;
	int _byteArrayLength;

	PointCloudRegion* _pointCloudRegions;
    int	_numRegions;

	unsigned short* _sourceDepthBytes;
	unsigned int* _sourceRGBBytes;
	short* _targetBytes;

	virtual void updateScale();
	virtual void updateTargetBytesLength();
	virtual void updateTargetBytesDirection();
};

#endif
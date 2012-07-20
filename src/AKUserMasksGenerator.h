#ifndef KinectExtension_AKUserMasksGenerator_h
#define KinectExtension_AKUserMasksGenerator_h

#include "ExtensionConfig.h"

class AKUserMasksGenerator
{
public:

	AKUserMasksGenerator();
	virtual ~AKUserMasksGenerator();

	virtual void setSourceDepthSize(int width, int height);
	virtual void setSourceSceneSize(int width, int height);
	virtual void setSourceRGBSize(int width, int height);
	virtual void setTargetSize(int width, int height);

	virtual void setSourceSceneMirrored(bool value);
	virtual void setSourceDepthMirrored(bool value);
	virtual void setSourceRGBMirrored(bool value);
	virtual void setTargetMirrored(bool value);

	virtual void setMaxUsers(int maxUsers);
	virtual int getTargetPixelCount();

	virtual void setSourceDepthBytes(unsigned short* depthBytes);
	virtual void setSourceRGBBytes(unsigned int* rgbBytes);

	virtual unsigned int** getTargetBytes();
	virtual void generateTargetBytes();

protected:
	int _sourceDepthWidth;
	int _sourceDepthHeight;
	int _sourceDepthPixelCount;
	bool _sourceDepthMirrored;

	int _sourceSceneWidth;
	int _sourceSceneHeight;
	int _sourceScenePixelCount;
	bool _sourceSceneMirrored;

	int _sourceRGBWidth;
	int _sourceRGBHeight;
	int _sourceRGBPixelCount;
	bool _sourceRGBMirrored;

	int _maxUsers;

	int _targetPixelCount;
	int _targetWidth;
	int _targetHeight;
	bool _targetMirrored;
	int _scale;

	int _direction;
	int _directionFactor;
	int _scaledWidth;

	unsigned short* _sourceDepthBytes;
	unsigned int* _sourceRGBBytes;
	unsigned int** _targetBytes;

	virtual void updateScale();
	virtual void updateTargetBytesDirection();
	virtual void deallocTargetBytes();
	virtual void updateTargetBytesLength();
};

#endif
#include "AKImageBytesGenerator.h"

AKImageBytesGenerator::AKImageBytesGenerator()
{
	_sourceWidth = 0;
	_sourceHeight = 0;
	_sourcePixelCount = 0;
	_sourceMirrored = false;
	_targetPixelCount = 0;
	_targetWidth = 0;
	_targetHeight = 0;
	_targetMirrored = false;
	_scale = 0;

	_direction = 0;
	_directionFactor = 0;
	_scaledWidth = 0;

	_targetBytes = 0;
}

AKImageBytesGenerator::~AKImageBytesGenerator()
{
	if(_targetBytes != 0)
		delete [] _targetBytes;
	_targetBytes = 0;
}

void AKImageBytesGenerator::setSourceSize(int width, int height)
{
	_sourceWidth = width;
	_sourceHeight = height;
	_sourcePixelCount = _sourceWidth * _sourceHeight;
	updateScale();
}

void AKImageBytesGenerator::setTargetSize(int width, int height)
{
	_targetWidth = width;
	_targetHeight = height;
	_targetPixelCount = _targetWidth * _targetHeight;
	updateScale();
	updateTargetBytesLength();
}

void AKImageBytesGenerator::updateScale()
{
	if(_sourceWidth > 0 && _targetWidth > 0)
	{
		_scale = _sourceWidth / _targetWidth;
		_scaledWidth = _sourceWidth * _scale;
	}
}

void AKImageBytesGenerator::updateTargetBytesLength()
{
	if(_targetBytes != 0)
		delete [] _targetBytes;
	_targetBytes = new unsigned int[_targetPixelCount];
}

bool AKImageBytesGenerator::getTargetMirrored()
{
	return _targetMirrored;
}
void AKImageBytesGenerator::setTargetMirrored(bool value)
{
	_targetMirrored = value;
	updateTargetBytesDirection();
}

bool AKImageBytesGenerator::getSourceMirrored()
{
	return _sourceMirrored;
}
void AKImageBytesGenerator::setSourceMirrored(bool value)
{
	_sourceMirrored = value;
	updateTargetBytesDirection();
}

void AKImageBytesGenerator::updateTargetBytesDirection()
{
	_direction = _targetMirrored ? 1 : -1;
	_direction *= _sourceMirrored ? -1 : 1;
	_directionFactor = (_direction == -1) ? 1 : 0;
}

int AKImageBytesGenerator::getSourceWidth()
{
	return _sourceWidth;
}
int AKImageBytesGenerator::getSourceHeight()
{
	return _sourceHeight;
}
int AKImageBytesGenerator::getSourcePixelCount()
{
	return _sourcePixelCount;
}
int AKImageBytesGenerator::getTargetWidth()
{
	return _targetWidth;
}
int AKImageBytesGenerator::getTargetHeight()
{
	return _targetHeight;
}
int AKImageBytesGenerator::getTargetPixelCount()
{
	return _targetPixelCount;
}
int AKImageBytesGenerator::getScale()
{
	return _scale;
}
void AKImageBytesGenerator::setSourceBytes(unsigned int* sourceBytes)
{
	_sourceBytes = sourceBytes;
}
unsigned int* AKImageBytesGenerator::getTargetBytes()
{
	return _targetBytes;
}
void AKImageBytesGenerator::generateTargetBytes()
{
	for(int y = 0; y < _targetHeight; y++)
	{
		generateTargetBytesForRow(y);
	}
}
void AKImageBytesGenerator::generateTargetBytesForRow(int row)
{
	unsigned int* targetBytesRun = _targetBytes + (row * _targetWidth);
	const unsigned int* sourceBytesRun = _sourceBytes + ((row + _directionFactor) * _scaledWidth) - _directionFactor;
        
	for(int x = 0; x < _targetWidth; x++)
	{
		*targetBytesRun = *sourceBytesRun;
		targetBytesRun++;
		sourceBytesRun += (_scale * _direction);
	}
}
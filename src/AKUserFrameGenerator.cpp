#include "AKUserFrameGenerator.h"

AKUserFrameGenerator::AKUserFrameGenerator()
{
	_depthTargetMirrored = false;
	_depthTargetWidth = 0;
	_depthTargetHeight = 0;
	_depthSourceWidth = 0;
	_depthSourceHeight = 0;
	_rgbTargetMirrored = false;
	_rgbTargetWidth = 0;
	_rgbTargetHeight = 0;
	_rgbSourceWidth = 0;
	_rgbSourceHeight = 0;
	_skeletonMirrored = false;
	_skeletonTrackingEnabled = false;
}

AKUserFrameGenerator::~AKUserFrameGenerator()
{
}

void AKUserFrameGenerator::setRGBTargetMirrored(bool rgbTargetMirrored)
{
	_rgbTargetMirrored = rgbTargetMirrored;
}
void AKUserFrameGenerator::setRGBTargetSize(int width, int height)
{
	_rgbTargetWidth = width;
	_rgbTargetHeight = height;
}
void AKUserFrameGenerator::setRGBSourceSize(int width, int height)
{
	_rgbSourceWidth = width;
	_rgbSourceHeight = height;
}

void AKUserFrameGenerator::setDepthTargetMirrored(bool depthTargetMirrored)
{
	_depthTargetMirrored = depthTargetMirrored;
}
void AKUserFrameGenerator::setDepthTargetSize(int width, int height)
{
	_depthTargetWidth = width;
	_depthTargetHeight = height;
}
void AKUserFrameGenerator::setDepthSourceSize(int width, int height)
{
	_depthSourceWidth = width;
	_depthSourceHeight = height;
}

void AKUserFrameGenerator::setSkeletonTrackingEnabled(bool skeletonTrackingEnabled)
{
	_skeletonTrackingEnabled = skeletonTrackingEnabled;
}

void AKUserFrameGenerator::setSkeletonMirrored(bool skeletonMirrored)
{
	_skeletonMirrored = skeletonMirrored;
}

int AKUserFrameGenerator::getMaxUsers()
{
	return _maxSkeletons;
}

void AKUserFrameGenerator::generateUserFrame()
{
}

FREObject AKUserFrameGenerator::getFREObject()
{
	return NULL;
}
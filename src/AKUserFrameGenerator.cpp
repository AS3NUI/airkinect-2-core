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
	deallocateUserFrame();
}

void AKUserFrameGenerator::allocateUserFrame()
{
	_userFrame = new AKUserFrame();
	_userFrame->asUserFrameClass = _asUserFrameClass;
	_userFrame->users = new AKUser[_maxSkeletons];
	_userFrame->jointNames = _jointNames;
	_userFrame->maxSkeletons = _maxSkeletons;
	for(int i = 0; i < _maxSkeletons; i++)
	{
		_userFrame->users[i].asUserClass = _asUserClass;
		_userFrame->users[i].framework = _framework;
		_userFrame->users[i].skeletonJoints = new AKSkeletonJoint[_numJoints];
		_userFrame->users[i].jointNames = _jointNames;
		_userFrame->users[i].numJoints = _numJoints;
		for(int j = 0; j < _numJoints; j++)
		{
			_userFrame->users[i].skeletonJoints[j].asJointClass = _asJointClass;
			_userFrame->users[i].skeletonJoints[j].jointName = _jointNames[j];
		}
	}
}

void AKUserFrameGenerator::deallocateUserFrame()
{
	if(_userFrame->users != 0)
	{
		for(int i = 0; i < _maxSkeletons; i++)
		{
			delete [] _userFrame->users[i].skeletonJoints;
			_userFrame->users[i].skeletonJoints = 0;
		}
		delete [] _userFrame->users;
		_userFrame->users = 0;
	}
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
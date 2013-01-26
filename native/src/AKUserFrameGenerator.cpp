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
	_userFrame->users = new AKUser[_maxSkeletons];
	_userFrame->maxSkeletons = _maxSkeletons;
	for(int i = 0; i < _maxSkeletons; i++)
	{
		_userFrame->users[i].skeletonJoints = new AKSkeletonJoint[_numJoints];
		_userFrame->users[i].numJoints = _numJoints;
		for(int j = 0; j < _numJoints; j++)
		{
			_userFrame->users[i].skeletonJoints[j].jointNameIndex = j;
		}
		_userFrame->users[i].skeletonBones = new AKSkeletonBone[_numBones];
		_userFrame->users[i].numBones = _numBones;
		for(int j = 0; j < _numBones; j++)
		{
			_userFrame->users[i].skeletonBones[j].boneNameIndex = j;
		}
	}
}

void AKUserFrameGenerator::deallocateUserFrame()
{
	if(_userFrame != 0)
	{
		if(_userFrame->users != 0)
		{
			for(int i = 0; i < _maxSkeletons; i++)
			{
				delete [] _userFrame->users[i].skeletonJoints;
				_userFrame->users[i].skeletonJoints = 0;

				delete [] _userFrame->users[i].skeletonBones;
				_userFrame->users[i].skeletonBones = 0;
			}
			delete [] _userFrame->users;
			_userFrame->users = 0;
		}
		delete _userFrame;
		_userFrame = 0;
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

FREObject AKUserFrameGenerator::freGetSkeletonJointNameIndices()
{
	FREObject skeletonJointNameIndices, jointIndex;
	FRENewObject( (const uint8_t*) "flash.utils.Dictionary", 0, NULL, &skeletonJointNameIndices, NULL);
	for(int i = 0; i < _numJoints; i++)
	{
		FRENewObjectFromUint32(i, &jointIndex);
		FRESetObjectProperty(skeletonJointNameIndices, (const uint8_t*) _jointNames[i], jointIndex, NULL);
	}
	return skeletonJointNameIndices;
}

FREObject AKUserFrameGenerator::freGetSkeletonJointNames()
{ 
	FREObject skeletonJointNames;
	FRENewObject( (const uint8_t*) "Vector.<String>", 0, NULL, &skeletonJointNames, NULL);
	for(int i = 0; i < _numJoints; i++)
	{
		FRESetArrayElementAt(skeletonJointNames, i, createFREObjectForUTF8(_jointNames[i]));
	}
	return skeletonJointNames;
}

FREObject AKUserFrameGenerator::freGetSkeletonBoneNameIndices()
{
	FREObject skeletonBoneNameIndices, boneIndex;
	FRENewObject( (const uint8_t*) "flash.utils.Dictionary", 0, NULL, &skeletonBoneNameIndices, NULL);
	for(int i = 0; i < _numBones; i++)
	{
		FRENewObjectFromUint32(i, &boneIndex);
		FRESetObjectProperty(skeletonBoneNameIndices, (const uint8_t*) _boneNames[i], boneIndex, NULL);
	}
	return skeletonBoneNameIndices;
}

FREObject AKUserFrameGenerator::freGetSkeletonBoneNames()
{ 
	FREObject skeletonBoneNames;
	FRENewObject( (const uint8_t*) "Vector.<String>", 0, NULL, &skeletonBoneNames, NULL);
	for(int i = 0; i < _numBones; i++)
	{
		FRESetArrayElementAt(skeletonBoneNames, i, createFREObjectForUTF8(_boneNames[i]));
	}
	return skeletonBoneNames;
}
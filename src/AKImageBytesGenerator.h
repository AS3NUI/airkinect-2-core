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
#ifndef KinectExtension_AKImageBytesGenerator_h
#define KinectExtension_AKImageBytesGenerator_h

class AKImageBytesGenerator
{
public:
	AKImageBytesGenerator();
	~AKImageBytesGenerator();

	void setSourceSize(int width, int height);
	void setTargetSize(int width, int height);

	bool getSourceMirrored();
	void setSourceMirrored(bool value);

	bool getTargetMirrored();
	void setTargetMirrored(bool value);

	int getSourceWidth();
	int getSourceHeight();
	int getSourcePixelCount();
	int getTargetWidth();
	int getTargetHeight();
	int getTargetPixelCount();
	int getScale();

	void setSourceBytes(unsigned int* sourceBytes);
	unsigned int* getTargetBytes();

	void generateTargetBytes();


private:
	int _sourceWidth;
	int _sourceHeight;
	int _sourcePixelCount;
	bool _sourceMirrored;
	int _targetPixelCount;
	int _targetWidth;
	int _targetHeight;
	bool _targetMirrored;
	int _scale;

	int _direction;
	int _directionFactor;
	int _scaledWidth;

	unsigned int* _sourceBytes;
	unsigned int* _targetBytes;

	void generateTargetBytesForRow(int row);

	void updateScale();
	void updateTargetBytesLength();
	void updateTargetBytesDirection();
};

#endif
#ifndef KinectExtension_AKOpenNIDepthParser_h
#define KinectExtension_AKOpenNIDepthParser_h

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_OPENNI

#include <XnCppWrapper.h>

class AKOpenNIDepthParser
{
	static const int MAX_DEPTH = 10000;
public:

	AKOpenNIDepthParser();
	~AKOpenNIDepthParser();

	void setImageSize(int width, int height);
	int getWidth();
	int getHeight();
	int getNumPixels();

	void setDepthMetaData(xn::DepthMetaData* depthMetaData);
	void setSceneMetaData(xn::SceneMetaData* sceneMetaData);
	void setShowUserColors(bool showUserColors);
	void setUserIndexColors(float** userIndexColors);

	void parseData();

	unsigned int* getImageByteArray();
	unsigned short* getDepthByteArray();

private:

	xn::DepthMetaData* _depthMetaData;
	xn::SceneMetaData* _sceneMetaData;
	float _depthHistogram[MAX_DEPTH];
	bool _showUserColors;
	float** _userIndexColors;

	int _width;
	int _height;
	int _numPixels;

	unsigned int* _imageByteArray;
	unsigned short* _depthByteArray;

	void updateByteArrayLengths();

	void updateImageByteArrayLength();
	void updateDepthByteArrayLength();

	void calculateHistogram();

};

#endif
#endif
//
//  AKOpenNIInfraredParser.h
//  KinectExtension
//
//  Created by Wouter Verweirder on 08/10/12.
//
//

#ifndef __KinectExtension__AKOpenNIInfraredParser__
#define __KinectExtension__AKOpenNIInfraredParser__

#include "../ExtensionConfig.h"
#ifdef AIRKINECT_TARGET_OPENNI

#include <XnCppWrapper.h>

class AKOpenNIInfraredParser
{
public:
    
	AKOpenNIInfraredParser();
	~AKOpenNIInfraredParser();
    
	void setImageSize(int width, int height);
	int getWidth();
	int getHeight();
	int getNumPixels();
    
	void setInfraredMetaData(xn::IRMetaData* infraredMetaData);
	void parseData();
    
	unsigned int* getImageByteArray();
    
private:
	xn::IRMetaData* _infraredMetaData;
    
	int _width;
	int _height;
	int _numPixels;
    
	unsigned int* _imageByteArray;
    
	void updateImageByteArrayLength();
};


#endif

#endif /* defined(__KinectExtension__AKOpenNIInfraredParser__) */

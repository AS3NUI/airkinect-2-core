#include "ExtensionConfig.h"
#include "AKUtilityFunctions.h"
#include <stdlib.h>

unsigned int createUnsignedIntFromFREObject(FREObject freObject)
{
	unsigned int value; 
	FREGetObjectAsUint32(freObject, &value);
	return value;
}

bool createBoolFromFREObject(FREObject freObject)
{
	unsigned int value;
	FREGetObjectAsBool(freObject, &value);
    return (value != 0);
}

#ifdef AIRKINECT_TARGET_MSSDK
NUI_IMAGE_RESOLUTION getNuiImageResolutionForGivenWidthAndHeight(int width, int height){
	NUI_IMAGE_RESOLUTION rtnRes = NUI_IMAGE_RESOLUTION_320x240;
	if(width == 80 && height == 60) rtnRes = NUI_IMAGE_RESOLUTION_80x60;
	if(width == 640 && height == 480) rtnRes = NUI_IMAGE_RESOLUTION_640x480;
	if(width == 1280 && height == 960) rtnRes = NUI_IMAGE_RESOLUTION_1280x960;
	return rtnRes;
}
#endif
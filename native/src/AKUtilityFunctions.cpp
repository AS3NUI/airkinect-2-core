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
#include "ExtensionConfig.h"
#include "AKUtilityFunctions.h"
#include <stdlib.h>
#include <string.h>

unsigned int createUnsignedIntFromFREObject(FREObject freObject)
{
	unsigned int value; 
	FREGetObjectAsUint32(freObject, &value);
	return value;
}

double createDoubleFromFREObject(FREObject freObject)
{
	double value;
	FREGetObjectAsDouble(freObject, &value);
	return value;
}

bool createBoolFromFREObject(FREObject freObject)
{
	unsigned int value;
	FREGetObjectAsBool(freObject, &value);
    return (value != 0);
}

FREObject createFREObjectForUTF8(const char* str)
{
	FREObject fre;
	if(str != 0)
		FRENewObjectFromUTF8(strlen(str), (const uint8_t*) str, &fre);
	return fre;
}

FREObject createFREObjectForUnsignedInt(unsigned int i)
{
	FREObject fre;
	FRENewObjectFromUint32(i, &fre);
	return fre;
}

FREObject createFREObjectForDouble(double d)
{
	FREObject fre;
	FRENewObjectFromDouble(d, &fre);
	return fre;
}

FREObject createFREObjectForBool(bool b)
{
	FREObject fre;
	FRENewObjectFromBool((b) ? 1 : 0, &fre);
	return fre;
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
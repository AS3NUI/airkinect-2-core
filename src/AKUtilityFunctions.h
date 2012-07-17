#ifndef KinectExtension_AKUtilityFunctions_h
#define KinectExtension_AKUtilityFunctions_h

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

unsigned int createUnsignedIntFromFREObject(FREObject freObject);
bool createBoolFromFREObject(FREObject freObject);

#ifdef AIRKINECT_TARGET_MSSDK
NUI_IMAGE_RESOLUTION getNuiImageResolutionForGivenWidthAndHeight(int width, int height);
#endif

#endif
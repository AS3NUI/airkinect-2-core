#ifndef KinectExtension_AKOpenNIUserFrame_h
#define KinectExtension_AKOpenNIUserFrame_h

#include "../../ExtensionConfig.h"

#ifdef AIRKINECT_TARGET_OPENNI

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include "../../Data/AKUserFrame.h"
#include "AKOpenNIUser.h"

#ifndef _AKOpenNIUserFrame_
#define _AKOpenNIUserFrame_
typedef struct _AKOpenNIUserFrame
{
    AKUserFrame* userFrame;
	AKOpenNIUser* openNIUsers;

} AKOpenNIUserFrame;
#endif _AKOpenNIUserFrame_

#endif

#endif
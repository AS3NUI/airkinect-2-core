#ifndef KinectExtension_AKUserFrame_h
#define KinectExtension_AKUserFrame_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include "AKBasicStructs.h"
#include "AKUser.h"

#ifndef _AKUserFrame_
#define _AKUserFrame_
typedef struct _AKUserFrame
{
    int frameNumber;
    int timeStamp;

	AKUser* users;

} AKUserFrame;
#endif _AKUserFrame_

#endif
#ifndef KinectExtension_AKMSSDKUserFrame_h
#define KinectExtension_AKMSSDKUserFrame_h

#include "../../ExtensionConfig.h"

#ifdef AIRKINECT_TARGET_MSSDK

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include "../../Data/AKUserFrame.h"
#include "AKMSSDKUser.h"

#ifndef _AKMSSDKUserFrame_
#define _AKMSSDKUserFrame_
typedef struct _AKMSSDKUserFrame
{
    AKUserFrame* userFrame;
	AKMSSDKUser* mssdkUsers;
} AKMSSDKUserFrame;
#endif _AKMSSDKUserFrame_

#endif

#endif
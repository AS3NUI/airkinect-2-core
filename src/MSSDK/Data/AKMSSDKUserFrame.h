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

	FREObject asFREObject()
	{
		FREObject freUserFrame = userFrame->asFREObjectWithoutUsers();

		short int trackedSkeletons = 0;
		FREObject freUsers;
		FREGetObjectProperty(freUserFrame, (const uint8_t*) "users", &freUsers, NULL);
		for(int i = 0; i < userFrame->maxSkeletons; i++)
		{
			if(this->mssdkUsers[i].user->isTracking)
			{
				FREObject freUser = this->mssdkUsers[i].asFREObject();
				FRESetArrayElementAt(freUsers, trackedSkeletons, freUser);
				trackedSkeletons++;
			}
		}
		FRESetObjectProperty(freUserFrame, (const uint8_t*) "users", freUsers, NULL);

		return freUserFrame;
	};
} AKMSSDKUserFrame;
#endif _AKMSSDKUserFrame_

#endif

#endif
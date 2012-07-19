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

	FREObject asFREObject()
	{
		FREObject freUserFrame = userFrame->asFREObjectWithoutUsers();

		short int trackedSkeletons = 0;
		FREObject freUsers;
		FREGetObjectProperty(freUserFrame, (const uint8_t*) "users", &freUsers, NULL);
		for(int i = 0; i < userFrame->maxSkeletons; i++)
		{
			if(this->openNIUsers[i].user->isTracking)
			{
				FREObject freUser = this->openNIUsers[i].asFREObject();
				FRESetArrayElementAt(freUsers, trackedSkeletons, freUser);
				trackedSkeletons++;
			}
		}
		FRESetObjectProperty(freUserFrame, (const uint8_t*) "users", freUsers, NULL);

		return freUserFrame;
	};
} AKOpenNIUserFrame;
#endif _AKOpenNIUserFrame_

#endif

#endif
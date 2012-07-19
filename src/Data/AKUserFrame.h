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

	char** jointNames;
	const char* asUserFrameClass;
	int maxSkeletons;

	FREObject asFREObject()
	{
		FREObject freUserFrame = asFREObjectWithoutUsers();
		short int trackedSkeletons = 0;
		FREObject freUsers;
		FREGetObjectProperty(freUserFrame, (const uint8_t*) "users", &freUsers, NULL);
		for(int i = 0; i < maxSkeletons; i++)
		{
			if(this->users[i].isTracking)
			{
				//create the user object
				FREObject freUser = this->users[i].asFREObject();
            
				FRESetArrayElementAt(freUsers, trackedSkeletons, freUser);
				trackedSkeletons++;
			}
		}
		FRESetObjectProperty(freUserFrame, (const uint8_t*) "users", freUsers, NULL);
		return freUserFrame;
	};

	FREObject asFREObjectWithoutUsers()
	{
		FREObject freUserFrame, freFrameNumber, freTimestamp, freUsers;

		FRENewObject( (const uint8_t*) asUserFrameClass, 0, NULL, &freUserFrame, NULL);

		FRENewObjectFromUint32(this->frameNumber, &freFrameNumber);
		FRENewObjectFromUint32(this->timeStamp, &freTimestamp);

		FRESetObjectProperty(freUserFrame, (const uint8_t*) "frameNumber", freFrameNumber, NULL);
		FRESetObjectProperty(freUserFrame, (const uint8_t*) "timeStamp", freTimestamp, NULL);
    
		FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.User>", 0, NULL, &freUsers, NULL);
		FRESetObjectProperty(freUserFrame, (const uint8_t*) "users", freUsers, NULL);

		return freUserFrame;
	};

} AKUserFrame;
#endif _AKUserFrame_

#endif
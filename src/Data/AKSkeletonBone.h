#ifndef KinectExtension_AKSkeletonBone_h
#define KinectExtension_AKSkeletonBone_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include <string.h>
#include "AKBasicStructs.h"

#ifndef _AKSkeletonBone_
#define _AKSkeletonBone_
typedef struct _AKSkeletonBone
{
	AKOrientation orientation;

	const char* asBoneClass;
	const char* boneName;

	FREObject asFREObject()
	{
		FREObject freBone, freBoneName;
		
		FRENewObject( (const uint8_t*) this->asBoneClass, 0, NULL, &freBone, NULL);

		FRENewObjectFromUTF8(strlen(this->boneName), (const uint8_t*) this->boneName, &freBoneName);

		FRESetObjectProperty(freBone, (const uint8_t*) "name", freBoneName, NULL);
		FRESetObjectProperty(freBone, (const uint8_t*) "orientation", this->orientation.asFREObject(), NULL);

		return freBone;
	};

} AKSkeletonBone;
#endif _AKSkeletonBone_

#endif
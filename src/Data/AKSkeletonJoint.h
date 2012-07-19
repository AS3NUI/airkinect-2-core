#ifndef KinectExtension_AKSkeletonJoint_h
#define KinectExtension_AKSkeletonJoint_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include <string.h>
#include "AKBasicStructs.h"

#ifndef _AKSkeletonJoint_
#define _AKSkeletonJoint_
typedef struct _AKSkeletonJoint
{
	double positionConfidence;
	AKPosition position;

	const char* asJointClass;
	const char* jointName;

	FREObject asFREObject()
	{
		FREObject freJoint, freJointName;
		FREObject frePositionConfidence;
		
		FRENewObject( (const uint8_t*) this->asJointClass, 0, NULL, &freJoint, NULL);

		FRENewObjectFromUTF8(strlen(this->jointName), (const uint8_t*) this->jointName, &freJointName);
		FRENewObjectFromDouble(this->positionConfidence, &frePositionConfidence);

		FRESetObjectProperty(freJoint, (const uint8_t*) "name", freJointName, NULL);
		FRESetObjectProperty(freJoint, (const uint8_t*) "position", this->position.asFREObject(), NULL);
		FRESetObjectProperty(freJoint, (const uint8_t*) "positionConfidence", frePositionConfidence, NULL);

		return freJoint;
	};

} AKSkeletonJoint;
#endif _AKSkeletonJoint_

#endif
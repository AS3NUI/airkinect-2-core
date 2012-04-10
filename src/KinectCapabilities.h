#ifndef KinectExtension_KinectCapabilities_h
#define KinectExtension_KinectCapabilities_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

typedef struct {
	bool			hasCameraElevationSupport;
	bool			hasRGBCameraSupport;
	bool			hasDepthCameraSupport;
	bool			hasDepthUserSupport;
	bool			hasPointCloudSupport;
	bool			hasPointCloudRegionSupport;
	bool			hasUserMaskSupport;
	bool			hasInfraredSupport;
	bool			hasSkeletonSupport;
	bool			hasAdvancedJointSupport;
	bool			hasJointOrientationSupport;
	bool			hasJointOrientationConfidenceSupport;
	bool			hasPositionConfidenceSupport;
	bool			hasMultipleSensorSupport;
	bool			hasNearModeSupport;
    
	unsigned int	maxSensors;
    
	const char *	framework;
    
	FREObject		asFREObject() {
        FREObject capabilities;
        
        FREObject freObject;
        
        FRENewObject((const uint8_t*)"Object", 0, NULL, &capabilities, NULL);
        
        //Bools
        FRENewObjectFromBool(hasCameraElevationSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasCameraElevationSupport", freObject, NULL);
        
        FRENewObjectFromBool(hasRGBCameraSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasRGBCameraSupport", freObject, NULL);
        
        FRENewObjectFromBool(hasDepthCameraSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasDepthCameraSupport", freObject, NULL);
        
        FRENewObjectFromBool(hasDepthUserSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasDepthUserSupport", freObject, NULL);
        
        FRENewObjectFromBool(hasPointCloudSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasPointCloudSupport", freObject, NULL);
        
        FRENewObjectFromBool(hasPointCloudRegionSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasPointCloudRegionSupport", freObject, NULL);
        
        FRENewObjectFromBool(hasUserMaskSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasUserMaskSupport", freObject, NULL);
        
        FRENewObjectFromBool(hasInfraredSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasInfraredSupport", freObject, NULL);
        
        FRENewObjectFromBool(hasSkeletonSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasSkeletonSupport", freObject, NULL);
        
        FRENewObjectFromBool(hasAdvancedJointSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasAdvancedJointSupport", freObject, NULL);
        
        FRENewObjectFromBool(hasJointOrientationSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasJointOrientationSupport", freObject, NULL);
        
        FRENewObjectFromBool(hasJointOrientationConfidenceSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasJointOrientationConfidenceSupport", freObject, NULL);
        
        FRENewObjectFromBool(hasPositionConfidenceSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasPositionConfidenceSupport", freObject, NULL);
        
        FRENewObjectFromBool(hasMultipleSensorSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasMultipleSensorSupport", freObject, NULL);
        
		FRENewObjectFromBool(hasNearModeSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasNearModeSupport", freObject, NULL);
        
        //Ints
        FRENewObjectFromUint32(maxSensors, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"maxSensors", freObject, NULL);
        
        //Strings
        FRENewObjectFromUTF8(strlen(framework), (const uint8_t *) framework, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"framework", freObject, NULL);
        
        return capabilities;
    };
    
} KinectCapabilities;

#endif

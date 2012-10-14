/*
 * Copyright 2012 AS3NUI
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
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
	bool			hasSeatedSkeletonSupport;
	bool			hasChooseSkeletonsSupport;
    bool            hasHandTrackingSupport;
    
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

		FRENewObjectFromBool(hasSeatedSkeletonSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasSeatedSkeletonSupport", freObject, NULL);

		FRENewObjectFromBool(hasChooseSkeletonsSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasChooseSkeletonsSupport", freObject, NULL);
        
        FRENewObjectFromBool(hasHandTrackingSupport, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"hasHandTrackingSupport", freObject, NULL);
        
        FRENewObjectFromUint32(maxSensors, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"maxSensors", freObject, NULL);
        
        FRENewObjectFromUTF8(strlen(framework), (const uint8_t *) framework, &freObject);
        FRESetObjectProperty(capabilities, (const uint8_t*)"framework", freObject, NULL);
        
        return capabilities;
    };
    
} KinectCapabilities;

#endif

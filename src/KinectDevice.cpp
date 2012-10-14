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
#include "KinectDevice.h"
#include "AKUtilityFunctions.h"

void KinectDevice::createPointCloudGenerator()
{
}

void KinectDevice::createUserMasksGenerator()
{
}

void KinectDevice::createUserFrameGenerator()
{
}

void KinectDevice::setDefaults()
{
	started = false;
    running = false;

	asUserMirrored = false;
    asUserEnabled = false;
    asSkeletonMirrored = false;
    asSkeletonEnabled = false;
	asSeatedSkeletonEnabled = false;
	asChooseSkeletonsEnabled = false;
    asSkeletonSmoothing = 0.5;
	asDepthEnabled = false;
    asDepthShowUserColors = false;
	asNearModeEnabled = false;
    asHandTrackingMirrored = false;
    asHandTrackingEnabled = false;
	asInfraredEnabled = false;
	asRGBEnabled = false;
	asUserMaskEnabled = false;
	asPointCloudEnabled = false;

	createUserFrameGenerator();

	depthImageBytesGenerator = new AKImageBytesGenerator();
	depthImageBytesGenerator->setTargetSize(320, 240);
	depthImageBytesGenerator->setTargetMirrored(false);
	depthImageBytesGenerator->setSourceSize(640, 480);

	infraredImageBytesGenerator = new AKImageBytesGenerator();
	infraredImageBytesGenerator->setTargetSize(320, 240);
	infraredImageBytesGenerator->setTargetMirrored(false);
	infraredImageBytesGenerator->setSourceSize(640, 480);

	rgbImageBytesGenerator = new AKImageBytesGenerator();

	setRGBMode(640, 480, 640, 480, false);

	createUserMasksGenerator();

	userMasksGenerator->setTargetSize(320, 240);
	userMasksGenerator->setMaxUsers(userFrameGenerator->getMaxUsers());
	userMasksGenerator->setTargetMirrored(false);
	userMasksGenerator->setSourceDepthSize(640, 480);
	userMasksGenerator->setSourceSceneSize(640, 480);
	userMasksGenerator->setSourceRGBSize(640, 480);

	createPointCloudGenerator();
	pointCloudGenerator->setTargetSize(320, 240);
	pointCloudGenerator->setTargetMirrored(false);
	pointCloudGenerator->setTargetDensity(1);
	pointCloudGenerator->setIncludeRGB(false);
	pointCloudGenerator->setSourceDepthSize(640, 480);
	pointCloudGenerator->setSourceRGBSize(640, 480);

	pointCloudRegions = 0;
	numRegions = 0;

	chosenSkeletonIds = 0;
	numChosenSkeletons = 0;
}

void KinectDevice::dispatchErrorMessage(const uint8_t* errorMessage)
{
	FREDispatchStatusEventAsync(freContext, (const uint8_t*) "error", errorMessage);
}

void KinectDevice::dispatchInfoMessage(const uint8_t* infoMessage)
{
	FREDispatchStatusEventAsync(freContext, (const uint8_t*) "info", infoMessage);
}

void KinectDevice::dispatchStatusMessage(const uint8_t* statusMessage)
{
	FREDispatchStatusEventAsync(freContext, (const uint8_t*) "status", statusMessage);
}

void KinectDevice::trace(const uint8_t* traceMessage)
{
	FREDispatchStatusEventAsync(freContext, (const uint8_t*) "trace", traceMessage);
}


void KinectDevice::cleanupByteArrays()
{
	if(rgbImageBytesGenerator != 0) 
		delete rgbImageBytesGenerator;
	rgbImageBytesGenerator = 0;

	if(depthImageBytesGenerator != 0) 
		delete depthImageBytesGenerator;
	depthImageBytesGenerator = 0;

	if(infraredImageBytesGenerator != 0) 
		delete infraredImageBytesGenerator;
	infraredImageBytesGenerator = 0;

	if(pointCloudGenerator != 0)
		delete pointCloudGenerator;
	pointCloudGenerator = 0;

	if(userMasksGenerator != 0)
		delete userMasksGenerator;
	userMasksGenerator = 0;

	if(userFrameGenerator != 0)
		delete userFrameGenerator;
	userFrameGenerator = 0;

    if(pointCloudRegions != 0)
		delete [] pointCloudRegions;
	pointCloudRegions = 0;

	if(chosenSkeletonIds != 0)
		delete [] chosenSkeletonIds;
	chosenSkeletonIds = 0;
}

//Getter/Setters for FREContext
FREContext KinectDevice::getFreContext()
{
	return freContext;
}

void KinectDevice::setFreContext(FREContext pFreContext)
{
	freContext = pFreContext;
}

//Starts and Stops the Kinect Device
void KinectDevice::start()
{
}
void KinectDevice::stop()
{
}

//Dispose the Device from memory
void KinectDevice::dispose()
{
}
    
FREObject KinectDevice::freGetCapabilities()
{
    return capabilities.asFREObject();
}
FREObject KinectDevice::freSetUserMode(FREObject argv[])
{
	asUserMirrored = createBoolFromFREObject(argv[1]);
    return NULL;
}
FREObject KinectDevice::freSetUserColor(FREObject argv[])
{
	unsigned int userID = createUnsignedIntFromFREObject(argv[1]);
    unsigned int color = createUnsignedIntFromFREObject(argv[2]);
    bool useIntensity = createBoolFromFREObject(argv[3]);
    setUserColor(userID, color, useIntensity);
    return NULL;
}
FREObject KinectDevice::freSetUserEnabled(FREObject argv[])
{
    asUserEnabled = createBoolFromFREObject(argv[1]);
    return NULL;
}
FREObject KinectDevice::freSetSkeletonMode(FREObject argv[])
{
	asSkeletonMirrored = createBoolFromFREObject(argv[1]);
	asSeatedSkeletonEnabled = createBoolFromFREObject(argv[2]);
	asChooseSkeletonsEnabled = createBoolFromFREObject(argv[3]);
    asSkeletonSmoothing = createDoubleFromFREObject(argv[4]);
    return NULL;
}
FREObject KinectDevice::freSetSkeletonEnabled(FREObject argv[])
{
    asSkeletonEnabled = createBoolFromFREObject(argv[1]);
    return NULL;
}
FREObject KinectDevice::freChooseSkeletons(FREObject argv[])
{
	FREObject asSkeletonIds = argv[1];
	FREObject asSkeletonId;
	int skeletonId;
    
    uint32_t freNumChosenSkeletonIds;
    FREGetArrayLength(asSkeletonIds, &freNumChosenSkeletonIds);
    
	int *freChosenSkeletonIds = new int[freNumChosenSkeletonIds];
    
    for(unsigned int i = 0; i < freNumChosenSkeletonIds; i++)
    {
		FREGetArrayElementAt(asSkeletonIds, i, &asSkeletonId);
		FREGetObjectAsInt32(asSkeletonId, &skeletonId);
		freChosenSkeletonIds[i] = skeletonId;
    }

	this->chosenSkeletonIds = freChosenSkeletonIds;
	this->numChosenSkeletons = freNumChosenSkeletonIds;
    
	return NULL;
}
FREObject KinectDevice::freGetUserFrame(FREObject argv[])
{
	lockUserMutex();
	FREObject freUserFrame = userFrameGenerator->getFREObject();
    unlockUserMutex();
    return freUserFrame;
}

FREObject KinectDevice::freSetUserMaskMode(FREObject argv[])
{
	unsigned int width; FREGetObjectAsUint32(argv[1], &width);
    unsigned int height; FREGetObjectAsUint32(argv[2], &height);
    
    lockUserMaskMutex();
	userMasksGenerator->setTargetSize(width, height);
	userMasksGenerator->setTargetMirrored(createBoolFromFREObject(argv[3]));
    unlockUserMaskMutex();

    return NULL;
}
FREObject KinectDevice::freSetUserMaskEnabled(FREObject argv[])
{
    asUserMaskEnabled = createBoolFromFREObject(argv[1]);
    return NULL;
}
FREObject KinectDevice::freGetUserMaskFrame(FREObject argv[])
{
	unsigned int trackingID; FREGetObjectAsUint32(argv[1], &trackingID);
    
    if(trackingID > 0) trackingID--;
    
    const unsigned int numUserMaskBytes = userMasksGenerator->getTargetPixelCount() * 4;
    
    FREObject objectByteArray = argv[2];
    FREByteArray byteArray;			
    FREObject length;
    FRENewObjectFromUint32(numUserMaskBytes, &length);
    FRESetObjectProperty(objectByteArray, (const uint8_t*) "length", length, NULL);
    FREAcquireByteArray(objectByteArray, &byteArray);
    lockUserMaskMutex();
	memcpy(byteArray.bytes, userMasksGenerator->getTargetBytes()[trackingID], numUserMaskBytes);
    unlockUserMaskMutex();
    FREReleaseByteArray(objectByteArray);
    
    return NULL;
}
FREObject KinectDevice::freSetDepthMode(FREObject argv[])
{
	unsigned int width; FREGetObjectAsUint32(argv[1], &width);
    unsigned int height; FREGetObjectAsUint32(argv[2], &height);
    
    lockDepthMutex();
	depthImageBytesGenerator->setTargetSize(width, height);
	depthImageBytesGenerator->setTargetMirrored(createBoolFromFREObject(argv[3]));
    unlockDepthMutex();

    return NULL;
}
FREObject KinectDevice::freSetDepthEnabled(FREObject argv[])
{
    asDepthEnabled = createBoolFromFREObject(argv[1]);
    return NULL;
}
FREObject KinectDevice::freGetDepthFrame(FREObject argv[])
{
	const unsigned int numDepthBytes = depthImageBytesGenerator->getTargetPixelCount() * 4;
    
    FREObject objectByteArray = argv[1];
    FREByteArray byteArray;			
    FREObject length;
    FRENewObjectFromUint32(numDepthBytes, &length);
    FRESetObjectProperty(objectByteArray, (const uint8_t*) "length", length, NULL);
    FREAcquireByteArray(objectByteArray, &byteArray);
    lockDepthMutex();
	memcpy(byteArray.bytes, depthImageBytesGenerator->getTargetBytes(), numDepthBytes);
    unlockDepthMutex();
    FREReleaseByteArray(objectByteArray);
    
    return NULL;
}
FREObject KinectDevice::freSetDepthShowUserColors(FREObject argv[])
{
    asDepthShowUserColors = createBoolFromFREObject(argv[1]);
    return NULL;
}
FREObject KinectDevice::freSetNearModeEnabled(FREObject argv[])
{
    asNearModeEnabled = createBoolFromFREObject(argv[1]);
    return NULL;
}
FREObject KinectDevice::freSetHandTrackingMode(FREObject argv[])
{
	asHandTrackingMirrored = createBoolFromFREObject(argv[1]);
    return NULL;
}
FREObject KinectDevice::freSetHandTrackingEnabled(FREObject argv[])
{
    asHandTrackingEnabled = createBoolFromFREObject(argv[1]);
    return NULL;
}
FREObject KinectDevice::freSetRGBMode(FREObject argv[])
{
	unsigned int width; FREGetObjectAsUint32(argv[1], &width);
    unsigned int height; FREGetObjectAsUint32(argv[2], &height);
    
    lockRGBMutex();
	setRGBMode(rgbImageBytesGenerator->getSourceWidth(), rgbImageBytesGenerator->getSourceHeight(), width, height, createBoolFromFREObject(argv[3]));
    unlockRGBMutex();
    return NULL;
}
FREObject KinectDevice::freSetRGBEnabled(FREObject argv[])
{
    asRGBEnabled = createBoolFromFREObject(argv[1]);
    return NULL;
}
FREObject KinectDevice::freGetRGBFrame(FREObject argv[])
{
	const unsigned int numRGBBytes = rgbImageBytesGenerator->getTargetPixelCount() * 4;
    
    FREObject objectByteArray = argv[1];
    FREByteArray byteArray;			
    FREObject length;
    FRENewObjectFromUint32(numRGBBytes, &length);
    FRESetObjectProperty(objectByteArray, (const uint8_t*) "length", length, NULL);
    FREAcquireByteArray(objectByteArray, &byteArray);
    lockRGBMutex();
	memcpy(byteArray.bytes, rgbImageBytesGenerator->getTargetBytes(), numRGBBytes);
    unlockRGBMutex();
    FREReleaseByteArray(objectByteArray);
    
    return NULL;
}

FREObject KinectDevice::freSetInfraredMode(FREObject argv[])
{
    unsigned int width; FREGetObjectAsUint32(argv[1], &width);
    unsigned int height; FREGetObjectAsUint32(argv[2], &height);
    
    lockInfraredMutex();
	infraredImageBytesGenerator->setTargetSize(width, height);
	infraredImageBytesGenerator->setTargetMirrored(createBoolFromFREObject(argv[3]));
    unlockInfraredMutex();

    return NULL;
}

FREObject KinectDevice::freSetInfraredEnabled(FREObject argv[])
{
    unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asInfraredEnabled = (enabled != 0);
    return NULL;
}

FREObject KinectDevice::freGetInfraredFrame(FREObject argv[])
{
    const unsigned int numInfraredBytes = infraredImageBytesGenerator->getTargetPixelCount() * 4;
    
    FREObject objectByteArray = argv[1];
    FREByteArray byteArray;			
    FREObject length;
    FRENewObjectFromUint32(numInfraredBytes, &length);
    FRESetObjectProperty(objectByteArray, (const uint8_t*) "length", length, NULL);
    FREAcquireByteArray(objectByteArray, &byteArray);
    lockInfraredMutex();
	memcpy(byteArray.bytes, infraredImageBytesGenerator->getTargetBytes(), numInfraredBytes);
    unlockInfraredMutex();
    FREReleaseByteArray(objectByteArray);

	return NULL;
}
FREObject KinectDevice::freSetPointCloudMode(FREObject argv[])
{
	lockPointCloudMutex();
    
    unsigned int width; FREGetObjectAsUint32(argv[1], &width);
    unsigned int height; FREGetObjectAsUint32(argv[2], &height);
    unsigned int density; FREGetObjectAsUint32(argv[4], &density);

	pointCloudGenerator->setTargetSize(width, height);
	pointCloudGenerator->setTargetDensity(density);
	pointCloudGenerator->setIncludeRGB(createBoolFromFREObject(argv[5]));
	pointCloudGenerator->setTargetMirrored(createBoolFromFREObject(argv[3]));
    
    unlockPointCloudMutex();
    
    return NULL;
}
FREObject KinectDevice::freSetPointCloudEnabled(FREObject argv[])
{
    asPointCloudEnabled = createBoolFromFREObject(argv[1]);
    return NULL;
}
FREObject KinectDevice::freGetPointCloudFrame(FREObject argv[])
{
	FREObject objectPointsByteArray = argv[1];
    
	const unsigned int numPointBytes = pointCloudGenerator->getByteArrayLength() * sizeof(short);
	if(numPointBytes == 0) return NULL;

    lockPointCloudMutex();
    
    FREByteArray pointsByteArray;			
    FREObject pointsLength;
    FRENewObjectFromUint32(numPointBytes, &pointsLength);
    FRESetObjectProperty(objectPointsByteArray, (const uint8_t*) "length", pointsLength, NULL);
    FREAcquireByteArray(objectPointsByteArray, &pointsByteArray);
	memcpy(pointsByteArray.bytes, pointCloudGenerator->getTargetBytes(), numPointBytes);
    FREReleaseByteArray(objectPointsByteArray);
    
    //set the region information?
    FREObject asPointCloudRegions = argv[2];
    if(asPointCloudRegions != NULL && &pointCloudRegions != 0)
    {
        //loop through these actionscript regions and get the native info back
        FREObject asPointCloudRegion, asRegionId;
        FREObject asNumPoints;
        unsigned int regionId;
        
        uint32_t freNumRegions;
        FREGetArrayLength(asPointCloudRegions, &freNumRegions);
        
        for(unsigned int i = 0; i < freNumRegions; i++)
        {
            FREGetArrayElementAt(asPointCloudRegions, i, &asPointCloudRegion);
            FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "regionId", &asRegionId, NULL);
            FREGetObjectAsUint32(asRegionId, &regionId);
            //get the region with this id from the device memory
            for(unsigned int j = 0; j < numRegions; j++)
            {
                PointCloudRegion *nativeRegion = &pointCloudRegions[j];
                if(nativeRegion->regionId == regionId)
                {
                    //update the actionscript properties
                    FRENewObjectFromUint32(nativeRegion->numPoints, &asNumPoints);
                    FRESetObjectProperty(asPointCloudRegion, (const uint8_t *) "numPoints", asNumPoints, NULL);
                    break;
                }
            }
        }
    }
    
    unlockPointCloudMutex();
    
    return NULL;
}
FREObject KinectDevice::freSetPointCloudRegions(FREObject argv[])
{
	FREObject asPointCloudRegions = argv[1];
    FREObject asPointCloudRegion, asRegionId, asX, asY, asZ, asWidth, asHeight, asDepth;
    unsigned int regionId;
    double x, y, z, width, height, depth;
    
    uint32_t freNumRegions;
    FREGetArrayLength(asPointCloudRegions, &freNumRegions);
    
    PointCloudRegion *nativeRegions = new PointCloudRegion[freNumRegions];
    
    for(unsigned int i = 0; i < freNumRegions; i++)
    {
        FREGetArrayElementAt(asPointCloudRegions, i, &asPointCloudRegion);
        FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "regionId", &asRegionId, NULL);
        FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "x", &asX, NULL);
        FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "y", &asY, NULL);
        FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "z", &asZ, NULL);
        FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "width", &asWidth, NULL);
        FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "height", &asHeight, NULL);
        FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "depth", &asDepth, NULL);
        FREGetObjectAsUint32(asRegionId, &regionId);
        FREGetObjectAsDouble(asX, &x);
        FREGetObjectAsDouble(asY, &y);
        FREGetObjectAsDouble(asZ, &z);
        FREGetObjectAsDouble(asWidth, &width);
        FREGetObjectAsDouble(asHeight, &height);
        FREGetObjectAsDouble(asDepth, &depth);
        PointCloudRegion *nativeRegion = new PointCloudRegion();
        nativeRegion->setProperties(regionId, x, y, z, width, height, depth);
        nativeRegions[i] = *nativeRegion;
    }
    
    this->pointCloudRegions = nativeRegions;
    this->numRegions = freNumRegions;
    
    return NULL;
}

FREObject KinectDevice::freCameraElevationGetAngle(FREObject argv[])
{
	return NULL;
}

FREObject KinectDevice::freCameraElevationSetAngle(FREObject argv[])
{
	return NULL;
}

void KinectDevice::lockUserMutex()
{
    userMutex.lock();
}

void KinectDevice::unlockUserMutex()
{
    userMutex.unlock();
}

void KinectDevice::lockDepthMutex()
{
    depthMutex.lock();
}

void KinectDevice::unlockDepthMutex()
{
    depthMutex.unlock();
}

void KinectDevice::lockRGBMutex()
{
    rgbMutex.lock();
}

void KinectDevice::unlockRGBMutex()
{
    rgbMutex.unlock();
}

void KinectDevice::lockUserMaskMutex()
{
    userMaskMutex.lock();
}

void KinectDevice::unlockUserMaskMutex()
{
    userMaskMutex.unlock();
}

void KinectDevice::lockInfraredMutex()
{
    infraredMutex.lock();
}

void KinectDevice::unlockInfraredMutex()
{
    infraredMutex.unlock();
}

void KinectDevice::lockPointCloudMutex()
{
    pointCloudMutex.lock();
}

void KinectDevice::unlockPointCloudMutex()
{
    pointCloudMutex.unlock();
}

void KinectDevice::setUserColor(int userID, int color, bool useIntensity)
{
}

void KinectDevice::setRGBMode(int rgbWidth, int rgbHeight, int asRGBWidth, int asRGBHeight, bool asRGBMirrored)
{   
	rgbImageBytesGenerator->setSourceSize(rgbWidth, rgbHeight);
	rgbImageBytesGenerator->setTargetSize(asRGBWidth, asRGBHeight);
	rgbImageBytesGenerator->setTargetMirrored(asRGBMirrored);
}
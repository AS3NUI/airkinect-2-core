#include "KinectDevice.h"
#include "KinectSkeleton.h"
#include "FREHelperFunctions.h"

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

	depthImageBytesGenerator = new AKImageBytesGenerator();
	depthImageBytesGenerator->setTargetSize(320, 240);
	depthImageBytesGenerator->setTargetMirrored(false);
	depthImageBytesGenerator->setSourceSize(640, 480);

    asDepthEnabled = false;
    asDepthShowUserColors = false;
	asNearModeEnabled = false;

	rgbImageBytesGenerator = new AKImageBytesGenerator();

	setRGBMode(640, 480, 640, 480, false);
    asRGBEnabled = false;

	//
	// USER MASK IMAGE
	//
	asUserMaskWidth = 320;
    asUserMaskHeight = 240;
    asUserMaskPixelCount = asUserMaskWidth * asUserMaskHeight;
    asUserMaskMirrored = false;
    asUserMaskEnabled = false;
	asUserMaskByteArray = 0;

	userMaskWidth = 640;
    userMaskHeight = 480;
    userMaskPixelCount = userMaskWidth * userMaskHeight;
    userMaskScale = userMaskWidth / asUserMaskWidth;

	//
	// POINT CLOUD IMAGE
	//
	asPointCloudWidth = 320;
    asPointCloudHeight = 240;
    asPointCloudPixelCount = asPointCloudWidth * asPointCloudHeight;
    asPointCloudMirrored = false;
    asPointCloudEnabled = false;
    asPointCloudDensity = 1;
    asPointCloudIncludeRGB = false;
	asPointCloudByteArray = 0;

	pointCloudWidth = 640;
    pointCloudHeight = 480;
    pointCloudPixelCount = pointCloudWidth * pointCloudHeight;
    pointCloudScale = pointCloudWidth / asPointCloudWidth;

	pointCloudRegions = 0;
	numRegions = 0;

	chosenSkeletonIds = 0;
	numChosenSkeletons = 0;

	setNumJointsAndJointNames();
	allocateUserFrame();
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

void KinectDevice::cleanupByteArrays()
{
	if(rgbImageBytesGenerator != 0) 
		delete rgbImageBytesGenerator;
	rgbImageBytesGenerator = 0;

	if(depthImageBytesGenerator != 0) 
		delete depthImageBytesGenerator;
	depthImageBytesGenerator = 0;

    if(asPointCloudByteArray != 0) 
		delete [] asPointCloudByteArray;
	asPointCloudByteArray = 0;

	if(asUserMaskByteArray != 0)
    {
		for(int i = 0; i < maxSkeletons; i++)
        {
            delete [] asUserMaskByteArray[i];
        }
        delete [] asUserMaskByteArray;
    }
	asUserMaskByteArray = 0;

    if(pointCloudRegions != 0)
		delete [] pointCloudRegions;
	pointCloudRegions = 0;

	if(chosenSkeletonIds != 0)
		delete [] chosenSkeletonIds;
	chosenSkeletonIds = 0;
}

void KinectDevice::setNumJointsAndJointNames()
{
}

void KinectDevice::allocateUserFrame()
{
	userFrame.users = new kinectUser[maxSkeletons];
	for(int i = 0; i < maxSkeletons; i++)
	{
		userFrame.users[i].joints = new kinectSkeletonJoint[numJoints];
	}
}

void KinectDevice::deallocateUserFrame()
{
	for(int i = 0; i < maxSkeletons; i++)
	{
		delete [] userFrame.users[i].joints;
	}
	delete [] userFrame.users;
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
	unsigned int userID; FREGetObjectAsUint32(argv[1], &userID);
    unsigned int color; FREGetObjectAsUint32(argv[2], &color);
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
    
    short int trackedSkeletons = 0;
    
    FREObject freUserFrame, frameNumber, timestamp, users, user, userType, userID, trackingID, hasSkeleton, joints, joint, jointName;
    FREObject positionConfidence;
	FREObject orientationConfidence;

	FRENewObject( (const uint8_t*) asUserFrameClass, 0, NULL, &freUserFrame, NULL);

	FRENewObjectFromUint32(userFrame.frameNumber, &frameNumber);
    FRENewObjectFromUint32(userFrame.timeStamp, &timestamp);

	FRESetObjectProperty(freUserFrame, (const uint8_t*) "frameNumber", frameNumber, NULL);
	FRESetObjectProperty(freUserFrame, (const uint8_t*) "timeStamp", timestamp, NULL);
    
    FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.User>", 0, NULL, &users, NULL);
    
    for(int i = 0; i < maxSkeletons; i++)
    {
        if(userFrame.users[i].isTracking)
        {
			//create the user object
			FRENewObject( (const uint8_t*) asUserClass, 0, NULL, &user, NULL);

			FRENewObjectFromUTF8(strlen(capabilities.framework), (const uint8_t*) capabilities.framework, &userType);
			FRENewObjectFromUint32(userFrame.users[i].userID, &userID);
			FRENewObjectFromUint32(userFrame.users[i].trackingID, &trackingID);
			FRENewObjectFromBool((userFrame.users[i].hasSkeleton) ? 1 : 0, &hasSkeleton);

			FRESetObjectProperty(user, (const uint8_t*) "framework", userType, NULL);
			FRESetObjectProperty(user, (const uint8_t*) "userID", userID, NULL);
			FRESetObjectProperty(user, (const uint8_t*) "trackingID", trackingID, NULL);
			FRESetObjectProperty(user, (const uint8_t*) "position", createFREVector3D(userFrame.users[i].worldX, userFrame.users[i].worldY, userFrame.users[i].worldZ, 0.0), NULL);
			FRESetObjectProperty(user, (const uint8_t*) "positionRelative", createFREVector3D(userFrame.users[i].worldRelativeX, userFrame.users[i].worldRelativeY, userFrame.users[i].worldRelativeZ, 0.0), NULL);
			FRESetObjectProperty(user, (const uint8_t*) "rgbPosition", createFREPoint(userFrame.users[i].rgbX, userFrame.users[i].rgbY), NULL);
			FRESetObjectProperty(user, (const uint8_t*) "rgbRelativePosition", createFREPoint(userFrame.users[i].rgbRelativeX, userFrame.users[i].rgbRelativeY), NULL);
			FRESetObjectProperty(user, (const uint8_t*) "depthPosition", createFREPoint(userFrame.users[i].depthX, userFrame.users[i].depthY), NULL);
			FRESetObjectProperty(user, (const uint8_t*) "depthRelativePosition", createFREPoint(userFrame.users[i].depthRelativeX, userFrame.users[i].depthRelativeY), NULL);
			FRESetObjectProperty(user, (const uint8_t*) "hasSkeleton", hasSkeleton, NULL);

			//create the joints vector
            FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint>", 0, NULL, &joints, NULL);
            
            for(int j = 0; j < numJoints; j++)
            {

				FRENewObject( (const uint8_t*) asJointClass, 0, NULL, &joint, NULL);

                FRENewObjectFromUTF8(strlen(jointNames[j]), (const uint8_t*) jointNames[j], &jointName);
				FRENewObjectFromDouble(userFrame.users[i].joints[j].positionConfidence, &positionConfidence);
				FRENewObjectFromDouble(userFrame.users[i].joints[j].orientationConfidence, &orientationConfidence);

				FRESetObjectProperty(joint, (const uint8_t*) "name", jointName, NULL);
				FRESetObjectProperty(joint, (const uint8_t*) "position", createFREVector3D(userFrame.users[i].joints[j].worldX, userFrame.users[i].joints[j].worldY, userFrame.users[i].joints[j].worldZ, 0.0), NULL);
				FRESetObjectProperty(joint, (const uint8_t*) "positionRelative", createFREVector3D(userFrame.users[i].joints[j].worldRelativeX, userFrame.users[i].joints[j].worldRelativeY, userFrame.users[i].joints[j].worldRelativeZ, 0.0), NULL);
				FRESetObjectProperty(joint, (const uint8_t*) "rgbPosition", createFREPoint(userFrame.users[i].joints[j].rgbX, userFrame.users[i].joints[j].rgbY), NULL);
				FRESetObjectProperty(joint, (const uint8_t*) "rgbRelativePosition", createFREPoint(userFrame.users[i].joints[j].rgbRelativeX, userFrame.users[i].joints[j].rgbRelativeY), NULL);
				FRESetObjectProperty(joint, (const uint8_t*) "depthPosition", createFREPoint(userFrame.users[i].joints[j].depthX, userFrame.users[i].joints[j].depthY), NULL);
				FRESetObjectProperty(joint, (const uint8_t*) "depthRelativePosition", createFREPoint(userFrame.users[i].joints[j].depthRelativeX, userFrame.users[i].joints[j].depthRelativeY), NULL);
				FRESetObjectProperty(joint, (const uint8_t*) "positionConfidence", positionConfidence, NULL);
				FRESetObjectProperty(joint, (const uint8_t*) "absoluteOrientationMatrix", createFREMatrix3DFromKinectRotationMatrix(userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix), NULL);
				FRESetObjectProperty(joint, (const uint8_t*) "absoluteOrientationQuaternion", createFREVector3DFromKinectRotationQuaternion(userFrame.users[i].joints[j].absoluteOrientation.rotationQuaternion), NULL);
				FRESetObjectProperty(joint, (const uint8_t*) "hierarchicalOrientationMatrix", createFREMatrix3DFromKinectRotationMatrix(userFrame.users[i].joints[j].hierarchicalOrientation.rotationMatrix), NULL);
				FRESetObjectProperty(joint, (const uint8_t*) "hierarchicalOrientationQuaternion", createFREVector3DFromKinectRotationQuaternion(userFrame.users[i].joints[j].hierarchicalOrientation.rotationQuaternion), NULL);
				FRESetObjectProperty(joint, (const uint8_t*) "orientationConfidence", orientationConfidence, NULL);
                
				FRESetArrayElementAt(joints, j, joint);
            }
            
			FRESetObjectProperty(user, (const uint8_t*) "skeletonJoints", joints, NULL);
			FRESetObjectProperty(user, (const uint8_t*) "skeletonJointNameIndices", freGetSkeletonJointNameIndices(NULL), NULL);
			FRESetObjectProperty(user, (const uint8_t*) "skeletonJointNames", freGetSkeletonJointNames(NULL), NULL);
            
            FRESetArrayElementAt(users, trackedSkeletons, user);
            trackedSkeletons++;
        }
    }

	FRESetObjectProperty(freUserFrame, (const uint8_t*) "users", users, NULL);
    
    unlockUserMutex();
    
    return freUserFrame;
}

FREObject KinectDevice::freGetSkeletonJointNameIndices(FREObject argv[])
{
	FREObject skeletonJointNameIndices, jointIndex;
    FRENewObject( (const uint8_t*) "flash.utils.Dictionary", 0, NULL, &skeletonJointNameIndices, NULL);
	for(int i = 0; i < numJoints; i++)
    {
        FRENewObjectFromUint32(i, &jointIndex);
        FRESetObjectProperty(skeletonJointNameIndices, (const uint8_t*) jointNames[i], jointIndex, NULL);
    }
    return skeletonJointNameIndices;
}
FREObject KinectDevice::freGetSkeletonJointNames(FREObject argv[])
{ 
	FREObject skeletonJointNames, skeletonJointName;
	FRENewObject( (const uint8_t*) "Vector.<String>", 0, NULL, &skeletonJointNames, NULL);
	for(int i = 0; i < numJoints; i++)
	{
	FRENewObjectFromUTF8(strlen(jointNames[i]), (const uint8_t*) jointNames[i], &skeletonJointName);
	FRESetArrayElementAt(skeletonJointNames, i, skeletonJointName);
	}
	return skeletonJointNames;
}
FREObject KinectDevice::freSetUserMaskMode(FREObject argv[])
{
	unsigned int width; FREGetObjectAsUint32(argv[1], &width);
    unsigned int height; FREGetObjectAsUint32(argv[2], &height);
    
    lockUserMaskMutex();
    
    asUserMaskWidth = width;
    asUserMaskHeight = height;
    asUserMaskPixelCount = asUserMaskWidth * asUserMaskHeight;
    asUserMaskMirrored = createBoolFromFREObject(argv[3]);
    userMaskScale = userMaskWidth / asUserMaskWidth;
    
    //reset bytearray
    if(asUserMaskByteArray != 0)
    {
        for(int i = 0; i < maxSkeletons; i++)
        {
            delete [] asUserMaskByteArray[i];
        }
        delete [] asUserMaskByteArray;
    }
    asUserMaskByteArray = new uint32_t*[maxSkeletons];
    for(int i = 0; i < maxSkeletons; i++)
    {
        asUserMaskByteArray[i] = new uint32_t[asUserMaskPixelCount];
    }
    
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
    
    const unsigned int numUserMaskBytes = asUserMaskPixelCount * 4;
    
    FREObject objectByteArray = argv[2];
    FREByteArray byteArray;			
    FREObject length;
    FRENewObjectFromUint32(numUserMaskBytes, &length);
    FRESetObjectProperty(objectByteArray, (const uint8_t*) "length", length, NULL);
    FREAcquireByteArray(objectByteArray, &byteArray);
    lockUserMaskMutex();
    memcpy(byteArray.bytes, asUserMaskByteArray[trackingID], numUserMaskBytes);
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
	return NULL;
}
FREObject KinectDevice::freSetInfraredEnabled(FREObject argv[])
{
    return NULL;
}
FREObject KinectDevice::freGetInfraredFrame(FREObject argv[])
{
	return NULL;
}
FREObject KinectDevice::freSetPointCloudMode(FREObject argv[])
{
	lockPointCloudMutex();
    
    unsigned int width; FREGetObjectAsUint32(argv[1], &width);
    unsigned int height; FREGetObjectAsUint32(argv[2], &height);

    unsigned int density; FREGetObjectAsUint32(argv[4], &density);
    
    asPointCloudWidth = width;
    asPointCloudHeight = height;
    asPointCloudDensity = density;
    asPointCloudIncludeRGB = createBoolFromFREObject(argv[5]);
    asPointCloudPixelCount = (asPointCloudWidth * asPointCloudHeight) / asPointCloudDensity;
    asPointCloudMirrored = createBoolFromFREObject(argv[3]);
    pointCloudScale = pointCloudWidth / asPointCloudWidth;
    
    if(asPointCloudByteArray != 0) delete [] asPointCloudByteArray;
    if(asPointCloudIncludeRGB)
    {
        asPointCloudByteArray = new short[asPointCloudPixelCount * 6];
    }
    else
    {
        asPointCloudByteArray = new short[asPointCloudPixelCount * 3];
    }
    
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
    
    const unsigned int numPointBytes = (asPointCloudWidth * asPointCloudHeight * sizeof(short) * (asPointCloudIncludeRGB ? 6 : 3)) / asPointCloudDensity;
    if(numPointBytes == 0) return NULL;

    lockPointCloudMutex();
    
    FREByteArray pointsByteArray;			
    FREObject pointsLength;
    FRENewObjectFromUint32(numPointBytes, &pointsLength);
    FRESetObjectProperty(objectPointsByteArray, (const uint8_t*) "length", pointsLength, NULL);
    FREAcquireByteArray(objectPointsByteArray, &pointsByteArray);
    memcpy(pointsByteArray.bytes, asPointCloudByteArray, numPointBytes);
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
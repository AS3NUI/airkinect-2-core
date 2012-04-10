#include "stdafx.h"
#include "KinectDevice.h"
#include "KinectSkeleton.h"

void KinectDevice::setDefaults()
{
	started = false;

	asUserMirrored = false;
    asUserEnabled = false;
    asSkeletonMirrored = false;
    asSkeletonEnabled = false;

	//
	// DEPTH IMAGE
	//
	asDepthWidth = 320;
    asDepthHeight = 240;
    asDepthPixelCount = asDepthWidth * asDepthHeight;
    asDepthMirrored = false;
    asDepthEnabled = false;
    asDepthShowUserColors = false;
	asDepthEnableNearMode = false;
	asDepthByteArray = 0;

	depthWidth = 640;
    depthHeight = 480;
    depthPixelCount = depthWidth * depthHeight;
    depthScale = depthWidth / asDepthWidth;

	//
	// RGB IMAGE
	//
	asRGBWidth = 640;
    asRGBHeight = 480;
    asRGBPixelCount = asRGBWidth * asRGBHeight;
    asRGBMirrored = false;
    asRGBEnabled = false;
	asRGBByteArray = 0;

	rgbWidth = 640;
    rgbHeight = 480;
    rgbPixelCount = rgbWidth * rgbHeight;
    rgbScale = rgbWidth / asRGBWidth;

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
	unsigned int mirrored; FREGetObjectAsBool(argv[1], &mirrored);
    asUserMirrored = (mirrored != 0);
    return NULL;
}
FREObject KinectDevice::freSetUserColor(FREObject argv[])
{
	unsigned int userID; FREGetObjectAsUint32(argv[1], &userID);
    unsigned int color; FREGetObjectAsUint32(argv[2], &color);
    unsigned int useIntensity; FREGetObjectAsBool(argv[3], &useIntensity);
    setUserColor(userID, color, (useIntensity != 0));
    return NULL;
}
FREObject KinectDevice::freSetUserEnabled(FREObject argv[])
{
	unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asUserEnabled = (enabled != 0);
    return NULL;
}
FREObject KinectDevice::freSetSkeletonMode(FREObject argv[])
{
	unsigned int mirrored; FREGetObjectAsBool(argv[1], &mirrored);
    asSkeletonMirrored = (mirrored != 0);
    return NULL;
}
FREObject KinectDevice::freSetSkeletonEnabled(FREObject argv[])
{
	unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asSkeletonEnabled = (enabled != 0);
    return NULL;
}
FREObject KinectDevice::freGetUserFrame(FREObject argv[])
{
	lockUserMutex();
    
    short int trackedSkeletons = 0;
    
    FREObject freUserFrame, frameNumber, timestamp, users, user, userType, userID, trackingID, hasSkeleton, joints, joint, jointName;
    FREObject position, positionRelative, positionConfidence, orientation, orientationConfidence, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition;
    FREObject orientationX, orientationY, orientationZ;
    FREObject positionX, positionY, positionZ, positionRelativeX, positionRelativeY, positionRelativeZ;
    FREObject rgbPositionX, rgbPositionY, rgbRelativePositionX, rgbRelativePositionY, depthPositionX, depthPositionY, depthRelativePositionX, depthRelativePositionY;
    
    FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.User>", 0, NULL, &users, NULL);
    
    for(int i = 0; i < MAX_SKELETONS; i++)
    {
        if(userFrame.users[i].isTracking)
        {   
            //create the joints vector
            FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint>", 0, NULL, &joints, NULL);
            
            for(int j = 0; j < NUM_JOINTS; j++)
            {
                //name
                FRENewObjectFromUTF8(strlen(JOINT_NAMES[j]), (const uint8_t*) JOINT_NAMES[j], &jointName);
                //position
                FRENewObjectFromDouble(userFrame.users[i].joints[j].worldX, &positionX);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].worldY, &positionY);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].worldZ, &positionZ);
                FREObject positionParams[] = {positionX, positionY, positionZ};
                FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, positionParams, &position, NULL);
                //position relative
                FRENewObjectFromDouble(userFrame.users[i].joints[j].worldRelativeX, &positionRelativeX);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].worldRelativeY, &positionRelativeY);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].worldRelativeZ, &positionRelativeZ);
                FREObject positionRelativeParams[] = {positionRelativeX, positionRelativeY, positionRelativeZ};
                FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, positionRelativeParams, &positionRelative, NULL);
                //position confidence
                FRENewObjectFromDouble(userFrame.users[i].joints[j].positionConfidence, &positionConfidence);
                
                FRENewObjectFromDouble(userFrame.users[i].joints[j].orientationX, &orientationX);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].orientationY, &orientationY);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].orientationZ, &orientationZ);
                
                //orientation
                FREObject orientationParams[] = {orientationX, orientationY, orientationZ};
                FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, orientationParams, &orientation, NULL);
                
                //orientation confidence
                FRENewObjectFromDouble(userFrame.users[i].joints[j].orientationConfidence, &orientationConfidence);
                //rgb position
                FRENewObjectFromInt32(userFrame.users[i].joints[j].rgbX, &rgbPositionX);
                FRENewObjectFromInt32(userFrame.users[i].joints[j].rgbY, &rgbPositionY);
                FREObject rgbPositionParams[] = {rgbPositionX, rgbPositionY};
                FRENewObject( (const uint8_t*) "flash.geom.Point", 2, rgbPositionParams, &rgbPosition, NULL);
                //rgb relative position
                FRENewObjectFromDouble(userFrame.users[i].joints[j].rgbRelativeX, &rgbRelativePositionX);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].rgbRelativeY, &rgbRelativePositionY);
                FREObject rgbRelativePositionParams[] = {rgbRelativePositionX, rgbRelativePositionY};
                FRENewObject( (const uint8_t*) "flash.geom.Point", 2, rgbRelativePositionParams, &rgbRelativePosition, NULL);
                //depth position
                FRENewObjectFromInt32(userFrame.users[i].joints[j].depthX, &depthPositionX);
                FRENewObjectFromInt32(userFrame.users[i].joints[j].depthY, &depthPositionY);
                FREObject depthPositionParams[] = {depthPositionX, depthPositionY};
                FRENewObject( (const uint8_t*) "flash.geom.Point", 2, depthPositionParams, &depthPosition, NULL);
                //depth relative position
                FRENewObjectFromDouble(userFrame.users[i].joints[j].depthRelativeX, &depthRelativePositionX);
                FRENewObjectFromDouble(userFrame.users[i].joints[j].depthRelativeY, &depthRelativePositionY);
                FREObject depthRelativePositionParams[] = {depthRelativePositionX, depthRelativePositionY};
                FRENewObject( (const uint8_t*) "flash.geom.Point", 2, depthRelativePositionParams, &depthRelativePosition, NULL);
                //create the joint
                FREObject jointParams[] = {jointName, position, positionRelative, positionConfidence, orientation, orientationConfidence, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition};
                FRENewObject( (const uint8_t*) asJointClass, 10, jointParams, &joint, NULL);
                
				FRESetArrayElementAt(joints, j, joint);
            }
            
            //user position
            FRENewObjectFromDouble(userFrame.users[i].worldX, &positionX);
            FRENewObjectFromDouble(userFrame.users[i].worldY, &positionY);
            FRENewObjectFromDouble(userFrame.users[i].worldZ, &positionZ);
            FREObject positionParams[] = {positionX, positionY, positionZ};
            FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, positionParams, &position, NULL);
            
            //user position relative
            FRENewObjectFromDouble(userFrame.users[i].worldRelativeX, &positionRelativeX);
            FRENewObjectFromDouble(userFrame.users[i].worldRelativeY, &positionRelativeY);
            FRENewObjectFromDouble(userFrame.users[i].worldRelativeZ, &positionRelativeZ);
            FREObject positionRelativeParams[] = {positionRelativeX, positionRelativeY, positionRelativeZ};
            FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, positionRelativeParams, &positionRelative, NULL);
            
            //user rgb position
            FRENewObjectFromInt32(userFrame.users[i].rgbX, &rgbPositionX);
            FRENewObjectFromInt32(userFrame.users[i].rgbY, &rgbPositionY);
            FREObject rgbPositionParams[] = {rgbPositionX, rgbPositionY};
            FRENewObject( (const uint8_t*) "flash.geom.Point", 2, rgbPositionParams, &rgbPosition, NULL);
            //user rgb relative position
            FRENewObjectFromDouble(userFrame.users[i].rgbRelativeX, &rgbRelativePositionX);
            FRENewObjectFromDouble(userFrame.users[i].rgbRelativeY, &rgbRelativePositionY);
            FREObject rgbRelativePositionParams[] = {rgbRelativePositionX, rgbRelativePositionY};
            FRENewObject( (const uint8_t*) "flash.geom.Point", 2, rgbRelativePositionParams, &rgbRelativePosition, NULL);
            //user depth position
            FRENewObjectFromInt32(userFrame.users[i].depthX, &depthPositionX);
            FRENewObjectFromInt32(userFrame.users[i].depthY, &depthPositionY);
            FREObject depthPositionParams[] = {depthPositionX, depthPositionY};
            FRENewObject( (const uint8_t*) "flash.geom.Point", 2, depthPositionParams, &depthPosition, NULL);
            //user depth relative position
            FRENewObjectFromDouble(userFrame.users[i].depthRelativeX, &depthRelativePositionX);
            FRENewObjectFromDouble(userFrame.users[i].depthRelativeY, &depthRelativePositionY);
            FREObject depthRelativePositionParams[] = {depthRelativePositionX, depthRelativePositionY};
            FRENewObject( (const uint8_t*) "flash.geom.Point", 2, depthRelativePositionParams, &depthRelativePosition, NULL);
            
            FRENewObjectFromUTF8(strlen(capabilities.framework), (const uint8_t*) capabilities.framework, &userType);
            FRENewObjectFromUint32(userFrame.users[i].userID, &userID);
            FRENewObjectFromUint32(userFrame.users[i].trackingID, &trackingID);
            FRENewObjectFromBool((userFrame.users[i].hasSkeleton) ? 1 : 0, &hasSkeleton);
            FREObject skeletonParams[] = {userType, userID, trackingID, position, positionRelative, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition, hasSkeleton, joints};
            
            FRENewObject( (const uint8_t*) asUserClass, 11, skeletonParams, &user, NULL);
            
            FRESetArrayElementAt(users, trackedSkeletons, user);
            trackedSkeletons++;
        }
    }
    
    FRENewObjectFromUint32(userFrame.frameNumber, &frameNumber);
    FRENewObjectFromUint32(userFrame.timeStamp, &timestamp);
    
    FREObject skeletonFrameParams[] = {frameNumber, timestamp, users};
    FRENewObject( (const uint8_t*) asUserFrameClass, 3, skeletonFrameParams, &freUserFrame, NULL);
    
    unlockUserMutex();
    
    return freUserFrame;
}
FREObject KinectDevice::freGetSkeletonJointNameIndices(FREObject argv[])
{
	FREObject skeletonJointNameIndices, jointIndex;
    FRENewObject( (const uint8_t*) "flash.utils.Dictionary", 0, NULL, &skeletonJointNameIndices, NULL);
	for(int i = 0; i < NUM_JOINTS; i++)
    {
        FRENewObjectFromUint32(i, &jointIndex);
        FRESetObjectProperty(skeletonJointNameIndices, (const uint8_t*) JOINT_NAMES[i], jointIndex, NULL);
    }
    return skeletonJointNameIndices;
}
FREObject KinectDevice::freGetSkeletonJointNames(FREObject argv[])
{ 
	FREObject skeletonJointNames, skeletonJointName;
	FRENewObject( (const uint8_t*) "Vector.<String>", 0, NULL, &skeletonJointNames, NULL);
	for(int i = 0; i < NUM_JOINTS; i++)
	{
	FRENewObjectFromUTF8(strlen(JOINT_NAMES[i]), (const uint8_t*) JOINT_NAMES[i], &skeletonJointName);
	FRESetArrayElementAt(skeletonJointNames, i, skeletonJointName);
	}
	return skeletonJointNames;
}
FREObject KinectDevice::freSetUserMaskMode(FREObject argv[])
{
	unsigned int width; FREGetObjectAsUint32(argv[1], &width);
    unsigned int height; FREGetObjectAsUint32(argv[2], &height);
    unsigned int mirrored; FREGetObjectAsBool(argv[3], &mirrored);
    
    lockUserMaskMutex();
    
    asUserMaskWidth = width;
    asUserMaskHeight = height;
    asUserMaskPixelCount = asUserMaskWidth * asUserMaskHeight;
    asUserMaskMirrored = (mirrored != 0);
    userMaskScale = userMaskWidth / asUserMaskWidth;
    
    //reset bytearray
    if(asUserMaskByteArray != 0)
    {
        for(int i = 0; i < MAX_SKELETONS; i++)
        {
            delete [] asUserMaskByteArray[i];
        }
        delete [] asUserMaskByteArray;
    }
    asUserMaskByteArray = new uint32_t*[MAX_SKELETONS];
    for(int i = 0; i < MAX_SKELETONS; i++)
    {
        asUserMaskByteArray[i] = new uint32_t[asUserMaskPixelCount];
    }
    
    unlockUserMaskMutex();
    return NULL;
}
FREObject KinectDevice::freSetUserMaskEnabled(FREObject argv[])
{
	unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asUserMaskEnabled = (enabled != 0);
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
    unsigned int mirrored; FREGetObjectAsBool(argv[3], &mirrored);
    
    lockDepthMutex();
    
    asDepthWidth = width;
    asDepthHeight = height;
    asDepthPixelCount = asDepthWidth * asDepthHeight;
    asDepthMirrored = (mirrored != 0);
    depthScale = depthWidth / asDepthWidth;
    
    //reset bytearray
    if(asDepthByteArray != 0) delete [] asDepthByteArray;
    asDepthByteArray = new uint32_t[asDepthPixelCount];
    
    unlockDepthMutex();
    return NULL;
}
FREObject KinectDevice::freSetDepthEnabled(FREObject argv[])
{
	unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asDepthEnabled = (enabled != 0);
    return NULL;
}
FREObject KinectDevice::freGetDepthFrame(FREObject argv[])
{
	const unsigned int numDepthBytes = asDepthPixelCount * 4;
    
    FREObject objectByteArray = argv[1];
    FREByteArray byteArray;			
    FREObject length;
    FRENewObjectFromUint32(numDepthBytes, &length);
    FRESetObjectProperty(objectByteArray, (const uint8_t*) "length", length, NULL);
    FREAcquireByteArray(objectByteArray, &byteArray);
    lockDepthMutex();
    memcpy(byteArray.bytes, asDepthByteArray, numDepthBytes);
    unlockDepthMutex();
    FREReleaseByteArray(objectByteArray);
    
    return NULL;
}
FREObject KinectDevice::freSetDepthShowUserColors(FREObject argv[])
{
	unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asDepthShowUserColors = (enabled != 0);
    return NULL;
}
FREObject KinectDevice::freSetDepthEnableNearMode(FREObject argv[])
{
	unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asDepthEnableNearMode = (enabled != 0);
    return NULL;
}
FREObject KinectDevice::freSetRGBMode(FREObject argv[])
{
	unsigned int width; FREGetObjectAsUint32(argv[1], &width);
    unsigned int height; FREGetObjectAsUint32(argv[2], &height);
    unsigned int mirrored; FREGetObjectAsBool(argv[3], &mirrored);
    
    lockRGBMutex();
    
    asRGBWidth = width;
    asRGBHeight = height;
    asRGBPixelCount = asRGBWidth * asRGBHeight;
    asRGBMirrored = (mirrored != 0);
    rgbScale = rgbWidth / asRGBWidth;
    
    //reset bytearray
    if(asRGBByteArray != 0) delete [] asRGBByteArray;
    asRGBByteArray = new uint32_t[asRGBPixelCount];
    
    unlockRGBMutex();
    return NULL;
}
FREObject KinectDevice::freSetRGBEnabled(FREObject argv[])
{
	unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asRGBEnabled = (enabled != 0);
    return NULL;
}
FREObject KinectDevice::freGetRGBFrame(FREObject argv[])
{
	const unsigned int numRGBBytes = asRGBPixelCount * 4;
    
    FREObject objectByteArray = argv[1];
    FREByteArray byteArray;			
    FREObject length;
    FRENewObjectFromUint32(numRGBBytes, &length);
    FRESetObjectProperty(objectByteArray, (const uint8_t*) "length", length, NULL);
    FREAcquireByteArray(objectByteArray, &byteArray);
    lockRGBMutex();
    memcpy(byteArray.bytes, asRGBByteArray, numRGBBytes);
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
    unsigned int mirrored; FREGetObjectAsBool(argv[3], &mirrored);
    unsigned int density; FREGetObjectAsUint32(argv[4], &density);
    unsigned int includeRGB; FREGetObjectAsBool(argv[5], &includeRGB);
    
    asPointCloudWidth = width;
    asPointCloudHeight = height;
    asPointCloudDensity = density;
    asPointCloudIncludeRGB = (includeRGB != 0);
    asPointCloudPixelCount = (asPointCloudWidth * asPointCloudHeight) / asPointCloudDensity;
    asPointCloudMirrored = (mirrored != 0);
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
	unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
    asPointCloudEnabled = (enabled != 0);
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
}

void KinectDevice::unlockUserMutex()
{
}

void KinectDevice::lockDepthMutex()
{
}

void KinectDevice::unlockDepthMutex()
{
}

void KinectDevice::lockRGBMutex()
{
}

void KinectDevice::unlockRGBMutex()
{
}

void KinectDevice::lockUserMaskMutex()
{
}

void KinectDevice::unlockUserMaskMutex()
{
}

void KinectDevice::lockInfraredMutex()
{
}

void KinectDevice::unlockInfraredMutex()
{
}

void KinectDevice::lockPointCloudMutex()
{
}

void KinectDevice::unlockPointCloudMutex()
{
}

void KinectDevice::setUserColor(int userID, int color, bool useIntensity)
{
}
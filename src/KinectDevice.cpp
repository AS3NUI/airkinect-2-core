#include "KinectDevice.h"
#include "KinectSkeleton.h"

void KinectDevice::setDefaults()
{
	started = false;
    running = false;

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
	setRGBMode(640, 480, 640, 480, false);
    asRGBEnabled = false;
	asRGBByteArray = 0;

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

	//user frame allocation
	userFrame.users = new kinectUser[maxSkeletons];
	for(int i = 0; i < maxSkeletons; i++)
	{
		userFrame.users[i].joints = new kinectSkeletonJoint[numJoints];
	}
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
    FREObject position, positionRelative, positionConfidence, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition;
	FREObject orientationConfidence;
	FREObject absoluteOrientationMatrix, absoluteOrientationQuaternion;
	FREObject hierarchicalOrientationMatrix, hierarchicalOrientationQuaternion;
    
    FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.User>", 0, NULL, &users, NULL);
    
    for(int i = 0; i < maxSkeletons; i++)
    {
        if(userFrame.users[i].isTracking)
        {   
            //create the joints vector
            FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint>", 0, NULL, &joints, NULL);
            
            for(int j = 0; j < numJoints; j++)
            {
                //name
                FRENewObjectFromUTF8(strlen(jointNames[j]), (const uint8_t*) jointNames[j], &jointName);
                //position
				position = createFREVector3D(userFrame.users[i].joints[j].worldX, userFrame.users[i].joints[j].worldY, userFrame.users[i].joints[j].worldZ, 0.0);
                positionRelative = createFREVector3D(userFrame.users[i].joints[j].worldRelativeX, userFrame.users[i].joints[j].worldRelativeY, userFrame.users[i].joints[j].worldRelativeZ, 0.0);
				rgbPosition = createFREPoint(userFrame.users[i].joints[j].rgbX, userFrame.users[i].joints[j].rgbY);
				rgbRelativePosition = createFREPoint(userFrame.users[i].joints[j].rgbRelativeX, userFrame.users[i].joints[j].rgbRelativeY);
				depthPosition = createFREPoint(userFrame.users[i].joints[j].depthX, userFrame.users[i].joints[j].depthY);
				depthRelativePosition = createFREPoint(userFrame.users[i].joints[j].depthRelativeX, userFrame.users[i].joints[j].depthRelativeY);
				//position confidence
				FRENewObjectFromDouble(userFrame.users[i].joints[j].positionConfidence, &positionConfidence);
				//orientation
				absoluteOrientationMatrix = createFREMatrix3DFromKinectRotationMatrix(userFrame.users[i].joints[j].absoluteOrientation.rotationMatrix);
				absoluteOrientationQuaternion = createFREVector3DFromKinectRotationQuaternion(userFrame.users[i].joints[j].absoluteOrientation.rotationQuaternion);
				hierarchicalOrientationMatrix = createFREMatrix3DFromKinectRotationMatrix(userFrame.users[i].joints[j].hierarchicalOrientation.rotationMatrix);
				hierarchicalOrientationQuaternion = createFREVector3DFromKinectRotationQuaternion(userFrame.users[i].joints[j].hierarchicalOrientation.rotationQuaternion);
                //orientation confidence
                FRENewObjectFromDouble(userFrame.users[i].joints[j].orientationConfidence, &orientationConfidence);

                //create the joint
                FREObject jointParams[] = {jointName, position, positionRelative, positionConfidence, absoluteOrientationMatrix, absoluteOrientationQuaternion, hierarchicalOrientationMatrix, hierarchicalOrientationQuaternion, orientationConfidence, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition};
                FRENewObject( (const uint8_t*) asJointClass, 13, jointParams, &joint, NULL);
                
				FRESetArrayElementAt(joints, j, joint);
            }
            
			position = createFREVector3D(userFrame.users[i].worldX, userFrame.users[i].worldY, userFrame.users[i].worldZ, 0.0);
			positionRelative = createFREVector3D(userFrame.users[i].worldRelativeX, userFrame.users[i].worldRelativeY, userFrame.users[i].worldRelativeZ, 0.0);
            rgbPosition = createFREPoint(userFrame.users[i].rgbX, userFrame.users[i].rgbY);
			rgbRelativePosition = createFREPoint(userFrame.users[i].rgbRelativeX, userFrame.users[i].rgbRelativeY);
			depthPosition = createFREPoint(userFrame.users[i].depthX, userFrame.users[i].depthY);
			depthRelativePosition = createFREPoint(userFrame.users[i].depthRelativeX, userFrame.users[i].depthRelativeY);
            
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

FREObject KinectDevice::createFREPoint(double x, double y)
{
	FREObject point;
	FREObject freX, freY;
	FRENewObjectFromDouble(x, &freX);
    FRENewObjectFromDouble(y, &freY);
    FREObject pointParams[] = {freX, freY};
    FRENewObject( (const uint8_t*) "flash.geom.Point", 2, pointParams, &point, NULL);
	return point;
}

FREObject KinectDevice::createFREVector3DFromKinectRotationQuaternion(kinectRotationQuaternion q)
{
	return createFREVector3D(q.x, q.y, q.z, q.w);
}

FREObject KinectDevice::createFREVector3D(double x, double y, double z, double w)
{
	FREObject vector;
	FREObject freX, freY, freZ, freW;
	FRENewObjectFromDouble(x, &freX);
	FRENewObjectFromDouble(y, &freY);
	FRENewObjectFromDouble(z, &freZ);
	FRENewObjectFromDouble(w, &freW);
    FREObject vectorParams[] = {freX, freY, freZ, freW};
    FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 4, vectorParams, &vector, NULL);
	return vector;
}

FREObject KinectDevice::createFREMatrix3DFromKinectRotationMatrix(kinectRotationMatrix m)
{
	return createFREMatrix3D(
		m.M11,
		m.M12,
		m.M13,
		m.M14,
		m.M21,
		m.M22,
		m.M23,
		m.M24,
		m.M31,
		m.M32,
		m.M33,
		m.M34,
		m.M41,
		m.M42,
		m.M43,
		m.M44);
}

FREObject KinectDevice::createFREMatrix3D(double m11, double m12, double m13, double m14, double m21, double m22, double m23, double m24, double m31, double m32, double m33, double m34, double m41, double m42, double m43, double m44)
{
	FREObject freM11, freM12, freM13, freM14;
	FRENewObjectFromDouble(m11, &freM11);
	FRENewObjectFromDouble(m12, &freM12);
	FRENewObjectFromDouble(m13, &freM13);
	FRENewObjectFromDouble(m14, &freM14);

	FREObject freM21, freM22, freM23, freM24;
	FRENewObjectFromDouble(m21, &freM21);
	FRENewObjectFromDouble(m22, &freM22);
	FRENewObjectFromDouble(m23, &freM23);
	FRENewObjectFromDouble(m24, &freM24);

	FREObject freM31, freM32, freM33, freM34;
	FRENewObjectFromDouble(m31, &freM31);
	FRENewObjectFromDouble(m32, &freM32);
	FRENewObjectFromDouble(m33, &freM33);
	FRENewObjectFromDouble(m34, &freM34);

	FREObject freM41, freM42, freM43, freM44;
	FRENewObjectFromDouble(m41, &freM41);
	FRENewObjectFromDouble(m42, &freM42);
	FRENewObjectFromDouble(m43, &freM43);
	FRENewObjectFromDouble(m44, &freM44);

	FREObject freMatrixNumbers;
	FRENewObject( (const uint8_t*) "Vector.<Number>", 0, NULL, &freMatrixNumbers, NULL);

	FRESetArrayElementAt(freMatrixNumbers, 0, freM11);
	FRESetArrayElementAt(freMatrixNumbers, 1, freM12);
	FRESetArrayElementAt(freMatrixNumbers, 2, freM13);
	FRESetArrayElementAt(freMatrixNumbers, 3, freM14);

	FRESetArrayElementAt(freMatrixNumbers, 4, freM21);
	FRESetArrayElementAt(freMatrixNumbers, 5, freM22);
	FRESetArrayElementAt(freMatrixNumbers, 6, freM23);
	FRESetArrayElementAt(freMatrixNumbers, 7, freM24);

	FRESetArrayElementAt(freMatrixNumbers, 8, freM31);
	FRESetArrayElementAt(freMatrixNumbers, 9, freM32);
	FRESetArrayElementAt(freMatrixNumbers, 10, freM33);
	FRESetArrayElementAt(freMatrixNumbers, 11, freM34);

	FRESetArrayElementAt(freMatrixNumbers, 12, freM41);
	FRESetArrayElementAt(freMatrixNumbers, 13, freM42);
	FRESetArrayElementAt(freMatrixNumbers, 14, freM43);
	FRESetArrayElementAt(freMatrixNumbers, 15, freM44);

	FREObject matrix;
	FREObject matrixParams[] = {freMatrixNumbers};
    FRENewObject( (const uint8_t*) "flash.geom.Matrix3D", 1, matrixParams, &matrix, NULL);

	return matrix;
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

	setRGBMode(rgbWidth, rgbHeight, width, height, (mirrored != 0));
    
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
	this->rgbWidth = rgbWidth;
    this->rgbHeight = rgbHeight;
	this->asRGBWidth = asRGBWidth;
    this->asRGBHeight = asRGBHeight;
	this->asRGBMirrored = asRGBMirrored;

	rgbPixelCount = rgbWidth * rgbHeight;
    rgbScale = rgbWidth / asRGBWidth;

    asRGBPixelCount = asRGBWidth * asRGBHeight;
}
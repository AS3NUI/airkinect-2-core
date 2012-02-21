#include <stdlib.h>
#include <stdio.h>

#include "KinectExtension.h"
#include "KinectDevice.h"
#include "KinectDeviceManager.h"
#include "PointCloudRegion.h"

// Symbols tagged with EXPORT are externally visible.
// Must use the -fvisibility=hidden gcc option.
#define EXPORT __attribute__((visibility("default")))

extern "C"
{
    
    KinectDeviceManager kinectDeviceManager;
    
    void addOrientationValueToMatrix3D(float val, int targetMatrixIndex, FREObject matrixData);
    
    FREObject Kinect_getDeviceCount(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        FREObject retObj;
		FRENewObjectFromInt32(kinectDeviceManager.getNumDevices(), &retObj);
		return retObj;
    }
    
    FREObject Kinect_getCapabilities(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
		return kinectDeviceManager.getCapabilities();
    }
    
    FREObject Kinect_applicationShutdown(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        kinectDeviceManager.shutDown();
        return NULL;
    }
    
    FREObject Kinect_start(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        kinectDeviceManager.getDevice(nr, ctx)->start();
        return NULL;
    }
    
    FREObject Kinect_stop(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        kinectDeviceManager.getDevice(nr, ctx)->stop();
        return NULL;
    }
    
    FREObject Kinect_setUserMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int mirrored; FREGetObjectAsBool(argv[1], &mirrored);
        kinectDeviceManager.getDevice(nr, ctx)->setUserMode((mirrored != 0));
        return NULL;
    }
    
	FREObject Kinect_setUserColor(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
		unsigned int userID; FREGetObjectAsUint32(argv[1], &userID);
		unsigned int color; FREGetObjectAsUint32(argv[2], &color);
        unsigned int useIntensity; FREGetObjectAsBool(argv[3], &useIntensity);
		kinectDeviceManager.getDevice(nr, ctx)->setUserColor(userID, color, (useIntensity != 0));
        return NULL;
    }
    
    FREObject Kinect_setUserEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
        kinectDeviceManager.getDevice(nr, ctx)->setUserEnabled((enabled != 0));
        return NULL;
    }
    
    FREObject Kinect_setSkeletonMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int mirrored; FREGetObjectAsBool(argv[1], &mirrored);
        kinectDeviceManager.getDevice(nr, ctx)->setSkeletonMode((mirrored != 0));
        return NULL;
    }
    
    FREObject Kinect_setSkeletonEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
        kinectDeviceManager.getDevice(nr, ctx)->setSkeletonEnabled((enabled != 0));
        return NULL;
    }
    
    FREObject Kinect_getSkeletonJointNameIndices(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
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
    
    FREObject Kinect_getSkeletonJointNames(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
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
    
    FREObject Kinect_getUserFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        
        KinectDevice *device = kinectDeviceManager.getDevice(nr, ctx);
        
        pthread_mutex_lock(&device->userMutex);
        
        short int trackedSkeletons = 0;
        
        FREObject userFrame, frameNumber, timestamp, users, user, userType, userID, trackingID, hasSkeleton, joints, joint, jointName;
        FREObject position, positionRelative, positionConfidence, orientation, orientationConfidence, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition;
        FREObject orientationX, orientationY, orientationZ;
        FREObject positionX, positionY, positionZ, positionRelativeX, positionRelativeY, positionRelativeZ;
        FREObject rgbPositionX, rgbPositionY, rgbRelativePositionX, rgbRelativePositionY, depthPositionX, depthPositionY, depthRelativePositionX, depthRelativePositionY;
        
        FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.User>", 0, NULL, &users, NULL);
        
        for(int i = 0; i < MAX_SKELETONS; i++)
        {
            if(device->userFrame.users[i].isTracking)
            {   
                //create the joints vector
                FRENewObject( (const uint8_t*) "Vector.<com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint>", 0, NULL, &joints, NULL);
                
                for(int j = 0; j < NUM_JOINTS; j++)
                {
                    //name
                    FRENewObjectFromUTF8(strlen(JOINT_NAMES[j]), (const uint8_t*) JOINT_NAMES[j], &jointName);
                    //position
                    FRENewObjectFromDouble(device->userFrame.users[i].joints[j].worldX, &positionX);
                    FRENewObjectFromDouble(device->userFrame.users[i].joints[j].worldY, &positionY);
                    FRENewObjectFromDouble(device->userFrame.users[i].joints[j].worldZ, &positionZ);
                    FREObject positionParams[] = {positionX, positionY, positionZ};
                    FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, positionParams, &position, NULL);
                    //position relative
                    FRENewObjectFromDouble(device->userFrame.users[i].joints[j].worldRelativeX, &positionRelativeX);
                    FRENewObjectFromDouble(device->userFrame.users[i].joints[j].worldRelativeY, &positionRelativeY);
                    FRENewObjectFromDouble(device->userFrame.users[i].joints[j].worldRelativeZ, &positionRelativeZ);
                    FREObject positionRelativeParams[] = {positionRelativeX, positionRelativeY, positionRelativeZ};
                    FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, positionRelativeParams, &positionRelative, NULL);
                    //position confidence
                    FRENewObjectFromDouble(device->userFrame.users[i].joints[j].positionConfidence, &positionConfidence);
                    
                    FRENewObjectFromDouble(device->userFrame.users[i].joints[j].orientationX, &orientationX);
                    FRENewObjectFromDouble(device->userFrame.users[i].joints[j].orientationY, &orientationY);
                    FRENewObjectFromDouble(device->userFrame.users[i].joints[j].orientationZ, &orientationZ);
                    
                    //orientation
                    FREObject orientationParams[] = {orientationX, orientationY, orientationZ};
                    FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, orientationParams, &orientation, NULL);
                     
                    //orientation confidence
                    FRENewObjectFromDouble(device->userFrame.users[i].joints[j].orientationConfidence, &orientationConfidence);
                    //rgb position
                    FRENewObjectFromInt32(device->userFrame.users[i].joints[j].rgbX, &rgbPositionX);
                    FRENewObjectFromInt32(device->userFrame.users[i].joints[j].rgbY, &rgbPositionY);
                    FREObject rgbPositionParams[] = {rgbPositionX, rgbPositionY};
                    FRENewObject( (const uint8_t*) "flash.geom.Point", 2, rgbPositionParams, &rgbPosition, NULL);
                    //rgb relative position
                    FRENewObjectFromDouble(device->userFrame.users[i].joints[j].rgbRelativeX, &rgbRelativePositionX);
                    FRENewObjectFromDouble(device->userFrame.users[i].joints[j].rgbRelativeY, &rgbRelativePositionY);
                    FREObject rgbRelativePositionParams[] = {rgbRelativePositionX, rgbRelativePositionY};
                    FRENewObject( (const uint8_t*) "flash.geom.Point", 2, rgbRelativePositionParams, &rgbRelativePosition, NULL);
                    //depth position
                    FRENewObjectFromInt32(device->userFrame.users[i].joints[j].depthX, &depthPositionX);
                    FRENewObjectFromInt32(device->userFrame.users[i].joints[j].depthY, &depthPositionY);
                    FREObject depthPositionParams[] = {depthPositionX, depthPositionY};
                    FRENewObject( (const uint8_t*) "flash.geom.Point", 2, depthPositionParams, &depthPosition, NULL);
                    //depth relative position
                    FRENewObjectFromDouble(device->userFrame.users[i].joints[j].depthRelativeX, &depthRelativePositionX);
                    FRENewObjectFromDouble(device->userFrame.users[i].joints[j].depthRelativeY, &depthRelativePositionY);
                    FREObject depthRelativePositionParams[] = {depthRelativePositionX, depthRelativePositionY};
                    FRENewObject( (const uint8_t*) "flash.geom.Point", 2, depthRelativePositionParams, &depthRelativePosition, NULL);
                    //create the joint
                    FREObject jointParams[] = {jointName, position, positionRelative, positionConfidence, orientation, orientationConfidence, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition};
                    FRENewObject( (const uint8_t*) "com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint", 10, jointParams, &joint, NULL);
                    FRESetArrayElementAt(joints, j, joint);
                }
                
                //user position
                FRENewObjectFromDouble(device->userFrame.users[i].worldX, &positionX);
                FRENewObjectFromDouble(device->userFrame.users[i].worldY, &positionY);
                FRENewObjectFromDouble(device->userFrame.users[i].worldZ, &positionZ);
                FREObject positionParams[] = {positionX, positionY, positionZ};
                FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, positionParams, &position, NULL);
                
                //user position relative
                FRENewObjectFromDouble(device->userFrame.users[i].worldRelativeX, &positionRelativeX);
                FRENewObjectFromDouble(device->userFrame.users[i].worldRelativeY, &positionRelativeY);
                FRENewObjectFromDouble(device->userFrame.users[i].worldRelativeZ, &positionRelativeZ);
                FREObject positionRelativeParams[] = {positionRelativeX, positionRelativeY, positionRelativeZ};
                FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, positionRelativeParams, &positionRelative, NULL);
                
                //user rgb position
                FRENewObjectFromInt32(device->userFrame.users[i].rgbX, &rgbPositionX);
                FRENewObjectFromInt32(device->userFrame.users[i].rgbY, &rgbPositionY);
                FREObject rgbPositionParams[] = {rgbPositionX, rgbPositionY};
                FRENewObject( (const uint8_t*) "flash.geom.Point", 2, rgbPositionParams, &rgbPosition, NULL);
                //user rgb relative position
                FRENewObjectFromDouble(device->userFrame.users[i].rgbRelativeX, &rgbRelativePositionX);
                FRENewObjectFromDouble(device->userFrame.users[i].rgbRelativeY, &rgbRelativePositionY);
                FREObject rgbRelativePositionParams[] = {rgbRelativePositionX, rgbRelativePositionY};
                FRENewObject( (const uint8_t*) "flash.geom.Point", 2, rgbRelativePositionParams, &rgbRelativePosition, NULL);
                //user depth position
                FRENewObjectFromInt32(device->userFrame.users[i].depthX, &depthPositionX);
                FRENewObjectFromInt32(device->userFrame.users[i].depthY, &depthPositionY);
                FREObject depthPositionParams[] = {depthPositionX, depthPositionY};
                FRENewObject( (const uint8_t*) "flash.geom.Point", 2, depthPositionParams, &depthPosition, NULL);
                //user depth relative position
                FRENewObjectFromDouble(device->userFrame.users[i].depthRelativeX, &depthRelativePositionX);
                FRENewObjectFromDouble(device->userFrame.users[i].depthRelativeY, &depthRelativePositionY);
                FREObject depthRelativePositionParams[] = {depthRelativePositionX, depthRelativePositionY};
                FRENewObject( (const uint8_t*) "flash.geom.Point", 2, depthRelativePositionParams, &depthRelativePosition, NULL);
                
                FRENewObjectFromUTF8(6, (const uint8_t*) "openni", &userType);
				FRENewObjectFromUint32(device->userFrame.users[i].userID, &userID);
				FRENewObjectFromUint32(device->userFrame.users[i].trackingID, &trackingID);
				FRENewObjectFromBool((device->userFrame.users[i].hasSkeleton) ? 1 : 0, &hasSkeleton);
				FREObject skeletonParams[] = {userType, userID, trackingID, position, positionRelative, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition, hasSkeleton, joints};
                
				FRENewObject( (const uint8_t*) "com.as3nui.nativeExtensions.air.kinect.data.User", 11, skeletonParams, &user, NULL);
                
				FRESetArrayElementAt(users, trackedSkeletons, user);
				trackedSkeletons++;
            }
        }
        
        FRENewObjectFromUint32(device->userFrame.frameNumber, &frameNumber);
        FRENewObjectFromUint32(device->userFrame.timeStamp, &timestamp);
        
        FREObject skeletonFrameParams[] = {frameNumber, timestamp, users};
        FRENewObject( (const uint8_t*) "com.as3nui.nativeExtensions.air.kinect.data.UserFrame", 3, skeletonFrameParams, &userFrame, NULL);
        
        pthread_mutex_unlock(&device->userMutex);
        return userFrame;
    }
    
    void addOrientationValueToMatrix3D(float val, int targetMatrixIndex, FREObject matrixData)
    {
        FREObject orientationValue;
        FRENewObjectFromDouble(val, &orientationValue);
        FRESetArrayElementAt(matrixData, targetMatrixIndex, orientationValue);
    }
    
    FREObject Kinect_setUserMaskMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int width; FREGetObjectAsUint32(argv[1], &width);
        unsigned int height; FREGetObjectAsUint32(argv[2], &height);
        unsigned int mirrored; FREGetObjectAsBool(argv[3], &mirrored);
        kinectDeviceManager.getDevice(nr, ctx)->setUserMaskMode(width, height, (mirrored != 0));
        return NULL;
    }
    
    FREObject Kinect_setUserMaskEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
        kinectDeviceManager.getDevice(nr, ctx)->setUserMaskEnabled((enabled != 0));
        return NULL;
    }
    
    FREObject Kinect_getUserMaskFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int trackingID; FREGetObjectAsUint32(argv[1], &trackingID);
        
        if(trackingID > 0) trackingID--;
        
        KinectDevice *device = kinectDeviceManager.getDevice(nr, ctx);
        
        const unsigned int numUserMaskBytes = device->getAsUserMaskWidth() * device->getAsUserMaskHeight() * 4;
        
		FREObject objectByteArray = argv[2];
		FREByteArray byteArray;			
		FREObject length;
		FRENewObjectFromUint32(numUserMaskBytes, &length);
		FRESetObjectProperty(objectByteArray, (const uint8_t*) "length", length, NULL);
		FREAcquireByteArray(objectByteArray, &byteArray);
        pthread_mutex_lock(&device->userMaskMutex);
		memcpy(byteArray.bytes, device->userMaskByteArray[trackingID], numUserMaskBytes);
        pthread_mutex_unlock(&device->userMaskMutex);		
        FREReleaseByteArray(objectByteArray);
        
        return NULL;
    }
    
    FREObject Kinect_setDepthMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int width; FREGetObjectAsUint32(argv[1], &width);
        unsigned int height; FREGetObjectAsUint32(argv[2], &height);
        unsigned int mirrored; FREGetObjectAsBool(argv[3], &mirrored);
        kinectDeviceManager.getDevice(nr, ctx)->setDepthMode(width, height, (mirrored != 0));
        return NULL;
    }
    
    FREObject Kinect_setDepthEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
        kinectDeviceManager.getDevice(nr, ctx)->setDepthEnabled((enabled != 0));
        return NULL;
    }
    
    FREObject Kinect_getDepthFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        KinectDevice *device = kinectDeviceManager.getDevice(nr, ctx);
        
        const unsigned int numDepthBytes = device->getAsDepthWidth() * device->getAsDepthHeight() * 4;
        
		FREObject objectByteArray = argv[1];
		FREByteArray byteArray;			
		FREObject length;
		FRENewObjectFromUint32(numDepthBytes, &length);
		FRESetObjectProperty(objectByteArray, (const uint8_t*) "length", length, NULL);
		FREAcquireByteArray(objectByteArray, &byteArray);
        pthread_mutex_lock(&device->depthMutex);
		memcpy(byteArray.bytes, device->depthByteArray, numDepthBytes);
        pthread_mutex_unlock(&device->depthMutex);		
        FREReleaseByteArray(objectByteArray);
        
        return NULL;
    }
    
    FREObject Kinect_setDepthShowUserColors(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
        kinectDeviceManager.getDevice(nr, ctx)->setDepthShowUserColors((enabled != 0));
        return NULL;
    }
    
    FREObject Kinect_setRGBMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int width; FREGetObjectAsUint32(argv[1], &width);
        unsigned int height; FREGetObjectAsUint32(argv[2], &height);
        unsigned int mirrored; FREGetObjectAsBool(argv[3], &mirrored);
        kinectDeviceManager.getDevice(nr, ctx)->setRGBMode(width, height, (mirrored != 0));
        return NULL;
    }
    
    FREObject Kinect_setRGBEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
        kinectDeviceManager.getDevice(nr, ctx)->setRGBEnabled((enabled != 0));
        return NULL;
    }
    
    FREObject Kinect_getRGBFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        
        KinectDevice *device = kinectDeviceManager.getDevice(nr, ctx);
        
        const unsigned int numRGBBytes = device->getAsRGBWidth() * device->getAsRGBHeight() * 4;
        
		FREObject objectByteArray = argv[1];
		FREByteArray byteArray;			
		FREObject length;
		FRENewObjectFromUint32(numRGBBytes, &length);
		FRESetObjectProperty(objectByteArray, (const uint8_t*) "length", length, NULL);
		FREAcquireByteArray(objectByteArray, &byteArray);
        pthread_mutex_lock(&device->rgbMutex);
		memcpy(byteArray.bytes, device->RGBByteArray, numRGBBytes);
        pthread_mutex_unlock(&device->rgbMutex);		
        FREReleaseByteArray(objectByteArray);
        
        return NULL;
    }
    
    FREObject Kinect_setInfraredMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int width; FREGetObjectAsUint32(argv[1], &width);
        unsigned int height; FREGetObjectAsUint32(argv[2], &height);
        unsigned int mirrored; FREGetObjectAsBool(argv[3], &mirrored);
        kinectDeviceManager.getDevice(nr, ctx)->setInfraredMode(width, height, (mirrored != 0));
        return NULL;
    }
    
    FREObject Kinect_setInfraredEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
        kinectDeviceManager.getDevice(nr, ctx)->setInfraredEnabled((enabled != 0));
        return NULL;
    }
    
    FREObject Kinect_getInfraredFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        
        KinectDevice *device = kinectDeviceManager.getDevice(nr, ctx);
        
        const unsigned int numInfraredBytes = device->getAsInfraredWidth() * device->getAsInfraredHeight() * 4;
        
		FREObject objectByteArray = argv[1];
		FREByteArray byteArray;			
		FREObject length;
		FRENewObjectFromUint32(numInfraredBytes, &length);
		FRESetObjectProperty(objectByteArray, (const uint8_t*) "length", length, NULL);
		FREAcquireByteArray(objectByteArray, &byteArray);
        pthread_mutex_lock(&device->infraredMutex);
		memcpy(byteArray.bytes, device->infraredByteArray, numInfraredBytes);
        pthread_mutex_unlock(&device->infraredMutex);		
        FREReleaseByteArray(objectByteArray);
        
        return NULL;
    }
    
    FREObject Kinect_setPointCloudMode(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int width; FREGetObjectAsUint32(argv[1], &width);
        unsigned int height; FREGetObjectAsUint32(argv[2], &height);
        unsigned int mirrored; FREGetObjectAsBool(argv[3], &mirrored);
        unsigned int density; FREGetObjectAsUint32(argv[4], &density);
        unsigned int includeRGB; FREGetObjectAsBool(argv[5], &includeRGB);
        kinectDeviceManager.getDevice(nr, ctx)->setPointCloudMode(width, height, (mirrored != 0), density, includeRGB);
        return NULL;
    }
    
    FREObject Kinect_setPointCloudEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        unsigned int enabled; FREGetObjectAsBool(argv[1], &enabled);
        kinectDeviceManager.getDevice(nr, ctx)->setPointCloudEnabled((enabled != 0));
        return NULL;
    }
    
    FREObject Kinect_getPointCloudFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        
        KinectDevice *device = kinectDeviceManager.getDevice(nr, ctx);
        
        FREObject objectPointsByteArray = argv[1];
        
        const unsigned int numPointBytes = device->getAsPointCloudByteArrayLength();
        
        FREByteArray pointsByteArray;			
        FREObject pointsLength;
		FRENewObjectFromUint32(numPointBytes, &pointsLength);
		FRESetObjectProperty(objectPointsByteArray, (const uint8_t*) "length", pointsLength, NULL);
		FREAcquireByteArray(objectPointsByteArray, &pointsByteArray);
        pthread_mutex_lock(&device->pointCloudMutex);
		memcpy(pointsByteArray.bytes, device->pointCloudByteArray, numPointBytes);
        pthread_mutex_unlock(&device->pointCloudMutex);		
		FREReleaseByteArray(objectPointsByteArray);
        
        //set the region information?
        FREObject asPointCloudRegions = argv[2];
        if(asPointCloudRegions != NULL && &device->pointCloudRegions != 0)
        {
            //loop through these actionscript regions and get the native info back
            FREObject asPointCloudRegion, asRegionId;
            FREObject asNumPoints;
            unsigned int regionId;
            
            uint32_t numRegions;
            FREGetArrayLength(asPointCloudRegions, &numRegions);
            
            for(int i = 0; i < numRegions; i++)
            {
                FREGetArrayElementAt(asPointCloudRegions, i, &asPointCloudRegion);
                FREGetObjectProperty(asPointCloudRegion, (const uint8_t *) "regionId", &asRegionId, NULL);
                FREGetObjectAsUint32(asRegionId, &regionId);
                //get the region with this id from the device memory
                for(int j = 0; j < device->numRegions; j++)
                {
                    PointCloudRegion *nativeRegion = &device->pointCloudRegions[j];
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
        
        return NULL;
    }
    
    FREObject Kinect_setPointCloudRegions(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[])
    {
        unsigned int nr; FREGetObjectAsUint32(argv[0], &nr);
        
        KinectDevice *device = kinectDeviceManager.getDevice(nr, ctx);
        
        FREObject asPointCloudRegions = argv[1];
        FREObject asPointCloudRegion, asRegionId, asX, asY, asZ, asWidth, asHeight, asDepth;
        unsigned int regionId;
        double x, y, z, width, height, depth;

        uint32_t numRegions;
        FREGetArrayLength(asPointCloudRegions, &numRegions);
        
        PointCloudRegion *nativeRegions = new PointCloudRegion[numRegions];
        
        for(int i = 0; i < numRegions; i++)
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
        
        device->setPointCloudRegions(nativeRegions, numRegions);
        
        return NULL;
    }
    
    FRENamedFunction _Static_methods[] = {
		{ (const uint8_t*) "getDeviceCount", 0, Kinect_getDeviceCount},
		{ (const uint8_t*) "getCapabilities", 0, Kinect_getCapabilities },
        { (const uint8_t*) "applicationShutdown", 0, Kinect_applicationShutdown}
    };
    
	FRENamedFunction _Instance_methods[] = {
		{ (const uint8_t*) "start", 0, Kinect_start },
		{ (const uint8_t*) "stop", 0, Kinect_stop },
        { (const uint8_t*) "setUserMode", 0, Kinect_setUserMode },
		{ (const uint8_t*) "setUserColor", 0, Kinect_setUserColor },
        { (const uint8_t*) "setUserEnabled", 0, Kinect_setUserEnabled },
        { (const uint8_t*) "setSkeletonMode", 0, Kinect_setSkeletonMode },
        { (const uint8_t*) "setSkeletonEnabled", 0, Kinect_setSkeletonEnabled },
        { (const uint8_t*) "getUserFrame", 0, Kinect_getUserFrame },
        { (const uint8_t*) "getSkeletonJointNameIndices", 0, Kinect_getSkeletonJointNameIndices },
        { (const uint8_t*) "getSkeletonJointNames", 0, Kinect_getSkeletonJointNames },
        { (const uint8_t*) "setUserMaskMode", 0, Kinect_setUserMaskMode },
        { (const uint8_t*) "setUserMaskEnabled", 0, Kinect_setUserMaskEnabled },
        { (const uint8_t*) "getUserMaskFrame", 0, Kinect_getUserMaskFrame },
        { (const uint8_t*) "setDepthMode", 0, Kinect_setDepthMode },
        { (const uint8_t*) "setDepthEnabled", 0, Kinect_setDepthEnabled },
        { (const uint8_t*) "getDepthFrame", 0, Kinect_getDepthFrame },
        { (const uint8_t*) "setDepthShowUserColors", 0, Kinect_setDepthShowUserColors },
        { (const uint8_t*) "setRGBMode", 0, Kinect_setRGBMode },
        { (const uint8_t*) "setRGBEnabled", 0, Kinect_setRGBEnabled },
        { (const uint8_t*) "getRGBFrame", 0, Kinect_getRGBFrame },
        { (const uint8_t*) "setInfraredMode", 0, Kinect_setInfraredMode },
        { (const uint8_t*) "setInfraredEnabled", 0, Kinect_setInfraredEnabled },
        { (const uint8_t*) "getInfraredFrame", 0, Kinect_getInfraredFrame },
        { (const uint8_t*) "setPointCloudMode", 0, Kinect_setPointCloudMode },
        { (const uint8_t*) "setPointCloudEnabled", 0, Kinect_setPointCloudEnabled },
        { (const uint8_t*) "getPointCloudFrame", 0, Kinect_getPointCloudFrame },
        { (const uint8_t*) "setPointCloudRegions", 0, Kinect_setPointCloudRegions },
		//{ (const uint8_t*) "setPointCloudDensity", 0, Kinect_setPointCloudDensity },
		//{ (const uint8_t*) "setPointCloudIncludeRGB", 0, Kinect_setPointCloudIncludeRGB },
		//{ (const uint8_t*) "setSkeletonMirror", 0, Kinect_setSkeletonMirror },
		//{ (const uint8_t*) "setImageMirror", 0, Kinect_setImageMirror }
	};
    
    void contextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctions, const FRENamedFunction** functions)
    {
        if ( 0 == strcmp( (const char*) ctxType, "shared" ) )
		{
			*numFunctions = sizeof( _Static_methods ) / sizeof( FRENamedFunction );
			*functions = _Static_methods;
		}
		else
        {
            *numFunctions = sizeof( _Instance_methods ) / sizeof( FRENamedFunction );
            *functions = _Instance_methods;
        }
        
        //make sure the manager is started
        if(!kinectDeviceManager.isStarted())
        {
            kinectDeviceManager.startUp();
        }
	}
    
	void contextFinalizer(FREContext ctx)
    {
		return;
	}
    
    EXPORT
	void initializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer)
    {
		*ctxInitializer = &contextInitializer;
		*ctxFinalizer = &contextFinalizer;
	}
    
    EXPORT
	void finalizer(void* extData)
    {
		return;
	}
}
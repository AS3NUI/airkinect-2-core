//
//  IKinectDevice.h
//  KinectExtension
//
//  Created by Wouter Verweirder on 22/02/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef KinectExtension_IKinectDevice_h
#define KinectExtension_IKinectDevice_h

#include "KinectSkeleton.h"
#include "PointCloudRegion.h"

class IKinectDevice
{
public:
	//Getter/Setters for FREContext
    virtual FREContext			getFreContext() = 0;
	virtual	void				setFreContext(FREContext pFreContext) = 0;
    
	//Starts and Sotp the Kinect Device
    virtual void				start() = 0;
    virtual void				stop() = 0;
    
	//Dispose the Device form memory
    virtual void				dispose() {};
    
    virtual FREObject           freGetCapabilities() { FREObject o; return o; };
    virtual FREObject           freSetUserMode(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetUserColor(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetUserEnabled(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetSkeletonMode(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetSkeletonEnabled(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freGetUserFrame(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freGetSkeletonJointNameIndices(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freGetSkeletonJointNames(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetUserMaskMode(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetUserMaskEnabled(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freGetUserMaskFrame(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetDepthMode(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetDepthEnabled(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freGetDepthFrame(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetDepthShowUserColors(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetRGBMode(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetRGBEnabled(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freGetRGBFrame(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetInfraredMode(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetInfraredEnabled(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freGetInfraredFrame(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetPointCloudMode(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetPointCloudEnabled(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freGetPointCloudFrame(FREObject argv[]) { FREObject o; return o; };
    virtual FREObject           freSetPointCloudRegions(FREObject argv[]) { FREObject o; return o; };
};

#endif

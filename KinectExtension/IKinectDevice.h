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
    
	//Sets the color of the player for Depth + Player index image
	virtual void				setUserColor(int userID, int color, bool useIntensity){ };
    
	//User and Skeleton Feature Setters
    virtual void				setUserMode(bool mirrored) {};
    virtual void				setUserEnabled(bool enabled) {};
    virtual void				setSkeletonMode(bool mirrored) {};
    virtual void				setSkeletonEnabled(bool enabled) {};
	
	//Should return the current User Frame Data
	virtual kinectUserFrame		getUserFrameBuffer() {kinectUserFrame k;  return k;};
    
	//Depth Image Feature Setters
    virtual void				setDepthMode(unsigned int width, unsigned int height, bool mirrored) {};
    virtual void				setDepthEnabled(bool enabled) {};
    virtual void				setDepthShowUserColors(bool show) {};
    
	//RGB Image Feature Setters
    virtual void				setRGBMode(unsigned int width, unsigned int height, bool mirrored) {};
    virtual void				setRGBEnabled(bool enabled) {};
    
	//IR Camera Feature Setters
	virtual void				setInfraredMode(unsigned int width, unsigned int height, bool mirrored) {};
	virtual void				setInfraredEnabled(bool enabled) {};
    
	//User Mask Image Feature Setters
    virtual void				setUserMaskMode(unsigned int width, unsigned int height, bool mirrored) {};
    virtual void				setUserMaskEnabled(bool enabled) {};
    
	//Point Cloud Feature Setters
    virtual void				setPointCloudMode(unsigned int width, unsigned int height, bool mirrored, unsigned int density, bool includeRGB) {};
    virtual void				setPointCloudEnabled(bool enabled) {};
    virtual void				setPointCloudRegions(PointCloudRegion *pointCloudRegions, unsigned int numRegions) {};
	
	//Depth Image Accessors
	virtual int					getAsDepthWidth() {return 0;};
    virtual int					getAsDepthHeight() {return 0;}
	//Returns the current Depth Frame as a byte array
	virtual uint32_t*			getAsDepthByteArray() {return 0;};
    
	//RGB Image Accessors
    virtual int					getAsRGBWidth() {return 0;};
    virtual int					getAsRGBHeight() {return 0;};
	//Returns the current RGB Frame as a byte array
    virtual uint32_t*			getAsRGBByteArray() {return 0;};
    
	//User Mask Image Accessors
    virtual int					getAsUserMaskWidth() {return 0;};
    virtual int					getAsUserMaskHeight() {return 0;};
	//Returns the current Player Mask Frame as a byte array
    virtual uint32_t*			getAsUserMaskByteArray(int userID) {return 0;};
    
	//IR Image Accessors
	virtual int					getAsInfraredWidth() { return  0;};
	virtual int					getAsInfraredHeight() {return 0;};
	virtual uint32_t*           getAsInfraredByteArray() {return 0;};
    
	//Point Cloud Accessors
    virtual int					getAsPointCloudWidth() {return 0;};
    virtual int					getAsPointCloudHeight() {return 0;}; 
	virtual bool				getASPointCloudMirror() {return false;};
	virtual int					getASPointCloudDensity() {return 1;}; 
	virtual bool				getASPointCloudIncludeRGB() {return false;}; 
    virtual int					getAsPointCloudByteArrayLength() {return 0;};
	//Returns the current Point cloud data as byte array (x,y,z) format
	virtual short*             getAsPointCloudByteArray() {return 0;};
    
	//Region Accessors
	//Returns the current poinbt cloud regions
    virtual PointCloudRegion*	getPointCloudRegions() {return 0;};
    virtual unsigned int		getNumRegions() {return 0;};
    
    virtual void                    lockUserMutex(){};
    virtual void                    unlockUserMutex(){};
    
    virtual void                    lockDepthMutex(){};
    virtual void                    unlockDepthMutex(){};
    
    virtual void                    lockRGBMutex(){};
    virtual void                    unlockRGBMutex(){};
    
    virtual void                    lockUserMaskMutex(){};
    virtual void                    unlockUserMaskMutex(){};
    
    virtual void                    lockInfraredMutex(){};
    virtual void                    unlockInfraredMutex(){};
    
    virtual void                    lockPointCloudMutex(){};
    virtual void                    unlockPointCloudMutex(){};
};

#endif

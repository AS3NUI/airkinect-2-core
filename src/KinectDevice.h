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
#ifndef KinectExtension_IKinectDevice_h
#define KinectExtension_IKinectDevice_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "KinectCapabilities.h"
#include "PointCloudRegion.h"

#include "AKImageBytesGenerator.h"
#include "AKPointCloudGenerator.h"
#include "AKUserMasksGenerator.h"
#include "AKUserFrameGenerator.h"

class KinectDevice
{
public:
	//Getter/Setters for FREContext
    virtual FREContext			getFreContext();
	virtual void				setFreContext(FREContext pFreContext);
    
	//Starts and Stops the Kinect Device
    virtual void				start();
    virtual void				stop();
    
	//Dispose the Device from memory
    virtual void				dispose();
    
    virtual FREObject           freGetCapabilities();
    virtual FREObject           freSetUserMode(FREObject argv[]);
    virtual FREObject           freSetUserColor(FREObject argv[]);
    virtual FREObject           freSetUserEnabled(FREObject argv[]);
    virtual FREObject           freSetSkeletonMode(FREObject argv[]);
    virtual FREObject           freSetSkeletonEnabled(FREObject argv[]);
	virtual FREObject			freChooseSkeletons(FREObject argv[]);
    virtual FREObject           freGetUserFrame(FREObject argv[]);
    virtual FREObject           freSetUserMaskMode(FREObject argv[]);
    virtual FREObject           freSetUserMaskEnabled(FREObject argv[]);
    virtual FREObject           freGetUserMaskFrame(FREObject argv[]);
    virtual FREObject           freSetDepthMode(FREObject argv[]);
    virtual FREObject           freSetDepthEnabled(FREObject argv[]);
    virtual FREObject           freGetDepthFrame(FREObject argv[]);
    virtual FREObject           freSetDepthShowUserColors(FREObject argv[]);
	virtual FREObject			freSetNearModeEnabled(FREObject argv[]);
    virtual FREObject           freSetHandTrackingMode(FREObject argv[]);
    virtual FREObject           freSetHandTrackingEnabled(FREObject argv[]);
    virtual FREObject           freSetRGBMode(FREObject argv[]);
    virtual FREObject           freSetRGBEnabled(FREObject argv[]);
    virtual FREObject           freGetRGBFrame(FREObject argv[]);
    virtual FREObject           freSetInfraredMode(FREObject argv[]);
    virtual FREObject           freSetInfraredEnabled(FREObject argv[]);
    virtual FREObject           freGetInfraredFrame(FREObject argv[]);
    virtual FREObject           freSetPointCloudMode(FREObject argv[]);
    virtual FREObject           freSetPointCloudEnabled(FREObject argv[]);
    virtual FREObject           freGetPointCloudFrame(FREObject argv[]);
    virtual FREObject           freSetPointCloudRegions(FREObject argv[]);
	virtual FREObject           freCameraElevationGetAngle(FREObject argv[]);
	virtual FREObject           freCameraElevationSetAngle(FREObject argv[]);
    
    void						dispatchErrorMessage(const uint8_t* errorMessage);
	void						dispatchInfoMessage(const uint8_t* infoMessage);
	void						dispatchStatusMessage(const uint8_t* statusMessage);
	void						trace(const uint8_t* traceMessage);

protected:

	virtual void                lockUserMutex();
    virtual void                unlockUserMutex();
    
    virtual void                lockDepthMutex();
    virtual void                unlockDepthMutex();
    
    virtual void                lockRGBMutex();
    virtual void                unlockRGBMutex();
    
    virtual void                lockUserMaskMutex();
    virtual void                unlockUserMaskMutex();
    
    virtual void                lockInfraredMutex();
    virtual void                unlockInfraredMutex();
    
    virtual void                lockPointCloudMutex();
    virtual void                unlockPointCloudMutex();

	virtual void				setDefaults();
	virtual void				cleanupByteArrays();
	virtual void				setUserColor(int userID, int color, bool useIntensity);

	virtual void				setRGBMode(int rgbWidth, int rgbHeight, int asRGBWidth, int asRGBHeight, bool asRGBMirrored);

	FREContext					freContext;
	KinectCapabilities			capabilities;

	int							nr;
	bool						running;
	bool						started;

	boost::thread				mThread;

	boost::mutex				userMutex;
    boost::mutex				depthMutex;
    boost::mutex				rgbMutex;
    boost::mutex				userMaskMutex;
    boost::mutex				pointCloudMutex;
    boost::mutex                infraredMutex;

    bool						asUserMirrored;
    bool						asUserEnabled;

	bool						asSkeletonMirrored;
    bool						asSkeletonEnabled;
	bool						asSeatedSkeletonEnabled;
	bool						asChooseSkeletonsEnabled;
    double                      asSkeletonSmoothing;

	bool						asUserMaskEnabled;

	bool						asDepthEnabled;
    bool						asDepthShowUserColors;

	bool						asNearModeEnabled;
    
    bool                        asHandTrackingMirrored;
    bool                        asHandTrackingEnabled;

	bool						asInfraredEnabled;

	bool						asRGBEnabled;

	AKImageBytesGenerator*		rgbImageBytesGenerator;
	AKImageBytesGenerator*		depthImageBytesGenerator;
	AKImageBytesGenerator*		infraredImageBytesGenerator;

	AKPointCloudGenerator*		pointCloudGenerator;
	virtual void				createPointCloudGenerator();

	bool						asPointCloudEnabled;

	AKUserFrameGenerator*		userFrameGenerator;
	virtual void				createUserFrameGenerator();

	AKUserMasksGenerator*		userMasksGenerator;
	virtual void				createUserMasksGenerator();

	PointCloudRegion*			pointCloudRegions;
    unsigned int				numRegions;

	int							numChosenSkeletons;
	int*						chosenSkeletonIds;

};

#endif
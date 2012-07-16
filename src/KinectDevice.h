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
#include "KinectSkeleton.h"
#include "PointCloudRegion.h"

#include "AKImageBytesGenerator.h"
#include "AKPointCloudGenerator.h"
#include "AKUserMasksGenerator.h"

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

protected:

	const char					*asJointClass;
	const char					*asUserClass;
	const char					*asUserFrameClass;

	int							numJoints;
	int							maxSkeletons;
	char						**jointNames;

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
	virtual void				setNumJointsAndJointNames();
	virtual void				allocateUserFrame();
	virtual void				deallocateUserFrame();
	virtual void				cleanupByteArrays();
	virtual void				setUserColor(int userID, int color, bool useIntensity);

	virtual void				setRGBMode(int rgbWidth, int rgbHeight, int asRGBWidth, int asRGBHeight, bool asRGBMirrored);

	virtual FREObject           freGetSkeletonJointNameIndices(FREObject argv[]);
    virtual FREObject			freGetSkeletonJointNames(FREObject argv[]);

	FREContext					freContext;
	KinectCapabilities			capabilities;

	void						dispatchErrorMessage(const uint8_t* errorMessage);
	void						dispatchInfoMessage(const uint8_t* infoMessage);
	void						dispatchStatusMessage(const uint8_t* statusMessage);
	void						trace(const uint8_t* traceMessage);

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

	bool						asUserMaskEnabled;

	bool						asDepthEnabled;
    bool						asDepthShowUserColors;

	bool						asNearModeEnabled;

	bool						asRGBEnabled;

	AKImageBytesGenerator*		rgbImageBytesGenerator;
	AKImageBytesGenerator*		depthImageBytesGenerator;

	AKPointCloudGenerator*		pointCloudGenerator;
	virtual void				createPointCloudGenerator();

	bool						asPointCloudEnabled;

	kinectUserFrame				userFrame;

	AKUserMasksGenerator*		userMasksGenerator;
	virtual void				createUserMasksGenerator();

	PointCloudRegion*			pointCloudRegions;
    unsigned int				numRegions;

	int							numChosenSkeletons;
	int*						chosenSkeletonIds;

};

#endif
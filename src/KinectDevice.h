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

class KinectDevice
{
public:
	//Getter/Setters for FREContext
    virtual FREContext			getFreContext();
	virtual void				setFreContext(FREContext pFreContext);
    
	//Starts and Stops the Kinect Device
    virtual void				start();
    virtual void				stop();
    
	//Dispose the Device form memory
    virtual void				dispose();
    
    virtual FREObject           freGetCapabilities();
    virtual FREObject           freSetUserMode(FREObject argv[]);
    virtual FREObject           freSetUserColor(FREObject argv[]);
    virtual FREObject           freSetUserEnabled(FREObject argv[]);
    virtual FREObject           freSetSkeletonMode(FREObject argv[]);
    virtual FREObject           freSetSkeletonEnabled(FREObject argv[]);
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
	virtual void				setUserColor(int userID, int color, bool useIntensity);

	virtual void				setRGBMode(int rgbWidth, int rgbHeight, int asRGBWidth, int asRGBHeight, bool asRGBMirrored);

	virtual FREObject           freGetSkeletonJointNameIndices(FREObject argv[]);
    virtual FREObject			freGetSkeletonJointNames(FREObject argv[]);

	virtual FREObject			createFREPoint(double x, double y);
	virtual FREObject			createFREVector3DFromKinectRotationQuaternion(kinectRotationQuaternion q);
	virtual FREObject			createFREVector3D(double x, double y, double z, double w);
	virtual FREObject			createFREMatrix3DFromKinectRotationMatrix(kinectRotationMatrix m);
	virtual FREObject			createFREMatrix3D(double m11, double m12, double m13, double m14, double m21, double m22, double m23, double m24, double m31, double m32, double m33, double m34, double m41, double m42, double m43, double m44);

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
	
	int							userMaskWidth;
	int							userMaskHeight;
	unsigned int				userMaskPixelCount;
	int							userMaskScale;

	int							depthWidth;
    int							depthHeight;
    unsigned int				depthPixelCount;
	int							depthScale;

	int							rgbWidth;
    int							rgbHeight;
    unsigned int				rgbPixelCount;
	int							rgbScale;

	int							pointCloudWidth;
	int							pointCloudHeight;
	unsigned int				pointCloudPixelCount;
	int							pointCloudScale;

    bool						asUserMirrored;
    bool						asUserEnabled;

	bool						asSkeletonMirrored;
    bool						asSkeletonEnabled;
	bool						asSeatedSkeletonEnabled;

	bool						asUserMaskEnabled;
	int							asUserMaskWidth;
    int							asUserMaskHeight;
    int							asUserMaskPixelCount;
    bool						asUserMaskMirrored;

	bool						asDepthEnabled;
    bool						asDepthShowUserColors;
	int							asDepthWidth;
    int							asDepthHeight;
    int							asDepthPixelCount;
    bool						asDepthMirrored;

	bool						asNearModeEnabled;

	bool						asRGBEnabled;
	int							asRGBWidth;
    int							asRGBHeight;
    int							asRGBPixelCount;
    bool						asRGBMirrored;

	bool						asPointCloudEnabled;
	int							asPointCloudWidth;
    int							asPointCloudHeight;
    int							asPointCloudPixelCount;
    bool						asPointCloudMirrored;
    int							asPointCloudDensity;
    bool						asPointCloudIncludeRGB;

	kinectUserFrame				userFrame;
	uint32_t					**asUserMaskByteArray;
	uint32_t					*asDepthByteArray;
	uint32_t					*asRGBByteArray;
	short						*asPointCloudByteArray;
	PointCloudRegion			*pointCloudRegions;
    unsigned int				numRegions;

};

#endif
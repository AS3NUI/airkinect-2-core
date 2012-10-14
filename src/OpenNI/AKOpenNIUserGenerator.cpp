//
//  AKOpenNIRGBGenerator.cpp
//  KinectExtension
//
//  Created by Wouter Verweirder on 13/10/12.
//
//

#include "AKOpenNIUserGenerator.h"

XnBool _needPose = FALSE;
XnChar _strPose[20] = "";

//USER CALLBACKS
void XN_CALLBACK_TYPE _newUserCallback(xn::UserGenerator& userGenerator, XnUserID nID, void* pCookie);
void XN_CALLBACK_TYPE _lostUserCallback(xn::UserGenerator& userGenerator, XnUserID nID, void* pCookie);
void XN_CALLBACK_TYPE _userExitCallback(xn::UserGenerator& userGenerator, XnUserID nID, void* pCookie);
void XN_CALLBACK_TYPE _userReentryCallback(xn::UserGenerator& userGenerator, XnUserID nID, void* pCookie);
void XN_CALLBACK_TYPE _poseDetectedCallback(xn::PoseDetectionCapability& rCapability, const XnChar* strPose, XnUserID nID, void* pCookie);
void XN_CALLBACK_TYPE _outOfPoseCallback(xn::PoseDetectionCapability& rCapability, const XnChar* strPose, XnUserID nID, void* pCookie);
void XN_CALLBACK_TYPE _calibrationStartCallback(xn::SkeletonCapability& rCapability, XnUserID nID, void* pCookie);
void XN_CALLBACK_TYPE _calibrationCompleteCallback(xn::SkeletonCapability& rCapability, XnUserID nID, XnCalibrationStatus calibrationError, void* pCookie);

AKOpenNIUserGenerator::AKOpenNIUserGenerator(OpenNIDevice* device)
{
    _device = device;
    
    _userGenerator = NULL;
    _userGeneratorCreated = false;
}

AKOpenNIUserGenerator::~AKOpenNIUserGenerator()
{
    stop();
}

void AKOpenNIUserGenerator::setup(xn::Context* xnContext)
{
    _xnContext = xnContext;
    
    _device->dispatchInfoMessage((const uint8_t*) "Creating User Generator");
    _rc = _userGenerator.Create(*_xnContext);
    if(_rc != XN_STATUS_OK)
    {
        _device->dispatchErrorMessage((const uint8_t*) "Create User Generator Failed");
    }
    else
    {
        _userGeneratorCreated = true;
        
        //register skeleton detection callbacks
        _userGenerator.RegisterUserCallbacks(_newUserCallback, _lostUserCallback, this, _userHandle);
        _userGenerator.RegisterToUserExit(_userExitCallback, this, _userExitHandle);
        _userGenerator.RegisterToUserReEnter(_userReentryCallback, this, _userReentryHandle);
        _userGenerator.GetSkeletonCap().RegisterToCalibrationStart(_calibrationStartCallback, this, _calibrationStartHandle);
        _userGenerator.GetSkeletonCap().RegisterToCalibrationComplete(_calibrationCompleteCallback, this, _calibrationCompleteHandle);
        _userGenerator.GetPoseDetectionCap().RegisterToPoseDetected(_poseDetectedCallback, this, _poseDetectedHandle);
        _userGenerator.GetPoseDetectionCap().RegisterToOutOfPose(_outOfPoseCallback, this, _outOfPoseHandle);
        
        _userCallbacksRegistered = true;
        
        if (!_userGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
        {
            _device->dispatchErrorMessage((const uint8_t*) "Supplied user generator doesn't support skeleton");
        }
        
        if (_userGenerator.GetSkeletonCap().NeedPoseForCalibration())
        {
            _needPose = true;
            if (!_userGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
            {
                _device->dispatchErrorMessage((const uint8_t*) "Pose required, but not supported");
            }
            else
            {
                _userGenerator.GetSkeletonCap().GetCalibrationPose(_strPose);
            }
        }
        else
        {
            _needPose = false;
        }
        
        _userGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
        _userGenerator.GetSkeletonCap().SetSmoothing((float) _smoothing);
    }
}

void AKOpenNIUserGenerator::start()
{
    if(_userGeneratorCreated)
    {
        _device->dispatchInfoMessage((const uint8_t*) "Starting User Generator");
        _rc = _userGenerator.StartGenerating();
        if(_rc != XN_STATUS_OK)
        {
            _device->dispatchErrorMessage((const uint8_t*) "Starting User Generator Failed");
        }
    }
}

void AKOpenNIUserGenerator::stop()
{
    if(_userGeneratorCreated && _userGenerator.IsValid())
    {
        if(_userGenerator.IsGenerating())
        {
            //stop generating
            _userGenerator.StopGenerating();
        }
        if(_userGenerator)
        {
            //unregister skeleton detection callbacks
            _userGenerator.UnregisterUserCallbacks(_userHandle);
            _userGenerator.UnregisterFromUserExit(_userExitHandle);
            _userGenerator.UnregisterFromUserReEnter(_userReentryHandle);
            _userGenerator.GetSkeletonCap().UnregisterFromCalibrationStart(_calibrationStartHandle);
            _userGenerator.GetSkeletonCap().UnregisterFromCalibrationComplete(_calibrationCompleteHandle);
            _userGenerator.GetPoseDetectionCap().UnregisterFromPoseDetected(_poseDetectedHandle);
            _userGenerator.GetPoseDetectionCap().UnregisterFromOutOfPose(_outOfPoseHandle);
        }
        _userGenerator.Release();
        _userGenerator = NULL;
    }
}

bool AKOpenNIUserGenerator::update()
{
    if(isGenerating())
    {
        _userGenerator.WaitAndUpdateData();
        _userGenerator.GetUserPixels(0, _sceneMetaData);
    }
    return true;
}

void AKOpenNIUserGenerator::parse()
{
    if(isGenerating())
    {
    }
}

void AKOpenNIUserGenerator::dispatchErrorMessage(const uint8_t* errorMessage)
{
    _device->dispatchErrorMessage(errorMessage);
}

void AKOpenNIUserGenerator::dispatchInfoMessage(const uint8_t* infoMessage)
{
    _device->dispatchInfoMessage(infoMessage);
}

void AKOpenNIUserGenerator::dispatchStatusMessage(const uint8_t* statusMessage)
{
    _device->dispatchStatusMessage(statusMessage);
}

void AKOpenNIUserGenerator::trace(const uint8_t* traceMessage)
{
    _device->trace(traceMessage);
}

//GET / SET

bool AKOpenNIUserGenerator::userGeneratorCreated()
{
    return _userGeneratorCreated;
}

bool AKOpenNIUserGenerator::isGenerating()
{
    return (_userGeneratorCreated && _userGenerator.IsGenerating());
}

xn::UserGenerator* AKOpenNIUserGenerator::getXnUserGenerator()
{
    return &_userGenerator;
}

xn::SceneMetaData* AKOpenNIUserGenerator::getXnSceneMetaData()
{
    return &_sceneMetaData;
}

void AKOpenNIUserGenerator::setSmoothing(double smoothing)
{
    if(_smoothing != smoothing)
    {
        _smoothing = smoothing;
        if(_userGeneratorCreated)
        {
            _userGenerator.GetSkeletonCap().SetSmoothing((float) _smoothing);
        }
    }
}

//USER CALLBACKS

void XN_CALLBACK_TYPE _newUserCallback(xn::UserGenerator& userGenerator, XnUserID nID, void* pCookie)
{
    AKOpenNIUserGenerator* thisCookie = static_cast<AKOpenNIUserGenerator*>(pCookie);
	XnStatus status;
    if(_needPose)
    {
        status = userGenerator.GetPoseDetectionCap().StartPoseDetection(_strPose, nID);
        char * buffer = new char[128];
        sprintf(buffer, "New user %i, waiting for calibration pose... status: %s", nID, xnGetStatusString(status));
        thisCookie->dispatchInfoMessage((const uint8_t *)buffer);
        delete buffer;
    }
    else
    {
        userGenerator.GetSkeletonCap().RequestCalibration(nID, TRUE);
    }
	
}

void XN_CALLBACK_TYPE _lostUserCallback(xn::UserGenerator& userGenerator, XnUserID nID, void* pCookie)
{
    AKOpenNIUserGenerator* thisCookie = static_cast<AKOpenNIUserGenerator*>(pCookie);
	XnStatus status;
	status = userGenerator.GetSkeletonCap().StopTracking(nID);
	status = userGenerator.GetSkeletonCap().Reset(nID);
    
    char * buffer = new char[128];
	sprintf(buffer, "Stopped tracking of user %i... status: %s", nID, xnGetStatusString(status));
    thisCookie->dispatchInfoMessage((const uint8_t *)buffer);
    delete buffer;
}

void XN_CALLBACK_TYPE _userExitCallback(xn::UserGenerator& userGenerator, XnUserID nID, void* pCookie)
{
    AKOpenNIUserGenerator* thisCookie = static_cast<AKOpenNIUserGenerator*>(pCookie);
	XnStatus status;
	status = userGenerator.GetSkeletonCap().Reset(nID);
    
    char * buffer = new char[128];
	sprintf(buffer, "User %i exited - resetting skeleton data... status: %s", nID, xnGetStatusString(status));
    thisCookie->dispatchInfoMessage((const uint8_t *)buffer);
    delete buffer;
}

void XN_CALLBACK_TYPE _userReentryCallback(xn::UserGenerator& userGenerator, XnUserID nID, void* pCookie)
{
    AKOpenNIUserGenerator* thisCookie = static_cast<AKOpenNIUserGenerator*>(pCookie);
	XnStatus status;
	XnChar calibrationPose[20];
	
	status = userGenerator.GetSkeletonCap().GetCalibrationPose(calibrationPose);
	status = userGenerator.GetPoseDetectionCap().StartPoseDetection(calibrationPose, nID);
    
    char * buffer = new char[128];
	sprintf(buffer, "User %i reentered, waiting for calibration pose... status: %s", nID, xnGetStatusString(status));
    thisCookie->dispatchInfoMessage((const uint8_t *)buffer);
    delete buffer;
}

void XN_CALLBACK_TYPE _poseDetectedCallback(xn::PoseDetectionCapability& rCapability, const XnChar* strPose, XnUserID nID, void* pCookie)
{
    AKOpenNIUserGenerator* thisCookie = static_cast<AKOpenNIUserGenerator*>(pCookie);
	XnStatus status;
	
	status = thisCookie->getXnUserGenerator()->GetPoseDetectionCap().StopPoseDetection(nID);
	status = thisCookie->getXnUserGenerator()->GetSkeletonCap().RequestCalibration(nID, true);
    
    char * buffer = new char[128];
	sprintf(buffer, "Pose detected for user %i, starting calibration... status: %s", nID, xnGetStatusString(status));
    thisCookie->dispatchInfoMessage((const uint8_t *)buffer);
    delete buffer;
}

void XN_CALLBACK_TYPE _outOfPoseCallback(xn::PoseDetectionCapability& rCapability, const XnChar* strPose, XnUserID nID, void* pCookie)
{
    AKOpenNIUserGenerator* thisCookie = static_cast<AKOpenNIUserGenerator*>(pCookie);
	XnStatus status;
	XnChar calibrationPose[20];
	status = thisCookie->getXnUserGenerator()->GetSkeletonCap().GetCalibrationPose(calibrationPose);
	status = thisCookie->getXnUserGenerator()->GetPoseDetectionCap().StartPoseDetection(calibrationPose, nID);
    
    char * buffer = new char[128];
	sprintf(buffer, "Out of pose, waiting for calibration pose of user %i... status: %s", nID, xnGetStatusString(status));
    thisCookie->dispatchInfoMessage((const uint8_t *)buffer);
    delete buffer;
}

void XN_CALLBACK_TYPE _calibrationStartCallback(xn::SkeletonCapability& rCapability, XnUserID nID, void* pCookie)
{
    AKOpenNIUserGenerator* thisCookie = static_cast<AKOpenNIUserGenerator*>(pCookie);
    
    char * buffer = new char[128];
	sprintf(buffer, "Starting calibration of user %i", nID);
    thisCookie->dispatchInfoMessage((const uint8_t *)buffer);
    delete buffer;
}

void XN_CALLBACK_TYPE _calibrationCompleteCallback(xn::SkeletonCapability& rCapability, XnUserID nID, XnCalibrationStatus calibrationError, void* pCookie)
{
    AKOpenNIUserGenerator* thisCookie = static_cast<AKOpenNIUserGenerator*>(pCookie);
	XnStatus status;
	
	if (calibrationError == XN_STATUS_OK)
	{
		status = thisCookie->getXnUserGenerator()->GetSkeletonCap().StartTracking(nID);
        
        char * buffer = new char[128];
        sprintf(buffer, "Calibration success, beginning to track user %i now... status: %s", nID, xnGetStatusString(status));
        thisCookie->dispatchInfoMessage((const uint8_t *)buffer);
        delete buffer;
    }
	else
	{
		XnChar calibrationPose[20];
		status = thisCookie->getXnUserGenerator()->GetSkeletonCap().GetCalibrationPose(calibrationPose);
		status = thisCookie->getXnUserGenerator()->GetPoseDetectionCap().StartPoseDetection(calibrationPose, nID);
        
        char * buffer = new char[128];
        sprintf(buffer, "Calibration failure for user %i, waiting for calibration pose... status: %s", nID, xnGetStatusString(status));
        thisCookie->dispatchInfoMessage((const uint8_t *)buffer);
        delete buffer;
	}
}
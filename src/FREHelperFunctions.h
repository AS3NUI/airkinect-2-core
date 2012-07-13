#ifndef KinectExtension_FREHelperFunctions_h
#define KinectExtension_FREHelperFunctions_h

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

#include "KinectSkeleton.h"

FREObject createFREPoint(double x, double y);
FREObject createFREVector3DFromKinectRotationQuaternion(kinectRotationQuaternion q);
FREObject createFREVector3D(double x, double y, double z, double w);
FREObject createFREMatrix3DFromKinectRotationMatrix(kinectRotationMatrix m);
FREObject createFREMatrix3D(double m11, double m12, double m13, double m14, double m21, double m22, double m23, double m24, double m31, double m32, double m33, double m34, double m41, double m42, double m43, double m44);

bool createBoolFromFREObject(FREObject freObject);

#endif
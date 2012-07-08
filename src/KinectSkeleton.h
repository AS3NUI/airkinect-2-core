#ifndef KinectExtension_KinectUser_h
#define KinectExtension_KinectUser_h

#include "ExtensionConfig.h"

#ifndef _kinectTransform_
#define _kinectTransform_
typedef struct _kinectTransform {
	double              worldX;
    double              worldY;
    double              worldZ;
    
    double              worldRelativeX;
    double              worldRelativeY;
    double              worldRelativeZ;
    
    int                 rgbX;
    int                 rgbY;
    
    double              rgbRelativeX;
    double              rgbRelativeY;
    
    int                 depthX;
    int                 depthY;
    
    double              depthRelativeX;
    double              depthRelativeY;
} kinectTransform;
#endif _kinectTransform_

#ifndef _kinectRotationQuaternion_
#define _kinectRotationQuaternion_
typedef struct _kinectRotationQuaternion {
    double x;
    double y;
    double z;
    double w;
} kinectRotationQuaternion;
#endif _kinectRotationQuaternion_

#ifndef _kinectRotationMatrix_
#define _kinectRotationMatrix_
typedef struct _kinectRotationMatrix {
    double M11;
    double M12;
    double M13;
    double M14;
    double M21;
    double M22;
    double M23;
    double M24;
    double M31;
    double M32;
    double M33;
    double M34;
    double M41;
    double M42;
    double M43;
    double M44;
} kinectRotationMatrix;
#endif _kinectRotationMatrix_

#ifndef _kinectOrientation_
#define _kinectOrientation_
typedef struct _kinectOrientation {
    kinectRotationMatrix rotationMatrix;
    kinectRotationQuaternion rotationQuaternion;
} kinectOrientation;
#endif _kinectOrientation_

#ifndef _kinectSkeletonJoint_
#define _kinectSkeletonJoint_
typedef struct _kinectSkeletonJoint : kinectTransform{
    double              positionConfidence;
    
    double              orientationConfidence;

	kinectOrientation	hierarchicalOrientation;
	kinectOrientation	absoluteOrientation;

} kinectSkeletonJoint;
#endif _kinectSkeletonJoint_

#ifndef _kinectUser_
#define _kinectUser_
typedef struct _kinectUser: kinectTransform{
    int                 trackingID;
	int                 userID;
    bool                isTracking;
    
    bool                hasSkeleton;
    kinectSkeletonJoint *joints;
} kinectUser;
#endif _kinectUser_

#ifndef _kinectUserFrame_
#define _kinectUserFrame_
typedef struct _kinectUserFrame{
    int                 frameNumber;
    int                 timeStamp;
	kinectUser          *users;
} kinectUserFrame;
#endif _kinectUserFrame_

#endif
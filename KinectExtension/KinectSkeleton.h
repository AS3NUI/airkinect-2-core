//
//  KinectUser.h
//  KinectExtension
//
//  Created by Wouter Verweirder on 22/01/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef KinectExtension_KinectUser_h
#define KinectExtension_KinectUser_h

#include "ExtensionConfig.h"

struct kinectTransform {
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
};

struct kinectSkeletonJoint : kinectTransform{
    double              positionConfidence;
    
    double              orientationConfidence;
    
    double              orientationX;
    double              orientationY;
    double              orientationZ;
};

struct kinectUser: kinectTransform{
    int                 trackingID;
	int                 userID;
    bool                isTracking;
    
    bool                hasSkeleton;
    kinectSkeletonJoint joints[NUM_JOINTS];
};

typedef struct {
    int                 frameNumber;
    int                 timeStamp;
	kinectUser          users[MAX_SKELETONS];
} kinectUserFrame;

#endif
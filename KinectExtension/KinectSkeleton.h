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

const char JOINT_NAMES[][16] = {
    "head",
    "neck",
    "torso",
    "waist",
    
    "left_collar",
    "left_shoulder",
    "left_elbow",
    "left_wrist",
    "left_hand",
    "left_fingertip",
    
    "right_collar",
    "right_shoulder",
    "right_elbow",
    "right_wrist",
    "right_hand",
    "right_fingertip",
    
    "left_hip",
    "left_knee",
    "left_ankle",
    "left_foot",
    
    "right_hip",
    "right_knee",
    "right_ankle",
    "right_foot"
};

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
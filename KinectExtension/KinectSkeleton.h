//
//  KinectUser.h
//  KinectExtension
//
//  Created by Wouter Verweirder on 22/01/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef KinectExtension_KinectUser_h
#define KinectExtension_KinectUser_h

const int MAX_SKELETONS = 15;
const int NUM_JOINTS = 24;

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

typedef struct {
    float               positionConfidence;
    
    float               worldX;
    float               worldY;
    float               worldZ;
    
    float               worldRelativeX;
    float               worldRelativeY;
    float               worldRelativeZ;
    
    float               orientationConfidence;
    
    float               orientationX;
    float               orientationY;
    float               orientationZ;
    
    int                 rgbX;
    int                 rgbY;
    
    float               rgbRelativeX;
    float               rgbRelativeY;
    
    int                 depthX;
    int                 depthY;
    
    float               depthRelativeX;
    float               depthRelativeY;
} kinectSkeletonJoint;

typedef struct {
    int                 trackingID;
    bool                isTracking;
    
    float               worldX;
    float               worldY;
    float               worldZ;
    
    float               worldRelativeX;
    float               worldRelativeY;
    float               worldRelativeZ;
    
    int                 rgbX;
    int                 rgbY;
    
    float               rgbRelativeX;
    float               rgbRelativeY;
    
    int                 depthX;
    int                 depthY;
    
    float               depthRelativeX;
    float               depthRelativeY;
    
    bool                hasSkeleton;
    kinectSkeletonJoint joints[NUM_JOINTS];
} kinectUser;

typedef struct {
    int                 frameNumber;
    int                 timeStamp;
    kinectUser      users[MAX_SKELETONS];
} kinectUserFrame;

#endif

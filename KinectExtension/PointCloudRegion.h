//
//  PointCloudRegion.h
//  KinectExtension
//
//  Created by Wouter Verweirder on 07/02/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef KinectExtension_PointCloudRegion_h
#define KinectExtension_PointCloudRegion_h

class PointCloudRegion
{
public:
    PointCloudRegion();
    void                    setProperties(unsigned int regionId, float x, float y, float z, float width, float height, float depth);
    
    unsigned int            numPoints;
    
    unsigned int            regionId;
    
    float                   x;
    float                   y;
    float                   z;
    float                   width;
    float                   height;
    float                   depth;
    
    float                   maxX;
    float                   maxY;
    float                   maxZ;
};

#endif

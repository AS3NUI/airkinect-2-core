//
//  PointCloudRegion.cpp
//  KinectExtension
//
//  Created by Wouter Verweirder on 07/02/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "PointCloudRegion.h"

PointCloudRegion::PointCloudRegion()
{
}

void PointCloudRegion::setProperties(unsigned int regionId, float x, float y, float z, float width, float height, float depth)
{
    this->regionId = regionId;
    this->x = x;
    this->y = y;
    this->z = z;
    this->width = width;
    this->height = height;
    this->depth = depth;
    
    maxX = x + width;
    maxY = y + height;
    maxZ = z + depth;
}

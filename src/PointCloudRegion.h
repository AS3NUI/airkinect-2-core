/*
 * Copyright 2012 AS3NUI
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#ifndef KinectExtension_PointCloudRegion_h
#define KinectExtension_PointCloudRegion_h

class PointCloudRegion
{
public:
    PointCloudRegion();
    void                    setProperties(unsigned int regionId, double x, double y, double z, double width, double height, double depth);
    
    unsigned int            numPoints;
    
    unsigned int            regionId;
    
    double                  x;
    double                  y;
    double                  z;
    double                  width;
    double                  height;
    double                  depth;

    double                  maxX;
    double                  maxY;
    double                  maxZ;
};

#endif

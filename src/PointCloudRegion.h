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

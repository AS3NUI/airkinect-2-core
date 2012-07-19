#ifndef KinectExtension_AKStructs_h
#define KinectExtension_AKStructs_h

#include "ExtensionConfig.h"

#ifdef AIRKINECT_OS_WINDOWS
    #include "FlashRuntimeExtensions.h"
#else
    #include <Adobe AIR/Adobe AIR.h>
#endif

//
// This structure describes a point in 2D space
//
#ifndef _AKPoint2D_
#define _AKPoint2D_
typedef struct _AKPoint2D
{
	double x;
	double y;

	void create(double x, double y)
	{
		this->x = x;
		this->y = y;
	};

	FREObject asFREObject()
	{
		FREObject point;
		FREObject freX, freY;
		FRENewObjectFromDouble(x, &freX);
		FRENewObjectFromDouble(y, &freY);
		FREObject pointParams[] = {freX, freY};
		FRENewObject( (const uint8_t*) "flash.geom.Point", 2, pointParams, &point, NULL);
		return point;
	};

} AKPoint2D;
#endif _AKPoint2D_

//
// This structure describes a point in 3D space
//
#ifndef _AKPoint3D_
#define _AKPoint3D_
typedef struct _AKPoint3D : AKPoint2D
{
	double z;

	void create(double x, double y, double z)
	{
		AKPoint2D::create(x, y);
		this->z = z;
	}

	FREObject asFREObject()
	{
		FREObject vector;
		FREObject freX, freY, freZ;
		FRENewObjectFromDouble(x, &freX);
		FRENewObjectFromDouble(y, &freY);
		FRENewObjectFromDouble(z, &freZ);
		FREObject vectorParams[] = {freX, freY, freZ};
		FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 3, vectorParams, &vector, NULL);
		return vector;
	};

} AKPoint3D;
#endif _AKPoint3D_

//
// This structure describes a position in the 3 coordinate spaces
//
#ifndef _AKPosition_
#define _AKPosition_
typedef struct _AKPosition 
{
	AKPoint3D			world;
	AKPoint3D			worldRelative;

	AKPoint2D			rgb;
	AKPoint2D			rgbRelative;

	AKPoint2D			depth;
	AKPoint2D			depthRelative;

	FREObject asFREObject()
	{
		FREObject position;
		FRENewObject( (const uint8_t*) "com.as3nui.nativeExtensions.air.kinect.data.Position", 0, NULL, &position, NULL);

		FRESetObjectProperty(position, (const uint8_t*) "world", world.asFREObject(), NULL);
		FRESetObjectProperty(position, (const uint8_t*) "worldRelative", worldRelative.asFREObject(), NULL);

		FRESetObjectProperty(position, (const uint8_t*) "rgb", rgb.asFREObject(), NULL);
		FRESetObjectProperty(position, (const uint8_t*) "rgbRelative", rgbRelative.asFREObject(), NULL);

		FRESetObjectProperty(position, (const uint8_t*) "depth", depth.asFREObject(), NULL);
		FRESetObjectProperty(position, (const uint8_t*) "depthRelative", depthRelative.asFREObject(), NULL);

		return position;
	}
} AKPosition;
#endif _AKPosition_

//
// This structure describes a quaternion
//
#ifndef _AKQuaternion_
#define _AKQuaternion_
typedef struct _AKQuaternion : AKPoint3D
{
    double w;

	void create(double x, double y, double z, double w)
	{
		AKPoint3D::create(x, y, z);
		this->w = w;
	}

	FREObject asFREObject()
	{
		FREObject vector;
		FREObject freX, freY, freZ, freW;
		FRENewObjectFromDouble(x, &freX);
		FRENewObjectFromDouble(y, &freY);
		FRENewObjectFromDouble(z, &freZ);
		FRENewObjectFromDouble(w, &freW);
		FREObject vectorParams[] = {freX, freY, freZ, freW};
		FRENewObject( (const uint8_t*) "flash.geom.Vector3D", 4, vectorParams, &vector, NULL);
		return vector;
	};

} AKQuaternion;
#endif _AKQuaternion_

//
// This structure describes a rotation matrix
//
#ifndef _AKMatrix3D_
#define _AKMatrix3D_
typedef struct _AKMatrix3D
{
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

	void create(
		double M11, double M12, double M13, double M14,
		double M21, double M22, double M23, double M24,
		double M31, double M32, double M33, double M34,
		double M41, double M42, double M43, double M44
	)
	{
		this->M11 = M11;
		this->M12 = M12;
		this->M13 = M13;
		this->M14 = M14;
		this->M21 = M21;
		this->M22 = M22;
		this->M23 = M23;
		this->M24 = M24;
		this->M31 = M31;
		this->M32 = M32;
		this->M33 = M33;
		this->M34 = M34;
		this->M41 = M41;
		this->M42 = M42;
		this->M43 = M43;
		this->M44 = M44;
	};

	FREObject asFREObject()
	{
		FREObject freM11, freM12, freM13, freM14;
		FRENewObjectFromDouble(M11, &freM11);
		FRENewObjectFromDouble(M12, &freM12);
		FRENewObjectFromDouble(M13, &freM13);
		FRENewObjectFromDouble(M14, &freM14);

		FREObject freM21, freM22, freM23, freM24;
		FRENewObjectFromDouble(M21, &freM21);
		FRENewObjectFromDouble(M22, &freM22);
		FRENewObjectFromDouble(M23, &freM23);
		FRENewObjectFromDouble(M24, &freM24);

		FREObject freM31, freM32, freM33, freM34;
		FRENewObjectFromDouble(M31, &freM31);
		FRENewObjectFromDouble(M32, &freM32);
		FRENewObjectFromDouble(M33, &freM33);
		FRENewObjectFromDouble(M34, &freM34);

		FREObject freM41, freM42, freM43, freM44;
		FRENewObjectFromDouble(M41, &freM41);
		FRENewObjectFromDouble(M42, &freM42);
		FRENewObjectFromDouble(M43, &freM43);
		FRENewObjectFromDouble(M44, &freM44);

		FREObject freMatrixNumbers;
		FRENewObject( (const uint8_t*) "Vector.<Number>", 0, NULL, &freMatrixNumbers, NULL);

		FRESetArrayElementAt(freMatrixNumbers, 0, freM11);
		FRESetArrayElementAt(freMatrixNumbers, 1, freM12);
		FRESetArrayElementAt(freMatrixNumbers, 2, freM13);
		FRESetArrayElementAt(freMatrixNumbers, 3, freM14);

		FRESetArrayElementAt(freMatrixNumbers, 4, freM21);
		FRESetArrayElementAt(freMatrixNumbers, 5, freM22);
		FRESetArrayElementAt(freMatrixNumbers, 6, freM23);
		FRESetArrayElementAt(freMatrixNumbers, 7, freM24);

		FRESetArrayElementAt(freMatrixNumbers, 8, freM31);
		FRESetArrayElementAt(freMatrixNumbers, 9, freM32);
		FRESetArrayElementAt(freMatrixNumbers, 10, freM33);
		FRESetArrayElementAt(freMatrixNumbers, 11, freM34);

		FRESetArrayElementAt(freMatrixNumbers, 12, freM41);
		FRESetArrayElementAt(freMatrixNumbers, 13, freM42);
		FRESetArrayElementAt(freMatrixNumbers, 14, freM43);
		FRESetArrayElementAt(freMatrixNumbers, 15, freM44);

		FREObject matrix;
		FREObject matrixParams[] = {freMatrixNumbers};
		FRENewObject( (const uint8_t*) "flash.geom.Matrix3D", 1, matrixParams, &matrix, NULL);

		return matrix;
	};

} AKMatrix3D;
#endif _AKMatrix3D_

#endif
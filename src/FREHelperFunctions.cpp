#include "FREHelperFunctions.h"

FREObject createFREPoint(double x, double y)
{
	FREObject point;
	FREObject freX, freY;
	FRENewObjectFromDouble(x, &freX);
    FRENewObjectFromDouble(y, &freY);
    FREObject pointParams[] = {freX, freY};
    FRENewObject( (const uint8_t*) "flash.geom.Point", 2, pointParams, &point, NULL);
	return point;
}

FREObject createFREVector3DFromKinectRotationQuaternion(kinectRotationQuaternion q)
{
	return createFREVector3D(q.x, q.y, q.z, q.w);
}

FREObject createFREVector3D(double x, double y, double z, double w)
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
}

FREObject createFREMatrix3DFromKinectRotationMatrix(kinectRotationMatrix m)
{
	return createFREMatrix3D(
		m.M11,
		m.M12,
		m.M13,
		m.M14,
		m.M21,
		m.M22,
		m.M23,
		m.M24,
		m.M31,
		m.M32,
		m.M33,
		m.M34,
		m.M41,
		m.M42,
		m.M43,
		m.M44);
}

FREObject createFREMatrix3D(double m11, double m12, double m13, double m14, double m21, double m22, double m23, double m24, double m31, double m32, double m33, double m34, double m41, double m42, double m43, double m44)
{
	FREObject freM11, freM12, freM13, freM14;
	FRENewObjectFromDouble(m11, &freM11);
	FRENewObjectFromDouble(m12, &freM12);
	FRENewObjectFromDouble(m13, &freM13);
	FRENewObjectFromDouble(m14, &freM14);

	FREObject freM21, freM22, freM23, freM24;
	FRENewObjectFromDouble(m21, &freM21);
	FRENewObjectFromDouble(m22, &freM22);
	FRENewObjectFromDouble(m23, &freM23);
	FRENewObjectFromDouble(m24, &freM24);

	FREObject freM31, freM32, freM33, freM34;
	FRENewObjectFromDouble(m31, &freM31);
	FRENewObjectFromDouble(m32, &freM32);
	FRENewObjectFromDouble(m33, &freM33);
	FRENewObjectFromDouble(m34, &freM34);

	FREObject freM41, freM42, freM43, freM44;
	FRENewObjectFromDouble(m41, &freM41);
	FRENewObjectFromDouble(m42, &freM42);
	FRENewObjectFromDouble(m43, &freM43);
	FRENewObjectFromDouble(m44, &freM44);

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
}

bool createBoolFromFREObject(FREObject freObject)
{
	unsigned int value;
	FREGetObjectAsBool(freObject, &value);
    return (value != 0);
}
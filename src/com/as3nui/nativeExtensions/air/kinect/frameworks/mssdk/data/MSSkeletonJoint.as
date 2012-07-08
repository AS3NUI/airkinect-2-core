package com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data
{
	import com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint;
	
	import flash.geom.Matrix3D;
	import flash.geom.Point;
	import flash.geom.Vector3D;
	
	public class MSSkeletonJoint extends SkeletonJoint
	{
		public function MSSkeletonJoint(
			name:String, 
			position:Vector3D,
			positionRelative:Vector3D, 
			positionConfidence:Number,
			absoluteOrientationMatrix:Matrix3D, 
			absoluteOrientationQuaternion:Vector3D, 
			hierarchicalOrientationMatrix:Matrix3D, 
			hierarchicalOrientationQuaternion:Vector3D, 
			orientationConfidence:Number, 
			rgbPosition:Point, 
			rgbRelativePosition:Point, 
			depthPosition:Point, 
			depthRelativePosition:Point)
		{
			super(name, position, positionRelative, positionConfidence, absoluteOrientationMatrix, absoluteOrientationQuaternion, hierarchicalOrientationMatrix, hierarchicalOrientationQuaternion, orientationConfidence, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition);
		}
	}
}
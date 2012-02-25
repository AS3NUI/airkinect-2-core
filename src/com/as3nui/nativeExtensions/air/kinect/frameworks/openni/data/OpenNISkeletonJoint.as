package com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data
{
	import com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint;
	
	import flash.geom.Point;
	import flash.geom.Vector3D;
	
	public class OpenNISkeletonJoint extends SkeletonJoint
	{
		public function OpenNISkeletonJoint(name:String, position:Vector3D, positionRelative:Vector3D, positionConfidence:Number, orientation:Vector3D, orientationConfidence:Number, rgbPosition:Point, rgbRelativePosition:Point, depthPosition:Point, depthRelativePosition:Point)
		{
			super(name, position, positionRelative, positionConfidence, orientation, orientationConfidence, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition);
		}
	}
}
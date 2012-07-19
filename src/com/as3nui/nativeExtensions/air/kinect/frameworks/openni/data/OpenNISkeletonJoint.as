package com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data
{
	import com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint;
	
	import flash.geom.Matrix3D;
	
	public class OpenNISkeletonJoint extends SkeletonJoint
	{
		
		public var nativeOrientationConfidence:Number;
		public var nativeOrientation:Matrix3D;
		
		public function OpenNISkeletonJoint()
		{
		}
		
		override public function copyFrom(otherJoint:SkeletonJoint):void
		{
			super.copyFrom(otherJoint);
			
			nativeOrientationConfidence = (otherJoint as OpenNISkeletonJoint).nativeOrientationConfidence;
			nativeOrientation ||= new Matrix3D();
			nativeOrientation.copyFrom((otherJoint as OpenNISkeletonJoint).nativeOrientation);
		}
	}
}
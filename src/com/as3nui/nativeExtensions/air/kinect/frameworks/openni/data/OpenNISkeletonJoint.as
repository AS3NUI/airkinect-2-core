package com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data
{
	import com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint;
	
	import flash.geom.Matrix3D;

	public class OpenNISkeletonJoint extends SkeletonJoint
	{
		
		public static const HEAD:String = "head";
		public static const NECK:String = "neck";
		public static const TORSO:String = "torso";
		
		public static const LEFT_SHOULDER:String = "left_shoulder";
		public static const LEFT_ELBOW:String = "left_elbow";
		public static const LEFT_HAND:String = "left_hand";
		
		public static const RIGHT_SHOULDER:String = "right_shoulder";
		public static const RIGHT_ELBOW:String = "right_elbow";
		public static const RIGHT_HAND:String = "right_hand";
		
		public static const LEFT_HIP:String = "left_hip";
		public static const LEFT_KNEE:String = "left_knee";
		public static const LEFT_FOOT:String = "left_foot";
		
		public static const RIGHT_HIP:String = "right_hip";
		public static const RIGHT_KNEE:String = "right_knee";
		public static const RIGHT_FOOT:String = "right_foot";
		
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
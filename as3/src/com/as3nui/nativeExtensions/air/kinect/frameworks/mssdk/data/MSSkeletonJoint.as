package com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data
{
	import com.as3nui.nativeExtensions.air.kinect.data.Quaternion;
	import com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint;
	
	import flash.geom.Matrix3D;
	
	public class MSSkeletonJoint extends SkeletonJoint
	{
		
		public static const WAIST:String = "waist";
		public static const TORSO:String = "torso";
		public static const NECK:String = "neck";
		public static const HEAD:String = "head";
		
		public static const LEFT_SHOULDER:String = "left_shoulder";
		public static const LEFT_ELBOW:String = "left_elbow";
		public static const LEFT_WRIST:String = "left_wrist";
		public static const LEFT_HAND:String = "left_hand";
		
		public static const RIGHT_SHOULDER:String = "right_shoulder";
		public static const RIGHT_ELBOW:String = "right_elbow";
		public static const RIGHT_WRIST:String = "right_wrist";
		public static const RIGHT_HAND:String = "right_hand";
		
		public static const LEFT_HIP:String = "left_hip";
		public static const LEFT_KNEE:String = "left_knee";
		public static const LEFT_ANKLE:String = "left_ankle";
		public static const LEFT_FOOT:String = "left_foot";
		
		public static const RIGHT_HIP:String = "right_hip";
		public static const RIGHT_KNEE:String = "right_knee";
		public static const RIGHT_ANKLE:String = "right_ankle";
		public static const RIGHT_FOOT:String = "right_foot";
		
		public var nativeHierarchicalRotationQuaternion:Quaternion;
		public var nativeHierarchicalRotationMatrix:Matrix3D;
		public var nativeAbsoluteRotationQuaternion:Quaternion;
		public var nativeAbsoluteRotationMatrix:Matrix3D;
		
		public function MSSkeletonJoint()
		{
		}
		
		override public function copyFrom(otherJoint:SkeletonJoint):void
		{
			super.copyFrom(otherJoint);
			
			nativeHierarchicalRotationQuaternion ||= new Quaternion();
			nativeHierarchicalRotationQuaternion.copyFrom((otherJoint as MSSkeletonJoint).nativeHierarchicalRotationQuaternion);
			
			nativeHierarchicalRotationMatrix ||= new Matrix3D();
			nativeHierarchicalRotationMatrix.copyFrom((otherJoint as MSSkeletonJoint).nativeHierarchicalRotationMatrix);
			
			nativeAbsoluteRotationQuaternion ||= new Quaternion();
			nativeAbsoluteRotationQuaternion.copyFrom((otherJoint as MSSkeletonJoint).nativeAbsoluteRotationQuaternion);
			
			nativeAbsoluteRotationMatrix ||= new Matrix3D();
			nativeAbsoluteRotationMatrix.copyFrom((otherJoint as MSSkeletonJoint).nativeAbsoluteRotationMatrix);
		}
	}
}
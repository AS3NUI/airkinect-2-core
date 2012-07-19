package com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data
{
	import com.as3nui.nativeExtensions.air.kinect.data.Quaternion;
	import com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint;
	
	import flash.geom.Matrix3D;
	
	public class MSSkeletonJoint extends SkeletonJoint
	{
		
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
package com.as3nui.nativeExtensions.air.kinect.data
{
	public class SkeletonBone
	{
		public static const NECK:String = "neck";
		public static const SPINE:String = "spine";
		
		public static const LEFT_UPPER_ARM:String = "left_upper_arm";
		public static const LEFT_LOWER_ARM:String = "left_lower_arm";
		
		public static const RIGHT_UPPER_ARM:String = "right_upper_arm";
		public static const RIGHT_LOWER_ARM:String = "right_lower_arm";
		
		public static const LEFT_UPPER_LEG:String = "left_upper_leg";
		public static const LEFT_LOWER_LEG:String = "left_lower_leg";
		
		public static const RIGHT_UPPER_LEG:String = "right_upper_leg";
		public static const RIGHT_LOWER_LEG:String = "right_lower_leg";
		
		/**
		 * Name of the bone
		 */ 
		public var name:String;
		
		/**
		 * Orientation of the bone
		 */ 
		public var orientation:Orientation;
		
		/**
		 * Name of the start joint
		 */ 
		public var startJointName:String;
		
		/**
		 * Name of the end joint
		 */ 
		public var endJointName:String;
		
		/**
		 * Name of the parent bone
		 */ 
		public var parentBoneName:String;
		
		public function SkeletonBone()
		{
		}
		
		/**
		 * Copies the properties from the other bone into this instance
		 */ 
		public function copyFrom(otherBone:SkeletonBone):void
		{
			name = otherBone.name;
			orientation ||= new Orientation();
			orientation.copyFrom(otherBone.orientation);
			startJointName = otherBone.startJointName;
			endJointName = otherBone.endJointName;
			parentBoneName = otherBone.parentBoneName;
		}
	}
}
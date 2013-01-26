package com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data
{
	import com.as3nui.nativeExtensions.air.kinect.data.SkeletonBone;
	
	public class OpenNISkeletonBone extends SkeletonBone
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
		
		public function OpenNISkeletonBone()
		{
			super();
		}
	}
}
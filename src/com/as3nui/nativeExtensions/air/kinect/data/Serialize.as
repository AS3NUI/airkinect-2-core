package com.as3nui.nativeExtensions.air.kinect.data
{
	import com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.*;
	import com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.*;
	
	import flash.geom.*;
	import flash.net.registerClassAlias;

	public class Serialize
	{
		
		private static var initialized:Boolean;
		
		public static function init():Boolean
		{
			if(!initialized)
			{
				initialized = true;
				registerClasses();
			}
			return true;
		}
		
		private static function registerClasses():Boolean
		{
			trace("[Serialize] registerClasses");
			registerClassAlias("flash.geom.Point", Point);
			registerClassAlias("flash.geom.Vector3D", Vector3D);
			registerClassAlias("flash.geom.Matrix3D", Matrix3D);
			registerClassAlias("String", String);
			
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.data.DeviceCapabilities", DeviceCapabilities);
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.data.Orientation", Orientation);
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.data.PointCloudRegion", PointCloudRegion);
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.data.Position", Position);
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.data.Quaternion", Quaternion);
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint", SkeletonJoint);
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.data.SkeletonBone", SkeletonBone);
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.data.User", User);
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.data.UserFrame", UserFrame);
			
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSSkeletonBone", MSSkeletonBone);
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSSkeletonJoint", MSSkeletonJoint);
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSUser", MSUser);
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSUserFrame", MSUserFrame);
			
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNISkeletonBone", OpenNISkeletonBone);
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNISkeletonJoint", OpenNISkeletonJoint);
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNIUser", OpenNIUser);
			registerClassAlias("com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data.OpenNIUserFrame", OpenNIUserFrame);
			
			return true;
		}
	}
}
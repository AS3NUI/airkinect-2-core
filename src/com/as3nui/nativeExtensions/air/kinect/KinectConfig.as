package com.as3nui.nativeExtensions.air.kinect
{
	public class KinectConfig
	{
		//--------------- depth configuration ---------------//
		public var depthEnabled:Boolean = false;
		public var depthWidth:uint = 320;
		public var depthHeight:uint = 240;
		public var depthMirrored:Boolean = false;
		public var depthShowUserColors:Boolean = false;
		
		//--------------- rgb configuration ---------------//
		public var rgbEnabled:Boolean = false;
		public var rgbWidth:uint = 320;
		public var rgbHeight:uint = 240;
		public var rgbMirrored:Boolean = false;
		
		//--------------- user configuration ---------------//
		public var userEnabled:Boolean = false;
		public var userMirrored:Boolean = false;
		public var skeletonEnabled:Boolean = false;
		public var skeletonMirrored:Boolean = false;
		
		//--------------- user mask configuration ---------------//
		public var userMaskEnabled:Boolean = false;
		public var userMaskWidth:uint = 320;
		public var userMaskHeight:uint = 240;
		public var userMaskMirrored:Boolean = false;
		
		//--------------- infrared configuration ---------------//
		//NOTE: you cannot use infrared with rgb or player mask
		public var infraredEnabled:Boolean = false;
		public var infraredWidth:uint = 320;
		public var infraredHeight:uint = 240;
		public var infraredMirrored:Boolean = false;
		
		//--------------- point cloud configuration ---------------//
		public var pointCloudEnabled:Boolean = false;
		public var pointCloudWidth:uint = 320;
		public var pointCloudHeight:uint = 240;
		public var pointCloudMirrored:Boolean = false;
		public var pointCloudDensity:uint = 1;
		public var pointCloudIncludeRGB:Boolean = false;
	}
}
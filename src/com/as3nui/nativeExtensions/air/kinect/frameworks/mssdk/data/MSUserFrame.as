package com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data
{
	import com.as3nui.nativeExtensions.air.kinect.data.User;
	import com.as3nui.nativeExtensions.air.kinect.data.UserFrame;
	
	public class MSUserFrame extends UserFrame
	{
		public function MSUserFrame(frameNumber:uint, timestamp:uint, users:Vector.<User>)
		{
			super(frameNumber, timestamp, users);
		}
	}
}
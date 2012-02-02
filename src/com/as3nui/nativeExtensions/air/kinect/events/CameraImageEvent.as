package com.as3nui.nativeExtensions.air.kinect.events
{
	import flash.display.BitmapData;
	import flash.events.Event;
	
	public class CameraImageEvent extends Event
	{
		
		public static const DEPTH_IMAGE_UPDATE:String = "depthImageUpdate";
		public static const RGB_IMAGE_UPDATE:String = "rgbImageUpdate";
		public static const INFRARED_IMAGE_UPDATE:String = "infraredImageUpdate";
		
		public var imageData:BitmapData;
		
		public function CameraImageEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false, imageData:BitmapData = null)
		{
			super(type, bubbles, cancelable);
			this.imageData = imageData;
		}
		
		override public function clone():Event
		{
			return new CameraImageEvent(type, bubbles, cancelable, imageData);
		}
	}
}
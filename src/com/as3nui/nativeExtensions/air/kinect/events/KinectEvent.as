package com.as3nui.nativeExtensions.air.kinect.events
{
	import flash.events.Event;
	
	public class KinectEvent extends Event
	{
		
		public static const STARTED:String = "started";
		public static const STOPPED:String = "stopped";
		
		public function KinectEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
		}
		
		override public function clone():Event
		{
			return new KinectEvent(type, bubbles, cancelable);
		}
	}
}
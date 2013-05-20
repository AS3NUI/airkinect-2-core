package com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.events
{
	import com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSHand;
	
	import flash.events.Event;
	
	public class MSHandEvent extends Event
	{
		
		public static const GRIP:String = "grip";
		public static const GRIP_RELEASE:String = "gripRelease";
		
		public var hand:MSHand;
		
		public function MSHandEvent(hand:MSHand, type:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
			this.hand = hand;
		}
		
		override public function clone():Event
		{
			return new MSHandEvent(hand, type, bubbles, cancelable);
		}
		
	}
}
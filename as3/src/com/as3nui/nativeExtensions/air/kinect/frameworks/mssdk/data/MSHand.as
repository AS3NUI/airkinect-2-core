package com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data
{
	import com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint;

	public class MSHand extends MSSkeletonJoint
	{
		
		public static const EVENT_NONE:uint = 0;
		public static const EVENT_GRIP:uint = 1;
		public static const EVENT_GRIP_RELEASE:uint = 2;
		
		public static const STATE_NOT_TRACKED:uint = 0x00;
		public static const STATE_TRACKED:uint = 0x01;           // Hand is tracked
		public static const STATE_ACTIVE:uint = 0x02;            // Hand is active within or near interaction region. In this state it is a candidate to become primary hand for user.
		public static const STATE_INTERACTIVE:uint = 0x04;       // Hand is in interaction region
		public static const STATE_PRESSED:uint = 0x08;           // Press happened in interaction region. Only possible in InteractionAdjuseHandPointers.
		public static const STATE_PRIMARY_FOR_USER:uint = 0x10;   // This hand is the primary hand for user.
		
		public var event:uint;
		public var state:uint;
		
		public function MSHand()
		{
		}
		
		override public function copyFrom(otherJoint:SkeletonJoint):void
		{
			super.copyFrom(otherJoint);
			
			var otherMSHand:MSHand = otherJoint as MSHand;
			
			this.event = otherMSHand.event;
			this.state = otherMSHand.state;
		}
		
	}
}
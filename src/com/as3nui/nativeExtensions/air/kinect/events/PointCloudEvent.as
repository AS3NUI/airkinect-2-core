package com.as3nui.nativeExtensions.air.kinect.events
{
	import com.as3nui.nativeExtensions.air.kinect.data.PointCloudRegion;
	
	import flash.events.Event;
	import flash.utils.ByteArray;
	
	public class PointCloudEvent extends Event
	{
		
		public static const POINT_CLOUD_UPDATE:String = "pointCloudUpdate";
		
		public var pointCloudData:ByteArray;
		public var pointCloudRegions:Vector.<PointCloudRegion>;
		
		public function PointCloudEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false, pointCloudData:ByteArray = null, pointCloudRegions:Vector.<PointCloudRegion> = null)
		{
			super(type, bubbles, cancelable);
			this.pointCloudData = pointCloudData;
			this.pointCloudRegions = pointCloudRegions;
		}
		
		override public function clone():Event
		{
			return new PointCloudEvent(type, bubbles, cancelable, pointCloudData, pointCloudRegions);
		}
	}
}
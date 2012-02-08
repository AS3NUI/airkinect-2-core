/*
* Copyright 2012 AS3NUI
*
*    Licensed under the Apache License, Version 2.0 (the "License");
*    you may not use this file except in compliance with the License.
*    You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*    Unless required by applicable law or agreed to in writing, software
*    distributed under the License is distributed on an "AS IS" BASIS,
*    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*    See the License for the specific language governing permissions and
*    limitations under the License.
*/
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
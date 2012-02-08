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
package com.as3nui.nativeExtensions.air.kinect.generators
{
	import com.as3nui.nativeExtensions.air.kinect.data.PointCloudRegion;
	import com.as3nui.nativeExtensions.air.kinect.events.PointCloudEvent;
	
	import flash.events.StatusEvent;
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	[Event(name="pointCloudUpdate", type="com.as3nui.nativeExtensions.air.kinect.events.PointCloudEvent")]
	public class PointCloudGenerator extends GeneratorBase
	{
		protected var _width:uint;
		
		public function get width():uint
		{
			return _width;
		}
		
		protected var _height:uint;
		
		public function get height():uint
		{
			return _height;
		}
		
		protected var _mirrored:Boolean;
		
		public function get mirrored():Boolean
		{
			return _mirrored;
		}
		
		protected var _density:uint;
		
		public function get density():uint
		{
			return _density;
		}
		
		protected var _includeRGB:Boolean;
		
		public function get includeRGB():Boolean
		{
			return _includeRGB;
		}
		
		private var pointBytes:ByteArray;
		private var pointCloudRegions:Vector.<PointCloudRegion>;
		
		public function PointCloudGenerator(nr:uint)
		{
			super(nr);
			_width = 320;
			_height = 240;
			_mirrored = false;
		}
		
		public function setPointCloudRegions(pointCloudRegions:Vector.<PointCloudRegion>):void
		{
			this.pointCloudRegions = pointCloudRegions;
			if(context != null)
			{
				context.call("setPointCloudRegions", nr, pointCloudRegions);
			}
		}
		
		override protected function applyConfig():void
		{
			enabled = config.pointCloudEnabled;
			_width = config.pointCloudResolution.x;
			_height = config.pointCloudResolution.y;
			_mirrored = config.pointCloudMirrored;
			_density = config.pointCloudDensity;
			_includeRGB = config.pointCloudIncludeRGB;
		}
		
		override protected function onStart():void
		{
			pointBytes = new ByteArray();
			context.call("setPointCloudEnabled", nr, enabled);
			context.call("setPointCloudMode", nr, _width, _height, _mirrored, _density, _includeRGB);
			context.call("setPointCloudRegions", nr, pointCloudRegions);
		}
		
		override protected function onStop():void
		{
			pointBytes = null;
		}
	
		override protected function statusHandler(event:StatusEvent):void
		{
			switch(event.code)
			{
				case "pointCloudFrame":
					context.call("getPointCloudFrame", nr, pointBytes, pointCloudRegions);
					pointBytes.position = 0;
					pointBytes.endian = Endian.LITTLE_ENDIAN;
					dispatchEvent(new PointCloudEvent(PointCloudEvent.POINT_CLOUD_UPDATE, false, false, pointBytes, pointCloudRegions));
					break;
			}
		}
		
	}
}
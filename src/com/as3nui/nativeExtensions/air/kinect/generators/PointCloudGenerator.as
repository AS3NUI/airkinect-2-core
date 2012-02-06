package com.as3nui.nativeExtensions.air.kinect.generators
{
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
		
		public function PointCloudGenerator(nr:uint)
		{
			super(nr);
			_width = 320;
			_height = 240;
			_mirrored = false;
		}
		
		override protected function applyConfig():void
		{
			enabled = config.pointCloudEnabled;
			_width = config.pointCloudWidth;
			_height = config.pointCloudHeight;
			_mirrored = config.pointCloudMirrored;
			_density = config.pointCloudDensity;
			_includeRGB = config.pointCloudIncludeRGB;
		}
		
		override protected function onStart():void
		{
			pointBytes = new ByteArray();
			context.call("setPointCloudEnabled", nr, enabled);
			context.call("setPointCloudMode", nr, _width, _height, _mirrored, _density, _includeRGB);
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
					context.call("getPointCloudFrame", nr, pointBytes);
					pointBytes.position = 0;
					pointBytes.endian = Endian.LITTLE_ENDIAN;
					dispatchEvent(new PointCloudEvent(PointCloudEvent.POINT_CLOUD_UPDATE, false, false, pointBytes));
					break;
			}
		}
		
	}
}
package com.as3nui.nativeExtensions.air.kinect.generators
{
	import com.as3nui.nativeExtensions.air.kinect.events.CameraImageEvent;
	
	import flash.events.StatusEvent;
	
	[Event(name="rgbImageUpdate", type="com.as3nui.nativeExtensions.air.kinect.events.CameraImageEvent")]
	public class RGBGenerator extends ImageGeneratorBase
	{
		
		public function RGBGenerator(nr:uint)
		{
			super(nr);
		}
		
		override protected function applyConfig():void
		{
			enabled = config.rgbEnabled;
			_width = config.rgbWidth;
			_height = config.rgbHeight;
			_mirrored = config.rgbMirrored;
		}
		
		override protected function onStart():void
		{
			super.onStart();
			context.call("setRGBEnabled", nr, enabled);
			context.call("setRGBMode", nr, _width, _height, _mirrored);
		}
		
		override protected function statusHandler(event:StatusEvent):void
		{
			switch(event.code)
			{
				case "RGBFrame": return updateImageData("getRGBFrame", CameraImageEvent.RGB_IMAGE_UPDATE);
			}
		}
	}
}
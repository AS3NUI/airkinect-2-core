package com.as3nui.nativeExtensions.air.kinect.generators
{
	import com.as3nui.nativeExtensions.air.kinect.events.CameraImageEvent;
	
	import flash.events.StatusEvent;

	[Event(name="infraredImageUpdate", type="com.as3nui.nativeExtensions.air.kinect.events.CameraImageEvent")]
	public class InfraredGenerator extends ImageGeneratorBase
	{
		
		public function InfraredGenerator(nr:uint)
		{
			super(nr);
		}
		
		override protected function applyConfig():void
		{
			enabled = config.infraredEnabled;
			_width = config.infraredWidth;
			_height = config.infraredHeight;
			_mirrored = config.infraredMirrored;
		}
		
		override protected function onStart():void
		{
			super.onStart();
			context.call("setInfraredEnabled", nr, enabled);
			context.call("setInfraredMode", nr, _width, _height, _mirrored);
		}
		
		override protected function statusHandler(event:StatusEvent):void
		{
			switch(event.code)
			{
				case "infraredFrame": return updateImageData("getInfraredFrame", CameraImageEvent.INFRARED_IMAGE_UPDATE);
			}
		}
	}
}
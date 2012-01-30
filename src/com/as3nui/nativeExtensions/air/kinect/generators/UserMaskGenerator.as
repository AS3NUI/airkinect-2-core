package com.as3nui.nativeExtensions.air.kinect.generators
{
	import com.as3nui.nativeExtensions.air.kinect.events.CameraImageEvent;
	
	import flash.events.StatusEvent;

	[Event(name="userMaskImageUpdate", type="com.as3nui.nativeExtensions.air.kinect.events.CameraImageEvent")]
	public class UserMaskGenerator extends ImageGeneratorBase
	{
		
		public function UserMaskGenerator(nr:uint)
		{
			super(nr);
		}
		
		override protected function applyConfig():void
		{
			enabled = config.userMaskEnabled;
			_width = config.userMaskWidth;
			_height = config.userMaskHeight;
			_mirrored = config.userMaskMirrored;
		}
		
		override protected function onStart():void
		{
			super.onStart();
			context.call("setUserMaskEnabled", nr, enabled);
			context.call("setUserMaskMode", nr, _width, _height, _mirrored);
		}
		
		override protected function statusHandler(event:StatusEvent):void
		{
			switch(event.code)
			{
				case "userMaskFrame": return updateImageData("getUserMaskFrame", CameraImageEvent.USER_MASK_IMAGE_UPDATE);
			}
		}
	}
}
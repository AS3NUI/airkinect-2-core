package com.as3nui.nativeExtensions.air.kinect.generators
{
	import com.as3nui.nativeExtensions.air.kinect.events.CameraImageEvent;
	
	import flash.events.StatusEvent;
	
	[Event(name="depthImageUpdate", type="com.as3nui.nativeExtensions.air.kinect.events.CameraImageEvent")]
	public class DepthGenerator extends ImageGeneratorBase
	{
		
		private var _showUserColors:Boolean;
		public function get showUserColors():Boolean { return _showUserColors; }
		
		public function set showUserColors(value:Boolean):void
		{
			if (_showUserColors == value)
				return;
			_showUserColors = value;
		}
		
		public function DepthGenerator(nr:uint)
		{
			super(nr);
		}
		
		override protected function applyConfig():void
		{
			enabled = config.depthEnabled;
			_width = config.depthWidth;
			_height = config.depthHeight;
			_mirrored = config.depthMirrored;
			_showUserColors = config.depthShowUserColors;
		}
		
		override protected function onStart():void
		{
			super.onStart();
			context.call("setDepthEnabled", nr, enabled);
			context.call("setDepthMode", nr, _width, _height, _mirrored);
			context.call("setDepthShowUserColors", nr, _showUserColors);
		}
		
		override protected function statusHandler(event:StatusEvent):void
		{
			switch(event.code)
			{
				case "depthFrame": return updateImageData("getDepthFrame", CameraImageEvent.DEPTH_IMAGE_UPDATE);
			}
		}
	}
}
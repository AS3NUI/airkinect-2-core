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
		
		override public function applyConfig():void
		{
			enabled = config.depthEnabled;
			_width = config.depthResolution.x;
			_height = config.depthResolution.y;
			_mirrored = config.depthMirrored;
			_showUserColors = config.depthShowUserColors;
		}
		
		override protected function onStart():void
		{
			super.onStart();
			sendConfigToContext();
		}
		
		override public function sendConfigToContext():void
		{
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
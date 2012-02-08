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
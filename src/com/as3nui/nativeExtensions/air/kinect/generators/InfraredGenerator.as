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
			_width = config.infraredResolution.x;
			_height = config.infraredResolution.y;
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
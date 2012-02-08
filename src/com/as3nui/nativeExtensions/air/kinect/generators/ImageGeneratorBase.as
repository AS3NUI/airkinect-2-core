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
	
	import flash.display.BitmapData;
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	public class ImageGeneratorBase extends GeneratorBase
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
		
		private var imageBytes:ByteArray;
		protected var imageData:BitmapData;
		
		public function ImageGeneratorBase(nr:uint)
		{
			super(nr);
			_width = 320;
			_height = 240;
			_mirrored = false;
		}
		
		override protected function onStart():void
		{
			imageBytes = new ByteArray();
			imageData = new BitmapData(_width, _height, true, 0x000000);
		}
		
		override protected function onStop():void
		{
			imageBytes = null;
			imageData.dispose();
		}
		
		protected function updateImageData(contextFunctionName:String, eventTypeToDispatch:String):void
		{
			//get the image data
			context.call(contextFunctionName, nr, imageBytes);
			imageBytes.position = 0;
			imageBytes.endian = Endian.LITTLE_ENDIAN;
			imageData.setPixels(imageData.rect, imageBytes);
			//dispatch the event
			dispatchEvent(new CameraImageEvent(eventTypeToDispatch, false, false, imageData));
		}
	}
}
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
package com.as3nui.nativeExtensions.air.kinect.frameworks.openni.events
{
	import flash.display.BitmapData;
	import flash.events.Event;

	public class OpenNICameraImageEvent extends Event
	{
		
		public static const INFRARED_IMAGE_UPDATE:String = "infraredImageUpdate";
		
		public var imageData:BitmapData;
		
		public function OpenNICameraImageEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false, imageData:BitmapData = null)
		{
			super(type, bubbles, cancelable);
			this.imageData = imageData;
		}
		
		override public function clone():Event
		{
			return new OpenNICameraImageEvent(type, bubbles, cancelable, imageData);
		}
	}
}
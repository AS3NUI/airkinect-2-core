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
	import com.as3nui.nativeExtensions.air.kinect.data.User;
	import com.as3nui.nativeExtensions.air.kinect.events.UserEvent;
	
	import flash.display.BitmapData;
	import flash.events.StatusEvent;
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	[Event(name="usersMaskImageUpdate", type="com.as3nui.nativeExtensions.air.kinect.events.UserEvent")]
	public class UserMaskGenerator extends GeneratorBase
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
		
		private var userGenerator:UserGenerator;
		private var userMaskByteArrays:Vector.<ByteArray>;
		
		public function UserMaskGenerator(userGenerator:UserGenerator, nr:uint)
		{
			super(nr);
			this.userGenerator = userGenerator;
		}
		
		override protected function applyConfig():void
		{
			enabled = config.userMaskEnabled;
			_width = config.userMaskResolution.x;
			_height = config.userMaskResolution.y;
			_mirrored = config.userMaskMirrored;
		}
		
		override protected function onStart():void
		{
			super.onStart();
			
			userMaskByteArrays = new Vector.<ByteArray>();
			for(var i:uint = 0; i < 15; i++)
			{
				userMaskByteArrays.push(new ByteArray());
			}
			
			context.call("setUserMaskEnabled", nr, enabled);
			context.call("setUserMaskMode", nr, _width, _height, _mirrored);
		}
		
		override protected function statusHandler(event:StatusEvent):void
		{
			switch(event.code)
			{
				case "userMaskFrame":
					//get the image for each user
					var i:uint = 0;
					for each(var user:User in userGenerator.users)
					{
						context.call("getUserMaskFrame", nr, user.trackingID, userMaskByteArrays[i]);
						userMaskByteArrays[i].position = 0;
						userMaskByteArrays[i].endian = Endian.LITTLE_ENDIAN;
						if(user.userMaskData == null)
						{
							user.userMaskData = new BitmapData(_width, _height, true, 0);
						}
						user.userMaskData.setPixels(user.userMaskData.rect, userMaskByteArrays[i]);
						i++;
					}
					if(userGenerator.users.length > 0)
					{
						dispatchEvent(new UserEvent(UserEvent.USERS_MASK_IMAGE_UPDATE, false, false, userGenerator.users));
					}
					break;
			}
		}
	}
}
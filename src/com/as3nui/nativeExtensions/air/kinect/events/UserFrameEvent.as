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
package com.as3nui.nativeExtensions.air.kinect.events
{
	import com.as3nui.nativeExtensions.air.kinect.data.UserFrame;

	import flash.events.Event;

	public class UserFrameEvent extends Event
	{
		public static const USER_FRAME_UPDATE:String = "userFrameUpdated";
		public var userFrame:UserFrame;
		public function UserFrameEvent(userFrame:UserFrame = null)
		{
			super(USER_FRAME_UPDATE);
			this.userFrame = userFrame;
		}
		
		override public function clone():Event
		{
			return new UserFrameEvent(userFrame);
		}
	}
}
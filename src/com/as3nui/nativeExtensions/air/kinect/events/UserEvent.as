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
	import com.as3nui.nativeExtensions.air.kinect.data.User;
	
	import flash.events.Event;
	
	public class UserEvent extends Event
	{
		
		public static const USERS_ADDED:String = "usersAdded";
		public static const USERS_REMOVED:String = "usersRemoved";
		
		public static const USERS_WITH_SKELETON_ADDED:String = "usersWithSkeletonAdded";
		public static const USERS_WITH_SKELETON_REMOVED:String = "usersWithSkeletonRemoved";
		
		public static const USERS_UPDATED:String = "usersUpdated";
		
		public static const USERS_MASK_IMAGE_UPDATE:String = "usersMaskImageUpdate";
		
		public var users:Vector.<User>;
		
		public function UserEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false, users:Vector.<User> = null)
		{
			super(type, bubbles, cancelable);
			this.users = users;
		}
		
		override public function clone():Event
		{
			return new UserEvent(type, bubbles, cancelable, users);
		}
	}
}
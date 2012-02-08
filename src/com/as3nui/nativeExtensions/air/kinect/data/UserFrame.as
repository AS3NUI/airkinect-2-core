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
package com.as3nui.nativeExtensions.air.kinect.data
{
	public class UserFrame
	{
		
		/**
		 * Current Frame Number from the Kinect
		 */
		private var _frameNumber:uint;
		
		public function get frameNumber():uint
		{
			return _frameNumber;
		}
		
		/**
		 * Timestamp of current Frame
		 */
		private var _timestamp:uint;
		
		public function get timestamp():uint
		{
			return _timestamp;
		}
		
		private var _users:Vector.<User>;
		
		/**
		 * Users on this frame
		 */
		public function get users():Vector.<User>
		{
			return _users;
		}
		
		private var _usersWithSkeleton:Vector.<User>;
		
		/**
		 * Users with full skeleton information on this frame
		 */ 
		public function get usersWithSkeleton():Vector.<User>
		{
			return _usersWithSkeleton;
		}
		
		public function UserFrame(frameNumber:uint, timestamp:uint, users:Vector.<User>)
		{
			_frameNumber = frameNumber;
			_timestamp = timestamp;
			_users = users;
			_usersWithSkeleton = new Vector.<User>();
			for each(var user:User in _users)
			{
				if(user.hasSkeleton)
				{
					_usersWithSkeleton.push(user);
				}
			}
		}
	}
}
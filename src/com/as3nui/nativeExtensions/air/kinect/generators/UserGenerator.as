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
	import com.as3nui.nativeExtensions.air.kinect.data.UserFrame;
	import com.as3nui.nativeExtensions.air.kinect.events.UserEvent;
	import com.as3nui.nativeExtensions.air.kinect.events.UserFrameEvent;
	import com.as3nui.nativeExtensions.air.kinect.namespaces.as3nui;
	
	import flash.events.StatusEvent;
	import flash.utils.Dictionary;
	
	[Event(name="usersAdded", type="com.as3nui.nativeExtensions.air.kinect.events.UserEvent")]
	[Event(name="usersRemoved", type="com.as3nui.nativeExtensions.air.kinect.events.UserEvent")]
	[Event(name="usersWithSkeletonAdded", type="com.as3nui.nativeExtensions.air.kinect.events.UserEvent")]
	[Event(name="usersWithSkeletonRemoved", type="com.as3nui.nativeExtensions.air.kinect.events.UserEvent")]
	[Event(name="usersUpdated", type="com.as3nui.nativeExtensions.air.kinect.events.UserEvent")]
	public class UserGenerator extends GeneratorBase
	{
		
		private var _mirrored:Boolean;
		
		public function get mirrored():Boolean
		{
			return _mirrored;
		}
		
		private var _skeletonEnabled:Boolean;
		
		public function set skeletonEnabled(value:Boolean):void
		{
			if (_skeletonEnabled == value)
				return;
			_skeletonEnabled = value;
		}
		
		public function get skeletonEnabled():Boolean
		{
			return _skeletonEnabled;
		}
		
		private var _skeletonMirrored:Boolean;
		
		public function get skeletonMirrored():Boolean
		{
			return _skeletonMirrored;
		}
		
		private var _users:Vector.<User>;
		public function get users():Vector.<User> { return _users; }
		
		private var usersByTrackingId:Dictionary;
		
		private var _usersWithSkeleton:Vector.<User>;
		public function get usersWithSkeleton():Vector.<User> { return _usersWithSkeleton; }
		
		private var usersWithSkeletonByTrackingId:Dictionary;
		
		private var skeletonJointNameIndices:Dictionary;
		private var skeletonJointNames:Vector.<String>;

		private var _simulationMode:Boolean;
		
		public function UserGenerator(nr:uint)
		{
			super(nr);
			_users = new Vector.<User>();
			usersByTrackingId = new Dictionary();
			_usersWithSkeleton = new Vector.<User>();
			usersWithSkeletonByTrackingId = new Dictionary();
		}
		
		override public function applyConfig():void
		{
			enabled = config.userEnabled;
			_mirrored = config.userMirrored;
			skeletonEnabled = config.skeletonEnabled;
			_skeletonMirrored = config.skeletonMirrored;
		}
		
		override protected function onStart():void
		{
			_users.length = 0;
			usersByTrackingId = new Dictionary();
			_usersWithSkeleton.length = 0;
			usersWithSkeletonByTrackingId = new Dictionary();
			sendConfigToContext();
			skeletonJointNameIndices = context.call("getSkeletonJointNameIndices", nr) as Dictionary;
			skeletonJointNames = context.call("getSkeletonJointNames", nr) as Vector.<String>;
		}
		
		override public function sendConfigToContext():void
		{
			context.call("setUserEnabled", nr, enabled);
			context.call("setUserMode", nr, _mirrored);
			context.call("setSkeletonEnabled", nr, _skeletonEnabled);
			context.call("setSkeletonMode", nr, _skeletonMirrored);
		}
		
		override protected function onStop():void
		{
		}
		
		override protected function statusHandler(event:StatusEvent):void
		{
			if(_simulationMode) return;
			switch(event.code)
			{
				case "userFrame":
					var userFrame:UserFrame = context.call("getUserFrame", nr) as UserFrame;
					handleUserFrame(userFrame);
			}
		}

		private function handleUserFrame(userFrame:UserFrame):void {
			//Allows for manual digesting of full user frame. This event should not be used
			this.dispatchEvent(new UserFrameEvent(userFrame));

			//update the users - copy the data inside existing users, so user references keep working
			var user:User;
			var otherUser:User;

			//which users should we remove?
			var removedUsers:Vector.<User> = getUsersToRemoveAndRemoveFromDictionary(userFrame.users, _users, usersByTrackingId);
			var removedUsersWithSkeleton:Vector.<User> = getUsersToRemoveAndRemoveFromDictionary(userFrame.usersWithSkeleton, _usersWithSkeleton, usersWithSkeletonByTrackingId);
			var addedUsers:Vector.<User> = new Vector.<User>();
			var addedUsersWithSkeleton:Vector.<User> = new Vector.<User>();
			var currentUsers:Vector.<User> = new Vector.<User>();
			var currentUsersWithSkeleton:Vector.<User> = new Vector.<User>();
			for each(otherUser in userFrame.users)
			{
				//existing? Update the properties
				user = usersByTrackingId[otherUser.trackingID];
				if(user != null)
				{
					user.copyFrom(otherUser);
				}
				else
				{
					usersByTrackingId[otherUser.trackingID] = user = otherUser;
					addedUsers.push(user);
				}

				user.as3nui::skeletonJointNameIndices = skeletonJointNameIndices;
				user.as3nui::_skeletonJointNames = skeletonJointNames;
				currentUsers.push(user);
				if(user.hasSkeleton)
				{
					currentUsersWithSkeleton.push(user);
					if(usersWithSkeletonByTrackingId[user.trackingID] == null)
					{
						usersWithSkeletonByTrackingId[otherUser.trackingID] = user;
						addedUsersWithSkeleton.push(user);
					}
				}
			}

			_users = currentUsers;
			_usersWithSkeleton = currentUsersWithSkeleton;

			//dispatch removed event
			if(removedUsers.length > 0)
			{
				dispatchEvent(new UserEvent(UserEvent.USERS_REMOVED, false, false, removedUsers));
			}
			if(removedUsersWithSkeleton.length > 0)
			{
				dispatchEvent(new UserEvent(UserEvent.USERS_WITH_SKELETON_REMOVED, false, false, removedUsersWithSkeleton));
			}
			//dispatch added event
			if(addedUsers.length > 0)
			{
				dispatchEvent(new UserEvent(UserEvent.USERS_ADDED, false, false, addedUsers));
			}
			if(addedUsersWithSkeleton.length > 0)
			{
				dispatchEvent(new UserEvent(UserEvent.USERS_WITH_SKELETON_ADDED, false, false, addedUsersWithSkeleton));
			}
			//dispatch updated event
			dispatchEvent(new UserEvent(UserEvent.USERS_UPDATED, false, false, _users));
		}
		
		private function getUsersToRemoveAndRemoveFromDictionary(newUsers:Vector.<User>, previousUsers:Vector.<User>, dictionary:Dictionary):Vector.<User>
		{
			var removedUsers:Vector.<User> = new Vector.<User>();
			for each(var user:User in previousUsers)
			{
				var remove:Boolean = true;
				for each(var otherUser:User in newUsers)
				{
					if(otherUser.trackingID == user.trackingID)
					{
						remove = false;
						break;
					}
				}
				if(remove)
				{
					removedUsers.push(user);
					delete dictionary[user.trackingID];
				}
			}
			return removedUsers;
		}

		public function set skeletonSimulationMode(value:Boolean):void {
			_simulationMode = value;
		}

		public function get skeletonSimulationMode():Boolean {
			return _simulationMode;
		}

		public function simulateUserFrame(userFrame:UserFrame):void {
			handleUserFrame(userFrame);
		}
	}
}
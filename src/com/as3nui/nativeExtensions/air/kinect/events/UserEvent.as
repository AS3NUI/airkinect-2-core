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
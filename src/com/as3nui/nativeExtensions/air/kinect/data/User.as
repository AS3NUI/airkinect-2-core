package com.as3nui.nativeExtensions.air.kinect.data
{
	import flash.geom.Point;
	import flash.geom.Vector3D;
	import flash.utils.Dictionary;

	public class User
	{
		
		/**
		 * Current user Tracking ID
		 */
		private var _trackingID:uint;
		
		public function get trackingID():uint
		{
			return _trackingID;
		}
		
		/**
		 * Position of the user in world coordinates
		 */ 
		public var position:Vector3D;
		
		/**
		 * Relative position of the user
		 */ 
		public var positionRelative:Vector3D;
		
		/**
		 * Absolute 2D position of the user, mapped in RGB space
		 */ 
		public var rgbPosition:Point;
		
		/**
		 * Relative 2D position of the user, mapped in RGB space
		 */ 
		public var rgbRelativePosition:Point;
		
		/**
		 * Absolute 2D position of the user, mapped in depth space
		 */ 
		public var depthPosition:Point;
		
		/**
		 * Relative 2D position of the user, mapped in depth space
		 */ 
		public var depthRelativePosition:Point;
		
		/**
		 * Does this user have a calibrated skeleton or not?
		 */
		private var _hasSkeleton:Boolean;
		
		public function get hasSkeleton():Boolean
		{
			return _hasSkeleton;
		}
		
		/**
		 * Collection of Skeleton joints
		 */
		private var _skeletonJoints:Vector.<SkeletonJoint>;
		
		public function get skeletonJoints():Vector.<SkeletonJoint>
		{
			return _skeletonJoints;
		}
		
		/**
		 * Type of user
		 */ 
		private var _type:String;
		
		public function get type():String
		{
			return _type;
		}
		
		public var skeletonJointNameIndices:Dictionary;
		
		public function User(type:String, trackingID:uint, position:Vector3D, positionRelative:Vector3D, rgbPosition:Point, rgbRelativePosition:Point, depthPosition:Point, depthRelativePosition:Point, hasSkeleton:Boolean, skeletonJoints:Vector.<SkeletonJoint>)
		{
			_type = type;
			_trackingID = trackingID;
			this.position = position;
			this.positionRelative = positionRelative;
			this.rgbPosition = rgbPosition;
			this.rgbRelativePosition = rgbRelativePosition;
			this.depthPosition = depthPosition;
			this.depthRelativePosition = depthRelativePosition;
			_hasSkeleton = hasSkeleton;
			_skeletonJoints = skeletonJoints;
		}
		
		public function getJointByName(jointName:String):SkeletonJoint
		{
			return _skeletonJoints[skeletonJointNameIndices[jointName]];
		}
		
		/**
		 * Copies the properties from the other user into this instance
		 */ 
		public function copyFrom(otherUser:User):void
		{
			_type = otherUser.type;
			_trackingID = otherUser.trackingID;
			position.copyFrom(otherUser.position);
			positionRelative.copyFrom(otherUser.positionRelative);
			rgbPosition.copyFrom(otherUser.rgbPosition);
			rgbRelativePosition.copyFrom(otherUser.rgbRelativePosition);
			depthPosition.copyFrom(otherUser.depthPosition);
			depthRelativePosition.copyFrom(otherUser.depthRelativePosition);
			_hasSkeleton = otherUser.hasSkeleton;
			for(var i:uint = 0; i < otherUser.skeletonJoints.length; i++)
			{
				_skeletonJoints[i].copyFrom(otherUser.skeletonJoints[i]);
			}
		}
	}
}
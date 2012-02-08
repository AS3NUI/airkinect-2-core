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
	import com.as3nui.nativeExtensions.air.kinect.constants.JointNames;
	import com.as3nui.nativeExtensions.air.kinect.namespaces.as3nui;
	
	import flash.display.BitmapData;
	import flash.geom.Point;
	import flash.geom.Vector3D;
	import flash.utils.Dictionary;

	/**
	 * This class represents a user, tracked by the kinect.
	 * 
	 * <p>A user doesn't necessarely have a tracked skeleton, but will always
	 * have a position. You can use the <code>hasSkeleton</code> property, to
	 * check if a user has a tracked skeleton.</p>
	 * 
	 * <p>You can access the skeleton information through different properties
	 * and methods:</p>
	 * 
	 * <p>
	 * <ul>
	 * <li><code>skeletonJoints</code>: collection of all skeleton joints</li>
	 * <li><code>getJointByName(jointName:String)</code>: get a skeleton joint by a given name</li>
	 * <li><code>head, neck, torso, leftShoulder, ...</code>: getters for default joints</li>
	 * </ul>
	 * </p>
	 * 
	 * <p>When you enable user masking in the kinect config, the <code>userMask</code>
	 * property will contain the rgb pixels for this user.</p>
	 */ 
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
		
		public function get head():SkeletonJoint { return getJointByName(JointNames.HEAD); }
		public function get neck():SkeletonJoint { return getJointByName(JointNames.NECK); }
		public function get torso():SkeletonJoint { return getJointByName(JointNames.TORSO); }
		
		public function get leftShoulder():SkeletonJoint { return getJointByName(JointNames.LEFT_SHOULDER); }
		public function get leftElbow():SkeletonJoint { return getJointByName(JointNames.LEFT_ELBOW); }
		public function get leftHand():SkeletonJoint { return getJointByName(JointNames.LEFT_HAND); }
		
		public function get rightShoulder():SkeletonJoint { return getJointByName(JointNames.RIGHT_SHOULDER); }
		public function get rightElbow():SkeletonJoint { return getJointByName(JointNames.RIGHT_ELBOW); }
		public function get rightHand():SkeletonJoint { return getJointByName(JointNames.RIGHT_HAND); }
		
		public function get leftHip():SkeletonJoint { return getJointByName(JointNames.LEFT_HIP); }
		public function get leftKnee():SkeletonJoint { return getJointByName(JointNames.LEFT_KNEE); }
		public function get leftFoot():SkeletonJoint { return getJointByName(JointNames.LEFT_FOOT); }
		
		public function get rightHip():SkeletonJoint { return getJointByName(JointNames.RIGHT_HIP); }
		public function get rightKnee():SkeletonJoint { return getJointByName(JointNames.RIGHT_KNEE); }
		public function get rightFoot():SkeletonJoint { return getJointByName(JointNames.RIGHT_FOOT); }
		
		/**
		 * Type of skeleton
		 * @see com.as3nui.nativeExtensions.air.kinect.constants.UserType for possible options
		 */ 
		private var _type:String;
		
		public function get type():String
		{
			return _type;
		}
		
		/**
		 * User Mask Bitmapdata
		 */ 
		public var userMaskData:BitmapData;
		
		/**
		 * Dictionary of skeleton joint names, mapping to index in the skeleton joints vector
		 * This is set by the usergenerator, and is used to get a joint by it's name
		 * 
		 * @private
		 */ 
		as3nui var skeletonJointNameIndices:Dictionary;
		
		/**
		 * Vector of skeleton joint names.
		 * This is set by the usergenerator
		 * 
		 * @private
		 */ 
		as3nui var _skeletonJointNames:Vector.<String>;
		
		/**
		 * Get a list of all skeleton joint names for this user
		 */ 
		public function get skeletonJointNames():Vector.<String>
		{
			return as3nui::_skeletonJointNames.concat();
		}
		
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
		
		/**
		 * Get a joint by it's name
		 * @see com.as3nui.nativeExtensions.air.kinect.constants.JointNames for the basic options
		 */ 
		public function getJointByName(jointName:String):SkeletonJoint
		{
			return _skeletonJoints[as3nui::skeletonJointNameIndices[jointName]];
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
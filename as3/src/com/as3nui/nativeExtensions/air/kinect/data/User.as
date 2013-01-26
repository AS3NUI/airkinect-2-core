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
	import flash.display.BitmapData;
	import flash.utils.Dictionary;

	/**
	 * This class represents a user, tracked by the kinect.
	 * 
	 * <p>A user doesn't necessarily have a tracked skeleton, but will always
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
		 * Current user User ID
		 */
		public var userID:uint;

		/**
		 * Current user Tracking ID
		 */
		public var trackingID:uint;
		
		/**
		 * Position of the user
		 */ 
		public var position:Position;
		
		/**
		 * Does this user have a calibrated skeleton or not?
		 */
		public var hasSkeleton:Boolean;
		
		/**
		 * Collection of Skeleton joints
		 */
		public var skeletonJoints:Vector.<SkeletonJoint>;
		
		/**
		 * Collection of Skeleton bones
		 */ 
		public var skeletonBones:Vector.<SkeletonBone>;
		
		/**
		 * Framework used to generate this skeleton
		 * @see com.as3nui.nativeExtensions.air.kinect.constants.Framework for possible options
		 */ 
		public var framework:String;
		
		/**
		 * User Mask Bitmapdata
		 */ 
		public var userMaskData:BitmapData;
		
		/**
		 * Dictionary of skeleton joint names, mapping to index in the skeleton joints vector
		 * 
		 * @private
		 */ 
		public var skeletonJointNameIndices:Dictionary;
		
		/**
		 * List of all skeleton joint names for this user
		 */ 
		public var skeletonJointNames:Vector.<String>;
		
		/**
		 * Dictionary of skeleton bone names, mapping to index in the skeleton bones vector
		 * 
		 * @private
		 */ 
		public var skeletonBoneNameIndices:Dictionary;
		
		/**
		 * List of all skeleton bone names for this user
		 */ 
		public var skeletonBoneNames:Vector.<String>;
		
		public function User()
		{
		}
		
		public function get head():SkeletonJoint { return getJointByName(SkeletonJoint.HEAD); }
		public function get neck():SkeletonJoint { return getJointByName(SkeletonJoint.NECK); }
		public function get torso():SkeletonJoint { return getJointByName(SkeletonJoint.TORSO); }
		
		public function get leftShoulder():SkeletonJoint { return getJointByName(SkeletonJoint.LEFT_SHOULDER); }
		public function get leftElbow():SkeletonJoint { return getJointByName(SkeletonJoint.LEFT_ELBOW); }
		public function get leftHand():SkeletonJoint { return getJointByName(SkeletonJoint.LEFT_HAND); }
		
		public function get rightShoulder():SkeletonJoint { return getJointByName(SkeletonJoint.RIGHT_SHOULDER); }
		public function get rightElbow():SkeletonJoint { return getJointByName(SkeletonJoint.RIGHT_ELBOW); }
		public function get rightHand():SkeletonJoint { return getJointByName(SkeletonJoint.RIGHT_HAND); }
		
		public function get leftHip():SkeletonJoint { return getJointByName(SkeletonJoint.LEFT_HIP); }
		public function get leftKnee():SkeletonJoint { return getJointByName(SkeletonJoint.LEFT_KNEE); }
		public function get leftFoot():SkeletonJoint { return getJointByName(SkeletonJoint.LEFT_FOOT); }
		
		public function get rightHip():SkeletonJoint { return getJointByName(SkeletonJoint.RIGHT_HIP); }
		public function get rightKnee():SkeletonJoint { return getJointByName(SkeletonJoint.RIGHT_KNEE); }
		public function get rightFoot():SkeletonJoint { return getJointByName(SkeletonJoint.RIGHT_FOOT); }
		
		/**
		 * Get a joint by it's name
		 */ 
		public function getJointByName(jointName:String):SkeletonJoint
		{
			if(skeletonJointNameIndices[jointName] == undefined) return null;
			return skeletonJoints[skeletonJointNameIndices[jointName]];
		}
		
		/**
		 * Get a bone by it's name
		 */ 
		public function getBoneByName(boneName:String):SkeletonBone
		{
			if(skeletonBoneNameIndices[boneName] == undefined) return null;
			return skeletonBones[skeletonBoneNameIndices[boneName]];
		}
		
		/**
		 * Copies the properties from the other user into this instance
		 */ 
		public function copyFrom(otherUser:User):void
		{
			framework = otherUser.framework;
			userID = otherUser.userID;
			position ||= new Position();
			position.copyFrom(otherUser.position);
			hasSkeleton = otherUser.hasSkeleton;
			updateJointCount(otherUser.skeletonJoints.length);
			var i:uint = 0
			for(i = 0; i < otherUser.skeletonJoints.length; i++)
			{
				skeletonJoints[i].copyFrom(otherUser.skeletonJoints[i]);
			}
			updateBoneCount(otherUser.skeletonBones.length);
			for(i = 0; i < otherUser.skeletonBones.length; i++)
			{
				skeletonBones[i].copyFrom(otherUser.skeletonBones[i]);
			}
		}
		
		/**
		 * Resizes the skeletonJoints vector
		 * This may be needed when we switch between seated - normal tracking
		 */ 
		private function updateJointCount(count:uint):void
		{
			skeletonJoints.length = count;
			for(var i:uint = 0; i < count; i++)
			{
				skeletonJoints[i] ||= new SkeletonJoint();
			}
		}
		
		/**
		 * Resizes the skeletonBones vector
		 * This may be needed when we switch between seated - normal tracking
		 */ 
		private function updateBoneCount(count:uint):void
		{
			skeletonBones.length = count;
			for(var i:uint = 0; i < count; i++)
			{
				skeletonBones[i] ||= new SkeletonBone();
			}
		}
	}
}
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
	import flash.geom.Matrix3D;
	import flash.geom.Point;
	import flash.geom.Vector3D;

	/**
	 * This class represents one joint in a user skeleton.
	 * 
	 * <p>It has position and orientation info. Next to global
	 * positions (x, y, z), you can get position info in 2D in
	 * RGB or depth space aswell.</p>
	 */ 
	[RemoteClass(alias='com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint')]
	public class SkeletonJoint extends Object
	{
		public static const HEAD:String = "head";
		public static const NECK:String = "neck";
		public static const TORSO:String = "torso";
		
		public static const LEFT_SHOULDER:String = "left_shoulder";
		public static const LEFT_ELBOW:String = "left_elbow";
		public static const LEFT_HAND:String = "left_hand";
		
		public static const RIGHT_SHOULDER:String = "right_shoulder";
		public static const RIGHT_ELBOW:String = "right_elbow";
		public static const RIGHT_HAND:String = "right_hand";
		
		public static const LEFT_HIP:String = "left_hip";
		public static const LEFT_KNEE:String = "left_knee";
		public static const LEFT_FOOT:String = "left_foot";
		
		public static const RIGHT_HIP:String = "right_hip";
		public static const RIGHT_KNEE:String = "right_knee";
		public static const RIGHT_FOOT:String = "right_foot";
		
		/**
		 * Name of the joint
		 */ 
		public var name:String;
		
		/**
		 * Position of the joint in world coordinates
		 */ 
		public var position:Vector3D;
		
		/**
		 * Relative position of the joint
		 */ 
		public var positionRelative:Vector3D;
		
		/**
		 * Absolute 2D position of the joint, mapped in RGB space
		 */ 
		public var rgbPosition:Point;
		
		/**
		 * Relative 2D position of the joint, mapped in RGB space
		 */ 
		public var rgbRelativePosition:Point;
		
		/**
		 * Absolute 2D position of the joint, mapped in depth space
		 */ 
		public var depthPosition:Point;
		
		/**
		 * Relative 2D position of the joint, mapped in depth space
		 */ 
		public var depthRelativePosition:Point;

		/**
		 * Accuracy of the joint position
		 */
		public var positionConfidence:Number;
		
		public var absoluteOrientationMatrix:Matrix3D;
		public var absoluteOrientationQuaternion:Vector3D;
		public var hierarchicalOrientationMatrix:Matrix3D;
		public var hierarchicalOrientationQuaternion:Vector3D;

		/**
		 * Accuracy of the joint orientation
		 */
		public var orientationConfidence:Number;


		public function SkeletonJoint()
		{
		}
		
		/**
		 * Copies the properties from the other joint into this instance
		 */ 
		public function copyFrom(otherJoint:SkeletonJoint):void
		{
			name = otherJoint.name;
			position ||= new Vector3D();
			position.copyFrom(otherJoint.position);
			positionRelative ||= new Vector3D();
			positionRelative.copyFrom(otherJoint.positionRelative);
			rgbPosition ||= new Point();
			rgbPosition.copyFrom(otherJoint.rgbPosition);
			rgbRelativePosition ||= new Point();
			rgbRelativePosition.copyFrom(otherJoint.rgbRelativePosition);
			depthPosition ||= new Point();
			depthPosition.copyFrom(otherJoint.depthPosition);
			depthRelativePosition ||= new Point();
			depthRelativePosition.copyFrom(otherJoint.depthRelativePosition);
			positionConfidence = otherJoint.positionConfidence;
			absoluteOrientationMatrix ||= new Matrix3D();
			absoluteOrientationMatrix.copyFrom(otherJoint.absoluteOrientationMatrix);
			absoluteOrientationQuaternion ||= new Vector3D();
			absoluteOrientationQuaternion.copyFrom(otherJoint.absoluteOrientationQuaternion);
			hierarchicalOrientationMatrix ||= new Matrix3D();
			hierarchicalOrientationMatrix.copyFrom(otherJoint.hierarchicalOrientationMatrix);
			hierarchicalOrientationQuaternion ||= new Vector3D();
			hierarchicalOrientationQuaternion.copyFrom(otherJoint.hierarchicalOrientationQuaternion);
			orientationConfidence = otherJoint.orientationConfidence;
		}
	}
}
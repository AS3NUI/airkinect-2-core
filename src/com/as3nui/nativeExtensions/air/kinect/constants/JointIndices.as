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
package com.as3nui.nativeExtensions.air.kinect.constants
{
	/**
	 * Indices of joints, which are available on both platform.
	 * 
	 * <p>It is recommended you refer to all joints by constants in <code>constants.JointNames</code>. This class is a utility to provide array access and easy lookup</p>
	 */ 
	public class JointIndices
	{
		public static const HEAD:uint = 0;
		public static const NECK:uint = 1;
		public static const TORSO:uint = 2;
		
		public static const LEFT_SHOULDER:uint = 3;
		public static const LEFT_ELBOW:uint = 4;
		public static const LEFT_HAND:uint = 5;
		
		public static const RIGHT_SHOULDER:uint = 6;
		public static const RIGHT_ELBOW:uint = 7;
		public static const RIGHT_HAND:uint = 8;
		
		public static const LEFT_HIP:uint = 9;
		public static const LEFT_KNEE:uint = 10;
		public static const LEFT_FOOT:uint = 11;
		
		public static const RIGHT_HIP:uint = 12;
		public static const RIGHT_KNEE:uint = 13;
		public static const RIGHT_FOOT:uint = 14;

		public static const JOINT_LOOKUP:Vector.<String> = new <String>[
			JointNames.HEAD,  JointNames.NECK, JointNames.TORSO,
			JointNames.LEFT_SHOULDER, JointNames.LEFT_ELBOW, JointNames.LEFT_HAND,
			JointNames.RIGHT_SHOULDER, JointNames.RIGHT_ELBOW, JointNames.RIGHT_HAND,
			JointNames.LEFT_HIP, JointNames.LEFT_KNEE, JointNames.LEFT_FOOT,
			JointNames.RIGHT_HIP, JointNames.RIGHT_KNEE, JointNames.RIGHT_FOOT];
	}
}
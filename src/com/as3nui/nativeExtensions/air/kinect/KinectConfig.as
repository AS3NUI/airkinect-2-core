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
package com.as3nui.nativeExtensions.air.kinect
{
	import com.as3nui.nativeExtensions.air.kinect.constants.CameraResolution;
	
	import flash.geom.Point;

	public class KinectConfig
	{
		//--------------- depth configuration ---------------//
		public var depthEnabled:Boolean = false;
		public var depthResolution:Point = CameraResolution.RESOLUTION_640_480;
		public var depthMirrored:Boolean = true;
		public var depthShowUserColors:Boolean = false;
		
		//--------------- rgb configuration ---------------//
		public var rgbEnabled:Boolean = false;
		public var rgbResolution:Point = CameraResolution.RESOLUTION_640_480;
		public var rgbMirrored:Boolean = true;
		
		//--------------- user configuration ---------------//
		public var userEnabled:Boolean = false;
		public var userMirrored:Boolean = true;
		public var skeletonEnabled:Boolean = false;
		public var skeletonMirrored:Boolean = true;
		
		//--------------- user mask configuration ---------------//
		public var userMaskEnabled:Boolean = false;
		public var userMaskResolution:Point = CameraResolution.RESOLUTION_640_480;
		public var userMaskMirrored:Boolean = true;
		
		//--------------- infrared configuration ---------------//
		//NOTE: you cannot use infrared with rgb or player mask
		public var infraredEnabled:Boolean = false;
		public var infraredResolution:Point = CameraResolution.RESOLUTION_320_240;
		public var infraredMirrored:Boolean = true;
		
		//--------------- point cloud configuration ---------------//
		public var pointCloudEnabled:Boolean = false;
		public var pointCloudResolution:Point = CameraResolution.RESOLUTION_640_480;
		public var pointCloudMirrored:Boolean = true;
		public var pointCloudDensity:uint = 1;
		public var pointCloudIncludeRGB:Boolean = false;
	}
}
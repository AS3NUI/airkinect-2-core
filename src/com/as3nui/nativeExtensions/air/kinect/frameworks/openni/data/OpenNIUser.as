package com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data
{
	import com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint;
	import com.as3nui.nativeExtensions.air.kinect.data.User;
	
	import flash.geom.Point;
	import flash.geom.Vector3D;
	
	public class OpenNIUser extends User
	{
		
		public function OpenNIUser(framework:String, userID:uint, trackingID:uint, position:Vector3D, positionRelative:Vector3D, rgbPosition:Point, rgbRelativePosition:Point, depthPosition:Point, depthRelativePosition:Point, hasSkeleton:Boolean, skeletonJoints:Vector.<SkeletonJoint>)
		{
			super(framework, userID, trackingID, position, positionRelative, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition, hasSkeleton, skeletonJoints);
		}
	}
}
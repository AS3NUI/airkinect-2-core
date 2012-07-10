package com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data
{
	import com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint;
	import com.as3nui.nativeExtensions.air.kinect.data.User;
	
	import flash.geom.Point;
	import flash.geom.Vector3D;
	
	public class MSUser extends User
	{
		public function MSUser()
		{
			super();
			//super(framework, userID, trackingID, position, positionRelative, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition, hasSkeleton, skeletonJoints);
		}
	}
}
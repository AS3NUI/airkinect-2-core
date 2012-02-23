package com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data
{
	import com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint;
	
	import flash.geom.Point;
	import flash.geom.Vector3D;
	
	public class MSSkeletonJoint extends SkeletonJoint
	{
		public function MSSkeletonJoint(name:String, position:Vector3D, positionRelative:Vector3D, rgbPosition:Point, rgbRelativePosition:Point, depthPosition:Point, depthRelativePosition:Point)
		{
			super(name, position, positionRelative, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition);
		}
		
		/**
		 * Creates a copy of the joint
		 */ 
		override public function cloneJoint():SkeletonJoint
		{
			return new MSSkeletonJoint(name, position.clone(), positionRelative.clone(), rgbPosition.clone(), rgbRelativePosition.clone(), depthPosition.clone(), depthRelativePosition.clone());
		}
	}
}
package com.as3nui.nativeExtensions.air.kinect.frameworks.openni.data
{
	import com.as3nui.nativeExtensions.air.kinect.data.SkeletonJoint;
	
	import flash.geom.Point;
	import flash.geom.Vector3D;
	
	public class OpenNISkeletonJoint extends SkeletonJoint
	{
		
		/**
		 * Accuracy of the joint position
		 */ 
		public var positionConfidence:Number;
		
		/**
		 * Orientation of the joint as euler coordinates (radians), mapped in world space
		 */ 
		public var orientation:Vector3D;
		
		/**
		 * Accuracy of the joint orientation
		 */ 
		public var orientationConfidence:Number;
		
		public function OpenNISkeletonJoint(name:String, position:Vector3D, positionRelative:Vector3D, positionConfidence:Number, orientation:Vector3D, orientationConfidence:Number, rgbPosition:Point, rgbRelativePosition:Point, depthPosition:Point, depthRelativePosition:Point)
		{
			super(name, position, positionRelative, rgbPosition, rgbRelativePosition, depthPosition, depthRelativePosition);
			
			this.positionConfidence = positionConfidence;
			this.orientation = orientation;
			this.orientationConfidence = orientationConfidence;
		}
		
		/**
		 * Creates a copy of the joint
		 */ 
		override public function cloneJoint():SkeletonJoint
		{
			return new OpenNISkeletonJoint(name, position.clone(), positionRelative.clone(), positionConfidence, orientation.clone(), orientationConfidence, rgbPosition.clone(), rgbRelativePosition.clone(), depthPosition.clone(), depthRelativePosition.clone());
		}
		
		/**
		 * Copies the properties from the other joint into this instance
		 */ 
		override public function copyFrom(otherJoint:SkeletonJoint):void
		{
			super.copyFrom(otherJoint);
			var otherOpenNISkeletonJoint:OpenNISkeletonJoint = otherJoint as OpenNISkeletonJoint;
			positionConfidence = otherOpenNISkeletonJoint.positionConfidence;
			orientation.copyFrom(otherOpenNISkeletonJoint.orientation);
			orientationConfidence = otherOpenNISkeletonJoint.orientationConfidence;
		}
	}
}
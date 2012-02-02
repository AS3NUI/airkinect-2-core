package com.as3nui.nativeExtensions.air.kinect.data
{
	import flash.geom.Point;
	import flash.geom.Vector3D;

	public class SkeletonJoint extends Object
	{
		
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
		 * Accuracy of the joint position
		 */ 
		public var positionConfidence:Number;
		
		/**
		 * Orientation of the joint as euler coordinates, mapped in world space
		 */ 
		public var orientation:Vector3D;
		
		/**
		 * Accuracy of the joint orientation
		 */ 
		public var orientationConfidence:Number;
		
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
		
		public function SkeletonJoint(name:String, position:Vector3D, positionRelative:Vector3D, positionConfidence:Number, orientation:Vector3D, orientationConfidence:Number, rgbPosition:Point, rgbRelativePosition:Point, depthPosition:Point, depthRelativePosition:Point)
		{
			this.name = name;
			this.position = position;
			this.positionRelative = positionRelative;
			this.positionConfidence = positionConfidence;
			this.orientation = orientation;
			this.orientationConfidence = orientationConfidence;
			this.rgbPosition = rgbPosition;
			this.rgbRelativePosition = rgbRelativePosition;
			this.depthPosition = depthPosition;
			this.depthRelativePosition = depthRelativePosition;
		}
		
		/**
		 * Creates a copy of the joint
		 */ 
		public function cloneJoint():SkeletonJoint
		{
			return new SkeletonJoint(name, position.clone(), positionRelative.clone(), positionConfidence, orientation.clone(), orientationConfidence, rgbPosition.clone(), rgbRelativePosition.clone(), depthPosition.clone(), depthRelativePosition.clone());
		}
		
		/**
		 * Copies the properties from the other joint into this instance
		 */ 
		public function copyFrom(otherJoint:SkeletonJoint):void
		{
			name = otherJoint.name;
			position.copyFrom(otherJoint.position);
			positionRelative.copyFrom(otherJoint.positionRelative);
			positionConfidence = otherJoint.positionConfidence;
			orientation.copyFrom(otherJoint.orientation);
			orientationConfidence = otherJoint.orientationConfidence;
			rgbPosition.copyFrom(otherJoint.rgbPosition);
			rgbRelativePosition.copyFrom(otherJoint.rgbRelativePosition);
			depthPosition.copyFrom(otherJoint.depthPosition);
			depthRelativePosition.copyFrom(otherJoint.depthRelativePosition);
		}
	}
}
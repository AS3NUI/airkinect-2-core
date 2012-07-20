package com.as3nui.nativeExtensions.air.kinect.data
{
	import flash.geom.Point;
	import flash.geom.Vector3D;

	public class Position
	{
		/**
		 * 3D position in world coordinate space
		 */ 
		public var world:Vector3D;
		
		/**
		 * 3D position in world relative coordinate space
		 */ 
		public var worldRelative:Vector3D;
		
		/**
		 * 2D position in rgb coordinate space
		 */ 
		public var rgb:Point;
		
		/**
		 * 2D position in rgb relative coordinate space
		 */ 
		public var rgbRelative:Point;
		
		/**
		 * 2D position in depth coordinate space
		 */ 
		public var depth:Point;
		
		/**
		 * 2D position in depth relative coordinate space
		 */ 
		public var depthRelative:Point;
		
		public function Position()
		{
		}
		
		public function copyFrom(otherPosition:Position):void
		{
			world ||= new Vector3D();
			world.copyFrom(otherPosition.world);
			
			worldRelative ||= new Vector3D();
			worldRelative.copyFrom(otherPosition.worldRelative);
			
			rgb ||= new Point();
			rgb.copyFrom(otherPosition.rgb);
			
			rgbRelative ||= new Point();
			rgbRelative.copyFrom(otherPosition.rgbRelative);
			
			depth ||= new Point();
			depth.copyFrom(otherPosition.depth);
			
			depthRelative ||= new Point();
			depthRelative.copyFrom(otherPosition.depthRelative);
		}
		
		public function clone():Position
		{
			var cloned:Position = new Position();
			cloned.copyFrom(this);
			return cloned;
		}
	}
}
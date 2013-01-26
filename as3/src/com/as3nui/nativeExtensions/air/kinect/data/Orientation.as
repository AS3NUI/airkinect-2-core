package com.as3nui.nativeExtensions.air.kinect.data
{
	import flash.geom.Matrix3D;

	public class Orientation
	{
		
		/**
		 * 3D Absolute Rotation Matrix
		 */ 
		public var absoluteOrientationMatrix:Matrix3D;
		
		public function Orientation()
		{
		}
		
		public function copyFrom(otherOrientation:Orientation):void
		{
			absoluteOrientationMatrix ||= new Matrix3D();
			absoluteOrientationMatrix.copyFrom(otherOrientation.absoluteOrientationMatrix);
		}
		
		public function clone():Orientation
		{
			var cloned:Orientation = new Orientation();
			cloned.copyFrom(this);
			return cloned;
		}
	}
}
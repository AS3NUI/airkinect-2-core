package com.as3nui.nativeExtensions.air.kinect.data
{
	
	public class Quaternion
	{
		public var x:Number = 0;
		public var y:Number = 0;
		public var z:Number = 0;
		public var w:Number = 0;
		
		public function Quaternion()
		{
		}
		
		public function copyFrom(otherQuaternion:Quaternion):void
		{
			this.x = otherQuaternion.x;
			this.y = otherQuaternion.y;
			this.z = otherQuaternion.z;
			this.w = otherQuaternion.w;
		}
	}
}
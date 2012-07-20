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
package com.as3nui.nativeExtensions.air.kinect.data
{

	/**
	 * The pointcloud region class defines a 3D region in the point cloud.
	 * 
	 * <p>The x &amp; y position is the pixel position in the point cloud image, the z
	 * position is the depth. The <code>numPoints</code> property will be updated by the
	 * extension, and will contain the nr of points in this 3D area</p>
	 */ 
	public class PointCloudRegion
	{
		
		private static var count:uint = 0;
		private var _regionId:uint;
		
		public function get regionId():uint
		{
			return _regionId;
		}
		
		private var _x:Number;
		public function get x():Number { return _x; }
		
		public function set x(value:Number):void
		{
			if (_x == value)
				return;
			_x = value;
		}
		
		private var _y:Number;
		public function get y():Number { return _y; }
		
		public function set y(value:Number):void
		{
			if (_y == value)
				return;
			_y = value;
		}
		
		private var _z:Number;
		public function get z():Number { return _z; }
		
		public function set z(value:Number):void
		{
			if (_z == value)
				return;
			_z = value;
		}
		
		private var _width:Number;
		public function get width():Number { return _width; }
		
		public function set width(value:Number):void
		{
			if (_width == value)
				return;
			_width = value;
		}
		
		private var _height:Number;
		public function get height():Number { return _height; }
		
		public function set height(value:Number):void
		{
			if (_height == value)
				return;
			_height = value;
		}
		
		private var _depth:Number;
		public function get depth():Number { return _depth; }
		
		public function set depth(value:Number):void
		{
			if (_depth == value)
				return;
			_depth = value;
		}
		
		public var numPoints:uint;
		
		public function PointCloudRegion(x:Number, y:Number, z:Number, width:Number, height:Number, depth:Number)
		{
			_regionId = ++count;
			_x = x;
			_y = y;
			_z = z;
			_width = width;
			_height = height;
			_depth = depth;
		}
	}
}
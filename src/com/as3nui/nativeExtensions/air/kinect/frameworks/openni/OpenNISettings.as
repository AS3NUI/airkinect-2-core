/**
 *
 * User: rgerbasi
 * Date: 2/22/12
 * Time: 12:08 PM
 */
package com.as3nui.nativeExtensions.air.kinect.frameworks.openni {
	import com.as3nui.nativeExtensions.air.kinect.DeviceSettings;
	import com.as3nui.nativeExtensions.air.kinect.constants.CameraResolution;

	import flash.geom.Point;

	public class OpenNISettings extends DeviceSettings {

		private var _infraredEnabled:Boolean				= false;
		private var _infraredResolution:Point				= CameraResolution.RESOLUTION_320_240;
		private var _infraredMirrored:Boolean				= true;


		public function get infraredEnabled():Boolean {
			return _infraredEnabled;
		}

		public function set infraredEnabled(value:Boolean):void {
			_infraredEnabled = value;
		}

		public function get infraredResolution():Point {
			return _infraredResolution;
		}

		public function set infraredResolution(value:Point):void {
			_infraredResolution = value;
		}

		public function get infraredMirrored():Boolean {
			return _infraredMirrored;
		}

		public function set infraredMirrored(value:Boolean):void {
			_infraredMirrored = value;
		}
	}
}
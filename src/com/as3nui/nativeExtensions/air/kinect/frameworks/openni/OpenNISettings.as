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
	import flash.utils.describeType;

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
		
		/**
		 * Factory method to create an instance of OpenNISettings, based on an
		 * anonymously typed object
		 */ 
		public static function create(deviceSettings:Object):OpenNISettings
		{
			var settings:OpenNISettings = new OpenNISettings();
			//automatic copying-in properties from the object
			var def:XML = describeType(deviceSettings);
			var props:XMLList = def..variable.@name;
			props += def..accessor.@name;
			for each (var prop:String in props)
			{
				if(settings.hasOwnProperty(prop)) settings[prop] = deviceSettings[prop];
			}
			return settings;
		}
	}
}
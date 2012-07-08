/**
 *
 * User: rgerbasi
 * Date: 2/22/12
 * Time: 12:08 PM
 */
package com.as3nui.nativeExtensions.air.kinect.frameworks.openni {
	import com.as3nui.nativeExtensions.air.kinect.KinectSettings;
	import com.as3nui.nativeExtensions.air.kinect.constants.CameraResolution;
	
	import flash.geom.Point;
	import flash.utils.describeType;

	public class OpenNIKinectSettings extends KinectSettings {

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
		
		override public function copyFrom(otherKinectSettings:KinectSettings):void
		{
			super.copyFrom(otherKinectSettings);
			if(otherKinectSettings is OpenNIKinectSettings)
			{
				var otherOpenNIKinectSettings:OpenNIKinectSettings = otherKinectSettings as OpenNIKinectSettings;
				
				_infraredEnabled = otherOpenNIKinectSettings.infraredEnabled;
				_infraredResolution.copyFrom(otherOpenNIKinectSettings.infraredResolution);
				_infraredMirrored = otherOpenNIKinectSettings.infraredMirrored;
			}
		}
		
		override public function clone():KinectSettings
		{
			var cloned:OpenNIKinectSettings = new OpenNIKinectSettings();
			cloned.copyFrom(this);
			return cloned;
		}
		
		/**
		 * Factory method to create an instance of OpenNISettings, based on an
		 * anonymously typed object
		 */ 
		public static function create(deviceSettings:Object):OpenNIKinectSettings
		{
			var settings:OpenNIKinectSettings = new OpenNIKinectSettings();
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
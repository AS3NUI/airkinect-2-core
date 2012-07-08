/**
 *
 * User: rgerbasi
 * Date: 2/22/12
 * Time: 12:09 PM
 */
package com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk {
	import com.as3nui.nativeExtensions.air.kinect.KinectSettings;
	
	import flash.utils.describeType;

	public class MSKinectSettings extends KinectSettings {
		
		override public function copyFrom(otherKinectSettings:KinectSettings):void
		{
			super.copyFrom(otherKinectSettings);
		}
		
		override public function clone():KinectSettings
		{
			var cloned:MSKinectSettings = new MSKinectSettings();
			cloned.copyFrom(this);
			return cloned;
		}
		
		/**
		 * Factory method to create an instance of MSSettings, based on an
		 * anonymously typed object
		 */ 
		public static function create(deviceSettings:Object):MSKinectSettings
		{
			var settings:MSKinectSettings = new MSKinectSettings();
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
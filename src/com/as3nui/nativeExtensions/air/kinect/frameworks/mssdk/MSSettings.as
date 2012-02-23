/**
 *
 * User: rgerbasi
 * Date: 2/22/12
 * Time: 12:09 PM
 */
package com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk {
	import com.as3nui.nativeExtensions.air.kinect.DeviceSettings;
	
	import flash.utils.describeType;

	public class MSSettings extends DeviceSettings {
		
		/**
		 * Factory method to create an instance of MSSettings, based on an
		 * anonymously typed object
		 */ 
		public static function create(deviceSettings:Object):MSSettings
		{
			var settings:MSSettings = new MSSettings();
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
/**
 *
 * User: rgerbasi
 * Date: 2/22/12
 * Time: 12:26 PM
 */
package com.as3nui.nativeExtensions.air.kinect.constants {
	import flash.system.Capabilities;

	public class OS {
		public static const WINDOWS:String 	= "windows";
		public static const OSX:String		= "osx";
		public static const LINUX:String	= "linux";

		public static function getOS():String {
			var os:String = Capabilities.os;
			if((os.indexOf("Windows") >= 0)) return WINDOWS;
			if((os.indexOf("Mac") >= 0)) return OSX;
			return OSX;
		}

		public static function isOSX():Boolean {
			return getOS() == OSX;
		}

		public static function isWindows():Boolean {
			return getOS() == WINDOWS;
		}
	}
}
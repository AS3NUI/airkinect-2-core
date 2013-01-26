/**
 *
 * User: rgerbasi
 * Date: 2/22/12
 * Time: 12:32 PM
 */
package com.as3nui.nativeExtensions.air.kinect.frameworks.openni {
	import com.as3nui.nativeExtensions.air.kinect.Kinect;
	import com.as3nui.nativeExtensions.air.kinect.KinectSettings;
	import com.as3nui.nativeExtensions.air.kinect.bridge.ExtensionContextBridge;
	
	import flash.display.BitmapData;
	import flash.events.StatusEvent;
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	public class OpenNIKinect extends Kinect {

		public function get openNISettings():OpenNIKinectSettings {
			return _settings as OpenNIKinectSettings;
		}

		public function OpenNIKinect(nr:uint = 0) {
			super(nr);
		}
		
		override protected function parseSettings(deviceSettings:Object):KinectSettings
		{
			return OpenNIKinectSettings.create(deviceSettings);
		}

		override protected function initSettings():void {
			super.initSettings();
		}

		override protected function contextStatusHandler(event:StatusEvent):void {
			super.contextStatusHandler(event);
		}
	}
}
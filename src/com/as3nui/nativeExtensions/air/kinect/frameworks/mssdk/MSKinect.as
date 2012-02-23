/**
 *
 * User: rgerbasi
 * Date: 2/22/12
 * Time: 12:32 PM
 */
package com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk {
	import com.as3nui.nativeExtensions.air.kinect.Kinect;
	import com.as3nui.nativeExtensions.air.kinect.KinectSettings;

	public class MSKinect extends Kinect {

		public function get msSettings():MSKinectSettings { return _settings as MSKinectSettings; }
		public function MSKinect(nr:uint) {
			super(nr);
		}
		
		override protected function parseSettings(deviceSettings:Object):KinectSettings
		{
			return MSKinectSettings.create(deviceSettings);
		}
	}
}
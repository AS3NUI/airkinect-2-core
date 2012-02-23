/**
 *
 * User: rgerbasi
 * Date: 2/22/12
 * Time: 12:32 PM
 */
package com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk {
	import com.as3nui.nativeExtensions.air.kinect.Device;

	public class MSDevice extends Device {

		public function get msSettings():MSSettings { return _settings as MSSettings; }
		public function MSDevice(nr:uint) {
			super(nr);
		}
	}
}
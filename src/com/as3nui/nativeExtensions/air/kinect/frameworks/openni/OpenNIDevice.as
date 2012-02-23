/**
 *
 * User: rgerbasi
 * Date: 2/22/12
 * Time: 12:32 PM
 */
package com.as3nui.nativeExtensions.air.kinect.frameworks.openni {
	import com.as3nui.nativeExtensions.air.kinect.Device;
	import com.as3nui.nativeExtensions.air.kinect.frameworks.openni.OpenNISettings;
	import com.as3nui.nativeExtensions.air.kinect.frameworks.openni.events.OpenNICameraImageEvent;

	import flash.display.BitmapData;
	import flash.events.StatusEvent;
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	[Event(name="infraredImageUpdate", type="com.as3nui.nativeExtensions.air.kinect.events.CameraImageEvent")]
	public class OpenNIDevice extends Device {
		protected static const EXTENSION_EVENT_INFRARED_FRAME_AVAILABLE:String = 'infraredFrame';

		protected static const EXTENSION_REQUEST_SET_INFRARED_MODE:String = "setInfraredMode";
		protected static const EXTENSION_REQUEST_SET_INFRARED_ENABLED:String = "setInfraredEnabled";
		protected static const EXTENSION_REQUEST_GET_INFRARED_FRAME:String = "getInfraredFrame";

		protected var infraredImageBytes:ByteArray;
		protected var infraredImageData:BitmapData;

		public function get openNISettings():OpenNISettings {
			return _settings as OpenNISettings;
		}

		public function OpenNIDevice(nr:uint = 0) {
			super(nr);
		}

		override protected function initSettings():void {
			super.initSettings();
			initInfraredSettings();
		}

		//----------------------------------
		// Infrared Handlers
		//----------------------------------
		private function initInfraredSettings():void {
			infraredImageBytes = new ByteArray();
			infraredImageData = new BitmapData(openNISettings.infraredResolution.x, openNISettings.infraredResolution.y, true, 0x000000);
			applyInfraredSettings()
		}

		protected function applyInfraredSettings():void {
			context.call(EXTENSION_REQUEST_SET_INFRARED_ENABLED, _nr, openNISettings.infraredEnabled);
			context.call(EXTENSION_REQUEST_SET_INFRARED_MODE, _nr, openNISettings.infraredResolution.x, openNISettings.infraredResolution.y, openNISettings.infraredMirrored);
		}

		protected function handleInfraredFrame():void {
			if (!hasEventListener(OpenNICameraImageEvent.INFRARED_IMAGE_UPDATE)) return;
			context.call(EXTENSION_REQUEST_GET_INFRARED_FRAME, _nr, infraredImageBytes);
			infraredImageBytes.position = 0;
			infraredImageBytes.endian = Endian.LITTLE_ENDIAN;
			infraredImageData.setPixels(infraredImageData.rect, infraredImageBytes);

			//dispatch the event
			dispatchEvent(new OpenNICameraImageEvent(OpenNICameraImageEvent.INFRARED_IMAGE_UPDATE, false, false, infraredImageData));
		}

		override protected function contextStatusHandler(event:StatusEvent):void {
			super.contextStatusHandler(event);
			switch (event.code) {
				case EXTENSION_EVENT_INFRARED_FRAME_AVAILABLE:
					handleInfraredFrame();
					break;
			}
		}
	}
}
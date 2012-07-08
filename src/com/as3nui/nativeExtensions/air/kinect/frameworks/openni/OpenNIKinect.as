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
	import com.as3nui.nativeExtensions.air.kinect.frameworks.openni.OpenNIKinectSettings;
	import com.as3nui.nativeExtensions.air.kinect.frameworks.openni.events.OpenNICameraImageEvent;
	
	import flash.display.BitmapData;
	import flash.events.StatusEvent;
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	[Event(name="infraredImageUpdate", type="com.as3nui.nativeExtensions.air.kinect.frameworks.openni.events.OpenNICameraImageEvent")]
	public class OpenNIKinect extends Kinect {

		protected var infraredImageBytes:ByteArray;
		protected var infraredImageData:BitmapData;

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
			contextBridge.setInfraredEnabled(_nr, openNISettings.infraredEnabled);
			contextBridge.setInfraredMode(_nr, openNISettings.infraredResolution.x, openNISettings.infraredResolution.y, openNISettings.infraredMirrored);
		}

		protected function handleInfraredFrame():void {
			if (!hasEventListener(OpenNICameraImageEvent.INFRARED_IMAGE_UPDATE)) return;
			contextBridge.getInfraredFrame(_nr, infraredImageBytes);
			infraredImageBytes.position = 0;
			infraredImageBytes.endian = Endian.LITTLE_ENDIAN;
			infraredImageData.setPixels(infraredImageData.rect, infraredImageBytes);

			//dispatch the event
			dispatchEvent(new OpenNICameraImageEvent(OpenNICameraImageEvent.INFRARED_IMAGE_UPDATE, false, false, infraredImageData));
		}

		override protected function contextStatusHandler(event:StatusEvent):void {
			super.contextStatusHandler(event);
			switch (event.level) {
				case ExtensionContextBridge.EXTENSION_EVENT_INFRARED_FRAME_AVAILABLE:
					handleInfraredFrame();
					break;
			}
		}
	}
}
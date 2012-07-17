package com.as3nui.nativeExtensions.air.kinect.bridge
{
	import com.as3nui.nativeExtensions.air.kinect.data.DeviceCapabilities;
	import com.as3nui.nativeExtensions.air.kinect.data.PointCloudRegion;
	import com.as3nui.nativeExtensions.air.kinect.data.UserFrame;
	
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.StatusEvent;
	import flash.external.ExtensionContext;
	import flash.net.dns.AAAARecord;
	import flash.utils.ByteArray;
	import flash.utils.Dictionary;
	import flash.utils.Endian;
	
	public class ExtensionContextBridge extends EventDispatcher implements IContextBridge
	{
		
		public static const EXTENSION_EVENT_DEVICE_STARTED:String = 'started';
		public static const EXTENSION_EVENT_DEVICE_STOPPED:String = 'stopped';
		
		public static const EXTENSION_EVENT_RGB_FRAME_AVAILABLE:String = 'RGBFrame';
		public static const EXTENSION_EVENT_DEPTH_FRAME_AVAILABLE:String = 'depthFrame';
		public static const EXTENSION_EVENT_USER_FRAME_AVAILABLE:String = 'userFrame';
		public static const EXTENSION_EVENT_USERMASK_FRAME_AVAILABLE:String = 'userMaskFrame';
		public static const EXTENSION_EVENT_POINTCLOUD_FRAME_AVAILABLE:String = 'pointCloudFrame';
		public static const EXTENSION_EVENT_RECONNECT:String = 'deviceReconnected';
		public static const EXTENSION_EVENT_INFRARED_FRAME_AVAILABLE:String = 'infraredFrame';
		
		//SHARED CONTEXT REQUESTS
		/** @private */
		protected static const EXTENSION_REQUEST_APPLICATION_STARTUP:String = "applicationStartup";
		/** @private */
		protected static const EXTENSION_REQUEST_GET_DEVICE_COUNT:String = "getDeviceCount";
		/** @private */
		protected static const EXTENSION_REQUEST_APPLICATION_SHUTDOWN:String = "applicationShutdown";
		
		//Main Context Requests
		/** @private */
		protected static const EXTENSION_REQUEST_START:String = "start";
		/** @private */
		protected static const EXTENSION_REQUEST_STOP:String = "stop";
		
		/** @private */
		protected static const EXTENSION_REQUEST_GET_CAPABILITIES:String = "getCapabilities";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_USER_MODE:String = "setUserMode";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_USER_COLOR:String = "setUserColor";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_USER_ENABLED:String = "setUserEnabled";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_SKELETON_MODE:String = "setSkeletonMode";
		/** @private */
		protected static const EXTENSION_REQUEST_CHOOSE_SKELETONS:String = "chooseSkeletons";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_SKELETON_ENABLED:String = "setSkeletonEnabled";
		/** @private */
		protected static const EXTENSION_REQUEST_GET_USER_FRAME:String = "getUserFrame";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_USER_MASK_MODE:String = "setUserMaskMode";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_USER_MASK_ENABLED:String = "setUserMaskEnabled";
		/** @private */
		protected static const EXTENSION_REQUEST_GET_USER_MASK_FRAME:String = "getUserMaskFrame";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_DEPTH_MODE:String = "setDepthMode";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_DEPTH_ENABLED:String = "setDepthEnabled";
		/** @private */
		protected static const EXTENSION_REQUEST_GET_DEPTH_FRAME:String = "getDepthFrame";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_DEPTH_SHOW_USER_COLORS:String = "setDepthShowUserColors";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_NEAR_MODE_ENABLED:String = "setNearModeEnabled";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_RGB_MODE:String = "setRGBMode";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_RGB_ENABLED:String = "setRGBEnabled";
		/** @private */
		protected static const EXTENSION_REQUEST_GET_RGB_FRAME:String = "getRGBFrame";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_POINTCLOUD_MODE:String = "setPointCloudMode";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_POINTCLOUD_ENABLED:String = "setPointCloudEnabled";
		/** @private */
		protected static const EXTENSION_REQUEST_GET_POINTCLOUD_FRAME:String = "getPointCloudFrame";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_POINTCLOUD_REGIONS:String = "setPointCloudRegions";
		/** @private */
		protected static const EXTENSION_REQUEST_GET_CAMERA_ELEVATION_ANGLE:String = "getCameraElevationAngle";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_CAMERA_ELEVATION_ANGLE:String = "setCameraElevationAngle";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_INFRARED_MODE:String = "setInfraredMode";
		/** @private */
		protected static const EXTENSION_REQUEST_SET_INFRARED_ENABLED:String = "setInfraredEnabled";
		/** @private */
		protected static const EXTENSION_REQUEST_GET_INFRARED_FRAME:String = "getInfraredFrame";
		
		private var context:ExtensionContext;
		
		public function ExtensionContextBridge(shared:Boolean = false)
		{
			context = ExtensionContext.createExtensionContext("com.as3nui.nativeExtensions.air.kinect", (shared) ? "shared" : null);
			context.addEventListener(StatusEvent.STATUS, contextStatusHandler, false, 0, true);
		}
		
		protected function contextStatusHandler(event:Event):void
		{
			dispatchEvent(event.clone());
		}
		
		public function getDeviceCount():uint
		{
			return context.call(EXTENSION_REQUEST_GET_DEVICE_COUNT) as uint;
		}
		
		public function applicationStartup(framework:uint):void
		{
			context.call(EXTENSION_REQUEST_APPLICATION_STARTUP, framework);
		}
		
		public function applicationShutdown():void
		{
			context.call(EXTENSION_REQUEST_APPLICATION_SHUTDOWN);
		}
		
		public function getCapabilities(nr:uint):DeviceCapabilities
		{
			var nativeCapabilities:Object = context.call(EXTENSION_REQUEST_GET_CAPABILITIES, nr) as Object;
			return new DeviceCapabilities(nativeCapabilities);
		}
		
		public function getCameraElevationAngle(nr:uint):int
		{
			return context.call(EXTENSION_REQUEST_GET_CAMERA_ELEVATION_ANGLE, nr) as int;
		}
		
		public function setCameraElevationAngle(nr:uint, value:int):void 
		{
			context.call(EXTENSION_REQUEST_SET_CAMERA_ELEVATION_ANGLE, nr, value);
		}
		
		public function start(nr:uint):void
		{
			context.call(EXTENSION_REQUEST_START, nr);
		}
		
		public function stop(nr:uint):void
		{
			context.call(EXTENSION_REQUEST_STOP, nr);
		}
		
		public function setRgbEnabled(nr:uint, rgbEnabled:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_RGB_ENABLED, nr, rgbEnabled);
		}
		
		public function setRgbMode(nr:uint, width:uint, height:uint, mirrored:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_RGB_MODE, nr, width, height, mirrored);
		}
		
		public function setDepthEnabled(nr:uint, enabled:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_DEPTH_ENABLED, nr, enabled);
		}
		
		public function setDepthMode(nr:uint, width:uint, height:uint, mirrored:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_DEPTH_MODE, nr, width, height, mirrored);
		}
		
		public function setDepthShowUserColors(nr:uint, showUserColors:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_DEPTH_SHOW_USER_COLORS, nr, showUserColors);
		}
		
		public function setNearModeEnabled(nr:uint, enableNearMode:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_NEAR_MODE_ENABLED, nr, enableNearMode);
		}
		
		public function setPointcloudEnabled(nr:uint, enabled:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_POINTCLOUD_ENABLED, nr, enabled);
		}
		
		public function setPointcloudMode(nr:uint, width:uint, height:uint, mirrored:Boolean, density:uint, includeRgb:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_POINTCLOUD_MODE, nr, width, height, mirrored, density, includeRgb);
		}
		
		public function setPointCloudRegions(nr:uint, pointCloudRegions:Vector.<PointCloudRegion>):void
		{
			if (context != null)  context.call(EXTENSION_REQUEST_SET_POINTCLOUD_REGIONS, nr, pointCloudRegions);
		}
		
		public function setUserEnabled(nr:uint, enabled:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_USER_ENABLED, nr, enabled);
		}
		
		public function setUserMode(nr:uint, mirrored:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_USER_MODE, nr, mirrored);
		}
		
		public function setSkeletonEnabled(nr:uint, enabled:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_SKELETON_ENABLED, nr, enabled);
		}
		
		public function setSkeletonMode(nr:uint, mirrored:Boolean, seatedSkeletonEnabled:Boolean, chooseSkeletons:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_SKELETON_MODE, nr, mirrored, seatedSkeletonEnabled, chooseSkeletons);
		}
		
		public function chooseSkeletons(nr:uint, trackingIds:Vector.<uint>):void
		{
			context.call(EXTENSION_REQUEST_CHOOSE_SKELETONS, nr, trackingIds);
		}
		
		public function setUserMaskEnabled(nr:uint, enabled:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_USER_MASK_ENABLED, nr, enabled);
		}
		
		public function setUserMaskMode(nr:uint, width:uint, height:uint, mirrored:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_USER_MASK_MODE, nr, width, height, mirrored);
		}
		
		public function getDepthFrame(nr:uint, imageBytes:ByteArray):void
		{
			context.call(EXTENSION_REQUEST_GET_DEPTH_FRAME, nr, imageBytes);
			imageBytes.position = 0;
			imageBytes.endian = Endian.LITTLE_ENDIAN;
		}
		
		public function getRgbFrame(nr:uint, imageBytes:ByteArray):void
		{
			context.call(EXTENSION_REQUEST_GET_RGB_FRAME, nr, imageBytes);
			imageBytes.position = 0;
			imageBytes.endian = Endian.LITTLE_ENDIAN;
		}
		
		public function getPointcloudFrame(nr:uint, bytes:ByteArray, regions:Vector.<PointCloudRegion>):void
		{
			context.call(EXTENSION_REQUEST_GET_POINTCLOUD_FRAME, nr, bytes, regions);
			bytes.position = 0;
			bytes.endian = Endian.LITTLE_ENDIAN;
		}
		
		public function getUserMaskFrame(nr:uint, userID:uint, maskByteArray:ByteArray):void
		{
			context.call(EXTENSION_REQUEST_GET_USER_MASK_FRAME, nr, userID, maskByteArray);
			maskByteArray.position = 0;
			maskByteArray.endian = Endian.LITTLE_ENDIAN;
		}
		
		public function getUserFrame(nr:uint):UserFrame
		{
			return context.call(EXTENSION_REQUEST_GET_USER_FRAME, nr) as UserFrame;
		}
		
		public function setInfraredEnabled(nr:uint, enabled:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_INFRARED_ENABLED, nr, enabled);
		}
		
		public function setInfraredMode(nr:uint, width:uint, height:uint, mirrored:Boolean):void
		{
			context.call(EXTENSION_REQUEST_SET_INFRARED_MODE, nr, width, height, mirrored);
		}
		
		public function getInfraredFrame(nr:uint, imageBytes:ByteArray):void
		{
			context.call(EXTENSION_REQUEST_GET_INFRARED_FRAME, nr, imageBytes);
			imageBytes.position = 0;
			imageBytes.endian = Endian.LITTLE_ENDIAN;
		}
	}
}
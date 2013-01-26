package com.as3nui.nativeExtensions.air.kinect.manager
{
	import avmplus.getQualifiedClassName;
	
	import com.as3nui.nativeExtensions.air.kinect.Kinect;
	import com.as3nui.nativeExtensions.air.kinect.bridge.ExtensionContextBridge;
	import com.as3nui.nativeExtensions.air.kinect.bridge.IContextBridge;
	import com.as3nui.nativeExtensions.air.kinect.constants.Framework;
	import com.as3nui.nativeExtensions.air.kinect.constants.OS;
	import com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.MSKinect;
	import com.as3nui.nativeExtensions.air.kinect.frameworks.openni.OpenNIKinect;
	
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.StatusEvent;
	import flash.utils.Dictionary;
	import flash.utils.getDefinitionByName;
	
	/**
	 * Dispatched when a device is connected to the system.
	 */ 
	[Event(name="connected", type="com.as3nui.nativeExtensions.air.kinect.events.DeviceEvent")]
	
	/**
	 * Dispatched when a device is disconnected from the system.
	 */ 
	[Event(name="disconnected", type="com.as3nui.nativeExtensions.air.kinect.events.DeviceEvent")]
	
	public class KinectManager extends EventDispatcher
	{
		
		private static var instance:KinectManager;
		
		public static function getInstance():KinectManager
		{
			if(instance == null)
				instance = new KinectManager(new Enforcer());
			return instance;
		}
		
		private var _deviceInstanceMap:Dictionary;
		
		private var _sharedBridge:IContextBridge;
		
		private function get sharedBridge():IContextBridge {
			if (_sharedBridge == null)
				_sharedBridge = new ExtensionContextBridge(true);
			return _sharedBridge;
		}
		
		public function KinectManager(e:Enforcer)
		{
			if(e == null)
				throw("KinectManager is a singleton and cannot be instantiated directly");
			
			sharedBridge.addEventListener(StatusEvent.STATUS, statusHandler, false, 0, true);
			sharedBridge.applicationStartup(0);
		}
		
		protected function statusHandler(event:StatusEvent):void
		{
		}
		
		/**
		 * Check if there is a Sensor available for use
		 */
		public function isSupported():Boolean {
			return (numDevices() > 0);
		}
		
		/**
		 * Get the current number of sensors connected to the system
		 * @return    Number of Sensors connected
		 */
		public function numDevices():uint {
			return sharedBridge.getDeviceCount();
		}
		
		/**
		 * Get an instance of the Kinect class. You'll use this method to get
		 * instances of the class, instead of creating instances yourself.
		 * @param deviceClass
		 * @param nr
		 * @return
		 */
		public function getDeviceByClass(deviceClass:Class, nr:uint = 0):Kinect {
			if(!isDeviceRegistered(deviceClass, nr))
			{
				var device:Kinect = new deviceClass(nr);
				registerDevice(device);
			}
			return _deviceInstanceMap[deviceClass][nr];
		}
		
		/**
		 * Get an instance of the Kinect class. You'll use this method to get
		 * instances of the class, instead of creating instances yourself.
		 * @param framework        @see com.as3nui.nativeExtensions.air.kinect.constants.Framework
		 * @param nr        DeviceId to get
		 * @return
		 */
		public function getDeviceByFramework(framework:String, nr:uint = 0):Kinect {
			var deviceClass:Class = Framework.MSSDK ? MSKinect : OpenNIKinect;
			return getDeviceByClass(deviceClass, nr);
		}
		
		/**
		 * Automatically chooses from the "default" devices for an operating system
		 * this is currently OpenNI for OSX and MSSDK / OpenNI for windows (depending on the ane)
		 * @param nr    Device number to get
		 * @return
		 */
		public function getDevice(nr:uint = 0):Kinect {
			var deviceClass:Class = OS.isWindows() ? MSKinect : OpenNIKinect;
			return getDeviceByClass(deviceClass, nr);
		}
		
		public function registerDevice(device:Kinect):void {
			var className:String = getQualifiedClassName(device);
			var deviceClass:Class = getDefinitionByName(className) as Class;
			if(!isDeviceRegistered(deviceClass, device.nr))
				_deviceInstanceMap[deviceClass][device.nr] = device;
		}
		
		private function isDeviceRegistered(deviceClass:Class, nr:uint):Boolean {
			setupDictionaryForDevice(deviceClass);
			if (_deviceInstanceMap[deviceClass][nr] != null)
				return true;
			return false;
		}
		
		private function setupDictionaryForDevice(deviceClass:Class):void {
			if (_deviceInstanceMap == null) 
				_deviceInstanceMap = new Dictionary();
			if (_deviceInstanceMap[deviceClass] == null)
				_deviceInstanceMap[deviceClass] = new Dictionary();
		}
		
		/**
		 * Remove a registered device from the dictionary.
		 */ 
		public function disposeDevice(device:Kinect):void {
			var className:String = getQualifiedClassName(device);
			if(className != null)
			{
				try
				{
					var managementClass:Class = getDefinitionByName(className) as Class;
					if(_deviceInstanceMap[managementClass] != null && _deviceInstanceMap[managementClass][device.nr] != null)
						delete _deviceInstanceMap[managementClass][device.nr];
				}
				catch(error:Error)
				{
				}
			}
		}
		
		/**
		 * Called when the application is closed.
		 * Used to clean up native code in the extension.
		 */ 
		public function applicationShutdown():void
		{
			sharedBridge.removeEventListener(StatusEvent.STATUS, statusHandler, false);
			sharedBridge.applicationShutdown();
		}
	}
}
internal class Enforcer{};
/**
 *
 * User: rgerbasi
 * Date: 2/22/12
 * Time: 12:32 PM
 */
package com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk {
	import com.as3nui.nativeExtensions.air.kinect.Kinect;
	import com.as3nui.nativeExtensions.air.kinect.KinectSettings;
	import com.as3nui.nativeExtensions.air.kinect.data.User;
	import com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.data.MSHand;
	import com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.events.MSHandEvent;
	
	import flash.events.Event;

	[Event(name="grip", type="com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.events.MSHandEvent")]
	[Event(name="gripRelease", type="com.as3nui.nativeExtensions.air.kinect.frameworks.mssdk.events.MSHandEvent")]
	public class MSKinect extends Kinect {

		public function get msSettings():MSKinectSettings { return _settings as MSKinectSettings; }
		public function MSKinect(nr:uint) {
			super(nr);
		}
		
		override protected function parseSettings(deviceSettings:Object):KinectSettings {
			return MSKinectSettings.create(deviceSettings);
		}
		
		/** @private */
		override protected function updateExistingUser(existingUser:User, otherUser:User):void {
			var leftHandEvent:Event = getHandEventIfNeeded(existingUser.leftHand as MSHand, otherUser.leftHand as MSHand);
			var rightHandEvent:Event = getHandEventIfNeeded(existingUser.rightHand as MSHand, otherUser.rightHand as MSHand);
			existingUser.copyFrom(otherUser);
			if(leftHandEvent) {
				dispatchEvent(leftHandEvent);
			}
			if(rightHandEvent) {
				dispatchEvent(rightHandEvent);
			}
		}
		
		private function getHandEventIfNeeded(currentHand:MSHand, updatedHand:MSHand):Event {
			if(currentHand.event != updatedHand.event) {
				switch(updatedHand.event) {
					case MSHand.EVENT_GRIP:
						return new MSHandEvent(currentHand, MSHandEvent.GRIP);
						break;
					case MSHand.EVENT_GRIP_RELEASE:
						return new MSHandEvent(currentHand, MSHandEvent.GRIP_RELEASE);
						break;
				}
			}
			return null;
		}
	}
}
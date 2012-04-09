/*
 * Copyright 2012 AS3NUI
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
package com.as3nui.nativeExtensions.air.kinect.events {
	import flash.events.Event;

	public class DeviceInfoEvent extends Event {

		public static const INFO:String = "info";
		private var _message:String;

		public function DeviceInfoEvent(message:String) {
			super(INFO);
			_message = message
		}

		override public function clone():Event {
			return new DeviceInfoEvent(message);
		}

		public function get message():String {
			return _message;
		}
	}
}
/**
 *
 * User: Ross
 * Date: 2/20/12
 * Time: 1:36 PM
 */
package com.as3nui.nativeExtensions.air.kinect.data {
	import com.as3nui.nativeExtensions.air.kinect.constants.Framework;

	public class DeviceCapabilities {
		private var _hasCameraElevationSupport:Boolean					= false;
		private var _hasRGBCameraSupport:Boolean						= false;
		private var _hasDepthCameraSupport:Boolean						= false;
		private var _hasDepthUserSupport:Boolean						= false;
		private var _hasPointCloudSupport:Boolean						= false;
		private var _hasPointCloudRegionSupport:Boolean					= false;
		private var _hasUserMaskSupport:Boolean							= false;
		private var _hasInfraredSupport:Boolean							= false;
		private var _hasSkeletonSupport:Boolean							= false;
		private var _hasJointOrientationSupport:Boolean					= false;
		private var _hasJointOrientationConfidenceSupport:Boolean		= false;
		private var _hasPositionConfidenceSupport:Boolean				= false;
		private var _hasMultipleSensorSupport:Boolean					= false;

		private var _maxSensors:uint									= 0;

		private var _framework:String									= Framework.NONE;

		public function DeviceCapabilities(capabilities:Object) {
			for (var capability:String in capabilities) {
				var hasProperty:Boolean = this.hasOwnProperty(capability);
				if (hasProperty) this["_" + capability] = capabilities[capability];
			}
		}

		public function get hasCameraElevationSupport():Boolean {
			return _hasCameraElevationSupport;
		}

		public function get hasRGBCameraSupport():Boolean {
			return _hasRGBCameraSupport;
		}

		public function get hasDepthCameraSupport():Boolean {
			return _hasDepthCameraSupport;
		}

		public function get hasDepthUserSupport():Boolean {
			return _hasDepthUserSupport;
		}

		public function get hasPointCloudSupport():Boolean {
			return _hasPointCloudSupport;
		}

		public function get hasPointCloudRegionSupport():Boolean {
			return _hasPointCloudRegionSupport;
		}

		public function get hasUserMaskSupport():Boolean {
			return _hasUserMaskSupport;
		}

		public function get hasInfraredSupport():Boolean {
			return _hasInfraredSupport;
		}

		public function get hasSkeletonSupport():Boolean {
			return _hasSkeletonSupport;
		}

		public function get hasJointOrientationSupport():Boolean {
			return _hasJointOrientationSupport;
		}

		public function get hasJointOrientationConfidenceSupport():Boolean {
			return _hasJointOrientationConfidenceSupport;
		}

		public function get hasPositionConfidenceSupport():Boolean {
			return _hasPositionConfidenceSupport;
		}

		public function get hasMultipleSensorSupport():Boolean {
			return _hasMultipleSensorSupport;
		}

		public function get maxSensors():uint {
			return _maxSensors;
		}

		public function get framework():String {
			return _framework;
		}
	}
}
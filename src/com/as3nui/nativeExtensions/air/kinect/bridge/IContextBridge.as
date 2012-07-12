package com.as3nui.nativeExtensions.air.kinect.bridge
{
	import com.as3nui.nativeExtensions.air.kinect.data.DeviceCapabilities;
	import com.as3nui.nativeExtensions.air.kinect.data.PointCloudRegion;
	import com.as3nui.nativeExtensions.air.kinect.data.UserFrame;
	
	import flash.events.IEventDispatcher;
	import flash.utils.ByteArray;
	import flash.utils.Dictionary;

	public interface IContextBridge extends IEventDispatcher
	{
		
		function getDeviceCount():uint;
		function applicationStartup(framework:uint):void;
		function applicationShutdown():void;
		function getCapabilities(nr:uint):DeviceCapabilities;
		function getCameraElevationAngle(nr:uint):int;
		function setCameraElevationAngle(nr:uint, value:int):void;
		function start(nr:uint):void;
		function stop(nr:uint):void;
		function setRgbEnabled(nr:uint, rgbEnabled:Boolean):void;
		function setRgbMode(nr:uint, width:uint, height:uint, mirrored:Boolean):void;
		function setDepthEnabled(nr:uint, enabled:Boolean):void;
		function setDepthMode(nr:uint, width:uint, height:uint, mirrored:Boolean):void;
		function setDepthShowUserColors(nr:uint, showUserColors:Boolean):void;
		function setNearModeEnabled(nr:uint, enableNearMode:Boolean):void;
		function setPointcloudEnabled(nr:uint, enabled:Boolean):void;
		function setPointcloudMode(nr:uint, width:uint, height:uint, mirrored:Boolean, density:uint, includeRgb:Boolean):void;
		function setPointCloudRegions(nr:uint, pointCloudRegions:Vector.<PointCloudRegion>):void;
		function setUserEnabled(nr:uint, enabled:Boolean):void;
		function setUserMode(nr:uint, mirrored:Boolean):void;
		function setSkeletonEnabled(nr:uint, enabled:Boolean):void;
		function setSkeletonMode(nr:uint, mirrored:Boolean, seatedSkeletonEnabled:Boolean, chooseSkeletons:Boolean):void;
		function chooseSkeletons(nr:uint, trackingIds:Vector.<uint>):void;
		function setUserMaskEnabled(nr:uint, enabled:Boolean):void;
		function setUserMaskMode(nr:uint, width:uint, height:uint, mirrored:Boolean):void;
		function getDepthFrame(nr:uint, imageBytes:ByteArray):void;
		function getRgbFrame(nr:uint, imageBytes:ByteArray):void;
		function getPointcloudFrame(nr:uint, bytes:ByteArray, regions:Vector.<PointCloudRegion>):void;
		function getUserMaskFrame(nr:uint, userID:uint, maskByteArray:ByteArray):void;
		function getUserFrame(nr:uint):UserFrame;
		function setInfraredEnabled(nr:uint, enabled:Boolean):void;
		function setInfraredMode(nr:uint, width:uint, height:uint, mirrored:Boolean):void;
		function getInfraredFrame(nr:uint, imageBytes:ByteArray):void;
	}
}
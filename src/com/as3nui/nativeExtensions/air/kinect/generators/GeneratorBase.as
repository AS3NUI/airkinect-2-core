package com.as3nui.nativeExtensions.air.kinect.generators
{
	import com.as3nui.nativeExtensions.air.kinect.KinectConfig;
	
	import flash.events.EventDispatcher;
	import flash.events.StatusEvent;
	import flash.external.ExtensionContext;
	
	public class GeneratorBase extends EventDispatcher
	{
		
		private var _enabled:Boolean;
		public function get enabled():Boolean { return _enabled; }
		
		public function set enabled(value:Boolean):void
		{
			if (_enabled == value)
				return;
			_enabled = value;
		}
		
		protected var nr:uint;
		
		protected var started:Boolean;
		protected var context:ExtensionContext;
		protected var config:KinectConfig;
		
		public function GeneratorBase(nr:uint)
		{
			this.nr = nr;
		}
		
		public function start(context:ExtensionContext, config:KinectConfig):void
		{
			if(!started)
			{
				started = true;
				this.context = context;
				this.config = config;
				applyConfig();
				context.addEventListener(StatusEvent.STATUS, statusHandler, false, 0, true);
				onStart();
			}
		}
		
		protected function applyConfig():void
		{
		}
		
		protected function onStart():void
		{
		}
		
		protected function onStop():void
		{
		}
		
		public function stop():void
		{
			if(started)
			{
				started = false;
				context.removeEventListener(StatusEvent.STATUS, statusHandler);
				onStop();
				this.context = null;
			}
		}
		
		protected function statusHandler(event:StatusEvent):void
		{
		}
	}
}
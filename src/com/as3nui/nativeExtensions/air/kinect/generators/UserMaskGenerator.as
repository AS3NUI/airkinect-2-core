package com.as3nui.nativeExtensions.air.kinect.generators
{
	import com.as3nui.nativeExtensions.air.kinect.data.User;
	import com.as3nui.nativeExtensions.air.kinect.events.UserEvent;
	
	import flash.display.BitmapData;
	import flash.events.StatusEvent;
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	[Event(name="usersMaskImageUpdate", type="com.as3nui.nativeExtensions.air.kinect.events.UserEvent")]
	public class UserMaskGenerator extends GeneratorBase
	{
		
		protected var _width:uint;
		
		public function get width():uint
		{
			return _width;
		}
		
		protected var _height:uint;
		
		public function get height():uint
		{
			return _height;
		}
		
		protected var _mirrored:Boolean;
		
		public function get mirrored():Boolean
		{
			return _mirrored;
		}
		
		private var userGenerator:UserGenerator;
		private var userMaskByteArrays:Vector.<ByteArray>;
		
		public function UserMaskGenerator(userGenerator:UserGenerator, nr:uint)
		{
			super(nr);
			this.userGenerator = userGenerator;
		}
		
		override protected function applyConfig():void
		{
			enabled = config.userMaskEnabled;
			_width = config.userMaskWidth;
			_height = config.userMaskHeight;
			_mirrored = config.userMaskMirrored;
		}
		
		override protected function onStart():void
		{
			super.onStart();
			
			userMaskByteArrays = new Vector.<ByteArray>();
			for(var i:uint = 0; i < 15; i++)
			{
				userMaskByteArrays.push(new ByteArray());
			}
			
			context.call("setUserMaskEnabled", nr, enabled);
			context.call("setUserMaskMode", nr, _width, _height, _mirrored);
		}
		
		override protected function statusHandler(event:StatusEvent):void
		{
			switch(event.code)
			{
				case "userMaskFrame":
					//get the image for each user
					var i:uint = 0;
					for each(var user:User in userGenerator.users)
					{
						context.call("getUserMaskFrame", nr, user.trackingID, userMaskByteArrays[i]);
						userMaskByteArrays[i].position = 0;
						userMaskByteArrays[i].endian = Endian.LITTLE_ENDIAN;
						if(user.userMaskData == null)
						{
							user.userMaskData = new BitmapData(_width, _height, true, 0);
						}
						user.userMaskData.setPixels(user.userMaskData.rect, userMaskByteArrays[i]);
						i++;
					}
					if(userGenerator.users.length > 0)
					{
						dispatchEvent(new UserEvent(UserEvent.USERS_MASK_IMAGE_UPDATE, false, false, userGenerator.users));
					}
					break;
			}
		}
	}
}
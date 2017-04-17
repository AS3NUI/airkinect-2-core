# AIRKinect

AIRKinect Extension is a Native Extension for use with Adobe AIR 3.0. AIRKinect allows AIR developers to tap into the rich experience of the Microsoft Kinect and push interactivity to a new level.

You can find the compiled .ane file in the bin directory of this repository.

## Driver Installation


Before you can use the native extension in your AIR project, you will need to install the kinect drivers. You have several options, depending on your platform.


### Windows:


#### MS SDK version: (airkinect-2-core-mssdk.ane)

1. Install Microsoft Kinect SDK: <http://kinectforwindows.org/>
2. That's it

#### OpenNI version: (airkinect-2-core-openni.ane)

You can use the instructions "Install OpenNI,NITE and the Sensor Driver" of the SimpleOpenNI project to get OpenNI up and running on your windows 7 machine: <https://code.google.com/p/simple-openni/wiki/Installation_PreOpenNI2#Windows>. Make sure you install the 32-bit version. AIRKinect does not work with the 64-bit version.


### OSX 10.6+:

You can use the instructions "Install OpenNI the short way" of the SimpleOpenNI project to get OpenNI up and running on your OSX machine: <https://code.google.com/p/simple-openni/wiki/Installation_PreOpenNI2#OSX>.

## Linking AIRKinect to your project


All you need is the ane file matching your driver. If you are using the MS SDK, you will need [airkinect-2-core-mssdk.ane](https://github.com/AS3NUI/airkinect-2-core/raw/master/bin/airkinect-2-core-mssdk.ane), if you are using OpenNI, you will need [airkinect-2-core-openni.ane](https://github.com/AS3NUI/airkinect-2-core/raw/master/bin/airkinect-2-core-openni.ane).

Once you have the correct file, you will need to link it to your AIR project:

### Flash Builder 4.6


1. Right click on your AIR for desktop project and choose properties.
2. Select Actionscript build path > Library path and click on Add SWC… Select the ane file you just downloaded.
3. In that same window, choose Native Extensions and click on Add ANE… Select that same ane file.
4. Select Actionscript Build Packaging > Native extensions. Check the checkbox next to the native extension. Ignore the warning that says the extension isn't used.

### Flash CS6


1. Go the File > Actionscript settings.
2. On the Library Path tab, click on the "Browse to a Native Extension (ANE)" button (button to the right of the SWC button)
3. Choose the ane file you just downloaded.

### IntelliJ IDEA


1. Right click on your module and choose "Open Module Settings".
2. Select the build configuration for your Module and open the Dependencies tab
3. Click on the plus (+) button on the bottom of that window and choose "New Library…"
4. Choose the ane file you just downloaded

## Basic Usage


AIRKinect has a lot of built-in features. You can check some examples in our [examples repository](https://github.com/AS3NUI/airkinect-2-examples).

A quick example, showing the depth image would be something like this:

	private var depthBitmap:Bitmap;
    private var device:Kinect;
    
    public function Demo() {
	    if (Kinect.isSupported()) {
			device = Kinect.getDevice();
            depthBitmap = new Bitmap();
            addChild(depthBitmap);
	device.addEventListener(CameraImageEvent.DEPTH_IMAGE_UPDATE, depthImageUpdateHandler);
            var settings:KinectSettings = new KinectSettings();
            settings.depthEnabled = true;            
            device.start(settings);
        }
    }
    
    protected function depthImageUpdateHandler(event:CameraImageEvent):void {
        depthBitmap.bitmapData = event.imageData;
    }
    
    



## License
Copyright 2012 AS3NUI
Licensed under the Apache License, Version 2.0 (the ""License");

You may not use this file except in compliance with the License. You may obtain a copy of the License at

<http://www.apache.org/licenses/LICENSE-2.0/>

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and limitations under the License.

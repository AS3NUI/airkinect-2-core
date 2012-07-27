# AIRKinect

AIRKinect Extension is a Native Extension for use with Adobe AIR 3.0. AIRKinect allows AIR developers to tap into the rich experience of the Microsoft Kinect and push interactivity to a new level.

You can find the compiled .ane file in the bin directory of this repository.

##License:
Copyright 2012 AS3NUI
Licensed under the Apache License, Version 2.0 (the ""License");

You may not use this file except in compliance with the License. You may obtain a copy of the License at

<http://www.apache.org/licenses/LICENSE-2.0/>

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and limitations under the License.

##Requirements:


Actionscript IDE supporting AIR 3.0 projects. (Flash Builder 4.6, IntelliJ, FDT, ...)

###Windows:


####MS SDK version: (airkinect-2-core-mssdk.ane)

1. Install Microsoft Kinect SDK: <http://kinectforwindows.org/>
2. That's it

####OpenNI version: (airkinect-2-core-openni.ane)

You can use the instructions "Install OpenNI,NITE and the Sensor Driver" of the SimpleOpenNI project to get OpenNI up and running on your windows 7 machine: <http://code.google.com/p/simple-openni/wiki/Installation#Windows>. Make sure you install the 32-bit version. AIRKinect does not work with the 64-bit version.


###OSX 10.6+:

You can use the instructions "Install OpenNI the short way" of the SimpleOpenNI project to get OpenNI up and running on your OSX machine: <http://code.google.com/p/simple-openni/wiki/Installation#OSX>.

Alternatively, you can follow the instructions below:

1. Install XCode (required by Macports)
2. Install Macports: <http://www.macports.org>
3. Execute the following command in Terminal: `sudo port install libtool`
4. Execute the following command in Terminal: `sudo port install libusb-devel +universal`
5. Download & install OpenNI OSX binary (unstable): <http://www.openni.org/>
6. Download & install Nite OSX binary (unstable): <http://www.openni.org/>
7. Download & install SensorKinect: <https://github.com/avin2/SensorKinect>
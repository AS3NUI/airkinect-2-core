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
#ifndef KinectExtension_ExtensionConfig_h
#define KinectExtension_ExtensionConfig_h

//this file contains extension specific config
//we can use this to set platform / driver specific parameters at compile time
//You can define the Operating System (AIRKINECT_OS_WINDOWS / AIRKINECT_OS_OSX)
//and the framework (AIRKINECT_TARGET_MSSDK / AIRKINECT_TARGET_OPENNI)
//normally, you will define these in your preprocessor definitions

#ifdef AIRKINECT_OS_WINDOWS
	#ifdef AIRKINECT_TARGET_OPENNI
		#pragma comment(lib, "openNI.lib") //include openni library
	#endif
	#ifdef AIRKINECT_TARGET_MSSDK
		#pragma comment(lib, "Kinect10.lib") //include kinect sdk library
		#pragma comment(lib, "KinectInteraction170_32.lib") //include kinect interaction library
	#endif
	#include <windows.h>
	#include <ole2.h>
	#ifdef AIRKINECT_TARGET_OPENNI
		#ifdef AIRKINECT_TARGET_MSSDK
			#define AIRKINECT_TARGET_BOTH
		#endif
	#endif
#endif

#ifdef AIRKINECT_TARGET_MSSDK
	#include <NuiApi.h>
	#include <KinectInteraction.h>
#endif

#endif
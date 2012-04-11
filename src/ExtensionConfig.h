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
	#include "NuiApi.h"
#endif

#endif
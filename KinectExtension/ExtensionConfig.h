#ifndef KinectExtension_ExtensionConfig_h
#define KinectExtension_ExtensionConfig_h

//this file contains extension specific config
//we can use this to set platform / driver specific parameters

const int AIRKINECT_TARGET_OPENNI = 1;

#ifdef AIRKINECT_TARGET_MSSDK
	//MS SDK specific values
	const int MAX_SKELETONS = NUI_SKELETON_COUNT;
	const int NUM_JOINTS = 20;
#else
	//OpenNI specific values
	const int MAX_SKELETONS = 15;
	const int NUM_JOINTS = 24;
#endif

#endif
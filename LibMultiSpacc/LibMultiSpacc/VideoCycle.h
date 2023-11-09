#ifndef _MultiSpacc_VideoCycle_h_
#define _MultiSpacc_VideoCycle_h_

bool MultiSpacc_WaitUpdateDisplay( MultiSpacc_Window *window, Uint32 *nextTick );

//#if defined(MultiSpacc_Target_NES)
	//bool MultiSpacc_WaitUpdateDisplay_NES(void);
	//#define MultiSpacc_WaitUpdateDisplay( a, b ) MultiSpacc_WaitUpdateDisplay_NES()
//#endif

#endif // _MultiSpacc_VideoCycle_h_

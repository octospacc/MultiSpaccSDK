#pragma once

#ifndef MultiSpacc_Target_NDS
	#define MultiSpacc_Target_NDS
#endif

#include "../MultiSpacc.h"
#include <nds.h>

#define Uint32 int32
#define MultiSpacc_Surface PrintConsole
#define MultiSpacc_Window PrintConsole

typedef struct MultiSpacc_Event {
    int Keys;
} MultiSpacc_Event;

int MultiSpacc_UpdateWindowSurface( MultiSpacc_Window *Window );

void MultiSpacc_Sleep( int milliseconds );
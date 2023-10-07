#pragma once

#ifndef MultiSpacc_Target_NDS
	#define MultiSpacc_Target_NDS
#endif

#include "../MultiSpacc.h"
#include <nds.h>

#define Uint32 int32
#define MultiSpacc_Window PrintConsole
#define MultiSpacc_Surface PrintConsole
#define MultiSpacc_Sleep {}

typedef struct MultiSpacc_Event {
    int Keys;
} MultiSpacc_Event;

int MultiSpacc_UpdateWindowSurface( MultiSpacc_Window *Window );

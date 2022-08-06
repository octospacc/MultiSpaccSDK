#pragma once
#include <stdbool.h>
#include <stddef.h>

#ifdef MultiSpacc_Target_SDL12
	#include "SDL12/SDL.h"
#endif

#ifdef MultiSpacc_Target_SDL20
	#include "SDL20/SDL.h"
#endif

MultiSpacc_Window *MultiSpacc_SetWindow( int Width, int Height, int Bits, Uint32 Flags );
MultiSpacc_Surface *MultiSpacc_GetWindowSurface( MultiSpacc_Window *Window );

void MultiSpacc_SetAppTitle( MultiSpacc_Window *Window, const char *Title );
void MultiSpacc_SetAppIcon( MultiSpacc_Window *Window, MultiSpacc_Surface *Icon );

int MultiSpacc_SetColorKey( MultiSpacc_Surface *Surface, bool Flag, Uint32 Key );

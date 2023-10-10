#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifdef MultiSpacc_Target_SDL12
	#include "SDL12/SDL.h"
	#include "SDLCom/SDL.h"
#endif

#ifdef MultiSpacc_Target_SDL20
	#include "SDL20/SDL.h"
	#include "SDLCom/SDL.h"
#endif

#ifdef MultiSpacc_Target_NDS
	#include "NDS/NDS.h"
#endif

typedef struct MultiSpacc_SurfaceConfig {
	int Width;
	int Height;
	int Bits;
	Uint32 Flags;
} MultiSpacc_SurfaceConfig;

MultiSpacc_Window MultiSpacc_SetWindow( MultiSpacc_SurfaceConfig WindowConfig );
MultiSpacc_Surface *MultiSpacc_GetWindowSurface( MultiSpacc_Window *Window );

void MultiSpacc_SetAppTitle( MultiSpacc_Window *Window, const char Title[] );
void MultiSpacc_SetAppIcon( MultiSpacc_Window *Window, MultiSpacc_Surface *Icon );

MultiSpacc_Surface *MultiSpacc_LoadImage( char FilePath[], MultiSpacc_Surface *Screen, Uint32 *ColorKey );
int MultiSpacc_SetColorKey( MultiSpacc_Surface *Surface, bool Flag, Uint32 Key );

int MultiSpacc_PollEvent( MultiSpacc_Event *Event );

void MultiSpacc_PrintText( char Text[], MultiSpacc_Surface *Surface, MultiSpacc_SurfaceConfig WindowConfig, int x, int y, MultiSpacc_Surface *Tiles /*, int FontSize, int Color */ ); // WIP

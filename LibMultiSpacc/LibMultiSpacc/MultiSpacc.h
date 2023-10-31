#pragma once
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifdef MultiSpacc_Target_SDL12
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
	#include <SDL/SDL_mixer.h>
	#include <SDL/SDL_ttf.h>
	#define MultiSpacc_Window SDL_Surface
	#define MultiSpacc_UpdateWindowSurface SDL_Flip
	typedef struct MultiSpacc_Event {
		Uint32 Type;
		SDLKey Key;
	} MultiSpacc_Event;
#endif

#ifdef MultiSpacc_Target_SDL20
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_mixer.h>
	#include <SDL2/SDL_ttf.h>
	#define MultiSpacc_Window SDL_Window
	#define MultiSpacc_UpdateWindowSurface SDL_UpdateWindowSurface
	typedef struct MultiSpacc_Event {
		Uint32 Type;
		SDL_Keycode Key;
	} MultiSpacc_Event;
#endif

#ifdef MultiSpacc_Target_SDLCom
	#define MultiSpacc_Init SDL_Init
	#define MultiSpacc_Surface SDL_Surface
	#define MultiSpacc_GetTicks SDL_GetTicks
	#define MultiSpacc_Sleep SDL_Delay
	#define MultiSpacc_Rect SDL_Rect
#endif

#ifdef MultiSpacc_Target_NDS
	#include <nds.h>
	#define Uint32 int32
	#define MultiSpacc_Surface PrintConsole
	#define MultiSpacc_Window PrintConsole
	typedef struct MultiSpacc_Event {
		int Keys;
	} MultiSpacc_Event;
	int MultiSpacc_UpdateWindowSurface( MultiSpacc_Window *Window );
	void MultiSpacc_Sleep( int milliseconds );
#endif

#ifdef MultiSpacc_Target_NES
	#include <nes.h>
	#include "neslib.h"
#endif

typedef struct MultiSpacc_SurfaceConfig {
	int Width;
	int Height;
	int Bits;
	Uint32 Flags;
} MultiSpacc_SurfaceConfig;

MultiSpacc_Window *MultiSpacc_SetWindow( MultiSpacc_SurfaceConfig WindowConfig );
MultiSpacc_Surface *MultiSpacc_GetWindowSurface( MultiSpacc_Window *Window );

void MultiSpacc_SetAppTitle( MultiSpacc_Window *Window, const char Title[] );
void MultiSpacc_SetAppIcon( MultiSpacc_Window *Window, MultiSpacc_Surface *Icon );

MultiSpacc_Surface *MultiSpacc_LoadImage( char FilePath[], MultiSpacc_Surface *Screen, Uint32 *ColorKey );
int MultiSpacc_SetColorKey( MultiSpacc_Surface *Surface, bool Flag, Uint32 Key );

int MultiSpacc_PollEvent( MultiSpacc_Event *Event );

void MultiSpacc_PrintDebug( const char *format, ... );
void MultiSpacc_PrintText( char Text[], MultiSpacc_Surface *Surface, MultiSpacc_SurfaceConfig WindowConfig, int x, int y, MultiSpacc_Surface *Tiles /*, int FontSize, int Color */ ); // WIP

void MultiSpacc_Sprite( int id, int x, int y, int sprite, MultiSpacc_Surface *Tiles, MultiSpacc_Surface *Surface );

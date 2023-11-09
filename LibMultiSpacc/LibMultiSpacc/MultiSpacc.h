#ifndef _MultiSpacc_MultiSpacc_h_
#define _MultiSpacc_MultiSpacc_h_

#include <stdarg.h>
#ifndef MultiSpacc_Target_NES
	#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
#endif

#ifdef MultiSpacc_Target_Web
	#include <emscripten.h>
#endif

#ifdef MultiSpacc_Target_NDS
	#include <nds.h>
	#define Uint32 int32
	#define MultiSpacc_Surface PrintConsole
	#define MultiSpacc_Window PrintConsole
	typedef struct MultiSpacc_Event {
		int Keys;
	} MultiSpacc_Event;
#endif

#ifdef MultiSpacc_Target_NES
	#include <nes.h>
	#include "neslib.h"
	#define Uint32 int
	#define MultiSpacc_Window char
	#define MultiSpacc_Surface char
	#define MultiSpacc_Event char
	#define main( argc, argv ) main(void)
#endif

#ifdef MultiSpacc_Target_SDLCom
	#define MultiSpacc_Sleep SDL_Delay
#else
	void MultiSpacc_Sleep( int milliseconds );
#endif

#ifdef MultiSpacc_Target_SDLCom
	#define MultiSpacc_Point SDL_Point
	#define MultiSpacc_Rect SDL_Rect
#else
	typedef struct MultiSpacc_Point {
		int x, y;
	} MultiSpacc_Point;
	typedef struct MultiSpacc_Rect {
		int x, y, w, h;
	} MultiSpacc_Rect;
#endif

typedef struct MultiSpacc_SurfaceConfig {
	int width;
	int height;
	int bits;
	char palette[32];
	Uint32 flags;
} MultiSpacc_SurfaceConfig;

typedef struct MultiSpacc_SpritesMap {
	int *chr;
	int *x;
	int *y;
	int *flags;	
} MultiSpacc_SpritesMap;

MultiSpacc_Window *MultiSpacc_SetWindow( MultiSpacc_SurfaceConfig *windowConfig );
MultiSpacc_Surface *MultiSpacc_GetWindowSurface( MultiSpacc_Window *Window );

void MultiSpacc_SetAppTitle( MultiSpacc_Window *Window, const char Title[] );
void MultiSpacc_SetAppIcon( MultiSpacc_Window *Window, MultiSpacc_Surface *Icon );

bool MultiSpacc_SetMainLoop( bool function( void *args ), void *args );

MultiSpacc_Surface *MultiSpacc_LoadImage( char FilePath[], MultiSpacc_Surface *Screen, Uint32 *ColorKey );
int MultiSpacc_SetColorKey( MultiSpacc_Surface *Surface, bool Flag, Uint32 Key );

int MultiSpacc_PollEvent( MultiSpacc_Event *Event );

void MultiSpacc_PrintText( char Text[], MultiSpacc_Surface *Surface, MultiSpacc_SurfaceConfig *surfaceConfig, int x, int y, MultiSpacc_Surface *Tiles /*, int FontSize, int Color */ ); // WIP
void MultiSpacc_PrintDebug( const char *format, ... );

void MultiSpacc_SetSprite( int id, int x, int y, int sprite, MultiSpacc_Surface *tiles, MultiSpacc_Surface *screen );
void MultiSpacc_SetMetaSprite( int id, int x, int y, MultiSpacc_SpritesMap *map, int mapSize, MultiSpacc_Surface *tiles, MultiSpacc_Surface *screen );

MultiSpacc_Surface *MultiSpacc_CreateSurface( MultiSpacc_SurfaceConfig *surfaceConfig );
void MultiSpacc_BlitLayer( MultiSpacc_Surface *source, MultiSpacc_Surface *destination );

#include "./Keys.h"
#include "./VideoCycle.h"

#endif // _MultiSpacc_MultiSpacc_h_

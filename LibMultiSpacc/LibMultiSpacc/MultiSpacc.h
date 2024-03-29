#ifndef _MultiSpacc_MultiSpacc_h_
#define _MultiSpacc_MultiSpacc_h_

#ifdef __cplusplus
extern "C" {
#endif

// Amount of times per second to call the FixedUpdate function, independent from video rate and hardcoded in some places due to consoles, don't change
#define MultiSpacc_GameTick 50

#include <stdarg.h>
#ifndef MultiSpacc_Target_NES
	#include <stdbool.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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

#ifdef MultiSpacc_Target_SDLCommon
	#define MultiSpacc_Surface SDL_Surface
	#define MultiSpacc_GetTicks SDL_GetTicks
#endif

#ifdef MultiSpacc_Target_Web
	#include <emscripten.h>
#endif

#ifdef MultiSpacc_Target_Switch
	#include <switch.h>
	#include <unistd.h>
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
	#define float int
	#define double int
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

typedef struct MultiSpacc_SpriteFlags {
	bool flipHorizontal;
	bool flipVertical;
} MultiSpacc_SpriteFlags;

typedef struct {
	int *chr;
	int *x;
	int *y;
	MultiSpacc_SpriteFlags *flags;	
} MultiSpacc_SpritesMap, MultiSpacc_TilesMap;

typedef struct MultiSpacc_MainLoopHandlerArgs {
	bool (*functionFixedUpdate)( void *args );
	bool (*functionRealUpdate)( void *args, double deltaTime );
	void *args;
	Uint32 elapsedRealTime;
	Uint32 elapsedFixedTime;
} MultiSpacc_MainLoopHandlerArgs;

#if defined(MultiSpacc_Target_NES)
	#define MultiSpacc_StaticScreenWidth 256
	#define MultiSpacc_StaticScreenHeight 240
	#define MultiSpacc_StaticScreenSizeNull false
#else
	#define MultiSpacc_StaticScreenWidth 0
	#define MultiSpacc_StaticScreenHeight 0
	#define MultiSpacc_StaticScreenSizeNull true
#endif

#if defined(MultiSpacc_Target_NES)
	#define MultiSpacc_SystemClockAvailable false
#else
	#define MultiSpacc_SystemClockAvailable true
#endif

bool MultiSpacc_InitSystem(void);

MultiSpacc_Window *MultiSpacc_SetWindow( MultiSpacc_SurfaceConfig *windowConfig );
MultiSpacc_Surface *MultiSpacc_GetWindowSurface( MultiSpacc_Window *Window );

void MultiSpacc_SetAppTitle( MultiSpacc_Window *Window, const char Title[] );
void MultiSpacc_SetAppIcon( MultiSpacc_Window *Window, MultiSpacc_Surface *Icon );

bool MultiSpacc_SetMainLoop( bool functionFixedUpdate( void *args ), bool functionRealUpdate( void *args, double deltaTime ), void *args );
bool MultiSpacc_MainLoopHandler( MultiSpacc_MainLoopHandlerArgs *handlerArgs );

MultiSpacc_Surface *MultiSpacc_LoadImage( char FilePath[], MultiSpacc_Surface *Screen, Uint32 *ColorKey );
int MultiSpacc_SetColorKey( MultiSpacc_Surface *Surface, bool Flag, Uint32 Key );

int MultiSpacc_PollEvent( MultiSpacc_Event *Event );

void MultiSpacc_PrintText( char text[], MultiSpacc_Surface *Surface, MultiSpacc_SurfaceConfig *surfaceConfig, int x, int y, MultiSpacc_Surface *Tiles /*, int FontSize, int Color */ ); // WIP
void MultiSpacc_PrintDebug( const char *format, ... );

void MultiSpacc_SetSprite( int id, int x, int y, int sprite, MultiSpacc_SpriteFlags *flags, MultiSpacc_Surface *tiles, MultiSpacc_Surface *screen );
void MultiSpacc_SetMetaSprite( int id, int x, int y, MultiSpacc_SpritesMap *map, int mapSize, MultiSpacc_Surface *tiles, MultiSpacc_Surface *screen );

void MultiSpacc_SetTile( int x, int y, int tile, MultiSpacc_Surface *tiles, MultiSpacc_Surface *screen );
void MultiSpacc_SetMetaTile( int x, int y, MultiSpacc_TilesMap *map, int mapSize, MultiSpacc_Surface *tiles, MultiSpacc_Surface *screen );

MultiSpacc_Surface *MultiSpacc_CreateSurface( MultiSpacc_SurfaceConfig *surfaceConfig );
void MultiSpacc_BlitLayer( MultiSpacc_Surface *source, MultiSpacc_Surface *destination );

bool MultiSpacc_UpdateDisplay( MultiSpacc_Window *window );

#include "./Keys.h"

#ifdef __cplusplus
}
#endif

#endif // _MultiSpacc_MultiSpacc_h_

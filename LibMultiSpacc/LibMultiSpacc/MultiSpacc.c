#include "./MultiSpacc.h"

MultiSpacc_Surface *MultiSpacc_GetWindowSurface( MultiSpacc_Window *Window )
{
	#if defined(MultiSpacc_Target_SDL12)
		return Window;
	#elif defined(MultiSpacc_Target_SDL20)
		return SDL_GetWindowSurface(Window);
	#elif defined(MultiSpacc_Target_NDS)
		return Window;
	#endif
}

MultiSpacc_Surface *MultiSpacc_LoadImage( char FilePath[], MultiSpacc_Surface *Screen, Uint32 *ColorKey )
{
	#ifdef MultiSpacc_Target_SDLCom
		MultiSpacc_Surface *Final = NULL;
		MultiSpacc_Surface *Raw = IMG_Load(FilePath);
		if( Raw == NULL ) {
			MultiSpacc_PrintDebug( "[E] Error Reading Image %s.\n", FilePath );
		} else {
			Final = SDL_ConvertSurface( Raw, Screen->format, 0 );
			SDL_FreeSurface(Raw);
			if( Final == NULL ) {
				MultiSpacc_PrintDebug( "[E] Error Adapting Image %s.\n", FilePath );
			} else {
				Uint32 FinalColorKey = SDL_MapRGB( Final->format, 0xFF, 0x00, 0xFF ); // Magenta
				if( ColorKey != NULL ){
					FinalColorKey = *ColorKey;
				}
				MultiSpacc_SetColorKey( Final, true, FinalColorKey );
			};
		};
		return Final;
	#else
		return true;
	#endif
}

int MultiSpacc_SetColorKey( MultiSpacc_Surface *Surface, bool Flag, Uint32 Key )
{
	#ifdef MultiSpacc_Target_SDLCom
		Uint32 useKey;

		if(Flag)
		{
			#if defined(MultiSpacc_Target_SDL12)
				useKey = SDL_SRCCOLORKEY;
			#elif defined(MultiSpacc_Target_SDL20)
				useKey = SDL_TRUE;
			#endif
		}
		else
		{
			#if defined(MultiSpacc_Target_SDL12)
				useKey = 0;
			#elif defined(MultiSpacc_Target_SDL20)
				useKey = SDL_FALSE;
			#endif
		}

		return SDL_SetColorKey( Surface, useKey, Key );
	#endif
}

#ifndef MultiSpacc_Target_SDLCom
	void MultiSpacc_Sleep( int milliseconds )
	{
		#if defined(MultiSpacc_Target_NDS)
			#define NativeFps 60
		#elif defined(MultiSpacc_Target_NES)
			#define NativeFps 50
		#endif

		int frames = (NativeFps * milliseconds / 1000);
		while( --frames )
		{
			#if defined(MultiSpacc_Target_NDS)
				swiWaitForVBlank();
			#elif defined(MultiSpacc_Target_NES)
				ppu_wait_frame();
			#endif
		};
	}
#endif

#ifdef MultiSpacc_Target_SDLCom
	int MultiSpacc_PollEvent( MultiSpacc_Event *Event )
	{
		SDL_Event FromEvent;
		int Result = SDL_PollEvent( &FromEvent );
		*Event = (MultiSpacc_Event){
			.Type = FromEvent.type,
			.Key = FromEvent.key.keysym.sym,
		};
		return Result;
	}
#endif

void MultiSpacc_SetSprite( int id, int x, int y, int sprite, MultiSpacc_SpriteFlags *flags, MultiSpacc_Surface *tiles, MultiSpacc_Surface *screen )
{
	#if defined(MultiSpacc_Target_SDLCommon)
		MultiSpacc_Rect offset = { .x = x, .y = y, };
		MultiSpacc_Rect clip = {
			.x = (8 * (sprite % 16)),
			.y = (8 * (sprite / 16)),
			.w = 8,
			.h = 8,
		};
		SDL_BlitSurface( tiles, &clip, screen, &offset );

	#elif defined(MultiSpacc_Target_NES)
		oam_spr( x, y, sprite, (flags == NULL ? 0 : 0|(flags->flipHorizontal ? OAM_FLIP_H : 0)|(flags->flipVertical ? OAM_FLIP_V : 0)), id*4 );

	#endif
}

void MultiSpacc_SetMetaSprite( int id, int x, int y, MultiSpacc_SpritesMap *map, int mapSize, MultiSpacc_Surface *tiles, MultiSpacc_Surface *screen )
{
	int i;
	for(i=0; i<mapSize; i++)
	{
		MultiSpacc_SetSprite( (id + i), (x + map->x[i]), (y + map->y[i]), map->chr[i], &map->flags[i], tiles, screen );
	}
}

void MultiSpacc_SetTile( int x, int y, int tile, MultiSpacc_Surface *tiles, MultiSpacc_Surface *screen )
{
	#if defined(MultiSpacc_Target_SDLCommon)
		MultiSpacc_Rect offset = {
			.x = 8*x,
			.y = 8*y,
		};
		MultiSpacc_Rect clip = {
			.x = (8 * (tile % 16)),
			.y = (8 * (tile / 16)),
			.w = 8,
			.h = 8,
		};
		SDL_BlitSurface( tiles, &clip, screen, &offset );

	#elif defined(MultiSpacc_Target_NES)
		ppu_off();
		vram_adr(NTADR_A( x, y ));
		vram_put(tile);
		ppu_on_all();

	#endif
}

void MultiSpacc_SetMetaTile( int x, int y, MultiSpacc_TilesMap *map, int mapSize, MultiSpacc_Surface *tiles, MultiSpacc_Surface *screen )
{
	int i;

	#if defined(MultiSpacc_Target_NES)
		ppu_off();
	#endif

	for(i=0; i<mapSize; i++)
	{
		#if defined(MultiSpacc_Target_NES)
			vram_adr(NTADR_A( (x + map->x[i]), (y + map->y[i]) ));
			vram_put( map->chr[i] );
		#else
			MultiSpacc_SetTile( (x + map->x[i]), (y + map->y[i]), map->chr[i], tiles, screen );
		#endif
	}

	#if defined(MultiSpacc_Target_NES)
		ppu_on_all();
	#endif
}

void MultiSpacc_BlitLayer( MultiSpacc_Surface *source, MultiSpacc_Surface *destination )
{
	#if defined(MultiSpacc_Target_SDLCommon)
		SDL_BlitSurface( source, NULL, destination, NULL );
	#endif
}

MultiSpacc_Surface *MultiSpacc_CreateSurface( MultiSpacc_SurfaceConfig *surfaceConfig )
{
	#if defined(MultiSpacc_Target_SDLCommon)
		return SDL_CreateRGBSurface( 0, surfaceConfig->width, surfaceConfig->height, surfaceConfig->bits, 0, 0, 0, 0 );
	#endif
}

// partially copied from Unity's implementation, see <https://docs.unity3d.com/Manual/TimeFrameManagement.html>
bool MultiSpacc_MainLoopHandler( MultiSpacc_MainLoopHandlerArgs *handlerArgs )
{
	#define AssertDirectCallUpdates ( ( handlerArgs->functionFixedUpdate != NULL && !handlerArgs->functionFixedUpdate(handlerArgs->args) ) || ( handlerArgs->functionRealUpdate != NULL && !handlerArgs->functionRealUpdate( handlerArgs->args, 1 ) ) )

	#if defined(MultiSpacc_Target_SDLCommon)
		Uint32 ticksNow;
		Uint32 realDeltaTime;
		double fixedDeltaTime;

		ticksNow = SDL_GetTicks();
		realDeltaTime = (ticksNow - handlerArgs->elapsedRealTime);
		handlerArgs->elapsedRealTime += realDeltaTime;
		fixedDeltaTime = (double)(handlerArgs->elapsedRealTime - handlerArgs->elapsedFixedTime) / (double)(1000 / MultiSpacc_GameTick);

		while ( (handlerArgs->elapsedRealTime - handlerArgs->elapsedFixedTime) > (1000 / MultiSpacc_GameTick) ) {
			if( handlerArgs->functionFixedUpdate != NULL && !handlerArgs->functionFixedUpdate(handlerArgs->args) ){
				return false;
			}
			handlerArgs->elapsedFixedTime += (1000 / MultiSpacc_GameTick);
		}

		// TODO: limit real updates on native platforms, otherwise we waste infinite CPU cycles
		if( handlerArgs->functionRealUpdate != NULL && !handlerArgs->functionRealUpdate( handlerArgs->args, fixedDeltaTime ) ){
			return false;
		}

	#elif defined(MultiSpacc_Target_NDS)
		// TODO: limit FixedUpdate to 50 FPS, since NDS vblank is 60 Hz
		swiWaitForVBlank();
		if( AssertDirectCallUpdates ){
			return false;
		}

	#elif defined(MultiSpacc_Target_NES)
		ppu_wait_frame();
		if( AssertDirectCallUpdates ){
			return false;
		}

	#endif
	return true;
}

bool MultiSpacc_UpdateDisplay( MultiSpacc_Window *window )
{
	#if defined(MultiSpacc_Target_SDL12)
		return !SDL_Flip(window);
	#elif defined(MultiSpacc_Target_SDL20)
		return !SDL_UpdateWindowSurface(window);
	#else
		return true;
	#endif
}

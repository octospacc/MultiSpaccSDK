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
		/* TODO: 50 or 60 FPS based on region for appropriate consoles */
		int frames = (60 * milliseconds / 1000);
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

void MultiSpacc_SetSprite( int id, int x, int y, int sprite, MultiSpacc_Surface *tiles, MultiSpacc_Surface *screen )
{
	#if defined(MultiSpacc_Target_SDLCom)
		MultiSpacc_Rect Offset = { .x = x, .y = y, };
		MultiSpacc_Rect Clip = {
			.x = (8 * (sprite % 16)),
			.y = (8 * (sprite / 16)),
			.w = 8,
			.h = 8,
		};
		SDL_BlitSurface( tiles, &Clip, screen, &Offset );
	#elif defined(MultiSpacc_Target_NES)
		oam_spr( x, y, sprite, 0, id*4 );
	#endif
}

void MultiSpacc_SetMetaSprite( int id, int x, int y, MultiSpacc_SpritesMap *map, int mapSize, MultiSpacc_Surface *tiles, MultiSpacc_Surface *screen )
{
	int i;

	for(i=0; i<mapSize; i++)
	{
		MultiSpacc_SetSprite( (id + i), (x + map->x[i]), (y + map->y[i]), map->chr[i], tiles, screen );
	}
}

void MultiSpacc_BlitLayer( MultiSpacc_Surface *source, MultiSpacc_Surface *destination )
{
	#if defined(MultiSpacc_Target_SDLCom)
		SDL_BlitSurface( source, NULL, destination, NULL );
	#endif
}

MultiSpacc_Surface *MultiSpacc_CreateSurface( MultiSpacc_SurfaceConfig *surfaceConfig )
{
	#if defined(MultiSpacc_Target_SDLCom)
		return SDL_CreateRGBSurface( 0, surfaceConfig->width, surfaceConfig->height, surfaceConfig->bits, 0, 0, 0, 0 );
	#endif
}

// partially copied from Unity's implementation, see <https://docs.unity3d.com/Manual/TimeFrameManagement.html>
bool MultiSpacc_MainLoopHandler( MultiSpacc_MainLoopHandlerArgs *handlerArgs )
{
	#if defined(MultiSpacc_Target_SDLStandard)
		//if( !handlerArgs->functionFixedUpdate(handlerArgs->args) || !handlerArgs->functionUpdate( handlerArgs->args, *handlerArgs->nextTick ) ){
		//	return false;
		//}

		Uint32 ticksNow;
		Uint32 deltaTime;
		//int cyclesSinceLast;

		//MultiSpacc_WaitFrame(handlerArgs->nextTick);
		ticksNow = SDL_GetTicks();
		deltaTime = (ticksNow - handlerArgs->elapsedRealTime);
		handlerArgs->elapsedRealTime += deltaTime;
		//cyclesSinceLast = deltaTime / (1000 / MultiSpacc_GameTick);

		while ( handlerArgs->elapsedRealTime - handlerArgs->elapsedFixedTime > (1000 / MultiSpacc_GameTick) ) {
		//for( int i=0; i<cyclesSinceLast; i++ ){
		//if( handlerArgs->elapsedTime - handlerArgs->elapsedFixedTime > (1000 / MultiSpacc_GameTick) ){
			if( !handlerArgs->functionFixedUpdate(handlerArgs->args) ){
				return false;
			}
			//handlerArgs->ticksLast = SDL_GetTicks();
			handlerArgs->elapsedFixedTime += (1000 / MultiSpacc_GameTick);
		}

		if( !handlerArgs->functionRealUpdate( handlerArgs->args, (double)(handlerArgs->elapsedRealTime - handlerArgs->elapsedFixedTime)/((double)1000 / MultiSpacc_GameTick) ) ){
			return false;
		}
	#elif defined(MultiSpacc_Target_NES)
		ppu_wait_frame();
		if( !handlerArgs->functionFixedUpdate(handlerArgs->args) || !handlerArgs->functionRealUpdate( handlerArgs->args, 1 ) ){
			
		}
	#endif

	return true;
}

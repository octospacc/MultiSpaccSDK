#include "./MultiSpacc.h"

bool MultiSpacc_SetMainLoop( bool function( void *args ), void *args )
{
	#ifdef MultiSpacc_Target_Web
		emscripten_set_main_loop_arg( function, args, -1, true );
	#else
		while(true){
			if( !function(args) ){
				return false;
			}
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
	#endif
	return true;
}

bool MultiSpacc_WaitFrame( Uint32 *nextTick )
{
	#if defined(MultiSpacc_Target_SDLStandard)
		// TODO: check if this actually works with any framerate or it caps the game speed
		Uint32 now;
		now = SDL_GetTicks();
		if ( *nextTick <= now ) {
			MultiSpacc_Sleep(0);
		} else {
			MultiSpacc_Sleep( *nextTick - now );
		}
		*nextTick += 1000/60; // TODO: specified framerate and variable framerate
		*nextTick = SDL_GetTicks() + 1000/60; // this last part should execute at the start of a loop maybe ???
	#elif defined(MultiSpacc_Target_NES)
		ppu_wait_frame();
	#endif
	return true;
}

bool MultiSpacc_WaitUpdateDisplay( MultiSpacc_Window *window, Uint32 *nextTick )
{
	if( !MultiSpacc_UpdateDisplay(window) || !MultiSpacc_WaitFrame(nextTick) ){
		return false;
	}
	return true;
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

	MultiSpacc_Surface *MultiSpacc_LoadImage( char FilePath[], MultiSpacc_Surface *Screen, Uint32 *ColorKey )
	{
		MultiSpacc_Surface *Final = NULL;
		MultiSpacc_Surface *Raw = IMG_Load( FilePath );
		if( Raw == NULL ) {
			MultiSpacc_PrintDebug("[E] Error reading image %s.\n", FilePath);
		} else {
			Final = SDL_ConvertSurface( Raw, Screen->format, 0 );
			SDL_FreeSurface( Raw );
			if( Final == NULL ) {
				MultiSpacc_PrintDebug("[E] Error adapting image %s.\n", FilePath);
			} else {
				Uint32 FinalColorKey = SDL_MapRGB( Final->format, 0xFF, 0x00, 0xFF ); // Magenta
				if( ColorKey != NULL )
					FinalColorKey = *ColorKey;
				MultiSpacc_SetColorKey( Final, true, FinalColorKey );
			};
		};
		return Final;
	}
#endif

#ifdef MultiSpacc_Target_NDS
	int MultiSpacc_UpdateWindowSurface( MultiSpacc_Window *Window )
	{
		return 0;
	}

	void MultiSpacc_Sleep( int milliseconds )
	{
		int frames = (60 * milliseconds / 1000);
		for (int i=0; i<frames; i++)
		{
			swiWaitForVBlank();
		};
	}
	
	MultiSpacc_Surface *MultiSpacc_LoadImage( char FilePath[], MultiSpacc_Surface *Screen, Uint32 *ColorKey )
	{
		return NULL;
	};
#endif

void MultiSpacc_Sprite( int id, int x, int y, int sprite, MultiSpacc_Surface *Tiles, MultiSpacc_Surface *Surface )
{
	#ifdef MultiSpacc_Target_SDLCom
		MultiSpacc_Rect Offset = { .x = x, .y = y, };
		MultiSpacc_Rect Clip = {
			.x = (8 * (sprite % 16)),
			.y = (8 * (sprite / 16)),
			.w = 8,
			.h = 8,
		};
		SDL_BlitSurface( Tiles, &Clip, Surface, &Offset );
	#endif
	
	#ifdef MultiSpacc_Target_NES
		oam_spr(x, y, sprite, 0, id);
	#endif
}

#ifdef MultiSpacc_Target_NES
	int MultiSpacc_UpdateWindowSurface( MultiSpacc_Window *Window )
	{
		ppu_wait_frame();
		return 0;
	}

	void MultiSpacc_Sleep( int milliseconds ){}

	MultiSpacc_Surface *MultiSpacc_LoadImage( char FilePath[], MultiSpacc_Surface *Screen, Uint32 *ColorKey )
	{
		return NULL;
	};
#endif

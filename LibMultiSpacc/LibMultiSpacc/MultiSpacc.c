#include "./MultiSpacc.h"

#ifdef MultiSpacc_Target_SDL12
	int MultiSpacc_SetColorKey( MultiSpacc_Surface *Surface, bool Flag, Uint32 Key ) {
		if( Flag )
			return SDL_SetColorKey( Surface, SDL_SRCCOLORKEY, Key );
		else
			return SDL_SetColorKey( Surface, 0, Key );
	}
#endif

#ifdef MultiSpacc_Target_SDL20
	int MultiSpacc_SetColorKey( MultiSpacc_Surface *Surface, bool Flag, Uint32 Key ) {
		if ( Flag ) {
			return SDL_SetColorKey( Surface, SDL_TRUE, Key );
		} else {
			return SDL_SetColorKey( Surface, SDL_FALSE, Key );
		}
	}
#endif

#ifdef MultiSpacc_Target_SDLCom
	int MultiSpacc_PollEvent( MultiSpacc_Event *Event )
	{
		SDL_Event FromEvent;
		int Result = SDL_PollEvent( &FromEvent );
		*Event = (MultiSpacc_Event) {
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
#endif

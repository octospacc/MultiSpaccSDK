#include "./MultiSpacc.h"

MultiSpacc_Surface *MultiSpacc_LoadImage( char FilePath[], MultiSpacc_Surface *Screen, Uint32 *ColorKey )
{
	#ifdef MultiSpacc_Target_SDLCom
		MultiSpacc_Surface *Final = NULL;
		MultiSpacc_Surface *Raw = IMG_Load( FilePath );
		if( Raw == NULL ) {
			MultiSpacc_PrintDebug("[E] Error Reading Image %s.\n", FilePath);
		} else {
			Final = SDL_ConvertSurface( Raw, Screen->format, 0 );
			SDL_FreeSurface( Raw );
			if( Final == NULL ) {
				MultiSpacc_PrintDebug("[E] Error Adapting Image %s.\n", FilePath);
			} else {
				Uint32 FinalColorKey = SDL_MapRGB( Final->format, 0xFF, 0x00, 0xFF ); // Magenta
				if( ColorKey != NULL )
					FinalColorKey = *ColorKey;
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

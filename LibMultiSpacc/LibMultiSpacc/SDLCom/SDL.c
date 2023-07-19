#include "../MultiSpacc.h"

int MultiSpacc_PollEvent( MultiSpacc_Event *Event )
{
	SDL_Event FromEvent;
	int Result = SDL_PollEvent( &FromEvent );
	*Event = (MultiSpacc_Event) {
		.Type = FromEvent.type,
		.Key = FromEvent.key.keysym.sym,
	};
	return Result;
};

MultiSpacc_Surface *MultiSpacc_LoadImage( char FilePath[], MultiSpacc_Surface *Screen, Uint32 *ColorKey )
{
	MultiSpacc_Surface *Final = NULL;
	MultiSpacc_Surface *Raw = IMG_Load( FilePath );
	if( Raw == NULL ) {
		printf("[E] Error reading image %s.\n", FilePath);
	} else {
		Final = SDL_ConvertSurface( Raw, Screen->format, 0 );
		SDL_FreeSurface( Raw );
		if( Final == NULL ) {
			printf("[E] Error adapting image %s.\n", FilePath);
		} else {
			Uint32 FinalColorKey = SDL_MapRGB( Final->format, 0xFF, 0x00, 0xFF ); // Magenta
			if( ColorKey != NULL )
				FinalColorKey = *ColorKey;
			MultiSpacc_SetColorKey( Final, true, FinalColorKey );
		};
	};
	return Final;
};

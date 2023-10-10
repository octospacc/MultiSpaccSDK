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
}

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
}

void MultiSpacc_PrintText( char Text[], MultiSpacc_Surface *Surface, MultiSpacc_SurfaceConfig WindowConfig, int x, int y, MultiSpacc_Surface *Tiles /*, int FontSize, int Color */ )
{
	for( int i = 0; i < strlen( Text ); i++ )
	{
		MultiSpacc_Rect Offset = {
			.x = (x * 8) + (8 * i),
			.y = (y * 8),
		};
		MultiSpacc_Rect Clip = {
			.x = 8 * (Text[i] % 16),
			.y = 8 * (Text[i] / 16),
			.w = 8,
			.h = 8,
		};
		SDL_BlitSurface( Tiles, &Clip, Surface, &Offset );
	};
}

void MultiSpacc_PrintDebug( const char *format, ... )
{
	va_list args;
	va_start(args, format);
	fprintf(stderr, format, args);
	va_end(args);
}
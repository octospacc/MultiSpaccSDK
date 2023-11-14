#include "./MultiSpacc.h"

void MultiSpacc_PrintText( char text[], MultiSpacc_Surface *Surface, MultiSpacc_SurfaceConfig *surfaceConfig, int x, int y, MultiSpacc_Surface *Tiles /*, int FontSize, int Color */ )
{
	#if defined(MultiSpacc_Target_SDLCommon)
		/* TODO: not just 8x8 tiles, and account for surface dimensions */
		for( int i = 0; i < strlen(text); i++ )
		{
			MultiSpacc_Rect offset = {
				.x = 8*x + 8*i,
				.y = 8*y,
			};
			MultiSpacc_Rect clip = {
				.x = 8 * (text[i] % 16),
				.y = 8 * (text[i] / 16),
				.w = 8,
				.h = 8,
			};
			SDL_BlitSurface( Tiles, &clip, Surface, &offset );
		};

	#elif defined(MultiSpacc_Target_NDS)
		iprintf( "\x1b[%d;%dH%s", y, x, text );

	#elif defined(MultiSpacc_Target_NES)
		// NOTE: is there no alternative to ppu off and on there? it makes the screen flicker and so makes programming more difficult
		ppu_off();
		vram_adr(NTADR_A( x, y ));
		vram_write( text, strlen(text) );
		ppu_on_all();

	#endif
}

void MultiSpacc_PrintDebug( const char *format, ... )
{
	#ifdef MultiSpacc_Target_SDLCom
		va_list args;
		va_start( args, format );
		SDL_Log( format, args );
		va_end(args);
	#endif
}

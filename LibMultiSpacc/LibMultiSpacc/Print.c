#include "./MultiSpacc.h"

void MultiSpacc_PrintText( char Text[], MultiSpacc_Surface *Surface, MultiSpacc_SurfaceConfig *surfaceConfig, int x, int y, MultiSpacc_Surface *Tiles /*, int FontSize, int Color */ )
{
	#ifdef MultiSpacc_Target_SDLCom
		/* TODO: not just 8x8 tiles, and account for surface dimensions */
		for( int i = 0; i < strlen(Text); i++ )
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
	#endif

	#ifdef MultiSpacc_Target_NDS
		iprintf( "%s", Text );
	#endif

	#ifdef MultiSpacc_Target_NES
		ppu_off();
		vram_adr(NTADR_A( x, y ));
		vram_write( Text, strlen(Text) );
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

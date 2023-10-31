#include "./MultiSpacc.h"

void MultiSpacc_PrintText( char Text[], MultiSpacc_Surface *Surface, MultiSpacc_SurfaceConfig WindowConfig, int x, int y, MultiSpacc_Surface *Tiles /*, int FontSize, int Color */ )
{
	#ifdef MultiSpacc_Target_SDLCom
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
		iprintf("%s", Text);
	#endif
}

void MultiSpacc_PrintDebug( const char *format, ... )
{
	#ifdef MultiSpacc_Target_SDLCom
		va_list args;
		va_start(args, format);
		fprintf(stderr, format, args);
		va_end(args);
	#endif
}

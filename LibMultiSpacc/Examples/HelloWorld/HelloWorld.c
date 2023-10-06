#include "../../LibMultiSpacc/MultiSpacc.h"

#define AppName "Hello World"
#define ScreenWidth 320
#define ScreenHeight 240
#define ScreenBits 16

int main( int argc, char *args[] )
{
	MultiSpacc_Window *Window = MultiSpacc_SetWindow( ScreenWidth, ScreenHeight, ScreenBits, 0 );
	MultiSpacc_Surface *Screen = MultiSpacc_GetWindowSurface( Window );

	if( Screen == NULL )
	{
		printf("[E] Error initializing Video System.\n");
		return -1;
	};

	MultiSpacc_SetAppTitle( Window, AppName );

	// Bitmap font borrowed from: <https://github.com/nesdoug/01_Hello/blob/master/Alpha.chr>
	// Copyright (c) 2018 Doug Fraker www.nesdoug.com (MIT)
	MultiSpacc_Surface *TilesImg = MultiSpacc_LoadImage( "Tiles.png", Screen, NULL );

	/*char Text[] = "Hello, World!";
	for(int i = 0; i < sizeof( Text ); i++){
		MultiSpacc_Rect Offset = {
			.x = (8 * i) + (ScreenWidth / sizeof( Text )),
			.y = ScreenHeight / 3,
		};
		MultiSpacc_Rect Clip = {
			.x = 8 * (Text[i] % 16),
			.y = 8 * (Text[i] / 16),
			.w = 8,
			.h = 8,
		};
		SDL_BlitSurface( TilesImg, &Clip, Screen, &Offset );
	};*/
	MultiSpacc_PrintString( "Hello, World!", Screen, ScreenWidth, ScreenHeight, 4, 4, TilesImg );

	if( MultiSpacc_UpdateWindowSurface( Window ) != 0 )
	{
		printf("[E] Error updating Screen.\n");
		return -1;
	};

	MultiSpacc_Sleep( 3000 );
	return 0;
};

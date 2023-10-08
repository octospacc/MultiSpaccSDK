#include "../../LibMultiSpacc/MultiSpacc.h"

#define AppName "Hello World"

int main( int argc, char *args[] )
{
	MultiSpacc_SurfaceConfig WindowConfig = { .Width = 320, .Height = 240, .Bits = 16 };
	MultiSpacc_Window *Window = MultiSpacc_SetWindow( WindowConfig );
	MultiSpacc_Surface *Screen = MultiSpacc_GetWindowSurface( Window );

	if( Screen == NULL )
	{
		printf("[E] Error Initializing Video System.\n");
		return -1;
	};

	MultiSpacc_SetAppTitle( Window, AppName );

	// Bitmap font borrowed from: <https://github.com/nesdoug/01_Hello/blob/master/Alpha.chr>
	// Copyright (c) 2018 Doug Fraker www.nesdoug.com (MIT)
	//MultiSpacc_Surface *TilesImg = MultiSpacc_LoadImage( "Tiles.png", Screen, NULL );

	//MultiSpacc_PrintText( "Hello, World!", Screen, WindowConfig.Width, WindowConfig.Height, 4, 4, TilesImg );

	if( MultiSpacc_UpdateWindowSurface( Window ) != 0 )
	{
		printf("[E] Error Updating Screen.\n");
		return -1;
	};

	MultiSpacc_Sleep( 3000 );
	return 0;
}

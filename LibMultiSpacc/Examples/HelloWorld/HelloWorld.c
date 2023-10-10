#include "../../LibMultiSpacc/MultiSpacc.h"

#define AppName "Hello World"

//MultiSpacc_Window consoleInnit(MultiSpacc_Window console){
//	consoleSelect(&console);
//	PrintConsole *consolePtr = consoleInit(&console, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
//	consoleSelect(&console);
//	consoleSelect(consolePtr);
//	return *consolePtr;
//return *consoleInit(&Window, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
//}

int main( void )
{
	MultiSpacc_SurfaceConfig WindowConfig = { .Width = 320, .Height = 240, .Bits = 16 };
	MultiSpacc_Window Window = MultiSpacc_SetWindow( WindowConfig );
	MultiSpacc_Surface *Screen = MultiSpacc_GetWindowSurface( &Window );

/*	if( Screen == NULL )
	{
		printf("[E] Error Initializing Video System.\n");
		return -1;
	};*/

	//MultiSpacc_SetAppTitle( Window, AppName );

	//PrintConsole bottomScreen;
	//consoleInit(&Window, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	//Window = consoleInnit(Window);
	//consoleSelect(&Window);

	// Bitmap font borrowed from: <https://github.com/nesdoug/01_Hello/blob/master/Alpha.chr>
	// Copyright (c) 2018 Doug Fraker www.nesdoug.com (MIT)
	//MultiSpacc_Surface *TilesImg = MultiSpacc_LoadImage( "Tiles.png", Screen, NULL );
	MultiSpacc_PrintText( "Hello, World!", NULL, WindowConfig, 4, 4, NULL );

	while(1){ swiWaitForVBlank(); }

	/*if( MultiSpacc_UpdateWindowSurface( Window ) != 0 )
	{
		printf("[E] Error Updating Screen.\n");
		return -1;
	};*/

	MultiSpacc_Sleep( 3000 );
	return 0;
}

#include "../../LibMultiSpacc/MultiSpacc.h"

#define AppName "Hello World"

/*{pal:"nes",layout:"nes"}*/
const char PALETTE[32] = { 
	0x03,                // screen
	0x11,0x30,0x27,0x00, // background 0
	0x1c,0x20,0x2c,0x00, // background 1
	0x00,0x10,0x20,0x00, // background 2
	0x06,0x16,0x26,0x00, // background 3
	0x16,0x35,0x24,0x00, // sprite 0
	0x00,0x37,0x25,0x00, // sprite 1
	0x0d,0x2d,0x3a,0x00, // sprite 2
	0x0d,0x27,0x2a,      // sprite 3
};

int main( int argc, char *argv[] )
{
	int spriteX = 0;
	int spriteY = 0;
	int accelX = +1;
	int accelY = +2;

	MultiSpacc_SurfaceConfig WindowConfig = {0};
	MultiSpacc_Window *Window;
	MultiSpacc_Surface *Screen;
	MultiSpacc_Surface *TilesImg;

	WindowConfig.Width = 320;
	WindowConfig.Height = 240;
	WindowConfig.Bits = 16;
	memcpy( WindowConfig.Palette, PALETTE, 32 );
	//WindowConfig.Frequency = 50;
	Window = MultiSpacc_SetWindow( &WindowConfig /*, &PALETTE*/ );
	Screen = MultiSpacc_GetWindowSurface( Window );

	if( Screen == NULL )
	{
		MultiSpacc_PrintDebug("[E] Error Initializing Video System.\n");
		return -1;
	};

	MultiSpacc_SetAppTitle( Window, AppName );

	// Bitmap font borrowed from: <https://github.com/nesdoug/01_Hello/blob/master/Alpha.chr>
	// Copyright (c) 2018 Doug Fraker www.nesdoug.com (MIT)
	TilesImg = MultiSpacc_LoadImage( "CHARS.png", Screen, NULL );
	MultiSpacc_PrintText( "Hello, World!", Screen, &WindowConfig, 2, 2, TilesImg );
	MultiSpacc_PrintDebug("[I] Ready!\n");

	while(true)
	{
		MultiSpacc_Sprite( 0, spriteX, spriteY, 1, TilesImg, Screen );
		//scroll(spriteX,0);

		spriteX += accelX;
		spriteY += accelY;

		if( spriteX >= WindowConfig.Width )
		{
			spriteX = 0;
		}
	
		if( spriteY == 0 || spriteY == (WindowConfig.Height - 8) )
		{
			accelY *= -1;
		}
	
		if( MultiSpacc_UpdateWindowSurface(Window) != 0 )
		{
			MultiSpacc_PrintDebug("[E] Error Updating Screen.\n");
			return -1;
		};
		
		// TODO: Implement cross-platform vblank-wait
		MultiSpacc_Sleep(16);
	}

	return 0;
}

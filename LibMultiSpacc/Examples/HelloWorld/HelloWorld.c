#include "../../LibMultiSpacc/MultiSpacc.h"

#define AppName "Hello World"

typedef struct MainArgs {
	int spriteX;
	int spriteY;
	int accelX;
	int accelY;
	MultiSpacc_SurfaceConfig *WindowConfig;
	MultiSpacc_Window *Window;
	MultiSpacc_Surface *Screen;
	MultiSpacc_Surface *Background;
	MultiSpacc_Surface *Foreground;
	MultiSpacc_Surface *TilesImg;
} MainArgs;

/*{pal:"nes",layout:"nes"}*/
const char palette[32] = { 
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

Uint32 nextTick;

bool MainLoop( void *args )
{
	MainArgs *margs = (MainArgs*)args;

	MultiSpacc_Sprite( 0, margs->spriteX, margs->spriteY, 1, margs->TilesImg, margs->Screen );
	// ... this must go on Foreground
	//scroll(spriteX,0);

	margs->spriteX += margs->accelX;
	margs->spriteY += margs->accelY;

	if( margs->spriteX >= margs->WindowConfig->Width )
	{
		margs->spriteX = 0;
	}
	if( margs->spriteY == 0 || margs->spriteY == ( margs->WindowConfig->Height - 8 ) )
	{
		margs->accelY *= -1;
	}

	/* TODO: listen for OS terminate signal */
	if( MultiSpacc_CheckKey( MultiSpacc_Key_Pause, 0 ) )
	{
		return false;
	}

	if( !MultiSpacc_WaitUpdateDisplay( margs->Window, &nextTick ) )
	{
		MultiSpacc_PrintDebug("[E] Error Updating Screen.\n");
		return false;
	}

	// apply Background and then Foreground on Screen
	// ...

	return true;
}

int main( int argc, char *argv[] )
{
	MainArgs margs = {0};
	MultiSpacc_SurfaceConfig WindowConfig = {0};

	margs.WindowConfig = &WindowConfig;
	margs.accelX = +1;
	margs.accelY = +2;

	WindowConfig.Width = 320;
	WindowConfig.Height = 240;
	WindowConfig.Bits = 16;
	memcpy( WindowConfig.Palette, palette, 32 );
	//WindowConfig.Frequency = 50;

	margs.Window = MultiSpacc_SetWindow( &WindowConfig );
	margs.Screen = MultiSpacc_GetWindowSurface( margs.Window );
	if( margs.Screen == NULL )
	{
		MultiSpacc_PrintDebug("[E] Error Initializing Video System.\n");
		return -1;
	};

	MultiSpacc_SetAppTitle( margs.Window, AppName );
	MultiSpacc_PrintDebug("[I] Ready!\n");

	// Bitmap font borrowed from: <https://github.com/nesdoug/01_Hello/blob/master/Alpha.chr>
	// Copyright (c) 2018 Doug Fraker www.nesdoug.com (MIT)
	margs.TilesImg = MultiSpacc_LoadImage( "CHARS.png", margs.Screen, NULL );
	if( margs.TilesImg == NULL )
	{
		return -1;
	}

	MultiSpacc_PrintText( "Hello, World!", margs.Screen, &WindowConfig, 2, 2, margs.TilesImg );
	// ... this must print on Background

	return MultiSpacc_SetMainLoop( MainLoop, &margs );
}

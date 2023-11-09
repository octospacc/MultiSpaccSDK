#include "../../LibMultiSpacc/MultiSpacc.h"

#define AppName "Hello World"

typedef struct MainArgs {
	int spriteX;
	int spriteY;
	int accelX;
	int accelY;
	MultiSpacc_SurfaceConfig *windowConfig;
	MultiSpacc_Window *window;
	MultiSpacc_Surface *screen;
	MultiSpacc_Surface *background;
	//MultiSpacc_Surface *Foreground;
	MultiSpacc_Surface *tilesImg;
} MainArgs;

/*{pal:"nes",layout:"nes"}*/
const char palette[32] = { 
	0x0F,                // screen
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

	//SDL_FillRect(margs->Background, &rect, 0x000000);
	MultiSpacc_BlitLayer( margs->background, margs->screen );
	//SDL_BlitSurface( margs->Foreground, &rect, margs->Screen, &rect );
	MultiSpacc_SetSprite( 0, margs->spriteX, margs->spriteY, 1, margs->tilesImg, margs->screen );
	//scroll(spriteX,0);

	margs->spriteX += margs->accelX;
	margs->spriteY += margs->accelY;

	if( margs->spriteX >= margs->windowConfig->width )
	{
		margs->spriteX = 0;
	}
	if( margs->spriteY == 0 || margs->spriteY == ( margs->windowConfig->height - 8 ) )
	{
		margs->accelY *= -1;
	}

	/* TODO: listen for OS terminate signal */
	if( MultiSpacc_CheckKey( MultiSpacc_Key_Pause, 0 ) )
	{
		return false;
	}

	if( !MultiSpacc_WaitUpdateDisplay( margs->window, &nextTick ) )
	{
		MultiSpacc_PrintDebug("[E] Error Updating Screen.\n");
		return false;
	}

	return true;
}

int main( int argc, char *argv[] )
{
	MainArgs margs = {0};
	MultiSpacc_SurfaceConfig windowConfig = {0};

	margs.windowConfig = &windowConfig;
	margs.accelX = +2;
	margs.accelY = +2;

	windowConfig.width = 320;
	windowConfig.height = 240;
	windowConfig.bits = 16;
	memcpy( windowConfig.palette, palette, 32 );

	margs.window = MultiSpacc_SetWindow( &windowConfig );
	margs.screen = MultiSpacc_GetWindowSurface( margs.window );
	margs.background = MultiSpacc_CreateSurface( &windowConfig );
	//margs.Foreground = MultiSpacc_CreateSurface( &windowConfig );
	if( margs.screen == NULL || margs.background == NULL /*|| margs.Foreground == NULL*/ )
	{
		MultiSpacc_PrintDebug("[E] Error Initializing Video System.\n");
		return -1;
	};

	MultiSpacc_SetAppTitle( margs.window, AppName );
	MultiSpacc_PrintDebug("[I] Ready!\n");

	// Bitmap font forked from: <https://github.com/nesdoug/01_Hello/blob/master/Alpha.chr>
	// Original copyright (c) 2018 Doug Fraker www.nesdoug.com (MIT)
	margs.tilesImg = MultiSpacc_LoadImage( "../CHARS.png", margs.screen, NULL );
	if( margs.tilesImg == NULL )
	{
		return -1;
	}

	MultiSpacc_PrintText( "Hello, World!", margs.background, &windowConfig, 2, 2, margs.tilesImg );

	return MultiSpacc_SetMainLoop( MainLoop, &margs );
}

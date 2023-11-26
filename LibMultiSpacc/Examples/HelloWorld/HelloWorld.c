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
	MultiSpacc_KeysStates *buttonsStates;
} MainArgs;

const char palette[32] = { 
	0x0F,                // screen
	0x11,0x02,0x02,0x00, // background 0
	0x00,0x00,0x00,0x00, // background 1
	0x00,0x00,0x00,0x00, // background 2
	0x00,0x00,0x00,0x00, // background 3
	0x28,0x00,0x00,0x00, // sprite 0
	0x00,0x00,0x00,0x00, // sprite 1
	0x00,0x00,0x00,0x00, // sprite 2
	0x00,0x00,0x00,      // sprite 3
};

bool MainLoop( void *args )
{
	MainArgs *margs = (MainArgs*)args;

	//SDL_FillRect(margs->background, NULL, 0x00F);
	MultiSpacc_BlitLayer( margs->background, margs->screen );
	//SDL_BlitSurface( margs->Foreground, &rect, margs->Screen, &rect );
	MultiSpacc_SetSprite( 0, margs->spriteX, margs->spriteY, 1, NULL, margs->tilesImg, margs->screen );
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

	MultiSpacc_PollButtons( 0, margs->buttonsStates );

	/* TODO: listen for OS terminate signal */
	if( MultiSpacc_CheckKeyPress( MultiSpacc_Key_Pause, margs->buttonsStates ) )
	{
		return false;
	}

// for( int i=0; i< margs->screen->w * margs->screen->h * margs->screen->format->BytesPerPixel; i++ )
// {
	// NOTE: format is BGRA for 32bits
	// SDL_memset( (char*)margs->screen->pixels + i, 0xFF, 1 );
// }

	if( !MultiSpacc_UpdateDisplay( margs->window ) )
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
	MultiSpacc_KeysStates buttonsStates = {0};

	margs.windowConfig = &windowConfig;
	margs.buttonsStates = &buttonsStates;
	margs.accelX = +2;
	margs.accelY = +2;

	windowConfig.width = 320;
	windowConfig.height = 240;
	memcpy( windowConfig.palette, palette, 32 );

	MultiSpacc_InitSystem();
	// TODO: integrate into LibMultiSpacc and make cross-platform to change into current directory
	//romfsInit();
	//chdir("romfs:/");

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
	margs.tilesImg = MultiSpacc_LoadImage( "./Assets/CHARS.png", margs.background, NULL );
	if( margs.tilesImg == NULL )
	{
		MultiSpacc_PrintDebug("[E] Error Loading tilesImg (%s).\n", TilesImgFile);
		return -1;
	}

	MultiSpacc_PrintText( "Hello, World!", margs.background, &windowConfig, 2, 2, margs.tilesImg );

	return MultiSpacc_SetMainLoop( MainLoop, NULL, &margs );
}

#include "../../LibMultiSpacc/MultiSpacc.h"

#define AppName "Pong"

bool paused = false;
Uint32 nextTick;

int ballX;
int ballY;
int accelX = 2;
int accelY = 2;

// the Y position of the paddles, measured from the top
int paddleSx;
int paddleDx;

MultiSpacc_SurfaceConfig windowConfig = {0};
MultiSpacc_Window *window;
MultiSpacc_Surface *screen;
MultiSpacc_Surface *background;
MultiSpacc_Surface *tilesImg;

#define BallSize 8

#define TileBall   128
#define TilePaddle 129

#define SpriteBall     0
#define SpritePaddleSx 1
#define SpritePaddleDx 1+4

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

bool MainLoop( void *args )
{
	if (!paused)
	{
		MultiSpacc_BlitLayer( background, screen );

		MultiSpacc_Sprite( SpriteBall, ballX, ballY, TileBall, tilesImg, screen );

		// TODO: metasprites

		MultiSpacc_Sprite( SpritePaddleSx  ,                        BallSize, paddleSx             , TilePaddle, tilesImg, screen );
		MultiSpacc_Sprite( SpritePaddleSx+1,                        BallSize, paddleSx +   BallSize, TilePaddle, tilesImg, screen );
		MultiSpacc_Sprite( SpritePaddleSx+2,                        BallSize, paddleSx + 2*BallSize, TilePaddle, tilesImg, screen );
		MultiSpacc_Sprite( SpritePaddleSx+3,                        BallSize, paddleSx + 3*BallSize, TilePaddle, tilesImg, screen );

		MultiSpacc_Sprite( SpritePaddleDx  , windowConfig.width - 2*BallSize, paddleDx             , TilePaddle, tilesImg, screen );
		MultiSpacc_Sprite( SpritePaddleDx+1, windowConfig.width - 2*BallSize, paddleDx +   BallSize, TilePaddle, tilesImg, screen );
		MultiSpacc_Sprite( SpritePaddleDx+2, windowConfig.width - 2*BallSize, paddleDx + 2*BallSize, TilePaddle, tilesImg, screen );
		MultiSpacc_Sprite( SpritePaddleDx+3, windowConfig.width - 2*BallSize, paddleDx + 3*BallSize, TilePaddle, tilesImg, screen );

		ballX += accelX;
		ballY += accelY;

		if( ballX == 0 || ballX == ( windowConfig.width - 8 ) )
		{
			accelX *= -1;
		}

		if( ballY == 0 || ballY == ( windowConfig.height - 8 ) )
		{
			accelY *= -1;
		}
	}

	/* TODO: listen for OS terminate signal */
	if( MultiSpacc_CheckKey( MultiSpacc_Key_Pause, 0 ) )
	{
		if (!paused) paused = true;
		else return false;
	}

	if( !MultiSpacc_WaitUpdateDisplay( window, &nextTick ) )
	{
		MultiSpacc_PrintDebug("[E] Error Updating Screen.\n");
		return false;
	}

	return true;
}

int main( int argc, char *argv[] )
{
	windowConfig.width = 320;
	windowConfig.height = 240;
	windowConfig.bits = 16;
	memcpy( windowConfig.palette, palette, 32 );

	window = MultiSpacc_SetWindow( &windowConfig );
	screen = MultiSpacc_GetWindowSurface( window );
	background = MultiSpacc_CreateSurface( &windowConfig );
	if( window == NULL || screen == NULL || background == NULL )
	{
		MultiSpacc_PrintDebug("[E] Error Initializing Video System.\n");
		return -1;
	};

	MultiSpacc_SetAppTitle( window, AppName );
	MultiSpacc_PrintDebug("[I] Ready!\n");

	// Bitmap font forked from: <https://github.com/nesdoug/01_Hello/blob/master/Alpha.chr>
	// Original copyright (c) 2018 Doug Fraker www.nesdoug.com (MIT)
	tilesImg = MultiSpacc_LoadImage( "../CHARS.png", screen, NULL );
	if( tilesImg == NULL )
	{
		return -1;
	}

	ballX = windowConfig.width/2;
	ballY = windowConfig.height/2;
	paddleSx = windowConfig.height/2 - 24;
	paddleDx = windowConfig.height/2 - 24;

	return MultiSpacc_SetMainLoop( MainLoop, NULL );
}

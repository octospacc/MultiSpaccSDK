#include "../../LibMultiSpacc/MultiSpacc.h"

#define AppName "Pong"

bool paused = false;
Uint32 nextTick;

int ballX;
int ballY;
int accelX = 2;
int accelY = 2;

int paddleSxY;
int paddleDxY;

MultiSpacc_SurfaceConfig windowConfig = {0};
MultiSpacc_Window *window;
MultiSpacc_Surface *screen;
MultiSpacc_Surface *background;
MultiSpacc_Surface *tilesImg;

#define BallSize 8
#define PaddleWidth 8
#define PaddleHeight 4

#define BallTile   128
#define PaddleTile 129

#define BallSprite     0
#define PaddleSxSprite 1
#define PaddleDxSprite 1 + PaddleHeight

#define PaddleSxX PaddleWidth
#define PaddleDxX windowConfig.width - 2*PaddleWidth

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

const unsigned char paddleMetaSprite[] = {
	0,  0, PaddleTile, 0,
	0,  8, PaddleTile, 0,
	0, 16, PaddleTile, 0,
	0, 24, PaddleTile, 0,
	128
};

bool MainLoop( void *args )
{
	MultiSpacc_SpritesMap msdata;

	int   chr[] = { 129, 129, 129, 129 };
	int     x[] = { 0, 0,  0,  0 };
	int     y[] = { 0, 8, 16, 24 };

	msdata.chr = chr;
	msdata.x = x;
	msdata.y = y;

	if (!paused)
	{
		MultiSpacc_BlitLayer( background, screen );

		MultiSpacc_SetSprite( BallSprite, ballX, ballY, BallTile, tilesImg, screen );

		MultiSpacc_SetMetaSprite( PaddleSxSprite, PaddleSxX, paddleSxY, &msdata, PaddleHeight, tilesImg, screen );
		MultiSpacc_SetMetaSprite( PaddleDxSprite, PaddleDxX, paddleDxY, &msdata, PaddleHeight, tilesImg, screen );

		//oam_meta_spr(                        PaddleWidth, paddleSxY,  4, paddleMetaSprite );
		//oam_meta_spr( windowConfig.width - 2*PaddleWidth, paddleDxY, 20, paddleMetaSprite );

		// MultiSpacc_Sprite( PaddleSxSprite  ,                        PaddleWidth, paddleSxY                , PaddleTile, tilesImg, screen );
		// MultiSpacc_Sprite( PaddleSxSprite+1,                        PaddleWidth, paddleSxY +   PaddleWidth, PaddleTile, tilesImg, screen );
		// MultiSpacc_Sprite( PaddleSxSprite+2,                        PaddleWidth, paddleSxY + 2*PaddleWidth, PaddleTile, tilesImg, screen );
		// MultiSpacc_Sprite( PaddleSxSprite+3,                        PaddleWidth, paddleSxY + 3*PaddleWidth, PaddleTile, tilesImg, screen );

		// MultiSpacc_Sprite( PaddleDxSprite  , windowConfig.width - 2*PaddleWidth, paddleDxY                , PaddleTile, tilesImg, screen );
		// MultiSpacc_Sprite( PaddleDxSprite+1, windowConfig.width - 2*PaddleWidth, paddleDxY +   PaddleWidth, PaddleTile, tilesImg, screen );
		// MultiSpacc_Sprite( PaddleDxSprite+2, windowConfig.width - 2*PaddleWidth, paddleDxY + 2*PaddleWidth, PaddleTile, tilesImg, screen );
		// MultiSpacc_Sprite( PaddleDxSprite+3, windowConfig.width - 2*PaddleWidth, paddleDxY + 3*PaddleWidth, PaddleTile, tilesImg, screen );

		ballX += accelX;
		ballY += accelY;

		if( ballX <= 0 || ballX >= (windowConfig.width - BallSize) )
		{
			accelX *= -1;
		}

		if( ballY <= 0 || ballY >= (windowConfig.height - BallSize) )
		{
			accelY *= -1;
		}

		#define TouchingPaddleSx ( ballX <= PaddleSxX+BallSize && ballY >= paddleSxY-BallSize && ballY <= (paddleSxY + 8*PaddleHeight) )
		#define TouchingPaddleDx ( ballX >= PaddleDxX-BallSize && ballY >= paddleDxY-BallSize && ballY <= (paddleDxY + 8*PaddleHeight) )
		if( TouchingPaddleSx || TouchingPaddleDx )
		{
			accelX *= -1;
		}
	}
	else
	{
		MultiSpacc_PrintText( "Pause", background, &windowConfig, 3, 3, tilesImg );
	}

	if( paddleSxY > 0 && MultiSpacc_CheckKey( MultiSpacc_Key_Up, 0 ) )
	{
		--paddleSxY;
	}
	else if( paddleSxY < (windowConfig.height - 8*PaddleHeight) && MultiSpacc_CheckKey( MultiSpacc_Key_Down, 0 ) )
	{
		++paddleSxY;
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
	paddleSxY = windowConfig.height/2 - 24;
	paddleDxY = windowConfig.height/2 - 24;

	return MultiSpacc_SetMainLoop( MainLoop, NULL );
}

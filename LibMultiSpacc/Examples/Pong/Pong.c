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

char paddleSxMove = 0;
char paddleDxMove = 0;

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

#define PaddleAccel 2
#define DeltaTime 1

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

MultiSpacc_SpritesMap msdata;

bool FixedUpdate( void *args )
{
	if(!paused)
	{
		ballX += accelX * DeltaTime;
		ballY += accelY * DeltaTime;

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

		if (paddleSxMove == 1)
		{
			paddleSxY -= PaddleAccel * DeltaTime;
			paddleSxMove = 0;
		}
		else if (paddleSxMove == 2)
		{
			paddleSxY += PaddleAccel * DeltaTime;
			paddleSxMove = 0;
		}
	}

	return true;
}

bool RealUpdate( void *args, double deltaTime )
{
	//MultiSpacc_WaitFrame(&nextTick);

	if(!paused)
	{
		MultiSpacc_BlitLayer( background, screen );
//printf("%d %f %f %d\n", ballX, deltaTime, accelX*deltaTime, ballX+accelX*deltaTime);
		MultiSpacc_SetSprite( BallSprite, ballX+accelX*deltaTime, ballY+accelY*deltaTime, BallTile, tilesImg, screen );

		#define PaddleSxYDisplay (paddleSxMove == 0 ? paddleSxY : paddleSxY+PaddleAccel*deltaTime)
		#define PaddleDxYDisplay (paddleDxMove == 0 ? paddleDxY : paddleDxY+PaddleAccel*deltaTime)
		MultiSpacc_SetMetaSprite( PaddleSxSprite, PaddleSxX, paddleSxY, &msdata, PaddleHeight, tilesImg, screen );
		MultiSpacc_SetMetaSprite( PaddleDxSprite, PaddleDxX, paddleDxY, &msdata, PaddleHeight, tilesImg, screen );
	}

	if( paddleSxY > 0 && MultiSpacc_CheckKey( MultiSpacc_Key_Up, 0 ) )
	{
		//paddleSxY -= PaddleAccel * DeltaTime;
		paddleSxMove = 1;
	}
	else if( paddleSxY < (windowConfig.height - 8*PaddleHeight) && MultiSpacc_CheckKey( MultiSpacc_Key_Down, 0 ) )
	{
		//paddleSxY += PaddleAccel * DeltaTime;
		paddleSxMove = 2;
	}

	// TODO: listen for OS terminate signal
	// TODO: fix SDL not waiting for key release with inputs checked this way
	if( MultiSpacc_CheckKey( MultiSpacc_Key_Pause, 0 ) )
	{
		if(!paused)
		{
			paused = true;
			MultiSpacc_PrintText( "Pause", background, &windowConfig, 3, 3, tilesImg );
		}
		else
		{
			MultiSpacc_PrintText( "Exit", background, &windowConfig, 3, 3, tilesImg );
			return false;
		}
	}

	//if( !MultiSpacc_WaitUpdateDisplay( window, &nextTick ) )
	if( !MultiSpacc_UpdateDisplay(window) )
	{
		MultiSpacc_PrintDebug("[E] Error Updating Screen.\n");
		return false;
	}

	return true;
}

/*
bool MainLoop( void *args )
{
	MultiSpacc_WaitFrame(&nextTick);

	if (!paused)
	{
		MultiSpacc_BlitLayer( background, screen );

		MultiSpacc_SetSprite( BallSprite, ballX, ballY, BallTile, tilesImg, screen );

		MultiSpacc_SetMetaSprite( PaddleSxSprite, PaddleSxX, paddleSxY, &msdata, PaddleHeight, tilesImg, screen );
		MultiSpacc_SetMetaSprite( PaddleDxSprite, PaddleDxX, paddleDxY, &msdata, PaddleHeight, tilesImg, screen );

		ballX += accelX * DeltaTime;
		ballY += accelY * DeltaTime;

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
		paddleSxY -= PaddleAccel * DeltaTime;
	}
	else if( paddleSxY < (windowConfig.height - 8*PaddleHeight) && MultiSpacc_CheckKey( MultiSpacc_Key_Down, 0 ) )
	{
		paddleSxY += PaddleAccel * DeltaTime;
	}

	// TODO: listen for OS terminate signal
	if( MultiSpacc_CheckKey( MultiSpacc_Key_Pause, 0 ) )
	{
		if (!paused) paused = true;
		else return false;
	}

	//if( !MultiSpacc_WaitUpdateDisplay( window, &nextTick ) )
	if( !MultiSpacc_UpdateDisplay(window) )
	{
		MultiSpacc_PrintDebug("[E] Error Updating Screen.\n");
		return false;
	}

	return true;
}
*/

int main( int argc, char *argv[] )
{
	int   chr[] = { 129, 129, 129, 129 };
	int     x[] = { 0, 0,  0,  0 };
	int     y[] = { 0, 8, 16, 24 };
	msdata.chr = chr;
	msdata.x = x;
	msdata.y = y;

	windowConfig.width = 256;
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

	//return MultiSpacc_SetMainLoop( MainLoop, NULL );
	return MultiSpacc_SetMainLoop( FixedUpdate, RealUpdate, &nextTick, NULL );
}

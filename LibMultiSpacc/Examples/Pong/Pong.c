#include "../../LibMultiSpacc/MultiSpacc.h"

#define AppName "Pong"

bool paused = false;
int scoreSx = 0;
int scoreDx = 0;

int ballX;
int ballY;
int accelX = 3;
int accelY = 3;

int paddleSxY;
int paddleDxY;
signed char paddleSxMove = 0;
signed char paddleDxMove = 0;

char scoreChar[6];

MultiSpacc_SurfaceConfig windowConfig = {0};
MultiSpacc_Window *window;
MultiSpacc_Surface *screen;
MultiSpacc_Surface *background;
MultiSpacc_Surface *tilesImg;

#define BallSize 8
#define PaddleWidth 8
#define PaddleHeightTiles 4
#define PaddleHeightPx 8*PaddleHeightTiles

#define BallTile   128
#define PaddleTile 129

#define BallSprite     0
#define PaddleSxSprite 1
#define PaddleDxSprite 1 + PaddleHeightTiles

#define PaddleSxX PaddleWidth
#define PaddleDxX windowConfig.width - 2*PaddleWidth

#define PaddleAccel 4

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

void ResetBall(void)
{
	ballX = windowConfig.width/2;
	ballY = windowConfig.height/2;
}

void UpdateBall(void)
{
	ballX += accelX;
	ballY += accelY;

	if( ballX <= 0-BallSize )
	{
		++scoreDx;
		ResetBall();
	}
	else if( ballX >= windowConfig.width )
	{
		++scoreSx;
		ResetBall();
	}

	#define IsTouchingPaddleSx ( ballX >= PaddleSxX-BallSize && ballX <= PaddleSxX+BallSize && ballY >= paddleSxY-BallSize && ballY <= paddleSxY+PaddleHeightPx )
	#define IsTouchingPaddleDx ( ballX >= PaddleDxX-BallSize && ballX <= PaddleDxX+BallSize && ballY >= paddleDxY-BallSize && ballY <= paddleDxY+PaddleHeightPx )

	if( IsTouchingPaddleSx || IsTouchingPaddleDx )
	{
		accelX *= -1;
		ballX += accelX;
		ballY += accelY;
	}

	// TODO: fix collision with upper borders of paddle, currently too aggressive and also broken? disabling everything makes the ball stick to paddles tho --- edit: with current setup it's kinda better but can still get stuck near the bottom/top of the screen and between paddle and side border, should fix (simply: don't allow paddle or ball near the very bottom of screen, put a mini-wall)
	if( ballY <= 0 || ballY >= (windowConfig.height - BallSize) || (IsTouchingPaddleSx && ballX >= PaddleSxX && ballX <= PaddleSxX+BallSize) || (IsTouchingPaddleDx && ballX >= PaddleDxX && ballX <= PaddleDxX+BallSize) )
	{
		accelY *= -1;
		ballX += accelX;
		ballY += accelY;
	}
}

void UpdatePlayer(void)
{
	// if (paddleSxMove == -1)
	// {
		// paddleSxY -= PaddleAccel;
		// paddleSxMove = 0;
	// }
	// else if (paddleSxMove == +1)
	// {
		// paddleSxY += PaddleAccel;
		// paddleSxMove = 0;
	// }
	paddleSxY += PaddleAccel*paddleSxMove;
	paddleSxMove = 0;
}

void UpdateCpuPlayer(void)
{
	#define PaddleDxYCenter paddleDxY+PaddleHeightPx/2
	if( accelX <= 0 )
	{
		if( paddleDxY < windowConfig.height/2 )
		{
			paddleDxY += PaddleAccel;
		}
		else if( paddleDxY > windowConfig.height/2 )
		{
			paddleDxY -= PaddleAccel;
		}
	}
	else if( rand() % PaddleAccel != 1 )
	{
		if ( PaddleDxYCenter < ballY )
		{
			paddleDxY += PaddleAccel;
		}
		else if ( PaddleDxYCenter > ballY )
		{
			paddleDxY -= PaddleAccel;
		}
	}
}

bool FixedUpdate( void *args )
{
	if(!paused)
	{
		UpdateBall();
		UpdatePlayer();
		UpdateCpuPlayer();
	}
	return true;
}

bool RealUpdate( void *args, double deltaTime )
{
	if(!paused)
	{
		//SDL_FillRect( background, &background->clip_rect, SDL_MapRGB( background->format, 0, 0, 0 ) );
		MultiSpacc_BlitLayer( background, screen );

		MultiSpacc_SetSprite( BallSprite, ballX+accelX*deltaTime, ballY+accelY*deltaTime, BallTile, tilesImg, screen );

		#define PaddleSxYDisplay (paddleSxY + PaddleAccel*paddleSxMove*deltaTime)
		#define PaddleDxYDisplay (paddleDxY + PaddleAccel*paddleDxMove*deltaTime)
		MultiSpacc_SetMetaSprite( PaddleSxSprite, PaddleSxX, PaddleSxYDisplay, &msdata, PaddleHeightTiles, tilesImg, screen );
		MultiSpacc_SetMetaSprite( PaddleDxSprite, PaddleDxX, PaddleDxYDisplay, &msdata, PaddleHeightTiles, tilesImg, screen );

		//itoa(scoreSx, scoreChar, 10);
		//MultiSpacc_PrintText( scoreChar, screen, &windowConfig, 1, 1, tilesImg );
		//itoa(scoreDx, scoreChar, 10);
		//MultiSpacc_PrintText( scoreChar, screen, &windowConfig, windowConfig.width/8-6, 1, tilesImg );

		if( paddleSxY > 0 && MultiSpacc_CheckKey( MultiSpacc_Key_Up, 0 ) )
		{
			paddleSxMove = -1;
		}
		else if( paddleSxY < windowConfig.height-PaddleHeightPx && MultiSpacc_CheckKey( MultiSpacc_Key_Down, 0 ) )
		{
			paddleSxMove = +1;
		}
	}

	// TODO: listen for OS terminate signal
	// TODO: fix SDL not waiting for key release with inputs checked this way
	// TODO: proper pause menu
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

	if( !MultiSpacc_UpdateDisplay(window) )
	{
		MultiSpacc_PrintDebug("[E] Error Updating Screen.\n");
		return false;
	}

	return true;
}

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

	ResetBall();
	paddleSxY = windowConfig.height/2 - 24;
	paddleDxY = windowConfig.height/2 - 24;

	return MultiSpacc_SetMainLoop( FixedUpdate, RealUpdate, NULL );
}

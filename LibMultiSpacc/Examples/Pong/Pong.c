#include "../../LibMultiSpacc/MultiSpacc.h"

#define AppName "Pong"

bool paused = false;
int scoreSx = 0;
int scoreDx = 0;

int ballX;
int ballY;

// NOTE: changing these breaks some logic (mainly AI) on different platforms, should be fixed
int accelX = 2;
int accelY = 3;

int paddleSxY;
int paddleDxY;
signed char paddleSxMove = 0;
signed char paddleDxMove = 0;

//char scoreChar[6];

MultiSpacc_SurfaceConfig windowConfig = {0};
MultiSpacc_Window *window;
MultiSpacc_Surface *screen;
MultiSpacc_Surface *background;
MultiSpacc_Surface *tilesImg;

MultiSpacc_SpritesMap paddleSpriteMap;
MultiSpacc_TilesMap divisorTileMap, borderTileMap;

#define BallSize 8
#define PaddleWidth 8
#define PaddleHeightTl 4
#define PaddleHeightPx 8*PaddleHeightTl

#define BallTile   128
#define PaddleTile 129
#define DivisorTile 130
#define BorderTile 131

#define BallSprite     0
#define PaddleSxSprite 1
#define PaddleDxSprite 1 + PaddleHeightTl

#define PaddleSxX PaddleWidth
#define PaddleDxX windowConfig.width - 2*PaddleWidth

#define PaddleAccel 4
#define PaddleMarginXPx 8
#define PaddleMarginYPx 8
#define ScreenMarginYPx 8

// TODO: more defines for frequently-used expressions

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

void ResetBall(void)
{
	ballX = windowConfig.width/2;
	ballY = windowConfig.height/2;
}

void AccelerateBall(void)
{
	ballX += accelX;
	ballY += accelY;
}

void UpdateBall(void)
{
	AccelerateBall();

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
		// idk what I did there but it's working to add some variety to the bounce angles
		if( accelX > 0 || abs( ballY - paddleSxY+PaddleHeightPx ) > PaddleHeightPx/4 )
		{
			++accelX;
		}
		else if( accelX < 0 || abs( ballY - paddleDxY+PaddleHeightPx ) > PaddleHeightPx/4 )
		{
			++accelY;
		}

		accelX *= -1;
		AccelerateBall();
	}

	#define IsTouchingFieldTop    ( ballY <=                   0+ScreenMarginYPx+PaddleMarginYPx          )
	#define IsTouchingFieldBottom ( ballY >= windowConfig.height-ScreenMarginYPx-PaddleMarginYPx-BallSize )
	#define IsTouchingPaddleAngle ( (IsTouchingPaddleSx && ballX >= PaddleSxX && ballX <= PaddleSxX+BallSize) || (IsTouchingPaddleDx && ballX >= PaddleDxX && ballX <= PaddleDxX+BallSize) )
	if( IsTouchingFieldTop || IsTouchingFieldBottom )
	{
		accelY *= -1;
		AccelerateBall();

		if( IsTouchingPaddleAngle )
		{
			AccelerateBall();
		}
	}
}

void UpdatePlayer(void)
{
	paddleSxY += PaddleAccel*paddleSxMove;
	paddleSxMove = 0;
}

// TODO: fix this, it breaks when all conditions mentioned in other places change, and it gets always worse (moves relatively too slow) the larger the screen is
void UpdateCpuPlayer(void)
{
	#define PaddleDxYCenter paddleDxY+PaddleHeightPx/2
	if( accelX <= 0 )
		// ball is going left, recenter CPU paddle
	{
		if( paddleDxY > windowConfig.height/2 )
		{
			paddleDxY -= PaddleAccel;
		}
		else if( paddleDxY < windowConfig.height/2 )
		{
			paddleDxY += PaddleAccel;
		}
	}
	else if( rand() % 3*PaddleAccel < 2*PaddleAccel )
		// if CPU is lucky, it can get close to the ball coming right (note: works good only with these values...)
	{
		if ( PaddleDxYCenter > ballY && paddleDxY > 0+ScreenMarginYPx+PaddleMarginYPx+BallSize )
		{
			paddleDxY -= PaddleAccel;
		}
		else if ( PaddleDxYCenter < ballY && paddleDxY < windowConfig.height-ScreenMarginYPx-PaddleMarginYPx-PaddleHeightPx-BallSize )
		{
			paddleDxY += PaddleAccel;
		}
	}
}

bool PollPlayerPaddle(void)
{
	if( paddleSxY > 0+ScreenMarginYPx+PaddleMarginYPx+BallSize && MultiSpacc_CheckKey( MultiSpacc_Key_Up, 0 ) )
	{
		paddleSxMove = -1;
	}
	else if( paddleSxY < windowConfig.height-ScreenMarginYPx-PaddleMarginYPx-PaddleHeightPx-BallSize && MultiSpacc_CheckKey( MultiSpacc_Key_Down, 0 ) )
	{
		paddleSxMove = +1;
	}
}

// TODO: the ball bouncing should be more varied, the AI should work better with different parameters
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

// TODO: flip needed sprites, must implement flags in MultiSpacc API first
// TODO: set metatile without cycle here
bool DisplayBorders(void)
{
	int i;

	for( i=1; i<(windowConfig.height/8 - 1); i++ )
	{
		MultiSpacc_SetTile( windowConfig.width/8/2    , i, DivisorTile, tilesImg, background );
		MultiSpacc_SetTile( windowConfig.width/8/2 - 1, i, DivisorTile, tilesImg, background );
	}

	for( i=0; i<windowConfig.width/8; i++ )
	{
		MultiSpacc_SetTile( i,                         1, BorderTile, tilesImg, background );
		MultiSpacc_SetTile( i, windowConfig.height/8 - 2, BorderTile, tilesImg, background );
	}
}

bool RealUpdate( void *args, double deltaTime )
{
	if(!paused)
	{
		//SDL_FillRect( background, &background->clip_rect, SDL_MapRGB( background->format, 0, 0, 0 ) );
		MultiSpacc_BlitLayer( background, screen );

		MultiSpacc_SetSprite( BallSprite, ballX+accelX*deltaTime, ballY+accelY*deltaTime, BallTile, tilesImg, screen );

		#define PaddleAccelDelta PaddleAccel*deltaTime
		#define PaddleSxYDisplay (paddleSxY + paddleSxMove*PaddleAccelDelta)
		#define PaddleDxYDisplay (paddleDxY + paddleDxMove*PaddleAccelDelta)
		MultiSpacc_SetMetaSprite( PaddleSxSprite, PaddleSxX, PaddleSxYDisplay, &paddleSpriteMap, PaddleHeightTl, tilesImg, screen );
		MultiSpacc_SetMetaSprite( PaddleDxSprite, PaddleDxX, PaddleDxYDisplay, &paddleSpriteMap, PaddleHeightTl, tilesImg, screen );

		//RefreshScore():
		//itoa(scoreSx, scoreChar, 10);
		//MultiSpacc_PrintText( scoreChar, screen, &windowConfig, 1, 1, tilesImg );
		//itoa(scoreDx, scoreChar, 10);
		//MultiSpacc_PrintText( scoreChar, screen, &windowConfig, windowConfig.width/8-6, 1, tilesImg );

		PollPlayerPaddle();
	}

	// TODO: listen for OS terminate signal
	// TODO: fix SDL not waiting for key release with inputs checked this way
	// TODO: proper pause menu?
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
	int i;
	int chr[] = { PaddleTile, PaddleTile, PaddleTile, PaddleTile };
	int   x[] = { 0, 0,  0,  0 };
	int   y[] = { 0, 8, 16, 24 };

	paddleSpriteMap.chr = chr;
	paddleSpriteMap.x = x;
	paddleSpriteMap.y = y;

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
	tilesImg = MultiSpacc_LoadImage( "./Assets/CHARS.png", screen, NULL );
	if( tilesImg == NULL )
	{
		return -1;
	}

	// divisorTileMap.chr = malloc(windowConfig.height/8 * sizeof(int*));
	// divisorTileMap.x = malloc(windowConfig.height/8 * sizeof(int*));
	// divisorTileMap.y = malloc(windowConfig.height/8 * sizeof(int*));
	// borderTileMap.chr = malloc(windowConfig.height/8 * sizeof(int*));
	// borderTileMap.x = malloc(windowConfig.height/8 * sizeof(int*));
	// borderTileMap.y = malloc(windowConfig.height/8 * sizeof(int*));

	for( i=0; i<windowConfig.height/8; i++ )
	{
		//divisorTileMap.chr[i] = DivisorTile;
		// divisorTileMap.x[i] = windowConfig.width/2;
		// divisorTileMap.y[i] = 0;
		// borderTileMap.chr[i] = BorderTile;
		// borderTileMap.x[i] = 0;
		// borderTileMap.y[i] = 8;
	}

	DisplayBorders();
	ResetBall();
	paddleSxY = windowConfig.height/2 - PaddleHeightPx;
	paddleDxY = windowConfig.height/2 - PaddleHeightPx;

	return MultiSpacc_SetMainLoop( FixedUpdate, RealUpdate, NULL );
}

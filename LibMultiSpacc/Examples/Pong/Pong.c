#include "../../LibMultiSpacc/MultiSpacc.h"

#define AppName "Pong"

#define iabs(x) (((x) >= 0) ? (x) : -(x))
#define maxOf2(a, b) (((a + b) / 2) + (iabs(a - b) / 2))

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

MultiSpacc_KeysStates buttonsStates;
MultiSpacc_SurfaceConfig windowConfig = {0};
MultiSpacc_Window *window;
MultiSpacc_Surface *screen;
MultiSpacc_Surface *background;
MultiSpacc_Surface *tilesImg;

MultiSpacc_SpritesMap paddleSxSpriteMap, paddleDxSpriteMap;
MultiSpacc_TilesMap divisorTileMap, borderTileMap, tempTileMap;

#define TileSize 8
#define BallSize TileSize
#define PaddleWidth TileSize
#define PaddleHeightTl 4
#define PaddleHeightPx TileSize*PaddleHeightTl

#define BallTile   128
#define PaddleTile 129
#define BorderTile 130
#define DivisorTileSx 131
#define DivisorTileDx 132

#define BallSprite     0
#define PaddleSxSprite 1
#define PaddleDxSprite 1 + PaddleHeightTl

#define PaddleSxX PaddleWidth
#define PaddleDxX windowConfig.width - 2*PaddleWidth

#define PaddleAccel 4
#define PaddleMarginXPx TileSize
#define PaddleMarginYPx TileSize
#define ScreenMarginYPx TileSize

#define StaticScreenMaxSidePx maxOf2(MultiSpacc_StaticScreenWidth, MultiSpacc_StaticScreenHeight)
#define StaticScreenMaxSideTl (StaticScreenMaxSidePx/TileSize)

#define RealScreenMaxSidePx maxOf2(windowConfig.width, windowConfig.height)
#define RealScreenMaxSideTl (RealScreenMaxSidePx/TileSize)

int tempX[StaticScreenMaxSideTl + MultiSpacc_StaticScreenSizeNull];
int tempY[StaticScreenMaxSideTl + MultiSpacc_StaticScreenSizeNull];
int tempChr[StaticScreenMaxSideTl + MultiSpacc_StaticScreenSizeNull];

// TODO: more defines for frequently-used expressions

const char palette[32] = { 
	0x1B,                // screen
	0x30,0x20,0x20,0x20, // background 0
	0x00,0x00,0x00,0x00, // background 1
	0x00,0x00,0x00,0x00, // background 2
	0x00,0x00,0x00,0x00, // background 3
	0x30,0x20,0x20,0x20, // sprite 0
	0x00,0x00,0x00,0x00, // sprite 1
	0x00,0x00,0x00,0x00, // sprite 2
	0x00,0x00,0x00,      // sprite 3
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
	if( paddleSxY > 0+ScreenMarginYPx+PaddleMarginYPx+BallSize && MultiSpacc_CheckKeyHold( MultiSpacc_Key_Up, &buttonsStates ) )
	{
		paddleSxMove = -1;
	}
	else if( paddleSxY < windowConfig.height-ScreenMarginYPx-PaddleMarginYPx-PaddleHeightPx-BallSize && MultiSpacc_CheckKeyHold( MultiSpacc_Key_Down, &buttonsStates ) )
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

bool DisplayBorders(void)
{
	int i;

	if( MultiSpacc_StaticScreenSizeNull )
	{
		tempTileMap.chr = malloc(RealScreenMaxSideTl * sizeof(int));
		tempTileMap.x = malloc(RealScreenMaxSideTl * sizeof(int));
		tempTileMap.y = malloc(RealScreenMaxSideTl * sizeof(int));
	}

	for( i=1; i<(windowConfig.height/TileSize - 1); i++ )
	{
		tempTileMap.chr[i] = DivisorTileSx;
		tempTileMap.x[i] = 0;
		tempTileMap.y[i] = i;
	}
	MultiSpacc_SetMetaTile( windowConfig.width/TileSize/2 - 1, 0, &tempTileMap, windowConfig.height/TileSize, tilesImg, background );

	for( i=1; i<(windowConfig.height/TileSize - 1); i++ )
	{
		tempTileMap.chr[i] = DivisorTileDx;
	}
	MultiSpacc_SetMetaTile( windowConfig.width/TileSize/2 - 0, 0, &tempTileMap, windowConfig.height/TileSize, tilesImg, background );

	for( i=0; i<windowConfig.width/TileSize; i++ )
	{
		tempTileMap.chr[i] = BorderTile;
		tempTileMap.x[i] = i;
		tempTileMap.y[i] = 0;
	}
	MultiSpacc_SetMetaTile( 0,                                1, &tempTileMap, windowConfig.width/TileSize, tilesImg, background );
	MultiSpacc_SetMetaTile( 0, windowConfig.height/TileSize - 2, &tempTileMap, windowConfig.width/TileSize, tilesImg, background );

	if( MultiSpacc_StaticScreenSizeNull )
	{
		free(tempTileMap.chr);
		free(tempTileMap.x);
		free(tempTileMap.y);
	}
}

bool RealUpdate( void *args, double deltaTime )
{
	MultiSpacc_PollButtons( 0, &buttonsStates );

	if(!paused)
	{
		//SDL_FillRect( background, &background->clip_rect, SDL_MapRGB( background->format, 0, 0, 0 ) );
		MultiSpacc_BlitLayer( background, screen );

		MultiSpacc_SetSprite( BallSprite, ballX+accelX*deltaTime, ballY+accelY*deltaTime, BallTile, NULL, tilesImg, screen );

		#define PaddleAccelDelta PaddleAccel*deltaTime
		#define PaddleSxYDisplay (paddleSxY + paddleSxMove*PaddleAccelDelta)
		#define PaddleDxYDisplay (paddleDxY + paddleDxMove*PaddleAccelDelta)
		MultiSpacc_SetMetaSprite( PaddleSxSprite, PaddleSxX, PaddleSxYDisplay, &paddleSxSpriteMap, PaddleHeightTl, tilesImg, screen );
		MultiSpacc_SetMetaSprite( PaddleDxSprite, PaddleDxX, PaddleDxYDisplay, &paddleDxSpriteMap, PaddleHeightTl, tilesImg, screen );

		//RefreshScore():
		//itoa(scoreSx, scoreChar, 10);
		//MultiSpacc_PrintText( scoreChar, screen, &windowConfig, 1, 1, tilesImg );
		//itoa(scoreDx, scoreChar, 10);
		//MultiSpacc_PrintText( scoreChar, screen, &windowConfig, windowConfig.width/8-6, 1, tilesImg );

		PollPlayerPaddle();
	}

	// TODO: listen for OS terminate signal
	// TODO: proper pause menu?
	if( MultiSpacc_CheckKeyPress( MultiSpacc_Key_Pause, &buttonsStates ) )
	{
		if(!paused)
		{
			paused = true;
			MultiSpacc_PrintText( "Pause", background, &windowConfig, 3, 3, tilesImg );
		}
		else
		{
			MultiSpacc_PrintText( "Exited.", background, &windowConfig, 3, 3, tilesImg );
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

	int x[PaddleHeightTl] = { 0, 0, 0, 0 };
	int y[PaddleHeightTl] = { 0, 8, 16, 24 };
	int chr[PaddleHeightTl] = { PaddleTile, PaddleTile, PaddleTile, PaddleTile };
	MultiSpacc_SpriteFlags flagsSx[PaddleHeightTl];
	MultiSpacc_SpriteFlags flagsDx[PaddleHeightTl];

	for( i=0; i<PaddleHeightTl; ++i ){
		flagsDx[i].flipHorizontal = true;
	}

	paddleSxSpriteMap.x = paddleDxSpriteMap.x = x;
	paddleSxSpriteMap.y = paddleDxSpriteMap.y = y;
	paddleSxSpriteMap.chr = paddleDxSpriteMap.chr = chr;
	paddleSxSpriteMap.flags = (MultiSpacc_SpriteFlags*)&flagsSx;
	paddleDxSpriteMap.flags = (MultiSpacc_SpriteFlags*)&flagsDx;

	tempTileMap.chr = tempChr;
	tempTileMap.x = tempX;
	tempTileMap.y = tempY;

	windowConfig.width = 320;
	windowConfig.height = 240;
	memcpy( windowConfig.palette, palette, 32 );

	MultiSpacc_InitSystem();
	window = MultiSpacc_SetWindow( &windowConfig );
	screen = MultiSpacc_GetWindowSurface( window );
	background = MultiSpacc_CreateSurface( &windowConfig );
	if( window == NULL || screen == NULL || background == NULL )
	{
		MultiSpacc_PrintDebug("[E] Error Initializing Video System.\n");
		return -1;
	};

	MultiSpacc_SetAppTitle( window, AppName );

	// Bitmap font forked from: <https://github.com/nesdoug/01_Hello/blob/master/Alpha.chr>
	// Original copyright (c) 2018 Doug Fraker www.nesdoug.com (MIT)
	tilesImg = MultiSpacc_LoadImage( "./Assets/CHARS.png", screen, NULL );
	if( tilesImg == NULL )
	{
		MultiSpacc_PrintDebug("[E] Error Loading tilesImg (%s).\n", TilesImgFile);
		return -1;
	}

	DisplayBorders();
	ResetBall();
	paddleSxY = windowConfig.height/2 - PaddleHeightPx;
	paddleDxY = windowConfig.height/2 - PaddleHeightPx;

	MultiSpacc_PrintDebug("[I] Ready!\n");
	return MultiSpacc_SetMainLoop( FixedUpdate, RealUpdate, NULL );
}

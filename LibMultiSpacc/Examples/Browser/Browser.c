#include "../../LibMultiSpacc/MultiSpacc.h"
#include <dirent.h>

#define AppName "Browser"
#define TilesImgFile "./Assets/CHARS.png"

#define TileSize 8
#define ScreenTilesH (windowConfig.width / TileSize)

MultiSpacc_KeysStates buttonsStates;
MultiSpacc_SurfaceConfig windowConfig = {0};
MultiSpacc_Window *window;
MultiSpacc_Surface *screen;
MultiSpacc_Surface *tilesImg;

struct dirent *de;
DIR *dr;

bool TryReadDirectory( char path[] )
{
	int i = 0;
	char previousPath[1024];
	char workingPath[1024];
	char displayPath[ScreenTilesH+1];

	memset( displayPath, 0, ScreenTilesH+1 );

	if( dr != NULL ){
		closedir(dr);
	}

	if( (dr = opendir(path)) != NULL ){
		strcpy( previousPath, path );
	}
	else{
		dr = opendir(previousPath);
	}

	if( strlen(getcwd( workingPath, sizeof(workingPath) )) > ScreenTilesH )
	{
		strncpy( displayPath, "...", 3 );
		i = 3;
	}

	// TODO: fix bug, if path is too short we have parts of it to the right
	while( workingPath[i] )
	{
		if( strlen(workingPath) - ScreenTilesH <= i-3 )
		{
			strcat( displayPath, &workingPath[i] );
		}
		++i;
	}

	SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0, 0, 0 ) );
	MultiSpacc_PrintText( displayPath, screen, &windowConfig, 0, 0, tilesImg );

	return true;
}

bool RealUpdate( void *args, double deltaTime )
{
	int entriesCount = 0;

	MultiSpacc_PollButtons( 0, &buttonsStates );

	// TODO: show cursor, allow moving it in list and to select entries
	if( MultiSpacc_CheckKeyPress( MultiSpacc_Key_Confirm, &buttonsStates ) )
	{
		chdir("..");
		TryReadDirectory(".");
	}
	else if( MultiSpacc_CheckKeyPress( MultiSpacc_Key_Up, &buttonsStates ) )
	{
		
	}
	else if( MultiSpacc_CheckKeyPress( MultiSpacc_Key_Down, &buttonsStates ) )
	{
		
	}
	// TODO: listen for OS terminate signal
	else if( MultiSpacc_CheckKeyPress( MultiSpacc_Key_Pause, &buttonsStates ) )
	{
		return false;
	}

	de = readdir(dr); // "delete" first entry (".", meaning current directory) from list
    while ( (de = readdir(dr)) != NULL ) 
	{
		MultiSpacc_PrintText( de->d_name, screen, &windowConfig, 2, 2+i, tilesImg );
		++i;
	}
	// TODO: handle directories with more entries than screen height

	if( !MultiSpacc_UpdateDisplay(window) )
	{
		MultiSpacc_PrintDebug("[E] Error Updating Screen.\n");
		return false;
	}

	return true;
}

int main( int argc, char *argv[] )
{
	if( !MultiSpacc_InitSystem() )
	{
		MultiSpacc_PrintDebug("[E] Error Initializing System Core.\n");
		return -1;
	}

	window = MultiSpacc_SetWindow( &windowConfig );
	screen = MultiSpacc_GetWindowSurface( window );
	if( window == NULL || screen == NULL )
	{
		MultiSpacc_PrintDebug("[E] Error Initializing Video System.\n");
		return -1;
	};

	// Bitmap font forked from: <https://github.com/nesdoug/01_Hello/blob/master/Alpha.chr>
	// Original copyright (c) 2018 Doug Fraker www.nesdoug.com (MIT)
	tilesImg = MultiSpacc_LoadImage( TilesImgFile, screen, NULL );
	if( tilesImg == NULL )
	{
		MultiSpacc_PrintDebug("[E] Error Loading tilesImg (%s).\n", TilesImgFile);
		return -1;
	}

	TryReadDirectory(".");
	MultiSpacc_SetAppTitle( window, AppName );
	MultiSpacc_PrintDebug("[I] Ready!\n");
	return MultiSpacc_SetMainLoop( NULL, RealUpdate, NULL );
}

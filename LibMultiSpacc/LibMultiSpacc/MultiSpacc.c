#include "./MultiSpacc.h"

#ifdef MultiSpacc_Target_SDL12
	MultiSpacc_Window *MultiSpacc_SetWindow( MultiSpacc_SurfaceConfig WindowConfig ) {
		return SDL_SetVideoMode( WindowConfig.Width, WindowConfig.Height, WindowConfig.Bits, WindowConfig.Flags );
	}
	MultiSpacc_Surface *MultiSpacc_GetWindowSurface( MultiSpacc_Window *Window ) {
		return Window;
	}

	void MultiSpacc_SetAppTitle( MultiSpacc_Window *Window, const char *Title ) {
		SDL_WM_SetCaption( Title, NULL );
	}
	void MultiSpacc_SetAppIcon( MultiSpacc_Window *Window, MultiSpacc_Surface *Icon ) {
		SDL_WM_SetIcon( Icon, NULL );
	}

	int MultiSpacc_SetColorKey( MultiSpacc_Surface *Surface, bool Flag, Uint32 Key ) {
		if( Flag )
			return SDL_SetColorKey( Surface, SDL_SRCCOLORKEY, Key );
		else
			return SDL_SetColorKey( Surface, 0, Key );
	}
#endif

#ifdef MultiSpacc_Target_SDL20
	MultiSpacc_Window *MultiSpacc_SetWindow( MultiSpacc_SurfaceConfig WindowConfig ) {
		return SDL_CreateWindow( NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WindowConfig.Width, WindowConfig.Height, WindowConfig.Flags );
	}
	MultiSpacc_Surface *MultiSpacc_GetWindowSurface( MultiSpacc_Window *Window ) {
		return SDL_GetWindowSurface( Window );
	}

	void MultiSpacc_SetAppTitle( MultiSpacc_Window *Window, const char Title[] ) {
		SDL_SetWindowTitle( Window, Title );
	}
	void MultiSpacc_SetAppIcon( MultiSpacc_Window *Window, MultiSpacc_Surface *Icon ) {
		SDL_SetWindowIcon( Window, Icon );
	}

	int MultiSpacc_SetColorKey( MultiSpacc_Surface *Surface, bool Flag, Uint32 Key ) {
		if ( Flag ) {
			return SDL_SetColorKey( Surface, SDL_TRUE, Key );
		} else {
			return SDL_SetColorKey( Surface, SDL_FALSE, Key );
		}
	}
#endif

#ifdef MultiSpacc_Target_SDLCom
	int MultiSpacc_PollEvent( MultiSpacc_Event *Event )
	{
		SDL_Event FromEvent;
		int Result = SDL_PollEvent( &FromEvent );
		*Event = (MultiSpacc_Event) {
			.Type = FromEvent.type,
			.Key = FromEvent.key.keysym.sym,
		};
		return Result;
	}

	MultiSpacc_Surface *MultiSpacc_LoadImage( char FilePath[], MultiSpacc_Surface *Screen, Uint32 *ColorKey )
	{
		MultiSpacc_Surface *Final = NULL;
		MultiSpacc_Surface *Raw = IMG_Load( FilePath );
		if( Raw == NULL ) {
			printf("[E] Error reading image %s.\n", FilePath);
		} else {
			Final = SDL_ConvertSurface( Raw, Screen->format, 0 );
			SDL_FreeSurface( Raw );
			if( Final == NULL ) {
				printf("[E] Error adapting image %s.\n", FilePath);
			} else {
				Uint32 FinalColorKey = SDL_MapRGB( Final->format, 0xFF, 0x00, 0xFF ); // Magenta
				if( ColorKey != NULL )
					FinalColorKey = *ColorKey;
				MultiSpacc_SetColorKey( Final, true, FinalColorKey );
			};
		};
		return Final;
	}

	void MultiSpacc_PrintText( char Text[], MultiSpacc_Surface *Surface, MultiSpacc_SurfaceConfig WindowConfig, int x, int y, MultiSpacc_Surface *Tiles /*, int FontSize, int Color */ )
	{
		for( int i = 0; i < strlen( Text ); i++ )
		{
			MultiSpacc_Rect Offset = {
				.x = (x * 8) + (8 * i),
				.y = (y * 8),
			};
			MultiSpacc_Rect Clip = {
				.x = 8 * (Text[i] % 16),
				.y = 8 * (Text[i] / 16),
				.w = 8,
				.h = 8,
			};
			SDL_BlitSurface( Tiles, &Clip, Surface, &Offset );
		};
	}

	void MultiSpacc_PrintDebug( const char *format, ... )
	{
		va_list args;
		va_start(args, format);
		fprintf(stderr, format, args);
		va_end(args);
	}
#endif

#ifdef MultiSpacc_Target_NDS
	MultiSpacc_Window *MultiSpacc_SetWindow( MultiSpacc_SurfaceConfig WindowConfig )
	{
		PrintConsole *bottomScreen = NULL;

		videoSetModeSub(MODE_0_2D);
		vramSetBankC(VRAM_C_SUB_BG);

		bottomScreen = consoleInit(bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
		consoleSelect(bottomScreen);
		return bottomScreen;
	}

	MultiSpacc_Surface *MultiSpacc_GetWindowSurface( MultiSpacc_Window *Window )
	{
		return Window;
	}

	void MultiSpacc_SetAppTitle( MultiSpacc_Window *Window, const char Title[] ){}
	void MultiSpacc_PrintDebug( const char *format, ... ){}

	int MultiSpacc_UpdateWindowSurface( MultiSpacc_Window *Window )
	{
		return 0;
	}

	void MultiSpacc_PrintText( char Text[], MultiSpacc_Surface *Surface, MultiSpacc_SurfaceConfig WindowConfig, int x, int y, MultiSpacc_Surface *Tiles /*, int FontSize, int Color */ )
	{
		iprintf(Text);
	}

	void MultiSpacc_Sleep( int milliseconds )
	{
		int frames = (60 * milliseconds / 1000);
		for (int i=0; i<frames; i++)
		{
			swiWaitForVBlank();
		};
	}
	
	MultiSpacc_Surface *MultiSpacc_LoadImage( char FilePath[], MultiSpacc_Surface *Screen, Uint32 *ColorKey )
	{
		return NULL;
	};
#endif

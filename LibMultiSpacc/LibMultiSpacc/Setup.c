#include "./MultiSpacc.h"

bool MultiSpacc_SetMainLoop( bool functionFixedUpdate( void *args ), bool functionRealUpdate( void *args, double deltaTime ), void *args )
{
	MultiSpacc_MainLoopHandlerArgs handlerArgs;

	handlerArgs.functionFixedUpdate = functionFixedUpdate;
	handlerArgs.functionRealUpdate = functionRealUpdate;
	handlerArgs.args = args;
	handlerArgs.elapsedRealTime = 0;
	handlerArgs.elapsedFixedTime = 0;

	#ifdef MultiSpacc_Target_Web
		emscripten_set_main_loop_arg( (em_arg_callback_func)MultiSpacc_MainLoopHandler, &handlerArgs, -1, true );
	#else
		while(true){
			if ( !MultiSpacc_MainLoopHandler(&handlerArgs) ){
				return false;
			}
		}
	#endif
	return true;
}

MultiSpacc_Window *MultiSpacc_SetWindow( MultiSpacc_SurfaceConfig *windowConfig )
{
	#if defined(MultiSpacc_Target_SDLCommon)
		if( windowConfig->width <= 0 || windowConfig->height <= 0 ){
			#if defined(MultiSpacc_Target_Switch)
				windowConfig->width = 1280;
				windowConfig->height = 720;
			#else
				windowConfig->width = 512;
				windowConfig->height = 480;
			#endif
		}

		if( windowConfig->bits <= 0 ){
			windowConfig->bits = 32;
		}

		#if defined(MultiSpacc_Target_SDL12)
			return SDL_SetVideoMode( windowConfig->width, windowConfig->height, windowConfig->bits, windowConfig->flags );
		#elif defined(MultiSpacc_Target_SDL20)
			return SDL_CreateWindow( NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowConfig->width, windowConfig->height, windowConfig->flags );
		#endif

	#elif defined(MultiSpacc_Target_NDS)
		PrintConsole *bottomScreen = NULL;

		videoSetModeSub(MODE_0_2D);
		vramSetBankC(VRAM_C_SUB_BG);

		bottomScreen = consoleInit(bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
		consoleSelect(bottomScreen);
		return bottomScreen;

	#elif defined(MultiSpacc_Target_NES)
		windowConfig->width = 256;
		windowConfig->height = 240;
		oam_clear();
		pal_all(windowConfig->palette);
		ppu_on_all();

	#endif
}

void MultiSpacc_SetAppTitle( MultiSpacc_Window *Window, const char *Title )
{
	#if defined(MultiSpacc_Target_SDL12)
		SDL_WM_SetCaption( Title, NULL );
	#elif defined(MultiSpacc_Target_SDL20)
		SDL_SetWindowTitle( Window, Title );
	#endif
}

void MultiSpacc_SetAppIcon( MultiSpacc_Window *Window, MultiSpacc_Surface *Icon )
{
	#if defined(MultiSpacc_Target_SDL12)
		SDL_WM_SetIcon( Icon, NULL );
	#elif defined(MultiSpacc_Target_SDL20)
		SDL_SetWindowIcon( Window, Icon );
	#endif
}

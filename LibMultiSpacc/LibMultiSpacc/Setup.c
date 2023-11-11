#include "./MultiSpacc.h"

bool MultiSpacc_SetMainLoop( bool functionFixedUpdate( void *args ), bool functionRealUpdate( void *args, double deltaTime ), Uint32 *nextTick, void *args )
{
	MultiSpacc_MainLoopHandlerArgs handlerArgs;

	handlerArgs.functionFixedUpdate = functionFixedUpdate;
	handlerArgs.functionRealUpdate = functionRealUpdate;
	handlerArgs.args = args;
	//handlerArgs.nextTick = nextTick;
	//handlerArgs.ticksLast = 0;
	handlerArgs.elapsedRealTime = 0;
	handlerArgs.elapsedFixedTime = 0;

	#ifdef MultiSpacc_Target_Web
		emscripten_set_main_loop_arg( (em_arg_callback_func)MultiSpacc_MainLoopHandler, &handlerArgs, -1, true );
	#else
		while(true){
			//MultiSpacc_WaitFrame(nextTick);
			//if( !functionUpdate(args) || !functionFixedUpdate(args) ){
			//	return false;
			//}
			if ( !MultiSpacc_MainLoopHandler(&handlerArgs) ){
				return false;
			}
		}
	#endif
	return true;
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

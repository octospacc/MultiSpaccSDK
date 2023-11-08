#include "./MultiSpacc.h"

bool MultiSpacc_SetMainLoop( bool function( void *args ), void *args )
{
	#ifdef MultiSpacc_Target_Web
		emscripten_set_main_loop_arg( (em_arg_callback_func)function, args, -1, true );
	#else
		while(true){
			if( !function(args) ){
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

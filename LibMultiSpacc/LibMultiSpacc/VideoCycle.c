#include "./MultiSpacc.h"

// todo: frame-indepentent movements with delta time
void MultiSpacc_WaitFrame( Uint32 *nextTick )
{
	#if defined(MultiSpacc_Target_SDLStandard)
		Uint32 now = SDL_GetTicks();
		if ( *nextTick <= now ) {
			MultiSpacc_Sleep(0);
		} else {
			MultiSpacc_Sleep( *nextTick - now );
		}
		//*nextTick += 1000/MultiSpacc_GameTick;
		*nextTick = SDL_GetTicks() + 1000/MultiSpacc_GameTick;
	#elif defined(MultiSpacc_Target_NDS)
		swiWaitForVBlank();
	#elif defined(MultiSpacc_Target_NES)
		ppu_wait_frame();
		//ppu_wait_nmi();
	#endif
}

bool MultiSpacc_UpdateDisplay( MultiSpacc_Window *window )
{
	#if defined(MultiSpacc_Target_SDL12)
		return !SDL_Flip(window);
	#elif defined(MultiSpacc_Target_SDL20)
		return !SDL_UpdateWindowSurface(window);
	#else
		return true;
	#endif
}

bool MultiSpacc_WaitUpdateDisplay( MultiSpacc_Window *window, Uint32 *nextTick )
{
	if( !MultiSpacc_UpdateDisplay(window) ){
		return false;
	}
	MultiSpacc_WaitFrame(nextTick);
	return true;
}

// #if defined(MultiSpacc_Target_NES)
	// bool MultiSpacc_WaitUpdateDisplay_NES(void)
	// {
		// MultiSpacc_UpdateDisplay(NULL);
		// MultiSpacc_WaitFrame(NULL);
		// return true;
	// }
// #endif

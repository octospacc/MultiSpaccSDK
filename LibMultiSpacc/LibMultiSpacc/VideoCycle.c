#include "./MultiSpacc.h"

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

bool MultiSpacc_WaitFrame( Uint32 *nextTick )
{
	#if defined(MultiSpacc_Target_SDLStandard)
		// TODO: check if this actually works with any framerate or it caps the game speed
		Uint32 now;
		now = SDL_GetTicks();
		if ( *nextTick <= now ) {
			MultiSpacc_Sleep(0);
		} else {
			MultiSpacc_Sleep( *nextTick - now );
		}
		*nextTick += 1000/60; // TODO: framerate specified from app and variable framerate
		*nextTick = SDL_GetTicks() + 1000/60; // this last part should execute at the start of a loop maybe ???
	#elif defined(MultiSpacc_Target_NDS)
		swiWaitForVBlank();
	#elif defined(MultiSpacc_Target_NES)
		ppu_wait_frame();
	#endif
	return true;
}

bool MultiSpacc_WaitUpdateDisplay( MultiSpacc_Window *window, Uint32 *nextTick )
{
	if( !MultiSpacc_UpdateDisplay(window) || !MultiSpacc_WaitFrame(nextTick) ){
		return false;
	}
	return true;
}

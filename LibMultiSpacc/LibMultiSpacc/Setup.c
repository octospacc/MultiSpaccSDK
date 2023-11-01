#include "./MultiSpacc.h"

MultiSpacc_Window *MultiSpacc_SetWindow( MultiSpacc_SurfaceConfig *WindowConfig )
{
	#ifdef MultiSpacc_Target_SDL12
		return SDL_SetVideoMode( WindowConfig->Width, WindowConfig->Height, WindowConfig->Bits, WindowConfig->Flags );
	#endif

	#ifdef MultiSpacc_Target_SDL20
		return SDL_CreateWindow( NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WindowConfig->Width, WindowConfig->Height, WindowConfig->Flags );
	#endif

	#ifdef MultiSpacc_Target_NDS
		PrintConsole *bottomScreen = NULL;

		videoSetModeSub(MODE_0_2D);
		vramSetBankC(VRAM_C_SUB_BG);

		bottomScreen = consoleInit(bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
		consoleSelect(bottomScreen);
		return bottomScreen;
	#endif

	#ifdef MultiSpacc_Target_NES
		oam_clear();
		pal_all(WindowConfig->Palette);
		ppu_on_all();
	#endif
}

MultiSpacc_Surface *MultiSpacc_GetWindowSurface( MultiSpacc_Window *Window )
{
	#ifdef MultiSpacc_Target_SDL12
		return Window;
	#endif
	#ifdef MultiSpacc_Target_SDL20
		return SDL_GetWindowSurface(Window);
	#endif
	#ifdef MultiSpacc_Target_NDS
		return Window;
	#endif
}

void MultiSpacc_SetAppTitle( MultiSpacc_Window *Window, const char *Title )
{
	#ifdef MultiSpacc_Target_SDL12
		SDL_WM_SetCaption( Title, NULL );
	#endif
	#ifdef MultiSpacc_Target_SDL20
		SDL_SetWindowTitle( Window, Title );
	#endif
}

void MultiSpacc_SetAppIcon( MultiSpacc_Window *Window, MultiSpacc_Surface *Icon )
{
	#ifdef MultiSpacc_Target_SDL12
		SDL_WM_SetIcon( Icon, NULL );
	#endif
	#ifdef MultiSpacc_Target_SDL20
		SDL_SetWindowIcon( Window, Icon );
	#endif
}

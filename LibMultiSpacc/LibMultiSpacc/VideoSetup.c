#include "./MultiSpacc.h"

MultiSpacc_Window *MultiSpacc_SetWindow( MultiSpacc_SurfaceConfig *windowConfig )
{
	#if defined(MultiSpacc_Target_SDL12)
		return SDL_SetVideoMode( windowConfig->width, windowConfig->height, windowConfig->bits, windowConfig->flags );

	#elif defined(MultiSpacc_Target_SDL20)
		return SDL_CreateWindow( NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowConfig->width, windowConfig->height, windowConfig->flags );

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

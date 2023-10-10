#include "../MultiSpacc.h"

MultiSpacc_Window MultiSpacc_SetWindow( MultiSpacc_SurfaceConfig WindowConfig )
{
	//PrintConsole topScreen;
	PrintConsole bottomScreen;
	//PrintConsole *bottomScreenPtr;

	//videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	//vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);

	//consoleInit(&topScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
	//bottomScreenPtr = consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);

	//consoleSelect(&bottomScreen);
	//return *bottomScreenPtr;
	
	
	PrintConsole *bottomScreenPtr = consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	bottomScreen = *bottomScreenPtr;
	consoleSelect(&bottomScreen);
	return bottomScreen;
}

MultiSpacc_Surface *MultiSpacc_GetWindowSurface( MultiSpacc_Window *Window )
{
	consoleSelect(Window);
	return Window;
}

void MultiSpacc_SetAppTitle( MultiSpacc_Window *Window, const char Title[] ){}

int MultiSpacc_UpdateWindowSurface( MultiSpacc_Window *Window )
{
	return 0;
}

void MultiSpacc_PrintText( char Text[], MultiSpacc_Surface *Surface, MultiSpacc_SurfaceConfig WindowConfig, int x, int y, MultiSpacc_Surface *Tiles /*, int FontSize, int Color */ )
{
	iprintf(Text);
}
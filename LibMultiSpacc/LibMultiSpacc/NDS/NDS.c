#include "../MultiSpacc.h"

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
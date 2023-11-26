#include "../../LibMultiSpacc/MultiSpacc.h"

#define AppName "Clock"



int main( int argc, char *argv[] )
{
	// ... do setup things
	
	if( !MultiSpacc_SystemClockAvailable )
	{
		// ... make the user set time in-app
	}

	// ... retrieve system time by common API or keep a clock manually
	// ... display and update a clock
}

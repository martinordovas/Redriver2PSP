#include <stdio.h>
#include "PSPPlatform.h"

extern "C" {
void psp_platform_init(void)
{
	// Video / graphics initialization via PSP GU / gl4es
	pspDebugScreenPrintf("PSP Platform initialized.\n");
}
}

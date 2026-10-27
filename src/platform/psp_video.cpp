#include <stdio.h>
#include "PSPPlatform.h"
#include "PsyX/PsyX_public.h"

extern "C" {
void psp_platform_init(void)
{
	PsyX_Initialise((char*)"Redriver2 PSP", 480, 272, 1);
}
}

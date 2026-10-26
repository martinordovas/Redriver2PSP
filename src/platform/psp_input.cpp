#include <pspctrl.h>
#include "PSPPlatform.h"

extern "C" {
void psp_poll_controls(void)
{
	SceCtrlData pad;
	sceCtrlReadBufferPositive(&pad, 1);
}
}

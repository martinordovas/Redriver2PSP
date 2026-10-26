#include <pspaudio.h>
#include <pspaudiolib.h>
#include "PSPPlatform.h"

extern "C" {
void psp_audio_init(void)
{
	pspAudioInit();
}

void psp_audio_shutdown(void)
{
	pspAudioEnd();
}
}

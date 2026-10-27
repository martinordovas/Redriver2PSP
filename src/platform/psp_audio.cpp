#include <pspaudio.h>
#include <pspaudiolib.h>
#include "PSPPlatform.h"

extern "C" {
void psp_audio_init(void)
{
	// OpenAL handles audio directly via sceAudio
}

void psp_audio_shutdown(void)
{
}
}

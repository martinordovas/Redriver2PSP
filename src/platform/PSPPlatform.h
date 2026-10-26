#ifndef PSP_PLATFORM_H
#define PSP_PLATFORM_H

#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspaudio.h>
#include <pspaudiolib.h>

#ifdef __cplusplus
extern "C" {
#endif

void psp_platform_init(void);
void psp_audio_init(void);
void psp_audio_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif // PSP_PLATFORM_H

#ifndef PSP_SDL_MESSAGEBOX_H
#define PSP_SDL_MESSAGEBOX_H

#if defined(PSP) || defined(__PSP__)
#include <pspkernel.h>
#include <pspdebug.h>
#include <stdio.h>

#ifndef SDL_MESSAGEBOX_INFORMATION
#define SDL_MESSAGEBOX_INFORMATION 0x00000001
#endif
#ifndef SDL_MESSAGEBOX_ERROR
#define SDL_MESSAGEBOX_ERROR 0x00000010
#endif

static inline int psp_show_messagebox(int flags, const char* title, const char* message) {
    printf("[SDL Box - %s] %s\n", title ? title : "Message", message ? message : "");
    fflush(stdout);
    if (flags & SDL_MESSAGEBOX_ERROR) {
        pspDebugScreenInit();
        pspDebugScreenPrintf("\n--- REDRIVER2 PSP ERROR ---\n%s:\n%s\n", title ? title : "Error", message ? message : "");
        sceKernelDelayThread(8000000);
    }
    return 0;
}

#undef SDL_ShowSimpleMessageBox
#define SDL_ShowSimpleMessageBox(flags, title, message, window) psp_show_messagebox((int)(flags), (title), (message))

#endif

#endif // PSP_SDL_MESSAGEBOX_H

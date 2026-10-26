#ifndef SDL_MESSAGEBOX_H
#define SDL_MESSAGEBOX_H

#if defined(PSP) || defined(__PSP__)
#include <SDL.h>
#include <SDL_messagebox.h>
#include <stdio.h>

#ifndef SDL_MESSAGEBOX_INFORMATION
#define SDL_MESSAGEBOX_INFORMATION 0x00000001
#endif
#ifndef SDL_MESSAGEBOX_ERROR
#define SDL_MESSAGEBOX_ERROR 0x00000010
#endif

#ifndef SDL_ShowSimpleMessageBox
inline int SDL_ShowSimpleMessageBox(int flags, const char* title, const char* message, void* window) {
    printf("[SDL Box - %s] %s\n", title ? title : "Message", message ? message : "");
    return 0;
}
#endif
#endif

#endif // SDL_MESSAGEBOX_H

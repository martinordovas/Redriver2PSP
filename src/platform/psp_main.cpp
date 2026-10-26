#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "PSPPlatform.h"

PSP_MODULE_INFO("Redriver2PSP", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

// Exit callback
int exit_callback(int arg1, int arg2, void *common)
{
	sceKernelExitGame();
	return 0;
}

int CallbackThread(SceSize args, void *argp)
{
	int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

int SetupCallbacks(void)
{
	int thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, 0, 0);
	if (thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}
	return thid;
}

extern "C" {
int redriver2_main(int argc, char** argv);
}

void SwitchMappings(int menu) {}

extern "C" int main(int argc, char** argv)
{
	SetupCallbacks();

	pspDebugScreenInit();
	pspDebugScreenPrintf("Redriver2 PSP Port starting...\n");

	// Initialize PSP controls
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	// Initialize platform subsystems
	psp_platform_init();
	psp_audio_init();

	// Run main game
	char* game_argv[] = { (char*)"redriver2_psp", NULL };
	int game_argc = 1;

	redriver2_main(game_argc, game_argv);

	return 0;
}

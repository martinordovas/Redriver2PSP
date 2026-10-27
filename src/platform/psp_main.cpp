#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "PSPPlatform.h"
#include "driver2.h"
#include "C/system.h"

extern volatile char* _overlay_buffer;
extern volatile char* _frontend_buffer;
extern volatile char* _other_buffer;
extern volatile char* _other_buffer2;
extern volatile OTTYPE* _OT1;
extern volatile OTTYPE* _OT2;
extern volatile char* _primTab1;
extern volatile char* _primTab2;
extern volatile char* _sbank_buffer;
extern volatile char* _replay_buffer;

static char g_Overlay_buffer[0x50000];
static char g_Frontend_buffer[0x60000];
static char g_Other_buffer[0x50000];
static char g_Other_buffer2[0x50000];
static OTTYPE g_OT1[OTSIZE];
static OTTYPE g_OT2[OTSIZE];
static char g_PrimTab1[PRIMTAB_SIZE];
static char g_PrimTab2[PRIMTAB_SIZE];
static char g_SBank_buffer[0x50000];
static char g_Replay_buffer[0x50000];

static void psp_init_memory_buffers(void)
{
	_overlay_buffer = g_Overlay_buffer;
	_frontend_buffer = g_Frontend_buffer;
	_other_buffer = g_Other_buffer;
	_other_buffer2 = g_Other_buffer2;
	_OT1 = g_OT1;
	_OT2 = g_OT2;
	_primTab1 = g_PrimTab1;
	_primTab2 = g_PrimTab2;
	_sbank_buffer = g_SBank_buffer;
	_replay_buffer = g_Replay_buffer;
}

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
void PsyX_CDFS_Init(const char* imageFileName, int track, int sectorSize);
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
	psp_init_memory_buffers();

	// Initialize CD filesystem image reader
	const char* cdPaths[] = {
		"data/install/Driver2CD1.bin",
		"install/Driver2CD1.bin",
		"Driver2CD1.bin",
		NULL
	};
	const char* selectedCdPath = NULL;
	for (int i = 0; cdPaths[i] != NULL; i++)
	{
		FILE* f = fopen(cdPaths[i], "rb");
		if (f)
		{
			fclose(f);
			selectedCdPath = cdPaths[i];
			break;
		}
	}

	if (selectedCdPath)
	{
		PsyX_CDFS_Init(selectedCdPath, 0, 0);
	}
	else
	{
		// Default to standard path
		PsyX_CDFS_Init("data/install/Driver2CD1.bin", 0, 0);
	}

	// Run main game
	char* game_argv[] = { (char*)"redriver2_psp", (char*)"-nofmv", NULL };
	int game_argc = 2;

	int ret = redriver2_main(game_argc, game_argv);
	if (ret != 0 && ret != 1)
	{
		pspDebugScreenInit();
		pspDebugScreenPrintf("\nredriver2_main returned error code: %d\n", ret);
		pspDebugScreenPrintf("Please check logs or assets.\n");
		sceKernelDelayThread(8000000);
	}

	return 0;
}

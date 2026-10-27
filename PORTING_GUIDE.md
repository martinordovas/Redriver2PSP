# Complete Porting Guide: Redriver2 (PC Version) to PlayStation Portable (PSP)

This document details the analysis of the **REDRIVER2** repository and establishes the step-by-step strategy, architecture, and project structure for porting it to **PSP** under **`REDRIVER2PSP/Redriver2PSP/`**.

---

## 1. Base Repository Analysis (`REDRIVER2`)

The `REDRIVER2` repository contains the complete C/C++ reimplementation of **Driver 2 (PSX)** obtained via high-precision reverse engineering (translation of MIPS assembly to C without CPU emulation).

### Key Components:
1. **`src_rebuild/Game/`**: Contains all game logic decoupled from the original platform:
   - Rendering engine and cells (`engine/`, `cell.c`, `mdl.h`, etc.).
   - Artificial intelligence and physics (`C/ai.c`, `C/handling.c`, `C/bcollide.c`, `C/cop_ai.c`, etc.).
   - User interface and menus (`Frontend/FEmain.c`).
   - Mission, cutscene, and sound management.
2. **`src_rebuild/PsyCross/` (PsyX)**: Abstraction layer emulating PSX BIOS calls, GTE (Geometry Transformation Engine), and GPU, translating PSX primitives to modern graphics APIs (OpenGL / OpenGL ES).
3. **Existing Platform Layers**: Windows (SDL2 + OpenGL), Linux, Emscripten (Web), and Android.

---

## 2. Technical Challenges of the PSP Port

1. **Hardware & Performance**:
   - **CPU**: MIPS R4000 (Allegrex) at 233/333 MHz.
   - **RAM**: 32 MB (FAT) and 64 MB (Slim). Efficient memory management is crucial for open-world streaming.
2. **Graphics System (PsyCross + PSP GU)**:
   - PSP uses hardware graphics **PSP GU (`sceGu`)**.
   - Adaptation of PsyCross/rendering backend to translate PSX rendering commands.
3. **Audio**:
   - Replacing OpenAL (`snd_al_source.cpp`) with native PSP audio (`pspaudiolib` / `sceAudio`).
4. **Input**:
   - Mapping SDL2 input to PSP controls via `sceCtrlReadBufferPositive` (`pspCtrlData`).
5. **File System (`sceIo`)**:
   - Adapting I/O (`fs.cpp`) for PSP Memory Stick access (`ms0:/`).

---

## 3. Step-by-Step Execution Plan

### Phase 1: Development Environment Setup (PSP Toolchain)
1. Install **PSPSDK** and pspdev toolchain (`psp-gcc`, `psp-g++`, `psp-config`).

### Phase 2: PSP Platform Layer (`platform/`)
1. **`psp_main.cpp`**: Entry point, thread callbacks, lifecycle initialization.
2. **`psp_video.cpp`**: `sceGu` graphics backend.
3. **`psp_input.cpp`**: `sceCtrl` gamepad mapping.
4. **`psp_audio.cpp`**: `pspaudio` sound backend.

### Phase 3: PsyCross & Compilation Configuration
1. Compile with MIPS cross-compilation tools.
2. Configure PSPSDK `Makefile` with `-O3 -G0` flags and PSP library links (`-lpspgu -lpspgum -lpspaudiolib -lpspaudio`).

### Phase 4: Packaging and Asset Preparation
1. Place extracted PSX Driver 2 game assets inside `data/DRIVER2/`.
2. Generate **`EBOOT.PBP`**.
3. Test via PPSSPP or real hardware deployment.

---

## 4. Compilation vs. Disc Metadata (Assets) Explanation

### How `EBOOT.PBP` Compiles Without Disc Metadata:
- **Compilation Stage**: Compiling source code (`.c`, `.cpp`) into `redriver2_psp.elf` and packaging it into `EBOOT.PBP` only requires the **source code files** and the **development SDK libraries** (PSPSDK, SDL2, OpenAL, pspGU, etc.). 
- **Disc Metadata / Assets**: Game assets (such as `DATA`, `GFX`, `LANG`, `REPLAYS`, `XA` extracted from the PlayStation Driver 2 CD images) are **data files required at runtime**, not at compile time. 
- **Runtime Requirement**: To run the compiled `EBOOT.PBP` on an emulator (PPSSPP) or physical PSP, the extracted game assets must be placed in the `data/` folder (or `ms0:/PSP/GAME/Redriver2PSP/data/`) on the Memory Stick so the game engine can stream levels, textures, and audio when launched.

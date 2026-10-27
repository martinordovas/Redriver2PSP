# PSP Port Roadmap: Redriver2 (PC Version)

This document details the roadmap and architectural separation between pure reusable logic and the platform-specific layer to port **Redriver2** to **PlayStation Portable (PSP)** in the `REDRIVER2PSP/Redriver2PSP/` directory.

---

## 1. Detailed Architecture Map

### Block A: Pure Code & Logic (100% Reusable)
*Source code of the reverse-engineered PC engine (`REDRIVER2/src_rebuild/`). Game logic requires no rewriting.*

1. **`src/Game/C/` (Game Logic, Physics & AI)**
   - Physics and vehicle handling (`handling.c`, `car.c`, `physics.c`).
   - Collision detection (`bcollide.c`, `colliding.c`).
   - AI for enemies, police, and traffic (`ai.c`, `cop_ai.c`, `ped.c`).
   - Mission and game management (`mission.c`, `game.c`, etc.).
2. **`src/Game/engine/` (Rendering Engine & Cells)**
   - Streaming and loading open-world map cells (`cell.c`).
   - 3D models and geometry (`mdl.h`).
3. **`src/Game/Frontend/` (User Interface)**
   - Main menus, loading screens, and HUD (`FEmain.c`, `FEscreens.inc`).
4. **`src/Game/` (Structures & Base Math)**
   - `driver2.h`, `dr2types.h`, `dr2math.h`, `dr2limits.h`, `dr2locale.c`.
5. **`src/PsyCross/` (PSX Hardware Emulation / GTE)**
   - Abstraction layer and PSX geometric transformation mathematics (GTE).

---

### Block B: Platform & Adaptation Layer (PSP Specific Design & Development)
*Modules implemented to connect the engine with PSP hardware and APIs via **PSPSDK**.*

1. **Entry Point & Lifecycle (`src/platform/psp_main.cpp`)**
   - PSP SDK initialization (`pspDebugScreenInit`, HOME exit callbacks).
   - Main execution loop.
2. **Graphics System & Rendering Backend (`src/platform/psp_video.cpp` + PsyCross)**
   - **PSP GU** initialization (`sceGuInit`, VRAM buffers, double buffering).
   - Translation of graphic primitives to PSP geometry operations (`sceGum`, `sceGuDrawArray`).
3. **Control / Input System (`src/platform/psp_input.cpp`)**
   - Reading PSP pad via `sceCtrlReadBufferPositive`.
   - Mapping analog and digital buttons to Driver 2 actions.
4. **Sound System (`src/platform/psp_audio.cpp`)**
   - Native audio subsystem initialization via **`pspaudiolib`** / `sceAudio`.
   - Feeding PCM buffers for sound effects and music.
5. **File System & I/O (`src/utils/fs.cpp`)**
   - File path adaptation for PSP Memory Stick access (`ms0:/PSP/GAME/Redriver2PSP/data/` or relative paths).
6. **Build System & Metadata (`Makefile` and `sce_sys/`)**
   - **`Makefile`**: Configured with PSPSDK, `-O3 -G0` optimizations, and library linking (`-lpspgu -lpspgum -lpspaudiolib -lpspaudio -lm -lc -lstdc++`).
   - **`sce_sys/`**: XMB menu metadata (`PARAM.SFO`, `ICON0.PNG`, `PIC0.PNG`).

---

### Block C: Asset Management & Disc Images
* **Data Requirement**: Extracted and converted PlayStation disc files (Disc 1 and Disc 2) using REDRIVER2 conversion tools (`_convert_cd_fmv_xa.sh` and `jpsxdec`) are required to populate the `data/DRIVER2/` directory (`DATA`, `GFX`, `LANG`, `REPLAYS`, `XA`).

---

## 2. Work Plan (Phases)

- [x] **Phase 1: Code Synchronization & Base Structure**
  - Directory creation and file copying from `REDRIVER2/src_rebuild/` to `src/`.
- [x] **Phase 2: PSP Platform Layer Implementation**
  - Development of `PSPPlatform.h`, `psp_main.cpp`, `psp_video.cpp`, `psp_input.cpp`, and `psp_audio.cpp`.
- [x] **Phase 3: Makefile Configuration & Cross-Compilation**
  - Creation of PSPSDK `Makefile` and verification of `EBOOT.PBP` compilation.
- [ ] **Phase 4: Packaging, Assets & Testing**
  - Resource integration (`data/`), emulator testing (PPSSPP), and validation on real hardware.

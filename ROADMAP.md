# Roadmap del Port a PSP: Redriver2 (Versión PC)

Este documento detalla el mapa de ruta y la separación arquitectónica entre la lógica pura reutilizable y la capa de plataforma específica para realizar el port de **Redriver2** a **PlayStation Portable (PSP)** en el directorio `REDRIVER2PSP/Redriver2PSP/`.

---

## 1. Mapa Detallado de Arquitectura

### Bloque A: Puro Código y Lógica (100% Reutilizable)
*Código fuente del motor reverse-engineered de PC (`REDRIVER2/src_rebuild/`). No requiere reescribir la lógica de juego.*

1. **`src/Game/C/` (Lógica de Juego, Físicas e IA)**
   - Físicas y manejo vehicular (`handling.c`, `car.c`, `physics.c`).
   - Detección de colisiones (`bcollide.c`, `colliding.c`).
   - Inteligencia artificial de enemigos, policías y tráfico (`ai.c`, `cop_ai.c`, `ped.c`).
   - Gestión de misiones y juego (`mission.c`, `game.c`, etc.).
2. **`src/Game/engine/` (Motor de Renderizado y Celdas)**
   - Streaming y carga de celdas del mundo abierto (`cell.c`).
   - Modelos 3D y geometría (`mdl.h`).
3. **`src/Game/Frontend/` (Interfaz de Usuario)**
   - Menús principales, pantallas de carga y HUD (`FEmain.c`, `FEscreens.inc`).
4. **`src/Game/` (Estructuras y Matemáticas Base)**
   - `driver2.h`, `dr2types.h`, `dr2math.h`, `dr2limits.h`, `dr2locale.c`.
5. **`src/PsyCross/` (Emulación de Hardware PSX / GTE)**
   - Capa de abstracción y matemáticas de transformación geométrica de PSX (GTE).

---

### Bloque B: Capa de Plataforma y Adaptación (Diseño y Desarrollo Específico para PSP)
*Módulos que deben implementarse para conectar el motor con el hardware y APIs de la PSP vía **PSPSDK**.*

1. **Punto de Entrada y Ciclo de Vida (`src/platform/psp_main.cpp`)**
   - Inicialización del SDK de PSP (`pspDebugScreenInit`, callbacks de salida por HOME).
   - Bucle principal de ejecución.
2. **Sistema Gráfico y Backend de Renderizado (`src/platform/psp_video.cpp` + PsyCross)**
   - Inicialización de **PSP GU** (`sceGuInit`, buffers en VRAM, doble búfer).
   - Traducción de primitivas gráficas a operaciones geométricas de la PSP (`sceGum`, `sceGuDrawArray`).
3. **Sistema de Control / Entrada (`src/platform/psp_input.cpp`)**
   - Lectura del pad de PSP mediante `sceCtrlReadBufferPositive`.
   - Mapeo de botones analógicos y digitales a las acciones de Driver 2.
4. **Sistema de Sonido (`src/platform/psp_audio.cpp`)**
   - Inicialización del subsistema de audio nativo mediante **`pspaudiolib`** / `sceAudio`.
   - Envío de buffers PCM para efectos de sonido y música.
5. **Sistema de Archivos y E/S (`src/utils/fs.cpp`)**
   - Adaptación de rutas de archivos para acceso a la Memory Stick de la PSP (`ms0:/PSP/GAME/Redriver2PSP/data/` o rutas relativas).
6. **Sistema de Compilación y Metadatos (`Makefile` y `sce_sys/`)**
   - **`Makefile`**: Configurado con PSPSDK, optimizaciones `-O3 -G0`, y enlace de librerías (`-lpspgu -lpspgum -lpspaudio -lm -lc -lstdc++`).
   - **`sce_sys/`**: Metadatos del menú XMB (`PARAM.SFO`, `ICON0.PNG`, `PIC0.PNG`).

---

### Bloque C: Gestión de Activos y Imágenes de Disco (Assets)
* **Requisito de Datos**: Se requieren los archivos extraídos y convertidos de los discos de PlayStation (Disc 1 y Disc 2) mediante las herramientas de conversión de REDRIVER2 (`_convert_cd_fmv_xa.sh` y `jpsxdec`) para poblar la carpeta `data/DRIVER2/` (`DATA`, `GFX`, `LANG`, `REPLAYS`, `XA`).

---

## 2. Plan de Trabajo (Fases)

- [ ] **Fase 1: Sincronización de Código y Estructura Base**
  - Creación de directorios y copia de ficheros desde `REDRIVER2/src_rebuild/` a `src/`.
- [ ] **Fase 2: Implementación de la Capa de Plataforma PSP**
  - Desarrollo de `PSPPlatform.h`, `psp_main.cpp`, `psp_video.cpp`, `psp_input.cpp` y `psp_audio.cpp`.
- [ ] **Fase 3: Configuración del Makefile y Compilación Cruzada**
  - Creación del `Makefile` con PSPSDK y verificación de compilación del `EBOOT.PBP`.
- [ ] **Fase 4: Empaquetado, Activos y Pruebas**
  - Integración de recursos (`data/`), pruebas en emulador (PPSSPP) y validación en hardware real.

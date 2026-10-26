# Guía Completa de Portabilidad: Redriver2 (Versión PC) a PlayStation Portable (PSP)

Este documento detalla el análisis del repositorio **REDRIVER2** y establece la estrategia paso a paso, junto con la esquemática y la estructura del proyecto para realizar el port completo a **PSP** bajo la ruta **`REDRIVER2PSP/Redriver2PSP/`**.

---

## 1. Análisis del Repositorio Base (`REDRIVER2`)

El repositorio `REDRIVER2` contiene la reimplementación completa en C/C++ de **Driver 2 (PSX)** obtenida mediante ingeniería inversa de alta precisión (traducción de ensamblador MIPS a C sin emulación de CPU).

### Componentes Clave:
1. **`src_rebuild/Game/`**: Contiene toda la lógica del juego desacoplada de la plataforma original:
   - Motor de renderizado y celdas (`engine/`, `cell.c`, `mdl.h`, etc.).
   - Inteligencia artificial y físicas (`C/ai.c`, `C/handling.c`, `C/bcollide.c`, `C/cop_ai.c`, etc.).
   - Interfaz de usuario y menús (`Frontend/FEmain.c`).
   - Gestión de misiones, cinemáticas y sonidos.
2. **`src_rebuild/PsyCross/` (PsyX)**: La capa de abstracción que emula las llamadas de la BIOS de PSX, el GTE (Geometry Transformation Engine) y la GPU. Traduce las primitivas de PSX a APIs gráficas modernas (OpenGL / OpenGL ES).
3. **Capas de Plataforma Existentes**: El proyecto ya soporta Windows (SDL2 + OpenGL), Linux, Emscripten (Web) y Android, lo que demuestra que su arquitectura modular permite adaptarlo a nuevas plataformas mediante una **Capa de Plataforma (`platform/`)**.

---

## 2. Retos Técnicos del Port a PSP

Llevar Redriver2 a la PlayStation Portable (PSP) implica superar las siguientes limitaciones y desafíos de hardware:

1. **Hardware y Rendimiento**:
   - **CPU**: Procesador MIPS R4000 (Allegrex) a 233/333 MHz. Curiosamente, la arquitectura MIPS es familiar a la del PSX (R3000), pero la PSP requiere un código altamente optimizado y compilado con flags estrictas (`-O3`, `-G0`).
   - **Memoria RAM**: 32 MB en modelos FAT (PSP-1000) y 64 MB en modelos SLIM/GO (PSP-2000 en adelante). Driver 2 realiza streaming masivo de mundos abiertos. Será crucial gestionar la memoria de manera eficiente y evitar fugas o saturación de RAM.
2. **Sistema Gráfico (PsyCross + PSP GU)**:
   - La PSP no cuenta con soporte nativo para OpenGL completo, sino con su propio motor gráfico por hardware: **PSP GU (`sceGu`)**.
   - **Estrategia**: Adaptar PsyCross para que utilice un backend de renderizado basado en `sceGu`/`sceGum` (o integrar la biblioteca `gl4es` para traducir OpenGL ES 1.1 a PSP GU). La opción de integrar un backend directo o `gl4es` debe evaluarse por rendimiento de las tablas de ordenamiento (OT) de PSX.
3. **Audio**:
   - Reemplazar las llamadas a OpenAL (`snd_al_source.cpp`) por la API de audio nativa de PSP (`sceAudio` / `pspaudiolib`) para efectos de sonido y música reproducida en streaming.
4. **Entrada (Controles)**:
   - Mapear el sistema de entrada de SDL2 al sistema de control de PSP mediante `sceCtrlReadBufferPositive` (`pspCtrlData`), aprovechando el stick analógico y los botones frontales y de hombro (L/R).
5. **Sistema de Archivos (`sceIo`)**:
   - Adaptar las funciones de E/S (`fs.cpp`) para utilizar las funciones de acceso a la Memory Stick (`ms0:/`) de la PSP a través de las librerías de PSPSDK.

---

## 3. Pasos a Seguir para el Port Completo

### Fase 1: Configuración del Entorno de Desarrollo (Toolchain PSP)
1. Instalar el kit de desarrollo oficial de código abierto para PSP (**PSPSDK**).
2. Configurar las variables de entorno (`PSPSDK`, `PSPDEV`), asegurando que `psp-gcc`, `psp-g++` y `make` estén disponibles en el PATH.

### Fase 2: Creación de la Capa de Plataforma PSP (`platform/PSP/`)
1. **`psp_main.cpp`**: Punto de entrada estándar para aplicaciones PSP (incluyendo bucle principal y manejo del ciclo de vida de la aplicación con `sceKernelExitGame`).
2. **`psp_video.cpp`**: Inicialización de `sceGu` y gestión de buffers de pantalla (Double Buffering) para PsyCross.
3. **`psp_input.cpp`**: Lectura del pad analógico y digital de la PSP y traducción a las estructuras de entrada de Redriver2.
4. **`psp_audio.cpp`**: Inicialización de canales de audio mediante `pspaudiolib` y carga de buffers de audio PCM/WAV/XM.

### Fase 3: Adaptación de PsyCross para PSP
1. Modificar o añadir las definiciones de compatibilidad en PsyCross para compilar con herramientas de compilación cruzada MIPS de PSP.
2. Implementar o enlazar el renderizador gráfico para que las llamadas del GTE/GPU de PSX se traduzcan eficientemente a operaciones de geometría de la PSP (`sceGumDrawArray`, etc.) o mediante la integración de `gl4es`.

### Fase 4: Configuración del Sistema de Compilación (`Makefile`)
1. Crear un `Makefile` compatible con `pspsdk` que compile todos los archivos `.c` y `.cpp` de `Game/`, `PsyCross/`, `utils/` y `platform/PSP/`.
2. Especificar las librerías de enlaces obligatorias de PSP:
   ```makefile
   LIBS = -lpspgu -lpspgum -lpspaudio -lpthread -lm -lc
   ```

### Fase 5: Empaquetado y Generación del EBOOT
1. Crear la carpeta `sce_sys/` con el archivo de metadatos `PARAM.SFO`.
2. Generar el ejecutable empaquetado **`EBOOT.PBP`** utilizando la herramienta `pack-pbp` o `mksfoex`.
3. Preparar la estructura de datos del juego (`data/`), asegurando rutas relativas correctas para la carga de texturas y niveles desde la Memory Stick de la PSP (`ms0:/PSP/GAME/Redriver2PSP/data/`).

### Fase 6: Pruebas y Depuración
1. Realizar pruebas iniciales en emuladores optimizados para PSP como **PPSSPP**.
2. Realizar pruebas en hardware real (PSP FAT / Slim con Custom Firmware) para verificar rendimiento, uso de memoria RAM y estabilidad del streaming de mundos.

---

## 4. Esquemática y Estructura del Proyecto en `REDRIVER2PSP/`

La estructura organizada del proyecto dentro de `/home/mordovas/REDRIVER2PSP/Redriver2PSP/` se plantea de la siguiente manera:

```text
REDRIVER2PSP/
└── Redriver2PSP/
    ├── README.md               # Descripción general del proyecto PSP
    ├── PORTING_GUIDE.md        # Esta guía técnica detallada
    ├── Makefile                # Makefile principal para PSPSDK
    ├── sce_sys/                # Metadatos para el XMB de PSP
    │   ├── PARAM.SFO           # Configuración de título, versión y permisos del EBOOT
    │   ├── ICON0.PNG           # Icono principal del juego en el menú de PSP (144x80)
    │   └── PIC0.PNG            # Imagen de fondo en el menú de PSP (480x272)
    ├── src/                    # Código fuente unificado
    │   ├── Game/               # Núcleo del juego (portado desde REDRIVER2/src_rebuild/Game/)
    │   │   ├── engine/         # Motor gráfico, celdas y modelos (cell.c, mdl.h, etc.)
    │   │   ├── C/              # Lógica de juego, físicas, IA, colisiones (ai.c, handling.c, bcollide.c, etc.)
    │   │   ├── Frontend/       # Menús y pantallas de inicio (FEmain.c, FEscreens.inc)
    │   │   ├── driver2.h       # Definiciones globales y estructuras principales
    │   │   ├── dr2types.h      # Tipos de datos
    │   │   ├── dr2math.h       # Matemáticas y vectores
    │   │   └── ...             # Resto de archivos .h / .c / .cpp
    │   ├── PsyCross/           # Capa PsyX adaptada para renderizado PSX en PSP
    │   │   ├── include/        # Cabeceras públicas de GTE y GPU
    │   │   └── ...             # Implementación de emulación de hardware PSX
    │   ├── utils/              # Utilidades de soporte (fs.cpp, ini.c, targa.cpp, etc.)
    │   └── platform/           # Capa de plataforma específica para PSP
    │       ├── PSPPlatform.h   # Cabeceras de plataforma PSP
    │       ├── psp_main.cpp    # Función main() e inicialización del sistema
    │       ├── psp_video.cpp   # Inicialización de pantalla y backend gráfico GU
    │       ├── psp_input.cpp   # Mapeo de mandos y lectura de botones/analógico
    │       └── psp_audio.cpp   # Sistema de sonido utilizando pspaudiolib / sceAudio
    └── data/                   # Recursos y archivos de datos de Driver 2 (niveles, texturas, música)
```

---

## 5. Próximos Pasos Recomendados

1. Inicializar el repositorio de trabajo en `/home/mordovas/REDRIVER2PSP/Redriver2PSP/`.
2. Sincronizar y adaptar los archivos fuente desde `REDRIVER2/src_rebuild/`.
3. Escribir los módulos de la plataforma PSP (`platform/PSP/`).
4. Configurar el `Makefile` con PSPSDK y compilar la primera versión de prueba (`EBOOT.PBP`).

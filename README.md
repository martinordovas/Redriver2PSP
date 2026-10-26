# Redriver2PSP

Port completo de **Redriver2** (la versión de PC basada en ingeniería inversa de Driver 2 de PSX) a la **PlayStation Portable (PSP)**.

Consulta la **[Guía de Portabilidad (PORTING_GUIDE.md)](PORTING_GUIDE.md)** para ver el análisis detallado del repositorio base (`REDRIVER2`), los retos técnicos, los pasos a seguir y la esquemática de la estructura del proyecto.

## Estructura del Proyecto
- `src/`: Código fuente del juego (`Game/`, `PsyCross/`, `utils/` y `platform/PSP/`).
- `sce_sys/`: Metadatos de la aplicación PSP (`PARAM.SFO`, iconos).
- `data/`: Recursos del juego (`Driver 2`).
- `Makefile`: Sistema de compilación para PSPSDK.
